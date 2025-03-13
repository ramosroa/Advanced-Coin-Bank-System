//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"
#include "coin_bank_system.h"
#include "coin_bank_system.h"


//=====[Defines]===============================================================

#define NUMBER_OF_KEYS                           4
#define NUMBER_OF_AVG_SAMPLES                  100
#define OVER_TEMP_LEVEL                         50
#define TIME_INCREMENT_MS                       10
#define DEBOUNCE_KEY_TIME_MS                    50
#define KEYPAD_NUMBER_OF_ROWS                    4
#define KEYPAD_NUMBER_OF_COLS                    4

//=====[Declaration of private data types]======================================

typedef enum {
    MATRIX_KEYPAD_SCANNING,
    MATRIX_KEYPAD_DEBOUNCE,
    MATRIX_KEYPAD_KEY_HOLD_PRESSED
} matrixKeypadState_t; //Variables useful to debounce the matrix keypad 

//=====[Declaration and initialization of public global objects]===============


DigitalOut incorrectCodeLed(LED3);
DigitalOut systemBlockedLed(LED2);
DigitalOut correctCodeLed(LED1);

UnbufferedSerial serialMonitor(USBTX, USBRX, 115200);


DigitalOut keypadRowPins[KEYPAD_NUMBER_OF_ROWS] = {PB_3, PB_5, PC_7, PA_15}; //Sets up the matrix keypad
DigitalIn keypadColPins[KEYPAD_NUMBER_OF_COLS]  = {PB_12, PB_13, PB_15, PC_6};//Sets up the matrix keypad

//=====[Declaration and initialization of public global variables]=============

bool incorrectCode = false;
bool correctCode = false;

int numberOfIncorrectCodes = 0;
int numberOfHashKeyReleasedEvents = 0;
int keyBeingCompared    = 0;
int keyBeingConfigured  = 0;
char codeSequence[NUMBER_OF_KEYS]   = { '8', '8', '8', '8' };
char keysPressed[NUMBER_OF_KEYS] = { '0', '0', '0', '0' };
int accumulatedTimeAlarm = 0;


int accumulatedDebounceMatrixKeypadTime = 0;
int matrixKeypadCodeIndex = 0;
char matrixKeypadLastKeyPressed = '\0';
char matrixKeypadIndexToCharArray[] = {
    '1', '2', '3', 'A',
    '4', '5', '6', 'B',
    '7', '8', '9', 'C',
    '*', '0', '#', 'D',
};
matrixKeypadState_t matrixKeypadState;

//=====[Declarations (prototypes) of public functions]=========================

void securityOutputsInit();

void alarmDeactivationUpdate();
bool checkIfCodeCorrect();
bool checkIfCodeInorrect();



void matrixKeypadInit();

//=====[Declarations (prototypes) of private functions]========================

static char matrixKeypadScan();
static char matrixKeypadUpdate();
static bool areEqual();


//=====[Main function, the program entry point after power on or reset]========


//=====[Implementations of public functions]===================================

void securityOutputsInit() //Initializes the used variables for the security module
{
    incorrectCodeLed = OFF;
    systemBlockedLed = OFF;
    correctCodeLed = OFF;
    bool incorrectCode = false;
    bool correctCode = false;
}

void alarmDeactivationUpdate() //updates for each password inputted where it evaluates whether the password is right or wrong and changed the outputs accordingly 
{
    
    if ( numberOfIncorrectCodes < 5 ) {
        char keyReleased = matrixKeypadUpdate();
        if( keyReleased != '\0' && keyReleased != '#' ) {
            keysPressed[matrixKeypadCodeIndex] = keyReleased;
            if( matrixKeypadCodeIndex >= NUMBER_OF_KEYS ) {
                matrixKeypadCodeIndex = 0;
            } else {
                matrixKeypadCodeIndex++;
            }
        }
        if( keyReleased == '#' ) {
            
            char buffer[50]; // Temporary buffer for serial output
            sprintf(buffer, "Keys Entered: %c%c%c%c\n", keysPressed[0], keysPressed[1], keysPressed[2], keysPressed[3]);
            serialMonitor.write(buffer, strlen(buffer));

            if( incorrectCodeLed ) {
                numberOfHashKeyReleasedEvents++;
                if( numberOfHashKeyReleasedEvents >= 2 ) {
                    incorrectCodeLed = OFF;
                    correctCodeLed = OFF;
                    correctCode = false;
                    incorrectCode = false;

                    numberOfHashKeyReleasedEvents = 0;
                    matrixKeypadCodeIndex = 0;
                    keysPressed[0] = '0';
                    keysPressed[1] = '0';
                    keysPressed[2] = '0';
                    keysPressed[3] = '0';
                }
            } else {
                    if ( areEqual() ) {
                        correctCodeLed = ON;
                        correctCode = true;
                        numberOfIncorrectCodes = 0;
                        matrixKeypadCodeIndex = 0;
                        keysPressed[0] = '0';
                        keysPressed[1] = '0';
                        keysPressed[2] = '0';
                        keysPressed[3] = '0';
                    } else {
                        incorrectCodeLed = ON;
                        incorrectCode = true;
                        numberOfIncorrectCodes++;
                    }
                }
            
        }
    } else {
        systemBlockedLed = ON;
    }
}


