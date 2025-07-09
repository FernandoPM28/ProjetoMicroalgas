#include <HardwareSerial.h>

// Instancia duas portas seriais adicionais
HardwareSerial mySerial1(1);  // UART1
HardwareSerial mySerial2(2);  // UART2

void setup() {
  Serial.begin(9600);  // Porta serial padrão (USB para PC)

  // Inicializa UART1 com RX=16, TX=17
  mySerial1.begin(9600, SERIAL_8N1, 16, 17);

  // Inicializa UART2 com RX=21, TX=22
  mySerial2.begin(9600, SERIAL_8N1, 21, 22);
}

void loop() {
  // Leitura da porta UART1
  if (mySerial1.available()) {
    String sensorData = mySerial1.readStringUntil('\n');
    Serial.println("[UART1] " + sensorData);
  }

  // Leitura da porta UART2
  if (mySerial2.available()) {
    String sensorData = mySerial2.readStringUntil('\n');
    Serial.println("[UART2] " + sensorData);
  }
}

