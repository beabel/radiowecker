# Aufbau des Radiowecker-Projekts

Es gibt mehrere Möglichkeiten, das Radiowecker-Projekt aufzubauen. Hier sind die verschiedenen Optionen beschrieben, einschließlich der benötigten Komponenten und Schritte.

## 1. Option 1: Verwenden des AZ-Touch Sets von AZ-Delivery

### Komponenten:
- **AZ-Touch Set** (inkl. ESP32 Dev Kit v4, 2.8" TFT-Display, Max98357A, Lautsprecher)
- Dieses Set enthält alle notwendigen Komponenten und eine vorgefertigte Platine, sodass keine zusätzlichen Teile benötigt werden.

### Aufbau:

1. **Bestellen des Sets:**
   - Das AZ-Touch Set kann über diesen [Link](https://www.az-delivery.de/products/radiowecker-mit-dem-az-touch?_pos=2&_psq=az-touch&_ss=e&_v=1.0) erworben werden.

2. **Zusammenbau:**
   - Folge den Anweisungen, die im [hier](https://www.az-delivery.de/blogs/azdelivery-blog-fur-arduino-und-raspberry-pi/internet-radiowecker-mit-touchscreen) enthalten sind.
   - Löte die Verbindungen, wie in der Anleitung beschrieben, um die Komponenten zu verbinden
   - Montiere das Display und die Lautsprecher in das Gehäuse des AZ-Touch.
   - Verbinde den ESP32 Dev Kit v4 mit der Platine im Gehäuse.

3. **Software hochladen:**
   - Nachdem der Aufbau abgeschlossen ist, lade den Sketch wie in den vorherigen Anleitungen beschrieben auf das ESP32-Board.

4. **Test:**
   - Stelle sicher, dass das Display, die Lautsprecher und alle anderen Komponenten ordnungsgemäß funktionieren.

---

## 2. Option 2: Verwenden einer fertigen Platine

### Komponenten:
- **ESP32 Dev Kit v4**
- **2.8" TFT-Display**
- **2x Max98357A I2S Digital-Analog-Wandler**
- **Lautsprecher**
- **Fertige Platine nach dem Schaltplan auf EasyEDA**

### Aufbau:

1. **Platinenlayout:**
   - Besuche den Schaltplan auf EasyEDA: [EasyEDA Schaltplan](https://easyeda.com/editor#project_id=b7c90a5cbe554e4dbcf25dbf78662e11).
   - Du kannst die Platine entweder selbst herstellen oder bei einem PCB-Hersteller in Auftrag geben.

2. **Zusammenbau:**
   - Sobald die Platine fertig ist, montiere die Komponenten auf der Platine.
   - Löte die Verbindungen gemäß dem Schaltplan.

3. **Software hochladen:**
   - Nach dem Zusammenbau der Platine lade den Sketch wie zuvor beschrieben auf das ESP32-Board.

4. **Test:**
   - Prüfe, ob die Verbindungen korrekt sind und alle Komponenten funktionieren.

---

## 3. Option 3: Aufbau auf einem Steckbrett

### Komponenten:
- **ESP32 Dev Kit v4**
- **2.8" TFT-Display**
- **2x Max98357A I2S Digital-Analog-Wandler**
- **Lautsprecher**
- **Steckbrett und Jumper-Kabel**

### Aufbau:

1. **Bestellen der Komponenten:**
   - Besorge dir die notwendigen Komponenten:
     - ESP32 Dev Kit v4
     - 2.8" TFT-Display
     - 2x Max98357A
     - Lautsprecher
     - Steckbrett und Jumper-Kabel

2. **Schaltplan und Verdrahtung:**
   - Verwende den Schaltplan auf [Github](https://github.com/beabel/radiowecker/blob/main/DOKU/Cheap/Fritzing_Steckplatine.png) als Referenz für die Verdrahtung.
   - Platziere die Komponenten auf dem Steckbrett und verbinde sie mit den Jumper-Kabeln.

3. **Verbindungen herstellen:**
   - **TFT-Display**: Verbinde die Pins des Displays entsprechend mit dem ESP32 Dev Kit.
   - **Max98357A**: Verbinde die zwei Max98357A-Module mit dem ESP32 für die Audio-Ausgabe.
   - **Lautsprecher**: Schließe die Lautsprecher an die Ausgänge der Max98357A-Module an.

4. **Software hochladen:**
   - Lade den Sketch auf das ESP32-Board hoch, wie in den vorherigen Anleitungen beschrieben.

5. **Test:**
   - Überprüfe die Verbindungen und teste die Funktionalität des Radioweckers.

---

## Abschluss

Egal für welche Option du dich entscheidest, nach dem Aufbau und Hochladen der Software sollte dein Radiowecker funktionieren. Bei Problemen überprüfe alle Verbindungen und stelle sicher, dass alle Komponenten korrekt installiert sind. Viel Spaß beim Bauen und Programmieren!
