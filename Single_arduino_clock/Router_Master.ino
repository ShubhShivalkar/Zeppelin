#include <XBee.h>
#include <SoftwareSerial.h>
#include<Servo.h>

Servo ServoR;
Servo ServoL;
Servo ServoC;
Servo BLDC;

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

#define bldcpin 9
#define servoLpin 11
#define servoRpin 10
#define servoCpin 6

SoftwareSerial nss(ssRX, ssTX);
//SoftwareSerial slave(ssRX_1,ssTX_1);
int flag = 0;
int swi;
int spd ;
int alt ;
int dir ;
int pos = 0;
int updateInterval = 25;
unsigned long lastUpdate;
void Update(unsigned long currentMillis)
{
  if ((currentMillis - lastUpdate) > updateInterval) // time to update
  {
    lastUpdate = millis();
    BLDC.write(pos);
    if (pos != 0 )
      digitalWrite(13, HIGH);
    else
      digitalWrite(13, LOW);
  }
}

void setup() {
  // start serial
  pinMode(LED_BUILTIN, OUTPUT);
  OCR0A = 0xAA;
  TIMSK0 |= _BV(OCIE0A);


  ServoR.attach(servoRpin);
  ServoL.attach(servoLpin);
  ServoC.attach(servoCpin);
  BLDC.attach(bldcpin);
  Serial.begin(9600);
  nss.begin(9600);
  //slave.begin(9600);
  xbee.setSerial(nss);
  ServoC.write(90);
  ServoR.write(90);
  ServoL.write(90);
}

SIGNAL(TIMER0_COMPA_vect)
{
  unsigned long currentMillis = millis();
  Update(currentMillis);
}



void loop() {

  xbee.readPacket();
  if (xbee.getResponse().isAvailable()) {

    if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE) {
      xbee.getResponse().getZBRxResponse(rx);
      swi = int(rx.getData()[1]);
      spd = int(rx.getData()[4]);
      alt = int(rx.getData()[7]);
      dir = int(rx.getData()[10]);
      flag = 0;

      Serial.println("******************************");
      Serial.write(swi);
      Serial.write(spd);
      Serial.write(alt);
      Serial.write(dir);

    }
    //Serial.println("");


  }
  else if (xbee.getResponse().isError()) {
    Serial.print("************************************* error code:");
    Serial.println(xbee.getResponse().getErrorCode(), DEC);
  }

  if (flag == 0 && swi == '1') {
    Serial.println("");
    altitude_c(alt);
    direction_c(dir);
    delay(10);
    flag = 1;
  } if (swi != '0')
  {
    if (spd == '0')

      pos = 0;
    else
      pos = 45;
  }
  else if (swi == '0')
    pos = 0;


}


void direction_c(int data) {
  switch (data) {
    case '0':
      ServoC.write(90);
      Serial.println("dirCase0");
      break;
    case '1':
      ServoC.write(105);
      Serial.println("dirCase1");
      break;
    case '2':
      ServoC.write(85);
      Serial.println("dirCase2");
      break;
  }
}
void altitude_c(int data) {
  switch (data) {
    case '0':
      ServoR.write(90);
      ServoL.write(90);
      Serial.println("altcase0");
      break;
    case '1':
      ServoR.write(105);
      ServoL.write(85);
      Serial.println("altcase1");

      break;
    case '2':
      ServoR.write(85);
      ServoL.write(105);
      Serial.println("altcase2");
      break;
  }
}
