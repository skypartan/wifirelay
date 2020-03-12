#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>


#define HOSTNAME "WifiRelay"
#define APNAME HOSTNAME

IPAddress ap_ip(10, 0, 0, 1);
IPAddress ap_gateway(10, 0, 0, 0);
IPAddress ap_subnet(255, 255, 255, 0);

ESP8266WebServer server(80);

bool relay_on = false;
bool config_mode = false;


void setup()
{
    Serial.begin(115200); // Inicializando canal de comunicação para depuração
    Serial.println("[SISTEMA]: Inicializando.");

    pinMode(0, OUTPUT);

    WiFi.setAutoConnect(true);

    server.on("/", server_handle_control);
    server.on("/config", server_handle_config);
    server.onNotFound(server_handle_notfound);
}

void loop()
{
    // Mantendo/criando a conexão
    if (!WiFi.isConnected())
    {
        // Caso a conexão caia, tente realizar novamente
        bool connection_success = wifi_connect("", "");
        if (connection_success == false && config_mode == false) // Caso não consiga conectar, iniciar modo de configuração
        {
            config_mode = true;
            wifi_setupap();
        }
    }

    server.handleClient();

    if (relay_on == true)
        digitalWrite(0, HIGH);
    else
        digitalWrite(0, LOW);
}


bool wifi_connect(String ssid, String password)
{
    Serial.print("[WIFI]: Conectando");

    if (ssid == "")
        WiFi.begin(); // Utiliza últimas credenciais
    else
        WiFi.begin(ssid, password);
    
    for (int i = 0; i < 10; i++)
    {
        delay(500);
        Serial.print(".");
    }
    
    if (WiFi.isConnected())
    {
        Serial.println();
        Serial.print("[SISTEMA]: IP: ");
        Serial.println(WiFi.localIP());
        
        if (MDNS.begin("wifirelay"))
            Serial.println("[mDNS]: DNS 'wifirelay.local' configurado com sucesso.");
        else
            Serial.println("[mDNS]: Erro ao configurar DNS.");


        server.begin();
        Serial.println("[SISTEMA]: Servidor iniciado.");

        return true;
    }

    Serial.println("[WIFI]: Falha ao conectar.");

    return true;
}

void wifi_setupap()
{
    Serial.println("[WIFI]: Iniciando Access Point.");
    
    WiFi.softAPConfig(ap_ip, ap_gateway, ap_subnet);
    WiFi.softAP(APNAME);

    Serial.print("[SISTEMA]: IP: ");
    Serial.println(WiFi.softAPIP());

    if (MDNS.begin("wifirelay"))
        Serial.println("[mDNS]: DNS 'wifirelay.local' configurado com sucesso.");
    else
        Serial.println("[mDNS]: Erro ao configurar DNS.");


    server.begin();
    Serial.println("[SISTEMA]: Servidor iniciado.");
}


void server_handle_control()
{
    if (server.method() == HTTP_GET)
    {
        Serial.println("[SISTEMA]: Nova conexão.");
        server_print_controlpage();
    }

    if (server.method() == HTTP_POST)
    {
        Serial.println("[SISTEMA]: Recebendo informações de controle.");

        if (!relay_on)
        {
            Serial.println("[WIFI]: Alterando estado para ligado.");
            relay_on = true;
        }
        else
        {
            Serial.println("[WIFI]: Alterando estado para desligado.");
            relay_on = false;
        }

        server_print_controlpage();
    }
}

void server_handle_config()
{
    if (server.method() == HTTP_GET)
    {
        Serial.println("[SISTEMA]: Nova conexão.");
        server_print_configpage();
    }

    if (server.method() == HTTP_POST)
    {
        Serial.println("[SISTEMA]: Recebendo informações de WiFi.");

        String ssid = server.arg("ssid");
        String password = server.arg("password");

        Serial.println("[WIFI]: Dados informados > SSID: " + ssid + " | Password: " + password);

        server_print_connectedpage();
        bool connection_success = wifi_connect(ssid, password);
        if (connection_success == false)
            WiFi.softAPdisconnect(); // Desligando AP?
    }
}

void server_handle_notfound()
{
    Serial.println("[SISTEMA]: Nova conexão.");

    String page = "<!DOCTYPE html>";
    page += "<html>";
    page += "    <head>";
    page += "        <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
    page += "        <title>WifiRelay - Config</title>";
    page += "    </head>";
    page += "    <body>";
    page += "        <p>Função não encontrada.</p>";
    page += "    </body>";
    page += "</html>";
    
    server.send(404, "text/html", page);
}


void server_print_controlpage()
{
    String page = "<!DOCTYPE html>";
    page += "<html>";
    
    page += "    <head>";
    page += "        <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
    page += "        <style>";
    page += "            html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}";
    page += "            .button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;";
    page += "            text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}";
    page += "           .button2 {background-color: #77878A;}";
    page += "        </style>";
    page += "        <title>WifiRelay</title>";
    page += "    </head>";
    
    page += "    <body>";
    page += "        <h1>WifiRelay</h1>";
    
    if (relay_on)
        page += "       <p><a href=\"/\"><button class=\"button button2\">Desligar</button></a></p>";
    else
        page += "       <p><a href=\"/\"><button class=\"button\">Ligar</button></a></p>";

    page += "    </body>";
    page += "</html>";
    
    server.send(200, "text/html", page);
}

void server_print_configpage()
{
    String page = "<!DOCTYPE html>";
    page += "<html>";
    
    page += "    <head>";
    page += "        <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
    page += "        <style>";
    page += "            html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}";
    page += "            .button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;";
    page += "            text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}";
    page += "        </style>";
    page += "        <title>WifiRelay - Config</title>";
    page += "    </head>";
    
    page += "    <body>";
    page += "        <h1>WifiRelay</h1>";
    page += "        <form action=\"\" method=\"POST\">";
    page += "            SSID: <input type=\"text\" name=\"ssid\" /><br />";
    page += "            Password: <input type=\"password\" name=\"password\" /><br />";
    page += "            <input type=\"submit\" class=\"button\" value=\"Conectar\" />";
    page += "        </form>";
    page += "    </body>";
    
    page += "</html>";
    
    server.send(200, "text/html", page);
}

void server_print_connectedpage()
{
    String page = "<!DOCTYPE html>";
    page += "<html>";
    page += "    <head>";
    page += "        <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
    page += "        <title>WifiRelay - Config</title>";
    page += "    </head>";
    page += "    <body>";
    page += "        <p>Dados recebidos, tentando conexão.</p>";
    page += "    </body>";
    page += "</html>";
    
    server.send(200, "text/html", page);
}
