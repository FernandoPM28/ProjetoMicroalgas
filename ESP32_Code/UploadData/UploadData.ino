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
const char *mqtt_topic = "FURGC3";

// Tamanho do buffer JSON
constexpr uint16_t JSON_BUFFER_SIZE = 512;

// Objetos globais
WiFiClient espClient;
PubSubClient client(espClient);
HardwareSerial mySerial1(1); // UART para comunicação com o sensor
HardwareSerial mySerial2(2);

// Função para conectar ao Wi-Fi
void connectToWiFi() {
  Serial.println("\nConectando ao Wi-Fi...");

  // Limpa configurações Wi-Fi salvas
  WiFi.disconnect(true);
  delay(1000);

  // Define a potência máxima de transmissão
  WiFi.setTxPower(WIFI_POWER_19_5dBm);

  // Conecta ao Wi-Fi
  WiFi.begin(ssid, password); // Força o canal 6
  delay(5000);

  int tentativas = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(5000);
    tentativas++;
    Serial.print("Tentativa ");
    Serial.print(tentativas);
    Serial.println(": Tentando conectar ao Wi-Fi...");

    // Exibe o status atual da conexão Wi-Fi
    switch (WiFi.status()) {
      case WL_NO_SSID_AVAIL:
        Serial.println("Erro: Rede não encontrada. Verifique o SSID.");
        break;
      case WL_CONNECT_FAILED:
        Serial.println("Erro: Falha na conexão. Verifique a senha.");
        break;
      case WL_CONNECTION_LOST:
        Serial.println("Erro: Conexão perdida.");
        break;
      case WL_DISCONNECTED:
        Serial.println("Erro: Desconectado.");
        break;
      default:
        Serial.println("Erro desconhecido.");
        break;
    }

    // Se exceder um número máximo de tentativas, reinicia o ESP32
    if (tentativas > 10) {
      Serial.println("Falha ao conectar ao Wi-Fi. Reiniciando o ESP32...");
      ESP.restart();
    }
  }

  Serial.println("Conectado ao Wi-Fi!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

// Função para conectar ao broker MQTT
void connectToMQTT() {
  Serial.println("\nConectando ao broker MQTT...");
  String clientId = "ESP32-" + String(WiFi.macAddress());

  client.connect(clientId.c_str());
  delay(5000);

  int tentativas = 0;
  while (!client.connected()) {
    delay(5000);
    tentativas++;
    Serial.print("Tentativa ");
    Serial.print(tentativas);
    Serial.println(": Tentando conectar ao broker MQTT...");
    
    // Exibe o status atual da conexão MQTT
    switch (client.state()) {
      case MQTT_CONNECTION_TIMEOUT:
        Serial.println("Erro: Tempo de conexão excedido.");
        break;
      case MQTT_CONNECTION_LOST:
        Serial.println("Erro: Conexão perdida.");
        break;
      case MQTT_CONNECT_FAILED:
        Serial.println("Erro: Falha na conexão.");
        break;
      case MQTT_DISCONNECTED:
        Serial.println("Erro: Desconectado.");
        break;
      case MQTT_CONNECTED:
        Serial.println("Erro: Já conectado.");
        break;
      case MQTT_CONNECT_BAD_PROTOCOL:
        Serial.println("Erro: Protocolo inválido.");
        break;
      case MQTT_CONNECT_BAD_CLIENT_ID:
        Serial.println("Erro: ID do cliente inválido.");
        break;
      case MQTT_CONNECT_UNAVAILABLE:
        Serial.println("Erro: Servidor indisponível.");
        break;
      case MQTT_CONNECT_BAD_CREDENTIALS:
        Serial.println("Erro: Credenciais inválidas.");
        break;
      case MQTT_CONNECT_UNAUTHORIZED:
        Serial.println("Erro: Não autorizado.");
        break;
      default:
        Serial.println("Erro desconhecido.");
        break;
    }

    // Se exceder um número máximo de tentativas, reinicia o ESP32
    if (tentativas > 10) {
      Serial.println("Falha ao conectar ao broker MQTT. Reiniciando o ESP32...");
      ESP.restart();
    }
  }

  Serial.println("Conectado ao broker MQTT!");
  Serial.print("ID: ");
  Serial.println(clientId);
  client.subscribe(mqtt_topic); // Inscreve-se no tópico
}

// Função de callback para mensagens MQTT recebidas
void callback(char *topic, byte *payload, unsigned int length) {
  Serial.print("\nMensagem recebida no tópico: ");
  Serial.println(topic);
  Serial.print("Conteúdo: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.print("\n");
}

// Função para enviar dados ao broker MQTT
void sendAcquanativaDataToBroker(const String &data) {
  Serial.print("\nDados recebidos do arduino: ");
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
  DynamicJsonDocument jsonDoc(JSON_BUFFER_SIZE); // Usando DynamicJsonDocument
  jsonDoc["payload"] = "dados do esp"; // Define o payload como uma string
  JsonObject fields = jsonDoc.createNestedObject("fields"); // Cria o objeto fields

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
    Serial.println("Dados sensores Acquanativa enviados com sucesso!");
  } else {
    Serial.println("Erro ao enviar dados sensores Acquanativa para o broker!");
  }
}

void sendThallesDataToBroker(const String &data) {
  Serial.print("\nDados recebidos do arduino: ");
  Serial.println(data);

  // Processamento dos dados do sensor
  float volt, ph;
  int parsed = sscanf(data.c_str(), "Tensao: %f V | pH %f", 
                      &volt, &ph);

  if (parsed != 2) {
    Serial.println("Erro ao processar os dados do sensor!");
    return;
  }

  // Criação do JSON
  DynamicJsonDocument jsonDoc(JSON_BUFFER_SIZE); // Usando DynamicJsonDocument
  jsonDoc["payload"] = "dados do esp"; // Define o payload como uma string
  JsonObject fields = jsonDoc.createNestedObject("fields"); // Cria o objeto fields

  fields["Tensao"] = volt;
  fields["pH"] = ph;

  // Serialização do JSON
  char jsonBuffer[JSON_BUFFER_SIZE];
  serializeJson(jsonDoc, jsonBuffer);

  // Verificação de conexão e envio dos dados
  if (!client.connected()) {
    Serial.println("Erro: Não conectado ao broker MQTT!");
    connectToMQTT(); // Tenta reconectar
  }

  if (client.publish(mqtt_topic, jsonBuffer)) {
    Serial.println("Dados sensor Thalles enviados com sucesso!");
  } else {
    Serial.println("Erro ao enviar dados sensor Thalles para o broker!");
  }
}

// Configuração inicial
void setup() {
  Serial.begin(9600);
  mySerial1.begin(9600, SERIAL_8N1, 16, 17); // RX=16, TX=17
  mySerial2.begin(9600, SERIAL_8N1, 21, 22); // RX=21, TX=22

  Serial.println("\nIniciando conexões...");
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
  if (mySerial1.available()) {
    String sensorData = Serial1.readStringUntil('\n');
    sendAcquanativaDataToBroker(sensorData); // Envia os dados para o broker
  }
  if (mySerial2.available()) {
    String sensorData = Serial2.readStringUntil('\n');
    sendThallesDataToBroker(sensorData); // Envia os dados para o broker
  }
}
