#include <DHT.h>

// ===============================
// CONFIGURAÇÃO DE PINOS
// ===============================
#define DHTPIN 4          // Pino do DHT
#define DHTTYPE DHT11     // Ou DHT22
#define LED_PIN 2         // LED de alerta
#define BUZZER_PIN 5      // Buzzer ativo (digital)
#define BOTAO_PIN 14      // Botão para mutar alarme

// ===============================
// CONFIGURAÇÕES DO SISTEMA
// ===============================
float limiteTemperatura = 30.0; // Temperatura para disparar alarme
bool alarmeAtivo = true;        // Alarme começa ativo

DHT dht(DHTPIN, DHTTYPE);

// ===============================
// CONFIGURAÇÃO DA INTERRUPÇÃO
// ===============================
void IRAM_ATTR toggleAlarme() {
  alarmeAtivo = !alarmeAtivo;  // Liga/desliga o alarme
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(LED_PIN, LOW);
}

void setup() {
  Serial.begin(115200);
  dht.begin();

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(BOTAO_PIN, INPUT_PULLUP);

  // Interrupção no botão (flanco de descida)
  attachInterrupt(digitalPinToInterrupt(BOTAO_PIN), toggleAlarme, FALLING);

  Serial.println("Iniciando Estação Climática...");
}

void loop() {

  // ===============================
  // LEITURA DO SENSOR
  // ===============================
  float temperatura = dht.readTemperature();
  float umidade = dht.readHumidity();

  if (isnan(temperatura) || isnan(umidade)) {
    Serial.println("Falha ao ler o DHT!");
    return;
  }

  Serial.print("Temp: ");
  Serial.print(temperatura);
  Serial.print(" °C | Umidade: ");
  Serial.print(umidade);
  Serial.println(" %");

  // ===============================
  // LÓGICA DO ALARME
  // ===============================

  if (alarmeAtivo && temperatura >= limiteTemperatura) {
    digitalWrite(LED_PIN, HIGH);
    digitalWrite(BUZZER_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);
  }

  delay(1000);
}
