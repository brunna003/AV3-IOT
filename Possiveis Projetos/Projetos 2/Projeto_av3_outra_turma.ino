#include <DHT.h>
#include <Servo.h>

#define LDR_PIN A0
#define DHTPIN 5
#define DHTTYPE DHT11
#define BUTTON_PIN 3
#define SERVO_PIN 9

DHT dht(DHTPIN, DHTTYPE);
Servo servo;

int mode = 1;  // 0=OFF, 1=AUTO, 2=MANUAL
bool lastButtonState = HIGH;
unsigned long lastDebounce = 0;
const int debounceDelay = 80;

void setup() {
  Serial.begin(115200);
  
  pinMode(LDR_PIN, INPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  servo.attach(SERVO_PIN);
  dht.begin();

  servo.write(0); // posição inicial
}

void loop() {

  // ------------------ LEITURA DO BOTÃO COM DEBOUNCE -------------------
  bool reading = digitalRead(BUTTON_PIN);

  if (reading != lastButtonState) {
    lastDebounce = millis();
  }

  if ((millis() - lastDebounce) > debounceDelay) {
    if (reading == LOW && lastButtonState == HIGH) {
      mode++;
      if (mode > 2) mode = 0;
      Serial.print("Modo alterado para: ");
      Serial.println(mode);
    }
  }

  lastButtonState = reading;

  // ------------------ MONITORAMENTO DO AMBIENTE -------------------
  int luz = analogRead(LDR_PIN);
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  Serial.print("Luz: "); Serial.print(luz);
  Serial.print(" | TEMP: "); Serial.print(t);
  Serial.print(" | UMID: "); Serial.print(h);
  Serial.print(" | MODO: "); Serial.println(mode);

  // ------------------ LÓGICA DE CONTROLE DO SERVO -------------------
  if (mode == 0) {
    // DESLIGADO
    servo.write(0);
  }

  else if (mode == 1) {
    // MODO AUTOMÁTICO (abre se estiver escuro)
    if (luz < 400) {
      servo.write(90);   // abre a luz
    } else {
      servo.write(0);    // fecha
    }
  }

  else if (mode == 2) {
    // MODO MANUAL (posição fixa)
    servo.write(45);
  }

  delay(200);
}
