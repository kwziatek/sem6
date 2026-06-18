#include <Arduino.h>

#include "Wheels.h"


#define SET_MOVEMENT(side,f,b) digitalWrite( side[0], f);\
                               digitalWrite( side[1], b)

Wheels::Wheels() 
{ }

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
    if (this->isMovingBack) {
        long microseconds = map(this->currentSpeed, 0, 255, 1000000, 100000); // is it right?
    }
}

void Wheels::attach(int pRF, int pRB, int pRS, int pLF, int pLB, int pLS)
{
    this->attachRight(pRF, pRB, pRS);
    this->attachLeft(pLF, pLB, pLS);

    pinMode(13, OUTPUT); //Initialize LED pin
}

void Wheels::forwardLeft() 
{
    SET_MOVEMENT(pinsLeft, HIGH, LOW);
}

void Wheels::forwardRight() 
{
    SET_MOVEMENT(pinsRight, HIGH, LOW);
}

void Wheels::backLeft()
{
    SET_MOVEMENT(pinsLeft, LOW, HIGH);
}

void Wheels::backRight()
{
    SET_MOVEMENT(pinsRight, LOW, HIGH);
}

void Wheels::forward()
{
    this->forwardLeft();
    this->forwardRight();

    this->isMovingBack = false;
    Timer1.detachInterrupt();
    digitalWrite(13, LOW);
}

void Wheels::back()
{
    this->backLeft();
    this->backRight();

    this->isMovingBack = true;

    long microseconds = map(this->currentSpeed, 0, 255, 1000000, 100000);

    Timer1.initialize(microseconds);
    Timer1.attachInterrupt(Wheels::blinkLED);
}

void Wheels::stopLeft()
{
    SET_MOVEMENT(pinsLeft, LOW, LOW);
}

void Wheels::stopRight()
{
    SET_MOVEMENT(pinsRight, LOW, LOW);
}

void Wheels::stop()
{
    this->stopLeft();
    this->stopRight();

    this->isMovingBack = false;
    Timer1.detachInterrupt();
    digitalWrite(13, LOW);
}

void Wheels::goForward(int cm)
{
    this->setSpeed(150);
    this->forward();
    delay(30 * cm);
    this->stop();
}

void Wheels::goBack(int cm)
{
    this->setSpeed(150);
    this->back();
    delay(30 * cm);
    this->stop();
}

void Wheels::blinkLED()
{
    digitalWrite(13, !ditialRead(13));
}
