# Dozator inteligent de pastile cu autentificare prin amprentă

Proiect pentru laboratorul de SIO-EP.

Membri: Dușa Adriana Cristina, Carcalețeanu Cristian Nicolae, Andronache Darius Fabian

## Despre proiect

Dozatorul eliberează pastilele la orele setate de utilizator. Pastila cade într-o cutie închisă cu un capac, iar capacul se deschide doar după ce senzorul recunoaște amprenta. Așa pacientul își ia doza la timp, iar altcineva nu are acces la medicamente.

## Cum funcționează

Pastilele stau în compartimentele unei roți. Sub roată se află un disc fix cu o singură fantă. La ora programată, motorul pas cu pas rotește roata până când următorul compartiment ajunge deasupra fantei, iar pastila cade în cutie. Senzorul optic verifică dacă pastila a ajuns în cutie.

Dacă pastila nu apare, dozatorul mai încearcă o rotire. După 3 încercări ratate, scrie `ERROR` în log și pornește alarma.

Când pastila e în cutie, sistemul așteaptă amprenta. O amprentă recunoscută deschide capacul (servomotorul SG90). O amprentă greșită crește un contor de erori, limitat la 3 încercări. După ce capacul se deschide, senzorul optic verifică dacă pastila a fost luată:

- dacă da, se scrie `TAKEN` în log, capacul se închide și dozatorul revine în așteptare;
- dacă nu, capacul se închide și sistemul așteaptă din nou amprenta.

Dacă vine ora unei doze noi și cutia încă are pastila de data trecută, doza veche se marchează `MISSED`. Dozatorul se blochează, pornește alarma și nu mai eliberează nimic până când utilizatorul se autentifică și golește cutia.


## Componente

| Nr. | Componentă |
|---|---|
| 1 | Arduino UNO R3 |
| 2 | Ceas RTC DS3231 (cu memorie AT24C32) |
| 3 | Display OLED I2C |
| 4 | Senzor optic reflectiv TCRT5000 |
| 5 | Motor pas cu pas 28BYJ-48 cu driver ULN2003 |
| 6 | Senzor de amprentă AS608 |
| 7 | Servomotor SG90 |
| 8 | LED-uri 3 mm (verde, roșu) |
| 9 | Butoane |
| 10 | Sursă 5V 2A |
| 11 | Buzzer |
| 12 | Rezistente pentru LED-uri |
| 13 | Sursa alimentare 5V 2A (pentru motor pas cu pas, servomotor) |

## Conexiuni

| Pin Arduino | Componentă |
|---|---|
| D2 | RTC DS3231, SQW (întrerupere pentru alarmă) |
| D3 | Buton SET |
| D4, D5, D6, D7 | Driver ULN2003, IN4, IN3, IN2, IN1 |
| D8 | Buton RESET |
| D9 | Servomotor SG90, semnal |
| D10 | AS608, TX |
| D11 | AS608, RX |
| D12 | Buzzer |
| D13 | LED verde |
| A0 | Senzor optic TCRT5000 |
| A1 | LED roșu |
| A2 | Buton URGENȚĂ |
| A4 (SDA), A5 (SCL) | Magistrala I2C: RTC DS3231 și ecran OLED |


## Software

TODO: servo, RTC, servo, amprenta, OLED

## Ce se învață din proiect

- controlul unui motor pas cu pas și al unui servomotor pentru dozarea automată.
- lucrul cu un senzor de amprentă, cu un modul RTC și cu un display OLED pe I2C.

## Stadiu

sanatate
