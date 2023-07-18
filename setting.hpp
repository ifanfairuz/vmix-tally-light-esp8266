#ifndef MY_SETTING_H
#define MY_SETTING_H

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#define SSID_MAX_LENGTH 64
#define PASS_MAX_LENGTH 64
#define VMIX_HOST_MAX_LENGTH 64
#define VMIX_PORT_MAX_LENGTH 6
#define VMIX_SOURCE_MAX_LENGTH 3
#define LED_PIN_MAX_LENGTH 3
#define LED_COLOR_MAX_LENGTH 2
#define EEPROM_SIZE 512

static const uint8_t C_OFF = 0;
static const uint8_t C_RED = 1;
static const uint8_t C_GREEN = 2;
static const uint8_t C_BLUE = 3;
static const uint8_t C_YELLOW = 4;
static const uint8_t C_CYAN = 5;
static const uint8_t C_PURPLE = 6;

struct Setting
{
  char apSSID[SSID_MAX_LENGTH];
  char apPass[PASS_MAX_LENGTH];
  char vmixHost[VMIX_HOST_MAX_LENGTH];
  int vmixPort;
  int sourceNumber;
  uint8_t ledRed;
  uint8_t ledGreen;
  uint8_t ledBlue;
  uint8_t colorAP;
  uint8_t colorConnectingVMix;
  uint8_t colorPreviewVMix;
  uint8_t colorLiveVMix;
};

void printSetting(Setting s);
void saveSetting(Setting s);
Setting loadSetting();

#endif