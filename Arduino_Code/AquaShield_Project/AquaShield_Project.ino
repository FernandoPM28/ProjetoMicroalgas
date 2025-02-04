#include "include/AcquaShield.h"

// Instâncias do SoftwareSerial para cada shield
SoftwareSerial mySerial_EC(3, 2);  // RX, TX para EC Shield
SoftwareSerial mySerial_OD(7, 6);  // RX, TX para OD Shield
SoftwareSerial mySerial_pH(5, 4);  // RX, TX para pH Shield

// Instâncias dos shields
ECShield shieldEC(mySerial_EC);
ODShield shieldOD(mySerial_OD);
pHShield shieldPH(mySerial_pH);

void setup() {
    Serial.begin(9600);  // Inicializa a comunicação serial com o computador

    // Inicializa os shields
    shieldEC.init();
    shieldOD.init();
    shieldPH.init();

    // Mensagens iniciais nos displays
    shieldEC.writeToDisplay(3, 13, 21, "EC Shield");
    shieldEC.showDisplay();
    delay(500);

    shieldPH.writeToDisplay(3, 13, 21, "pH Shield");
    shieldPH.showDisplay();
    delay(500);

    shieldOD.writeToDisplay(3, 13, 21, "OD Shield");
    shieldOD.showDisplay();
    delay(500);

    Serial.println("\nInicialização concluída. Iniciando leituras...");
}

void loop() {
    static unsigned long previousMillis = 0;
    const unsigned long interval = 250;  // Intervalo de 1 segundo entre as leituras

    if (millis() - previousMillis >= interval) {
        previousMillis = millis();

        // Leitura dos valores dos sensores
        float ec = shieldEC.readEC();         // Condutividade Elétrica (EC)
        float tds = shieldEC.readTDS();       // Total de Sólidos Dissolvidos (TDS)
        float salin = shieldEC.readSalinity(); // Salinidade
        float ph = shieldPH.readPH();         // pH
        float od = shieldOD.readOD();         // Oxigênio Dissolvido (OD)

        float temp_EC = shieldEC.readTemp();
        float temp_OD = shieldOD.readTemp();
        float temp_PH = shieldPH.readTemp();
        
        float temp = 100.0;  // Inicializa como valor inválido

        if (temp_PH != 100.0) {
            temp = temp_PH;
        } else if (temp_EC != 100.0) {
            temp = temp_EC;
        } else if (temp_OD != 100.0) {
            temp = temp_OD;
        } else {
            temp = 100.0;
        }

        // Exibe os valores no Serial Monitor em uma única linha
        Serial.print("EC: ");
        Serial.print(ec);
        Serial.print(" uS/cm | TDS: ");
        Serial.print(tds);
        Serial.print(" ppm | Salin: ");
        Serial.print(salin);
        Serial.print(" ppt | pH: ");
        Serial.print(ph);
        Serial.print(" | OD: ");
        Serial.print(od);
        Serial.print(" mg/L | Temp: ");
        Serial.print(temp);
        Serial.println(" °C");
    }
}