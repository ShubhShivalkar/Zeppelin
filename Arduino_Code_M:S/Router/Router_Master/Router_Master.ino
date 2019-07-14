#include <XBee.h>
#include <SoftwareSerial.h>
#include<Servo.h>

Servo ServoR;
Servo ServoL;
Servo ServoC;


XBee xbee = XBee();
XBeeResponse response = XBeeResponse();
ZBRxResponse rx = ZBRxResponse();

#define ssRX 2
#define ssTX 3
#define ssRX_1 4
#define ssTX_1 5
#define servo_R 
#define servo_L
#define servo_C

#define servoLpin 9
#define servoRpin 10
#define servoCpin 6

SoftwareSerial nss(ssRX, ssTX);
//SoftwareSerial slave(ssRX_1,ssTX_1);
int spd ;
int alt ;
int dir ;

void setup() { 
  // start serial
  ServoR.attach(servoRpin);
  ServoL.attach(servoLpin);
  ServoC.attach(servoCpin);
  
  Serial.begin(9600);
  nss.begin(9600);
  //slave.begin(9600);
  xbee.setSerial(nss);
  ServoC.write(90);
}

void loop() {
//int alt_r ,spd_r ,dir_r;

    xbee.readPacket();
    if (xbee.getResponse().isAvailable()) {
      
      if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE) {
        xbee.getResponse().getZBRxResponse(rx);
       spd = int(rx.getData()[4]);
       alt = int(rx.getData()[7]);
       dir = int(rx.getData()[10]);
       Serial.write(spd);
       Serial.write(alt);
       Serial.write(dir);
       
      }
      Serial.println("");
       
      
    }
    else if (xbee.getResponse().isError()) {
      Serial.print("************************************* error code:");
      Serial.println(xbee.getResponse().getErrorCode(),DEC);
    }
}
/*
if(spd!=spd_r){
send_to_slave();
spd_r = spd;
}
if(alt!=alt_r){
  altitude_c(alt);
}
if(dir!=dir_r){

}
}
void send_to_slave(){
slave.print(spd);
}

void direction_c(int data){
  switch(data){
  case 0: 
  ServoC.write(90);
  break;
  case 1:
  ServoC.write(105);
  case 2:
  ServoC.write(85);
  
  }
}
void altitude_c(int data){
  switch(data){
  case 0: 
  ServoR.write(90);
  ServoL.write(90);
  break;
  case 1:
  ServoR.write(105);
  ServoL.write(85);
  break;
  case 2:
  ServoR.write(85);
  ServoL.write(105);
  break;
}
}
*/
