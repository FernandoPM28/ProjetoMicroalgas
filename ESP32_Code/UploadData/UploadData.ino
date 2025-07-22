#include <Wire.h>
#include <WiFi.h>
#include <WiFiManager.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// --- CONFIGURAÇÕES I2C ---
// Endereços I²C dos Arduinos escravos
#define PH_ARDUINO_ADDR     0x08
#define SENSORS_ARDUINO_ADDR  0x09

// --- ESTRUTURAS DE DADOS ---
// As definições das structs DEVEM SER IDÊNTICAS às dos Arduinos escravos.
// Elas funcionam como um "contrato" para a comunicação.
struct DadosPH {
  uint16_t ph; // pH * 100 (ex: 725 para 7.25)
};

struct DadosSensores {
  float ec;
  float tds;
  float salinidade;
  float od;
  float temperatura;
};

// --- CONFIGURAÇÕES MQTT ---
const char *mqtt_server = "200.132.77.45";
const int mqtt_port = 1883;
const char *mqtt_topic = "FURGC3";
constexpr uint16_t JSON_BUFFER_SIZE = 512;

// --- OBJETOS GLOBAIS ---
WiFiClient espClient;
PubSubClient client(espClient);

// Função para conectar ao MQTT (sua função original, sem alterações)
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

// Callback MQTT para mensagens recebidas (sua função original, sem alterações)
void callback(char *topic, byte *payload, unsigned int length) {
  Serial.print("\nMensagem recebida no tópico: ");
  Serial.println(topic);
  Serial.print("Conteúdo: ");
  for (unsigned int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

// Envio de dados para o broker MQTT (sua função original, sem alterações)
void sendDataToBroker(float ph, float ec, float tds, float salin, float od, float temp) {
  Serial.println("\n== Enviando dados MQTT ==");
  DynamicJsonDocument jsonDoc(JSON_BUFFER_SIZE);
  jsonDoc["payload"] = "dados do esp32";
  JsonObject fields = jsonDoc.createNestedObject("fields");
  fields["pH"] = ph;
  fields["EC"] = ec;
  fields["TDS"] = tds;
  fields["Salinity"] = salin;
  fields["OD"] = od;
  fields["Temperature"] = temp;

  char jsonBuffer[JSON_BUFFER_SIZE];
  serializeJson(jsonDoc, jsonBuffer);

  if (!client.connected()) {
    connectToMQTT();
  }

  if (client.publish(mqtt_topic, jsonBuffer)) {
    Serial.println("✅ Dados enviados com sucesso para o broker!");
  } else {
    Serial.println("❌ Erro ao enviar dados para o broker!");
  }
}

void setup() {
  Serial.begin(115200); // Use uma velocidade maior no ESP32 para mais performance
  Wire.begin(); // SDA = 21, SCL = 22 (padrão no ESP32)

  // Inicia WiFi usando WiFiManager
  WiFi.mode(WIFI_STA);
  WiFiManager wm;
  bool res = wm.autoConnect("esp32-sonda-furg", "furg@2024");
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
}

void loop() {
  if (!client.connected()) {
    connectToMQTT();
  }
  client.loop();

  static unsigned long lastSend = 0;
  // Intervalo de 5 segundos para leitura e envio
  if (millis() - lastSend >= 5000) {
    lastSend = millis();

    // --- NOVA LÓGICA DE LEITURA I2C ---

    // 1. Lendo dados do Arduino de pH
    DadosPH ph_recebido;
    float ph_final = -1.0; // Inicia com valor de erro
    
    Wire.requestFrom(PH_ARDUINO_ADDR, sizeof(DadosPH));
    if (Wire.available() == sizeof(DadosPH)) {
      // Lê os bytes diretamente para dentro da struct
      Wire.readBytes((char *)&ph_recebido, sizeof(DadosPH));
      // Converte o valor de volta para float
      ph_final = ph_recebido.ph / 100.0;
    } else {
      Serial.println("❌ Erro ao comunicar com o Arduino de pH!");
    }

    // 2. Lendo dados do Arduino de Sensores
    DadosSensores sensores_recebidos;
    bool sensores_ok = false;

    Wire.requestFrom(SENSORS_ARDUINO_ADDR, sizeof(DadosSensores));
    if (Wire.available() == sizeof(DadosSensores)) {
      Wire.readBytes((char *)&sensores_recebidos, sizeof(DadosSensores));
      sensores_ok = true;
    } else {
      Serial.println("❌ Erro ao comunicar com o Arduino de Sensores!");
    }
    
    Serial.println("\n=== Dados Recebidos Via I2C ===");
    Serial.print("pH: "); Serial.println(ph_final, 2);
    if(sensores_ok) {
      Serial.print("EC: "); Serial.println(sensores_recebidos.ec, 2);
      Serial.print("TDS: "); Serial.println(sensores_recebidos.tds, 2);
      Serial.print("Salinidade: "); Serial.println(sensores_recebidos.salinidade, 2);
      Serial.print("OD: "); Serial.println(sensores_recebidos.od, 2);
      Serial.print("Temperatura: "); Serial.println(sensores_recebidos.temperatura, 2);
    } else {
      Serial.println("Dados dos outros sensores não foram recebidos.");
    }
    
    // 3. Envia os dados para o broker MQTT se ambos foram lidos com sucesso
    if (ph_final != -1.0 && sensores_ok) {
      sendDataToBroker(
        ph_final,
        sensores_recebidos.ec,
        sensores_recebidos.tds,
        sensores_recebidos.salinidade,
        sensores_recebidos.od,
        sensores_recebidos.temperatura
      );
    } else {
      Serial.println("\n❌ Envio MQTT cancelado devido a falha na leitura de um dos sensores.");
    }
  }
}
