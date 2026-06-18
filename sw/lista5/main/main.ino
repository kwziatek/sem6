#include <Servo.h> //Biblioteka odpowiedzialna za serwa
#include "LiquidCrystal_I2C.h"
#include "Wheels.h"

byte LCDAddress = 0x27;

LiquidCrystal_I2C lcd(LCDAddress, 16, 2);
Wheels w;

 
Servo serwomechanizm;  //Tworzymy obiekt, dzięki któremu możemy odwołać się do serwa 
int pozycja = 90; //Aktualna pozycja serwa 0-180
int zmiana = 10; //Co ile ma się zmieniać pozycja serwa?
 
void setup() 
{ 
  serwomechanizm.attach(11);  //Serwomechanizm podłączony do pinu 9
} 
 
// void loop() 
// {  
//   if (pozycja < 180) { //Jeśli pozycja mieści się w zakresie
//     serwomechanizm.write(pozycja); //Wykonaj ruch
//   } else { //Jeśli nie, to powrót na początek
//     pozycja = 0;
//   }    
  
//   pozycja = pozycja + zmiana; //Zwiększenie aktualnej pozycji serwa
//   delay(500); //Opóźnienie dla lepszego efektu                        
// }

void loop() 
{
  pozycja = pozycja + zmiana;
  serwomechanizm.write(pozycja);
  delay(300);
  pozycja = pozycja - zmiana;
  serwomechanizm.write(pozycja);
  delay(300);
  pozycja = pozycja - zmiana;
  serwomechanizm.write(pozycja);
  delay(300);
  pozycja = pozycja + zmiana;
  serwomechanizm.write(pozycja);
  delay(300);
}