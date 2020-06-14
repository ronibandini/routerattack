/*
Router Attack - ESP8266 micro game 
Roni Bandini @RoniBandini
June 2020
*/
#include "ESP8266WiFi.h"
#include <Arduino.h>
#include <U8g2lib.h>
#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

const int potPin = A0; 
const int buttonPin = 12; // D6
const int buzzerPin = 13; // D7

int buttonValue = 1;
int potValue=0;
int mappedPot=0;
int selectedChannel=0;
int selectedRSSI=0;
int currentChannel=0;
int currentRSSI=0;
float z=0;
int score=0;
int attempts=0;
int runMinutes=0;


U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ 5, /* data=*/ 4, /* reset=*/ 16);

static const unsigned char logo[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x7C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0xC0, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x7C, 0xF0, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0xD0, 0x1F, 0x02, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x06, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xEF, 0x02, 
  0xF8, 0xC3, 0xE3, 0xFE, 0xFF, 0xF7, 0x03, 0xFF, 0xFF, 0xFF, 0xC3, 0xE7, 
  0x19, 0xF0, 0xFF, 0x03, 0xF8, 0xE7, 0xE7, 0xFE, 0xFF, 0xF7, 0x07, 0xFF, 
  0xFF, 0xFF, 0xE3, 0xEF, 0x1D, 0x38, 0xFF, 0x03, 0xF8, 0xF7, 0xEF, 0xFE, 
  0xFF, 0xF7, 0x07, 0xFF, 0xFF, 0xFF, 0xF3, 0xEF, 0x1D, 0x9C, 0xFF, 0x03, 
  0x78, 0xF7, 0xEF, 0x9E, 0xE7, 0x70, 0x0F, 0x1F, 0x87, 0xE3, 0xF7, 0xEE, 
  0x0F, 0xEC, 0xFF, 0x03, 0x78, 0xF7, 0xEE, 0x9E, 0xE7, 0x70, 0x0F, 0x1F, 
  0x87, 0xE3, 0xF7, 0xEE, 0x0F, 0xE6, 0xFF, 0x01, 0x78, 0xF7, 0xEE, 0x9E, 
  0xE7, 0x70, 0x8F, 0x3B, 0x87, 0xE3, 0xF7, 0xEE, 0x0F, 0xF2, 0xFF, 0x03, 
  0xF8, 0xF7, 0xEE, 0x9E, 0xE7, 0xF7, 0x87, 0x3B, 0x87, 0x63, 0xF7, 0xEE, 
  0x07, 0xFB, 0xFF, 0x03, 0xF8, 0xF3, 0xEE, 0x9E, 0xE7, 0xF7, 0x83, 0x3B, 
  0x87, 0x73, 0xF7, 0xE0, 0x07, 0xFB, 0xFF, 0x03, 0xF8, 0xF7, 0xEE, 0x9E, 
  0xE7, 0xF7, 0x87, 0x3B, 0x87, 0x73, 0xF7, 0xE0, 0x07, 0xFF, 0xFF, 0x03, 
  0x78, 0xF7, 0xEE, 0x9E, 0xE7, 0x70, 0x8F, 0x3B, 0x87, 0x73, 0xF6, 0xEE, 
  0x0F, 0xFF, 0xFF, 0x03, 0x78, 0xF7, 0xEE, 0x9E, 0xE7, 0x70, 0x8F, 0x3F, 
  0x87, 0xF3, 0xFF, 0xEE, 0x0F, 0xFF, 0xFF, 0x03, 0x78, 0xF7, 0xEE, 0x9E, 
  0xE7, 0x70, 0x8F, 0x3F, 0x87, 0xF3, 0xFF, 0xEE, 0x0F, 0xFE, 0xFF, 0x03, 
  0x78, 0xF7, 0xEF, 0x9F, 0xE7, 0x70, 0x8F, 0x7F, 0x87, 0xF3, 0xFF, 0xEE, 
  0x1F, 0xFE, 0xFF, 0x01, 0x78, 0xF7, 0xEF, 0x9F, 0xE7, 0x77, 0xCF, 0x71, 
  0x87, 0x73, 0xFE, 0xEF, 0x1D, 0xFC, 0xFF, 0x01, 0x78, 0xE7, 0xC7, 0x8F, 
  0xE7, 0x77, 0xCF, 0x73, 0x87, 0x73, 0xEE, 0xEF, 0x1D, 0xFC, 0xFF, 0x00, 
  0x78, 0xC7, 0x83, 0x87, 0xE7, 0x77, 0xCF, 0x73, 0x87, 0x7B, 0xCE, 0xE3, 
  0x3D, 0xF8, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x0F, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, };

