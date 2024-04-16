


<div align="center">

|[:skull:ISSUE](https://github.com/beabel/radiowecker/issues?q=is%3Aissue)|[:speech_balloon: Forum /Discussion](https://github.com/beabel/radiowecker/discussions?discussions_q=)|[:grey_question:WiKi](https://github.com/beabel/radiowecker/wiki)|
|--|--|--|
|![GitHub issues](https://img.shields.io/github/issues/beabel/radiowecker)![GitHub closed issues](https://img.shields.io/github/issues-closed/beabel/radiowecker)|![GitHub Discussions](https://img.shields.io/github/discussions/beabel/radiowecker)![GitHub User's stars](https://img.shields.io/github/stars/beabel)|![GitHub release (with filter)](https://img.shields.io/github/v/release/beabel/radiowecker)


</div>


|This project is based on the blog article "[Internet clock radio with touchscreen](https://www.az-delivery.de/blogs/azdelivery-blog-fur-arduino-und-raspberry-pi/internet-radiowecker-mit-touchscreen)" by Gerald Lechner / Az-Delivery. | Dieses Projekt basiert auf dem Blogartikel "[Internet Radiowecker mit Touchscreen](https://www.az-delivery.de/blogs/azdelivery-blog-fur-arduino-und-raspberry-pi/internet-radiowecker-mit-touchscreen)" von Gerald Lechner / Az-Delivery.  |
|--|--|


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

The main changes include:

1. customizable colors.
2. more information on the main page.
3. splitting the radio and settings pages to make space for favorites buttons.
4. a completely redesigned web configuration page with full remote view and control of the radio.

NEW SINCE V3.0.0

5. Alarm Setting possible on Touchscreen (Big Thx to JeaneLG)

At the time of publishing this blog article, a complete set was available for purchase from AZ-Delivery. Currently this is not offered, but all required parts can be ordered individually.


**[Additional information::point_down:](#informationen)**

## [Screenshots:point_down:](#screenshots)

## German Version

# Radiowecker

Ich habe dieses Projekt erweitert und angepasst.

Die wesentlichen Änderungen umfassen:

1. Anpassbare Farben.
2. Mehr Informationen auf der Hauptseite.
3. Aufteilung der Radio- und Einstellungsseiten, um Platz für Favoritentasten zu schaffen.
4. Eine vollständig überarbeitete Webkonfigurationsseite mit umfassender Fernansicht und Steuerung des Radios.

NEU seit V3.0.0

5. Alarm Einstellungen auch auf dem Touchscreen möglich (Danke an JeaneLG)

Zum Zeitpunkt der Veröffentlichung des Blogartikels gab es ein vollständiges Set bei AZ-Delivery zu kaufen. Aktuell wird dies nicht angeboten, aber alle benötigten Teile können einzeln bestellt werden.

# Informationen

**Boardverwalter URL:**
[https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json](https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json)
- ESP32 von Espressif Systems

**Bibliotheken:**
- [Adafruit_ILI9341 by Adafruit Version 1.6.00](https://github.com/adafruit/Adafruit_ILI9341)
- [Adafruit_GFX by Adafruit Version 1.11.9](https://github.com/adafruit/Adafruit-GFX-Library)
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
