/*
  Projeto: Porta Automática para Estufa com Sistema de Parada

  Componentes:
    - LDR (sensor de luz)
    - DHT11 (temperatura e umidade)
    - Servo SG90 (abre/fecha a porta)
    - Botão 1: alterna modos (Desligado / Automático / Manual)
    - Botão 2: interrupção externa para PARADA TOTAL do sistema

  Funcionamento:
    - Em modo automático, a porta abre quando há luz suficiente e fecha quando escurece.
    - O botão de modo permite alternar entre funcionamento automático, manual ou desligado.
    - O botão de parada aciona uma interrupção externa que desativa o servo e trava o sistema
      permanentemente até que o microcontrolador seja resetado.
*/


#include <DHT.h>
#include <Servo.h>

#define LDR_PIN A0
#define DHTPIN 5
#define DHTTYPE DHT11

#define BUTTON_MODE 3        // alternar modos
#define BUTTON_STOP 4        // interrupção externa

#define SERVO_PIN 9

DHT dht(DHTPIN, DHTTYPE);
Servo servo;

volatile bool pararSistema = false;

int mode = 1;  
bool lastButtonState = HIGH;
unsigned long lastDebounce = 0;
const int debounceDelay = 80;

// ---------- INTERRUPÇÃO ----------
void IRAM_ATTR parar() {
  pararSistema = true;
}

void setup() {
  Serial.begin(115200);

  pinMode(LDR_PIN, INPUT);
  pinMode(BUTTON_MODE, INPUT_PULLUP);
  pinMode(BUTTON_STOP, INPUT_PULLUP);

  servo.attach(SERVO_PIN);
  dht.begin();

  attachInterrupt(digitalPinToInterrupt(BUTTON_STOP), parar, FALLING);

  servo.write(0);
}

void loop() {

  // ---------- SE O BOTÃO STOP FOI PRESSIONADO ----------
  if (pararSistema) {
    Serial.println("⚠ Sistema PARADO pela interrupção externa!");
    servo.detach();   // desliga o servo
    while (true) {
      // Travamento eterno até resetar fisicamente o ESP
    }
  }

  // ---------- DEBOUNCE DO BOTÃO DE MODO ----------
  bool reading = digitalRead(BUTTON_MODE);

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

  // ---------- LEITURAS ----------
  int luz = analogRead(LDR_PIN);
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  Serial.print("Luz: "); Serial.print(luz);
  Serial.print(" | Temp: "); Serial.print(t);
  Serial.print(" | Umid: "); Serial.print(h);
  Serial.print(" | Modo: "); Serial.println(mode);

  // ---------- LÓGICA DO SISTEMA ----------
  if (mode == 0) {
    servo.write(0);   // fechado
  }

  else if (mode == 1) {  // AUTOMÁTICO
    if (luz > 500) {
      servo.write(90);   // abre porta
    } else {
      servo.write(0);    // fecha porta
    }
  }

  else if (mode == 2) {  // MANUAL
    servo.write(45);
  }

  delay(200);
}
