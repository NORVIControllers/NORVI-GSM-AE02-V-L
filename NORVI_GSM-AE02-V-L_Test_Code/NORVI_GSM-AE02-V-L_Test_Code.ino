
/*
 * SC-G2-V-G Bottom
 * 
 * OLED
 * PUSH BUTTON -3
 * GSM
 * DIGITAL INPUTS -8
 * TRANSISTER OUTPUT -2
 * Analog Inputs  0-10V -4
 * RS485 
 */


#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_ADS1X15.h>

#define ANALOG_PIN_0 36

#define INPUT1 27
#define INPUT2 34
#define INPUT3 35
#define INPUT4 14
#define INPUT5 13
#define INPUT6 5
#define INPUT7 15
#define INPUT8 19

#define OUTPUT1 12
#define OUTPUT2 2

#define RS485_RX 25
#define RS485_TX 26
#define RS485_FC 22

#define GSM_RX 33
#define GSM_TX 32
#define GSM_RESET 21

#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


Adafruit_ADS1115 ads1;
#define VOLTAGE_DIVIDER_RATIO 0.2065

int analog_value = 0;
int readSwitch(){
  analog_value = analogRead(ANALOG_PIN_0);
  return analog_value  ; 
}

unsigned long int timer1 = 0;


void setup() {
 
  Serial.begin(115200);

  Serial.println("Hello");
  
  Serial1.begin(9600, SERIAL_8N1, RS485_RX, RS485_TX); 
  Serial2.begin(115200, SERIAL_8N1, GSM_RX, GSM_TX); 
  
  pinMode(RS485_FC, OUTPUT);
  digitalWrite(RS485_FC, HIGH);   // RS-485
  
  pinMode(GSM_RESET, OUTPUT);
  digitalWrite(GSM_RESET, HIGH);   // RS-485 

  pinMode(OUTPUT1, OUTPUT);
  pinMode(OUTPUT2, OUTPUT);

  pinMode(INPUT1, INPUT);
  pinMode(INPUT2, INPUT);
  pinMode(INPUT3, INPUT);
  pinMode(INPUT4, INPUT);
  pinMode(INPUT5, INPUT);
  pinMode(INPUT6, INPUT);
  pinMode(INPUT7, INPUT);
  pinMode(INPUT8, INPUT);
  
  Wire.begin(16,17);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.display();

  if (!ads1.begin(0x48)) {
    Serial.println("Failed to initialize ADS 1 .");
    while (1);
  }

  WiFi.mode(WIFI_MODE_APSTA);
  delay(300);
  wifi_test();
  

  Serial.println("Testing Modem");
  

 timer1 = millis();
  Serial2.println("AT");
  while(millis()<timer1+5000){
    while (Serial2.available()) {
    int inByte = Serial2.read();
    Serial.write(inByte);
    }
  }

  timer1 = millis();
  Serial2.println("AT+CPIN?");
  while(millis()<timer1+2000){
    while (Serial2.available()) {
    int inByte = Serial2.read();
    Serial.write(inByte);
    }
  }
  timer1 = millis();
  Serial2.println("AT+GSN");
  while(millis()<timer1+2000){
    while (Serial2.available()) {
    int inByte = Serial2.read();
    Serial.write(inByte);
    }
  }

  timer1 = millis();
  Serial2.println("AT+CSQ");
  while(millis()<timer1+2000){
    while (Serial2.available()) {
    int inByte = Serial2.read();
    Serial.write(inByte);
    }
  }

  Serial.println("Testing Modem Done");

  digitalWrite(RS485_FC, HIGH);   

}


void loop() {
  //GSM
  while (Serial.available()) {
    int inByte = Serial.read();
    Serial2.write(inByte);
  }

  while (Serial2.available()) {
    int inByte = Serial2.read();
    Serial.write(inByte);
  }
  
//PUSH BUTOON
  Serial.print("Push button : ");
  Serial.println(readSwitch());
  Serial.println(""); 

  //DIGITAL INPUT
  
  Serial.print(digitalRead(INPUT1));
  Serial.print(digitalRead(INPUT2));
  Serial.print(digitalRead(INPUT3));
  Serial.print(digitalRead(INPUT4));
  Serial.print(digitalRead(INPUT5));
  Serial.print(digitalRead(INPUT6));
  Serial.print(digitalRead(INPUT7));
  Serial.print(digitalRead(INPUT8));
  Serial.println(""); 

  //ADS VOLTAGE
  
  for (int i = 0; i < 4; i++) {
    int16_t adc = ads1.readADC_SingleEnded(i);
    float voltage = adc * 0.125 / 1000.0 / VOLTAGE_DIVIDER_RATIO;
    Serial.printf("V %d: %.2f V\n", i, voltage);
  }
  
  // OUTPUT
  
  digitalWrite(OUTPUT1, HIGH);
  digitalWrite(OUTPUT2, LOW);
  delay(200);
  digitalWrite(OUTPUT1, LOW);
  digitalWrite(OUTPUT2, HIGH);
  delay(200);

  //MODE BUS 
  
  digitalWrite(RS485_FC, HIGH);                   
  delay(100);
  Serial1.println(F("RS485 01 SUCCESS"));   
  delay(100);                                
  digitalWrite(RS485_FC, LOW) ;                  

                                           
  delay(100);     
  
  while (Serial1.available()) {  
    char c = Serial1.read();     
    Serial.write(c);             
  }
 
  Serial.println("-------------------------------");
  delay(500);
 
}

void wifi_test(){
    Serial.println("");
String str_macAddress;
byte mac[6];

WiFi.macAddress(mac);

   str_macAddress = (String(mac[0] >> 4,  HEX) + String(mac[0] & 0x0F, HEX)) + (":") +
                   (String(mac[1] >> 4,  HEX) + String(mac[1] & 0x0F, HEX)) + (":") +
                   (String(mac[2] >> 4,  HEX) + String(mac[2] & 0x0F, HEX)) + (":") +
                   (String(mac[3] >> 4,  HEX) + String(mac[3] & 0x0F, HEX)) + (":") +
                   (String(mac[4] >> 4,  HEX) + String(mac[4] & 0x0F, HEX)) + (":") +
                   (String(mac[5] >> 4,  HEX) + String(mac[5] & 0x0F, HEX));
  str_macAddress.toUpperCase();
  
  Serial.println("MAC Address: " + str_macAddress);

  String ssid =  str_macAddress; 
  const char* password = "12345678";   

  Serial.println("Setting up the Wi-Fi Access Point...");
  WiFi.softAP(ssid.c_str(), password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("Access Point IP address: ");
  Serial.println(IP);

  Serial.print("Access Point SSID: ");
  Serial.println(ssid);

  Serial.println("Wi-Fi Access Point is active!");
  Serial.println("");
}
