/***************************************************************************************************
* FUJI OTA 程序 251219
==================================================================================================*/
#ifndef FUJIOTA_H_
#define FUJIOTA_H_

// #include "define.h"
#include "config.h"

#define MaxSendLen 64 // todo add 260303

extern u8 OTA_Step;

extern void OTA_Handler(u8 *OtaData, u16 len);
extern void OTA_Upgrade_Pro(void);
extern void FUJIOTA_Send_Pro(void);

#endif
