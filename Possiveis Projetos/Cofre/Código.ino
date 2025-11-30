// ===============================
// PINOS
// ===============================
#define LDR_PIN A0          // Entrada analógica do LDR
#define LED_PIN 2           // LED de alerta
#define BUZZER_PIN 5        // Buzzer
#define BOTAO_PIN 14        // Botão de reset

// ===============================
// VARIÁVEIS DO SISTEMA
// ===============================
bool alarmeDisparado = false;
int limiteLuz = 800;  // Ajuste conforme o ambiente (0–4095 no ESP32)

// Debounce da interrupção
unsigned long debounce = 0;

// ===============================
// INTERRUPÇÃO DO BOTÃO
// ===============================
void IRAM_ATTR resetAlarme() {
  unsigned long agora = millis();

  if (agora - debounce > 300) {  
    alarmeDisparado = false;

    digitalWrite(LED_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);

    debounce = agora;
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(BOTAO_PIN, INPUT_PULLUP);

  // Interrupção
  attachInterrupt(digitalPinToInterrupt(BOTAO_PIN), resetAlarme, FALLING);

  Serial.println("Cofre IoT iniciado. Aguardando abertura...");
}

void loop() {

  // ===============================
  // LER LDR
  // ===============================
  int luz = analogRead(LDR_PIN);  

  Serial.print("LDR: ");
  Serial.println(luz);

  // ===============================
  // DETECTAR ABERTURA DO COFRE
  // ===============================
  if (!alarmeDisparado && luz > limiteLuz) {
    alarmeDisparado = true;
    Serial.println("⚠️ COFRE ABERTO! DISPARANDO ALARME!");
  }

  // ===============================
  // ALARME ATIVO
  // ===============================
  if (alarmeDisparado) {

    // LED piscando
    digitalWrite(LED_PIN, HIGH);
    tone(BUZZER_PIN, 1500);  
    delay(150);

    digitalWrite(LED_PIN, LOW);
    tone(BUZZER_PIN, 800);
    delay(150);

  } else {
    // Sistema em standby
    digitalWrite(LED_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);
  }

  delay(100);
}
