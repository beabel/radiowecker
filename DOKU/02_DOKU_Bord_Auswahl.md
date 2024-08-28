# Konfiguration der Arduino IDE für ESP32

Nachdem alle Vorbereitungen abgeschlossen sind, folgt die Anleitung zur Auswahl des Boards, zum Finden des richtigen Ports und zur Installation des CH341-Treibers, falls notwendig.

## 1. Auswahl des ESP32-Boards

1. Öffne die **Arduino IDE**.
2. Gehe zu `Werkzeuge` > `Board` > `ESP32 Arduino`.
3. Wähle aus der Liste **"ESP32 Dev Module"** aus.

   Dies ist das Standard-Board, das für die meisten ESP32-Entwicklungsboards verwendet wird.

## 2. Den richtigen Port herausfinden

Um den richtigen COM-Port zu finden, an dem dein ESP32-Board angeschlossen ist, folge diesen Schritten:

1. **COM-Port identifizieren:**
   - Gehe in der Arduino IDE zu `Werkzeuge` > `Port`.
   - Notiere dir die angezeigten Ports, bevor du dein Board anschließt.
   - Schließe dein ESP32-Board an und öffne das Menü `Werkzeuge` > `Port` erneut. Ein neuer Eintrag sollte erscheinen.
   - Wähle den neuen Port aus (dies ist der COM-Port deines ESP32-Boards).

   **Hinweis:** Wenn du unsicher bist, welcher Port korrekt ist, kannst du den Port im Gerätemanager überprüfen:
   - Öffne den **Gerätemanager** (Rechtsklick auf das Windows-Startsymbol > **Gerätemanager**).
   - Unter **Anschlüsse (COM & LPT)** siehst du den neuen Port, der erscheint, wenn du das ESP32-Board anschließt. Dies ist der richtige COM-Port.

## 3. Installation des CH341-Treibers (falls benötigt)

Wenn dein ESP32-Board nach dem Anschließen keinen COM-Port anzeigt oder im Gerätemanager nicht erkannt wird, könnte es sein, dass du den CH341-Treiber installieren musst. Dies ist insbesondere für Boards notwendig, die den CH340/CH341-USB-zu-Seriell-Konverter verwenden.

### 3.1 CH341-Treiber herunterladen

1. Besuche die [offizielle CH341-Treiber-Seite](http://www.wch.cn/download/CH341SER_ZIP.html) oder lade den Treiber von einer vertrauenswürdigen Quelle herunter.

2. Lade die `.zip`-Datei herunter und entpacke sie.

### 3.2 Treiber installieren

1. Öffne die entpackte Datei und starte die `SETUP.exe`.
2. Folge den Anweisungen auf dem Bildschirm, um den Treiber zu installieren.
3. Nachdem die Installation abgeschlossen ist, verbinde das ESP32-Board erneut mit dem Computer.

### 3.3 Überprüfen der Installation

1. Öffne den **Gerätemanager** und überprüfe unter **Anschlüsse (COM & LPT)**, ob der CH340/CH341-Port korrekt angezeigt wird.
2. Gehe zurück zur Arduino IDE und wähle unter `Werkzeuge` > `Port` den entsprechenden COM-Port aus.

## 4. Abschluss

Nun sollte dein ESP32-Board einsatzbereit sein:

1. Stelle sicher, dass unter `Werkzeuge` > `Board` das **ESP32 Dev Module** ausgewählt ist.
2. Wähle unter `Werkzeuge` > `Port` den richtigen COM-Port.
3. Du kannst nun mit der Programmierung deines ESP32-Boards beginnen und das erste Programm hochladen.

---

Falls du auf Probleme stößt, überprüfe, ob alle Treiber korrekt installiert sind, und konsultiere die [offizielle Arduino-Hilfe](https://www.arduino.cc/en/Guide/Troubleshooting) für weitere Unterstützung.
