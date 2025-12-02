#define LDR_PIN 34
#define LED_PIN 2
#define BTN_PIN 14

bool modoAuto = true;

void setup(){
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  pinMode(BTN_PIN, INPUT_PULLUP);
}

void loop(){
  if (!digitalRead(BTN_PIN)) {
    modoAuto = !modoAuto;
    delay(300);
  }

  int luz = analogRead(LDR_PIN);

  if (modoAuto) {
    if (luz < 800) digitalWrite(LED_PIN, HIGH);
    else digitalWrite(LED_PIN, LOW);
  } else {
    digitalWrite(LED_PIN, LOW);
  }

  delay(100);
}
