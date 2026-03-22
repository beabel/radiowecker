# Arduino Projekt "Radiowecker" aus GitHub herunterladen und öffnen

Nachdem alle Vorbereitungen getroffen sind, folgt hier die Anleitung, wie du das Projekt aus dem GitHub-Repository herunterlädst und in der Arduino IDE öffnest.

## 1. GitHub-Repository herunterladen

1. **Besuche das GitHub-Repository:**
   - Gehe in deinem Browser auf die Seite des Repositories: [https://github.com/beabel/radiowecker](https://github.com/beabel/radiowecker).

2. **Projekt herunterladen:**
   - Klicke auf den grünen Button **Code**.
   - Wähle **Download ZIP** aus. Dies lädt das gesamte Repository als ZIP-Datei auf deinen Computer herunter.

3. **ZIP-Datei entpacken:**
   - Navigiere zu dem Ordner, in dem die ZIP-Datei heruntergeladen wurde.
   - Entpacke die ZIP-Datei, indem du mit der rechten Maustaste darauf klickst und **Alle extrahieren...** auswählst oder ein entsprechendes Entpack-Tool verwendest.
   - Der entpackte Ordner sollte denselben Namen wie das Repository haben (z.B. `radiowecker-main`).

**Repository statt ZIP (git clone):** Der Sketch liegt ebenfalls im Unterordner **`radiowecker/`** unter dem Repository-Root — z. B. nach `git clone … radiowecker` die Datei **`radiowecker/radiowecker/radiowecker.ino`**.

## 2. Sketch in der Arduino IDE öffnen

1. **Sketch öffnen:**
   - Starte die **Arduino IDE**.
   - Gehe zu `Datei` > `Öffnen...`.
   - Navigiere in den **Sketch-Unterordner** des Repositories: z. B. `radiowecker-main/radiowecker` (ZIP) bzw. `<Klonordner>/radiowecker` (git).
   - Wähle die Datei `radiowecker.ino` aus und klicke auf **Öffnen**.

2. **Überprüfung des Projekts:**
   - Sobald der Sketch in der Arduino IDE geöffnet ist, überprüfe, ob alle Bibliotheken, die im Sketch verwendet werden, bereits installiert sind (du kannst dies durch Kompilieren des Sketches prüfen, indem du auf das Häkchen-Symbol oben links klickst).
   - **v5.0.0:** Bibliotheken, Partition **No FS 4MB** und die Kopie von **`lv_conf.h`** nach `libraries/lvgl/src/` sind Voraussetzung — siehe [`01_DOKU_Vorbereitung_ARDUINO_IDE.md`](01_DOKU_Vorbereitung_ARDUINO_IDE.md) und [`README.md`](../README.md).
   - Falls Fehler auftreten, stelle sicher, dass alle benötigten Bibliotheken, die in den vorherigen Schritten installiert wurden, korrekt integriert sind.

## 3. Sketch hochladen

1. **Board und Port überprüfen:**
   - Stelle sicher, dass unter `Werkzeuge` > `Board` das **ESP32 Dev Module** ausgewählt ist.
   - Wähle unter `Werkzeuge` > `Port` den richtigen COM-Port aus, an dem dein ESP32-Board angeschlossen ist.

2. **Sketch hochladen:**
   - Klicke auf den **Hochladen**-Button (Pfeil-Symbol) in der Arduino IDE, um den Sketch auf dein ESP32-Board zu übertragen.

## 4. Abschluss

Nachdem der Sketch erfolgreich hochgeladen wurde, sollte das Radiowecker-Projekt auf deinem ESP32-Board laufen. Du kannst jetzt den seriellen Monitor in der Arduino IDE öffnen, um Ausgaben zu beobachten und sicherzustellen, dass alles wie erwartet funktioniert.

---

Für weitere Anpassungen oder Informationen zum Projekt kannst du das GitHub-Repository durchsuchen oder die Dokumentation des Projekts lesen.
