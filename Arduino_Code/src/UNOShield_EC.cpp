#include "../include/AcquaShield.h"
#include <SoftwareSerial.h>


SoftwareSerial mySerial_EC(3, 2); //RX,TX


//variável global para definir a variável a ser impressa no display
var_disp var_type = temperatura;


ECShield::init()
{
    mySerial_EC.begin(9600);
}

/*######################################################################################################################################################
Função responsável por retornar a leitura do sensor de temperatura do AcquaShied_EC.
Esta função retorna um valor float correspondente à leitura em graus celsius do DS18B20.
######################################################################################################################################################*/
float ECShield::temp(void)
{
     mySerial_EC.listen();  // Garante que o SoftwareSerial está escutando o EC Shield

     float temp = 0;
     unsigned int contador = 0;
     unsigned int rx_timeout = 0;
     char rx_vetor[6] = {0,0,0,0,0,0};
     
     mySerial_EC.write(ShieldEC_addr);  //Envia o endereço do AcquaShield_EC pela UART.
     delay(10);
     mySerial_EC.write("tmp"); //Envia o comando para leitura da temperatura pelo AcquaShield.
     delay(10);
     mySerial_EC.write(0xED);   //Envia o caractere de finalização do comando.

     while((contador <= 6)&(rx_timeout < 2000)) //Aguarda receber os 6 bytes da leitura de temperatura e, caso de algum erro, verifica a variável de timeout.
     {
         if(mySerial_EC.available())                   //Verifica se recebeu um dado pela UART do AcquaShield_EC
         {
             rx_vetor[contador] = mySerial_EC.read();  //Adiciona o dado ao vetor de RX.
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
Função responsável por retornar a leitura de EC do AcquaShield_EC.
Esta função retorna um valor float correspondente à leitura do valor de EC.
######################################################################################################################################################*/
float ECShield::EC(void)
{ 
     mySerial_EC.listen();  // Garante que o SoftwareSerial está escutando o EC Shield
     float ec = 0;
     unsigned int contador = 0;
     unsigned int rx_timeout = 0;
     char rx_vetor[6] = {0,0,0,0,0,0};
     
     mySerial_EC.write(ShieldEC_addr); //Envia o endereço do AcquaShield_EC pela UART.
     delay(10);
     mySerial_EC.write("ec");          //Envia o comando para leitura da condutividade pelo AcquaShield.
     delay(10);
     mySerial_EC.write(0xED);          //Envia o caractere de finalização do comando.

     while((contador <= 6)&(rx_timeout < 2000)) //Aguarda receber os 6 bytes da leitura de temperatura e, caso de algum erro, verifica a variável de timeout.
     {
         if(mySerial_EC.available())                   //Verifica se recebeu um dado pela UART do AcquaShield_EC
         {
             rx_vetor[contador] = mySerial_EC.read();  //Adiciona o dado ao vetor de RX.
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
         ec = atof(rx_vetor);     
         return ec;
     }
}

/*######################################################################################################################################################
Função responsável por retornar o valor calculado de Salinidade do AcquaShield_EC.
Esta função retorna um valor float correspondente ao valor de Salinidade.
######################################################################################################################################################*/
float ECShield::Salin(void)
{
     mySerial_EC.listen();  // Garante que o SoftwareSerial está escutando o EC Shield
     float salin = 0;
     unsigned int contador = 0;
     unsigned int rx_timeout = 0;
     char rx_vetor[6] = {0,0,0,0,0,0};
     
     mySerial_EC.write(ShieldEC_addr); //Envia o endereço do AcquaShield_EC pela UART.
     delay(10);
     mySerial_EC.write("sal");          //Envia o comando para leitura de salinidade pelo AcquaShield.
     delay(10);
     mySerial_EC.write(0xED);          //Envia o caractere de finalização do comando.

     while((contador <= 6)&(rx_timeout < 2000)) //Aguarda receber os 6 bytes da leitura de temperatura e, caso de algum erro, verifica a variável de timeout.
     {
         if(mySerial_EC.available())                   //Verifica se recebeu um dado pela UART do AcquaShield_EC
         {
             rx_vetor[contador] = mySerial_EC.read();  //Adiciona o dado ao vetor de RX.
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
         salin = atof(rx_vetor);     
         return salin;
     }
}

/*######################################################################################################################################################
Função responsável por retornar o valor calculado de TDS do AcquaShield_EC.
Esta função retorna um valor float correspondente ao valor de TDS.
######################################################################################################################################################*/
float ECShield::Tds(void)
{
     mySerial_EC.listen();  // Garante que o SoftwareSerial está escutando o EC Shield
     float _tds = 0;
     unsigned int contador = 0;
     unsigned int rx_timeout = 0;
     char rx_vetor[6] = {0,0,0,0,0,0};
     
     mySerial_EC.write(ShieldEC_addr); //Envia o endereço do AcquaShield_EC pela UART.
     delay(10);
     mySerial_EC.write("tds");          //Envia o comando para leitura de salinidade pelo AcquaShield.
     delay(10);
     mySerial_EC.write(0xED);          //Envia o caractere de finalização do comando.

     while((contador <= 6)&(rx_timeout < 2000)) //Aguarda receber os 6 bytes da leitura de temperatura e, caso de algum erro, verifica a variável de timeout.
     {
         if(mySerial_EC.available())                   //Verifica se recebeu um dado pela UART do AcquaShield_EC
         {
             rx_vetor[contador] = mySerial_EC.read();  //Adiciona o dado ao vetor de RX.
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
         _tds = atof(rx_vetor);     
         return _tds;
     }
}

/*######################################################################################################################################################
Função responsável por escrever uma String no diplay 128x32 do AcquaShield_EC
######################################################################################################################################################*/
void ECShield::DispWrite(char font, int x, int y, char str[])
{
     mySerial_EC.listen();  // Garante que o SoftwareSerial está escutando o EC Shield
     unsigned int i = 0;
     
     mySerial_EC.write(ShieldEC_addr);  //Envia o endereço do AcquaShield_EC pela UART.
     delay(10);
     mySerial_EC.write("dsp");
     delay(10);
     mySerial_EC.write(font);
     delay(10);
     mySerial_EC.write(x);  
     delay(10);
     mySerial_EC.write(y); 
     delay(10);
     mySerial_EC.write(str); 
     delay(10);
     mySerial_EC.write(0xED);   //Envia o caractere de finalização do comando.   

     mySerial_EC.flush();
}

/*######################################################################################################################################################
Função responsável por apagar o buffer do diplay 128x32 do AcquaShield_EC
######################################################################################################################################################*/
void ECShield::DispBuffClear(void)
{    
     mySerial_EC.listen();  // Garante que o SoftwareSerial está escutando o EC Shield
     mySerial_EC.write(ShieldEC_addr);  //Envia o endereço do AcquaShield_EC pela UART.
     delay(10);
     mySerial_EC.write("bff");
     delay(10);
     mySerial_EC.write(0xED);          //Envia o caractere de finalização do comando.    
}

/*######################################################################################################################################################
Função responsável por apagar os dados do diplay 128x32 do AcquaShield_EC
######################################################################################################################################################*/
void ECShield::DispClear(void)
{    
     mySerial_EC.listen();  // Garante que o SoftwareSerial está escutando o EC Shield
     mySerial_EC.write(ShieldEC_addr);  //Envia o endereço do AcquaShield_EC pela UART.
     delay(10);
     mySerial_EC.write("clr");
     delay(10);
     mySerial_EC.write(0xED);          //Envia o caractere de finalização do comando.    
}

/*######################################################################################################################################################
Função responsável por mostras os caracteres no diplay 128x32 do AcquaShield_EC
######################################################################################################################################################*/
void ECShield::DispShow(void)
{    
     mySerial_EC.listen();  // Garante que o SoftwareSerial está escutando o EC Shield
     mySerial_EC.write(ShieldEC_addr);  //Envia o endereço do AcquaShield_EC pela UART.
     delay(10);
     mySerial_EC.write("snd");
     delay(10);
     mySerial_EC.write(0xED);          //Envia o caractere de finalização do comando.    
}

/*######################################################################################################################################################
Função responsável por chamar a rotina de calibração no AcquaShield_EC
######################################################################################################################################################*/
void EC_Cal(void)
{
    unsigned char uart_byte = 0;        //Recebe o byte recebido pela UART.
    unsigned char serial_byte = 0;      //Recebe o byte recebido pela UART.
    mySerial_EC.listen();  // Garante que o SoftwareSerial está escutando o EC Shield
     
    mySerial_EC.write(ShieldEC_addr);  //Envia o endereço do AcquaShield_pH pela UART.
    delay(10);
    mySerial_EC.write("Cal");
    delay(10);
    mySerial_EC.write(0xED);           //Envia o caractere de finalização do comando.   


    while(true) //Aguarda receber o byte de finalização da rotida de calibração do Shield de EC ('F').
    {                  
        if(mySerial_EC.available())                 //Verifica se recebeu um dado pela UART do AcquaShield_EC
        {
            uart_byte = mySerial_EC.read();
            Serial.write(uart_byte);                            
        }

        if(Serial.available())                   //Verifica se recebeu um dado pela UART do AcquaShield_pH
        {
            serial_byte = Serial.read();   
            mySerial_EC.write(serial_byte);  
        }
        if(serial_byte == 'F')
        {
            Serial.write("Rotina de calibração finalizada.");
            Serial.println(); 
            while(true)
            { 
                delay(100);
            }
        }
    } 
}

/*######################################################################################################################################################
Função responsável por retornar o valor de Offset do eletrodo de EC no AcquaShield_EC na condição de contato com o Ar
######################################################################################################################################################*/
float EC_offset(void)
{
     mySerial_EC.listen();  // Garante que o SoftwareSerial está escutando o EC Shield
     float ec_offset = 0;
     unsigned int contador = 0;
     unsigned int rx_timeout = 0;
     char rx_vetor[6] = {0,0,0,0,0,0};
    
     mySerial_EC.write(ShieldEC_addr);  //Envia o endereço do AcquaShield_EC pela UART.
     delay(10);
     mySerial_EC.write("Oft");         //Envia o comando para leitura do OffSet pelo AcquaShield.
     delay(10);
     mySerial_EC.write(0xED);          //Envia o caractere de finalização do comando.
     
     while((contador <= 6)&(rx_timeout < 2000)) //Aguarda receber os 6 bytes da leitura de temperatura e, caso de algum erro, verifica a variável de timeout.
     {
         if(mySerial_EC.available())                   //Verifica se recebeu um dado pela UART do AcquaShield_EC
         {
             rx_vetor[contador] = mySerial_EC.read();  //Adiciona o dado ao vetor de RX.
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
         ec_offset = atof(rx_vetor);     
         return ec_offset;
     }    
}

/*######################################################################################################################################################
Função responsável por retornar o valor do Slope (Lo ou Hi) do eletrodo de EC no AcquaShield_EC
######################################################################################################################################################*/
float EC_slope(char slope[]) 
{
     mySerial_EC.listen();  // Garante que o SoftwareSerial está escutando o EC Shield
     float ec_sl = 0;
     unsigned int contador = 0;
     unsigned int rx_timeout = 0;
     char rx_vetor[6] = {0,0,0,0,0,0};

     mySerial_EC.write(ShieldEC_addr);  //Envia o endereço do AcquaShield_EC pela UART.
     delay(10);
     mySerial_EC.write(slope);        //Envia o comando para leitura do slope pelo AcquaShield.
     delay(10);
     mySerial_EC.write(0xED);          //Envia o caractere de finalização do comando.

     contador = 0;
     rx_timeout = 0;     
     while((contador <= 6)&(rx_timeout < 2000)) //Aguarda receber os 6 bytes da leitura de temperatura e, caso de algum erro, verifica a variável de timeout.
     {
         if(mySerial_EC.available())                   //Verifica se recebeu um dado pela UART do AcquaShield
         {
             rx_vetor[contador] = mySerial_EC.read();  //Adiciona o dado ao vetor de RX.
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
         ec_sl = atof(rx_vetor);     
         return ec_sl;
     }    
}

/*######################################################################################################################################################
Função responsável por retornar o valor do Coeficiente angular (Lo ou Hi) do eletrodo de EC no AcquaShield_EC
######################################################################################################################################################*/
float EC_linear_coef(char linc[]) 
{
     mySerial_EC.listen();  // Garante que o SoftwareSerial está escutando o EC Shield
     float ec_lc = 0;
     unsigned int contador = 0;
     unsigned int rx_timeout = 0;
     char rx_vetor[6] = {0,0,0,0,0,0};
    
     mySerial_EC.write(ShieldEC_addr);  //Envia o endereço do AcquaShield_EC pela UART.
     delay(10);
     mySerial_EC.write(linc);        //Envia o comando para leitura do coeficiente linear pelo AcquaShield.
     delay(10);
     mySerial_EC.write(0xED);          //Envia o caractere de finalização do comando.
     

     contador = 0;
     rx_timeout = 0;
     while((contador <= 6)&(rx_timeout < 2000)) //Aguarda receber os 6 bytes da leitura de temperatura e, caso de algum erro, verifica a variável de timeout.
     {
         if(mySerial_EC.available())                   //Verifica se recebeu um dado pela UART do AcquaShield
         {
             rx_vetor[contador] = mySerial_EC.read();  //Adiciona o dado ao vetor de RX.
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
         ec_lc = atof(rx_vetor);     
         return ec_lc;
     }    
}

/*######################################################################################################################################################
Função responsável por retornar o valor da temperatura de calibração do AcquaShield_EC.
######################################################################################################################################################*/
float EC_CalTemp(void) 
{
     mySerial_EC.listen();  // Garante que o SoftwareSerial está escutando o EC Shield
     float CalTemp = 0;
     unsigned int contador = 0;
     unsigned int rx_timeout = 0;
     char rx_vetor[6] = {0,0,0,0,0,0};
    
     mySerial_EC.write(ShieldEC_addr);  //Envia o endereço do AcquaShield_EC pela UART.
     delay(10);
     mySerial_EC.write("Ctmp"); //Envia o comando para leitura da temperatura de calibracao pelo AcquaShield.
     delay(10);
     mySerial_EC.write(0xED);   //Envia o caractere de finalização do comando.
     
     while((contador <= 6)&(rx_timeout < 2000)) //Aguarda receber os 6 bytes da leitura de temperatura e, caso de algum erro, verifica a variável de timeout.
     {
         if(mySerial_EC.available())                   //Verifica se recebeu um dado pela UART do AcquaShield_EC
         {
             rx_vetor[contador] = mySerial_EC.read();  //Adiciona o dado ao vetor de RX.
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
Função responsável por retornar o valor do do Fator de Conversão de TDS do AcquaShield_EC.
######################################################################################################################################################*/
float EC_FatorTds(void) 
{
     mySerial_EC.listen();  // Garante que o SoftwareSerial está escutando o EC Shield
     float _FatorTds = 0;
     unsigned int contador = 0;
     unsigned int rx_timeout = 0;
     char rx_vetor[6] = {0,0,0,0,0,0};
    
     mySerial_EC.write(ShieldEC_addr);  //Envia o endereço do AcquaShield_EC pela UART.
     delay(10);
     mySerial_EC.write("Ftds"); //Envia o comando para leitura do Fator TDS pelo AcquaShield.
     delay(10);
     mySerial_EC.write(0xED);   //Envia o caractere de finalização do comando.
     
     while((contador <= 6)&(rx_timeout < 2000)) //Aguarda receber os 6 bytes da leitura de temperatura e, caso de algum erro, verifica a variável de timeout.
     {
         if(mySerial_EC.available())                   //Verifica se recebeu um dado pela UART do AcquaShield_EC
         {
             rx_vetor[contador] = mySerial_EC.read();  //Adiciona o dado ao vetor de RX.
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
         _FatorTds = atof(rx_vetor);     
         return _FatorTds;
     }    
}

/*######################################################################################################################################################
Função responsável por setar e retornar um novo valor para Fator de Conversão de TDS do AcquaShield_EC.
######################################################################################################################################################*/
float EC_SetFatorTds(char comando[]) 
{
    mySerial_EC.listen();  // Garante que o SoftwareSerial está escutando o EC Shield
    mySerial_EC.write(ShieldEC_addr);  //Envia o endereço do AcquaShield_EC pela UART.
    delay(10);
    mySerial_EC.write(comando);        //Envia o comando para definir Fator TDS pelo AcquaShield.
    delay(10);
    mySerial_EC.write(0xED);           //Envia o caractere de finalização do comando.
}

/*######################################################################################################################################################
Função responsável por resetar para o valor default o Fator de Conversão de TDS do AcquaShield_EC.
######################################################################################################################################################*/
float EC_ResetFatorTds(void) 
{
    mySerial_EC.listen();  // Garante que o SoftwareSerial está escutando o EC Shield   
    mySerial_EC.write(ShieldEC_addr);  //Envia o endereço do AcquaShield_EC pela UART.
    delay(10);
    mySerial_EC.write("RFtds");        //Envia comando para resetar o Fator de Coversao TDS
    delay(10);
    mySerial_EC.write(0xED);           //Envia o caractere de finalização do comando.
}

/*######################################################################################################################################################
Função responsável por habilitar a compensação de temperatura de EC do AcquaShield_EC.
######################################################################################################################################################*/
void EC_EnableCompTemp(void) 
{
    mySerial_EC.listen();  // Garante que o SoftwareSerial está escutando o EC Shield   
    mySerial_EC.write(ShieldEC_addr);  //Envia o endereço do AcquaShield_EC pela UART.
    delay(10);
    mySerial_EC.write("EComp");        //Envia comando para habilitar compensação de temperatura
    delay(10);
    mySerial_EC.write(0xED);           //Envia o caractere de finalização do comando.
}

/*######################################################################################################################################################
Função responsável por desabilitar a compensação de temperatura de EC do AcquaShield_EC.
######################################################################################################################################################*/
void EC_DisableCompTemp(void) 
{
    mySerial_EC.listen();  // Garante que o SoftwareSerial está escutando o EC Shield   
    mySerial_EC.write(ShieldEC_addr);  //Envia o endereço do AcquaShield_EC pela UART.
    delay(10);
    mySerial_EC.write("DComp");        //Envia comando para desabilitar compensação de temperatura
    delay(10);
    mySerial_EC.write(0xED);           //Envia o caractere de finalização do comando.
}


/*######################################################################################################################################################
Função responsável por retornar o valor da temperatura de calibração do AcquaShield_EC.
######################################################################################################################################################*/
uint8_t EC_StatusCompTemp(void) 
{
     uint8_t StatusCompTemp = 0;
     unsigned int contador = 0;
     unsigned int rx_timeout = 0;
     char rx_vetor[6] = {0,0,0,0,0,0};

     mySerial_EC.listen();  // Garante que o SoftwareSerial está escutando o EC Shield

     mySerial_EC.write(ShieldEC_addr);  //Envia o endereço do AcquaShield_EC pela UART.
     delay(10);
     mySerial_EC.write("STComp"); //Envia o comando para leitura da temperatura de calibracao pelo AcquaShield.
     delay(10);
     mySerial_EC.write(0xED);   //Envia o caractere de finalização do comando.
     
     contador = 0;
     while((contador < 1)&(rx_timeout < 2000)) //Aguarda receber os 6 bytes da leitura de temperatura e, caso de algum erro, verifica a variável de timeout.
     {
        if(mySerial_EC.available())                   //Verifica se recebeu um dado pela UART do AcquaShield_EC
        {
            
            StatusCompTemp = mySerial_EC.read();      //Le o dado
            contador++;                            
        }
        rx_timeout++;                       
        delay(1);
     }
     if(rx_timeout >= 2000)
     {
        return 255;
     }   
     else
     {
        //StatusCompTemp = atoi(rx_vetor);     
        return StatusCompTemp;
     }    
}

/*######################################################################################################################################################
Função responsável por retornar o valor do Coeficiente de tmeperatura de EC do AcquaShield_EC.
######################################################################################################################################################*/
float EC_FatorComp(void) 
{
     float _FatorTemp = 0;
     unsigned int contador = 0;
     unsigned int rx_timeout = 0;
     char rx_vetor[6] = {0,0,0,0,0,0};
    
     mySerial_EC.listen();  // Garante que o SoftwareSerial está escutando o EC Shield

     mySerial_EC.write(ShieldEC_addr);  //Envia o endereço do AcquaShield_EC pela UART.
     delay(10);
     mySerial_EC.write("FComp"); //Envia o comando para leitura do Coeficiente de temperatura pelo AcquaShield.
     delay(10);
     mySerial_EC.write(0xED);   //Envia o caractere de finalização do comando.
     
     while((contador <= 6)&(rx_timeout < 2000)) //Aguarda receber os 6 bytes da leitura de temperatura e, caso de algum erro, verifica a variável de timeout.
     {
         if(mySerial_EC.available())                   //Verifica se recebeu um dado pela UART do AcquaShield_EC
         {
             rx_vetor[contador] = mySerial_EC.read();  //Adiciona o dado ao vetor de RX.
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
         _FatorTemp = atof(rx_vetor);     
         return _FatorTemp;
     }    
}

/*######################################################################################################################################################
Função responsável por setar valor para o Coeficiente de Temperatura do AcquaShield_EC.
######################################################################################################################################################*/
float EC_SetFatorComp(char comando[]) 
{

    mySerial_EC.listen();  // Garante que o SoftwareSerial está escutando o EC Shield
    mySerial_EC.write(ShieldEC_addr);  //Envia o endereço do AcquaShield_EC pela UART.
    delay(10);
    mySerial_EC.write(comando);        //Envia o comando para definir Coeficiente de Temperatura  pelo AcquaShield.
    delay(10);
    mySerial_EC.write(0xED);           //Envia o caractere de finalização do comando.
}

/*######################################################################################################################################################
Função responsável por resetar para o valor default o Coeficiente de Temperatura do AcquaShield_EC.
######################################################################################################################################################*/
float EC_ResetFatorComp(void) 
{
    mySerial_EC.listen();  // Garante que o SoftwareSerial está escutando o EC Shield   
    mySerial_EC.write(ShieldEC_addr);  //Envia o endereço do AcquaShield_EC pela UART.
    delay(10);
    mySerial_EC.write("RFComp");        //Envia comando para resetar o Coeficiente de Temperatura de EC
    delay(10);
    mySerial_EC.write(0xED);           //Envia o caractere de finalização do comando.
}





/*######################################################################################################################################################
Função responsável por receber os dados da UART, vindos do Arduino UNO,a tratá-los conforme suas respectivas funções.
Esta função é chamada toda vez que um byte é recebudo pela UART e um buffer é preenchido com os dados para posterior tratamento.
######################################################################################################################################################*/
void serialEvent2(void)
{
    unsigned char uart_byte = 0;         //Recebe o byte recebido pela UART.
    unsigned char uart_vetor[16];        //Vetor para armazenar os bytes de comando.
    char comando_vetor[11];               //vetor para armazenar o comando recebido pela UART 
    unsigned int cont_comando = 0;       //Contador para indexar o vetor de comando.
    static boolean verif_addr = false;   //Bit para controle de acesso à função de comandos.
    unsigned int i = 0;
    static char vetor_serial[5];

    mySerial_EC.listen();  // Garante que o SoftwareSerial está escutando o EC Shield

    while(Serial.available() > 0)  //Aguarda o buffer da UART ser lido totalmente.
    {
        uart_byte = Serial.read();
    
        if((uart_byte == EndCommand)&(verif_addr == true))                               //Verifica se o comando de finalização chegou pela UART.
        {   
            for(i = (cont_comando); i < 16; i++)  
                uart_vetor[i] = '\0';
            
            if((uart_vetor[0] == 'E')&(uart_vetor[1] == 'C')&(uart_vetor[2] == 'C')&(uart_vetor[3] == 'a')&(uart_vetor[4] == 'l')) 
            {
                EC_Cal();
            }       
            else if((uart_vetor[0] == 'E')&(uart_vetor[1] == 'C')&(uart_vetor[2] == 'D')&(uart_vetor[3] == 'r')&(uart_vetor[4] == 'y')) 
            {
                dtostrf((EC_offset()), 2, 1, vetor_serial);
                strcpy(vetor_serial, vetor_serial);
                Serial.write("EC Dry: ");
                Serial.write(vetor_serial);
                Serial.write("mV");
                Serial.println();
            }
            else if((uart_vetor[0] == 'E')&(uart_vetor[1] == 'C')&(uart_vetor[2] == 'P')&(uart_vetor[3] == 'a')&(uart_vetor[4] == 'r')&(uart_vetor[5] == 'a')&(uart_vetor[6] == 'm')) 
            {
                dtostrf(EC_slope("SlopL"), 2, 4, vetor_serial);
                strcpy(vetor_serial, vetor_serial);
                Serial.write("Slope Low EC: ");
                Serial.write(vetor_serial);
                Serial.write(" mV/(uS/cm)");
                Serial.println();

                Serial.flush();

                dtostrf(EC_linear_coef("LincL"), 3, 1, vetor_serial);
                strcpy(vetor_serial, vetor_serial);
                Serial.write("Coef. Lin. Low EC: ");
                Serial.write(vetor_serial);
                Serial.write(" mV");
                Serial.println();

                Serial.flush();

                dtostrf(EC_slope("SlopH"), 2, 4, vetor_serial);
                strcpy(vetor_serial, vetor_serial);
                Serial.write("Slope High EC: ");
                Serial.write(vetor_serial);
                Serial.write(" mV/(uS/cm)");
                Serial.println();

                Serial.flush();

                dtostrf(EC_linear_coef("LincH"), 3, 1, vetor_serial);
                strcpy(vetor_serial, vetor_serial);
                Serial.write("Coef. Lin. High EC: ");
                Serial.write(vetor_serial);
                Serial.write(" mV");
                Serial.println();

                Serial.flush();
            }
            else if((uart_vetor[0] == 'E')&(uart_vetor[1] == 'C')&(uart_vetor[2] == 'C')&(uart_vetor[3] == 'a')&(uart_vetor[4] == 't')&(uart_vetor[5] == 'm')&(uart_vetor[6] == 'p')) 
            {
                dtostrf(EC_CalTemp(), 2, 1, vetor_serial);
                strcpy(vetor_serial, vetor_serial);
                Serial.write("Temperatura(Cal): ");
                Serial.write(vetor_serial);
                Serial.write("ºC");
                Serial.println();
            }
            else if((uart_vetor[0] == 'E')&(uart_vetor[1] == 'C')&(uart_vetor[2] == 'F')&(uart_vetor[3] == 't')&(uart_vetor[4] == 'd')&(uart_vetor[5] == 's'))
            {
                dtostrf(EC_FatorTds(), 3, 2, vetor_serial);
                strcpy(vetor_serial, vetor_serial);
                Serial.write("Fator TDS: ");
                Serial.write(vetor_serial);              
                Serial.println();

            }
            else if((uart_vetor[0] == 'E')&(uart_vetor[1] == 'C')&(uart_vetor[2] == 'S')&(uart_vetor[3] == 'F')&(uart_vetor[4] == 't')&(uart_vetor[5] == 'd')&(uart_vetor[6] == 's'))
            {
                for(i=0; i<=9; i++)
                {
                    comando_vetor[i] = uart_vetor[i+2];     //filtra somente caracters do comando
                }
                strcpy(comando_vetor, comando_vetor);       //atribui o vetor char
                EC_SetFatorTds(comando_vetor);              //chama função para enviar comando para o Shield
              
                Serial.write("Novo Fator TDS setado! ");               
                Serial.println();  
                
            }
            else if((uart_vetor[0] == 'E')&(uart_vetor[1] == 'C')&(uart_vetor[2] == 'R')&(uart_vetor[3] == 'F')&(uart_vetor[4] == 't')&(uart_vetor[5] == 'd')&(uart_vetor[6] == 's'))
            {
                EC_ResetFatorTds();                        //chama função para enviar comando para o Shield

                Serial.write("Fator TDS resetado! ");
                Serial.println();
                
            }
            else if((uart_vetor[0] == 'E')&(uart_vetor[1] == 'C')&(uart_vetor[2] == 'C')&(uart_vetor[3] == 'e')&(uart_vetor[4] == 'r')&(uart_vetor[5] == 'a')&(uart_vetor[6] == 's')) 
            {
                mySerial_EC.write(ShieldEC_addr);  //Envia o endereço do AcquaShield_EC pela UART.
                delay(10);
                mySerial_EC.write("Cers");         //Envia o comando para resetar os parâmetros de calibração no AcquaShield.
                delay(10);
                mySerial_EC.write(0xED);           //Envia o caractere de finalização do comando.   
                
                Serial.write("Calibração EC apagada !");
                Serial.println();

            }
            else if((uart_vetor[0] == 'E')&(uart_vetor[1] == 'C')&(uart_vetor[2] == 'S')&(uart_vetor[3] == 'a')&(uart_vetor[4] == 'l')&(uart_vetor[5] == 'i')&(uart_vetor[6] == 'n'))
            {
                var_type = salinidade;
            }
            else if((uart_vetor[0] == 'E')&(uart_vetor[1] == 'C')&(uart_vetor[2] == 'T')&(uart_vetor[3] == 'e')&(uart_vetor[4] == 'm')&(uart_vetor[5] == 'p'))
            {
                var_type = temperatura;
            }
            else if((uart_vetor[0] == 'E')&(uart_vetor[1] == 'C')&(uart_vetor[2] == 'T')&(uart_vetor[3] == 'd')&(uart_vetor[4] == 's'))
            {
                var_type = tds;
            }
            //Comando para habilitar a compensacao de temperatura
            else if ((uart_vetor[0] == 'E')&(uart_vetor[1] == 'C')&(uart_vetor[2] == 'E')&(uart_vetor[3] == 'C')&(uart_vetor[4] == 'o')&(uart_vetor[5] == 'm')&(uart_vetor[6] == 'p'))
            {
                EC_EnableCompTemp();

                Serial.write("Compensação de Temperatura habilitada! ");
                Serial.println();

            }
            //Comando para desabilitar a compensacao de temperatura
            else if ((uart_vetor[0] == 'E')&(uart_vetor[1] == 'C')&(uart_vetor[2] == 'D')&(uart_vetor[3] == 'C')&(uart_vetor[4] == 'o')&(uart_vetor[5] == 'm')&(uart_vetor[6] == 'p'))
            {
                EC_DisableCompTemp();

                Serial.write("Compensação de Temperatura desabilitada! ");
                Serial.println();
            }
            //Comando para retornar o estado da compensação de temperatura
            else if((uart_vetor[0] == 'E')&(uart_vetor[1] == 'C')&(uart_vetor[2] == 'S')&(uart_vetor[3] == 'T')&(uart_vetor[4] == 'C')&(uart_vetor[5] == 'o')&(uart_vetor[6] == 'm')&(uart_vetor[7] == 'p'))
            {
                switch (EC_StatusCompTemp())
                {
                    case 0:
                        Serial.write("Estado da Compensação de Temperatura: DESABILITADA ");
                        break;
                    case 1:
                        Serial.write("Estado da Compensação de Temperatura: HABILITADA ");
                        break;

                    case 255:
                        Serial.write("Estado da Compensação de Temperatura: TIMEOUT ");
                        break;
                    
                    default:
                        Serial.write("Estado da Compensação de Temperatura: INDEFNIDO ");
                        break;
                }

                Serial.println();

            }
            //Comando para retornar o Coeficiente de temperatura de EC
            else if((uart_vetor[0] == 'E')&(uart_vetor[1] == 'C')&(uart_vetor[2] == 'F')&(uart_vetor[3] == 'C')&(uart_vetor[4] == 'o')&(uart_vetor[5] == 'm')&(uart_vetor[6] == 'p'))
            {
                dtostrf(EC_FatorComp(), 3, 2, vetor_serial);
                strcpy(vetor_serial, vetor_serial);
                Serial.write("Coeficiente de Temperatura: ");
                Serial.write(vetor_serial);   
                Serial.write("  [% uS/cm/C]");
                Serial.println();

            }
            //Comando para setar um novo valor para o coeficiente de temperatura
            else if ((uart_vetor[0] == 'E')&(uart_vetor[1] == 'C')&(uart_vetor[2] == 'S')&(uart_vetor[3] == 'F')&(uart_vetor[4] == 'C')&(uart_vetor[5] == 'o')&(uart_vetor[6] == 'm')&(uart_vetor[7] == 'p'))
            {
                for(i=0; i<=10; i++)
                {
                    comando_vetor[i] = uart_vetor[i+2];     //filtra somente caracters do comando
                }
                strcpy(comando_vetor, comando_vetor);       //atribui o vetor char
                EC_SetFatorComp(comando_vetor);              //chama função para enviar comando para o Shield
              
                Serial.write("Novo Coeficiente de Temperatura setado! ");               
                Serial.println(); 
            }
            //Comando para resetar coeficiente de temperatura para o valor default
            else if ((uart_vetor[0] == 'E')&(uart_vetor[1] == 'C')&(uart_vetor[2] == 'R')&(uart_vetor[3] == 'F')&(uart_vetor[4] == 'C')&(uart_vetor[5] == 'o')&(uart_vetor[6] == 'm')&(uart_vetor[7] == 'p'))
            {
                EC_ResetFatorComp();                        //chama função para enviar comando para o Shield

                Serial.write("Coeficiente de Temperatura resetado! ");
                Serial.println();
            }
            else         
            {
                Serial.write("Comando não válido !");
                Serial.println();
            }
        
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
