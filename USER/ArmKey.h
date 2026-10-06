#ifndef ARMKEY_H
#define ARMKEY_H

#include "config.h"
#include "_485Com.h"
#include <string.h>

extern RxPro_S RxVar;

extern bool Key_Lying_Up;
extern bool Key_Lying_Dw;

void ArmKeyInit(void);
void ArmKey_Handler(void);
void KeyEncoderHandler(void);
void ArmKeyDataTx(void);

#endif
