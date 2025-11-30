#define RED_PIN     12
#define GREEN_PIN    5
#define BLUE_PIN     4
#define BUTTON_PIN   2     // botão para GND (INPUT_PULLUP)
#define LDR_PIN     34     // LDR (fotoresistor)
#define POT_PIN     35     // Potenciômetro
#define NTC_PIN     32     // Termistor NTC
#define BUZZER_PIN  14     // Buzzer passivo

// Valor nominal escolhido para luminosidade mínima aceitável
#define LUMINOSIDADE_LIMIAR 800

// Variáveis de controle do LED (botão)
int lastButtonState = HIGH; 
int currentButtonState;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;
bool ledEnabled = true;

// Variáveis de controle do buzzer
bool buzzerMute = false;
int lastBuzzerButtonState = HIGH; 
int currentBuzzerButtonState;
unsigned long lastBuzzerDebounceTime = 0;

// ================== SETUP ==================
void setup() {
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP); 
  pinMode(BUZZER_PIN, OUTPUT);

  Serial.begin(115200);
  Serial.println("Sistema iniciado...");
}

// ================== LOOP ==================
void loop() {
  // Lê temperatura pelo NTC
  lerTemperaturaNTC();

  // Lê luminosidade e decide se ativa LED
  verificaLuminosidade();

  int reading = digitalRead(BUTTON_PIN);
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != currentButtonState) {
      currentButtonState = reading;
      if (currentButtonState == LOW) {
        ledEnabled = !ledEnabled;
        Serial.print("LED: ");
        Serial.println(ledEnabled ? "Ligado" : "Desligado");
      }
    }
  }
  lastButtonState = reading;

  // --- Controle do LED RGB pelo potenciômetro ---
  if (ledEnabled) {
    int potValue = analogRead(POT_PIN);
    int section = map(potValue, 0, 4095, 0, 6);

    switch(section) {
      case 0: digitalWrite(RED_PIN, HIGH); digitalWrite(GREEN_PIN, LOW); digitalWrite(BLUE_PIN, LOW); break;
      case 1: digitalWrite(RED_PIN, HIGH); digitalWrite(GREEN_PIN, HIGH); digitalWrite(BLUE_PIN, LOW); break;
      case 2: digitalWrite(RED_PIN, HIGH); digitalWrite(GREEN_PIN, HIGH); digitalWrite(BLUE_PIN, LOW); break;
      case 3: digitalWrite(RED_PIN, HIGH); digitalWrite(GREEN_PIN, HIGH); digitalWrite(BLUE_PIN, HIGH); break;
      case 4: digitalWrite(RED_PIN, LOW);  digitalWrite(GREEN_PIN, HIGH); digitalWrite(BLUE_PIN, HIGH); break;
      case 5: digitalWrite(RED_PIN, LOW);  digitalWrite(GREEN_PIN, LOW);  digitalWrite(BLUE_PIN, HIGH); break;
      case 6: digitalWrite(RED_PIN, LOW);  digitalWrite(GREEN_PIN, LOW);  digitalWrite(BLUE_PIN, HIGH); break;
    }

    Serial.print("Pot: "); Serial.print(potValue);
    Serial.print(" | Section: "); Serial.print(section);
    switch(section) {
      case 0: Serial.println(" - Vermelho"); break;
      case 1: Serial.println(" - Laranja"); break;
      case 2: Serial.println(" - Amarelo"); break;
      case 3: Serial.println(" - Branco"); break;
      case 4: Serial.println(" - Azul claro"); break;
      case 5: Serial.println(" - Azul"); break;
      case 6: Serial.println(" - Azul escuro"); break;
    }
  } else {
    digitalWrite(RED_PIN, LOW);
    digitalWrite(GREEN_PIN, LOW);
    digitalWrite(BLUE_PIN, LOW);
  }

  delay(100);
}

// ================== FUNÇÃO NTC ==================
void lerTemperaturaNTC() {
  int valorNTC = analogRead(NTC_PIN);


  float tensao = valorNTC * (3.3 / 4095.0);
  float resistencia = (10000 * tensao) / (3.3 - tensao); 
  float temperatura = 1 / (0.001129148 + (0.000234125 * log(resistencia)) + (0.0000000876741 * pow(log(resistencia), 3))) - 273.15;

  Serial.print("Temperatura (NTC): ");
  Serial.print(temperatura);
  Serial.println(" °C");

  controleBuzzer(temperatura);
}

// ================== FUNÇÃO BUZZER ==================
void controleBuzzer(float temp) {
  verificaBotaoBuzzer();

  if (!buzzerMute && temp > 30.0) {
    tone(BUZZER_PIN, 1000); // Som contínuo
    Serial.println("ALERTA! Temperatura alta.");
  } else {
    noTone(BUZZER_PIN);
  }
}


// ================== BOTÃO MUTE ==================
void verificaBotaoBuzzer() {
  int reading = digitalRead(BUTTON_PIN);
  if (reading != lastBuzzerButtonState) {
    lastBuzzerDebounceTime = millis();
  }
  if ((millis() - lastBuzzerDebounceTime) > debounceDelay) {
    if (reading != currentBuzzerButtonState) {
      currentBuzzerButtonState = reading;
      if (currentBuzzerButtonState == LOW) {
        buzzerMute = !buzzerMute;
        Serial.print("Mute do buzzer: ");
        Serial.println(buzzerMute ? "Ativado" : "Desativado");
      }
    }
  }
  lastBuzzerButtonState = reading;
}

// ================== FUNÇÃO FOTORESISTOR ==================
void verificaLuminosidade() {
  int ldrValue = analogRead(LDR_PIN);

  Serial.print("Luminosidade: ");
  Serial.println(ldrValue);

  if (ldrValue > LUMINOSIDADE_LIMIAR) {
    ledEnabled = true;  
    Serial.println("Ambiente escuro - LED RGB ligado automaticamente");
  }
  else {
    ledEnabled = false;
    Serial.println("Ambiente Claro");
  }
}
