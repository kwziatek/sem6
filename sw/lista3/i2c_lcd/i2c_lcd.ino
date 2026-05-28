#include "LiquidCrystal_I2C.h"
#include "Wheels.h"

byte LCDAddress = 0x27;

LiquidCrystal_I2C lcd(LCDAddress, 16, 2);
Wheels w;

uint8_t arrowRight[8] =
{
    0b01000,
    0b01100,
    0b00110,
    0b11111,
    0b11111,
    0b00110,
    0b01100,
    0b01000
};

int argIn = 0;

void setup() {
  w.attach(7,8,5,12,11,10);

  Serial.begin(9600);
  Serial.setTimeout(200);

  lcd.init();
  lcd.backlight();

  lcd.createChar(0, arrowRight);
  lcd.clear();


  lcd.print("READY!");
  delay(2000);
  lcd.clear();
}

// void loop() {

//   uint8_t barPos = 0;
  
//   argIn = Serial.parseInt(SKIP_ALL);
//   if(argIn <= 0) return;

//   lcd.clear();
//   lcd.setCursor(barPos, 1);
//   lcd.write(0);

//   for(int val=0;val<=argIn; val++) {
//     lcd.setCursor(0,0);
//     lcd.print(val);
//     barPos = map(val, 0, argIn, 0, 16);
//     lcd.setCursor(barPos, 1);
//     lcd.print('=');
//     lcd.write(0);
//     delay(20);
//   }
// }

void loop() {

  // while(Serial.available()) {
  //   char c = Serial.read();
  //   lcd.print(c);
  //   delay(500);
  // }
  // delay(1000);
  // lcd.clear();
  lcd.print("16");
  delay(50);
  w.goForward(16);
  delay(1000);
  lcd.clear();
  delay(50);

  lcd.print(-16);
  delay(50);
  w.goBack(16);
  delay(1000);
  lcd.clear();
  delay(50);

}

void loop() {
  //as an input take distance 'x' (can be in cm)
  // lets set speed - some const
  // lets calculate the time for which the engine needs to run in order for car to move 'x' cm
  // in while loop until the calculated time passes calculate the distance that is left and print it
  // stop the engine

  int cm = 50;
  int speed = 150;
  int time = 30 * cm;
  
  lcd.clear();
  lcd.print(cm)
  w.forward();
  while()




}
