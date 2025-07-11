#include <HardwareSerial.h>
#include <WiFi.h>
#include <WiFiManager.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>

// Configurações do MQTT
const char *mqtt_server = "200.132.77.45";
const int mqtt_port = 1883;
const char *mqtt_topic = "FURGC3";

// Tamanho do buffer JSON
constexpr uint16_t JSON_BUFFER_SIZE = 512;

// Objetos globais
WiFiClient espClient;
PubSubClient client(espClient);
HardwareSerial mySerial(1); // UART para comunicação com o Arduino

// Função para conectar ao MQTT
void connectToMQTT() {
  Serial.println("\nConectando ao broker MQTT...");
  String clientId = "ESP32-" + String(WiFi.macAddress());

  int tentativas = 0;
  while (!client.connected()) {
    if (client.connect(clientId.c_str())) {
      Serial.println("Conectado ao broker MQTT!");
      client.subscribe(mqtt_topic);
    } else {
      Serial.print("Tentativa ");
      Serial.print(++tentativas);
      Serial.print(": erro MQTT - ");
      Serial.println(client.state());
      delay(5000);

      if (tentativas > 10) {
        Serial.println("Falha ao conectar ao broker MQTT. Reiniciando...");
        ESP.restart();
      }
    }
  }
}

// Callback MQTT
void callback(char *topic, byte *payload, unsigned int length) {
  Serial.print("\nMensagem recebida no tópico: ");
  Serial.println(topic);
  Serial.print("Conteúdo: ");
  for (unsigned int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

// Envio de dados para o broker MQTT
void sendDataToBroker(const String &data) {
  Serial.print("\nDados recebidos do Arduino: ");
  Serial.println(data);

  float ec, tds, salin, ph, od, temp;
  int parsed = sscanf(data.c_str(), "EC: %f uS/cm | TDS: %f ppm | Salin: %f ppt | pH: %f | OD: %f mg/L | Temp: %f °C", 
                      &ec, &tds, &salin, &ph, &od, &temp);

  if (parsed != 6) {
    Serial.println("Erro ao processar os dados!");
    return;
  }

  DynamicJsonDocument jsonDoc(JSON_BUFFER_SIZE);
  jsonDoc["payload"] = "dados do esp";
  JsonObject fields = jsonDoc.createNestedObject("fields");
  fields["Temperature"] = temp;
  fields["pH"] = ph;
  fields["OD"] = od;
  fields["EC"] = ec;
  fields["TDS"] = tds;
  fields["Salinity"] = salin;

  char jsonBuffer[JSON_BUFFER_SIZE];
  serializeJson(jsonDoc, jsonBuffer);

  if (!client.connected()) {
    connectToMQTT();
  }

  if (client.publish(mqtt_topic, jsonBuffer)) {
    Serial.println("Dados enviados com sucesso!");
  } else {
    Serial.println("Erro ao enviar dados!");
  }
}

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600, SERIAL_8N1, 16, 17); // UART com pinos 16 RX, 17 TX

  // Inicia WiFi usando WiFiManager
  WiFi.mode(WIFI_STA);
  WiFiManager wm;
  bool res = wm.autoConnect("esp32", "12345678");
  if (!res) {
    Serial.println("Falha ao conectar com WiFiManager");
    ESP.restart();
  } else {
    Serial.println("WiFi conectado com sucesso!");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
  }

  // Configura MQTT
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  connectToMQTT();
}

void loop() {
  if (!client.connected()) {
    connectToMQTT();
  }
  client.loop();

  if (mySerial.available()) {
    String sensorData = mySerial.readStringUntil('\n');
    sendDataToBroker(sensorData);
  }
}
