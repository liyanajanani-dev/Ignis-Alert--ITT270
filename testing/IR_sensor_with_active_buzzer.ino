const int sensorPin = 4; // Pin connected to sensor OUT
const int buzzerPin = 23; // Pin connected to buzzer +

void setup() {
  pinMode(sensorPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  int sensorState = digitalRead(sensorPin);

  // Active Low logic: LOW means object detected
  if (sensorState == LOW) {
    
    //Serial.println("Object Detected!");
    digitalWrite(buzzerPin, HIGH); // For active buzzer
    delay(150);
    digitalWrite(buzzerPin, LOW);
    delay(150);
    // If passive buzzer, use: tone(buzzerPin, 2000);
  } else {
    digitalWrite(buzzerPin, LOW);
    // If passive buzzer, use: noTone(buzzerPin);
  }
  delay(100);
}