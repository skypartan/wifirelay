#include "Controller.h"
#include "WifiManager.h"
#include <ESP8266WiFi.h>


#define TIMEOUT 2000

WiFiServer server(80);

int next_time = 0;
int previous_time = 0;


void server_setup_manager()
{
    Serial.println("[WIFI]: Iniciando servidor de gerenciamento");

    wifi_setup_ap();
    server.begin();
}

void server_setup_controller()
{
    Serial.println("[WIFI]: Iniciando servidor de gerenciamento");

    wifi_connect();
    server.begin();
}

void server_loop_manager()
{
    // Verificar se existe novo cliente conectado
    WiFiClient client = server.available();
    if (client.status() != CLOSED)
    {
        Serial.println("\n[WIFI]: Cliente conectado");

        if (client.available())
        {
            /*while ()
            {

            }*/
        }
        else
        {
            client.println("<!DOCTYPE html>");
            client.println("<head>");
            client.println("    <title>Manager</title>");
            client.println("</head>");
            client.println("<body>");
            client.println("    <form action=\"10.0.0.1\">");
            client.println("        SSID: <input type=\"text\" name=\"ssid\" /><br />");
            client.println("        Password: <input type=\"password\" name=\"password\" /><br />");
            client.println("        <input type=\"submit\" />");
            client.println("    </form>");
            client.println("</body>");
            client.println();

            delay(2500); // give the web browser time to receive the data
        }

        /*
        while (client.connected())
        {
            if (client.available())
            {
                String line = client.readStringUntil('\r');
                Serial.print(line);

                if (line.length() == 1 && line[0] == '\n')
                {
                    client.println("resposta");
                    break;
                }
            }
        }
        */

        client.stop();
        
        //Serial.println("[WIFI]: Cliente desconectado");
    }
}

void server_loop_controller()
{
    // Verificar se existe novo cliente conectado
    WiFiClient client = server.available();
    if (client.status() != CLOSED)
    {
        Serial.println("\n[WIFI]: Cliente conectado");

        next_time = millis();
        previous_time = next_time;

        while (client.connected() && next_time - previous_time <= TIMEOUT)
        {
            next_time = millis();

            if (client.available() > 0)
            {
                // Requisição recebida

                String header = "";

                char c = client.read();
                Serial.println("[REDE]: Dados:");
                Serial.write(c);

                header += c;

                if (c == '\n') // Requisição vazia
                {

                }
            }
            else
            {
                // Do nothing
            }
        }
    }        
}

void switch_toggle(int status)
{
    digitalWrite(0, status);
}
