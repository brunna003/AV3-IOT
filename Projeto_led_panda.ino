//Liga um led, apaga pelo botão, e controla a intensidade com um potenciometro


// ----------------------
// Configurações
// ----------------------
int ledPin = 2;         // LED comum
int buttonPin = 3;      // Botão
int potPin = 4;         // Potenciômetro (ADC)

// Estado do LED
volatile bool ledOn = true;
int brilho = 128;        // Brilho inicial

// Debounce
volatile unsigned long lastInterruptTime = 0;

// ----------------------
// Interrupção do botão
// ----------------------
void IRAM_ATTR toggleLED() {
  unsigned long currentTime = millis();

  if (currentTime - lastInterruptTime > 200) {  // debounce
    ledOn = !ledOn;
    lastInterruptTime = currentTime;
  }
}

// ----------------------
// PWM manual (bit-bang)
// ----------------------
void pwmWrite(int pin, int value) {
  if (!ledOn) {
    digitalWrite(pin, LOW);
    delayMicroseconds(255);
    return;
  }

  if (value <= 0) {
    digitalWrite(pin, LOW);
    delayMicroseconds(255);
    return;
  }

  if (value >= 255) {
    digitalWrite(pin, HIGH);
    delayMicroseconds(255);
    return;
  }

  digitalWrite(pin, HIGH);
  delayMicroseconds(value);

  digitalWrite(pin, LOW);
  delayMicroseconds(255 - value);
}

// ----------------------
// Setup
// ----------------------
void setup() {
  Serial.begin(115200);

  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(buttonPin), toggleLED, FALLING);

  analogReadResolution(10); // 0–1023
}

// ----------------------
// Loop principal
// ----------------------
void loop() {

  // Lê potenciômetro 0–1023 e converte para 0–255
  int potValue = analogRead(potPin);
  brilho = map(potValue, 0, 900, 0, 255);

  Serial.print("Brilho: ");
  Serial.println(brilho);

  // Gera PWM
  pwmWrite(ledPin, brilho);
}
