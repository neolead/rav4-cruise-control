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

static const uint8_t LED_SPEED_UP_PIN = D5;
static const uint8_t LED_SPEED_DOWN_PIN = D6;
static const uint8_t LED_CANCEL_PIN = D7;
static const uint8_t LED_SWITCH_PIN = D8;

enum LedEnum : uint8_t {
    LED_SPEED_UP,
    LED_SPEED_DOWN,
    LED_CANCEL,
    LED_SWITCH,
    NUM_LEDS
};
enum ControlEnum : uint8_t { SPEED_UP,
                             SPEED_DOWN,
                             CANCEL_CMD,
                             SWITCH_CMD,
                             NUMS_CONTROL_STATES };

ControlEnum lastControlState{NUMS_CONTROL_STATES};

LedItem leds[NUM_LEDS] = {
    {LED_SPEED_UP_PIN},
    {LED_SPEED_DOWN_PIN},
    {LED_CANCEL_PIN},
    {LED_SWITCH_PIN},
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

static const int infelicity = 20;     //infelicity in cruise control controller
static const uint16_t down = 515;     //resistance of cruise controller when up speed in ohm
static const uint16_t up = 142;       //resistance of cruise controller when down speed in ohm
static const uint16_t cancel = 1379;  //resistance of cruise controller when cancel in ohm
static const float V_IN = 3.28;
static const float R1 = 2000;
static const float RX = 100000;


void onControlChange(ControlEnum newState) {
 if (lastControlState != newState) {
        if (lastControlState != NUMS_CONTROL_STATES) {
            leds[lastControlState].onStateChange(false);
        } 
        if (newState != NUMS_CONTROL_STATES) {
            leds[newState].onStateChange(true);
        }
        lastControlState = newState;    
    }
}

void updateControl() {
    uint16_t raw = analogRead(analogPin);
    if (!raw) {
        return;
    }
    float tmp = raw * V_IN;
    float Vout = tmp / 1024.0;
    tmp = (V_IN / Vout) - 1;
    float R2 = R1 * tmp;

    ControlEnum request = NUMS_CONTROL_STATES;
    if (R2 <= (down + (down / 100 * infelicity)) && R2 >= (down - (down / 100 * infelicity))) {
        request = SPEED_UP;
    } else if (R2 <= (up + (up / 100 * infelicity)) && R2 >= (up - (up / 100 * infelicity))) {
        request = SPEED_DOWN;
    } else if (R2 <= (cancel + (cancel / 100 * infelicity)) && R2 >= (cancel - (cancel / 100 * infelicity))) {
        request = CANCEL_CMD;
    } else if (R2 <= 0.5) {
        request = SWITCH_CMD;
    };
   
    onControlChange(request);        
}

WiFiServer server(80);
bool httpStarted{false};
uint32_t lastUpdateControl{0};
void loop() {
    if (!httpStarted) {
        server.begin();
        server.setNoDelay(true);
        httpStarted = true;
    }

    if (TimePassedSince(lastUpdateControl) > CONTROL_INTERVAL_ms) {
        updateControl();
        lastUpdateControl = millis();
    }

    refreshLeds();

    if (server.available()) {
        WiFiClient client = server.available();
        String requestStr = client.readStringUntil('\r');
        ControlEnum request = NUMS_CONTROL_STATES;
        if (requestStr.indexOf("LEDON1")) {
            requestStr = SPEED_UP;            
        } else if (requestStr.indexOf("LEDON2")) {
            requestStr = SPEED_DOWN;
        } else if (requestStr.indexOf("LEDON3")) {
            requestStr = CANCEL_CMD;
        } else if (requestStr.indexOf("LEDON4")) {
            requestStr = SWITCH_CMD;
        }
        if (request != NUMS_CONTROL_STATES) { 
            onControlChange(request);
        }
        client.print(response);
        client.flush();
    }
}