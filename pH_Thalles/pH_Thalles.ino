#include <Wire.h>
#include <math.h>

// --- CONFIGURAÇÕES ---
#define I2C_ADDR 8      // Endereço I2C deste Arduino (escravo)
const int pinSensor = A0; // Pino analógico onde o sensor de pH está conectado

// --- ESTRUTURA DE DADOS ---
struct DadosPH {
  uint16_t ph; // Armazenaremos o pH como um inteiro (ex: 7.25 será enviado como 725)
};

// --- VARIÁVEIS GLOBAIS ---
// Coeficientes para a CALIBRAÇÃO DIRETA da TENSÃO BRUTA para o pH
// ATENÇÃO: ESTES SÃO OS NOVOS VALORES CALCULADOS COM OS DADOS DE 18/07/2025
// COM BASE NA CURVA DE TENSÃO INVERSA. POLINÔMIO DE GRAU 3.
const float a = 3.037;
const float b = -20.334;
const float c = 34.577;
const float d = -0.646;

// Cria uma instância global da struct. É esta variável que será enviada via I2C.
DadosPH dados_ph_para_envio;

void setup() {
  Serial.begin(9600);
  Serial.println("Iniciando Escravo I2C - Sensor de pH (Calibracao Inversa)...");

  Wire.begin(I2C_ADDR);
  Wire.onRequest(responderRequisicao);

  Serial.println("Pronto para receber requisicoes do mestre.");
}

void loop() {
  // 1. Lê o valor bruto do pino analógico
  int leitura_analogica = analogRead(pinSensor);

  // 2. Converte a leitura para TENSÃO BRUTA
  float tensao_bruta = leitura_analogica * (5.0 / 1023.0);

  // 3. Calcula o pH usando a equação de grau 3, diretamente da TENSÃO BRUTA
  float v = tensao_bruta; // Usando uma variável mais curta para a equação
  float ph_calculado = a * pow(tensao_bruta, 3) + b * pow(tensao_bruta, 2) + c * tensao_bruta + d;


  // 4. Preenche a struct com o valor calculado e formatado
  // Garante que o valor não seja negativo e limita ao máximo/mínimo de pH comum
  if (ph_calculado < 0.0) {
    ph_calculado = 0.0;
  }
  if (ph_calculado > 14.0) { // pH geralmente não vai além de 14.00
    ph_calculado = 14.0;
  }
  dados_ph_para_envio.ph = static_cast<uint16_t>(ph_calculado * 100);

  // 5. Exibe os valores no monitor serial local para depuração
  Serial.print("Leitura Analogica: ");
  Serial.print(leitura_analogica);
  Serial.print(" | Tensao Bruta: ");
  Serial.print(tensao_bruta, 3);
  Serial.print(" V | pH Calculado: ");
  Serial.print(ph_calculado, 2);
  Serial.print(" | Valor a ser enviado via I2C: ");
  Serial.println(dados_ph_para_envio.ph);

  delay(2000); // Aguarda 2 segundos antes da próxima leitura
}

/**
 * @brief Função de callback do I2C.
 */
void responderRequisicao() {
  Wire.write((uint8_t *)&dados_ph_para_envio, sizeof(dados_ph_para_envio));
}