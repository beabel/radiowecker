void DrawFooterButtons_Power_Sleep_Alarm(void);

/* Stream-/Codec per malloc beim ersten startUrl — nicht in setup nach LVGL (sonst ~100 KiB Heap → LwIP/Wetter). */
const int preallocateBufferSize = 40 * 1024;
const int preallocateCodecSize = 29192;  // MP3-Decoder (ESP8266Audio)
static_assert(preallocateCodecSize >= AudioGeneratorMP3::preAllocSize(),
              "preallocateCodecSize < AudioGeneratorMP3::preAllocSize()");

void *preallocateBuffer = NULL;
void *preallocateCodec = NULL;

// Instanzen für Audio-Komponenten
AudioGenerator *decoder = NULL;         // MP3-Decoder
AudioFileSourceICYStream *file = NULL;  // Eingabemodul für ICY-Streams aus dem Web
AudioFileSourceBuffer *buff = NULL;     // Puffer für den Eingabestream
AudioOutputI2S *out = NULL;  // Ausgabemodul I2S

// Stream-/Codec-Puffer erst bei erster Wiedergabe (startUrl) — spart ~70 KiB Heap für WiFi/WebServer
// (WebServer::begin nutzt new; bei Heap-Fragmentierung/OOM: StoreProhibited an 0x0).
static int s_effective_buffer_size = preallocateBufferSize;

static bool ensure_audio_prealloc(void) {
  if (preallocateBuffer && preallocateCodec) return true;

  auto try_alloc = [&](int stream_sz) -> bool {
    if (preallocateBuffer) {
      free(preallocateBuffer);
      preallocateBuffer = NULL;
    }
    if (preallocateCodec) {
      free(preallocateCodec);
      preallocateCodec = NULL;
    }
    s_effective_buffer_size = stream_sz;
    preallocateBuffer = malloc((size_t)stream_sz);
    preallocateCodec = malloc((size_t)preallocateCodecSize);
    if (preallocateBuffer && preallocateCodec) return true;
    if (preallocateBuffer) {
      free(preallocateBuffer);
      preallocateBuffer = NULL;
    }
    if (preallocateCodec) {
      free(preallocateCodec);
      preallocateCodec = NULL;
    }
    return false;
  };

  /* Kleinste zuerst: ICY open() braucht danach noch ~tens of KB für Socket/TCP — bei 32+29 KiB Puffer
   * war nur noch ~43 KiB frei → open() scheitert trotzdem. */
  static const int kStreamTry[] = {
    12 * 1024, 16 * 1024, 24 * 1024, 32 * 1024, preallocateBufferSize,
  };
  for (size_t i = 0; i < sizeof(kStreamTry) / sizeof(kStreamTry[0]); i++) {
    if (try_alloc(kStreamTry[i])) {
      if (kStreamTry[i] < preallocateBufferSize) {
        RADIO_SERIAL(Serial.printf_P(PSTR("Audio: Stream-Puffer %d B (kleiner = mehr Heap für TCP)\n"), kStreamTry[i]));
      }
      return true;
    }
  }
  RADIO_SERIAL(Serial.printf_P(PSTR("WARNING: Audio-Puffer malloc fehlgeschlagen. freeHeap=%u\n"), (unsigned)ESP.getFreeHeap()));
  return false;
}

/* ESP32 + ESP8266Audio ≥2.4: AudioOutputI2S::begin() ruft immer i2s_new_channel() — es gibt kein
 * „bereits an“. AudioGeneratorMP3::begin() ruft danach selbst output->begin(). Darf also nur einmal
 * vor dem Decoder passieren: nach decoder->stop() nur Kanal freigeben, kein begin() hier. */
static void audio_i2s_release_after_pipeline_stop(void) {
  if (!out) return;
#if defined(ESP32)
  out->stop();
#endif
}

#if !defined(ESP32)
static bool audio_ensure_i2s_on_esp8266_style(void) {
  if (!out) return false;
  out->SetChannels(2);
  out->SetRate(44100);
  if (!out->begin()) {
    RADIO_SERIAL(Serial.println(F("Audio: I2S begin fehlgeschlagen (DMA/Heap?)")));
    return false;
  }
  return true;
}
#endif

