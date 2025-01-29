#include <HardwareSerial.h>

// Use uma porta serial secundária (ex.: Serial1)
HardwareSerial mySerial(1);

void setup() {
  Serial.begin(9600);          // Monitor Serial do ESP32
  mySerial.begin(9600, SERIAL_8N1, 16, 17); // RX=16, TX=17
}

void loop() {
  if (mySerial.available()) {
    // Lê os dados enviados pelo Arduino
    Serial.print("Recebido do Arduino: ");
    String sensorData = mySerial.readStringUntil('\n');
    Serial.println(sensorData);
  }
}