// ------------------- PINOS -------------------
const int LED_PIN = 2;
const int LDR_PIN = 34;

const int BTN_LED = 27;
const int BTN_SYS = 14;

// ------------------- ESTADOS -------------------
volatile bool sistemaLigado = true;
volatile bool ledLigado = false;

// Debounce por software
volatile unsigned long lastLedInterrupt = 0;
volatile unsigned long lastSysInterrupt = 0;
const unsigned long debounceDelay = 200;  // ms

// ------------------- ISR: BOTÃO DO LED -------------------
void IRAM_ATTR isrBotaoLed() {
  unsigned long agora = millis();
  if (agora - lastLedInterrupt > debounceDelay) {
    ledLigado = !ledLigado;
    lastLedInterrupt = agora;
  }
}

// ------------------- ISR: BOTÃO DO SISTEMA -------------------
void IRAM_ATTR isrBotaoSistema() {
  unsigned long agora = millis();
  if (agora - lastSysInterrupt > debounceDelay) {
    sistemaLigado = !sistemaLigado;  // Liga/desliga todo o sistema
    lastSysInterrupt = agora;
  }
}

// ------------------- SETUP -------------------
void setup() {
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  pinMode(BTN_LED, INPUT_PULLUP);
  pinMode(BTN_SYS, INPUT_PULLUP);

  // Ativar interrupções
  attachInterrupt(digitalPinToInterrupt(BTN_LED), isrBotaoLed, FALLING);
  attachInterrupt(digitalPinToInterrupt(BTN_SYS), isrBotaoSistema, FALLING);
}

// ------------------- LOOP PRINCIPAL -------------------
void loop() {

  // ----- SISTEMA DESLIGADO -----
  if (!sistemaLigado) {
    digitalWrite(LED_PIN, LOW);
    delay(50);           // pequeno descanso, não trava
    return;              // NÃO processa mais nada
  }

  // ----- CONTROLAR LED -----
  digitalWrite(LED_PIN, ledLigado ? HIGH : LOW);

  // ----- LEITURA DO LDR -----
  int ldr = analogRead(LDR_PIN);
  Serial.print("LDR: ");
  Serial.println(ldr);

  delay(200);
}
