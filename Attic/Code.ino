#include "WifiManager.h"
#include "Controller.h"

#include <ESP8266WiFi.h>


#define MODE_MANAGER 0    // Módulo está conectadoa o WiFi e pode operar normalmente
#define MODE_CONTROLLER 1 // Módulo não está conectado ao WiFi e está com o painel de configuração aberto

int current_mode = MODE_CONTROLLER;


void setup()
{
    Serial.begin(115200); // Inicializando canal de comunicação para depuração
    Serial.println("[SISTEMA]: Inicializando");

    pinMode(0, OUTPUT);
    
    server_setup_controller();
}

void loop()
{
    if (current_mode == MODE_CONTROLLER && wifi_keep() == FALSE)
    {
        current_mode = MODE_MANAGER;
        server_setup_manager();
    }

    // Decidindo qual loop executar
    if (current_mode == MODE_MANAGER)
        server_loop_manager();
    else
        server_loop_controller();
}
