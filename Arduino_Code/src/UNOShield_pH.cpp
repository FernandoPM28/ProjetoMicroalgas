#include "../include/AcquaShield.h"
#include <SoftwareSerial.h>


SoftwareSerial mySerial_pH(5, 4); //RX,TX


pHShield::init()
{
    mySerial_pH.begin(9600);
}

/*######################################################################################################################################################
Função responsável por retornar a leitura do sensor de temperatura do AcquaShied_pH.
Esta função retorna um valor float correspondente à leitura em graus celsius do DS18B20.
######################################################################################################################################################*/
float pHShield::temp(void)
{
     mySerial_pH.listen();  // Garante que o SoftwareSerial está escutando o pH Shield

     float temp = 0;
     unsigned int contador = 0;
     unsigned int rx_timeout = 0;
     char rx_vetor[6] = {0,0,0,0,0,0};
     
     mySerial_pH.write(0xE6);  //Envia o endereço do AcquaShield_pH pela UART.
     delay(10);
     mySerial_pH.write("tmp"); //Envia o comando para leitura da temperatura pelo AcquaShield.
     delay(10);
     mySerial_pH.write(0xED);   //Envia o caractere de finalização do comando.

     while((contador <= 6)&(rx_timeout < 2000)) //Aguarda receber os 6 bytes da leitura de temperatura e, caso de algum erro, verifica a variável de timeout.
     {
         if(mySerial_pH.available())                   //Verifica se recebeu um dado pela UART do AcquaShield_pH
         {
             rx_vetor[contador] = mySerial_pH.read();  //Adiciona o dado ao vetor de RX.
             contador++;                            
         }
         rx_timeout++;                       
         delay(1);
     }
     if(rx_timeout >= 2000)
     {
         return 99.999;
     }   
     else
     {
         temp = atof(rx_vetor);     
         return temp;
     }
}

/*######################################################################################################################################################
Função responsável por retornar a leitura do pH do AcquaShield_pH.
Esta função retorna um valor float correspondente à leitura do valor de pH.
######################################################################################################################################################*/
float pHShield::pH(void)
{
     mySerial_pH.listen();  // Garante que o SoftwareSerial está escutando o pH Shield

     float ph = 0;
     unsigned int contador = 0;
     unsigned int rx_timeout = 0;
     char rx_vetor[6] = {0,0,0,0,0,0};
     
     mySerial_pH.write(0xE6);  //Envia o endereço do AcquaShield_pH pela UART.
     delay(10);
     mySerial_pH.write("ph"); //Envia o comando para leitura da temperatura pelo AcquaShield.
     delay(10);
     mySerial_pH.write(0xED);   //Envia o caractere de finalização do comando.

     while((contador <= 6)&(rx_timeout < 2000)) //Aguarda receber os 6 bytes da leitura de temperatura e, caso de algum erro, verifica a variável de timeout.
     {
         if(mySerial_pH.available())                   //Verifica se recebeu um dado pela UART do AcquaShield_pH
         {
             rx_vetor[contador] = mySerial_pH.read();  //Adiciona o dado ao vetor de RX.
             contador++;                            
         }
         rx_timeout++;                       
         delay(1);
     }
     if(rx_timeout >= 2000)
     {
         return 99.999;
     }   
     else
     {
         ph = atof(rx_vetor);     
         return ph;
     }
}


/*######################################################################################################################################################
Função responsável por escrever uma String no diplay 128x32 do AcquaShield_pH
######################################################################################################################################################*/
void pHShield::DispWrite(char font, int x, int y, char str[])
{
     mySerial_pH.listen();  // Garante que o SoftwareSerial está escutando o pH Shield
     unsigned int i = 0;
     
     mySerial_pH.write(0xE6);  //Envia o endereço do AcquaShield_pH pela UART.
     delay(10);
     mySerial_pH.write("dsp");
     delay(10);
     mySerial_pH.write(font);
     delay(10);
     mySerial_pH.write(x);  
     delay(10);
     mySerial_pH.write(y); 
     delay(10);
     mySerial_pH.write(str); 
     delay(10);
     mySerial_pH.write(0xED);   //Envia o caractere de finalização do comando.   
}

/*######################################################################################################################################################
Função responsável por apagar o buffer do diplay 128x32 do AcquaShield_pH
######################################################################################################################################################*/
void pHShield::DispBuffClear(void)
{
     mySerial_pH.listen();  // Garante que o SoftwareSerial está escutando o pH Shield    
     mySerial_pH.write(0xE6);  //Envia o endereço do AcquaShield_pH pela UART.
     delay(10);
     mySerial_pH.write("bff");
     delay(10);
     mySerial_pH.write(0xED);   //Envia o caractere de finalização do comando.    
}

