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

uint8_t arrowUp[8] = 
{
  0b00100,
  0b01110,
  0b11111,
  0b00100,
  0b00100,
  0b00100,
  0b00100,
  0b00000
};

uint8_t arrowDown[8] = 
{
  0b00100,
  0b00100,
  0b00100,
  0b00100,
  0b11111,
  0b01110,
  0b00100,
  0b00000
};

uint8_t zero[8] = 
{
  0b11111,
  0b10001,
  0b10001,
  0b10001,
  0b10001,
  0b10001,
  0b11111,
  0b00000
};

int argIn = 0;

void setup() {
  w.attach(7,8,5,12,11,10);

  Serial.begin(9600);
  Serial.setTimeout(200);

  lcd.init();
  lcd.backlight();

  lcd.createChar(1, arrowUp);
  lcd.createChar(2, arrowDown);
  lcd.createChar(3, zero);


  lcd.print("READY!");
  delay(2000);
  lcd.clear();
}


void loop() {
  //as an input take distance 'x' (can be in cm)
  // lets set speed - some const
  // lets calculate the time for which the engine needs to run in order for car to move 'x' cm
  // in while loop until the calculated time passes calculate the distance that is left and print it
  // stop the engine

  if (Serial.available() > 0) {
    // Execute only if data has been sent from the Serial Monitor
    int cm = Serial.parseInt();

    // Procees only if the input is a valid distance
    if (cm > 0) {
      int speed = 150;
      unsigned long timeToRun = 30UL * cm;
      
      // go forward
      loopGo(speed, cm, timeToRun, true);
      // go backward
      loopGo(speed, cm, timeToRun, false);
    }
  }
}

void loopGo(int speed, int cm, int timeToRun, bool direction) {
  // set up
  lcd.clear();

  // set 1st line of dispaly unit
  lcd.setCursor(0,0);
  lcd.print("Dist left: ");
  lcd.print(cm);
  lcd.print(" cm  ");
  
  // set direction and 2nd line of display unit
  if (direction) {
    w.forward();
    lcd.setCursor(1,0);
    lcd.print("R:F");
    lcd.setCursor(1,7);
    // print arrow up
    lcd.write(1); // 1st memory slot (arrowUp)
    lcd.setCursor(1,13);
    lcd.print("L:F");
  } else {
    w.back();
    lcd.setCursor(1,0);
    lcd.print("R:B");
    lcd.setCursor(1,7);
    // print arrow down
    lcd.write(2); // 2nd memory slot (arrowDown)
    lcd.setCursor(1,13);
    lcd.print("L:B");
  }

  delay(500);
  
  // set speed -> start movement
  w.setSpeed(speed);
  
  // 1. Mark the starting time
  unsigned long start_time = millis();

  // 2. Loop until the current time reaches the end time
  while (millis() - start_time < timeToRun) {

    // calculate how much time has passed
    unsigned long elapsedTime = millis() - start_time;

    // calculate traveled distance basing on 30ms per 1cm
    int distTraveled = elapsedTime/30;
    int distLeft = cm - distTraveled;

    // prevent the screen from displaying negative numbers due to tiny timing overshoot
    if(distLeft < 0) {
      distLeft = 0;
    }
    lcd.setCursor(0,0);
    lcd.print("Dist left: ");
    lcd.print(distLeft);
    lcd.print(" cm  ");

    // prevent LCD from flickering because of too fast updates
    delay(50);
  }

  // stop the engine when time is up
  w.stop();

  // display left distance after completing movement
  lcd.setCursor(0,0);
  lcd.print("Dist left: 0 cm");
  
  // update the 2nd row
  lcd.setCursor(1,0);
  lcd.print("R:S");
  lcd.setCursor(1,7);
  // print zero
  lcd.write(3); // 3st memory slot (zero)
  lcd.setCursor(1,13);
  lcd.print("L:S");

  // wait in place for 3sec
  delay(3000);
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
