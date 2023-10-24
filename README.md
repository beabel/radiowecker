# radiowecker
ESP Internet Radio angepasst (Dank an Gerald Lechner und AZ-Delivery)

Dies ist meine privat angepasste Version vom original Radiowecker von Gerald Lechner / AZ-Delivery.

Original Beitrag: 

https://www.az-delivery.de/blogs/azdelivery-blog-fur-arduino-und-raspberry-pi/internet-radiowecker-mit-touchscreen?comment=134773833995#comments

Die Anpassungen sind bisher:

index.h ==> modernere Ansicht der Konfiguration und bessere Tages Eingabe der Wochentage beim Wecker

01_ziffern.ino ==> jetzt monochrom Zahlen, welche jetzt im Code einfacher an die Wunschfarbe angepasst werden können

tft_display.ino ==> anpassen der Farbe ermöglicht

tft_color_setting.h ==> NEU alle Einstellungen der Farben an 1 Stelle für die Hauptseite

knoepfe_neu.h ==> NEU jetzt sind die Knöpfe monochrom, somit anpassbar (neuer include in radiowecker.ino)
Später werden die Köpfe einzeln sein, um sie dynamisch anzupassen.

Hier einige Ansichten:

![Main Screen](screenshot/result.jpg?raw=true "Result")

![Knöpfe neu](screenshot/knoepfe_neu.jpg?raw=true "Knöpfe neu")

![Color Setting](screenshot/settings_color.png?raw=true "Setting")

![Konfig Website](screenshot/website.png?raw=true "Konfiguration")