/*######################################################################################################################################################
Função responsável por apagar os dados do diplay 128x32 do AcquaShield_pH
######################################################################################################################################################*/
void pHShield::DispClear(void)
{    
     mySerial_pH.listen();  // Garante que o SoftwareSerial está escutando o pH Shield
     mySerial_pH.write(0xE6);  //Envia o endereço do AcquaShield_pH pela UART.
     delay(10);
     mySerial_pH.write("clr");
     delay(10);
     mySerial_pH.write(0xED);   //Envia o caractere de finalização do comando.    
}

/*######################################################################################################################################################
Função responsável por mostras os caracteres no diplay 128x32 do AcquaShield_pH
######################################################################################################################################################*/
void pHShield::DispShow(void)
{    
     mySerial_pH.listen();  // Garante que o SoftwareSerial está escutando o pH Shield
     mySerial_pH.write(0xE6);  //Envia o endereço do AcquaShield_pH pela UART.
     delay(10);
     mySerial_pH.write("snd");
     delay(10);
     mySerial_pH.write(0xED);   //Envia o caractere de finalização do comando.    
}

/*######################################################################################################################################################
Função responsável por chamar a rotina de calibração no pH_Shield
######################################################################################################################################################*/
void pH_Cal(void)
{
    unsigned char uart_byte = 0;        //Recebe o byte recebido pela UART.
    unsigned char serial_byte = 0;      //Recebe o byte recebido pela UART.
    
    mySerial_pH.listen();  // Garante que o SoftwareSerial está escutando o pH Shield  
    mySerial_pH.write(0xE6);  //Envia o endereço do AcquaShield_pH pela UART.
    delay(10);
    mySerial_pH.write("Cal");
    delay(10);
    mySerial_pH.write(0xED);   //Envia o caractere de finalização do comando.   


    while(true) //Aguarda receber o byte de finalização da rotida de calibração do Shield de pH ('F').
    {                  
        if(mySerial_pH.available())                   //Verifica se recebeu um dado pela UART do AcquaShield_pH
        {
            uart_byte = mySerial_pH.read();
            Serial.write(uart_byte);                            
        }

        if(Serial.available())                   //Verifica se recebeu um dado pela UART do AcquaShield_pH
        {
            serial_byte = Serial.read();   
            mySerial_pH.write(serial_byte);  
        }
        if(serial_byte == 'F')
        {
            Serial.write("Rotina de calibração finalizada, reinicie o sistema...");
            Serial.println(); 
            while(true)
            { 
                delay(100);
            }
        }
    } 
}

/*######################################################################################################################################################
Função responsável por retornar o valor de Offset do eletrodo de pH no pH_Shield
######################################################################################################################################################*/
float pH_offset(void)
{
     mySerial_pH.listen();  // Garante que o SoftwareSerial está escutando o pH Shield
     float ph_offset = 0;
     unsigned int contador = 0;
     unsigned int rx_timeout = 0;
     char rx_vetor[6] = {0,0,0,0,0,0};
    
     mySerial_pH.write(0xE6);  //Envia o endereço do AcquaShield_pH pela UART.
     delay(10);
     mySerial_pH.write("Oft"); //Envia o comando para leitura do OffSet pelo AcquaShield.
     delay(10);
     mySerial_pH.write(0xED);   //Envia o caractere de finalização do comando.
     
     while((contador <= 6)&(rx_timeout < 2000)) //Aguarda receber os 6 bytes da leitura de temperatura e, caso de algum erro, verifica a variável de timeout.
     {
         if(mySerial_pH.available())                   //Verifica se recebeu um dado pela UART do AcquaShield_pH
         {
             rx_vetor[contador] = mySerial_pH.read();  //Adiciona o dado ao vetor de RX.
             contador++;                            
         }
         rx_timeout++;                       
         delay(1);
     }
     if(rx_timeout >= 2000)
     {
         return 99.999;
     }   
     else
     {
         ph_offset = atof(rx_vetor);     
         return ph_offset;
     }    
}

