#define LDR_PIN 34
#define BUZZER_PIN 15
#define LED_PIN 2
#define BTN_PIN 14

bool alarmeLigado = false;

void setup(){
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(BTN_PIN, INPUT_PULLUP);
}

void loop(){
  if (!digitalRead(BTN_PIN)) {
    alarmeLigado = !alarmeLigado;
    delay(300);
  }

  int luz = analogRead(LDR_PIN);

  if (alarmeLigado && luz < 500) {
    digitalWrite(LED_PIN, HIGH);
    tone(BUZZER_PIN, 1500);
  } else {
    digitalWrite(LED_PIN, LOW);
    noTone(BUZZER_PIN);
  }

  delay(150);
}
