#ifndef ACQUASHIELD_H
#define ACQUASHIELD_H

#include <Arduino.h>
#include <SoftwareSerial.h>

// Definições comuns
#define StartCommand '#'
#define EndCommand '#'

// Endereços dos Shields
#define ShieldEC_addr 0xE9   // Endereço do Shield EC
#define ShieldOD_addr 0xE7   // Endereço do Shield OD
#define ShieldPH_addr 0xE6   // Endereço do Shield pH

// Enumerações para tipo de exibição
enum od_disp { mgl, saturacao };       // Opções de impressão de OD
enum var_disp { temperatura, tds, salinidade };  // Opções de exibição para ECShield

// Variáveis globais para tipo de exibição
extern od_disp od_type;
extern var_disp var_type;

// Classe base para os shields
class Shield {
protected:
    SoftwareSerial &serial;  // Referência para a instância do SoftwareSerial
    uint8_t address;         // Endereço do shield

public:
    Shield(SoftwareSerial &serial, uint8_t address);  // Declaração do construtor
    void init();  // Método para inicializar o shield
    float readData(const char *command);
    void writeToDisplay(char font, int x, int y, const char *str);
    void clearDisplayBuffer();
    void clearDisplay();
    void showDisplay();
};

// Classe para o sensor EC
class ECShield : public Shield {
public:
    ECShield(SoftwareSerial &serial);  // Declaração do construtor
    float readEC();
    float readTemp();
    float readSalinity();
    float readTDS();
};

// Classe para o sensor OD
class ODShield : public Shield {
public:
    ODShield(SoftwareSerial &serial);  // Declaração do construtor
    float readOD();
    float readTemp();
    float readSaturation();
};

// Classe para o sensor pH
class pHShield : public Shield {
public:
    pHShield(SoftwareSerial &serial);  // Declaração do construtor
    float readPH();
    float readTemp();
    float readOffset();
};

#endif