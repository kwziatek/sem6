#include <Servo.h>
#include <LiquidCrystal_I2C.h>
#include "Wheels.h"

// --- DEFINICJE PINÓW I OBIEKTÓW ---
#define TRIG A4
#define ECHO A5
#define SERVO_PIN 11

#define EnA 5
#define In1 7
#define In2 8
#define In3 2
#define In4 4 
#define EnB 3

Wheels w;
Servo serwo;
LiquidCrystal_I2C lcd(0x27, 16, 2);

// --- ZMIENNE DO ZARZĄDZANIA CZASEM I STANEM ---
unsigned long lastSonarCheck = 0;
const int SONAR_INTERVAL = 60; // Sprawdzaj co 60 ms
const int SAFE_DISTANCE = 20;  // Zatrzymujemy się 20 cm przed ścianą

void setup() {
    Serial.begin(9600);
    lcd.init();
    lcd.backlight();
    
    pinMode(TRIG, OUTPUT);
    pinMode(ECHO, INPUT);
    serwo.attach(SERVO_PIN);
    serwo.write(90); // Patrz prosto
    
    w.attach(In4, In3, EnB, In2, In1, EnA); // Twoje piny
    w.attachEncoders(A0, A1);
    
    delay(2000); // Daj chwilę na ustabilizowanie przed startem
}

void loop() {
    // 1. Sprawdzanie zegarka (nieblokujące)
    if (millis() - lastSonarCheck >= SONAR_INTERVAL) {
        lastSonarCheck = millis();
        
        // 2. Pobierz dystans (patrząc prosto)
        int distance = lookAndTellDistance(90);
        updateLCD(90, distance); // Zaktualizuj ekran
        
        // 3. Logika unikania przeszkód
        if (distance > 0 && distance < SAFE_DISTANCE) {
            w.stop();
            avoidObstacle(); // Wywołanie logiki omijania
        } else {
            // Bezpiecznie, jedziemy prosto
            w.setSpeed(150);
            w.forward();
        }
    }
}

// Funkcja decyzyjna (wywoływana tylko, gdy napotkamy przeszkodę)
void avoidObstacle() {
    lcd.clear();
    lcd.print("Przeszkoda!");
    
    // Rozejrzyj się
    int distRight = lookAndTellDistance(0);   // Prawo
    delay(300);                               // Tutaj delay jest OK, bo auto stoi
    int distLeft = lookAndTellDistance(180);  // Lewo
    delay(300);
    serwo.write(90);                          // Wróć na wprost
    delay(300);
    
    // Podejmij decyzję
    if (distRight > distLeft) {
        lcd.setCursor(0, 1);
        lcd.print("Skret w prawo");
        w.turnRight(90); // Z użyciem Twoich enkoderów!
    } else {
        lcd.setCursor(0, 1);
        lcd.print("Skret w lewo");
        w.turnLeft(90);
    }
    lcd.clear();
}

int lookAndTellDistance(byte angle) {
    static byte lastAngle = 90; // Pamięta poprzedni kąt między wywołaniami
    
    serwo.write(angle);
    
    // Zrób pauzę na ruch serwa TYLKO, jeśli kąt się zmienił
    if (angle != lastAngle) {
        delay(300); 
        lastAngle = angle;
    }
    
    digitalWrite(TRIG, HIGH);
    delayMicroseconds(10); // impuls 10 mikrosekund
    digitalWrite(TRIG, LOW);
    
    // Zabezpieczenie przed zawieszeniem: timeout 30000 mikrosekund (ok. 5 metrów)
    unsigned long tot = pulseIn(ECHO, HIGH, 30000); 
    
    if (tot == 0) return 999; // Brak echa (bardzo daleko)
    return tot / 58;
}

void updateLCD(int angle, int dist) {
    lcd.setCursor(0, 0);
    lcd.print("Kat: "); lcd.print(angle); lcd.print(" st  ");
    lcd.setCursor(0, 1);
    lcd.print("Dyst: "); lcd.print(dist); lcd.print(" cm  ");
}