/*######################################################################################################################################################
Função responsável por retornar o valor do Sloope (pH04) do eletrodo de pH no pH_Shield.
######################################################################################################################################################*/
float pH_sloope_04(void) 
{
     mySerial_pH.listen();  // Garante que o SoftwareSerial está escutando o pH Shield
     float ph_sl04 = 0;
     unsigned int contador = 0;
     unsigned int rx_timeout = 0;
     char rx_vetor[6] = {0,0,0,0,0,0};
    
     mySerial_pH.write(0xE6);  //Envia o endereço do AcquaShield_pH pela UART.
     delay(10);
     mySerial_pH.write("Sl04"); //Envia o comando para leitura do sloope pH04 pelo AcquaShield.
     delay(10);
     mySerial_pH.write(0xED);   //Envia o caractere de finalização do comando.
     
     while((contador <= 6)&(rx_timeout < 2000)) //Aguarda receber os 6 bytes da leitura de temperatura e, caso de algum erro, verifica a variável de timeout.
     {
         if(mySerial_pH.available())                   //Verifica se recebeu um dado pela UART do AcquaShield_pH
         {
             rx_vetor[contador] = mySerial_pH.read();  //Adiciona o dado ao vetor de RX.
             contador++;                            
         }
         rx_timeout++;                       
         delay(1);
     }
     if(rx_timeout >= 2000)
     {
         return 99.999;
     }   
     else
     {
         ph_sl04 = atof(rx_vetor);     
         return ph_sl04;
     }    
}

/*######################################################################################################################################################
Função responsável por retornar o valor do Sloope (pH10) do eletrodo de pH no pH_Shield.
######################################################################################################################################################*/
float pH_sloope_10(void) 
{
     mySerial_pH.listen();  // Garante que o SoftwareSerial está escutando o pH Shield

     float ph_sl10 = 0;
     unsigned int contador = 0;
     unsigned int rx_timeout = 0;
     char rx_vetor[6] = {0,0,0,0,0,0};
    
     mySerial_pH.write(0xE6);  //Envia o endereço do AcquaShield_pH pela UART.
     delay(10);
     mySerial_pH.write("Sl10"); //Envia o comando para leitura do sloope pH04 pelo AcquaShield.
     delay(10);
     mySerial_pH.write(0xED);   //Envia o caractere de finalização do comando.
     
     while((contador <= 6)&(rx_timeout < 2000)) //Aguarda receber os 6 bytes da leitura de temperatura e, caso de algum erro, verifica a variável de timeout.
     {
         if(mySerial_pH.available())                   //Verifica se recebeu um dado pela UART do AcquaShield_pH
         {
             rx_vetor[contador] = mySerial_pH.read();  //Adiciona o dado ao vetor de RX.
             contador++;                            
         }
         rx_timeout++;                       
         delay(1);
     }
     if(rx_timeout >= 2000)
     {
         return 99.999;
     }   
     else
     {
         ph_sl10 = atof(rx_vetor);     
         return ph_sl10;
     }    
}

/*######################################################################################################################################################
Função responsável por retornar o valor da temperatura de calibração do Shield de pH.
######################################################################################################################################################*/
float pH_CalTemp(void) 
{
     mySerial_pH.listen();  // Garante que o SoftwareSerial está escutando o pH Shield
     float CalTemp = 0;
     unsigned int contador = 0;
     unsigned int rx_timeout = 0;
     char rx_vetor[6] = {0,0,0,0,0,0};
    
     mySerial_pH.write(0xE6);  //Envia o endereço do AcquaShield_pH pela UART.
     delay(10);
     mySerial_pH.write("Ctmp"); //Envia o comando para leitura do sloope pH04 pelo AcquaShield.
     delay(10);
     mySerial_pH.write(0xED);   //Envia o caractere de finalização do comando.
     
     while((contador <= 6)&(rx_timeout < 2000)) //Aguarda receber os 6 bytes da leitura de temperatura e, caso de algum erro, verifica a variável de timeout.
     {
         if(mySerial_pH.available())                   //Verifica se recebeu um dado pela UART do AcquaShield_pH
         {
             rx_vetor[contador] = mySerial_pH.read();  //Adiciona o dado ao vetor de RX.
             contador++;                            
         }
         rx_timeout++;                       
         delay(1);
     }
     if(rx_timeout >= 2000)
     {
         return 99.999;
     }   
     else
     {
         CalTemp = atof(rx_vetor);     
         return CalTemp;
     }    
}

