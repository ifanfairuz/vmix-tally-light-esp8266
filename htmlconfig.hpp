
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "setting.hpp"

#ifndef HTML_CONFIG_H
#define HTML_CONFIG_H


String html(String deviceName, String b);
String genAccordion(String t, String b);
String genInput(String n, String l, String v, String t = String("text"), String a = String("required"));
String genOption(String v, String l, bool s = false);
String genPinOptions(uint8_t s = 99);
String genColorOptions(uint8_t s = 99);
String genSelect(String n, String l, String o, String a = String("required"));
String genGrid(String b);

#endif