void setup() {

  Serial.begin(115200);
  Serial.println("Started...");

  pinMode(potPin, INPUT);  
  pinMode(buttonPin,INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);

  randomSeed(analogRead(4));

  tone(buzzerPin, 700); 
  delay(200);
  noTone(buzzerPin);
  
  u8g2.begin();  
  u8g2.clearBuffer();               
  u8g2.drawXBM(0,0, 128, 32, logo);
  u8g2.sendBuffer();          
  delay(3000);
  
  u8g2.clearBuffer();   
  u8g2.setFont(u8g2_font_t0_11_tf);  
  u8g2.drawStr(0,10,"Router Attack");  
  u8g2.drawStr(0,20,"Roni Bandini");   
  u8g2.drawStr(0,30,"iwlist wlan0 scan");   
  u8g2.sendBuffer();           
  delay(10000);
    
  // Set WiFi to station mode
  WiFi.mode(WIFI_STA);
  //WiFi.disconnect();
  delay(100);

  
}

void loop() {

  unsigned long runMillis= millis();
  unsigned long allSeconds=millis()/1000;
  int runHours= allSeconds/3600;
  int secsRemaining=allSeconds%3600;
  runMinutes=secsRemaining/60;
  int runSeconds=secsRemaining%60;

  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  Serial.println("Scan done");
  
  if (n == 0) {
    Serial.println("No networks found");
  } else {
    Serial.print(n);
    Serial.println(" networks found");
    
    
    for (int i = 0; i < n; ++i) {
      
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");

      u8g2.clearBuffer();    
      u8g2.drawStr(0,12,"Select target");                    
      u8g2.drawStr(0,22,WiFi.SSID(i).c_str());     

     int tolerance=0;

                  // size according to encryption
                  if (String(WiFi.encryptionType(i))=="2"){
                  tolerance=2;  
                  }
                 if (String(WiFi.encryptionType(i))=="4"){
                  tolerance=3;   
                  } 
                 if (String(WiFi.encryptionType(i))=="5"){
                  tolerance=4; 
                  }    
          
                 if (String(WiFi.encryptionType(i))=="7"){
                  tolerance=8;   
                  }  
          
                 if (String(WiFi.encryptionType(i))=="8"){
                   tolerance=6;    
                  }  
                     

        u8g2.drawStr(0,32,"Tolerance");        
        u8g2.drawStr(58,32,String(tolerance).c_str());        
                                         
                            
      u8g2.sendBuffer();                     
        
        buttonValue = digitalRead(buttonPin);        
            
        if (buttonValue == 0) {

                  beep1();
                           
                  // store selected SSID data
                  currentChannel  =WiFi.channel(i);
                  currentRSSI     =WiFi.RSSI(i);
                  
                  Serial.println("Target selected"); 
                  
                  attempts++;                           

                  Serial.println("Current RSSI");
                  Serial.println(currentRSSI);
                  Serial.println("Current Ch");
                  Serial.println(currentChannel);

                  u8g2.clearBuffer();       
                  u8g2.drawStr(0,12,WiFi.SSID(i).c_str());                  
                  u8g2.drawStr(0,22,"Scan location");                                                                        
                  u8g2.sendBuffer();  
                  delay(1000);
                        
                  // Display coordinates screen
                  u8g2.clearBuffer();                           
                  
                 printLines();
                                  
                  // draw target                                                                                                                                              
                  
                  int forScreenX  =currentRSSI*-1;                                      
                  int forScreenY  =32-currentChannel*2;
                  
                  // fit in screen
                  if (forScreenX<3){forScreenX=3;}   
                  if (forScreenY<3){forScreenY=3;}   
                  if (forScreenX>125){forScreenX=120;}   
                  if (forScreenY>28){forScreenY=25;} 
                       
                  u8g2.drawCircle(forScreenX, forScreenY, tolerance);   
                                                                                                                                  
                  u8g2.sendBuffer(); 

                  delay(5000);
                                    
                  // force entering loop
                  buttonValue=1;           
                         
                  while (buttonValue == 1) {         

                        potValue    = analogRead(potPin);
                        mappedPot   = map(potValue, 0, 1023, -120, 0);
        
                        u8g2.clearBuffer();                                                 
                        u8g2.drawStr(0,12,"Target: ");             
                        u8g2.drawStr(40,12,WiFi.SSID(i).c_str());  
                        u8g2.drawStr(0,22,"Select");  
                        u8g2.drawStr(0,32,"RSSI:");  
                        u8g2.drawStr(40,32,String(mappedPot).c_str());                                                                     
                        u8g2.sendBuffer();  
                  
                        delay(500);                                                           
                        buttonValue = digitalRead(buttonPin);          
                  } // end while

                  
                  beep1();
                  selectedRSSI=mappedPot;
                  
                  Serial.println("RSSI/X selected");

                  buttonValue=1;            
                  
                  while (buttonValue == 1) { 

                        potValue    = analogRead(potPin);
                        mappedPot   = map(potValue, 0, 1023, 1, 14);
        
                        u8g2.clearBuffer();                        
                        u8g2.drawStr(0,12,"Target: ");             
                        u8g2.drawStr(40,12,WiFi.SSID(i).c_str());  
                        u8g2.drawStr(0,22,"Select");                          
                        u8g2.drawStr(0,32,"Channel:");                                                                 
                        u8g2.drawStr(50,32,String(mappedPot).c_str());                                         
                        u8g2.sendBuffer();  
                        delay(500);
                                                                                
                        buttonValue = digitalRead(buttonPin);                           
                        
                  }
                  
                  beep1();                               
                  
                  selectedChannel= mappedPot;

                  Serial.println("Channel Y selected");

                  buttonValue=1; 
                         
                  while (buttonValue == 1) {                                                
                        
                        // Print target location
                        u8g2.clearBuffer(); 

                        // print 
                        printLines();
                                                              
                        u8g2.drawCircle(forScreenX, forScreenY,tolerance);   
                        u8g2.sendBuffer(); 
                        delay(1000);

                        // Shooting animation
                        
                        for (int x=1; x <=tolerance ; x++){                                                                                                    
                                                                                                                                                                   
                          u8g2.clearBuffer();
                                                    
                          printLines();
                                                                                                                                
                          u8g2.drawCircle(forScreenX, forScreenY, tolerance);                                                                                                                                            
                          
                          int forScreenSelectedX  =selectedRSSI*-1;                                      
                          int forScreenSelectedY  =32-selectedChannel*2;
                  
                          u8g2.drawCircle(forScreenSelectedX, forScreenSelectedY, 1); 
                          u8g2.drawCircle(forScreenSelectedX, forScreenSelectedY, x); 
                                                      
                          u8g2.sendBuffer(); 
                          delay(10);                                                
                       }                 

                        delay(1000);   
                        u8g2.clearBuffer();                               
                        u8g2.drawStr(0,12,"Target: ");             
                        u8g2.drawStr(40,12,WiFi.SSID(i).c_str());  
                        u8g2.drawStr(0,22,"Fire distance");     

                        // calculate distance between target and fire
                        z=sqrt( (currentRSSI-selectedRSSI)*(currentRSSI-selectedRSSI)+ (currentChannel-selectedChannel)*(currentChannel-selectedChannel) );                                             
                        
                        Serial.println("Z");
                        Serial.println(z);                        
                        
                        u8g2.drawStr(0,32,String(z).c_str());  

                        
                                                               
                        if (z<tolerance){
                           u8g2.drawStr(30,32,"-> destroyed");  
                           score++;                                                
                          }
                          else
                          {
                            u8g2.drawStr(30,32,"-> missed");                                                  
                            }
                                                    
                        u8g2.sendBuffer();  
                                                                                                        
                        buttonValue=0;

                        delay(3000);  

                        u8g2.clearBuffer();                                   
                        u8g2.drawStr(0,10,WiFi.SSID(i).c_str());           
                        u8g2.drawStr(0,22,String(WiFi.RSSI(i)).c_str());  
                        u8g2.drawStr(20,22,"% Channel ");   
                        u8g2.drawStr(80,22,String(WiFi.channel(i)).c_str());  
                                                
                         if (String(WiFi.encryptionType(i))=="2"){
                          u8g2.drawStr(0,32,"TKIP");   
                          }
                         if (String(WiFi.encryptionType(i))=="4"){
                          u8g2.drawStr(0,32,"CCMP");   
                          } 
                         if (String(WiFi.encryptionType(i))=="5"){
                          u8g2.drawStr(0,32,"WEP");  
                          }    
                  
                         if (String(WiFi.encryptionType(i))=="7"){
                          u8g2.drawStr(0,32,"Open");  
                          }  
                  
                         if (String(WiFi.encryptionType(i))=="8"){
                          u8g2.drawStr(0,32,"Auto");  
                          }  
                          u8g2.sendBuffer();  
     
                         delay(2500);  

                         printScoreInfo(); 
                         delay(2500);                                
                        
                  }

                  Serial.println("Exit");                                   
                   
                  
        } // end if

      // pause between target iteration
      delay(2000);
        
    }
  }
  Serial.println(""); 
       
  // Wait a bit before scanning again   
  delay(5000);
}

