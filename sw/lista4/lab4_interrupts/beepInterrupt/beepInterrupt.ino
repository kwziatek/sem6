#include "Wheels.h"
#include "LiquidCrystal_I2C.h"

Wheels w;
LiquidCrystal_I2C lcd(LCDAddress, 16, 2);

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
  w.goBack(50);
  delay(100);
  w.goForward(50);
  delay(100);
}

