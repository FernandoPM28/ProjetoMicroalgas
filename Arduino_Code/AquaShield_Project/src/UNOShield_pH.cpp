#include "../include/AcquaShield.h"

// Implementação do construtor da classe pHShield
pHShield::pHShield(SoftwareSerial &serial) : Shield(serial, ShieldPH_addr) {}

// Implementação das funções específicas do pH Shield
float pHShield::readPH() {
    static float lastValidPH = 7.0;  // Valor inicial arbitrário

    float value = readData("ph");
    if (value == 100.0) {
        // Serial.println("Erro na leitura do pH! Mantendo valor anterior.");
        return lastValidPH;
    }

    lastValidPH = value;  // Atualiza o último valor válido
    return value;
}

float pHShield::readTemp() {
    return readData("tmp");
}

void pHShield::pHCalibrate() {
    return calibrate();
}

void pHShield::readSlopeParameters() {
    Serial.println("\n--- Parâmetros de Slope pH ---");
    
    // Lê e exibe slopes (pH 4.0 e 7.0)
    float slope04 = readData("Sl04");
    float slope10 = readData("Sl10");
    Serial.print("Slope pH 4.0: "); Serial.print(slope04); Serial.println(" %");
    Serial.print("Slope pH 7.0: "); Serial.print(slope10); Serial.println(" %");

    // Lê e exibe offset
    float offset = readData("Oft");
    Serial.print("Offset: "); Serial.print(offset); Serial.println(" mV");

    // Lê e exibe temperatura de calibração
    float calTemp = readData("Ctmp");
    Serial.print("Temp. Calibração: "); Serial.print(calTemp); Serial.println(" °C");
}