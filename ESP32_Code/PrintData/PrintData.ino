#include <HardwareSerial.h>
// Use uma porta serial secundária (ex.: Serial1)
HardwareSerial mySerial1(1);
HardwareSerial mySerial2(2);

void setup() {
  Serial.begin(9600);          // Monitor Serial do ESP32
  mySerial1.begin(9600, SERIAL_8N1, 16, 17); // RX=16, TX=17
  mySerial2.begin(9600, SERIAL_8N1, 21, 22); // RX=21, TX=22
}
void loop() {
  if (mySerial1.available()) {
    String sensorData = Serial1.readStringUntil('\n');
    Serial.println(sensorData);
  }
  if (mySerial2.available()) {
    String sensorData = Serial2.readStringUntil('\n');
    Serial.println(sensorData);
  }
}
