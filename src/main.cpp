#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "FS.h"
#include "SD.h"
#include "SPI.h"

// pins => CS -> 5 , SCK -> D18 , MISO -> D19 , MOSI -> D23 , VCC -> 5v

// Replace with custom access point name and password
const char *ssid = "pzeinstein";
const char *password = "12341234";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

void initSDCard()
{
    if (!SD.begin())
    {
        Serial.println("Card Mount Failed");
        return;
    }
    uint8_t cardType = SD.cardType();

    if (cardType == CARD_NONE)
    {
        Serial.println("No SD card attached");
        return;
    }

    Serial.print("SD Card Type: ");
    if (cardType == CARD_MMC)
    {
        Serial.println("MMC");
    }
    else if (cardType == CARD_SD)
    {
        Serial.println("SDSC");
    }
    else if (cardType == CARD_SDHC)
    {
        Serial.println("SDHC");
    }
    else
    {
        Serial.println("UNKNOWN");
    }
    uint64_t cardSize = SD.cardSize() / (1024 * 1024);
    Serial.printf("SD Card Size: %lluMB\n", cardSize);
}

void initWiFi()
{
    WiFi.softAP(ssid, password);
    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP);
}

void setup()
{

    Serial.begin(115200);

    initWiFi();
    initSDCard();

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(SD, "/app/index.html", "text/html"); });

    server.serveStatic("/", SD, "/app/");
    server.onNotFound([](AsyncWebServerRequest *request)
                      { request->send(SD, "/app/index.html", "text/html"); });
    server.begin();
}

void loop() {}