// Funktion für die gesamte erforderliche Einrichtung
void setup_audio() {
  out = new AudioOutputI2S(0, AudioOutputI2S::EXTERNAL_I2S, 4, AudioOutputI2S::APLL_DISABLE);
  out->SetPinout(BCLK, LRCLK, DOUT);
  out->SetOutputModeMono(true);
#if defined(ESP32)
  /* I2S erst bei decoder->begin() — sonst doppeltes begin() beim ersten startUrl (Assert). */
#else
  out->SetChannels(2);
  out->SetRate(44100);
  if (!out->begin()) {
    RADIO_SERIAL(Serial.println(F("Audio: I2S begin fehlgeschlagen (DMA/Heap?)")));
  }
#endif
}

/* Mehrere decoder->loop()-Aufrufe pro loop()-Durchlauf: sonst verhungert die Pipeline
 * hinter WebServer + LVGL (Ruckler). 16 ≈ Kompromiss WDT vs. Durchsatz. */
#ifndef AUDIO_LOOPS_PER_FRAME
#define AUDIO_LOOPS_PER_FRAME 16
#endif

/* ESP8266Audio: loop() kann false liefern, während isRunning() noch true ist (Input(): kein Byte
 * vom Stream → MAD_FLOW_STOP). Sofortiger Reconnect zerstört einen sonst gesunden Stream. */
#ifndef AUDIO_SOFT_FAIL_MAX
#define AUDIO_SOFT_FAIL_MAX 500
#endif
static uint16_t s_audio_soft_fail_streak = 0;

static void audio_recover_or_stop(void) {
  stopPlaying();
  if (radio && connected && actStation < STATIONS) {
    RADIO_SERIAL(Serial.printf_P(PSTR("Stream unterbrochen — Reconnect %s\n"), stationlist[actStation].name));
    if (startUrl(stationlist[actStation].url)) {
      DrawFooterButtons_Power_Sleep_Alarm();
      return;
    }
  }
  radio = false;
  RADIO_SERIAL(Serial.println(F("Stream beendet / Reconnect fehlgeschlagen (Radio aus).")));
  DrawFooterButtons_Power_Sleep_Alarm();
}

// Diese Funktion überprüft, ob der Decoder läuft
void audio_loop() {
  if (!decoder) return;

  if (!decoder->isRunning()) {
    s_audio_soft_fail_streak = 0;
    audio_recover_or_stop();
    return;
  }

  for (int i = 0; i < AUDIO_LOOPS_PER_FRAME && decoder && decoder->isRunning(); i++) {
    if (!decoder->loop()) {
      if (!decoder->isRunning()) {
        s_audio_soft_fail_streak = 0;
        audio_recover_or_stop();
        return;
      }
      if (++s_audio_soft_fail_streak >= AUDIO_SOFT_FAIL_MAX) {
        s_audio_soft_fail_streak = 0;
        audio_recover_or_stop();
        return;
      }
      break;
    }
    s_audio_soft_fail_streak = 0;
  }
}

/* ICY/Shoutcast: StreamTitle oft ISO-8859-1 (ESP8266Audio: isUnicode=false). Display/Web brauchen UTF-8. */
static bool metadata_is_valid_utf8(const char *s) {
  const unsigned char *p = (const unsigned char *)s;
  while (*p) {
    if (*p < 0x80) {
      ++p;
      continue;
    }
    if ((*p & 0xE0) == 0xC0) {
      if (!p[1] || (p[1] & 0xC0) != 0x80) return false;
      p += 2;
      continue;
    }
    if ((*p & 0xF0) == 0xE0) {
      if (!p[1] || !p[2] || (p[1] & 0xC0) != 0x80 || (p[2] & 0xC0) != 0x80) return false;
      p += 3;
      continue;
    }
    if ((*p & 0xF8) == 0xF0) {
      if (!p[1] || !p[2] || !p[3] || (p[1] & 0xC0) != 0x80 || (p[2] & 0xC0) != 0x80 || (p[3] & 0xC0) != 0x80)
        return false;
      p += 4;
      continue;
    }
    return false;
  }
  return true;
}

