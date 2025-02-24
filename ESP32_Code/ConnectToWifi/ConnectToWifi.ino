#include <WiFi.h>

const char *ssid = "denso_broker";
const char *password = "denso_broker";

void setup() {
  Serial.begin(9600);

  // Limpa configurações Wi-Fi salvas
  WiFi.disconnect(true);
  delay(1000);

  // Define a potência máxima de transmissão
  WiFi.setTxPower(WIFI_POWER_19_5dBm);

  // Conecta ao Wi-Fi
  Serial.println("\nConectando ao Wi-Fi...");
  WiFi.begin(ssid, password, 6); // Força o canal 6

  int tentativas = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(5000);
    tentativas++;
    Serial.print("Tentativa ");
    Serial.print(tentativas);
    Serial.println(": Tentando conectar ao Wi-Fi...");

    // Exibe o status atual da conexão Wi-Fi
    switch (WiFi.status()) {
      case WL_NO_SSID_AVAIL:
        Serial.println("Erro: Rede não encontrada. Verifique o SSID.");
        break;
      case WL_CONNECT_FAILED:
        Serial.println("Erro: Falha na conexão. Verifique a senha.");
        break;
      case WL_CONNECTION_LOST:
        Serial.println("Erro: Conexão perdida.");
        break;
      case WL_DISCONNECTED:
        Serial.println("Erro: Desconectado.");
        break;
      default:
        Serial.println("Erro desconhecido.");
        break;
    }

    // Se exceder um número máximo de tentativas, reinicia o ESP32
    if (tentativas > 10) {
      Serial.println("Falha ao conectar ao Wi-Fi. Reiniciando o ESP32...");
      ESP.restart();
    }
  }

  Serial.println("Conectado ao Wi-Fi!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // Nada a fazer aqui
}
