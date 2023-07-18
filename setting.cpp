#include <EEPROM.h>
#include <string.h>
#include "setting.hpp"

Setting defaultSetting = {
    "NAMA SSID WIFI",
    "PASSWORD WIFI",
    "192.168.2.2",
    8099,
    1,
    D4,
    D2,
    D3,
    C_GREEN,
    C_YELLOW,
    C_BLUE,
    C_RED
};

String pinLabel(uint8_t p)
{
  String pin = "";
  switch (p)
  {
    case D0:
      pin = "D0";
      break;
    case D1:
      pin = "D1";
      break;
    case D2:
      pin = "D2";
      break;
    case D3:
      pin = "D3";
      break;
    case D4:
      pin = "D4";
      break;
    case D5:
      pin = "D5";
      break;
    case D6:
      pin = "D6";
      break;
    case D7:
      pin = "D7";
      break;
    case D8:
      pin = "D8";
      break;
    default:
      pin = "NULL";
      break;
  }

  return pin;
}

String colorLabel(uint8_t c)
{
  String color = "";
  switch (c)
  {
    case C_RED:
      color = "Red";
      break;
    case C_GREEN:
      color = "Green";
      break;
    case C_BLUE:
      color = "Blue";
      break;
    case C_PURPLE:
      color = "Purple";
      break;
    case C_CYAN:
      color = "Cyan";
      break;
    case C_YELLOW:
      color = "Yellow";
      break;
    default:
      color = "NULL";
      break;
  }

  return color;
}

void printSetting(Setting s)
{
  Serial.print("\n####################### SETTINGS #######################\n");
  Serial.printf("ssid = %s\n", s.apSSID);
  Serial.printf("pass = %s\n", s.apPass);
  Serial.printf("vmixHost = %s\n", s.vmixHost);
  Serial.printf("vmixPort = %d\n", s.vmixPort);
  Serial.printf("sourceNumber = %d\n", s.sourceNumber);
  Serial.printf("PIN Red = %s\n", pinLabel(s.ledRed));
  Serial.printf("PIN Green = %s\n", pinLabel(s.ledGreen));
  Serial.printf("PIN Blue = %s\n", pinLabel(s.ledBlue));
  Serial.printf("Color APMode = %s\n", colorLabel(s.colorAP));
  Serial.printf("Color Connecting VMix = %s\n", colorLabel(s.colorConnectingVMix));
  Serial.printf("Color Preview VMix = %s\n", colorLabel(s.colorPreviewVMix));
  Serial.printf("Color Live VMix = %s\n", colorLabel(s.colorLiveVMix));
}

void saveSetting(Setting s)
{
  for (int i = 0; i < EEPROM_SIZE; i++)
  {
    EEPROM.write(i, 0);
  }

  long pointer = 0;
  for (int i = 0; i < SSID_MAX_LENGTH; i++)
  {
    EEPROM.write(pointer, s.apSSID[i]);
    pointer++;
  }
  for (int i = 0; i < PASS_MAX_LENGTH; i++)
  {
    EEPROM.write(pointer, s.apPass[i]);
    pointer++;
  }
  for (int i = 0; i < VMIX_HOST_MAX_LENGTH; i++)
  {
    EEPROM.write(pointer, s.vmixHost[i]);
    pointer++;
  }
  char vmixPort[VMIX_PORT_MAX_LENGTH];
  String(s.vmixPort).toCharArray(vmixPort, VMIX_PORT_MAX_LENGTH);
  for (int i = 0; i < VMIX_PORT_MAX_LENGTH; i++)
  {
    EEPROM.write(pointer, vmixPort[i]);
    pointer++;
  }
  char sourceNumber[VMIX_SOURCE_MAX_LENGTH];
  String(s.sourceNumber).toCharArray(sourceNumber, VMIX_SOURCE_MAX_LENGTH);
  for (int i = 0; i < VMIX_SOURCE_MAX_LENGTH; i++)
  {
    EEPROM.write(pointer, sourceNumber[i]);
    pointer++;
  }
  char ledRed[LED_PIN_MAX_LENGTH];
  String(s.ledRed).toCharArray(ledRed, LED_PIN_MAX_LENGTH);
  for (int i = 0; i < LED_PIN_MAX_LENGTH; i++)
  {
    EEPROM.write(pointer, ledRed[i]);
    pointer++;
  }
  char ledGreen[LED_PIN_MAX_LENGTH];
  String(s.ledGreen).toCharArray(ledGreen, LED_PIN_MAX_LENGTH);
  for (int i = 0; i < LED_PIN_MAX_LENGTH; i++)
  {
    EEPROM.write(pointer, ledGreen[i]);
    pointer++;
  }
  char ledBlue[LED_PIN_MAX_LENGTH];
  String(s.ledBlue).toCharArray(ledBlue, LED_PIN_MAX_LENGTH);
  for (int i = 0; i < LED_PIN_MAX_LENGTH; i++)
  {
    EEPROM.write(pointer, ledBlue[i]);
    pointer++;
  }
  char colorAP[LED_COLOR_MAX_LENGTH];
  String(s.colorAP).toCharArray(colorAP, LED_COLOR_MAX_LENGTH);
  for (int i = 0; i < LED_COLOR_MAX_LENGTH; i++)
  {
    EEPROM.write(pointer, colorAP[i]);
    pointer++;
  }
  char colorConnectingVMix[LED_COLOR_MAX_LENGTH];
  String(s.colorConnectingVMix).toCharArray(colorConnectingVMix, LED_COLOR_MAX_LENGTH);
  for (int i = 0; i < LED_COLOR_MAX_LENGTH; i++)
  {
    EEPROM.write(pointer, colorConnectingVMix[i]);
    pointer++;
  }
  char colorPreviewVMix[LED_COLOR_MAX_LENGTH];
  String(s.colorPreviewVMix).toCharArray(colorPreviewVMix, LED_COLOR_MAX_LENGTH);
  for (int i = 0; i < LED_COLOR_MAX_LENGTH; i++)
  {
    EEPROM.write(pointer, colorPreviewVMix[i]);
    pointer++;
  }
  char colorLiveVMix[LED_COLOR_MAX_LENGTH];
  String(s.colorLiveVMix).toCharArray(colorLiveVMix, LED_COLOR_MAX_LENGTH);
  for (int i = 0; i < LED_COLOR_MAX_LENGTH; i++)
  {
    EEPROM.write(pointer, colorLiveVMix[i]);
    pointer++;
  }

  EEPROM.commit();
}

