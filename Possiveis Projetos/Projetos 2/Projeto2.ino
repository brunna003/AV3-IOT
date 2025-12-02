#include "DHT.h"

#define DHTPIN 4
#define DHTTYPE DHT11
#define LED_PIN 2
#define BUZZER_PIN 15
#define BTN_PIN 14

bool alarmeAtivo = true;

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(BTN_PIN, INPUT_PULLUP);
}

void loop() {
  float t = dht.readTemperature();

  if (!digitalRead(BTN_PIN)) alarmeAtivo = false;

  if (t > 30) {
    digitalWrite(LED_PIN, HIGH);
    if (alarmeAtivo) tone(BUZZER_PIN, 1200);
  } else {
    digitalWrite(LED_PIN, LOW);
    noTone(BUZZER_PIN);
    alarmeAtivo = true; // reseta para próxima vez
  }

  delay(300);
}
