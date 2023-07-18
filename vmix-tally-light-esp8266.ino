#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h>
#include "FS.h"
#include "setting.hpp"
#include "htmlconfig.hpp"
#include "rgbled.hpp"

ESP8266WebServer httpServer(80);
WiFiClient client;
Setting setting;
RGBLED led;
bool APMode = false;
char deviceName[32];
int interval = 1000;
ulong lastCheck = 0;
char tallyState = -1;

void getSetting()
{
    setting = loadSetting();
    printSetting(setting);
    led.loadSetting(setting);
    sprintf(deviceName, "TallySource%d", setting.sourceNumber);
}

void settingPage()
{
    String b = "<header><h2>" + String(deviceName) + " | Setting</h2></header>";
    b += genAccordion(
        "Access Point",
        genInput("ap_ssid", "SSID", setting.apSSID) +
        genInput("ap_password", "Password", setting.apPass)
    );
    b += genAccordion(
        "VMix",
        genInput("vmix_host", "Host", setting.vmixHost) +
        genInput("vmix_port", "Port", String(setting.vmixPort), "number", "required min='1' max='99999'") +
        genInput("vmix_source", "Source Number", String(setting.sourceNumber), "number", "required min='1' max='99'")
    );
    b += genAccordion(
        "LED",
        genGrid(
            genSelect("pin_red", "RED", genPinOptions(setting.ledRed)) +
            genSelect("pin_green", "GREEN", genPinOptions(setting.ledGreen)) +
            genSelect("pin_blue", "BLUE", genPinOptions(setting.ledBlue))
        ) +
        genGrid(
            genSelect("led_apmode", "AP Mode", genColorOptions(setting.colorAP)) +
            genSelect("led_connecting", "Connecting VMix", genColorOptions(setting.colorConnectingVMix)) +
            genSelect("led_preview", "Preview VMix", genColorOptions(setting.colorPreviewVMix)) +
            genSelect("led_live", "Live VMix", genColorOptions(setting.colorLiveVMix))
        )
    );
    b += "<footer><button type='submit'>Save</button></footer>";
    httpServer.sendHeader("Connection", "close");
    httpServer.send(200, "text/html", html(deviceName, b));
}

bool getHttpDataChar(String name, int l, char *res)
{
    if (httpServer.hasArg(name) && httpServer.arg(name).length() <= l)
    {
        String data = httpServer.arg(name);
        if (!data.equals(res)) {
            data.toCharArray(res, l);
            return true;
        }
    }
    return false;
}

bool getHttpDataInt(String name, int l, int *res)
{
    if (httpServer.hasArg(name) && httpServer.arg(name).length() <= l)
    {
        int data = httpServer.arg(name).toInt();
        if (*res != data) {
            *res = data;
            return true;
        }
    }
    return false;
}

bool getHttpDataUInt(String name, int l, uint8_t *res)
{
    if (httpServer.hasArg(name) && httpServer.arg(name).length() <= l)
    {
        int data = httpServer.arg(name).toInt();
        if (*res != data) {
            *res = data;
            return true;
        }
    }
    return false;
}

void onSettingSave()
{
    httpServer.sendHeader("Location", String("/"), true);
    httpServer.send(302, "text/plain", "redirected to: /");

    bool changed = false;
    bool wifiChanged = false;
    bool vMixChanged = false;
    String data;

    wifiChanged += getHttpDataChar("ap_ssid", SSID_MAX_LENGTH, setting.apSSID);
    wifiChanged += getHttpDataChar("ap_password", PASS_MAX_LENGTH, setting.apPass);

    vMixChanged += getHttpDataChar("vmix_host", VMIX_HOST_MAX_LENGTH, setting.vmixHost);
    vMixChanged += getHttpDataInt("vmix_port", VMIX_PORT_MAX_LENGTH, &setting.vmixPort);
    vMixChanged += getHttpDataInt("vmix_source", VMIX_SOURCE_MAX_LENGTH, &setting.sourceNumber);

    changed += getHttpDataUInt("pin_red", LED_PIN_MAX_LENGTH, &setting.ledRed);
    changed += getHttpDataUInt("pin_green", LED_PIN_MAX_LENGTH, &setting.ledGreen);
    changed += getHttpDataUInt("pin_blue", LED_PIN_MAX_LENGTH, &setting.ledBlue);
    changed += getHttpDataUInt("led_apmode", LED_COLOR_MAX_LENGTH, &setting.colorAP);
    changed += getHttpDataUInt("led_connecting", LED_COLOR_MAX_LENGTH, &setting.colorConnectingVMix);
    changed += getHttpDataUInt("led_preview", LED_COLOR_MAX_LENGTH, &setting.colorPreviewVMix);
    changed += getHttpDataUInt("led_live", LED_COLOR_MAX_LENGTH, &setting.colorLiveVMix);

    if (wifiChanged || vMixChanged || changed) {
        saveSetting(setting);
        Serial.println("# SETTING SAVED");
        getSetting();
    }

    if (wifiChanged) {
        reconnectToWifi();
    } else if (vMixChanged || changed) {
        reconnectTovMix();
    }
}

