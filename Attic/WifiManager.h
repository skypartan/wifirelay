#ifndef _H_WIFIMANAGER
#define _H_WIFIMANAGER


#define TRUE 1
#define FALSE 0

#define AP_NAME "WifiRelay"


int wifi_connect(); // Função para realizar conexão com o WiFI utilizando credenciais salvas
int wifi_connect(const char* ssid, const char* password);
int wifi_keep();    // Função para verificar se a conexão com o WiFi ainda existe

void wifi_setup_ap(); // Função para iniciar o servidor de configuração de WiFI

#endif