static void metadata_latin1_to_utf8(const char *in, char *out, size_t outsz) {
  size_t o = 0;
  for (const unsigned char *p = (const unsigned char *)in; *p && o + 3 < outsz; ++p) {
    if (*p < 0x80)
      out[o++] = (char)*p;
    else {
      out[o++] = (char)(0xC0 | ((*p) >> 6));
      out[o++] = (char)(0x80 | ((*p) & 0x3F));
    }
  }
  if (outsz == 0) return;
  if (o >= outsz) o = outsz - 1;
  out[o] = 0;
}

/*
 * ICY/Shoutcast-Metadaten (StreamTitle):
 * - Viele Sender schicken nichts oder nur alle 10–60 s (Header icy-metaint).
 * - Kodierung: oft UTF-8, sonst ISO-8859-1; russische Sender manchmal Windows-1251 —
 *   dann schlägt utf8_ok fehl und der Latin-1-Fallback liefert falsche Zeichen (kein Kyrillisch).
 * - Leerer StreamTitle (len=0): ICY liefert manchmal Padding/leere Blöcke — wird ignoriert, Titel bleibt.
 * Diagnose: RADIO_DEBUG_SERIAL=1 in 00_settings.h, seriell 115200 Baud.
 * Extern: ffplay -loglevel debug URL oder Stream im Browser (Entwicklertools → Netzwerk).
 */
#if RADIO_DEBUG_SERIAL
static void radio_debug_icy_raw(const char *s, size_t maxshow) {
  Serial.print(F("ICY raw hex: "));
  const unsigned char *p = (const unsigned char *)s;
  for (size_t i = 0; i < maxshow && p[i]; i++) {
    Serial.printf("%02x ", (unsigned)p[i]);
  }
  Serial.println();
}
#endif

// Callback-Funktion, die aufgerufen wird, wenn Metadaten im Eingabestream gefunden werden
void MDCallback(void *cbData, const char *type, bool isUnicode, const char *string) {
  (void)cbData;
  if (type == nullptr || string == nullptr) {
    RADIO_SERIAL(Serial.println("Error: Null pointer encountered in MDCallback."));
    return;
  }

  char s1[32];
  /* Genug Platz für lange StreamTitle-Zeilen; zu kurz → abgeschnittener UTF-8 → utf8_ok=0. */
  char s2[384];
  strlcpy(s1, type, sizeof(s1));
  strlcpy(s2, string, sizeof(s2));

  if (strstr(s1, "Title") != nullptr) {
    if (s2[0] == '\0') {
#if RADIO_DEBUG_SERIAL
      Serial.println(F("ICY StreamTitle leer (len=0) — Titel unverändert, utf8_ok=1 nur trivial"));
#endif
      return;
    }
#if RADIO_DEBUG_SERIAL
    radio_debug_icy_raw(s2, 48);
    Serial.printf("ICY StreamTitle type='%s' isUnicode=%d len=%u utf8_ok=%d path=%s\n", s1, isUnicode ? 1 : 0,
                  (unsigned)strlen(s2), metadata_is_valid_utf8(s2) ? 1 : 0,
                  isUnicode ? "unicode" : (metadata_is_valid_utf8(s2) ? "utf8" : "latin1"));
#endif
    if (isUnicode) {
      strlcpy(title, s2, sizeof(title));
    } else if (metadata_is_valid_utf8(s2)) {
      strlcpy(title, s2, sizeof(title));
    } else {
      metadata_latin1_to_utf8(s2, title, sizeof(title));
    }
    RADIO_SERIAL(Serial.printf("Title: %s\n", title));
    newTitle = true;
  } else {
    RADIO_SERIAL(Serial.printf("METADATA '%s' = '%s'\n", s1, s2));
  }
}

