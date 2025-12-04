/*
  Projeto: Sistema de Controle com Pausa via Interrupção Externa

  Descrição:
    Este projeto utiliza um LDR, um sensor DHT11, um servo SG90 e dois botões.
    O botão MODE alterna entre três modos de operação:
        0 = Desligado
        1 = Automático (controle baseado na luminosidade)
        2 = Manual (posição fixa do servo)
        
    O botão STOP aciona uma interrupção externa que coloca o sistema em modo PAUSADO.
    Nesse estado:
        - Todas as leituras (LDR e DHT11) são interrompidas
        - O servo permanece em posição fixa
        - O sistema aguarda até que o botão MODE seja pressionado novamente
        
    Após sair da pausa, o sistema volta a operar normalmente no modo selecionado.
*/

 

#include <DHT.h>
#include <Servo.h>

#define LDR_PIN A0
#define DHTPIN 5
#define DHTTYPE DHT11

#define BUTTON_MODE 3        
#define BUTTON_STOP 4        // interrupção externa

#define SERVO_PIN 9

DHT dht(DHTPIN, DHTTYPE);
Servo servo;

volatile bool pausado = false;

int mode = 1;  
bool lastButtonState = HIGH;
unsigned long lastDebounce = 0;
const int debounceDelay = 80;

// ---------- INTERRUPÇÃO EXTERNA ----------
void IRAM_ATTR pausarSistema() {
  pausado = true;
}

void setup() {
  Serial.begin(115200);

  pinMode(LDR_PIN, INPUT);
  pinMode(BUTTON_MODE, INPUT_PULLUP);
  pinMode(BUTTON_STOP, INPUT_PULLUP);

  servo.attach(SERVO_PIN);
  dht.begin();

  // Interrupção no botão STOP
  attachInterrupt(digitalPinToInterrupt(BUTTON_STOP), pausarSistema, FALLING);

  servo.write(0); // posição inicial
}

void loop() {

  // -------------------- SE PAUSADO --------------------
  if (pausado) {
    Serial.println("⚠ SISTEMA PAUSADO – leituras e controle desativados.");

    // servo parado em posição fixa
    servo.write(0);

    // só sai da pausa quando o usuário apertar o botão MODE
    bool reading = digitalRead(BUTTON_MODE);

    if (reading == LOW) {
      pausado = false;    // volta ao normal
      delay(300);
      Serial.println("▶ Sistema retomado!");
    }

    delay(200);
    return; // sai do loop aqui
  }

  // -------------------- BOTÃO DE MODO (DEBOUNCE) --------------------
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

  // -------------------- LEITURAS --------------------
  int luz = analogRead(LDR_PIN);
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  Serial.print("Luz: "); Serial.print(luz);
  Serial.print(" | Temp: "); Serial.print(t);
  Serial.print(" | Umid: "); Serial.print(h);
  Serial.print(" | Modo: "); Serial.println(mode);

  // -------------------- LÓGICA DOS MODOS --------------------
  if (mode == 0) {
    servo.write(0);
  }

  else if (mode == 1) {  // AUTOMÁTICO
    if (luz > 500) servo.write(90);
    else servo.write(0);
  }

  else if (mode == 2) {  // MANUAL
    servo.write(45);
  }

  delay(200);
}
