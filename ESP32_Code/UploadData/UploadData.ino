#include <HardwareSerial.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>

// Configurações de Wi-Fi
const char *ssid = "denso_broker";
const char *password = "denso_broker";

// Configurações do MQTT
const char *mqtt_server = "200.132.77.45";
const int mqtt_port = 1883;
const char *mqtt_username = "emqx";
const char *mqtt_password = "public";
const char *mqtt_topic = "FURGC3";

// Tamanho do buffer JSON
constexpr uint16_t JSON_BUFFER_SIZE = 512; // Aumentado para garantir espaço suficiente

// Objetos globais
WiFiClient espClient;
PubSubClient client(espClient);
HardwareSerial mySerial(1); // UART para comunicação com o sensor

// Função para conectar ao Wi-Fi
void connectToWiFi() {
  Serial.println("\nConectando ao Wi-Fi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(5000);
    Serial.println("Tentando conectar ao Wi-Fi...");
  }
  Serial.println("Conectado ao Wi-Fi!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

// Função para conectar ao broker MQTT
void connectToMQTT() {
  Serial.println("\nConectando ao broker MQTT...");
  String clientId = "ESP32-" + String(WiFi.macAddress());

  while (!client.connected()) {
    if (client.connect(clientId.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("Conectado ao broker MQTT!");
      client.subscribe(mqtt_topic); // Inscreve-se no tópico
    } else {
      Serial.print("Falha na conexão, rc=");
      Serial.print(client.state());
      Serial.println(" Tentando novamente em 5 segundos...");
      delay(5000);
    }
  }
}

// Função de callback para mensagens MQTT recebidas
void callback(char *topic, byte *payload, unsigned int length) {
  Serial.print("Mensagem recebida no tópico: ");
  Serial.println(topic);
  Serial.print("Conteúdo: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println("\n-----------------------");
}

// Função para enviar dados ao broker MQTT
void sendDataToBroker(const String &data) {
  Serial.print("\nDados recebidos: ");
  Serial.println(data);

  // Processamento dos dados do sensor
  float ec, tds, salin, ph, od, temp;
  int parsed = sscanf(data.c_str(), "EC: %f uS/cm | TDS: %f ppm | Salin: %f ppt | pH: %f | OD: %f mg/L | Temp: %f °C", 
                      &ec, &tds, &salin, &ph, &od, &temp);

  if (parsed != 6) {
    Serial.println("Erro ao processar os dados do sensor!");
    return;
  }

  // Criação do JSON
  StaticJsonDocument<JSON_BUFFER_SIZE> jsonDoc;
  JsonObject fields = jsonDoc.createNestedObject("fields");

  fields["Temperature"] = temp;
  fields["pH"] = ph;
  fields["OD"] = od;
  fields["EC"] = ec;
  fields["TDS"] = tds;
  fields["Salinity"] = salin;

  // Serialização do JSON
  char jsonBuffer[JSON_BUFFER_SIZE];
  serializeJson(jsonDoc, jsonBuffer);

  // Verificação de conexão e envio dos dados
  if (!client.connected()) {
    Serial.println("Erro: Não conectado ao broker MQTT!");
    connectToMQTT(); // Tenta reconectar
  }

  if (client.publish(mqtt_topic, jsonBuffer)) {
    Serial.println("Dados enviados com sucesso!");
  } else {
    Serial.println("Erro ao enviar dados para o broker!");
  }
}

// Configuração inicial
void setup() {
  Serial.begin(9600);
  mySerial.begin(9600, SERIAL_8N1, 16, 17); // RX=16, TX=17

  Serial.println("Iniciando conexões...");
  connectToWiFi(); // Conecta ao Wi-Fi
  client.setServer(mqtt_server, mqtt_port); // Configura o servidor MQTT
  client.setCallback(callback); // Define a função de callback
  connectToMQTT(); // Conecta ao broker MQTT
}

// Loop principal
void loop() {
  // Verifica e mantém a conexão com o broker MQTT
  if (!client.connected()) {
    connectToMQTT();
  }
  client.loop(); // Mantém a comunicação MQTT ativa

  // Verifica se há dados disponíveis no sensor
  if (mySerial.available()) {
    String sensorData = mySerial.readStringUntil('\n');
    sendDataToBroker(sensorData); // Envia os dados para o broker
  }
}
