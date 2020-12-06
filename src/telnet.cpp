#include "telnet.h"


bool Telnet::onInit() {
    server_ = new WiFiServer(port_);           
    return true;
}

void Telnet::onEnd(){
    delete server_;
}

bool Telnet::onStart() {
    server_->begin();
    server_->setNoDelay(true);
    return server_->status() != CLOSED;
}

void Telnet::onStop() {
    if (hasClient())
        client_.stop();
    server_->stop();
}

bool Telnet::hasClient() { return client_.connected(); }

void Telnet::sendData(const String& data) {
    if (hasClient())
        client_.write(data.c_str());
}

void Telnet::setEventHandler(TelnetEventHandler h) { eventHandler_ = h; }

void Telnet::onLoop() {
    if (server_->hasClient()) {
        if (!client_) {
            client_ = server_->available();
        } else {
            if (!client_.connected()) {
                server_->stop();
                client_ = server_->available();
            } else {
                WiFiClient rejected;
                rejected = server_->available();
                rejected.write("wtf");
                rejected.stop();
            }
        }
    }

    if (lastConnected_ != hasClient()) {
        lastConnected_ = hasClient();
        if (lastConnected_) {
            onConnect();
        } else {
            onDisconnect();
        }
    }    
}

void Telnet::onConnect() {    
    if (eventHandler_)
        eventHandler_(CLIENT_CONNECTED, &client_);
}

void Telnet::onDisconnect() {
    if (eventHandler_)
        eventHandler_(CLIENT_DISCONNECTED, nullptr);
}
