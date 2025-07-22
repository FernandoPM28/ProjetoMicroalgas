#include "include/AcquaShield.h"
#include <Wire.h>
#include <SoftwareSerial.h>
#include <math.h> // Para a função isnan()

// --- CONFIGURAÇÕES ---
#define I2C_ADDR 9 // Endereço I2C deste Arduino (escravo)

// --- PINAGEM E INSTÂNCIAS ---
SoftwareSerial mySerial_EC(5, 4); // RX, TX para EC Shield
SoftwareSerial mySerial_OD(7, 6); // RX, TX para OD Shield

ECShield shieldEC(mySerial_EC);
ODShield shieldOD(mySerial_OD);

// --- ESTRUTURA DE DADOS ---
// Esta struct deve ser IDÊNTICA à definida no código do ESP32 (mestre).
// Ela agrupa todos os dados que este Arduino enviará.
struct DadosSensores {
  float ec;
  float tds;
  float salinidade;
  float od;
  float temperatura;
};

// --- VARIÁVEIS GLOBAIS ---
// Cria uma instância global da struct. É esta variável que será enviada via I2C.
DadosSensores dados_sensores_para_envio;

void setup() {
    Serial.begin(9600);
    Serial.println("Iniciando Escravo I2C - Sensores EC e OD...");

    // Inicializa I2C como escravo no endereço definido
    Wire.begin(I2C_ADDR);
    // Registra a função que será chamada quando o mestre requisitar dados
    Wire.onRequest(responderRequisicao);

    // Inicializa os shields
    shieldEC.init();
    shieldOD.init();

    // Limpa os Displays
    shieldOD.clearDisplayBuffer();
    shieldEC.clearDisplayBuffer();
    shieldOD.clearDisplay();
    shieldEC.clearDisplay();

    // Mensagens iniciais
    shieldEC.writeToDisplay(3, 13, 21, "EC Shield");
    shieldEC.showDisplay();
    delay(500);
    shieldOD.writeToDisplay(3, 13, 21, "OD Shield");
    shieldOD.showDisplay();
    delay(500);

    shieldOD.clearDisplayBuffer();
    shieldEC.clearDisplayBuffer();
    shieldOD.clearDisplay();
    shieldEC.clearDisplay();

    Serial.println("\nInicializacao concluida. Iniciando leituras...");
}

/**
 * @brief Função de callback do I2C.
 *
 * Esta função é executada automaticamente quando o mestre (ESP32)
 * faz uma requisição de dados. Ela envia o conteúdo da struct
 * 'dados_sensores_para_envio' como um bloco de bytes.
 */
void responderRequisicao() {
  // Envia os bytes da struct. Este método é muito mais eficiente
  // e confiável do que enviar uma string formatada.
  Wire.write((uint8_t *)&dados_sensores_para_envio, sizeof(dados_sensores_para_envio));
}

