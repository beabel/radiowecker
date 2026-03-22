# Bedienungsanleitung

Aktuelle **Screenshots** liegen im Repository unter [`screenshot/`](../screenshot/). **Namenskonvention:**
- **`display_*`** — Fotos vom **TFT** (Hardware-Display, LVGL-Oberfläche)
- **`webseite_*`** — Fotos der **HTML-Ausgabe** des Webservers (Ansicht im Browser, z. B. `192.168.4.1` im Konfigurationsmodus oder die Geräte-IP im Heimnetz)

## Erste Schritte und Verbindung zum WLAN

1. **Gerät einschalten**  
   Nach dem ersten Start des DIY-Webradios oder wenn das Gerät kein bekanntes WLAN-Netzwerk findet, zeigt das Display die Meldung:

![Display: Startseite (Beispiel, Radio aus, Wetter sichtbar)](../screenshot/display_startseite_radio_off_weather_on.jpg)

2. **Verbindung mit dem Konfigurations-WLAN herstellen**  
   Auf deinem Smartphone, Tablet oder Computer öffnest du die WLAN-Einstellungen und suchst nach verfügbaren Netzwerken. Wähle das Netzwerk mit der SSID (Netzwerkname) `radioweckerconf` aus, um dich mit dem Radiowecker zu verbinden. Dies ist ein temporäres Netzwerk, das das Radio erstellt, um die Konfiguration zu ermöglichen.

3. **Zugriff auf das Einstellungsmenü**  
   Sobald die Verbindung hergestellt ist, öffne einen Webbrowser (z. B. Chrome, Firefox, Safari) und gib in der Adressleiste die IP-Adresse `192.168.4.1` ein. Diese IP-Adresse führt dich zur Einstellungsseite des Radios.

![Webseite (HTML): Startseite](../screenshot/webseite_1_startseite.png)

4. **WLAN-Konfiguration**  
   Auf der Einstellungsseite wirst du aufgefordert, das WLAN-Netzwerk auszuwählen, mit dem das Radio dauerhaft verbunden werden soll. Wähle dein gewünschtes WLAN-Netzwerk aus und gib das entsprechende Passwort ein. Speichere die Einstellungen.

Weitere Bereiche derselben Webseite (Navigation je nach Aufbau der HTML-Oberfläche):

![Webseite (HTML): Wecker](../screenshot/webseite_2_alarmpage.png)

![Webseite (HTML): Radiosender](../screenshot/webseite_3_radiestations.png)

![Webseite (HTML): Einstellungen](../screenshot/webseite_4_settings.png)

![Webseite (HTML): Info](../screenshot/webseite_5_info.png)

5. **Neustart des Geräts**  
   Nach der erfolgreichen Konfiguration wird der Radiowecker das temporäre Netzwerk `radioweckerconf` beenden und sich mit dem neu konfigurierten WLAN-Netzwerk verbinden. Das Radio sollte nun betriebsbereit sein und du kannst Sender auswählen und abspielen.

![Display: Startseite mit laufendem Radio (Beispiel nach Einrichtung)](../screenshot/display_startseite_radio_on.jpg)

## Fehlerbehebung

- **Verbindung fehlgeschlagen**  
   Falls das DIY-Webradio nach der Konfiguration keine Verbindung zum gewünschten WLAN herstellen kann, wird erneut die Meldung "Nicht verbunden" angezeigt. In diesem Fall wiederhole den Verbindungsprozess (ab Schritt 1; das Display zeigt wieder den Konfigurationshinweis wie oben).

- **WLAN-Netzwerkwechsel**  
   Solltest du das WLAN-Netzwerk wechseln müssen, kannst du das Gerät zurücksetzen oder einen ähnlichen Prozess durchlaufen, um das temporäre Netzwerk wiederherzustellen und eine neue Konfiguration vorzunehmen.

## Anzeige auf dem TFT-Display (Firmware v5, LVGL)

Schematische Raster-Übersichten (ohne Fotos): [`Main_Screen_Raster.svg`](Main_Screen_Raster.svg), [`Config_Screen_Raster.svg`](Config_Screen_Raster.svg), [`Alarm_Screen_Raster.svg`](Alarm_Screen_Raster.svg).

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

![Display: Startseite, Radio eingeschaltet](../screenshot/display_startseite_radio_on.jpg)

3. **Lautstärkeregler (unten im Display):**
   - Ein Schieberegler für die Lautstärke befindet sich am unteren Rand des Displays. Du kannst den Regler nach links oder rechts schieben, um die Lautstärke des Webradios anzupassen.

4. **Radio / Wetter / Infos:**
   - **Sendername und Titel**: Wenn ein Sender gerade läuft, werden hier die Informationen des Senders und des Titels angezeigt.
   - **Senderwechsel (ab Firmware v5.0.0, LVGL):** Vor/Zurück erfolgt über **Buttons im Datumsbereich** unter der Uhr (nicht mehr über schmale seitliche Touch-Streifen wie in älteren Versionen).

![Display: Startseite, Radio aus, Wetter sichtbar](../screenshot/display_startseite_radio_off_weather_on.jpg)

5. **Einstellungs-, Wecker- und Favoriten-Seiten:**
   - **Unterseiten:** Wechsel über die **LVGL-Oberfläche** (Touch im mittleren Bereich / Fußzeilen-Navigation; Details in [`README.md`](../README.md) Abschnitt „Bedienung: Touch-Display“).

![Display: Einstellungsseite](../screenshot/display_settingspage.jpg)

![Display: Wecker-Seite](../screenshot/display_alarmpage.jpg)

![Display: Favoritenseite](../screenshot/display_favoritenseite.jpg)
