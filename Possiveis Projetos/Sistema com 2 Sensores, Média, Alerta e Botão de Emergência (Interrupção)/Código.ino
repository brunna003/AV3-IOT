// =============================
// PROJETO 6 – ESP32
// Dois sensores analógicos → média
// Buzzer dispara se média ultrapassar limite
// Botão com interrupção externa trava o sistema
// =============================

// ---- PINOS ----
#define SENSOR1 34
#define SENSOR2 35
#define BUZZER  18
#define BOTAO   4   // interrupção externa

// ---- VARIÁVEIS ----
volatile bool pararSistema = false;  // setado pela interrupção
int limite = 2000;   // valor limite da média (ajuste conforme sensor)

// ---- INTERRUPÇÃO ----
void IRAM_ATTR pararTudo() {
  pararSistema = true;
}

void setup() {
  Serial.begin(115200);

  pinMode(SENSOR1, INPUT);
  pinMode(SENSOR2, INPUT);

  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, LOW);

  pinMode(BOTAO, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BOTAO), pararTudo, FALLING);

  Serial.println("PROJETO 6 iniciado...");
}

void loop() {

  // SE O BOTÃO FOI PRESSIONADO → TRAVA TUDO
  if (pararSistema) {
    digitalWrite(BUZZER, LOW);
    Serial.println("⚠ Sistema PARADO pelo botão! Reinicie o ESP.");
    while (true) {
      // travado para sempre até RESET
      delay(1000);
    }
  }

  // ---- LEITURA DOS SENSORES ----
  int leitura1 = analogRead(SENSOR1);
  int leitura2 = analogRead(SENSOR2);

  // ---- CÁLCULO DA MÉDIA ----
  int media = (leitura1 + leitura2) / 2;

  // ---- DEBUG ----
  Serial.print("Sensor1: "); Serial.print(leitura1);
  Serial.print(" | Sensor2: "); Serial.print(leitura2);
  Serial.print(" | Media: "); Serial.println(media);

  // ---- LÓGICA DO BUZZER ----
  if (media > limite) {
    digitalWrite(BUZZER, HIGH);
  } else {
    digitalWrite(BUZZER, LOW);
  }

  delay(500);
}
