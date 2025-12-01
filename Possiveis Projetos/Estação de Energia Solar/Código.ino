// ===============================
// PINOS
// ===============================
#define LDR_PIN A0            // Intensidade de luz (simula sol)
#define POT_PIN A3            // Consumo/bateria
#define LED_PIN 2             // LED indica geração

// ===============================
// VARIÁVEIS
// ===============================
int luz;          // 0–4095 ESP32 / 0–1023 Arduino
int consumo;      // 0–4095 ou 0–1023
int geracao;      // brilho do LED (0–255)

void setup() {
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);

  Serial.println("Mini Estação Solar IoT iniciada.");
}

void loop() {

  // ===============================
  // LEITURAS
  // ===============================
  luz = analogRead(LDR_PIN);
  consumo = analogRead(POT_PIN);

  // Converter luz em brilho do LED
  #ifdef ESP32
      geracao = map(luz, 0, 4095, 0, 255);
  #else
      geracao = map(luz, 0, 1023, 0, 255);
  #endif

  geracao = constrain(geracao, 0, 255);

  // LED indica geração solar
  analogWrite(LED_PIN, geracao);

  // ===============================
  // EXIBIÇÃO NO SERIAL
  // ===============================
  Serial.print("Sol (LDR): ");
  Serial.print(luz);
  Serial.print(" | Consumo (Pot): ");
  Serial.print(consumo);
  Serial.print(" | Geração (0-255): ");
  Serial.println(geracao);

  // ===============================
  // LÓGICA DE "BALANÇO ENERGÉTICO"
  // ===============================

  if (consumo > luz) {
    Serial.println("⚠️ Consumo maior que geração! Bateria descarregando...");
  } else {
    Serial.println("✔ Sistema equilibrado (energia positiva).");
  }

  delay(400);
}
