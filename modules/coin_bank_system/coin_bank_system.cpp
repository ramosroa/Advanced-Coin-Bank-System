//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"
#include "coin_total_counter.h"
#include "user_interface.h"
#include "coin_bank_system.h"
#include "security.h"
#include "motor.h"

//=====[Declaration of private defines]========================================

//=====[Declaration of private data types]=====================================

//=====[Declaration and initialization of public global objects]===============

//=====[Declaration of external public global variables]=======================

//=====[Declaration and initialization of public global variables]=============

//=====[Declaration and initialization of private global variables]============

//=====[Declarations (prototypes) of private functions]========================

//=====[Implementations of public functions]===================================

void coinBankSystemInit() // Initialized all our modules to start the coin bank system 
{
    uttonInit();
    userInterfaceInit();

    matrixKeypadInit();
    securityOutputsInit();

    motorInit();
}

void coinBankSystemUpdate() // makes sure that every input to the coinbank system and output like the display is always keeping track of any changes
{
    totalMoneyUpdate();
    alarmDeactivationUpdate();
    userInterfaceUpdate();
    motorUpdate();

}