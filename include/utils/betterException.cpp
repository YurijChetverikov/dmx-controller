#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <Arduino.h>
#include "utils/stringsUtils.h"

class Exception: public std::exception
{
private:
    const char* _msg;
    const byte _exitCode = 0;
public:
    Exception(const byte exitCode, const char* msg): _exitCode(exitCode), _msg{msg} {
        // Serial.println("30");
        Serial.println("Exception constructor");
    }
    Exception(const byte exitCode, String* msg): _exitCode(exitCode), _msg{msg->c_str()} {
        // Serial.println("30");
        Serial.println("Exception constructor");
    }
    Exception(const byte exitCode, const __FlashStringHelper* msg): _exitCode(exitCode) {
        // Serial.println("80");
        Serial.println("Exception constructor");
        this->_msg = StringsUtil::StringCopy(msg);
        // size_t size = strlen_P((PGM_P)msg);
        // if (size == 0)
        //     return;
        // char *data = new char[size];
        // if (data == NULL)
        //     return;
        // strcpy_P(data, (PGM_P)msg);
        // this->_msg = data;
        // delete [] data;
    }
    ~Exception(){
        Serial.println("Exception destructor");
        // delete[](_msg);
    }
    const char* what() const noexcept override
    {
        return _msg;
    }
    const byte exitCode(){
        return _exitCode;
    }
};


#endif