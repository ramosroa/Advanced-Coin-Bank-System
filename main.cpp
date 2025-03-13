//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"
#include "coin_bank_system.h"

//=====[Main function, the program entry point after power on or reset]========

int main()
{
    coinBankSystemInit();
    
    while (true) {
        coinBankSystemInit();
        delay(TIME_INCREMENT_MS);
    }
}
