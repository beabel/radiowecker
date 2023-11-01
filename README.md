# Radiowecker

Dieses Projekt basiert auf dem Blogartikel "[Internet Radiowecker mit Touchscreen](https://www.az-delivery.de/blogs/azdelivery-blog-fur-arduino-und-raspberry-pi/internet-radiowecker-mit-touchscreen)" von Gerald Lechner / Az-Delivery. 

Ich habe dieses Projekt erweitert und angepasst.

Die wesentlichen Änderungen umfassen:

1. Anpassbare Farben.
2. Mehr Informationen auf der Hauptseite.
3. Aufteilung der Radio- und Einstellungsseiten, um Platz für Favoritentasten zu schaffen.
4. Eine vollständig überarbeitete Webkonfigurationsseite mit umfassender Fernansicht und Steuerung des Radios.

Zum Zeitpunkt der Veröffentlichung des Blogartikels gab es ein vollständiges Set bei AZ-Delivery zu kaufen. Aktuell wird dies nicht angeboten, aber alle benötigten Teile können einzeln bestellt werden.

**Benötigte Teile:**

- [1 x AZ-Touch MOD mit 2,8" Touchscreen](https://www.amazon.de/AZDelivery-Wandgeh%25C3%25A4useset-Touchscreen-SPI-Touch-Controller-kompatibel/dp/B08LL6YKWS?ref_=ast_sto_dp&amp;th=1&_encoding=UTF8&tag=kunigunde-21&linkCode=ur2&linkId=f9715e9370dae9b8dfaa2f869e476cf6&camp=1638&creative=6742)
- [1 x ESP-32 Dev Kit C V4](https://www.amazon.de/AZDelivery-NodeMCU-Development-Nachfolgermodell-ESP8266/dp/B0BF5NRF8H?ref_=ast_sto_dp&amp;th=1&_encoding=UTF8&tag=kunigunde-21&linkCode=ur2&linkId=298a863084ebff496a7d676f496cc987&camp=1638&creative=6742)
- [2 x I2S 3W MAX98357A](https://www.amazon.de/AZDelivery-filterloses-Breakout-Modul-Decoder-Modul-Anwendungsbereich/dp/B09PL8XKPX?ref_=ast_sto_dp&amp;th=1&_encoding=UTF8&tag=kunigunde-21&linkCode=ur2&linkId=91b4a0e36db75088f843bea7ea1f7185&camp=1638&creative=6742)
- [1 x Lautsprecher Set](https://www.amazon.de/AZDelivery-tragbarer-Mini-Lautsprecher-Schnittstelle-elektronische/dp/B09PL6XFHB?ref_=ast_sto_dp&amp;th=1&_encoding=UTF8&tag=kunigunde-21&linkCode=ur2&linkId=add11a2c40e472311164a4afb2cbf93d&camp=1638&creative=6742)
- [1 x Widerstand 470 kOhm](https://www.amazon.de/dp/B07Q87JZ9G/ref=redir_mobile_desktop?_encoding=UTF8&amp;aaxitk=c9f745e69ea368a67c85ee24786eb4d0&amp;content-id=amzn1.sym.6f8b36f0-c2c9-44f2-97a8-5b151d2fc9c7%253Aamzn1.sym.6f8b36f0-c2c9-44f2-97a8-5b151d2fc9c7&amp;hsa_cr_id=4093011600002&amp;pd_rd_plhdr=t&amp;pd_rd_r=7fc9fc2d-6c55-4301-bda6-db2fc18511b1&amp;pd_rd_w=eVOTt&amp;pd_rd_wg=kTV0v&amp;qid=1698844633&amp;ref_=sbx_be_s_sparkle_mcd_asin_0_title&amp;sr=1-1-e0fa1fdd-d857-4087-adda-5bd576b25987&amp;th=1&_encoding=UTF8&tag=kunigunde-21&linkCode=ur2&linkId=7faa54f01d926614c4082c8bdde594dc&camp=1638&creative=6742)
- [DC Einbaubuchse](https://www.amazon.de/RUNCCI-YUN-Socket%25EF%25BC%2588Mit-DC-Hohlstecker-L%25C3%25B6tanschluss-%25C3%259Cberwachungskameras/dp/B0836C9PWR/ref=sr_1_3?__mk_de_DE=%25C3%2585M%25C3%2585%25C5%25BD%25C3%2595%25C3%2591&amp;crid=24MZIOM3BT8A4&amp;keywords=RUNCCI-YUN+5%252C5+mm+x+2%252C1+mm+DC+Socket%25EF%25BC%2588Mit+Kabel%25EF%25BC%2589+6X+DC-Hohlstecker+mit+L%25C3%25B6tanschluss%252C+5%252C5+mm&amp;qid=1698846578&amp;s=ce-de&amp;sprefix=runcci-yun+5+5+mm+x+2+1+mm+dc+socket+mit+kabel+6x+dc-hohlstecker+mit+l%25C3%25B6tanschluss+5+5+mm%252Celectronics%252C78&amp;sr=1-3&_encoding=UTF8&tag=kunigunde-21&linkCode=ur2&linkId=fc9b9732beaa2cc27185a6b539f25b46&camp=1638&creative=6742)
- [1 x LDR](https://www.amazon.de/AZDelivery-Lichtsensor-Fotowiderstand-Helligkeitssensor-kompatibel/dp/B07ZYXHF3C?ref_=ast_sto_dp&amp;th=1&_encoding=UTF8&tag=kunigunde-21&linkCode=ur2&linkId=16353e2b25a923beb8d6dfce46538414&camp=1638&creative=6742)

**Zusätzliche Informationen:**

- Boardverwalter URL: [https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json](https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json)
- ESP32 von Espressif Systems

**Benötigte Bibliotheken:**

- [Adafruit_ILI9341 by Adafruit Version 1.6.00](https://github.com/adafruit/Adafruit_ILI9341)
- [Adafruit_GFX by Adafruit Version 1.11.9](https://github.com/adafruit/Adafruit-GFX-Library)
- [XPT2046_Touchscreen by Paul Stoffregen Version 1.4.0](https://github.com/PaulStoffregen/XPT2046_Touchscreen)
- [Touchevent by Gerald-Lechner Version 1.3.0](https://github.com/GerLech/TouchEvent)
- [ESP8266Audio by Earle F. Philhower Version 1.9.7](https://github.com/earlephilhower/ESP8266Audio)

**Screenshots:**

![Main Screen](screenshot/result.jpg?raw=true "Hauptbildschirm")

![Radio Screen](screenshot/radio.jpg?raw=true "Radiobildschirm")

![Setting Screen](screenshot/settings.jpg?raw=true "Einstellungen")

![Color Setting](screenshot/settings_color.png?raw=true "Farbeinstellungen")

![Konfigurationswebsite](screenshot/website.png?raw=true "Musik")

![Konfigurationswebsite](screenshot/website_2.png?raw=true "Wecker")

![Konfigurationswebsite](screenshot/website_3.png?raw=true "Radio")

![Konfigurationswebsite](screenshot/website_4.png?raw=true "WLAN")
