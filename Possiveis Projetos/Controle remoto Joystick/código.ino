// ===============================
// PINOS DO JOYSTICK
// ===============================
#define JOY_X A0     // Eixo X do joystick
#define JOY_Y A3     // Eixo Y do joystick
#define JOY_BTN 15   // Botão do joystick

// ===============================
// FEEDBACKS
// ===============================
#define LED_PIN 2
#define BUZZER_PIN 5

// ===============================
// VARIÁVEIS DO SISTEMA
// ===============================
int xValue, yValue;
int btnValue;

bool mudou = false;   // Para detectar mudança e tocar buzzer

void setup() {
  Serial.begin(115200);

  pinMode(JOY_BTN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  Serial.println("Sistema de Controle IoT iniciado.");
}

void loop() {

  // ===============================
  // LEITURAS DO JOYSTICK
  // ===============================
  xValue = analogRead(JOY_X);  // 0–4095 no ESP32 / 0–1023 no Arduino
  yValue = analogRead(JOY_Y);
  btnValue = digitalRead(JOY_BTN);  // LOW quando pressionado

  Serial.print("X: ");
  Serial.print(xValue);
  Serial.print(" | Y: ");
  Serial.print(yValue);
  Serial.print(" | Botão: ");
  Serial.println(btnValue == LOW ? "Pressionado" : "Solto");

  // Detecta se houve mudança significativa
  static int lastX = 0;
  static int lastY = 0;

  mudou = (abs(xValue - lastX) > 50) || (abs(yValue - lastY) > 50);

  lastX = xValue;
  lastY = yValue;

  // ===============================
  // FEEDBACK COM LED
  // ===============================
  if (mudou) {
    digitalWrite(LED_PIN, HIGH);   // Pisca LED
    delay(30);
    digitalWrite(LED_PIN, LOW);
  }

  // ===============================
  // FEEDBACK COM BUZZER
  // ===============================
  if (btnValue == LOW) {           // Botão do joystick apertado
    tone(BUZZER_PIN, 1500, 120);   // beep curto
  } 
  else if (mudou) {
    tone(BUZZER_PIN, 800, 30);     // beep curto quando mover
  }

  delay(120);
}
