#pragma once

#include <ESP8266WiFi.h>

enum TelnetEventType {
    CLIENT_CONNECTED,
    CLIENT_DISCONNECTED
};

typedef std::function<void(TelnetEventType, WiFiClient*)> TelnetEventHandler;

class Telnet {
   public:
    Telnet(uint16_t port): lastConnected_(false), port_(port){};
   public:
    void setEventHandler(TelnetEventHandler);
    void sendData(const String&);
    bool hasClient();
    
    bool onInit() ;
    void onEnd() ;
    bool onStart();
    void onStop() ;
    void onLoop();

   private:
    void onConnect();
    void onDisconnect();
    void onData();
    void onOpen();
    void onClose();

   private:
    TelnetEventHandler eventHandler_;
    bool lastConnected_;
    uint16_t port_;
    WiFiClient client_;
    WiFiServer* server_;
};
