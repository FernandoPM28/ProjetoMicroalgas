#include "../include/AcquaShield.h"
#include <SoftwareSerial.h>


SoftwareSerial mySerial_OD(7, 6); //RX,TX

//iniciaiza variavel global para opcao de impressao de OD na unidade mg/L
od_disp od_type = mgl;

ODShield::init()
{
    mySerial_OD.begin(9600);
}


/*######################################################################################################################################################
Função responsável por retornar a leitura do sensor de temperatura do AcquaShied_pH.
Esta função retorna um valor float correspondente à leitura em graus celsius do DS18B20.
######################################################################################################################################################*/
float ODShield::temp(void)
{

     mySerial_OD.listen();  // Garante que o SoftwareSerial está escutando o OD Shield
     float temp = 0;
     unsigned int contador = 0;
     unsigned int rx_timeout = 0;
     char rx_vetor[6] = {0,0,0,0,0,0};
     
     mySerial_OD.write(ShieldOD_addr);  //Envia o endereço do AcquaShield_pH pela UART.
     delay(10);
     mySerial_OD.write("tmp"); //Envia o comando para leitura da temperatura pelo AcquaShield.
     delay(10);
     mySerial_OD.write(0xED);   //Envia o caractere de finalização do comando.

     while((contador <= 6)&(rx_timeout < 2000)) //Aguarda receber os 6 bytes da leitura de temperatura e, caso de algum erro, verifica a variável de timeout.
     {
         if(mySerial_OD.available())                   //Verifica se recebeu um dado pela UART do AcquaShield_pH
         {
             rx_vetor[contador] = mySerial_OD.read();  //Adiciona o dado ao vetor de RX.
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
Função responsável por retornar a leitura do OD do AcquaShield_OD.
Esta função retorna um valor float correspondente à leitura do valor de OD.
######################################################################################################################################################*/
float ODShield::OD(void)
{
     mySerial_OD.listen();  // Garante que o SoftwareSerial está escutando o OD Shield

     float od = 0;
     unsigned int contador = 0;
     unsigned int rx_timeout = 0;
     char rx_vetor[6] = {0,0,0,0,0,0};
     
     mySerial_OD.write(ShieldOD_addr);  //Envia o endereço do AcquaShield_pH pela UART.
     delay(10);
     mySerial_OD.write("odmgl"); //Envia o comando para leitura da temperatura pelo AcquaShield.
     delay(10);
     mySerial_OD.write(0xED);   //Envia o caractere de finalização do comando.

     while((contador <= 6)&(rx_timeout < 2000)) //Aguarda receber os 6 bytes da leitura de temperatura e, caso de algum erro, verifica a variável de timeout.
     {
         if(mySerial_OD.available())                   //Verifica se recebeu um dado pela UART do AcquaShield_pH
         {
             rx_vetor[contador] = mySerial_OD.read();  //Adiciona o dado ao vetor de RX.
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
         od = atof(rx_vetor);     
         return od;
     }
}

/*######################################################################################################################################################
Função responsável por retornar a leitura do OD sat. do AcquaShield_OD.
Esta função retorna um valor float correspondente à leitura do valor de OD sat.
######################################################################################################################################################*/
float ODShield::ODsat(void)
{
     mySerial_OD.listen();  // Garante que o SoftwareSerial está escutando o OD Shield
     float odsat = 0;
     unsigned int contador = 0;
     unsigned int rx_timeout = 0;
     char rx_vetor[6] = {0,0,0,0,0,0};
     
     mySerial_OD.write(ShieldOD_addr);  //Envia o endereço do AcquaShield_pH pela UART.
     delay(10);
     mySerial_OD.write("odsat"); //Envia o comando para leitura da temperatura pelo AcquaShield.
     delay(10);
     mySerial_OD.write(0xED);   //Envia o caractere de finalização do comando.

     while((contador <= 6)&(rx_timeout < 2000)) //Aguarda receber os 6 bytes da leitura de temperatura e, caso de algum erro, verifica a variável de timeout.
     {
         if(mySerial_OD.available())                   //Verifica se recebeu um dado pela UART do AcquaShield_pH
         {
             rx_vetor[contador] = mySerial_OD.read();  //Adiciona o dado ao vetor de RX.
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
         odsat = atof(rx_vetor);     
         return odsat;
     }
}



/*######################################################################################################################################################
Função responsável por escrever uma String no diplay 128x32 do AcquaShield_pH
######################################################################################################################################################*/
void ODShield::DispWrite(char font, int x, int y, char str[])
{
     unsigned int i = 0;
     
     mySerial_OD.listen();  // Garante que o SoftwareSerial está escutando o OD Shield
     mySerial_OD.write(ShieldOD_addr);  //Envia o endereço do AcquaShield_pH pela UART.
     delay(10);
     mySerial_OD.write("dsp");
     delay(10);
     mySerial_OD.write(font);
     delay(10);
     mySerial_OD.write(x);  
     delay(10);
     mySerial_OD.write(y); 
     delay(10);
     mySerial_OD.write(str); 
     delay(10);
     mySerial_OD.write(0xED);   //Envia o caractere de finalização do comando.   

     mySerial_OD.flush();
}

/*######################################################################################################################################################
Função responsável por apagar o buffer do diplay 128x32 do AcquaShield_pH
######################################################################################################################################################*/
void ODShield::DispBuffClear(void)
{    
     mySerial_OD.listen();  // Garante que o SoftwareSerial está escutando o OD Shield
     mySerial_OD.write(ShieldOD_addr);  //Envia o endereço do AcquaShield_pH pela UART.
     delay(10);
     mySerial_OD.write("bff");
     delay(10);
     mySerial_OD.write(0xED);   //Envia o caractere de finalização do comando.    
     
     //mySerial_OD.flush();
}

/*######################################################################################################################################################
Função responsável por apagar os dados do diplay 128x32 do AcquaShield_pH
######################################################################################################################################################*/
void ODShield::DispClear(void)
{    
     mySerial_OD.listen();  // Garante que o SoftwareSerial está escutando o OD Shield
     mySerial_OD.write(ShieldOD_addr);  //Envia o endereço do AcquaShield_pH pela UART.
     delay(10);
     mySerial_OD.write("clr");
     delay(10);
     mySerial_OD.write(0xED);   //Envia o caractere de finalização do comando.    
     
     //mySerial_OD.flush();
}

/*######################################################################################################################################################
Função responsável por mostras os caracteres no diplay 128x32 do AcquaShield_pH
######################################################################################################################################################*/
void ODShield::DispShow(void)
{    
     mySerial_OD.listen();  // Garante que o SoftwareSerial está escutando o OD Shield
     mySerial_OD.write(ShieldOD_addr);  //Envia o endereço do AcquaShield_pH pela UART.
     delay(10);
     mySerial_OD.write("snd");
     delay(10);
     mySerial_OD.write(0xED);   //Envia o caractere de finalização do comando.    

     //mySerial_OD.flush();
}

/*######################################################################################################################################################
Função responsável por chamar a rotina de calibração no pH_Shield
######################################################################################################################################################*/
void OD_Cal(void)
{
    unsigned char uart_byte = 0;        //Recebe o byte recebido pela UART.
    unsigned char serial_byte = 0;      //Recebe o byte recebido pela UART.
    
    mySerial_OD.listen();  // Garante que o SoftwareSerial está escutando o OD Shield
     
    mySerial_OD.write(ShieldOD_addr);  //Envia o endereço do AcquaShield_pH pela UART.
    delay(10);
    mySerial_OD.write("Cal");
    delay(10);
    mySerial_OD.write(0xED);   //Envia o caractere de finalização do comando.   


    while(true) //Aguarda receber o byte de finalização da rotida de calibração do Shield de pH ('F').
    {                  
        if(mySerial_OD.available())                   //Verifica se recebeu um dado pela UART do AcquaShield_OD
        {
            uart_byte = mySerial_OD.read();
            Serial.write(uart_byte);                            
        }

        if(Serial.available())                   //Verifica se recebeu um dado pela UART do AcquaShield_OD
        {
            serial_byte = Serial.read();   
            mySerial_OD.write(serial_byte);  
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
Função responsável por retornar os valores de calibração do Shield de OD.
######################################################################################################################################################*/
/* 
float OD_param(void)
{
     float ph_offset = 0;
     unsigned int contador = 0;
     unsigned int rx_timeout = 0;
     char rx_vetor[6] = {0,0,0,0,0,0};
    
     mySerial_OD.write(ShieldOD_addr);  //Envia o endereço do AcquaShield_pH pela UART.
     delay(10);
     mySerial_OD.write("Parm"); //Envia o comando para leitura do OffSet pelo AcquaShield.
     delay(10);
     mySerial_OD.write(0xED);   //Envia o caractere de finalização do comando.
     
     while((contador <= 6)&(rx_timeout < 2000)) //Aguarda receber os 6 bytes da leitura de temperatura e, caso de algum erro, verifica a variável de timeout.
     {
         if(mySerial_OD.available())                   //Verifica se recebeu um dado pela UART do AcquaShield_pH
         {
             rx_vetor[contador] = mySerial_OD.read();  //Adiciona o dado ao vetor de RX.
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
 */


/*######################################################################################################################################################
Função responsável por retornar o valor de Offset do eletrodo de OD no AcquaShield_OD
######################################################################################################################################################*/
float OD_offset(void)
{
     mySerial_OD.listen();  // Garante que o SoftwareSerial está escutando o OD Shield
     float od_offset = 0;
     unsigned int contador = 0;
     unsigned int rx_timeout = 0;
     char rx_vetor[6] = {0,0,0,0,0,0};
    
     mySerial_OD.write(ShieldOD_addr);  //Envia o endereço do AcquaShield_OD pela UART.
     delay(10);
     mySerial_OD.write("oft");         //Envia o comando para leitura do OffSet pelo AcquaShield.
     delay(10);
     mySerial_OD.write(0xED);          //Envia o caractere de finalização do comando.
     
     while((contador <= 6)&(rx_timeout < 2000)) //Aguarda receber os 6 bytes da leitura de temperatura e, caso de algum erro, verifica a variável de timeout.
     {
         if(mySerial_OD.available())                   //Verifica se recebeu um dado pela UART do AcquaShield_OD
         {
             rx_vetor[contador] = mySerial_OD.read();  //Adiciona o dado ao vetor de RX.
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
         od_offset = atof(rx_vetor);     
         return od_offset;
     }    
}

/*######################################################################################################################################################
Função responsável por retornar o valor da tensao do eletrodo de OD no AcquaShield_OD na solução de 0mg/L
######################################################################################################################################################*/
float OD_voltage_0(void)
{
     mySerial_OD.listen();  // Garante que o SoftwareSerial está escutando o OD Shield
     float od_v0 = 0;
     unsigned int contador = 0;
     unsigned int rx_timeout = 0;
     char rx_vetor[6] = {0,0,0,0,0,0};
    
     mySerial_OD.write(ShieldOD_addr);  //Envia o endereço do AcquaShield_OD pela UART.
     delay(10);
     mySerial_OD.write("v00");         //Envia o comando para leitura do OffSet pelo AcquaShield.
     delay(10);
     mySerial_OD.write(0xED);          //Envia o caractere de finalização do comando.
     
     while((contador <= 6)&(rx_timeout < 2000)) //Aguarda receber os 6 bytes da leitura de temperatura e, caso de algum erro, verifica a variável de timeout.
     {
         if(mySerial_OD.available())                   //Verifica se recebeu um dado pela UART do AcquaShield_OD
         {
             rx_vetor[contador] = mySerial_OD.read();  //Adiciona o dado ao vetor de RX.
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
         od_v0 = atof(rx_vetor);     
         return od_v0;
     }    
}

/*######################################################################################################################################################
Função responsável por retornar o valor da tensao do eletrodo de OD no AcquaShield_OD na condição de contato com o Ar
######################################################################################################################################################*/
float OD_voltage_Ar(void)
{

     float od_var = 0;
     unsigned int contador = 0;
     unsigned int rx_timeout = 0;
     char rx_vetor[6] = {0,0,0,0,0,0};
    
     mySerial_OD.listen();  // Garante que o SoftwareSerial está escutando o OD Shield

     mySerial_OD.write(ShieldOD_addr);  //Envia o endereço do AcquaShield_OD pela UART.
     delay(10);
     mySerial_OD.write("var");         //Envia o comando para leitura do OffSet pelo AcquaShield.
     delay(10);
     mySerial_OD.write(0xED);          //Envia o caractere de finalização do comando.
     
     while((contador <= 6)&(rx_timeout < 2000)) //Aguarda receber os 6 bytes da leitura de temperatura e, caso de algum erro, verifica a variável de timeout.
     {
         if(mySerial_OD.available())                   //Verifica se recebeu um dado pela UART do AcquaShield_OD
         {
             rx_vetor[contador] = mySerial_OD.read();  //Adiciona o dado ao vetor de RX.
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
         od_var = atof(rx_vetor);     
         return od_var;
     }    
}


/*######################################################################################################################################################
Função responsável por receber os dados da UART, vindos do Arduino UNO,a tratá-los conforme suas respectivas funções.
Esta função é chamada toda vez que um byte é recebudo pela UART e um buffer é preenchido com os dados para posterior tratamento.
######################################################################################################################################################*/
void serialEvent3(void)
{
    unsigned char uart_byte = 0;         //Recebe o byte recebido pela UART.
    unsigned char uart_vetor[16];        //Vetor para armazenar os bytes de comando.
    unsigned int cont_comando = 0;       //Contador para indexar o vetor de comando.
    static boolean verif_addr = false;   //Bit para controle de acesso à função de comandos.
    unsigned int i = 0;
    static char vetor_serial[10];
    
    int cont_teste = 0;

    mySerial_OD.listen();  // Garante que o SoftwareSerial está escutando o OD Shield
    
    while(Serial.available() > 0)  //Aguarda o buffer da UART ser lido totalmente.
    {
        uart_byte = Serial.read();
    
        if((uart_byte == EndCommand)&(verif_addr == true))                               //Verifica se o comando de finalização chegou pela UART.
        {   
            for(i = (cont_comando); i < 16; i++)  
                uart_vetor[i] = '\0';
            
            if((uart_vetor[0] == 'O')&(uart_vetor[1] == 'D')&(uart_vetor[2] == 'C')&(uart_vetor[3] == 'a')&(uart_vetor[4] == 'l')) 
            {
                OD_Cal();
            }       
            else if((uart_vetor[0] == 'O')&(uart_vetor[1] == 'D')&(uart_vetor[2] == 'O')&(uart_vetor[3] == 'f')&(uart_vetor[4] == 't')) 
            {
 
                //Recebe o valor de Offset
                dtostrf((OD_offset()), 2, 0, vetor_serial);
                
                strcpy(vetor_serial, vetor_serial);
                Serial.write("OD Offset: ");
                Serial.write(vetor_serial);
                Serial.write(" mV");
                Serial.println();

                //Serial.flush();
            }
            else if((uart_vetor[0] == 'O')&(uart_vetor[1] == 'D')&(uart_vetor[2] == 'V')&(uart_vetor[3] == '0')&(uart_vetor[4] == '0'))
            {

                //Recebe o valor de tensão na solução 00
                dtostrf(OD_voltage_0(), 2, 0, vetor_serial);
                
                strcpy(vetor_serial, vetor_serial);
                Serial.write("Tensao OD Sol. 0 mg/L: ");
                Serial.write(vetor_serial);
                Serial.write(" mV");
                Serial.println();

                //Serial.flush();

            }
            else if((uart_vetor[0] == 'O')&(uart_vetor[1] == 'D')&(uart_vetor[2] == 'V')&(uart_vetor[3] == 'a')&(uart_vetor[4] == 'r'))
            {
              
                //recebe o valor de tensão no Ar
                dtostrf(OD_voltage_Ar(), 2, 0, vetor_serial);
                
                strcpy(vetor_serial, vetor_serial);
                Serial.write("Tensao OD Ar: ");
                Serial.write(vetor_serial);
                Serial.write(" mV");
                Serial.println();

                //Serial.flush();

            }
            else if((uart_vetor[0] == 'O')&(uart_vetor[1] == 'D')&(uart_vetor[2] == 'M')&(uart_vetor[3] == 'g')&(uart_vetor[4] == 'l'))
            {
                od_type = mgl;
            }
            else if((uart_vetor[0] == 'O')&(uart_vetor[1] == 'D')&(uart_vetor[2] == 'S')&(uart_vetor[3] == 'a')&(uart_vetor[4] == 't'))
            {
                od_type = saturacao;
            }
            else if((uart_vetor[0] == 'O')&(uart_vetor[1] == 'D')&(uart_vetor[2] == 'C')&(uart_vetor[3] == 'e')&(uart_vetor[4] == 'r')&(uart_vetor[5] == 'a')&(uart_vetor[6] == 's')) 
            {
                mySerial_OD.write(ShieldOD_addr); //Envia o endereço do AcquaShield_OD pela UART.
                delay(10);
                mySerial_OD.write("Cers"); //Envia o comando para leitura do sloope pH04 pelo AcquaShield.
                delay(10);
                mySerial_OD.write(0xED);   //Envia o caractere de finalização do comando.   

                Serial.write("Calibração OD apagada !");
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
