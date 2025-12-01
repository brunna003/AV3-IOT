/*
INSTRUÇÕES PARA CONFIGURAR:
1) Altere seu Wi-Fi:
      const char* ssid = "SEU_WIFI";
      const char* password = "SUA_SENHA";

2) Altere broker e porta se quiser:
      mqtt_server = "broker.hivemq.com";
      mqtt_port   = 1883;

3) Altere o tópico usado para controlar o LED:
      client.subscribe("casa/led");
      client.publish("casa/led", ...);

4) LED conectado no pino 2 (LED_BUILTIN). Troque se quiser.
*/

#include <WiFi.h>
#include <PubSubClient.h>

// === CONFIGURAÇÕES EDITÁVEIS ===
const char* ssid = "SEU_WIFI";
const char* password = "SUA_SENHA";

const char* mqtt_server = "broker.hivemq.com";
const int   mqtt_port   = 1883;

#define LED_PIN 2   // LED no pino 2 (ESP32)

// === OBJETOS ===
WiFiClient espClient;
PubSubClient client(espClient);

// === CALLBACK: executa ao receber mensagem ===
void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Mensagem recebida no tópico: ");
  Serial.println(topic);

  String msg;
  for (int i = 0; i < length; i++) {
    msg += (char)message[i];
  }

  Serial.print("Conteúdo: ");
  Serial.println(msg);

  // Controle do LED
  if (msg == "ON") {
    digitalWrite(LED_PIN, HIGH);
    Serial.println("LED LIGADO");
  }
  else if (msg == "OFF") {
    digitalWrite(LED_PIN, LOW);
    Serial.println("LED DESLIGADO");
  }
}

// === Conectar ao Wi-Fi ===
void setup_wifi() {
  Serial.print("Conectando ao Wi-Fi ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi conectado!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

// === Reconnect MQTT ===
void reconnect() {
  while (!client.connected()) {
    Serial.print("Conectando ao MQTT...");

    if (client.connect("ESP32_LEDClient")) {
      Serial.println("conectado!");
      client.subscribe("casa/led");   // <-- TÓPICO DO LED
    } else {
      Serial.print("Falhou, rc=");
      Serial.print(client.state());
      Serial.println(" tentando novamente...");
      delay(2000);
    }
  }
}

// === SETUP ===
void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);

  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

// === LOOP ===
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
