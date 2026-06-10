#define BLYNK_TEMPLATE_ID "TMPL6zmv5IIlz"
#define BLYNK_TEMPLATE_NAME "Ignis Full"
#define BLYNK_AUTH_TOKEN "bqi7pw680CS5gxv6EmSnga65w037kSoi"

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// BLYNK CREDENTIALS
const char* ssid = "vivo Y18";
const char* pass = "yanani06";

BlynkTimer timer;

// PIN DEFINITIONS
#define DHTPIN 4     
#define MQ2PIN 34
#define FIRE_PIN 5
#define MOTION_PIN 13
#define SOUND_PIN 14
#define RELAY_PUMP 18    // PIN FOR WATER PUMP RELAY
#define BUZZER_PIN 32
#define LED_RED 17
#define LED_YELLOW 27
#define LED_GREEN 15

// BLYNK VIRTUAL PINS
#define V_TEMP V0
#define V_SMOKE V1
#define V_FIRE V2
#define V_MOTION V3
#define V_SOUND V4
#define V_PUMP V5
#define V_RED V6
#define V_YELLOW V7
#define V_GREEN V8
#define V_BUZZER V9

// SENSOR SETTINGS
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);

// THRESHOLDS
int smokeThreshold = 250; 
float tempThreshold = 25.0;

void sendSensorData(){
  float temp = dht.readTemperature();
  int smokeLevel = analogRead(MQ2PIN);
  int fireStatus; // LOW typically means fire
  if (digitalRead(FIRE_PIN)==LOW){
    fireStatus =1;
  }
  else{
    fireStatus=0; //pin HIGH, area SAFE
  }
  int motionStatus = digitalRead(MOTION_PIN);
  int soundStatus = digitalRead(SOUND_PIN);
  int buzzerStatus = digitalRead(BUZZER_PIN);

  Blynk.virtualWrite(V_TEMP, temp);
  Blynk.virtualWrite(V_SMOKE, smokeLevel);
  Blynk.virtualWrite(V_FIRE, fireStatus);
  if (motionStatus == HIGH){
    Blynk.virtualWrite(V_MOTION,255);
  }
  else{
    Blynk.virtualWrite(V_MOTION,0);
  }
  Blynk.virtualWrite(V_SOUND, soundStatus);
  Blynk.virtualWrite(V_BUZZER, buzzerStatus);
}


#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); // Disable brownout detector
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  dht.begin();
  lcd.init();
  lcd.backlight();

  pinMode(FIRE_PIN, INPUT);
  pinMode(MOTION_PIN, INPUT);
  pinMode(SOUND_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(RELAY_PUMP, OUTPUT); // Relay set as Output
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);

  // Default state: All OFF (Note: some relays are Active LOW)
  digitalWrite(RELAY_PUMP, HIGH); 
  digitalWrite(BUZZER_PIN, LOW);

  timer.setInterval(2000L, sendSensorData);
}

void loop() {
  Blynk.run();
  timer.run();

  float temp = dht.readTemperature();
  int smokeLevel = analogRead(MQ2PIN);
  bool fireDetected = (digitalRead(FIRE_PIN) == LOW); // LOW typically means fire
  bool motionDetected = (digitalRead(MOTION_PIN) == HIGH);
  bool soundDetected = (digitalRead(SOUND_PIN) == HIGH);


  // SITUATION 3: CRITICAL ALERT (ALL SENSORS HIGH)
  if (temp > tempThreshold && smokeLevel > smokeThreshold && fireDetected && motionDetected && soundDetected) {
    lcd.clear();
    lcd.setCursor(0, 0); lcd.print("CRITICAL! PumpON");
    lcd.setCursor(0, 1); lcd.print("FIRE+MOV+SOUND");

    Blynk.virtualWrite(V_GREEN, 0);
    Blynk.virtualWrite(V_YELLOW, 0);
    Blynk.virtualWrite(V_RED, 255);

    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_YELLOW, LOW);
    digitalWrite(LED_RED, HIGH);

    digitalWrite(BUZZER_PIN, HIGH);
    Blynk.virtualWrite(V_BUZZER, 255);

    Blynk.virtualWrite(V_MOTION,255);

    digitalWrite(RELAY_PUMP, HIGH); // WATER PUMP ACTIVATED
    Blynk.virtualWrite(V_PUMP, 255);
  }

  // SITUATION 2: EMERGENCY (FIRE DETECTED)
  else if (temp > tempThreshold && fireDetected) {
    lcd.clear();
    lcd.setCursor(0, 0); lcd.print("----EMERGENCY!----  ");
    lcd.setCursor(0, 1); lcd.print("  FIRE DETECTED   ");

    Blynk.virtualWrite(V_GREEN, 0);
    Blynk.virtualWrite(V_YELLOW, 0);
    Blynk.virtualWrite(V_RED, 255);

    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_YELLOW, LOW);
    digitalWrite(LED_RED, HIGH);

    digitalWrite(BUZZER_PIN, HIGH);
    Blynk.virtualWrite(V_BUZZER, 255);

    digitalWrite(RELAY_PUMP, LOW);
    // Pump remains OFF here
    Blynk.virtualWrite(V_PUMP, 0);
  }

  // SITUATION 1: WARNING (TEMP + SMOKE)
  else if (temp > tempThreshold && smokeLevel > smokeThreshold) { 
    lcd.setCursor(0, 0); lcd.print("----WARNING!------");
    lcd.setCursor(0, 1); lcd.print("  SMOKE & HEAT    ");

    Blynk.virtualWrite(V_GREEN, 0);
    Blynk.virtualWrite(V_YELLOW, 255);
    Blynk.virtualWrite(V_RED, 0);

    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_YELLOW, HIGH);
    digitalWrite(LED_RED, LOW);

    digitalWrite(BUZZER_PIN, HIGH);
    Blynk.virtualWrite(V_BUZZER, 255);

    digitalWrite(RELAY_PUMP, LOW);
    Blynk.virtualWrite(V_PUMP, 0);
  }

  // SAFE STATE
  else {
    lcd.setCursor(0, 0); 
    lcd.print("System: Secure  ");
    lcd.setCursor(0, 1); 
    lcd.print("T:"); 
    lcd.print(temp); 
    lcd.print("C S:"); lcd.print(smokeLevel);
    lcd.print("    ");

    
    Blynk.virtualWrite(V_GREEN, 255);
    Blynk.virtualWrite(V_YELLOW, 0);
    Blynk.virtualWrite(V_RED, 0);

    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_YELLOW, LOW);
    digitalWrite(LED_RED, LOW);

    digitalWrite(BUZZER_PIN, LOW);
    Blynk.virtualWrite(V_BUZZER, 0);

    digitalWrite(RELAY_PUMP, LOW);
    Blynk.virtualWrite(V_PUMP, 0);
  }

  delay(500); 
}