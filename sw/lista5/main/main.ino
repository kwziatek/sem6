#include <Servo.h>
#include "LiquidCrystal_I2C.h"
#include "Wheels.h"
#include "PinChangeInterrupt.h"

byte LCDAddress = 0x27;

LiquidCrystal_I2C lcd(LCDAddress, 16, 2);
Wheels w;

//piny silników
#define EnA 5
#define In1 7
#define In2 8
#define In3 2
#define In4 4
#define EnB 3

// piny dla sonaru (HC-SR04)
#define TRIG A4
#define ECHO A5

// piny dla interrupt
#define INTINPUT0 A0
#define INTINPUT1 A1

// pin kontroli serwo (musi być PWM)
#define SERVO 11

Servo serwo;

void setup() {
  w.attach(In4, In3, EnB, In2, In1, EnA);
  pinMode(TRIG, OUTPUT);    // TRIG startuje sonar
  pinMode(ECHO, INPUT);     // ECHO odbiera powracający impuls

  Serial.begin(9600);

  serwo.attach(SERVO);

/* rozejrzyj się w zakresie od 0 stopni (patrz na jedną burtę)
 *  do 180 stopni (patrz na prawą burtę). Wydrukuj na konsoli
 *  kierunek patrzenia i najbliżej widziany obiekt (pojedynczy pomiar)
 */
  for(byte angle = 0; angle < 180; angle+= 20) {
    lookAndTellDistance(angle);
    delay(500);
  }
  
/* patrz przed siebie */
  serwo.write(90);

}

void loop() 
{ 
  w.setSpeed(200);
  w.forward();
  delay(1000);
  w.stop();
  delay(500);
  
}

void lookAndTellDistance(byte angle) {
  
  unsigned long tot;      // czas powrotu (time-of-travel)
  unsigned int distance;

  Serial.print("Patrzę w kącie ");
  Serial.print(angle);
  serwo.write(angle);
  
/* uruchamia sonar (puls 10 ms na `TRIGGER')
 * oczekuje na powrotny sygnał i aktualizuje
 */
  digitalWrite(TRIG, HIGH);
  delay(10);
  digitalWrite(TRIG, LOW);
  tot = pulseIn(ECHO, HIGH);

/* prędkość dźwięku = 340m/s => 1 cm w 29 mikrosekund
 * droga tam i z powrotem, zatem:
 */
  distance = tot/58;

  Serial.print(": widzę coś w odległości ");
  Serial.println(distance);
}
