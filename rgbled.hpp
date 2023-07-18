#ifndef RGBLED_H
#define RGBLED_H

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "setting.hpp"

class RGBLED
{
private:
    uint8_t R;
    uint8_t G;
    uint8_t B;
    uint8_t color;
    bool state;
public:
    void loadSetting(Setting s);
    void on(u_int8_t color);
    void blink(u_int8_t color = C_OFF);
    void off();
    void test();
    void red();
    void green();
    void blue();
    void purple();
    void yellow();
    void cyan();
};

#endif