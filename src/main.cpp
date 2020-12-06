#include <Arduino.h>
#include <ESP8266WiFi.h>

#include "leds.h"

const char WiFiPassword[] = "12345678";
const char AP_NameChar[] = "cruisecontrol";
int analogPin = A0;
int raw = 0;
float Vin = 3.28;
float Vout = 0;
float R1 = 2000;
float R2 = 0;
float RX = 100000;
float buffer = 0;
float down = 515;     //resistance of cruise controller when up speed in ohm
float up = 142;       //resistance of cruise controller when down speed in ohm
float cancel = 1379;  //resistance of cruise controller when cancel in ohm
int infelicity = 20;  //infelicity in cruise control controller
unsigned long previousMillis = 0;  
const long interval = 200;           // interval of delay read analog


WiFiServer server(80);

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
    ////boolean conn = WiFi.softAP(AP_NameChar, WiFiPassword);
    WiFi.softAP(AP_NameChar, WiFiPassword); //disabled by leo
    Serial.println("I'm WIFI-ing");
}

void initLed() {
    for (uint8_t i = 0; i < NUM_LEDS; i++) {
        leds[i].init();
    }
}

void setup() {
    initSerial();
    initWiFi();
    server.begin();//added by leo
    initLed();
    delay (500);//added by leo
    

}

void refreshLeds() {
    for (uint8_t i = 0; i < NUM_LEDS; i++) {
        leds[i].loop();
    }
}

void handleControl() {
    // analog read resistance
    uint16_t raw = analogRead(analogPin);
    if (raw) {
        buffer = raw * Vin;
        Vout = (buffer) / 1024.0;
        buffer = (Vin / Vout) - 1;
        R2 = R1 * buffer;
        if (R2 <= (down + (down / 100 * infelicity)) && R2 >= (down - (down / 100 * infelicity))) {
            leds[LED_2].setState(true);
        }
        if (R2 <= (up + (up / 100 * infelicity)) && R2 >= (up - (up / 100 * infelicity))) {
            leds[LED_1].setState(true);
        }
        if (R2 <= (cancel + (cancel / 100 * infelicity)) && R2 >= (cancel - (cancel / 100 * infelicity))) {
            leds[LED_3].setState(true);
        }
        if (R2 <= 0.5) {
            leds[LED_4].setState(true);
        }
         //delay(300); //added by leo // Without it - no wifi
    }
}


void loop() {
    WiFiClient client = server.available();
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;
    handleControl();
    }

    if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;
    refreshLeds();
    }

    if (client.available()) {
        Serial.println("Server availible");
        /*
        WiFiClient client = server.available();  //disabled by leo
        Serial.println("server wifi avail");//disabled by leo
        */
        String request = client.readStringUntil('\r');
        if (request.indexOf("LEDON1") > 0) {
            leds[LED_1].setState(true);
        } else if (request.indexOf("LEDON2") > 0) {
            leds[LED_2].setState(true);
        } else if (request.indexOf("LEDON3") > 0) {
            leds[LED_3].setState(true);
        } else if (request.indexOf("LEDON4") > 0) {
            leds[LED_4].setState(true);
        }
        client.flush();
        client.print(response);
    }
}