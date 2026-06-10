// Pin Definitions
#define fire_sensor 4
#define MQ2_AOUT 18
#define MQ2_DOUT 19
#define BUZZER_PIN 23
#define RELAY_PIN 22

// Thresholds
const int gasThreshold = 1200;    

void setup() {
  Serial.begin(115200);
  
  // Initialize Sensors & Outputs
  pinMode(fire_sensor, INPUT);
  pinMode(MQ2_DOUT, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);

  // Default states
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(RELAY_PIN, LOW); 
  
  Serial.println("System Initializing 2026 (Gas & Fire Only)...");
}

void loop() {
  // 1. Read Sensor Data
  int analogGas = analogRead(MQ2_AOUT);
  int digitalGas = digitalRead(MQ2_DOUT);
  int fireDetected = digitalRead(fire_sensor); // 0 = Fire (Active-Low)

  // 2. Serial Debugging
  Serial.print("Gas Level: "); 
  Serial.print(analogGas); 
  Serial.print(" | ");
  Serial.print("Fire Status: "); 
  Serial.println(fireDetected == LOW ? "!!! ALERT !!!" : "Clear");

  // 3. Buzzer & Relay Logic
  // Triggers if Gas is high OR Fire is detected
  if (digitalGas == LOW || analogGas > gasThreshold || fireDetected == LOW) {
    digitalWrite(BUZZER_PIN, HIGH);
    
    // Relay triggers on Fire (e.g., to start a pump/fan)
    if (fireDetected == LOW) {
      digitalWrite(RELAY_PIN, HIGH);
      Serial.println("CRITICAL: FIRE DETECTED - RELAY ACTIVE");
    } else {
      Serial.println("WARNING: GAS DETECTED");
    }
  } else {
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(RELAY_PIN, LOW);
  }

  delay(500); // Faster response time without DHT sensor latency
}