bool checkIfCodeCorrect() //checks the variable which has the status of the code to use in other modules
{
    return correctCode;

}

bool checkIfCodeInorrect() //checks the variable which has the status of the code to use in other modules
{
    return incorrectCode;
}



void matrixKeypadInit() //Initializes the keypad
{
    matrixKeypadState = MATRIX_KEYPAD_SCANNING;
    int pinIndex = 0;
    for( pinIndex=0; pinIndex<KEYPAD_NUMBER_OF_COLS; pinIndex++ ) {
        (keypadColPins[pinIndex]).mode(PullUp);
    }
}

//=====[Implementations of private functions]==================================


static char matrixKeypadScan()//Scans for the pressed characters
{
    int row = 0;
    int col = 0;
    int i = 0;

    for( row=0; row<KEYPAD_NUMBER_OF_ROWS; row++ ) {

        for( i=0; i<KEYPAD_NUMBER_OF_ROWS; i++ ) {
            keypadRowPins[i] = ON;
        }

        keypadRowPins[row] = OFF;

        for( col=0; col<KEYPAD_NUMBER_OF_COLS; col++ ) {
            if( keypadColPins[col] == OFF ) {
                return matrixKeypadIndexToCharArray[row*KEYPAD_NUMBER_OF_ROWS + col];
            }
        }
    }
    return '\0';
}

static char matrixKeypadUpdate() //Updates the keypad with the keys that have been pressed aswell as debouncing the keypad
{
    char keyDetected = '\0';
    char keyReleased = '\0';

    switch( matrixKeypadState ) {

    case MATRIX_KEYPAD_SCANNING:
        keyDetected = matrixKeypadScan();
        if( keyDetected != '\0' ) {
            matrixKeypadLastKeyPressed = keyDetected;
            accumulatedDebounceMatrixKeypadTime = 0;
            matrixKeypadState = MATRIX_KEYPAD_DEBOUNCE;
        }
        break;

    case MATRIX_KEYPAD_DEBOUNCE:
        if( accumulatedDebounceMatrixKeypadTime >=
            DEBOUNCE_KEY_TIME_MS ) {
            keyDetected = matrixKeypadScan();
            if( keyDetected == matrixKeypadLastKeyPressed ) {
                matrixKeypadState = MATRIX_KEYPAD_KEY_HOLD_PRESSED;
            } else {
                matrixKeypadState = MATRIX_KEYPAD_SCANNING;
            }
        }
        accumulatedDebounceMatrixKeypadTime =
            accumulatedDebounceMatrixKeypadTime + TIME_INCREMENT_MS;
        break;

    case MATRIX_KEYPAD_KEY_HOLD_PRESSED:
        keyDetected = matrixKeypadScan();
        if( keyDetected != matrixKeypadLastKeyPressed ) {
            if( keyDetected == '\0' ) {
                keyReleased = matrixKeypadLastKeyPressed;
            }
            matrixKeypadState = MATRIX_KEYPAD_SCANNING;
        }
        break;

    default:
        matrixKeypadInit();
        break;
    }
    return keyReleased;
}

static bool areEqual()
{
    int i;

    for (i = 0; i < NUMBER_OF_KEYS; i++) {
        if (codeSequence[i] != keysPressed[i]) {
            return false;
        }
    }

    return true;
}
