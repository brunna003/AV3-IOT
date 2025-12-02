#define LDR_PIN 34
#define LED_PIN 2
#define BUZZER_PIN 15

int ultimaLeitura = 0;

void setup(){
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  ultimaLeitura = analogRead(LDR_PIN);
}

void loop(){
  int leitura = analogRead(LDR_PIN);

  if (abs(leitura - ultimaLeitura) > 500) {
    digitalWrite(LED_PIN, HIGH);
    tone(BUZZER_PIN, 1200);
    delay(800);
  }

  digitalWrite(LED_PIN, LOW);
  noTone(BUZZER_PIN);

  ultimaLeitura = leitura;
  delay(200);
}
