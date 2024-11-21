#ifndef LOGGER_H
#define LOGGER_H

// #include <Arduino.h>
// #include "utils/stringsUtils.h"
#include "server.h"
// #include "utils/sdCardUtil.h"

// #define getConfig()  

class Logger{
private:
    static void logInFile(String& msg){
        logInFile(msg.c_str());
    }
    static void logInFile(String* msg){
        logInFile(msg->c_str());
    }

    static void logInFile(const char* msg){
        try{
            SDCardUtil::GetFile(("PGP/logs/log.log"), [msg](fs::File* f){
                f->write(msg);
                f->write('\n');
                // f->println(msg);
                f->close();
            });
        }catch(Exception& ex){
            if (ex.exitCode() == 1){
                // Файла лога не существует => создаём
                SDCardUtil::CreateFile(("PGP/logs/log.log"), [msg](fs::File* f){
                    f->write(msg);
                    f->write('\n');
                    // f->println(msg);
                    f->close();
                }, true);
            }
        }
    }

    static const ServerSettings* getConfig(){
        return WebServer::GetInstance()->Config;
    }
public:
    /*Записывает сообщение об ошибке в сериал(если мы не работаем с dmx-ом) и на sd карту в лог(если она вставлена) */
    static void LogError(const char* msg){
        String* m = new String(F("E: "));
        m->concat(msg);
        if (Serial.availableForWrite() && ((getConfig() == nullptr ? true : bitRead(getConfig()->Flags, 2)) == true)){
            Serial.println(*m);
        }   
        if (getConfig() == nullptr ? true : bitRead(getConfig()->Flags, 1) == true) { logInFile(m); }  
        delete m; 
    }
    /*Записывает сообщение об ошибке в сериал(если мы не работаем с dmx-ом) и на sd карту в лог(если она вставлена) */
    static void LogError(String& msg){
        String* m = new String(F("E: "));
        m->concat(msg);
        if (Serial.availableForWrite() && ((getConfig() == nullptr ? true : bitRead(getConfig()->Flags, 2)) == true)){
            Serial.println(*m);
        }   
        if (getConfig() == nullptr ? true : bitRead(getConfig()->Flags, 1) == true) { logInFile(msg); }  
        delete m; 
    }
    /*Записывает сообщение об ошибке в сериал(если мы не работаем с dmx-ом) и на sd карту в лог(если она вставлена) */
    static void LogError(String* msg){
        String* m = new String(F("E: "));
        m->concat(*msg);
        if (Serial.availableForWrite() && ((getConfig() == nullptr ? true : bitRead(getConfig()->Flags, 2)) == true)){
            Serial.println(*m);
        }   
        if (getConfig() == nullptr ? true : bitRead(getConfig()->Flags, 1) == true) { logInFile(msg); }  
        delete m; 
        delete msg; 
    }
    /*Записывает сообщение об ошибке в сериал(если мы не работаем с dmx-ом) и на sd карту в лог(если она вставлена) */
    static void LogError(const __FlashStringHelper* msg){
        String* m = new String(F("ERR: "));
        m->concat(msg);
        if (Serial.availableForWrite() && ((getConfig() == nullptr ? true : bitRead(getConfig()->Flags, 2)) == true)){
            Serial.println(*m);
        }     
        if (getConfig() == nullptr ? true : bitRead(getConfig()->Flags, 1) == true) { logInFile(m); }  
        delete m; 
    }
    /*Записывает сообщение об фатальной ошибке в сериал(если мы не работаем с dmx-ом) и на sd карту в лог(если она вставлена)
    Также начиниет мигать светодиод состояния
     */
    static void LogFatalError(const char* msg){
        String* m = new String(F("FAT_ERR: "));
        m->concat(msg);
        if (Serial.availableForWrite() && ((getConfig() == nullptr ? true : bitRead(getConfig()->Flags, 2)) == true)){
            Serial.println(*m);
        }
        if (getConfig() == nullptr ? true : bitRead(getConfig()->Flags, 1) == true) { logInFile(m); }  
        delete m;
    }
    /*Записывает сообщение об фатальной ошибке в сериал(если мы не работаем с dmx-ом) и на sd карту в лог(если она вставлена)
    Также начиниет мигать светодиод состояния
     */
    static void LogFatalError(String& msg){
        String* m = new String(F("FAT_ERR: "));
        m->concat(msg);
        if (Serial.availableForWrite() && ((getConfig() == nullptr ? true : bitRead(getConfig()->Flags, 2)) == true)){
            Serial.println(*m);
        }
        if (getConfig() == nullptr ? true : bitRead(getConfig()->Flags, 1) == true) { logInFile(m); }  
        delete m;
    }
    /*Записывает сообщение об фатальной ошибке в сериал(если мы не работаем с dmx-ом) и на sd карту в лог(если она вставлена)
    Также начиниет мигать светодиод состояния
     */
    static void LogFatalError(String* msg){
        String* m = new String(F("FAT_ERR: "));
        m->concat(*msg);
        if (Serial.availableForWrite() && ((getConfig() == nullptr ? true : bitRead(getConfig()->Flags, 2)) == true)){
            Serial.println(*m);
        }
        if (getConfig() == nullptr ? true : bitRead(getConfig()->Flags, 1) == true) { logInFile(m); }  
        delete m;
        delete msg;
    }
    /*Записывает сообщение об фатальной ошибке в сериал(если мы не работаем с dmx-ом) и на sd карту в лог(если она вставлена)
    Также начиниет мигать светодиод состояния
     */
    static void LogFatalError(const __FlashStringHelper* msg){
        String* m = new String(F("FAT_ERR: "));
        m->concat(msg);
        if (Serial.availableForWrite() && ((getConfig() == nullptr ? true : bitRead(getConfig()->Flags, 2)) == true)){
            Serial.println(*m);
        }
        if (getConfig() == nullptr ? true : bitRead(getConfig()->Flags, 1) == true) { logInFile(m); }  
        delete m;
    }
    /*Записывает обычное сообщение в сериал(если мы не работаем с dmx-ом) и на sd карту в лог(если она вставлена) */
    static void Log(String& msg){
        String* m = new String(F("L: "));
        m->concat(msg);
        if (Serial.availableForWrite() && ((getConfig() == nullptr ? true : bitRead(getConfig()->Flags, 2)) == true)){
            Serial.println(*m);
        } 
        if (getConfig() == nullptr ? true : bitRead(getConfig()->Flags, 1) == true) { logInFile(m); }  
        delete m; 
    }
    /*Записывает обычное сообщение в сериал(если мы не работаем с dmx-ом) и на sd карту в лог(если она вставлена) */
    static void Log(String* msg){
        String* m = new String(F("L: "));
        m->concat(*msg);
        if (Serial.availableForWrite() && ((getConfig() == nullptr ? true : bitRead(getConfig()->Flags, 2)) == true)){
            Serial.println(*m);
        } 
        if (getConfig() == nullptr ? true : bitRead(getConfig()->Flags, 1) == true) { logInFile(m); }  
        delete m;
        delete msg;  
    }
    /*Записывает обычное сообщение в сериал(если мы не работаем с dmx-ом) и на sd карту в лог(если она вставлена) */
    static void Log(const __FlashStringHelper *msg){
        String* m = new String(F("L: "));
        m->concat(msg);
        if (Serial.availableForWrite() && ((getConfig() == nullptr ? true : bitRead(getConfig()->Flags, 2)) == true)){
            Serial.println(*m);
        } 
        if (getConfig() == nullptr ? true : bitRead(getConfig()->Flags, 1) == true) { logInFile(m); }  
        delete m; 
    }
};

#endif