// --- Blynk Cloud Configuration (Must match your Web Console Template) ---
#define BLYNK_TEMPLATE_ID "TMPL6LK3Os5Vb"      // Unique ID for your Blynk project template
#define BLYNK_TEMPLATE_NAME "ignis prototype"   // Name of your project template
#define BLYNK_AUTH_TOKEN "I4Yc2km1E2KARx-QQWbCWFQUR_S9WYwl" // Security token for this specific device

// --- Libraries ---
#include <WiFi.h>              // Library to enable ESP32 WiFi capabilities
#include <BlynkSimpleEsp32.h>  // Library to connect ESP32 to Blynk Cloud
#include <DHT.h>               // Library for DHT11/DHT22 Temperature & Humidity sensors

// --- Network Settings ---
char auth[] = BLYNK_AUTH_TOKEN; // Assigns your token to a variable for the connection function
char ssid[] = "vivo Y18";    // WiFi Name (Keep "Wokwi-GUEST" for simulator, change for real use)
char pass[] = "yanani06";               // WiFi Password (Leave empty for Wokwi, add your password for real use)

// --- Hardware Pin Assignments ---
//#define IR_FIRE 27
#define DHTPIN 4               // DHT sensor connected to Digital Pin 4
#define DHTTYPE DHT11          // Defines the sensor type (Change to DHT11 if using the blue sensor)
#define MQ2_AOUT 33            // MQ2 Analog output to Pin 33 (ADC1) to avoid WiFi interference
#define MQ2_DOUT 19            // MQ2 Digital output to Pin 13
#define BUZZER_PIN 23          // Positive leg of Buzzer to Pin 21
#define RELAY_PIN 22           // Signal pin of Relay to Pin 18

// --- Customizable Logic Thresholds ---
const int gasThreshold = 1200;    // Gas level trigger (Sensitivity). Range: 0 (Clean) to 4095 (Smoke)
const float tempThreshold = 40.0; // Temperature in Celsius to trigger the Relay (e.g., Fan/AC)
const float humiThreshold = 30.0;// Early ignition (30% - 50%)
//const int flameThreshold = 1;     // Digital Detaction

// --- Object Initialization ---
DHT dht(DHTPIN, DHTTYPE);      // Creates the DHT sensor object
BlynkTimer timer;              // Creates a timer to run functions without using delay()

// --- Main Function: Reads Data & Updates Blynk ---
void sendData() {
  //int flame = digitalRead(IR_FIRE);  // Read flame appearance (0=noFire, 1=Fire)
  float h = dht.readHumidity();      // Read humidity from sensor
  float t = dht.readTemperature();   // Read temperature from sensor
  int analogGas = analogRead(MQ2_AOUT); // Read smoke/gas density (analog value)
  int digitalGas = digitalRead(MQ2_DOUT); // Read gas detection (binary 0 or 1)

  // Error check: If sensor is unplugged or broken, stop the function
  if (isnan(h) || isnan(t)) {
    Serial.println("DHT Sensor error!");
    return;
  }

  // --- Update Blynk Dashboard Widgets ---
  //Blynk.virtualWrite(V0, flame);
  Blynk.virtualWrite(V0, t);         // Update Temperature gauge on Virtual Pin V0
  Blynk.virtualWrite(V1, h);         // Update Humidity gauge on Virtual Pin V1
  Blynk.virtualWrite(V2, analogGas); // Update Gas Level display on Virtual Pin V2

  // --- Emergency Logic: Gas/Smoke Detection ---
  if (digitalGas == LOW || analogGas > gasThreshold) { // If gas detected via digital or analog limit
    digitalWrite(BUZZER_PIN, HIGH);                    // Turn on physical buzzer
    Blynk.logEvent("gas_alert", "Gas Detected!");      // Send Push Notification to phone
  } else {
    digitalWrite(BUZZER_PIN, LOW);                     // Turn off buzzer when air is clean
  }

  // --- Automation Logic: Temperature Control ---
  if (t > tempThreshold) {           // If temperature exceeds your limit
    digitalWrite(RELAY_PIN, HIGH);   // Turn on the Relay (starts Fan/Cooler)
    Blynk.virtualWrite(V3, 1);       // Light up the LED widget in Blynk app
  } else {
    digitalWrite(RELAY_PIN, LOW);    // Turn off the Relay
    Blynk.virtualWrite(V3, 0);       // Turn off the LED widget in Blynk app
  }
}

// --- Setup Function: Runs once on startup ---
void setup() {
  Serial.begin(115200);              // Start Serial Monitor at 115200 speed for debugging
  
  Blynk.begin(auth, ssid, pass);     // Connect to WiFi and Blynk Server

  
  // Initialize Sensors & Outputs
  //pinMode(IR_FIRE, INPUT);
  dht.begin();                       // Initialize the DHT sensor
  pinMode(MQ2_DOUT, INPUT);          // Set Gas digital pin as input
  pinMode(BUZZER_PIN, OUTPUT);       // Set Buzzer pin as output
  pinMode(RELAY_PIN, OUTPUT);        // Set Relay pin as output

  // Default states
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(RELAY_PIN, LOW);

  // Schedule the 'sendData' function to run every 2000ms (2 seconds)
  timer.setInterval(2000L, sendData);
  
  Serial.println("System Initializing 2026..."); // Confirmation message
}

// --- Loop Function: Runs continuously ---
void loop() {
  Blynk.run();                       // Maintains connection to Blynk Cloud
  timer.run();                       // Keeps the internal timer running
}
