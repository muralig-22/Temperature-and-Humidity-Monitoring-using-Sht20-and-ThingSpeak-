#include <WiFi.h>
#include "DFRobot_SHT20.h"
#include <LiquidCrystal_I2C.h>
#include <ThingSpeak.h>
// Wi-Fi credentials (replace as needed)
const char* WIFI_NAME = "Mm";
const char* WIFI_PASSWORD = "70107010";

// ThingSpeak credentials
const int myChannelNumber = 2646995; // Replace with your Channel Number
const char* myApiKey = "1EVJPW39YJ1ZVP01"; // Replace with your API key
const char* server = "api.thingspeak.com"; // 


DFRobot_SHT20 sht20(&Wire, SHT20_I2C_ADDR);
LiquidCrystal_I2C lcd(0x27, 16, 4);
WiFiClient client;

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Initializing...");


  // Connect to WiFi
  WiFi.begin(WIFI_NAME, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("CONNECTING...");
    Serial.println("WiFi not connected");
    delay(1000);  
    }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("CONNECTED...");
  Serial.println("WiFi connected!");
  Serial.println("Local IP: " + String(WiFi.localIP()));
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);
  sht20.initSHT20();
  delay(100);
  Serial.println("Sensor init finish!");
  sht20.checkSHT20();
  lcd.clear();
}

void loop() {
  // Read sensor values
  float humd = sht20.readHumidity();
  float temp = sht20.readTemperature();

  if (temp == 998.0 && humd == 998.0) {
    Serial.println("Sensor Error");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Sensor Error");
    delay(2000); 

  } else {
    // Send data to ThingSpeak
    ThingSpeak.setField(1, temp);
    ThingSpeak.setField(2, humd);
    int x = ThingSpeak.writeFields(myChannelNumber, myApiKey);

    if (x == 200) {
      Serial.println("Data pushed to ThingSpeak successfully");

    } else {
      Serial.println("ThingSpeak push error " + String(x));
    }

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print(temp, 1);
    lcd.print((char)223);
    lcd.print("C");
    lcd.setCursor(0, 1);
    lcd.print("Humidity: ");
    lcd.print(humd, 1);
    lcd.print("%");
    Serial.print("Temp: ");
    Serial.print(temp, 1);   
    Serial.print(" C\n");
    Serial.print("Humd: ");
    Serial.print(humd, 1);
    Serial.print("%\n");

  }

  delay(900000); 
}
