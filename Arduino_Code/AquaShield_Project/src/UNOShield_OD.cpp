#include "../include/AcquaShield.h"

// Implementação do construtor da classe ODShield
ODShield::ODShield(SoftwareSerial &serial) : Shield(serial, ShieldOD_addr) {}

// Implementação das funções específicas do OD Shield
float ODShield::readOD() {
    static float lastValidOD = 5.0;  // Valor inicial arbitrário

    float value = readData("odmgl");
    if (value == 100.0) {
        // Serial.println("Erro na leitura do OD! Mantendo valor anterior.");
        return lastValidOD;
    }

    lastValidOD = value;
    return value;
}

float ODShield::readTemp() {
    return readData("tmp");
}

float ODShield::readSaturation() {
    return readData("odsat");
}