//=====[Libraries]=============================================================
#include "mbed.h"
#include "arm_book_lib.h"
#include "coin_total_counter.h"
#include "coin_bank_system.h"
#include "security.h"

//=====[Declaration of private defines]========================================
#define ONE 1
#define FIVE 5
#define TEN 10
#define TWENTYFIVE 25

//=====[Declaration and initialization of public global objects]===============
DigitalIn oneCent(D35);
DigitalIn fiveCent(D36);
DigitalIn tenCent(D37);
DigitalIn twentyFiveCent(D38);

DigitalIn sensor(D39);


//=====[Declaration and initialization of public global variables]=============
float totalAmount = 0;

//=====[Declaration and initialization of private global variables]============
static float amountAdded = 0;


//=====[Declarations (prototypes) of private functions]========================

static void currentCoinDenomination();



//=====[Implementations of public functions]===================================

void buttonInit() //Initialized all the button used for coin denominations into pull down mode aswell as the sensor into pull up mode
{
    oneCent.mode(PullDown);
    fiveCent.mode(PullDown);
    tenCent.mode(PullDown);
    twentyFiveCent.mode(PullDown);
    sensor.mode(PullUp);

}


void totalMoneyUpdate() // Updates the amount of money added to the total money aswell as clears the total money if the password inputed is correct
{
    static bool lastSensorState = true;  
    
    currentCoinDenomination();

    bool currentSensorState = sensor.read();  

    if (!currentSensorState && lastSensorState) {  
        totalAmount += amountAdded;
    }

    lastSensorState = currentSensorState;
    
    if (checkIfCodeCorrect() == true)

    {
        totalAmount = 0;
    }

}

float currentTotalMoney() //returns the total money amount to be used by other modules
{
    
    return totalAmount;

}

//=====[Implementations of private functions]==================================

static void currentCoinDenomination() //Depednign on the button pressed the amount of money added changes with this function
{
    
    if(oneCent)
    {

       amountAdded = ONE;

    }

    if (fiveCent)
    {

        amountAdded = FIVE;

    }
    if (tenCent)
    {

        amountAdded = TEN;

    }
    if (twentyFiveCent)
    {

        amountAdded = TWENTYFIVE;

    }


}
