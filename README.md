# Meteostanica
Zadanie: Meteostanica

## Cieľ projektu: 
Návrh a implementácia zariadenia na meranie aktuálnych meteorologických podmienok a zobrazovanie predpovede počasia. Údaje sú získavané pomocou senzorov LPS25HB a HTS221 a výsledné údaje sú zobrazované na TFT dispeji.

**Použité časti hardware:**
- Mikrokontrolér STM32
- Senzory LPS25HB a HTS221
- TFT displej (2.8 , 240x320) – ILI 9341
- SD karta
- Breadboard
- Počítač

### 1. Meranie a spracovanie údajov (Sensor Specialist)
Úloha: 
Zabezpečiť správne pripojenie senzorov teplota, tlak, vlhkosť k STM32, načítavanie údajov zo senzorov a ich spracovanie.
Konkrétne úlohy:
Pripojenie senzorov k STM32 a ich konfigurácia I2C, LPS25HB a HTS221
Naprogramovanie načítania a kalibrácie údajov zo senzorov.
Prepojenie senzorových údajov s algoritmom na výpočet základnej predpovede počasia na základe tlaku, vlhkosti a teploty.

##### Výstup: Kód na čítanie údajov zo senzorov a prepočet na predpoveď počasia.<br/>
##### Vypracovateľka : Klaudia Piwková

### 2. Zobrazenie údajov na TFT displeji (UI Designer)
Úloha: 
Zabezpečiť zobrazenie údajov o počasí na TFT displeji. Zabezpečiť interakciu s používateľom.
Konkrétne úlohy:
Práca s ILI ovládačom na komunikáciu s TFT displejom.
Navrhnúť rozloženie obrazovky, aby zobrazovala všetky potrebné informácie ako aktuálna teplota, vlhkosť, tlak, predpoveď.
Implementácia dotykových funkcií na prehliadanie histórie alebo obnovenie údajov.

##### Výstup: Kód na zobrazovanie údajov a interakciu s displejom.<br/>
##### Vypracovateľka : Zuzana Kittnerová

### 3. Systém súborov a ukladanie údajov (File System Manager)
Úloha: 
Implementovať FATFS na prístup k SD karte a ukladanie nameraných údajov do CSV súboru.
Konkrétne úlohy:
Pripojenie SD karty k STM32 a konfigurácia FATFS pre prístup k súborom.
Implementácia funkcie na zápis údajov do CSV formátu napr. čas + údaje v hex formáte. ("možné zmeniť")
Overenie správneho formátovania a zabezpečenie, aby bol súbor prístupný  pre uživateľa 

##### Výstup: Kód na zapisovanie nameraných údajov na SD kartu v CSV formáte.<br/>
##### Vypracovateľka : Angelika Loduhová

### 4. Integrácia a testovanie (System Integrator and Tester)
Úloha:
Zabezpečiť, aby všetky časti projektu spolupracovali a celý systém bol stabilný. ("hlavne čo sa týka testovania projektu") 
Konkrétne úlohy:
Spojiť kód z jednotlivých častí senzorov, displej, SD karta a zabezpečiť ich vzájomnú komunikáciu.
Riešiť problémy s pamäťou, časovaním a ladením celého systému.
Vytvoriť testovacie scenáre pre overenie správnosti ukladania a zobrazovania údajov.

##### Výstup: Finálny integrovaný systém s dokumentáciou o testovaní a riešených problémoch.<br/>
##### Vypracovateľka : Dorottya Szarka 