static void getSSID(char *addr, uint8_t* data) {
  int offset=0;
  sprintf(addr, "%c%c%c%c%c", data[offset+0], data[offset+1], data[offset+2], data[offset+3], data[offset+4]);  
}

void beep1(){
  tone(buzzerPin, 700); 
  delay(200);
  noTone(buzzerPin);
  }

void printLines(){
  // horizontal line
  u8g2.drawLine(1, 31, 128, 31);
  // vertical line
  u8g2.drawLine(1, 0, 1, 32);
  
  // horizontal scales
  for (int i=1; i <= 128; i=i+10){
    u8g2.drawLine(i, 28, i, 32);
   }
  
   // vertical scales
  for (int i=1; i <= 32; i=i+10){
    u8g2.drawLine(1, i, 4, i);
   }

   //u8g2.drawStr(32,128,"RSSI"); 
   //u8g2.drawStr(0,0,"CH"); 
  }

void printScoreInfo(){

  u8g2.clearBuffer();                          
  u8g2.setFont(u8g2_font_t0_11_tf);               
  u8g2.drawStr(0,12,"Running:");  
  u8g2.drawStr(50,12,String(runMinutes).c_str());  
  u8g2.drawStr(58,12," min");  
  u8g2.drawStr(0,22,"Attempts:");  
  u8g2.drawStr(55,22,String(attempts).c_str()); 
  u8g2.drawStr(0,32,"Score:");  
  u8g2.drawStr(40,32,String(score).c_str());  
  u8g2.sendBuffer();   
      
    }
