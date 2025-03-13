//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"
#include "motor.h"
#include "security.h"

#define TIME_PERIOD 0.02
#define DUTY_MIN            0.025
#define DUTY_MAX            0.125

//=====[Declaration and initialization of private global objects]===============

PwmOut motor(PD_14);

//=====[Declaration and initialization of private global variables]=============
static float max_angle = 0.0; //Used to store the angle we want to reach

//=====[Declarations (prototypes) of private functions]========================

static void setAngle(float degrees);

//=====[Implementations of public functions]===================================

void motorInit() //Initializes the motor
{
    motor.period(TIME_PERIOD);
    motor.write(DUTY_MAX);
}


void motorUpdate() //Updates the motor to move depening if the code inputed is correct or not
{
    if(checkIfCodeCorrect() == true)
    {

        setAngle(0);
        motor.write(max_angle);

    }
    if(checkIfCodeCorrect() == false)
        setAngle(180);
        motor.write(max_angle);
}

static void setAngle(float degrees) //Sets the max angle we want to reach from a given angle in degrees
{
    // Map degrees (0-180) to duty cycle (DUTY_MIN to DUTY_MAX)
    max_angle = DUTY_MIN + ((DUTY_MAX - DUTY_MIN) / 180.0) * degrees;

}