#ifndef ACQUASHIELD_H_INCLUDED
#define ACQUASHIELD_H_INCLUDED

#include <Arduino.h>

// Definições comuns a ambos os sensores
#define StartCommand '#'
#define EndCommand '#'

// Endereços dos Shields
#define ShieldOD_addr 0xE7   // Endereço do Shield OD
#define ShieldEC_addr 0xE9   // Endereço do Shield EC

// Enumerações para tipo de exibição
enum od_disp {mgl, saturacao};    // Opções de impressão de OD
enum var_disp {temperatura, tds, salinidade};  // Opções de exibição para ECShield

// Variáveis globais para tipo de exibição
extern od_disp od_type;
extern var_disp var_type;

// Classe para o sensor pH
class pHShield
{
    public:
        init(void);                                                 // Inicializa o pHShield.
        float temp(void);                                               // Retorna o valor de temperatura lido no pHShield.
        float pH(void);                                                 // Retorna o valor de pH.
        void DispWrite(char font, int x, int y, char str[]);            // Escreve uma string no display do pHShield.
        void DispBuffClear(void);                                       // Apaga o buffer do display do pHShield.
        void DispClear(void);                                           // Apaga o display do pHShield.
        void DispShow(void);                                            // Envia os dados do buffer para a tela do display do pHShield.
};

// Classe para o sensor EC (Condutividade Elétrica)
class ECShield
{
    public:
        init(void);                                                 // Inicializa o ECShield.
        float temp(void);                                               // Retorna o valor de temperatura lido no ECShield.
        float EC(void);                                                 // Retorna o valor de EC (Condutividade Elétrica).
        float Salin(void);                                              // Retorna o valor de Salinidade.
        float Tds(void);                                                // Retorna o valor de TDS (Total Dissolved Solids).
        void DispWrite(char font, int x, int y, char str[]);            // Escreve uma string no display do ECShield.
        void DispBuffClear(void);                                       // Apaga o buffer do display do ECShield.
        void DispClear(void);                                           // Apaga o display do ECShield.
        void DispShow(void);                                            // Envia os dados do buffer para a tela do ECShield.
};

// Classe para o sensor OD (Oxigênio Dissolvido)
class ODShield
{
    public:
        init(void);                                                 // Inicializa o ODShield.
        float temp(void);                                               // Retorna o valor de temperatura lido no ODShield.
        float OD(void);                                                 // Retorna o valor de Oxigênio Dissolvido.
        float ODsat(void);                                              // Retorna o valor de Oxigênio Dissolvido Saturado.
        void DispWrite(char font, int x, int y, char str[]);            // Escreve uma string no display do ODShield.
        void DispBuffClear(void);                                       // Apaga o buffer do display do ODShield.
        void DispClear(void);                                           // Apaga o display do ODShield.
        void DispShow(void);                                            // Envia os dados do buffer para a tela do ODShield.
};

#endif
