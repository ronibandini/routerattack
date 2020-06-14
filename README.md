# routerattack
ESP8266 Arduino micro game using nearby WIFI routers as enemies 

In a drawer I’ve found a Heltec WiFi Kit 8, which is a small ESP8266 with integrated 128x32 Oled screen and I’ve started to think what to do with it for the Tech at home contest.

I’ve came out with the idea of a small WiFi game named Router Attack, where nearby routers will populate the enemies. Each enemy is placed in a sort of grid with coordinates obtained from RSSI and Channel. Router encryption type is used as the tolerance to the fire. Example: if the router is open, it will be easier to take it down, even if your coordinates are not precise. If you are using AES-TKIP you will need accuracy to aim. 

So, the game starts when you select a Router SSID from the list. Then a graphic shows the approximate location. Based on that graph you have to enter coordinates. After that, a shooting animation shows target versus fire, total attempts and score. A final screen shows a kind of router sheet information with SSID, Channel, RSSI and Encryption.

I’ve added a battery from an old cell phone which last forever and I’ve designed a small case front to fit an Altoids can.  

The idea is pretty simple but when you play it, it turns out kind of fun. 

# Circuits

Oled screen is already integrated so, you only have to connect a button to GND and D6, a potentiometer to GND, 3v and A0, and a Buzzer to GND and D7. 

If you are going yo use a battery, you can directly plugin to the on board battery charger. Since I don´t have the required battery connector, I’ve decided to use an external TP charger and connect regular pins.

# Distance calculation

The formula used to calculate the distance between fire coordinates and target is the following:

z=sqrt( (currentRSSI-selectedRSSI)*(currentRSSI-selectedRSSI)+ (currentChannel-selectedChannel)*(currentChannel-selectedChannel) );  

# Where to go from here?           

EEProm storage of score and targets, statistics of routers by encryption type. You can even create a WiFi chart analyze tool with the same coding that the game has. 
