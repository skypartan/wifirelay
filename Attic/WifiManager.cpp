#include "WifiManager.h"
#include "Controller.h"

#include <ESP8266WiFi.h>


IPAddress ap_ip(10, 0, 0, 1);
IPAddress ap_gateway(10, 0, 0, 0);
IPAddress ap_subnet(255, 255, 255, 0);


int wifi_connect()
{
    //WiFi.begin(); // Utiliza últimas credenciais
    WiFi.begin("lucas@wifi", "35879423");
    
    Serial.print("[WIFI]: Conectando");
    
    for (int i = 0; i < 10; i++)
    {
        delay(500);
        Serial.print(".");
    }
    
    Serial.println();
    Serial.print("[WIFI]: IP");
    Serial.println(WiFi.localIP());

    return WiFi.isConnected();
}

int wifi_keep()
{
    if (!WiFi.isConnected())
        Serial.print("[WIFI]: Reconectando");
    
    for (int i = 0; i < 5 && !WiFi.isConnected(); i++)
    {
        delay(500);
        Serial.print(".");
        WiFi.reconnect();
    }

    Serial.println();
    Serial.print("[WIFI]: IP");
    Serial.println(WiFi.localIP());

    return WiFi.isConnected();
}

void wifi_setup_ap()
{
    Serial.println("[WIFI]: Configurando Access Point");
    
    WiFi.softAPConfig(ap_ip, ap_gateway, ap_subnet);
    WiFi.softAP(AP_NAME);

    Serial.print("[WIFI]: IP: ");
    Serial.println(WiFi.softAPIP());
}
