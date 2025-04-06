#ifndef ARMKEY_H
#define ARMKEY_H

#include "config.h"
#include "_485Com.h"
#include <string.h>

void ArmKeyInit(void);
void ArmKeyDataTx(u8 dat);
void A390_Handler(void);
void KeyEncoderHandler(void);
void ArmKeyDataRx(void);
extern u8 ArmKeyTx_DriveCmd;

#endif
