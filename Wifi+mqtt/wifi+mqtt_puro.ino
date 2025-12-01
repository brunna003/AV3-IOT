/*
INSTRUÇÕES PARA CONFIGURAR:
1) Altere o nome e senha do Wi-Fi:
      const char* ssid = "SEU_WIFI";
      const char* password = "SUA_SENHA";

2) Se quiser outro broker MQTT, mude:
      mqtt_server = "broker.hivemq.com";
      mqtt_port = 1883;

3) Altere os tópicos MQTT conforme desejar:
      client.subscribe("meutopico/teste");
      client.publish("meutopico/teste", "Mensagem...");

4) Opcional: troque o ID do cliente MQTT:
      client.connect("ESP32_TestClient");

Após isso, carregue no ESP32 e abra o Serial Monitor (115200).
*/

#include <WiFi.h>
#include <PubSubClient.h>

// === CONFIGURAÇÕES EDITÁVEIS ===
const char* ssid = "SEU_WIFI";
const char* password = "SUA_SENHA";

const char* mqtt_server = "broker.hivemq.com";
const int   mqtt_port   = 1883;

// === OBJETOS ===
WiFiClient espClient;
PubSubClient client(espClient);

// === CALLBACK ===
void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Mensagem no tópico: ");
  Serial.println(topic);

  Serial.print("Conteúdo: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
  }
  Serial.println();
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

// === Reconectar MQTT ===
void reconnect() {
  while (!client.connected()) {
    Serial.print("Conectando ao MQTT...");
    
    if (client.connect("ESP32_TestClient")) {  // ID do cliente MQTT
      Serial.println("conectado!");
      client.subscribe("meutopico/teste");     // tópico de entrada
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

  static unsigned long lastMsg = 0;
  if (millis() - lastMsg > 5000) {
    lastMsg = millis();
    client.publish("meutopico/teste", "Olá MQTT");
  }
}
