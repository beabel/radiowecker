# Arduino IDE Setup für ESP32 und benötigte Bibliotheken

Nachdem die Arduino IDE installiert ist, folgen die Schritte zur Integration des ESP32 Boards und der benötigten Bibliotheken.

## 1. ESP32 Boardverwalter-URL hinzufügen

1. Öffne die **Arduino IDE**.
2. Gehe zu `Datei` > `Voreinstellungen` (oder drücke `Strg + ,`).
3. Im Fenster **Voreinstellungen** findest du das Feld **Zusätzliche Boardverwalter-URLs**. Füge die folgende URL in dieses Feld ein:
```
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
```
4. Falls bereits andere URLs vorhanden sind, trenne sie mit einem Komma.
5. Klicke auf **OK**, um die Einstellungen zu speichern.

## 2. ESP32 Board installieren

1. Gehe zu `Werkzeuge` > `Board` > `Boardverwalter`.
2. Suche nach **ESP32** im Suchfeld.
3. Wähle **ESP32 by Espressif Systems** aus und klicke auf **Installieren**.

> [!IMPORTANT]  
> **Hinweis:** ~~Stelle sicher, dass du **Version 2.0.17** auswählst, da es zu Problemen mit größeren Versionen ab 3.x kommen kann.~~
> Wenn die ESP8266Audio Bibliothek Version mind. 1.9.9 hat, gibt es keine Probleme mehr. 

4. Warte, bis die Installation abgeschlossen ist.

Nach der Installation sollte **ESP32** unter `Werkzeuge` > `Board` auswählbar sein.


## 3. Benötigte Bibliotheken installieren

Um die benötigten Bibliotheken zu installieren, befolge diese Schritte:

### 3.1 Adafruit_ILI9341 by Adafruit (Version 1.6.1)

1. Gehe zu `Sketch` > `Include Library` > `Library Manager`.
2. Suche nach **Adafruit_ILI9341**.
3. Wähle die **Version 1.6.1** aus dem Dropdown-Menü und klicke auf **Installieren**.

### 3.2 Adafruit_GFX by Adafruit (Version 1.11.10)

1. Im selben **Library Manager** suche nach **Adafruit_GFX**.
2. Wähle die **Version 1.11.10** und klicke auf **Installieren**.

### 3.3 XPT2046_Touchscreen by Paul Stoffregen (Version 1.4.0)

1. Im **Library Manager** suche nach **XPT2046_Touchscreen**.
2. Wähle die **Version 1.4.0** und klicke auf **Installieren**.

### 3.4 Touchevent by Gerald-Lechner (Version 1.3.0)

1. Im **Library Manager** suche nach **Touchevent**.
2. Wähle die **Version 1.3.0** und klicke auf **Installieren**.

### 3.5 ESP8266Audio by Earle F. Philhower (Version 1.9.9)

1. Im **Library Manager** suche nach **ESP8266Audio**.
2. Wähle die **Version 1.9.9** und klicke auf **Installieren**.

## 4. Abschluss

Nach der Installation der Board-Verwaltung und Bibliotheken kannst du nun dein ESP32-Projekt starten. Wähle unter `Werkzeuge` > `Board` das ESP32-Board aus, das du verwendest, und beginne mit der Entwicklung!

---

Für weitere Informationen und Unterstützung kannst du die offiziellen Dokumentationen der jeweiligen Bibliotheken oder das Arduino-Forum besuchen.