void loop() {
    // Trata comandos de calibração via Serial (sua lógica original mantida)
    static String serialBuffer;
    while (Serial.available()) {
        char c = Serial.read();
        if (c == '#') {
            if (serialBuffer.length() > 0) {
                if (serialBuffer == "ECParam") shieldEC.readCalibrationParameters();
                else if (serialBuffer == "ODParam") shieldOD.readCalibrationParameters();
                else if (serialBuffer == "ECCal") shieldEC.ECCalibrate();
                else if (serialBuffer == "ODCal") shieldOD.ODCalibrate();
                else if (serialBuffer == "ECReset") shieldEC.resetCal();
                else if (serialBuffer == "ODReset") shieldOD.resetCal();
                serialBuffer = "";
            }
        } else {
            serialBuffer += c;
        }
    }

    static unsigned long previous_mod = 0;
    if (millis() - previous_mod >= 2000) { // Aumentei o delay para 2s para facilitar a leitura no serial
        previous_mod = millis();

        shieldEC.handleSerialInput();
        shieldOD.handleSerialInput();

        // 1. Leitura dos sensores
        float ec_lido = shieldEC.readEC();
        float tds_lido = shieldEC.readTDS();
        float salin_lido = shieldEC.readSalinity();
        float od_lido = shieldOD.readOD();
        float temp_EC = shieldEC.readTemp();
        float temp_OD = shieldOD.readTemp();
        float temp_final = 0.0;

        Serial.println(od_lido);

        // 2. Seleciona temperatura confiável (sua lógica original mantida)
        static float ultimaTempValida = 0.0;
        if (temp_EC >= 0.0 && temp_EC < 100.0) {
            temp_final = temp_EC;
            ultimaTempValida = temp_EC;
        } else if (temp_OD >= 0.0 && temp_OD < 100.0) {
            temp_final = temp_OD;
            ultimaTempValida = temp_OD;
        } else {
            temp_final = ultimaTempValida;
        }

        // 3. Preenche a struct 'dados_sensores_para_envio' com os valores lidos e tratados
        dados_sensores_para_envio.ec = (isnan(ec_lido) || ec_lido == 500.0) ? 0.0 : ec_lido;
        dados_sensores_para_envio.tds = (isnan(tds_lido)) ? 0.0 : tds_lido;
        dados_sensores_para_envio.salinidade = (isnan(salin_lido)) ? 0.0 : salin_lido;
        dados_sensores_para_envio.od = (isnan(od_lido) || od_lido == 500.0) ? 0.0 : od_lido;
        dados_sensores_para_envio.temperatura = (isnan(temp_final)) ? 0.0 : temp_final;

        // 4. Exibe no Serial Monitor para depuração local
        Serial.println("--- Dados Atuais ---");
        Serial.print("EC: "); Serial.print(dados_sensores_para_envio.ec);
        Serial.print(" uS/cm | TDS: "); Serial.print(dados_sensores_para_envio.tds);
        Serial.print(" ppm | Salin: "); Serial.print(dados_sensores_para_envio.salinidade);
        Serial.print(" | OD: "); Serial.print(dados_sensores_para_envio.od);
        Serial.print(" mg/L | Temp: "); Serial.print(dados_sensores_para_envio.temperatura);
        Serial.println(" C");
        Serial.println("--------------------");

        // 5. Exibir nos Displays (sua lógica original mantida)
        char vetor_temp[6], vetor_OD[12], vetor_EC[12], vetor_salin[6], vetor_tds[6];
        float ec_display = dados_sensores_para_envio.ec;
        if (ec_display < 100) dtostrf(ec_display, 5, 2, vetor_EC);
        else if (ec_display < 1000) dtostrf(ec_display, 5, 1, vetor_EC);
        else dtostrf(ec_display, 4, 0, vetor_EC);

        dtostrf(dados_sensores_para_envio.salinidade, 4, 2, vetor_salin);
        dtostrf(dados_sensores_para_envio.od, 5, 2, vetor_OD);
        dtostrf(dados_sensores_para_envio.tds, 4, 2, vetor_tds);
        dtostrf(dados_sensores_para_envio.temperatura, 4, 1, vetor_temp);

        shieldEC.writeToDisplay(1, 6, 10, "EC:");
        shieldEC.writeToDisplay(1, 25, 10, vetor_EC);
        shieldEC.writeToDisplay(1, 6, 25, "Salin:");
        shieldEC.writeToDisplay(1, 45, 25, vetor_salin);
        shieldEC.showDisplay();

        shieldOD.writeToDisplay(1, 6, 10, "OD:");
        shieldOD.writeToDisplay(1, 25, 10, vetor_OD);
        shieldOD.writeToDisplay(1, 6, 25, "TDS:");
        shieldOD.writeToDisplay(1, 40, 25, vetor_tds);
        shieldOD.writeToDisplay(1, 60, 10, "Temp:");
        shieldOD.writeToDisplay(1, 95, 10, vetor_temp);
        shieldOD.showDisplay();
    }
}