// Stoppt das Abspielen des Eingabestreams, gibt den Speicher frei und löscht die Instanzen
void stopPlaying() {
  if (decoder) {
    decoder->stop();  // Stoppt das Abspielen
    delete decoder;   // Gibt den Decoder und seinen Speicher frei
    decoder = NULL;   // Setzt den Zeiger auf NULL, um darauf hinzuweisen, dass der Speicher freigegeben wurde
  }

  if (buff) {
    buff->close();  // Schließt den Puffer
    delete buff;    // Gibt den Puffer und seinen Speicher frei
    buff = NULL;    // Setzt den Zeiger auf NULL
  }

  if (file) {       // Und schließlich für den Stream
    file->close();  // Schließt den Stream
    delete file;    // Gibt den Stream und seinen Speicher frei
    file = NULL;    // Setzt den Zeiger auf NULL
  }

#if defined(ESP32)
  /* decoder->stop() hat output->stop() schon aufgerufen — hier nur sicherstellen, falls kein Decoder lief. */
  audio_i2s_release_after_pipeline_stop();
#else
  /* ESP8266: begin() ist idempotent wenn i2sOn; nach stop() Kanal wieder an für nächsten Stream. */
  audio_ensure_i2s_on_esp8266_style();
#endif
}

// Startet das Abspielen eines Streams von der angegebenen Station (URL aus stationlist[].url o. Ä.)
bool startUrl(const char *url) {
  stopPlaying();  // Zuerst bestehende Streams schließen

  if (!url || !*url) {
    RADIO_SERIAL(Serial.println(F("startUrl: leere URL")));
    return false;
  }

  if (!out || !ensure_audio_prealloc()) {
    RADIO_SERIAL(Serial.println(F("startUrl: kein I2S oder Audio-Puffer-Allokation fehlgeschlagen")));
    return false;
  }

  RADIO_SERIAL(Serial.printf_P(PSTR("startUrl: heap=%u vor open\n"), (unsigned)ESP.getFreeHeap()));
  yield();

  RADIO_SERIAL(Serial.printf("Active station %s\n", url));
  file = new AudioFileSourceICYStream();
  if (!file->open(url)) {
    RADIO_SERIAL(Serial.printf_P(PSTR("startUrl: open fehlgeschlagen heap=%u\n"), (unsigned)ESP.getFreeHeap()));
    delete file;
    file = NULL;
    return false;
  }

  file->RegisterMetadataCB(MDCallback, NULL);

  buff = new AudioFileSourceBuffer(file, preallocateBuffer, s_effective_buffer_size);
  RADIO_SERIAL(Serial.printf_P(PSTR("sourcebuffer created - Free mem=%d\n"), ESP.getFreeHeap()));

  decoder = (AudioGenerator *)new AudioGeneratorMP3(preallocateCodec, preallocateCodecSize);
  RADIO_SERIAL(Serial.printf_P(PSTR("created decoder\n")));
  RADIO_SERIAL(Serial.printf_P(PSTR("Decoder start...\n")));
  if (!decoder->begin(buff, out)) {
    RADIO_SERIAL(Serial.printf_P(PSTR("MP3 decoder begin failed (codec %d B, need %d B, heap %u)\n"),
                    preallocateCodecSize, (int)AudioGeneratorMP3::preAllocSize(), (unsigned)ESP.getFreeHeap()));
    delete decoder;
    decoder = NULL;
    buff->close();
    delete buff;
    buff = NULL;
    file->close();
    delete file;
    file = NULL;
    return false;
  }
  {
    unsigned long t0 = millis();
    while (decoder && decoder->isRunning() && millis() - t0 < 250) {
      decoder->loop();
      yield();
    }
  }
  setGain(curGain);
  RADIO_SERIAL(Serial.printf_P(PSTR("MP3 ok running=%d\n"), decoder->isRunning() ? 1 : 0));

  return decoder->isRunning();
}

// Ändert die Lautstärke auf den aktuellen Gain-Wert
void setGain(float gain) {
  if (!out) return;
  float g = gain / 100.0f;
  if (g < 0.0f) g = 0.0f;
  if (g > 1.0f) g = 1.0f;
  /* Max. Verstärkung bei 100 % Slider: AUDIO_GAIN_MAX (0..4, typ. 0.35–0.5) */
  float v = g * AUDIO_GAIN_MAX;
  if (v > 4.0f) v = 4.0f;
  out->SetGain(v);
  RADIO_SERIAL(Serial.printf_P(PSTR("New volume = %4.2f\n"), v));
}
