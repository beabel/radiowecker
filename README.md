


<div align="center">

|[:skull:ISSUE](https://github.com/beabel/radiowecker/issues?q=is%3Aissue)|[:speech_balloon: Forum /Discussion](https://github.com/beabel/radiowecker/discussions?discussions_q=)|[:grey_question:WiKi](https://github.com/beabel/radiowecker/wiki)|
|--|--|--|
|![GitHub issues](https://img.shields.io/github/issues/beabel/radiowecker)![GitHub closed issues](https://img.shields.io/github/issues-closed/beabel/radiowecker)|![GitHub Discussions](https://img.shields.io/github/discussions/beabel/radiowecker)![GitHub User's stars](https://img.shields.io/github/stars/beabel)|![GitHub release (with filter)](https://img.shields.io/github/v/release/beabel/radiowecker)


</div>


|This project is based on the blog article "[Internet clock radio with touchscreen](https://www.az-delivery.de/blogs/azdelivery-blog-fur-arduino-und-raspberry-pi/internet-radiowecker-mit-touchscreen)" by Gerald Lechner / Az-Delivery. | Dieses Projekt basiert auf dem Blogartikel "[Internet Radiowecker mit Touchscreen](https://www.az-delivery.de/blogs/azdelivery-blog-fur-arduino-und-raspberry-pi/internet-radiowecker-mit-touchscreen)" von Gerald Lechner / Az-Delivery.  |
|--|--|

> [!NOTE]  
> **Instruction / Anleitung**
> Eine detailierte Anleitung befindet sich [hier](https://github.com/beabel/radiowecker/wiki).

**Parts required: / Benötigte Teile:** 

||Pro|Cheap|Minimal|
|--|--|--|--|
|[1 x AZ-Touch MOD mit 2,8" Touchscreen](https://amzn.to/3srKxjE)|:white_check_mark:|:ballot_box_with_check:[1xTFT](https://amzn.to/3MEtdyy)|:x:|
|[1 x ESP-32 Dev Kit C V4](https://amzn.to/40ApxUA)|:white_check_mark:|:white_check_mark:|:white_check_mark:|
|[2 x I2S 3W MAX98357A](https://amzn.to/3QSHtGF)|:white_check_mark:|:white_check_mark:|:white_check_mark:(1x)|
|[1 x Lautsprecher Set](https://amzn.to/3SSyQNR)|:white_check_mark:|:white_check_mark:|:white_check_mark:|
|[1 x Widerstand 470 kOhm](https://amzn.to/3Sw17cL)|:white_check_mark:|:white_check_mark:|:white_check_mark:(1x470kOhm)|
|[DC Einbaubuchse](https://amzn.to/47it0ZY)|:white_check_mark:|:x:|:x:|
|[1 x LDR](https://amzn.to/3FQYCKq)|:white_check_mark:|:x:|:x:|
||~95,-|~54,-|~37,-|

[German Version below / Deutsche Version unten:point_down:](#german-version)
----
# Clock radio

I have extended and adapted this project.


At the time of publishing this blog article, a complete set was available for purchase from AZ-Delivery. Currently this is not offered, but all required parts can be ordered individually.


**[Additional information::point_down:](#informationen)**

## [Screenshots:point_down:](#screenshots)

## German Version

# Radiowecker

Ich habe dieses Projekt erweitert und angepasst.

Zum Zeitpunkt der Veröffentlichung des Blogartikels gab es ein vollständiges Set bei AZ-Delivery zu kaufen. Aktuell wird dies nicht angeboten, aber alle benötigten Teile können einzeln bestellt werden.

# Informationen

**Boardverwalter URL:**
[https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json](https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json)
- ESP32 von Espressif Systems

**Bibliotheken:**
- [Adafruit_ILI9341 by Adafruit Version 1.6.1](https://github.com/adafruit/Adafruit_ILI9341)
- [Adafruit_GFX by Adafruit Version 1.11.10](https://github.com/adafruit/Adafruit-GFX-Library)
- [XPT2046_Touchscreen by Paul Stoffregen Version 1.4.0](https://github.com/PaulStoffregen/XPT2046_Touchscreen)
- [Touchevent by Gerald-Lechner Version 1.3.0](https://github.com/GerLech/TouchEvent)
- [ESP8266Audio by Earle F. Philhower Version 1.9.7](https://github.com/earlephilhower/ESP8266Audio)

# Screenshots

![Main Screen](screenshot/result.jpg?raw=true "Hauptbildschirm")

![Radio Screen](screenshot/radio.jpg?raw=true "Radiobildschirm")

![Setting Screen](screenshot/settings.jpg?raw=true "Einstellungen")

![Alarm Screen](screenshot/alarm.jpg?raw=true "Alarm Einstellungen")

![Color Setting](screenshot/settings_color.png?raw=true "Farbeinstellungen")

![Konfigurationswebsite](screenshot/website.png?raw=true "Musik")

![Konfigurationswebsite](screenshot/website_2.png?raw=true "Wecker")

![Konfigurationswebsite](screenshot/website_3.png?raw=true "Radio")

![Konfigurationswebsite](screenshot/website_4.png?raw=true "WLAN")

![Info Tab](screenshot/info_tab.png?raw=true "Info Tab")

# Bedienungsanleitung

## Erste Schritte und Verbindung zum WLAN

1. **Gerät einschalten**  
   Nach dem ersten Start des DIY-Webradios oder wenn das Gerät kein bekanntes WLAN-Netzwerk findet, zeigt das Display die Meldung:

![IMG20240829222852](https://github.com/user-attachments/assets/b9cd9628-1b1a-47af-a4ec-16bbda20790b)

2. **Verbindung mit dem Konfigurations-WLAN herstellen**  
   Auf deinem Smartphone, Tablet oder Computer öffnest du die WLAN-Einstellungen und suchst nach verfügbaren Netzwerken. Wähle das Netzwerk mit der SSID (Netzwerkname) `radioweckerconf` aus, um dich mit dem Radiowecker zu verbinden. Dies ist ein temporäres Netzwerk, das das Radio erstellt, um die Konfiguration zu ermöglichen.

3. **Zugriff auf das Einstellungsmenü**  
   Sobald die Verbindung hergestellt ist, öffne einen Webbrowser (z. B. Chrome, Firefox, Safari) und gib in der Adressleiste die IP-Adresse `192.168.4.1` ein. Diese IP-Adresse führt dich zur Einstellungsseite des Radios.

![Screenshot_2024-08-29-23-22-25-22_40deb401b9ffe8e1df2f1cc5ba480b12](https://github.com/user-attachments/assets/bfbfbaba-883e-4363-8fa7-d76cd5086622)

4. **WLAN-Konfiguration**  
   Auf der Einstellungsseite wirst du aufgefordert, das WLAN-Netzwerk auszuwählen, mit dem das Radio dauerhaft verbunden werden soll. Wähle dein gewünschtes WLAN-Netzwerk aus und gib das entsprechende Passwort ein. Speichere die Einstellungen.

5. **Neustart des Geräts**  
   Nach der erfolgreichen Konfiguration wird der Radiowecker das temporäre Netzwerk `radioweckerconf` beenden und sich mit dem neu konfigurierten WLAN-Netzwerk verbinden. Das Radio sollte nun betriebsbereit sein und du kannst Sender auswählen und abspielen.

## Fehlerbehebung

- **Verbindung fehlgeschlagen**  
   Falls das DIY-Webradio nach der Konfiguration keine Verbindung zum gewünschten WLAN herstellen kann, wird erneut die Meldung "Nicht verbunden" angezeigt. In diesem Fall wiederhole den Verbindungsprozess.

- **WLAN-Netzwerkwechsel**  
   Solltest du das WLAN-Netzwerk wechseln müssen, kannst du das Gerät zurücksetzen oder einen ähnlichen Prozess durchlaufen, um das temporäre Netzwerk wiederherzustellen und eine neue Konfiguration vorzunehmen.

![IMG20240829222947](https://github.com/user-attachments/assets/173331b3-420c-48e5-8cce-321bdef13102)

## Anzeige auf dem TFT-Display

1. **Statusleiste (oben im Display):**
   - **Weckerstatus**: Ein Symbol zeigt an, ob der Wecker aktiviert oder deaktiviert ist. Ein Glockensymbol, das bei aktivem Wecker orange ist und die nächste Weckzeit anzeigt. Bei deaktiviertem Wecker ist ein rotes, durchgestrichenes Symbol sichtbar.
   - **Einschlaftimer**: Ein weiteres Symbol zeigt den Status des Einschlaftimers an. Ist der Timer aktiv, erscheint ein "Bett"-Symbol. Wenn der Timer nicht aktiv ist, ist dieses Symbol ausgeblendet.
   - **IP-Adresse**: Die aktuelle IP-Adresse des Geräts wird angezeigt, um bei Bedarf eine direkte Verbindung oder weitere Einstellungen zu ermöglichen.
   - **WLAN-Signalstärke**: Die Signalstärke des WLANs wird sowohl als numerischer Wert als auch als farbiges Symbol dargestellt:
     - **Grün**: Starkes Signal
     - **Gelb**: Mittelmäßiges Signal
     - **Rot**: Schwaches Signal

2. **Hauptanzeige (mittig im Display):**
   - **Aktuelle Uhrzeit**: Die aktuelle Uhrzeit wird prominent angezeigt, damit sie leicht ablesbar ist.
   - **Aktuelles Datum**: Unter der Uhrzeit wird das aktuelle Datum angezeigt, damit du sowohl die Zeit als auch das Datum im Blick hast.

3. **Lautstärkeregler (unten im Display):**
   - Ein Schieberegler für die Lautstärke befindet sich am unteren Rand des Displays. Du kannst den Regler nach links oder rechts schieben, um die Lautstärke des Webradios anzupassen.

4. **Radio:**
   - **Sendername und Titel**: Wenn ein Sender gerade läuft, werden hier die Informationen des Senders und des Titels angezeigt.
   - **Senderwechsel**: Ab Version 3.0.5 ist es möglich, rechts und links zum nächsten aktivierten Sender zu springen, ohne erst auf eine Unterseite schalten zu müssen. Dies erfolgt durch Berührung an den Seiten, 40px jeweils von den Rändern.

5. **Anzeige Einstellungs-Seiten:**
   - **Unterseiten**: Durch eine Berührung in die Mitte des Touchscreens wird die Unterseite Radio-Einstellungen angezeigt. Wenn keine Berührung mehr registriert wird, schaltet es automatisch zurück zur Hauptanzeige.

![Menu_Struktur_Bedienung](https://github.com/user-attachments/assets/11992ce4-75e4-4e49-ab48-89516053c784)

---

[![DJ Perry](https://github.com/user-attachments/assets/d750acc2-be60-44ec-8f23-0b9f69b249b6)](https://www.pulse107.com) 
[![Pulse 107](https://github.com/user-attachments/assets/02919e84-bc1d-4336-82cd-6e2b9bc36ce8)](https://www.pulse107.com/pulse-107) 
[![Strobe Radio](https://github.com/user-attachments/assets/a911f569-5a28-416c-8ecd-8053fa2392ae)](https://www.pulse107.com/the-strobe-radio)






