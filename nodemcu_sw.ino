/*
 * connect to the ESP8266 AP then
 * use web broswer to go to 192.168.4.1
 */
 
 
#include <ESP8266WiFi.h>
const char WiFiPassword[] = "12345678";
const char AP_NameChar[] = "cruisecontrol" ;
int analogPin = A0;
int raw = 0;
float Vin = 3.28;
float Vout = 0;
float R1 = 2000;
float R2 = 0;
float RX = 100000;
float buffer = 0;
float down = 515; //resistance of cruise controller when up speed in ohm
float up = 142; //resistance of cruise controller when down speed in ohm
float cancel = 1379; //resistance of cruise controller when cancel in ohm
int infelicity = 20; //infelicity in cruise control controller

WiFiServer server(80);
String header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
String html_0 = "<!DOCTYPE html><html><head><title>LED Control</title></head><body>";
String html_1 = "<h2>Speed Up</h2><form id='F1' action='LEDON1'><input class='button' type='submit' value='LED ON' ></form>";
String html_2 = "<h2>Speed Down</h2><form id='F1' action='LEDON2'><input class='button' type='submit' value='LED ON' ></form>";
String html_3 = "<h2>Cancel</h2><form id='F1' action='LEDON3'><input class='button' type='submit' value='LED ON' ></form>";
String html_4 = "<h2>Switch On - Off</h2><form id='F1' action='LEDON4'><input class='button' type='submit' value='LED ON' ></form>";
String html_5 = "</body></html>";
String request = "";
int LED1_Pin = D5;
int LED2_Pin = D6;
int LED3_Pin = D7;
int LED4_Pin = D8;
 
void setup() 
{
    pinMode(LED1_Pin, OUTPUT); 
    pinMode(LED2_Pin, OUTPUT); 
    pinMode(LED3_Pin, OUTPUT); 
    pinMode(LED4_Pin, OUTPUT); 
    boolean conn = WiFi.softAP(AP_NameChar, WiFiPassword);
    Serial.begin(9600);
    server.begin();
} // void setup()
 
 
void loop() 
{

    // Check if a client has connected    
    WiFiClient client = server.available();
    ////if (!client)  {  return;  }
    if (!client)  {
      
    // analog read resistance
    raw = analogRead(analogPin);
    if(raw){
    buffer = raw * Vin;
    Vout = (buffer)/1024.0;
    buffer = (Vin/Vout) - 1;
    R2= R1 * buffer;
    //Check R2 and send Value
      if ( R2 <= (down + (down/100 * infelicity)) && R2 >= (down - (down/100 * infelicity))) { Serial.println("Value1: Down");digitalWrite(LED2_Pin, HIGH); delay(150); digitalWrite(LED2_Pin, LOW);  }
      if ( R2 <= (up + (up/100 * infelicity)) && R2 >= (up - (up/100 * infelicity))) { Serial.println("Value1: Up");digitalWrite(LED1_Pin, HIGH); delay(150); digitalWrite(LED1_Pin, LOW); } 
      if ( R2 <= (cancel + (cancel/100 * infelicity)) && R2 >= (cancel - (cancel/100 * infelicity))) { Serial.println("Value1: Cancel");digitalWrite(LED3_Pin, HIGH); delay(150); digitalWrite(LED3_Pin, LOW); } 
      if ( R2 <= 0.5) { Serial.println("Value1: OnOff");digitalWrite(LED4_Pin, HIGH); delay(150); digitalWrite(LED4_Pin, LOW); } 
      delay(300);
    //
      return;  }
    }
    
    // Read the first line of the request
    request = client.readStringUntil('\r');
 
    if       ( request.indexOf("LEDON1") > 0 )  { digitalWrite(LED1_Pin, HIGH); delay(250); digitalWrite(LED1_Pin, LOW);  }
    if       ( request.indexOf("LEDON2") > 0 )  { digitalWrite(LED2_Pin, HIGH); delay(250); digitalWrite(LED2_Pin, LOW);  }
    if       ( request.indexOf("LEDON3") > 0 )  { digitalWrite(LED3_Pin, HIGH); delay(250); digitalWrite(LED3_Pin, LOW);  }
    if       ( request.indexOf("LEDON4") > 0 )  { digitalWrite(LED4_Pin, HIGH); delay(250); digitalWrite(LED4_Pin, LOW);  }
 
    client.flush();
 
    client.print( header );
    client.print( html_1 );
    client.print( html_2 );
    client.print( html_3 );
    client.print( html_4);
    client.print( html_5);
 
    delay(5);
  // The client will actually be disconnected when the function returns and 'client' object is detroyed
} // void loop()