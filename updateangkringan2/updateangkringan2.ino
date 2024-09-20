#include <Arduino.h>
#include <WiFiManager.h>
#include <LiquidCrystal_I2C.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <DHT.h>
#include <FirebaseESP32.h>
#include <ESP32_Servo.h>

#define FIREBASE_HOST "iot-angkringan-default-rtdb.asia-southeast1.firebasedatabase.app"
#define FIREBASE_AUTH "P0jMkStl4tann1ntU5GMNSeTy83LueCVQ97lU5DE"

FirebaseData firebaseData;
FirebaseJson json;
String prefix = "users/KQSMP3qTe7OkJxhdQm4fSp5C1m22/";
unsigned long lastFirebaseSendTime = 0;
const unsigned long firebaseSendInterval = 1000;

#define RelayPin 12
#define RelayButtonPin 26
#define PageButtonPin 15
#define Sensorpin 32
#define ACS712pin 33
#define LDR_PIN 34
#define rainPin 35
#define gasSensorPin 39
#define ServoPin 13
#define DHTPin 4
#define DHTTYPE DHT11
DHT dht(DHTPin, DHTTYPE);
Servo myservo;

bool relayStatus = false;

int buttonState;
int lastButtonState = LOW;
int currentPage = 1;
int LDRValue;
int gasValue;
int rainAnalog;

float V_offset_actual = 2.35;
float voltageScaleFactor = 12.0 / 9.35;

float lastTemperature = 0.0;
float lastHumidity = 0.0;

unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;
unsigned long lastVoltageTime = 0;
unsigned long voltageInterval = 1000;
unsigned long lastDHTTime = 0;
unsigned long DHTInterval = 2000;
unsigned long lastPredictionTime = 0;
const unsigned long predictionInterval = 6000;
unsigned long lastGasReadTime = 0;
const unsigned long gasReadInterval = 1000;


LiquidCrystal_I2C lcd(0x27, 16, 2);  
byte wifi[] = {
  B00000,
  B00000,
  B00000,
  B00111,
  B01000,
  B10011,
  B10100,
  B10101
};
byte blank[] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};
byte temp[] = {
  B00100,
  B01010,
  B01010,
  B01110,
  B01110,
  B11111,
  B11111,
  B01110
};
byte humd[] = {
  B00100,
  B00100,
  B01010,
  B01010,
  B10001,
  B10001,
  B10001,
  B01110
};
byte lamp[] = {
  B00000,
  B01110,
  B10001,
  B10001,
  B10001,
  B01110,
  B01110,
  B00100
};
byte curr[] = {
  B00000,
  B00100,
  B01110,
  B11111,
  B00100,
  B00100,
  B00100,
  B00000
};
byte volt[] = {
  B00010,
  B00110,
  B01100,
  B11000,
  B11110,
  B00110,
  B01100,
  B11000
};

byte pwrs[] = {
  B01111,
  B11111,
  B11000,
  B11110,
  B11110,
  B11000,
  B11111,
  B01111
};
const long utcOffsetInSeconds = 25200;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

void setup() {
  Serial.begin(115200);
  
  dht.begin();
  myservo.attach(ServoPin);
  
  pinMode(RelayPin, OUTPUT);
  pinMode(RelayButtonPin, INPUT_PULLUP);
  pinMode(PageButtonPin, INPUT_PULLUP);
  pinMode(gasSensorPin, INPUT);
  pinMode(LDR_PIN, INPUT);
  pinMode(rainPin, INPUT);
  
  lcd.begin();        
  lcd.backlight();   
  lcd.createChar(0, wifi);
  lcd.createChar(1, blank);
  lcd.createChar(2, temp);
  lcd.createChar(3, humd);
  lcd.createChar(4, lamp);
  lcd.createChar(5, curr);
  lcd.createChar(6, volt);
  lcd.createChar(7, pwrs);

//------------------------------
  WiFiManager wm;
  bool res;
  res = wm.autoConnect("angkringan 5.0", "tumbasisek");
  if (!res) {
    Serial.println("Failed to connect");
    // ESP.restart();
  } else {
    Serial.println("Connected...yeey :)");
  }

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);


//-----------------------------  
  timeClient.begin();
  timeClient.update();

}

void loop() {
  unsigned long currentMillis = millis();
  
//-------------ganti halaman lcd dengan tombol-----------    
  int pageButtonReading = digitalRead(PageButtonPin);

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (pageButtonReading != lastButtonState) {
      lastButtonState = pageButtonReading;

      if (pageButtonReading == LOW) {
        currentPage = (currentPage == 3) ? 1 : (currentPage + 1); // Geser halaman antara 1, 2, dan 3
      }

      lastDebounceTime = millis();
    }
  }
  
//-----------status relay------------
  String pathRelay = prefix + "/lampu_led";
  if (Firebase.getBool(firebaseData, pathRelay)) {
    relayStatus = firebaseData.boolData();
    digitalWrite(RelayPin, relayStatus);
  //  Serial.println("Kondisi lampu: " + String(relayStatus));
  }
  
//------------status icon wifi dan lampu---------
  lcd.setCursor(15, 1);
  if (WiFi.status() == WL_CONNECTED) {
    lcd.write(byte(0));
  } else {
    lcd.write(byte(1));
  }

  lcd.setCursor(15, 0);
  lcd.write(relayStatus ? byte(4) : byte(1));
  
//----------voltage, current, power---------------
  float voltage, current, power; 
  if (currentMillis - lastVoltageTime >= voltageInterval) {
    lastVoltageTime = currentMillis;
    calculateVoltageCurrentPower(voltage, current, power);
  }

//-----------Temperature,danHumidity--------
  float newTemperature = dht.readTemperature();
  float newHumidity = dht.readHumidity();

  if (!isnan(newTemperature) && !isnan(newHumidity)) {
    if (newTemperature != lastTemperature || newHumidity != lastHumidity) {
      lastTemperature = newTemperature;
      lastHumidity = newHumidity;
    }
  }
//--------------LDR-------------------------
  LDRValue = analogRead(LDR_PIN);
  LDRValue = 4095 - LDRValue;

//------------rain--------------
  rainAnalog = analogRead(rainPin);
  
//------------prediksi hujan----------------
  if (currentMillis - lastPredictionTime >= predictionInterval) {
  
    PrediksiHujan(lastHumidity, lastTemperature, LDRValue);
    
    lastPredictionTime = currentMillis;
  }
  
//----------gasmq2-------------
  gasValue = analogRead(gasSensorPin);  
    
//--------------updateToFirebase------------
      if (millis() - lastFirebaseSendTime >= firebaseSendInterval) {
        FirebaseUpdate(lastTemperature, lastHumidity, current, voltage, power, LDRValue, gasValue, rainAnalog);
        lastFirebaseSendTime = millis();   
  }

//---------pergantian halaman lcd---------
  if (currentPage == 1) {
    Page1();
  } else if (currentPage == 2) {
    Page2(voltage, current, power); 
  } else if (currentPage == 3) {
    Page3();
  }


//----------servo get dari firebase--------------
  String pathServo = prefix + "/servo";
    if (Firebase.getInt(firebaseData, pathServo)) {
      int angle = firebaseData.intData();
      myservo.write(angle);
  //    Serial.println("Posisi Servo: " + String(angle));
    } else {
  //    Serial.println("Failed to read servo angle from Firebase");
    }
}
    
