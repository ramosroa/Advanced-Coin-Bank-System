//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"

#include "user_interface.h"
#include "display.h"
#include "coin_total_counter.h"
#include "coin_bank_system.h"
#include "security.h"


//=====[Declaration of private defines]========================================

#define DISPLAY_REFRESH_TIME_MS 1000

//=====[Declarations (prototypes) of private functions]========================

static void userInterfaceDisplayInit();
static void userInterfaceDisplayUpdate();

//=====[Implementations of public functions]===================================
void userInterfaceInit()
{
    userInterfaceDisplayInit();
}

void userInterfaceUpdate()
{
    userInterfaceDisplayUpdate();
}

//=====[Implementations of private functions]===================================
static void userInterfaceDisplayInit() //Initializes the display
{
    displayInit();
}

static void userInterfaceDisplayUpdate() //Updates the display with our total money and if a password has been inputted
{
    static int accumulatedDisplayTime = 0;
    char money[20] = "";

    if( accumulatedDisplayTime >= DISPLAY_REFRESH_TIME_MS ) {

        accumulatedDisplayTime = 0;


        if (checkIfCodeCorrect() == false && checkIfCodeInorrect() == false ){
        
        sprintf(money, "Total: $%.2f    ", currentTotalMoney()/100);

        displayCharPositionWrite(0, 0);
        displayStringWrite(money);
        displayCharPositionWrite(0, 1);
        displayStringWrite("                ");
        }
    
    
        else if (checkIfCodeCorrect() == true){

        
        displayCharPositionWrite(0, 0);
        displayStringWrite("Correct Code    ");
        displayCharPositionWrite(0, 1);
        displayStringWrite("Spend $ wisely  ");
        }
    
        else if (checkIfCodeInorrect() == true){

        displayCharPositionWrite(0, 0);
        displayStringWrite("Incorrect Code  ");
        displayCharPositionWrite(0, 1);
        displayStringWrite("Try Again        ");
        }
    }

   
    else //Keeps track of refresh time
    
    {
        accumulatedDisplayTime = accumulatedDisplayTime + TIME_INCREMENT_MS;   

    } 
    
}

