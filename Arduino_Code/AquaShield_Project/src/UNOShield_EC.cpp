#include "../include/AcquaShield.h"

// Implementação do construtor da classe ECShield
ECShield::ECShield(SoftwareSerial &serial) : Shield(serial, ShieldEC_addr) {}

// Implementação das funções específicas do EC Shield
float ECShield::readEC() {
    static float lastValidEC = 500.0;  // Valor inicial arbitrário

    float value = readData("ec");
    if (value == 100.0) {
        // Serial.println("Erro na leitura da EC! Mantendo valor anterior.");
        return lastValidEC;
    }

    lastValidEC = value;
    return value;
}

float ECShield::readTemp() {
    return readData("tmp");
}

float ECShield::readSalinity() {
    static float lastValidSalinity = 35.0;  // Valor inicial arbitrário

    float value = readData("sal");
    if (value == 100.0) {
        // Serial.println("Erro na leitura da Salinidade! Mantendo valor anterior.");
        return lastValidSalinity;
    }

    lastValidSalinity = value;
    return value;
}


float ECShield::readTDS() {
    static float lastValidTDS = 500.0;  // Valor inicial arbitrário

    float value = readData("tds");
    if (value == 100.0) {
        // Serial.println("Erro na leitura do TDS! Mantendo valor anterior.");
        return lastValidTDS;
    }

    lastValidTDS = value;
    return value;
}
