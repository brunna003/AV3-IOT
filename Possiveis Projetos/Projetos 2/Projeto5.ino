#include "DHT.h"

#define DHTPIN 4
#define DHTTYPE DHT11
#define LDR_PIN 34
#define LED_PIN 2
#define BTN_PIN 14

float maxTemp = 0;

DHT dht(DHTPIN, DHTTYPE);

void setup(){
  Serial.begin(115200);
  dht.begin();

  pinMode(LED_PIN, OUTPUT);
  pinMode(BTN_PIN, INPUT_PULLUP);
}

void loop(){
  float t = dht.readTemperature();
  int luz = analogRead(LDR_PIN);

  if (t > maxTemp) maxTemp = t;

  if (!digitalRead(BTN_PIN)) {
    maxTemp = t;
    delay(300);
  }

  if (t > 30) digitalWrite(LED_PIN, HIGH);
  else digitalWrite(LED_PIN, LOW);

  Serial.print("Temp: "); Serial.print(t);
  Serial.print("  Máx: "); Serial.print(maxTemp);
  Serial.print("  Luz: "); Serial.println(luz);

  delay(500);
}
