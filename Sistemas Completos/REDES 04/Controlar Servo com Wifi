#include <WiFi.h>
#include <PubSubClient.h>

#define HOR 32
#define VER 35
#define BUTTON 14

#define GREEN_LED 17
#define RED_LED 5

const char* ssid = "Wokwi-GUEST";
const char* password = "";

const char* mqtt_server = "broker.mqtt.cool"; // ou test.mosquitto.org
const int mqtt_port = 1883;
const char* mqtt_topic = "esp32/robo/comando";

WiFiClient espClient;
PubSubClient client(espClient);

bool roboLigado = true;
bool flagState = false;

static unsigned long last_time = 0;
static unsigned long actual_time = 0;

// === INTERRUPÇÃO COM DEBOUNCE ===
IRAM_ATTR void interrupt_fun() {
  actual_time = millis();
  if (actual_time - last_time > 250) {
    last_time = actual_time;
    flagState = true;
    roboLigado = !roboLigado;
  }
}

// === CONEXÃO WI-FI ===
void setup_wifi() {
  delay(100);
  Serial.println("Conectando ao Wi-Fi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWi-Fi conectado.");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

// === SETUP ===
void setup() {
  Serial.begin(115200);
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(HOR, INPUT);
  pinMode(VER, INPUT);


  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(RED_LED, LOW);

  attachInterrupt(digitalPinToInterrupt(BUTTON), interrupt_fun, FALLING);

  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);

  Serial.println("Sistema iniciado. Robô ligado.");
}

// === LOOP ===
void loop() {
  if (!client.connected()) {
    reconnectMQTT();
  }
  client.loop();

  // Trata estado ligado/desligado
  if (flagState) {
    if (roboLigado) {
      digitalWrite(GREEN_LED, HIGH);
      digitalWrite(RED_LED, LOW);
      Serial.println("Robô Ligado. Controle retomado.");
      client.publish(mqtt_topic, "LIGAR");
    } else {
      digitalWrite(GREEN_LED, LOW);
      digitalWrite(RED_LED, HIGH);
      Serial.println("Robô desligado. Sem controle.");
      client.publish(mqtt_topic, "DESLIGAR");
    }
    flagState = false;
  }

  // Envia comandos via MQTT
  if (roboLigado) {
    int eixoX = analogRead(HOR);
    int eixoY = analogRead(VER);

    String comando = "";
    Serial.print(eixoX);
    Serial.print(eixoY);
    if (eixoY > 3000 && eixoX > 1500 && eixoX < 2500) {
      comando = "Frente";
    } else if (eixoY < 1000 && eixoX > 1500 && eixoX < 2500) {
      comando = "Trás";
    } else if (eixoX < 1000 && eixoY > 1500 && eixoY < 2500) {
      comando = "Esquerda";
    } else if (eixoX > 3000 && eixoY > 1500 && eixoY < 2500) {
      comando = "Direita";
    } else {
      comando = "Parado";
    }

    Serial.println("Comando: " + comando);
    client.publish(mqtt_topic, comando.c_str());
  }

  delay(200); // Leitura do joystick
}

// === RECONEXÃO MQTT ===
void reconnectMQTT() {
  while (!client.connected()) {
    Serial.print("Tentando conectar ao MQTT... ");
    if (client.connect("ESP32Client")) {
      Serial.println("conectado!");
      client.publish(mqtt_topic, "Controle conectado");
    } else {
      Serial.print("falha, rc=");
      Serial.print(client.state());
      Serial.println(" tentando novamente em 3s");
      delay(3000);
    }
  }
}
