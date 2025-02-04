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

float pHShield::readOffset() {
    return readData("Oft");
}