#include "server.h"
#include "socket.h"


/**
 *  Получает инстанс WebServer'a.

    Throws:
    ex. code 1 - ServerSettings is null
    ex. code 2 - Достигнуто максимальное кол-во попыток подключения к точке доступа
    ex. code 3 - Не удалось создать точку доступа
    ex. code 4 - Не удалось конфигурировать точку доступа
    ex. code 5 - Не удалось поднять mDNS
 */
WebServer* WebServer::GetInstance(const ServerSettings* settings)
{
    if (settings == nullptr && _singleton == nullptr)
    {
        throw Exception(1, F("ServerSettings ptr is null"));
    }

    if(_singleton == nullptr){
        _singleton = new WebServer(settings);
        _singleton->_localIp = new IPAddress(settings->Ip[0], settings->Ip[1], settings->Ip[2], settings->Ip[3]);
        _singleton->_gateway = _singleton->_localIp;
        if (bitRead(settings->Flags, 0) == true)
        {
            WiFi.mode(WIFI_AP);
            if (!WiFi.softAP(settings->SSID, settings->Password)){
                throw Exception(3, F("Failed to set up an access point"));
            }
            delay(100); // Навсякий пусть будет
            if (!WiFi.softAPConfig(*_singleton->_localIp, *_singleton->_gateway, *_singleton->_subnet)){
                throw Exception(3, F("Failed to configure an access point"));
            }
            delay(100); // Навсякий пусть будет
            Logger::Log(F("Access point set up"));
            Logger::Log(StringsUtil::Concat(F("SSID: "), settings->SSID));
            Logger::Log(StringsUtil::Concat(F("Password: "), settings->Password));
        }
        else
        {
            WiFi.mode(WIFI_STA);
            if (settings->Password == nullptr)
            {
                WiFi.begin(settings->SSID);
            }
            else
            {
                WiFi.begin(settings->SSID, settings->Password);
            }
            byte attepmptsCounter = 0;
            while (WiFi.status() != WL_CONNECTED && attepmptsCounter < settings->MaxConnectingAttempts)
            {
                delay(500);
                attepmptsCounter++;
            }
            if (WiFi.status() != WL_CONNECTED)
            {
                throw Exception(2, F("The maximum number of attempts to connect to the access point have been reached"));
            }
        }

        _singleton->_server = new AsyncWebServer(settings->Port);
        _singleton->_socket = new WebSocket(settings);

        // Если нужно - поднимаем mDNS
        if (bitRead(settings->Flags, 3) == 1)
        {
            _singleton->_mdnsResponder = new MDNSResponder();
            if (!_singleton->_mdnsResponder->begin("esp8266", *_singleton->_localIp)) 
            {     
                throw Exception(5, F("Failed to set up mDNS"));
            }
        }
        

        _singleton->_socket->AppendSocket(_singleton->_server);

        _singleton->_server->on("/", HTTP_GET, [](AsyncWebServerRequest *request)
        {
            request->send_P(200, "text/html", TXT_WebPage);
        });
        _singleton->_server->on("/Localization.xml", HTTP_GET, [](AsyncWebServerRequest *request)
        {
            request->send_P(200, "text/plain", TXT_Localization);
        });
        _singleton->_server->on("/api/getFVerion", HTTP_GET, [](AsyncWebServerRequest *request)
        {
            request->send(200, "text/plain", FIRMWARE_VERSION_STR);
        });
        _singleton->_server->on("/api/GetProfilesIndices", HTTP_GET, [](AsyncWebServerRequest *request)
        {
            try{
		        SDCardUtil::GetFile(F("PGP/Profiles.dat"), [request](fs::File* filePtr){
                    uint8_t* buff = new uint8_t[filePtr->size()];
                    filePtr->read(buff, filePtr->size());
                    request->send(*filePtr);
                }, true);
	        }catch(const Exception& ex){
                // request->
                Logger::LogError(ex.what());	
	        }
            // request->send_P(200, "text/plain", FIRMWARE_VERSION_STR);
        });
        // _singleton->_server->on("/api/getProfileIndices", HTTP_GET, [](AsyncWebServerRequest *request)
        // {
        //     request->send_P(200, "text/plain", FIRMWARE_VERSION_STR);
        // });

        _singleton->_server->serveStatic("/", SPIFFS, "/");
        _singleton->_server->begin();

        if (bitRead(settings->Flags, 3) == 1){
            _singleton->_mdnsResponder->addService("http", "tcp", settings->Port);
        }

        _singleton->_serverRunning = true;
        Logger::Log(F("----------Server started----------"));
        Logger::Log(StringsUtil::Concat(F("Port: "), String(settings->Port)));
        Logger::Log(StringsUtil::Concat(F("Local IP: "), _singleton->_localIp->toString()));
        Logger::Log(StringsUtil::Concat(F("Gateway IP: "), _singleton->_gateway->toString()));
        Logger::Log(StringsUtil::Concat(F("Subnet IP: "), _singleton->_subnet->toString()));
        Logger::Log(StringsUtil::Concat(F("mDNS name: "), bitRead(settings->Flags, 3) == 1 ? F("http://pgp.local") : F("DISABLED")));
    }
    return _singleton;
}

WebServer::~WebServer(){
    Logger::Log(F("Server stopped"));
    if (this->_socket != nullptr)
    {
        delete this->_socket;
    }
    if (this->_server != nullptr)
    {
        this->_server->end();
    }

    delete this->_server;
    delete this->_socket;
    delete [] this->_gateway;
    delete [] this->_localIp;
    delete [] this->_subnet;

    this->_serverRunning = false;
}

bool WebServer::IsRunning(){
    return this->_serverRunning;
}

void WebServer::Update(){
    if (!this->_serverRunning) return;
    this->_socket->CleanupClients();
    if (IS_ENABLED_MDNS == 1){
        // Logger::Log(F("update"));
        this->_mdnsResponder->update();
    }
}
