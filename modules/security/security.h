//=====[#include guards - begin]===============================================
#ifndef _SECURITY_H
#define _SECURITY_H

//=====[Declaration of public defines]=========================================

//=====[Declarations (prototypes) of public functions]=========================

void matrixKeypadInit();
void securityOutputsInit();

void alarmDeactivationUpdate();
bool checkIfCodeCorrect();
bool checkIfCodeInorrect();

//=====[#include guards - end]=================================================
#endif // _SECURITY_H
