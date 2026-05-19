#include "Wheels.h"



Wheels w;
volatile char cmd;

void setup() {
  // put your setup code here, to run once:
  w.attach(7,8,5,12,11,10);
  
  Serial.begin(9600);
  Serial.println("Forward: WAD");
  Serial.println("Back: ZXC");
  Serial.println("Stop: S");

  for(int i = 1; i <= 1; i++) {
    w.goForward(16);
    w.stop();
    delay(1000);
    w.goBack(16);
    w.stop();
    delay(1000);
  }

}

void loop() {
  while(Serial.available())
  {
    cmd = Serial.read();
    switch(cmd)
    {
      case 'w': w.forward(); break;
      case 'x': w.back(); break;
      case 'a': w.forwardLeft(); break;
      case 'd': w.forwardRight(); break;
      case 'z': w.backLeft(); break;
      case 'c': w.backRight(); break;
      case 's': w.stop(); break;
      case '1': w.setSpeedLeft(200); break;
      case '2': w.setSpeedLeft(400); break;
      case '9': w.setSpeedRight(200); break;
      case '0': w.setSpeedRight(400); break;
      case '5': w.setSpeed(1000); Serial.print(5); break;
    }
  }
}
