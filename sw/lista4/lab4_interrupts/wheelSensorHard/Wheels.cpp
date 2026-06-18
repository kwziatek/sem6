#include <Arduino.h>
#include "Wheels.h"
#include <PinChangeInterrupt.h> // Dodajemy bibliotekę obsługującą przerwania PCI

#define SET_MOVEMENT(side,f,b) digitalWrite( side[0], f);\
                               digitalWrite( side[1], b)

// Zmienne globalne dla liczników. MUSZĄ być volatile, by były poprawnie obsługiwane w przerwań.
volatile unsigned long cntLeft = 0;  // Licznik lewego koła
volatile unsigned long cntRight = 0; // Licznik prawego koła

// Funkcje obsługi przerwań (tzw. Callbacks)
void countLeftISR() {
    cntLeft++;
}

void countRightISR() {
    cntRight++;
}

// Stałe do kalibracji (musisz dostosować je do swojego robota)
const float IMPULSES_PER_CM = 2.0;    // Ile impulsów to 1 cm? Zmierz to doświadczalnie.
const float IMPULSES_PER_DEGREE = 0.5; // Ile impulsów to 1 stopień obrotu autka?

Wheels::Wheels() 
{ }

// Nowa funkcja inicjująca enkodery
void Wheels::attachEncoders(int pinL, int pinR)
{
    pinMode(pinL, INPUT_PULLUP);
    pinMode(pinR, INPUT_PULLUP);
    
    // Podpinamy przerwania pod piny używając biblioteki
    attachPCINT(digitalPinToPCINT(pinL), countLeftISR, CHANGE);
    attachPCINT(digitalPinToPCINT(pinR), countRightISR, CHANGE);
}

void Wheels::attachRight(int pF, int pB, int pS)
{
    pinMode(pF, OUTPUT);
    pinMode(pB, OUTPUT);
    pinMode(pS, OUTPUT);
    this->pinsRight[0] = pF;
    this->pinsRight[1] = pB;
    this->pinsRight[2] = pS;
}

void Wheels::attachLeft(int pF, int pB, int pS)
{
    pinMode(pF, OUTPUT);
    pinMode(pB, OUTPUT);
    pinMode(pS, OUTPUT);
    this->pinsLeft[0] = pF;
    this->pinsLeft[1] = pB;
    this->pinsLeft[2] = pS;
}

void Wheels::setSpeedRight(uint8_t s)
{
    analogWrite(this->pinsRight[2], s);
}

void Wheels::setSpeedLeft(uint8_t s)
{
    analogWrite(this->pinsLeft[2], s);
}

void Wheels::setSpeed(uint8_t s)
{
    setSpeedLeft(s);
    setSpeedRight(s);
    this->currentSpeed = s;
}

void Wheels::attach(int pRF, int pRB, int pRS, int pLF, int pLB, int pLS)
{
    this->attachRight(pRF, pRB, pRS);
    this->attachLeft(pLF, pLB, pLS);
    pinMode(13, OUTPUT); //Initialize LED pin
}

void Wheels::forwardLeft() { SET_MOVEMENT(pinsLeft, HIGH, LOW); }
void Wheels::forwardRight() { SET_MOVEMENT(pinsRight, HIGH, LOW); }
void Wheels::backLeft() { SET_MOVEMENT(pinsLeft, LOW, HIGH); }
void Wheels::backRight() { SET_MOVEMENT(pinsRight, LOW, HIGH); }
void Wheels::stopLeft() { SET_MOVEMENT(pinsLeft, LOW, LOW); }
void Wheels::stopRight() { SET_MOVEMENT(pinsRight, LOW, LOW); }

void Wheels::forward()
{
    this->forwardLeft();
    this->forwardRight();
    this->isMovingBack = false;
    digitalWrite(13, LOW);
}

void Wheels::back()
{
    this->backLeft();
    this->backRight();
    this->isMovingBack = true;
}

void Wheels::stop()
{
    this->stopLeft();
    this->stopRight();
    this->isMovingBack = false;
    digitalWrite(13, LOW);
}

// ---------------------------------------------------------
// NOWE FUNKCJE RUCHU OPARTE NA ENKODERACH
// ---------------------------------------------------------

void Wheels::goForward(int cm)
{
    unsigned long targetImpulses = cm * IMPULSES_PER_CM;
    
    cntLeft = 0;
    cntRight = 0;

    this->setSpeed(150); // Prędkość bazowa
    this->forward();

    // Pętla działa, dopóki oba koła nie osiągną celu
    while (cntLeft < targetImpulses || cntRight < targetImpulses) {
        
        // Prosta korekta jazdy na wprost:
        if (cntLeft > cntRight) {
            this->setSpeedLeft(130);   // Lewe jedzie za szybko - zwalniamy
            this->setSpeedRight(170);  // Przyspieszamy prawe
        } else if (cntRight > cntLeft) {
            this->setSpeedLeft(170);
            this->setSpeedRight(130);
        } else {
            this->setSpeed(150);
        }

        // Zatrzymujemy koło, które jako pierwsze przejechało dystans
        if (cntLeft >= targetImpulses) this->stopLeft();
        if (cntRight >= targetImpulses) this->stopRight();
    }
    this->stop(); // Zabezpieczenie, zatrzymuje wszystko
}

void Wheels::goBack(int cm)
{
    unsigned long targetImpulses = cm * IMPULSES_PER_CM;
    cntLeft = 0;
    cntRight = 0;

    this->setSpeed(150);
    this->back();

    while (cntLeft < targetImpulses || cntRight < targetImpulses) {
        if (cntLeft >= targetImpulses) this->stopLeft();
        if (cntRight >= targetImpulses) this->stopRight();
    }
    this->stop();
}

void Wheels::turnRight(int degrees)
{
    unsigned long targetImpulses = degrees * IMPULSES_PER_DEGREE;
    cntLeft = 0;
    cntRight = 0;

    this->setSpeed(150);
    this->forwardLeft(); // Lewe do przodu
    this->backRight();   // Prawe do tyłu (skręt w miejscu)

    while (cntLeft < targetImpulses || cntRight < targetImpulses) {
        if (cntLeft >= targetImpulses) this->stopLeft();
        if (cntRight >= targetImpulses) this->stopRight();
    }
    this->stop();
}

void Wheels::turnLeft(int degrees)
{
    unsigned long targetImpulses = degrees * IMPULSES_PER_DEGREE;
    cntLeft = 0;
    cntRight = 0;

    this->setSpeed(150);
    this->backLeft();     // Lewe do tyłu
    this->forwardRight(); // Prawe do przodu

    while (cntLeft < targetImpulses || cntRight < targetImpulses) {
        if (cntLeft >= targetImpulses) this->stopLeft();
        if (cntRight >= targetImpulses) this->stopRight();
    }
    this->stop();
}

void Wheels::blinkLED()
{
    digitalWrite(13, !digitalRead(13)); // Poprawiono literówkę
}