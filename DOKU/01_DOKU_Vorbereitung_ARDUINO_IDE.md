# Arduino IDE Setup für ESP32 und benötigte Bibliotheken (Firmware v5.0.2)

Nachdem die Arduino IDE installiert ist, folgen die Schritte zur Integration des ESP32-Boards und der benötigten Bibliotheken.

**Maßgeblich:** Die Haupt-README im Repository-Root ([`README.md`](../README.md)) — Versionen und Partitionsschema bei Abweichungen dort nachlesen.

## 1. ESP32 Boardverwalter-URL hinzufügen

1. Öffne die **Arduino IDE** (Referenz: **2.3.8** oder andere **2.x**).
2. Gehe zu `Datei` > `Voreinstellungen` (oder drücke `Strg + ,`).
3. Im Fenster **Voreinstellungen** findest du das Feld **Zusätzliche Boardverwalter-URLs**. Füge die folgende URL in dieses Feld ein:

```
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
```

4. Falls bereits andere URLs vorhanden sind, trenne sie mit einem Komma.
5. Klicke auf **OK**, um die Einstellungen zu speichern.

## 2. ESP32 Board installieren

1. Gehe zu `Werkzeuge` > `Board` > `Boardverwalter`.
2. Suche nach **esp32** im Suchfeld.
3. Wähle **esp32 by Espressif Systems** und installiere eine **aktuelle 3.x-Version** des Pakets (oder die neueste stabile Version, die der Boardverwalter anbietet).

> [!NOTE]
> **Früher** (ältere 4.x-Firmware) wurde oft **ESP32-Arduino-Core 2.0.17** empfohlen, weil der Sketch mit **Standard-Partition** und neuerem Core zu groß wurde. **Ab v5.0.2** ist ein **aktueller Core** vorgesehen, sofern unter **Werkzeuge** das **Partitionsschema „No FS 4MB“** (oder vergleichbar große App-Partition) gewählt ist — siehe [`02_DOKU_Bord_Auswahl.md`](02_DOKU_Bord_Auswahl.md).

4. Warte, bis die Installation abgeschlossen ist.

Nach der Installation sollte **ESP32** unter `Werkzeuge` > `Board` auswählbar sein.

## 3. Benötigte Bibliotheken installieren

Installiere die Bibliotheken über `Sketch` > `Include Library` > `Library Manager`. **Referenzstände** (v5.0.2):

| Bibliothek | Version (Referenz) |
|------------|-------------------|
| **lvgl** | 9.5.0 |
| **Adafruit ILI9341** | 1.6.3 |
| **Adafruit GFX Library** | 1.12.5 |
| **XPT2046_Touchscreen** | 1.4 |
| **ESP8266Audio** (Earle F. Philhower) | 2.4.1 |

Etwas neuere Patch-Versionen derselben Major/Minor-Linie sind in der Regel unkritisch; bei Compile-Fehlern die in der README genannten Stände nachstellen.

### 3.1 Adafruit ILI9341

1. Im **Library Manager** nach **Adafruit ILI9341** suchen.
2. Version **1.6.3** (oder kompatibel) wählen und **Installieren**.

### 3.2 Adafruit GFX Library

1. Nach **Adafruit GFX Library** suchen.
2. Version **1.12.5** installieren (wird von ILI9341 mitbenötigt).

### 3.3 XPT2046_Touchscreen

1. Nach **XPT2046_Touchscreen** (Paul Stoffregen) suchen.
2. Version **1.4** installieren.

**Hinweis:** Die Bibliothek **TouchEvent** wird in **v5.0.2 nicht** mehr verwendet — **nicht** installieren.

### 3.4 lvgl (LVGL 9)

1. Im **Library Manager** nach **lvgl** suchen.
2. **Version 9.5.x** installieren (kein LVGL 8.x).

**Pflicht nach der Installation:** Die Datei **`lv_conf.h`** aus dem Sketch-Ordner (`radiowecker/lv_conf.h` im Repository) **identisch** nach  
`Arduino/libraries/lvgl/src/lv_conf.h`  
kopieren (Windows typisch: `Benutzer\<Name>\Documents\Arduino\libraries\lvgl\src\lv_conf.h`). Ohne diese Kopie fehlen häufig Fonts oder LVGL-Features beim Kompilieren. Details und Checkliste: [`README.md`](../README.md) Abschnitt „LVGL: Pflichtschritt lv_conf.h“.

### 3.5 ESP8266Audio

1. Nach **ESP8266Audio** von **Earle F. Philhower** suchen.
2. Version **2.4.1** installieren.

### 3.6 ArduinoJson — nicht aus dem Bibliotheksverwalter

**ArduinoJson** liegt im Projekt bereits als **`radiowecker/ArduinoJson.h`**. Eine **zusätzliche** Installation über den Library Manager führt zu **Doppeldefinitionen** — **weglassen**.

## 4. Abschluss

- Board **ESP32 Dev Module** (oder passendes Modul), **Flash 4 MB**, **Partition „No FS 4MB“** einstellen — siehe nächste Datei [`02_DOKU_Bord_Auswahl.md`](02_DOKU_Bord_Auswahl.md).
- Danach Sketch öffnen ([`03_DOKU_Sketch.md`](03_DOKU_Sketch.md)) und einmal **Kompilieren**.

---

Für weitere Informationen: [`README.md`](../README.md) und die jeweiligen Bibliotheks-Repositories.
