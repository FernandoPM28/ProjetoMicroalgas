#include <HardwareSerial.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>


#define WIFI_AP "denso_broker"
#define WIFI_PASS "denso_broker"

const char *ssid = "denso_broker";
const char *password = "denso_broker";

const char *mqtt_server = "200.132.77.45";
const int mqtt_port = 1883;
const char *mqtt_username = "emqx";
const char *mqtt_password = "public";
const char *mqtt_topic = "FURGC3";

constexpr uint16_t MAX_MESSAGE_SIZE = 256U; 

WiFiClient espClient;
PubSubClient client(espClient);

void connectToWiFi() {
  Serial.println("\nConnecting to WiFi...");
    
  while (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(ssid, password);
    WiFi.begin(WIFI_AP, WIFI_PASS);
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("Failed to connect to WiFi.");
      delay(5000);
    }
  }
  Serial.println("Connected to WiFi");
}

void connectToClient() { 
  Serial.println("\nConnecting to MQTT Client...");
    
  while (!client.connected()) {
    String clientId = "ESP32";
    clientId += String(WiFi.macAddress());
    if (client.connect(clientId.c_str(), mqtt_username, mqtt_password)) {
     Serial.println("Connected to MQTT Client");
     client.publish(mqtt_topic, "Hi, I'm ESP32 ^^");
     client.subscribe(mqtt_topic);
    } else {
     Serial.print("Failed to connect to client, rc=");
     Serial.print(client.state());
     Serial.println(" try again in 5 seconds");
     delay(5000);
    }
  }
}

void callback(char *topic, byte *payload, unsigned int length) {
    Serial.print("Message arrived in topic: ");
    Serial.println(topic);
    Serial.print("Message:");
    for (int i = 0; i < length; i++) {
        Serial.print((char) payload[i]);
    }
    Serial.println();
    Serial.println("-----------------------");
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
  
  StaticJsonBuffer<300> JSONbuffer;
  JsonObject& JSONencoder = JSONbuffer.createObject();
 
  JSONencoder["payload"] = "dados"; 
  
  JSONencoder["fields"]["Temperature"] = temp;
  JSONencoder["fields"]["pH"] = ph;
  JSONencoder["fields"]["OD"] = od;
  JSONencoder["fields"]["EC"] = ec;
  JSONencoder["fields"]["TDS"] = tds;
  JSONencoder["fields"]["Salinity"] = salin;
 
  char JSONmessageBuffer[300];
  JSONencoder.printTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));

  if (!client.connected()) {
    Serial.println("Erro: Não conectado ao MQTT Client!");
    return;
    
  }else{
    Serial.println(JSONmessageBuffer);
    Serial.println("Enviando dados para o broker...");
  
    if (client.publish(mqtt_topic, JSONmessageBuffer)) {
      Serial.println("Data enviado com sucesso!");
    } else {
      Serial.println("Erro ao enviar dados para o client!");
    }
  }
  
}

HardwareSerial mySerial(1);

void setup() {
  Serial.begin(9600);      
  mySerial.begin(9600, SERIAL_8N1, 16, 17); // RX=16, TX=17
  Serial.println("Iniciando conexões!");
  connectToWiFi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {

  if (!client.connected()) {
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
