#include <HardwareSerial.h>
#include <WiFi.h>
#include <ThingsBoard.h>
#include <Arduino_MQTT_Client.h>

#define WIFI_AP "denso_broker"
#define WIFI_PASS "denso_broker"

#define TB_SERVER "thingsboard.cloud"
#define TOKEN "rHgIa492iipgQGVCmPzE"

constexpr uint16_t MAX_MESSAGE_SIZE = 256U;

WiFiClient espClient;
Arduino_MQTT_Client mqttClient(espClient);
ThingsBoard tb(mqttClient, MAX_MESSAGE_SIZE);

void connectToWiFi() {
  Serial.println("Connecting to WiFi...");
  int attempts = 0;
  
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    WiFi.begin(WIFI_AP, WIFI_PASS, 6);
    delay(500);
    Serial.print(".");
    attempts++;
  }
  
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("\nFailed to connect to WiFi.");
  } else {
    Serial.println("\nConnected to WiFi");
  }
}

void connectToThingsBoard() {
  if (!tb.connected()) {
    Serial.println("Connecting to ThingsBoard server");
    
    if (!tb.connect(TB_SERVER, TOKEN)) {
      Serial.println("Failed to connect to ThingsBoard");
    } else {
      Serial.println("Connected to ThingsBoard");
    }
  }
}

void sendDataToThingsBoard(char data) {
  // String jsonData = "{\"tempIn\":" + String(temp) + ", \"humIn\":" + String(hum) + ", \"tempOut\":" + "0" + ", \"humOut\":" + "0"+ ", \"pressure\":" + "0"+ ", \"rainfall\":" + "0"+ ", \"windSpeed\":" + "0"+ ", \"windDirection\":" + "0" + ", \"windAVG\":" +"0"+"}";
  String jsonData = "{"+ data + "}"
  tb.sendTelemetryJson(jsonData.c_str());
  Serial.println("Data sent");
}

// Use uma porta serial secundária (ex.: Serial1)
HardwareSerial mySerial(1);

void setup() {
  Serial.begin(9600);          // Monitor Serial do ESP32
  mySerial.begin(9600, SERIAL_8N1, 16, 17); // RX=16, TX=17

  connectToWiFi();
  connectToThingsBoard();
}

void loop() {

  connectToWiFi();

  if (mySerial.available()) {
    // Lê os dados enviados pelo Arduino
    Serial.print("Recebido do Arduino: ");
    String sensorData = mySerial.readStringUntil('\n');
    Serial.println(sensorData);
  }

  if (!tb.connected()) {
    connectToThingsBoard();
  }

  sendDataToThingsBoard(sensorData);
  tb.loop();

}