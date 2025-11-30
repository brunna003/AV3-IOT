// ===============================
// PINOS
// ===============================
#define LDR_PIN A0       // Pino analógico do LDR
#define LED_PIN 2        // LED (suporta analogWrite)
#define BOTAO_PIN 14     // Botão troca modo

// ===============================
// VARIÁVEIS DO SISTEMA
// ===============================
bool modoAutomatico = true;      
unsigned long debounce = 0;

// ===============================
// INTERRUPÇÃO DO BOTÃO
// ===============================
void IRAM_ATTR trocaModo() {
  unsigned long agora = millis();
  if (agora - debounce > 300) {  // Debounce
    modoAutomatico = !modoAutomatico;
    debounce = agora;
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  pinMode(BOTAO_PIN, INPUT_PULLUP);

  // Interrupção no botão
  attachInterrupt(digitalPinToInterrupt(BOTAO_PIN), trocaModo, FALLING);

  Serial.println("Sistema iniciado — Modo AUTOMÁTICO");
}

void loop() {

  // ===============================
  // LER LDR
  // ===============================
  int leitura = analogRead(LDR_PIN);   // 0–4095 no ESP32 / 0–1023 no Arduino

  Serial.print("LDR: ");
  Serial.println(leitura);

  // Ajusta a faixa automaticamente dependendo da placa
  #ifdef ESP32
      int brilho = map(leitura, 0, 4095, 255, 0);  // invertido
  #else
      int brilho = map(leitura, 0, 1023, 255, 0);  // invertido
  #endif

  brilho = constrain(brilho, 0, 255);

  // ===============================
  // MODO AUTOMÁTICO
  // ===============================
  if (modoAutomatico) {
    analogWrite(LED_PIN, brilho);
    Serial.print("Modo AUTO | Brilho: ");
    Serial.println(brilho);

  } else {
    // ===============================
    // MODO MANUAL
    // ===============================
    analogWrite(LED_PIN, 255);  // LED 100%
    Serial.println("Modo MANUAL | LED em 100%");
  }

  delay(200);
}
