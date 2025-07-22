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

void ODShield::ODCalibrate() {
    return calibrate();
}

void ODShield::readCalibrationParameters() {
    Serial.println("\n--- Parâmetros de Calibração OD ---");
    
    // Lê e exibe offset (condição seca)
    float offset = readData("Oft");
    Serial.print("Offset: "); Serial.print(offset); Serial.println(" mV");

    // Lê e exibe tensões de referência
    float voltage0 = readData("V00");
    float voltageAr = readData("Var");
    Serial.print("Tensão (0mg/L): "); Serial.print(voltage0); Serial.println(" mV");
    Serial.print("Tensão (Ar): "); Serial.print(voltageAr); Serial.println(" mV");

    // Lê e exibe temperatura de calibração (se disponível)
    float calTemp = readData("Ctmp");
    Serial.print("Temp. Calibração: "); Serial.print(calTemp); Serial.println(" °C");
}