void setupHttp()
{
  httpServer.on("/", HTTP_GET, settingPage);
  httpServer.on("/save", HTTP_POST, onSettingSave);
  httpServer.serveStatic("/", SPIFFS, "/", "max-age=315360000");
  httpServer.begin();
}

void apStart()
{
    if (APMode) return;
    APMode = true;
    
    IPAddress Ip(192, 168, 2, 1);
    IPAddress NMask(255, 255, 255, 0);
    char password[64] = "12345678";

    WiFi.mode(WIFI_AP);
    WiFi.hostname(deviceName);
    WiFi.softAPConfig(Ip, Ip, NMask);
    WiFi.softAP(deviceName, password);

    led.on(setting.colorAP);
    Serial.printf("# AP MODE \nSSID: %s \nPASS: %s \nIP: %s \n", deviceName, password, Ip.toString().c_str());
}

bool wifiConnected()
{
    return WiFi.getMode() == WIFI_STA && WiFi.status() == WL_CONNECTED && WiFi.SSID().equals(setting.apSSID);
}

void connectToWifi()
{
    WiFi.mode(WIFI_STA);
    WiFi.hostname(deviceName); 
    APMode = false;
    
    int timeout = 20;
    Serial.printf("# CONNECT TO WIFI \nSSID: %s \nPASS: %s \n", setting.apSSID, setting.apPass);
    Serial.print("connecting...");
 
    wl_status_t status = WiFi.begin(setting.apSSID, setting.apPass);
    for (int i = 0; i < timeout; i++) {
        status = WiFi.status();

        if (status == WL_CONNECTED) {
            break;
        }

        led.blink(setting.colorAP);
        delay(500);
        Serial.print(".");
        led.blink(setting.colorAP);
        delay(500);
    }

    if (wifiConnected()) {
        Serial.print("Connected\n");
        Serial.printf("# WIFI STATUS \nSSID: %s \nIP: %s \nHOST: %s \n", WiFi.SSID(), WiFi.localIP().toString().c_str(), deviceName);
        connectTovMix();
        return;
    }

    if (status == WL_IDLE_STATUS) Serial.println("Idle");
    else if (status == WL_NO_SSID_AVAIL) Serial.println("No SSID Available");
    else if (status == WL_SCAN_COMPLETED) Serial.println("Scan Completed");
    else if (status == WL_CONNECT_FAILED) Serial.println("Connection Failed");
    else if (status == WL_CONNECTION_LOST) Serial.println("Connection Lost");
    else if (status == WL_DISCONNECTED) Serial.println("Disconnected");
    else Serial.println("Unknown Failure");
    apStart();
}

void reconnectToWifi()
{
    led.off();
    WiFi.disconnect();
    connectToWifi();
}

void connectTovMix()
{
    led.on(setting.colorConnectingVMix);
    Serial.printf("# CONNECT TO VMIX %s:%d...", setting.vmixHost, setting.vmixPort);

    if (client.connect(setting.vmixHost, setting.vmixPort)) {
        Serial.println("Connected");
        client.println("SUBSCRIBE TALLY");
        tallyState = -1;
        processTally();
    } else {
        led.off();
        Serial.println("NotFound");
    }
}

void reconnectTovMix()
{
    client.flush();
    client.stopAll();
    client.stop();
    connectTovMix();
}

void processTally()
{
    String data = client.readStringUntil('\r\n');
    if (data.indexOf("TALLY") == 0)
    {
        char newState = data.charAt(setting.sourceNumber + 8);
        if (tallyState != newState) {
            switch (newState) {
                case '0':
                    led.off();
                    break;
                case '1':
                    led.on(setting.colorLiveVMix);
                    break;
                case '2':
                    led.on(setting.colorPreviewVMix);
                    break;
                default:
                    led.off();
            }
            tallyState = newState;
        }
    }
}

void setup()
{
    Serial.begin(9600);
    EEPROM.begin(EEPROM_SIZE);
    SPIFFS.begin();

    setupHttp();
    getSetting();
    led.test();
    connectToWifi();
}

void loop()
{
    httpServer.handleClient();

    while (client.available()) {
        processTally();
    }

    if (millis() > lastCheck + interval) {
        if (!APMode && !wifiConnected()) {
            reconnectToWifi();
        } else if (wifiConnected() && !client.connected()) {
            reconnectTovMix();
        }

        lastCheck = millis();
    }
}
