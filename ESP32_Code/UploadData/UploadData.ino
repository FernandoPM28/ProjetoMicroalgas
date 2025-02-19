#include <HardwareSerial.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>

#define WIFI_AP "denso_broker"
#define WIFI_PASS "denso_broker"

const char* mqtt_server = "200.132.77.45";
const char* mqtt_topic = "topico";

constexpr uint16_t MAX_MESSAGE_SIZE = 256U; 

WiFiClient espClient;
PubSubClient client(espClient);

void connectToWiFi() {
  Serial.println("\nConnecting to WiFi...");
  int attempts = 0;
  
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    WiFi.begin(WIFI_AP, WIFI_PASS, 6);
    delay(2500);
    attempts++;
  }
  
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Failed to connect to WiFi.");
  } else {
    Serial.println("Connected to WiFi");
  }
}

void connectToClient() { 
  Serial.println("\nConnecting to MQTT Client...");
    
  while (!client.connected() && attempts < 20) {
    String clientId = "ESP32";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
     Serial.println("connected");
      // Once connected, publish an announcement...
     //client.publish("outTopic", "hello world");
      // ... and resubscribe
     //client.subscribe("exemple");
    } else {
     Serial.print("failed, rc=");
     Serial.print(client.state());
     Serial.println(" try again in 5 seconds");
     delay(5000);
    }
  }
}

void sendDataToBroker(String data) {
  Serial.print("\nDados recebidos: ");
  Serial.println(data);

  float ec, tds, salin, ph, od, temp;
  int parsed = sscanf(data.c_str(), "EC: %f uS/cm | TDS: %f ppm | Salin: %f ppt | pH: %f | OD: %f mg/L | Temp: %f °C", 
                                         &ec,            &tds,           &salin,      &ph,     &od,            &temp);

  if (parsed != 6) {  
    Serial.println("Erro ao processar os dados!");
    return;
  }
  
  DynamicJsonDocument doc(1024);
  doc["EC"] = ec;
  doc["TDS"] = tds;
  doc["Salinity"] = salin;
  doc["pH"] = ph;
  doc["OD"] = od;
  doc["Temperature"] = temp;

  size_t jsonSize = measureJson(doc);

  if (!client.connected()) {
    Serial.println("Erro: Não conectado ao MQTT Client!");
    return;
    
  }else{
    Serial.println("Enviando dados para o broker...");
  
    if (client.publish(mqtt_topic, doc)) {
      Serial.println("Data enviado com sucesso!");
    } else {
      Serial.println("Erro ao enviar dados para ThingsBoard!");
    }
  }
  
}

HardwareSerial mySerial(1);

void setup() {
  Serial.begin(9600);      
  mySerial.begin(9600, SERIAL_8N1, 16, 17); // RX=16, TX=17
  connectToWiFi();
  client.setServer(mqtt_server, 1883);
}

void loop() {

  if (!client.connected())) {
    if (WiFi.status() != WL_CONNECTED) {
      connectToWiFi();
    }
    connectToClient();
  }
  
  client.loop();

  if (mySerial.available()) {
    String sensorData = mySerial.readStringUntil('\n');
    sendDataToBroker(sensorData);
  }
}
