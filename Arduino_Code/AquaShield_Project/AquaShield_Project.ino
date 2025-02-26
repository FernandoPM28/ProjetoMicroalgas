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

    // Limpa os Displays
    shieldPH.clearDisplayBuffer();
    shieldOD.clearDisplayBuffer();
    shieldEC.clearDisplayBuffer();

    shieldPH.clearDisplay();
    shieldOD.clearDisplay();
    shieldEC.clearDisplay();

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
    
    // Limpa os Displays
    shieldPH.clearDisplayBuffer();
    shieldOD.clearDisplayBuffer();
    shieldEC.clearDisplayBuffer();

    shieldPH.clearDisplay();
    shieldOD.clearDisplay();
    shieldEC.clearDisplay();

    Serial.println("\nInicialização concluída. Iniciando leituras...");
}

void loop() {
    static unsigned int previous_mod = 0;  // Contador de tempo do loop

    // ** Buffers para exibição dos valores **
    static char vetor_temp[6], vetor_pH[5], vetor_OD[12], vetor_EC[12], vetor_salin[6], vetor_tds[6];

    if (millis() - previous_mod >= 500) {

        // ** Leitura dos valores dos sensores **
        float ec = shieldEC.readEC();           // Condutividade Elétrica (EC)
        float tds = shieldEC.readTDS();         // Total de Sólidos Dissolvidos (TDS)
        float salin = shieldEC.readSalinity();  // Salinidade
        float ph = shieldPH.readPH();           // pH
        float od = shieldOD.readOD();           // Oxigênio Dissolvido (OD)
        float sat = shieldOD.readSaturation();
        float temp_EC = shieldEC.readTemp();
        float temp_OD = shieldOD.readTemp();
        float temp_PH = shieldPH.readTemp();

        // Determinar temperatura válida
        float temp = (temp_PH != 100.0) ? temp_PH : (temp_EC != 100.0) ? temp_EC : (temp_OD != 100.0) ? temp_OD : 100.0;

        // ** Exibir valores no Serial Monitor **
        Serial.print("EC: "); Serial.print(ec);
        Serial.print(" uS/cm | TDS: "); Serial.print(tds);
        Serial.print(" ppm | Salin: "); Serial.print(salin);
        Serial.print(" ppt | pH: "); Serial.print(ph);
        Serial.print(" | OD: "); Serial.print(od);
        Serial.print(" mg/L | Temp: "); Serial.print(temp);
        Serial.println(" °C");

        // Garantir que os valores sejam válidos
        if (isnan(ph)) ph = 0.0;
        if (isnan(ec) || ec == 500.0) ec = 0.0;
        if (isnan(od) || od == 500.0) od = 0.0;
        if (isnan(temp)) temp = 0.0;
        
        // ** Atualizando Shield de pH **
        dtostrf(ph, 4, 2, vetor_pH);  // Agora com 4 dígitos e 2 casas decimais
        shieldPH.writeToDisplay(4, 18, 18, "pH:");
        shieldPH.writeToDisplay(4, 60, 18, vetor_pH);

        dtostrf(temp, 4, 1, vetor_temp);
        shieldPH.writeToDisplay(1, 45, 31, "Temp:");
        shieldPH.writeToDisplay(1, 78, 31, vetor_temp);

        shieldPH.showDisplay();  // Atualiza o display do Shield de pH

        // ** Atualizando Shield de EC **
        if (ec < 100)
            dtostrf(ec, 5, 2, vetor_EC);
        else if (ec < 1000)
            dtostrf(ec, 5, 1, vetor_EC);
        else
            dtostrf(ec, 4, 0, vetor_EC);

        shieldEC.writeToDisplay(5, 0, 18, "EC:");
        shieldEC.writeToDisplay(4, 50, 18, vetor_EC);

        dtostrf(salin, 2, 2, vetor_salin);
        shieldEC.writeToDisplay(1, 38, 31, "Salin:");
        shieldEC.writeToDisplay(1, 78, 31, vetor_salin);

        shieldEC.showDisplay();  // Atualiza o display do Shield de EC

        // ** Atualizando Shield de OD **
        dtostrf(od, 5, 2, vetor_OD);
        strcpy(vetor_OD, vetor_OD);
        shieldOD.writeToDisplay(4, 6, 16, "OD:");
        shieldOD.writeToDisplay(4, 49, 16, vetor_OD);

        dtostrf(tds, 2, 1, vetor_tds);
        shieldOD.writeToDisplay(1, 45, 32, "TDS:");
        shieldOD.writeToDisplay(1, 78, 31, vetor_tds);

        shieldOD.showDisplay();  // Atualiza o display do Shield de OD

        previous_mod = millis();

    }
}
