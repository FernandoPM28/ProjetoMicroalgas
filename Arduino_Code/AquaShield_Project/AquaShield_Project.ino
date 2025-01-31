#include <Arduino.h>
#include "include/AcquaShield.h"

// Instâncias dos shields
ECShield Shield_EC;   // Instância do Shield de EC
pHShield Shield_pH;   // Instância do Shield de pH
ODShield Shield_OD;   // Instância do Shield de OD

void setup() 
{
    Serial.begin(9600);  // Inicializa o Serial Monitor

    // Inicializa os shields
    Shield_EC.init();    // Inicializa o Shield de EC
    Shield_pH.init();    // Inicializa o Shield de pH
    Shield_OD.init();    // Inicializa o Shield de OD

    // Mensagens iniciais nos displays
    Shield_EC.DispBuffClear();
    Shield_EC.DispWrite(3, 13, 21, "EC Shield");
    Shield_EC.DispShow();
    delay(1000);

    Shield_pH.DispBuffClear();
    Shield_pH.DispWrite(3, 13, 21, "pH Shield");
    Shield_pH.DispShow();
    delay(1000);

    Shield_OD.DispBuffClear();
    Shield_OD.DispWrite(3, 13, 21, "OD Shield");
    Shield_OD.DispShow();
    delay(1000);
}

void loop() 
{
    static unsigned long previous_mod_ec = 0;  // Controle do intervalo do Shield EC
    static unsigned long previous_mod_ph = 0;  // Controle do intervalo do Shield pH
    static unsigned long previous_mod_od = 0;  // Controle do intervalo do Shield OD
    const unsigned long interval = 1000;       // Intervalo de 1 segundo para cada shield

    // Atualiza os valores do Shield EC
    if (millis() - previous_mod_ec >= interval) 
    {
        previous_mod_ec = millis();

        // Limpa o buffer do display do Shield EC
        Shield_EC.DispBuffClear();

        // Leitura de Condutividade Elétrica (EC).
        float ec = Shield_EC.EC();
        char ec_str[12];
        dtostrf(ec, 5, (ec < 100) ? 2 : (ec < 1000) ? 1 : 0, ec_str); // Ajusta casas decimais.
        Shield_EC.DispWrite(2, 0, 0, "EC (uS/cm):");
        Shield_EC.DispWrite(2, 80, 0, ec_str); // Mostra EC no display.

        // Leitura de Temperatura.
        float temp = Shield_EC.temp();
        char temp_str[6];
        dtostrf(temp, 2, 1, temp_str); // Converte temperatura para string.
        Shield_EC.DispWrite(1, 0, 10, "Temp (C):");
        Shield_EC.DispWrite(1, 70, 10, temp_str); // Mostra temperatura no display.

        // Leitura de TDS.
        float tds = Shield_EC.Tds();
        char tds_str[12];
        dtostrf(tds, 5, (tds < 100) ? 2 : (tds < 1000) ? 1 : 0, tds_str); // Ajusta casas decimais.
        Shield_EC.DispWrite(1, 0, 20, "TDS (ppm):");
        Shield_EC.DispWrite(1, 70, 20, tds_str); // Mostra TDS no display.

        // Leitura de Salinidade.
        float salin = Shield_EC.Salin();
        char salin_str[12];
        dtostrf(salin, 5, 2, salin_str); // Ajusta casas decimais.
        Shield_EC.DispWrite(1, 0, 30, "Salin (ppt):");
        Shield_EC.DispWrite(1, 70, 30, salin_str); // Mostra Salinidade no display.

        // Atualiza o display.
        Shield_EC.DispShow();

        // Envia os dados para o Serial Monitor.
        Serial.print("EC (uS/cm): ");
        Serial.print(ec);
        Serial.print(" | Temp (C): ");
        Serial.print(temp);
        Serial.print(" | TDS (ppm): ");
        Serial.print(tds);
        Serial.print(" | Salin (ppt): ");
        Serial.println(salin);
    }

    // Atualiza os valores do Shield pH
    if (millis() - previous_mod_ph >= interval) 
    {
        previous_mod_ph = millis();

        // Limpa o buffer do display do Shield pH
        Shield_pH.DispBuffClear();

        // Leitura de pH
        float phValue = Shield_pH.pH();
        char ph_str[12];
        dtostrf(phValue, 2, 1, ph_str);
        Shield_pH.DispWrite(1, 0, 0, "pH:");
        Shield_pH.DispWrite(1, 70, 0, ph_str);

        // Leitura de temperatura
        float temp = Shield_pH.temp();
        char temp_str[12];
        dtostrf(temp, 2, 1, temp_str);
        Shield_pH.DispWrite(2, 0, 10, "Temp (C):");
        Shield_pH.DispWrite(2, 70, 10, temp_str);

        // Atualiza o display do Shield pH
        Shield_pH.DispShow();

        // Envia os dados para o Serial Monitor
        Serial.print("pH: ");
        Serial.print(phValue);
        Serial.print(" | Temp (C): ");
        Serial.println(temp);
    }

    // Atualiza os valores do Shield OD
    if (millis() - previous_mod_od >= interval) 
    {
        previous_mod_od = millis();

        // Limpa o buffer do display do Shield OD
        Shield_OD.DispBuffClear();

        // Leitura de OD
        float od = Shield_OD.OD();
        char od_str[12];
        dtostrf(od, 2, 1, od_str);
        Shield_OD.DispWrite(4, 6, 16, "OD:");
        Shield_OD.DispWrite(4, 49, 16, od_str);
        Shield_OD.DispWrite(2, 96, 16, "mg/L");

        // Leitura de temperatura
        float temp = Shield_OD.temp();
        char temp_str[12];
        dtostrf(temp, 2, 1, temp_str);
        Shield_OD.DispWrite(1, 7, 32, "Temp:");
        Shield_OD.DispWrite(1, 40, 32, temp_str);

        // Atualiza o display do Shield OD
        Shield_OD.DispShow();

        // Envia os dados para o Serial Monitor
        Serial.print("OD (mg/L): ");
        Serial.print(od);
        Serial.print(" | Temp (C): ");
        Serial.println(temp);
    }
}
