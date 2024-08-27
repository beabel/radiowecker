// Puffergröße für das Stream-Buffern
const int preallocateBufferSize = 82 * 1024;  // Größe des vorab zugewiesenen Puffers in Bytes
const int preallocateCodecSize = 29192;       // Maximal benötigter Speicher für den MP3-Codec in Bytes

// Zeiger auf vorab zugewiesenen Speicher
void *preallocateBuffer = NULL;  // Zeiger auf den vorab zugewiesenen Puffer für die Daten
void *preallocateCodec = NULL;   // Zeiger auf den vorab zugewiesenen Puffer für den Codec

// Instanzen für Audio-Komponenten
AudioGenerator *decoder = NULL;         // MP3-Decoder
AudioFileSourceICYStream *file = NULL;  // Eingabemodul für ICY-Streams aus dem Web
AudioFileSourceBuffer *buff = NULL;     // Puffer für den Eingabestream
AudioOutputI2S *out;                    // Ausgabemodul zum Erzeugen von I2S-Signalen

// Funktion für die gesamte erforderliche Einrichtung
void setup_audio() {
  // Speicher für Decoder und Stream reservieren
  preallocateBuffer = malloc(preallocateBufferSize);  // Speicher für den Stream-Dateipuffer anfordern
  preallocateCodec = malloc(preallocateCodecSize);    // Speicher für den Decoder-Puffer anfordern
  delay(1000);                                        // Warte 1 Sekunde, um sicherzustellen, dass der Speicher zugewiesen wurde

  // Überprüfen, ob der Speicher erfolgreich zugewiesen wurde
  if (!preallocateBuffer || !preallocateCodec) {
    Serial.printf_P(PSTR("FATAL ERROR: Unable to preallocate %d bytes for app\n"), preallocateBufferSize + preallocateCodecSize);
    while (1) {
      yield();  // Endlosschleife zur Behebung des Fehlers
    }
  }

  // I2S-Ausgabe erstellen, um sie mit dem Decoder zu verwenden
  out = new AudioOutputI2S();         // Neues AudioOutputI2S-Objekt erstellen
  out->SetPinout(BCLK, LRCLK, DOUT);  // Pin-Konfiguration für I2S-Ausgabe festlegen
}

// Diese Funktion überprüft, ob der Decoder läuft
void audio_loop() {
  // Überprüfen, ob der Decoder läuft
  if (decoder->isRunning()) {
    // Wenn der Decoder läuft, überprüfen, ob der Loop noch läuft
    if (!decoder->loop()) {
      // Wenn der Loop nicht mehr läuft, stoppen Sie den Decoder
      decoder->stop();
    }
  } else {
    // Der Decoder hat gestoppt, was auf einen Fehler hindeutet
    Serial.printf("MP3 done\n");
    // Um zu vermeiden, dass das gleiche Problem nach einem Neustart erneut auftritt,
    // wird die Station auf den ersten Eintrag in der Liste gesetzt.
    // Der erste Eintrag sollte eine stabile, funktionierende URL sein.
    // Verwenden Sie diesen ersten Eintrag nicht für Experimente.
    pref.putUShort("station", 0);

    // Warten Sie 10 Sekunden, bevor Sie das ESP neu starten
    delay(10000);

    // Neustart des ESP
    ESP.restart();
  }
}

// Callback-Funktion, die aufgerufen wird, wenn Metadaten im Eingabestream gefunden werden
void MDCallback(void *cbData, const char *type, bool isUnicode, const char *string) {
  // Sicherstellen, dass die Zeiger gültig sind
  if (type == nullptr || string == nullptr) {
    Serial.println("Error: Null pointer encountered in MDCallback.");
    return;
  }

  // Hinweis: Der Typ und der String können im PROGMEM sein, daher Kopie ins RAM für printf
  char s1[32], s2[64];
  strncpy_P(s1, type, sizeof(s1) - 1);
  s1[sizeof(s1) - 1] = 0;
  strncpy_P(s2, string, sizeof(s2) - 1);
  s2[sizeof(s2) - 1] = 0;

  // Verarbeiten von Metadaten-Typ "Title"
  if (strstr_P(s1, PSTR("Title"))) {
    strncpy(title, s2, sizeof(title));
    title[sizeof(title) - 1] = 0;
    Serial.printf("Title: %s\n", title);
    // Zeige die Nachricht auf dem Display an
    newTitle = true;
  } else {
    // Nur den Typ und den String für generische Metadaten anzeigen
    Serial.printf("METADATA '%s' = '%s'\n", s1, s2);
  }

  // Serial.flush(); // Optional: Zum Flushing der Serial-Ausgabe, falls notwendig
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
}

// Startet das Abspielen eines Streams von der angegebenen Station
bool startUrl(String url) {
  bool ret = true;
  stopPlaying();  // Zuerst bestehende Streams schließen

  // Öffne die Eingabedatei für die ausgewählte URL
  Serial.printf("Active station %s\n", url.c_str());
  file = new AudioFileSourceICYStream();
  ret = file->open(url.c_str());

  if (ret) {
    // Registriere Callback für Metadaten
    file->RegisterMetadataCB(MDCallback, NULL);

    // Erstelle einen neuen Puffer, der den vorab zugewiesenen Speicher verwendet
    buff = new AudioFileSourceBuffer(file, preallocateBuffer, preallocateBufferSize);
    Serial.printf_P(PSTR("sourcebuffer created - Free mem=%d\n"), ESP.getFreeHeap());

    // Erstelle und starte einen neuen Decoder
    decoder = (AudioGenerator *)new AudioGeneratorMP3(preallocateCodec, preallocateCodecSize);
    Serial.printf_P(PSTR("created decoder\n"));
    Serial.printf_P("Decoder start...\n");
    delay(1000);
    decoder->begin(buff, out);
  } else {
    delete file;  // Wenn kein Erfolg, lösche die Datei
    file = NULL;
  }

  return ret;
}

// Ändert die Lautstärke auf den aktuellen Gain-Wert
void setGain(float gain) {
  float v = gain / 100 * 0.5;                // Berechnet die Lautstärke als Prozentsatz von 0,5
  out->SetGain(v);                           // Setzt die Lautstärke
  Serial.printf("New volume = %4.2f\n", v);  // Gibt den neuen Lautstärkewert aus
}
