#include "DHT.h"

#define DHTPIN 4
#define DHTTYPE DHT11

#define LDR_PIN 34
#define LED_PIN 2
#define BUZZER_PIN 15

DHT dht(DHTPIN, DHTTYPE);

void setup(){
  Serial.begin(115200);
  dht.begin();

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
}

void loop(){
  float t = dht.readTemperature();
  int luz = analogRead(LDR_PIN);

  Serial.print("Temp: "); Serial.print(t);
  Serial.print("  Luz: "); Serial.println(luz);

  if (t > 30 || luz < 800) {
    digitalWrite(LED_PIN, HIGH);
    tone(BUZZER_PIN, 1000);
  } else {
    digitalWrite(LED_PIN, LOW);
    noTone(BUZZER_PIN);
  }

  delay(500);
}
