#include<SoftwareSerial.h>
#define rx 2
#define tx 3
SoftwareSerial ser(rx,tx);
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
ser.begin(9600);


}

void loop() {
  // put your main code here, to run repeatedly:
if(ser.available()){
unsigned int data = ser.read();
int x = (data - 130)/8;
Serial.println(x);
delay(500);
}
}
