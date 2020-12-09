#include <Arduino.h>
#include <ESP8266WiFi.h>

#include "consts.h"
#include "leds.h"
#include "telnet.h"
#include "utils.h"

const char WiFiPassword[] = "12345678";
const char AP_NameChar[] = "cruisecontrol";
int analogPin = A0;
String readString;
String tri = "";

static const char* response =
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
uint32_t lastControlStateTime{0};
Telnet* telnet;
WiFiClient* telnet_client{nullptr};

LedItem leds[NUM_LEDS] = {
    {LED_SPEED_UP_PIN},
    {LED_SPEED_DOWN_PIN},
    {LED_CANCEL_PIN, true},
    {LED_SWITCH_PIN},
};

void initSerial() {
    Serial.begin(115200);
    //Serial.println();
}

void initWiFi() {
    WiFi.softAP(AP_NameChar, WiFiPassword);
    delay(1000);
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

static const int INFELICITY = 10;     //infelicity in cruise control controller
static const uint16_t down = 515;     //resistance of cruise controller when up speed in ohm
static const uint16_t up = 142;       //resistance of cruise controller when down speed in ohm
static const uint16_t cancel = 1379;  //resistance of cruise controller when cancel in ohm
static const float V_IN = 3.28;
static const float R1 = 2000;
static const float RX = 100000;

void onControlChange(ControlEnum newState) {
    if (lastControlState != newState) {
        if (TimePassedSince(lastControlStateTime) < CONTROL_IGNORE_INTERVAL_ms) {
            return;
        }
    }
    lastControlStateTime = millis();

    if (lastControlState != newState) {
        Serial.println(newState);
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
    ControlEnum request = NUMS_CONTROL_STATES;
    if (raw > 100) {
        float tmp = raw * V_IN;
        float Vout = tmp / 1024.0;
        tmp = (V_IN / Vout) - 1;
        float R2 = R1 * tmp;
        if (R2 <= (down + (down / 100 * INFELICITY)) && R2 >= (down - (down / 100 * INFELICITY))) {
            request = SPEED_UP;
        } else if (R2 <= (up + (up / 100 * INFELICITY)) && R2 >= (up - (up / 100 * INFELICITY))) {
            request = SPEED_DOWN;
        } else if (R2 <= (cancel + (cancel / 100 * INFELICITY)) && R2 >= (cancel - (cancel / 100 * INFELICITY))) {
            request = CANCEL_CMD;
        } else if (R2 <= 0.5) {
            request = SWITCH_CMD;
        };
    }
    onControlChange(request);
}

bool telnetStarted{false};
uint32_t lastUpdateControl{0};

void handleRequestStr(const String& str) {
    if (str.isEmpty()) {
        return;
    }
    Serial.println("\"" + str + "\"");
    ControlEnum request = NUMS_CONTROL_STATES;

    if (str.equalsIgnoreCase("UP")) {
        request = SPEED_UP;
    } else if (str.equalsIgnoreCase("DOWN")) {
        request = SPEED_DOWN;
    } else if (str.equalsIgnoreCase("CANCEL")) {
        request = CANCEL_CMD;
    } else if (str.equalsIgnoreCase("ONOFF")) {
        request = SWITCH_CMD;
    }
    if (request != NUMS_CONTROL_STATES) {
        onControlChange(request);
        //telnet->sendData(response);
    }
}

void handleTelnet() {
    if (telnet_client) {
              if ( tri == "tripple") {
                    telnet->sendData("#ARRIVES#\r\n");
                    tri = "";  }

        if (telnet_client->available()) {

            while (telnet_client->available()) {
              //delay(3);  

              char chr = telnet_client->read();
              readString += chr; 
            }
            readString.trim();
            if (readString.length() >0)
            {
                if (readString == "UP"){
                    Serial.println("\r\nswitching UP\r\n");
                    handleRequestStr("UP");
                    telnet->sendData("UPOK");}
                if (readString == "DOWN"){
                    Serial.println("\r\nswitching DOWN\r\n");
                    handleRequestStr("DOWN");
                    telnet->sendData("DOWNOK");}
                if (readString == "CANCEL"){
                    Serial.println("\r\nswitching CANCEL\r\n");
                    handleRequestStr("CANCEL");
                    telnet->sendData("CANCELOK");}
                if (readString == "ONOFF"){
                    Serial.println("\r\nswitching ONOFF\r\n");
                    handleRequestStr("ONOFF");
                    telnet->sendData("ONOFFOK");}

                readString="";
            }
        };
    };
};

void loop() {
    if (!telnetStarted) {
        if (!telnet) {
            telnet = new Telnet(23);
            telnet->onInit();
            telnet->setEventHandler([](TelnetEventType event, WiFiClient* client) {
                switch (event) {
                    case CLIENT_CONNECTED:
                        telnet_client = client;
                        client->flush();
                        client->println("220 Sendmail ESMTP ready\n\n");
                        break;
                    case CLIENT_DISCONNECTED:
                        telnet_client = nullptr;
                        Serial.println("disconnected");
                        break;
                    default:
                        break;
                }
            });
        }
        telnetStarted = telnet->onStart();
        Serial.printf("telnet: %s/n", telnetStarted ? "started" : "failed");
    }

    if (telnetStarted) {
        telnet->onLoop();
        handleTelnet();
    }

    if (TimePassedSince(lastUpdateControl) > CONTROL_INTERVAL_ms) {
        updateControl();
        lastUpdateControl = millis();
    }

    refreshLeds();
}