/*######################################################################################################################################################
Função responsável por receber os dados da UART, vindos do Arduino UNO,a tratá-los conforme suas respectivas funções.
Esta função é chamada toda vez que um byte é recebudo pela UART e um buffer é preenchido com os dados para posterior tratamento.
######################################################################################################################################################*/
void serialEvent(void)
{
    unsigned char uart_byte = 0;         //Recebe o byte recebido pela UART.
    unsigned char uart_vetor[16];        //Vetor para armazenar os bytes de comando.
    unsigned int cont_comando = 0;       //Contador para indexar o vetor de comando.
    static boolean verif_addr = false;   //Bit para controle de acesso à função de comandos.
    unsigned int i = 0;
    static char vetor_serial[5];
    
    int cont_teste = 0;
    
    mySerial_pH.listen();  // Garante que o SoftwareSerial está escutando o pH Shield

    while(Serial.available() > 0)  //Aguarda o buffer da UART ser lido totalmente.
    {
        uart_byte = Serial.read();
    
        if((uart_byte == EndCommand)&(verif_addr == true))                               //Verifica se o comando de finalização chegou pela UART.
        {   
            for(i = (cont_comando); i < 16; i++)  
                uart_vetor[i] = '\0';
            
            if((uart_vetor[0] == 'p')&(uart_vetor[1] == 'H')&(uart_vetor[2] == 'C')&(uart_vetor[3] == 'a')&(uart_vetor[4] == 'l')) 
            {
                pH_Cal();
            }       
            else
                if((uart_vetor[0] == 'p')&(uart_vetor[1] == 'H')&(uart_vetor[2] == 'O')&(uart_vetor[3] == 'f')&(uart_vetor[4] == 't')) 
                {
                    dtostrf((pH_offset()*1000.0), 2, 1, vetor_serial);
                    strcpy(vetor_serial, vetor_serial);
                    Serial.write("pH Offset: ");
                    Serial.write(vetor_serial);
                    Serial.write("mV");
                    Serial.println();
                }
                else       
                    if((uart_vetor[0] == 'p')&(uart_vetor[1] == 'H')&(uart_vetor[2] == 'S')&(uart_vetor[3] == 'l')&(uart_vetor[4] == 'p')&(uart_vetor[5] == '0')&(uart_vetor[6] == '4')) 
                    {
                        dtostrf(pH_sloope_04(), 2, 1, vetor_serial);
                        strcpy(vetor_serial, vetor_serial);
                        Serial.write("Sloope pH04: ");
                        Serial.write(vetor_serial);
                        Serial.write("%");
                        Serial.println();
                    }
                    else     
                        if((uart_vetor[0] == 'p')&(uart_vetor[1] == 'H')&(uart_vetor[2] == 'S')&(uart_vetor[3] == 'l')&(uart_vetor[4] == 'p')&(uart_vetor[5] == '1')&(uart_vetor[6] == '0')) 
                        {
                            dtostrf(pH_sloope_10(), 2, 1, vetor_serial);
                            strcpy(vetor_serial, vetor_serial);
                            Serial.write("Sloope pH10: ");-
                            Serial.write(vetor_serial);
                            Serial.write("%");
                            Serial.println();
                        }
                        else     
                            if((uart_vetor[0] == 'p')&(uart_vetor[1] == 'H')&(uart_vetor[2] == 'C')&(uart_vetor[3] == 'a')&(uart_vetor[4] == 't')&(uart_vetor[5] == 'm')&(uart_vetor[6] == 'p')) 
                            {
                                dtostrf(pH_CalTemp(), 2, 1, vetor_serial);
                                strcpy(vetor_serial, vetor_serial);
                                Serial.write("Temperatura(Cal): ");
                                Serial.write(vetor_serial);
                                Serial.write("ºC");
                                Serial.println();
                            }
                            else
                                if((uart_vetor[0] == 'p')&(uart_vetor[1] == 'H')&(uart_vetor[2] == 'C')&(uart_vetor[3] == 'e')&(uart_vetor[4] == 'r')&(uart_vetor[5] == 'a')&(uart_vetor[6] == 's')) 
                                {
                                    mySerial_pH.write(0xE6);  //Envia o endereço do AcquaShield_pH pela UART.
                                    delay(10);
                                    mySerial_pH.write("Cers"); //Envia o comando para leitura do sloope pH04 pelo AcquaShield.
                                    delay(10);
                                    mySerial_pH.write(0xED);   //Envia o caractere de finalização do comando.   

                                    Serial.write("Calibração pH apagada !");
                                    Serial.println();
                                }
                                else         
                                    Serial.write("Comando não válido !");
            
            verif_addr = false;
            cont_comando = 0;    
        }
        if(verif_addr == true)    //Carrega o vetor da UART enquanto o comando de finalização não chegar.
        {   
            uart_vetor[cont_comando] = uart_byte;
            cont_comando++;  
            if(cont_comando > 16)  //Verifica se os bytes de comando estouraram o contador de comandos.
            {
                Serial.write("Estouro de comando !");
                cont_comando = 0;    
            }
        }
        if((uart_byte == StartCommand) & (verif_addr == false))  //Verifica se o byte recebido corresponde ao endereço do shield pH.
        {
            verif_addr = true;                                   //Ativa o bit de verificação de acesso. 
            cont_comando = 0;
        }
    }
    verif_addr = false;
    cont_comando = 0;   
    for(i = 0; i < 16; i++)
        uart_vetor[i] = '\0';
}
