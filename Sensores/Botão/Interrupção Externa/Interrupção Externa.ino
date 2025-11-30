const int LED = 16;
const int BUTTON = 5;

volatile bool interrupted = false;
bool ledStopped = false;
unsigned long lastInterruptTime = 0;
const unsigned long debounceDelay = 200; // Tempo de debounce em milissegundos

void IRAM_ATTR ISR() {
  // Verifica se passou tempo suficiente desde a última interrupção
  if (millis() - lastInterruptTime > debounceDelay) {
    interrupted = true;
    lastInterruptTime = millis();
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  pinMode(BUTTON, INPUT);
  attachInterrupt(digitalPinToInterrupt(BUTTON), ISR, FALLING);
}

void loop() {
  if (interrupted) {
    interrupted = false;
    
    if (!ledStopped) {
      // Primeiro pressionamento - para o LED
      ledStopped = true;
      digitalWrite(LED, LOW);
      Serial.println("LED interrompido! Pressione o botão novamente para retomar.");
    } else {
      // Segundo pressionamento - retoma o LED
      ledStopped = false;
      Serial.println("LED retomado!");
    }
  }

  if (!ledStopped) {
    // Comportamento normal do LED (piscando)
    digitalWrite(LED, HIGH);
    delay(500);
    digitalWrite(LED, LOW);
    delay(500);
  }
}
