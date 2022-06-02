# mood_tracker
IoT tracker of input feedback

Dispozitivul se foloseste de senzorul ultrasonic pentru a detecta miscarea in proximitatea sa, iluminandu-se in consecinta.
Acesta ofera prin intermediul butoanelor incorporate trei variante de feedback, pozitiv ▲ , neutru ~ sau negativ ▼, feedback ce este insotit de un semnal
sonor oferit de buzzer si apoi inregistrat si afisat pe ecranul LCD. 
In momentul in care sunt inregistrate 10 feedback-uri de orice fel, acesta trimite printr-o cerere http efectuata la api-ul askSensors numarul updatat de 
pareri inregistrate. Acest lucru este posibil cu ajutorul modulului Wifi ESP8266, care faciliteaza conectarea la internet si comunicarea seriala cu
placuta Arduino.
Comunicarea dintre modulul wifi si arduino este facilitata de biblioteca SoftwareSerial, ce permite definirea propriilor pini RX si TX.
