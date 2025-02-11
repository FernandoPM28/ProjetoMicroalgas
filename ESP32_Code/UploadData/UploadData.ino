#include <HardwareSerial.h>
#include <WiFi.h>
#include <ThingsBoard.h>
#include <Arduino_MQTT_Client.h>
#include <ArduinoJson.h>

#define WIFI_AP "denso_broker"
#define WIFI_PASS "denso_broker"

#define TB_SERVER "thingsboard.cloud"
#define TOKEN "rHgIa492iipgQGVCmPzE"

constexpr uint16_t MAX_MESSAGE_SIZE = 256U;

WiFiClient espClient;
Arduino_MQTT_Client mqttClient(espClient);
ThingsBoard tb(mqttClient, MAX_MESSAGE_SIZE);

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

void connectToThingsBoard() {
  if (!tb.connected()) {
    Serial.println("\nConnecting to ThingsBoard server");
    
    if (!tb.connect(TB_SERVER, TOKEN)) {
      Serial.println("Failed to connect to ThingsBoard");
    } else {
      Serial.println("Connected to ThingsBoard");
    }
  }
}

void sendDataToThingsBoard(String data) {
  Serial.print("\nDados recebidos: ");
  Serial.println(data);

  if (!tb.connected()) {
    Serial.println("Erro: Não conectado ao ThingsBoard!");
    return;
  }

  Serial.println("Enviando dados para ThingsBoard...");

  DynamicJsonDocument doc(1024);

  float ec, tds, salin, ph, od, temp;
  int parsed = sscanf(data.c_str(), "EC: %f uS/cm | TDS: %f ppm | Salin: %f ppt | pH: %f | OD: %f mg/L | Temp: %f °C", 
                      &ec, &tds, &salin, &ph, &od, &temp);

  if (parsed != 6) {  
    Serial.println("Erro ao processar os dados!");
    return;
  }

  doc["EC"] = ec;
  doc["TDS"] = tds;
  doc["Salinity"] = salin;
  doc["pH"] = ph;
  doc["OD"] = od;
  doc["Temperature"] = temp;

  size_t jsonSize = measureJson(doc);

  tb.setBufferSize(256, 256);
  
  if (tb.sendTelemetryJson(doc, jsonSize)) {
    Serial.println("Data enviado com sucesso!");
  } else {
    Serial.println("Erro ao enviar dados para ThingsBoard!");
  }
}

HardwareSerial mySerial(1);

void setup() {
  Serial.begin(9600);      
  mySerial.begin(9600, SERIAL_8N1, 16, 17); // RX=16, TX=17
  connectToWiFi();
  connectToThingsBoard();
}

void loop() {
  if (mySerial.available()) {
    String sensorData = mySerial.readStringUntil('\n');
    sendDataToThingsBoard(sensorData);
  }

  if (!tb.connected()) {
    if (WiFi.status() != WL_CONNECTED) {
      connectToWiFi();
    }
    connectToThingsBoard();
  }

  tb.loop();

}
