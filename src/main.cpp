#include <Arduino.h>
#include <ESP8266WiFi.h>

#include "consts.h"
#include "leds.h"
#include "utils.h"

const char WiFiPassword[] = "12345678";
const char AP_NameChar[] = "cruisecontrol";
int analogPin = A0;

static const char *response =
    "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n"
    "<!DOCTYPE html><html><head><title>LED Control</title></head><body>"
    "<h2>Speed Up</h2><form id='F1' action='LEDON1'><input class='button' type='submit' value='LED ON' ></form>"
    "<h2>Speed Down</h2><form id='F1' action='LEDON2'><input class='button' type='submit' value='LED ON' ></form>"
    "<h2>Cancel</h2><form id='F1' action='LEDON3'><input class='button' type='submit' value='LED ON' ></form>"
    "<h2>Switch On - Off</h2><form id='F1' action='LEDON4'><input class='button' type='submit' value='LED ON' ></form>"
    "</body></html>";

static const uint8_t LED1_PIN = D5;
static const uint8_t LED2_PIN = D6;
static const uint8_t LED3_PIN = D7;
static const uint8_t LED4_PIN = D8;

enum LedEnum : uint8_t {
    LED_1,
    LED_2,
    LED_3,
    LED_4,
    NUM_LEDS
};

LedItem leds[NUM_LEDS] = {
    {LED1_PIN},
    {LED2_PIN},
    {LED3_PIN},
    {LED4_PIN},
};

void initSerial() {
    Serial.begin(115200);
    Serial.println();
}

void initWiFi() {
    WiFi.softAP(AP_NameChar, WiFiPassword);
    delay(2000);
}

void initLed() {
    for (uint8_t i = 0; i < NUM_LEDS; i++) {
        leds[i].init();
    }
}

void setup() {
    initSerial();
    initWiFi();
    initLed();
}

void refreshLeds() {
    for (uint8_t i = 0; i < NUM_LEDS; i++) {
        leds[i].loop();
    }
}

static const int infelicity = 20;  //infelicity in cruise control controller
static const uint16_t down = 515;     //resistance of cruise controller when up speed in ohm
static const uint16_t up = 142;       //resistance of cruise controller when down speed in ohm
static const uint16_t cancel = 1379;  //resistance of cruise controller when cancel in ohm
static const float V_IN = 3.28;
static const float R1 = 2000;
static const float RX = 100000;
void handleControl() {        
    uint16_t raw = analogRead(analogPin);
    if (raw) {
        float tmp = raw * V_IN;
        float Vout = tmp / 1024.0;
        tmp = (V_IN / Vout) - 1;       
        float R2 = R1 * tmp;
        if (R2 <= (down + (down / 100 * infelicity)) && R2 >= (down - (down / 100 * infelicity))) {
            leds[LED_2].onPress(true);
        } else 
        if (R2 <= (up + (up / 100 * infelicity)) && R2 >= (up - (up / 100 * infelicity))) {
            leds[LED_1].onPress(true);
        } else 
        if (R2 <= (cancel + (cancel / 100 * infelicity)) && R2 >= (cancel - (cancel / 100 * infelicity))) {
            leds[LED_3].onPress(true);
        } else 
        if (R2 <= 0.5) {
            leds[LED_4].onPress(true);
        };
    }
}

WiFiServer server(80);
bool httpStarted{false};
uint32_t lastHandleControl{0};
void loop() {
    if (!httpStarted) {        
        server.begin();        
        server.setNoDelay(true);
        httpStarted = true;
    }

    if (TimePassedSince(lastHandleControl) > CONTROL_INTERVAL_ms) {
        handleControl();
        lastHandleControl = millis();
    }

    refreshLeds();

    if (server.available()) {
        WiFiClient client = server.available();                
        String request = client.readStringUntil('\r');        
        if (request.indexOf("LEDON1")) {
            leds[LED_1].onPress(true);
        } else if (request.indexOf("LEDON2")) {
            leds[LED_2].onPress(true);
        } else if (request.indexOf("LEDON3")) {
            leds[LED_3].onPress(true);
        } else if (request.indexOf("LEDON4")) {
            leds[LED_4].onPress(true);
        }        
        client.print(response);
        client.flush();
    }
}