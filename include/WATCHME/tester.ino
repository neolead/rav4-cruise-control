int analogPin = A0;
int raw = 0;
float Vin = 3.28;
float Vout = 0;
float R1 = 2000;
float R2 = 0;
float RX = 100000;
float buffer = 0;
float down = 515;
//float up = 142;
float up = 107;
float cancel = 1379;
int infelicity = 20;


void setup(){
Serial.begin(9600);
}

void loop(){
  raw = analogRead(analogPin);
  if(raw){
    buffer = raw * Vin;
    Vout = (buffer)/1024.0;
    buffer = (Vin/Vout) - 1;
    R2= R1 * buffer;
//Check R2 and send Value
      if ( R2 <= (down + (down/100 * infelicity)) && R2 >= (down - (down/100 * infelicity))) { Serial.println("Value: Down");  }
      if ( R2 <= (up + (up/100 * infelicity)) && R2 >= (up - (up/100 * infelicity))) { Serial.println("Value: Up"); } 
      if ( R2 <= (cancel + (cancel/100 * infelicity)) && R2 >= (cancel - (cancel/100 * infelicity))) { Serial.println("Value: Cancel"); } 
      if ( R2 <= 0.5) { Serial.println("Value: OnOff"); } 
      Serial.println(R2);
      delay(150);
  }
}