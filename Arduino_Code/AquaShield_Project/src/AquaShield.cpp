#include "../include/AcquaShield.h"

// Implementação do construtor da classe base Shield
Shield::Shield(SoftwareSerial &serial, uint8_t address) : serial(serial), address(address) {}

// Implementação do método init()
void Shield::init() {
    serial.begin(9600);  // Inicializa a comunicação serial com o shield
}

// Implementação da função para ler dados do shield
float Shield::readData(const char *command) {
    serial.listen();  // Garante que o canal correto está ativo
    float value = 0.0;
    char rx_buffer[10] = {0};  // Buffer maior para evitar truncamento
    unsigned long start_time = millis(); // Marca o tempo de início

    // Envia comando para o shield
    serial.write(address);
    delay(10);
    serial.write(command);
    delay(10);
    serial.write(0xED);

    // Aguarda e recebe os dados
    int count = 0;
    while (millis() - start_time < 2000) {  // Timeout de 2 segundos
        if (serial.available()) {
            char c = serial.read();
            if (c == '\n' || c == '\r') break;  // Final de linha
            if (count < sizeof(rx_buffer) - 1) {
                rx_buffer[count++] = c;
            }
        }
    }
    rx_buffer[count] = '\0';  // Garante terminação da string

    // Serial.print("Recebido: ");
    // Serial.println(rx_buffer);  // Exibe no Serial Monitor
    
    if( *rx_buffer == '\0' ){
        // Serial.print("Não recebeu valor");
        value = 100.0;
    }
    else{
        value = atof(rx_buffer);
    }
    return value;
}


// Implementação da função para escrever no display do shield
void Shield::writeToDisplay(char font, int x, int y, const char *str) {
    serial.listen();  // Garante que o SoftwareSerial está escutando o shield correto
    serial.write(address);
    delay(10);
    serial.write("dsp");
    delay(10);
    serial.write(font);
    delay(10);
    serial.write(x);
    delay(10);
    serial.write(y);
    delay(10);
    serial.write(str);
    delay(10);
    serial.write(0xED);
}

// Implementação da função para limpar o buffer do display
void Shield::clearDisplayBuffer() {
    serial.listen();  // Garante que o SoftwareSerial está escutando o shield correto
    serial.write(address);
    delay(10);
    serial.write("bff");
    delay(10);
    serial.write(0xED);
}

// Implementação da função para limpar o display
void Shield::clearDisplay() {
    serial.listen();  // Garante que o SoftwareSerial está escutando o shield correto
    serial.write(address);
    delay(10);
    serial.write("clr");
    delay(10);
    serial.write(0xED);
}

// Implementação da função para mostrar os dados no display
void Shield::showDisplay() {
    serial.listen();  // Garante que o SoftwareSerial está escutando o shield correto
    serial.write(address);
    delay(10);
    serial.write("snd");
    delay(10);
    serial.write(0xED);
}