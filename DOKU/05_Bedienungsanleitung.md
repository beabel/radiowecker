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

