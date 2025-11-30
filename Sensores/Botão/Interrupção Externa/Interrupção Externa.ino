#define Pino_DHT  x
#define Tipo_DHT  DHT11
#define buzzer    y   // PINO DO BUZZER (substituir y pelo pino real)

DHT Nome_SensorDHT(Pino_DHT, Tipo_DHT);

void setup() {
  Serial.begin(115200);
  Nome_SensorDHT.begin();
  pinMode(buzzer, OUTPUT);
}

void loop() {

  float Nome_variavel_temperatura = Nome_SensorDHT.readTemperature();
  float Nome_variavel_umidade     = Nome_SensorDHT.readHumidity();

  Serial.print("Temp: ");
  Serial.print(Nome_variavel_temperatura);
  Serial.print("  Umidade: ");
  Serial.println(Nome_variavel_umidade);

  // --------------------------------------------------------
  //   ALERTA DE TEMPERATURA
  // --------------------------------------------------------
  if (Nome_variavel_temperatura > 30 || Nome_variavel_temperatura < 0) {
    
    Serial.println("⚠ TEMPERATURA CRÍTICA! BUZZER ATIVADO!");

    // Bipe intermitente
    tone(buzzer, 1000);  // apita a 1000 Hz
    delay(200);
    noTone(buzzer);
    delay(200);

  } else {
    // Temperatura normal → buzzer fica desligado
    noTone(buzzer);
  }

  delay(1000);
}