Setting loadSetting()
{
  Setting s = defaultSetting;
  long pointer = 0;
  for (int i = 0; i < SSID_MAX_LENGTH; i++)
  {
    s.apSSID[i] = EEPROM.read(pointer);
    pointer++;
  }
  for (int i = 0; i < PASS_MAX_LENGTH; i++)
  {
    s.apPass[i] = EEPROM.read(pointer);
    pointer++;
  }
  for (int i = 0; i < VMIX_HOST_MAX_LENGTH; i++)
  {
    s.vmixHost[i] = EEPROM.read(pointer);
    pointer++;
  }
  char vmixPort[VMIX_PORT_MAX_LENGTH];
  for (int i = 0; i < VMIX_PORT_MAX_LENGTH; i++)
  {
    vmixPort[i] = EEPROM.read(pointer);
    pointer++;
  }
  s.vmixPort = String(vmixPort).toInt();
  char sourceNumber[VMIX_SOURCE_MAX_LENGTH];
  for (int i = 0; i < VMIX_SOURCE_MAX_LENGTH; i++)
  {
    sourceNumber[i] = EEPROM.read(pointer);
    pointer++;
  }
  s.sourceNumber = String(sourceNumber).toInt();
  char ledRed[LED_PIN_MAX_LENGTH];
  for (int i = 0; i < LED_PIN_MAX_LENGTH; i++)
  {
    ledRed[i] = EEPROM.read(pointer);
    pointer++;
  }
  s.ledRed = String(ledRed).toInt();
  char ledGreen[LED_PIN_MAX_LENGTH];
  for (int i = 0; i < LED_PIN_MAX_LENGTH; i++)
  {
    ledGreen[i] = EEPROM.read(pointer);
    pointer++;
  }
  s.ledGreen = String(ledGreen).toInt();
  char ledBlue[LED_PIN_MAX_LENGTH];
  for (int i = 0; i < LED_PIN_MAX_LENGTH; i++)
  {
    ledBlue[i] = EEPROM.read(pointer);
    pointer++;
  }
  s.ledBlue = String(ledBlue).toInt();
  char colorAP[LED_COLOR_MAX_LENGTH];
  for (int i = 0; i < LED_COLOR_MAX_LENGTH; i++)
  {
    colorAP[i] = EEPROM.read(pointer);
    pointer++;
  }
  s.colorAP = String(colorAP).toInt();
  char colorConnectingVMix[LED_COLOR_MAX_LENGTH];
  for (int i = 0; i < LED_COLOR_MAX_LENGTH; i++)
  {
    colorConnectingVMix[i] = EEPROM.read(pointer);
    pointer++;
  }
  s.colorConnectingVMix = String(colorConnectingVMix).toInt();
  char colorPreviewVMix[LED_COLOR_MAX_LENGTH];
  for (int i = 0; i < LED_COLOR_MAX_LENGTH; i++)
  {
    colorPreviewVMix[i] = EEPROM.read(pointer);
    pointer++;
  }
  s.colorPreviewVMix = String(colorPreviewVMix).toInt();
  char colorLiveVMix[LED_COLOR_MAX_LENGTH];
  for (int i = 0; i < LED_COLOR_MAX_LENGTH; i++)
  {
    colorLiveVMix[i] = EEPROM.read(pointer);
    pointer++;
  }
  s.colorLiveVMix = String(colorLiveVMix).toInt();

  return s;
}