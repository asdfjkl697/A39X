#ifndef HANDLER_H
#define HANDLER_H

#include "config.h"
#include "_485Com.h"
#include "KeyFunc.h"
#include <string.h>

#define MaxKeyData 50 + 11

typedef struct
{
	uint8_t RxState;				   // 接收状态
	uint16_t RxDataNum;				   // 接收数据数量
	uint8_t RxDataLenL8;			   // 接收数据长度L8
	uint8_t RxDataLenH8;			   // 接收数据长度H8
	uint8_t RxDataCmdL8;			   // 接收数据命令L8
	uint8_t RxDataCmdH8;			   // 接收数据命令H8
	uint16_t RxDataCmd;				   // 接收数据命令
	uint8_t RxCount;				   // 接收计数
	uint8_t RxBuffer[MaxKeyData]; // 接收缓存 帧头2+目的地址1+源地址1+序号1+命令2+数据长度2+校验位2

	uint8_t RxOrder; /* 接收命令序号 */
} KeyRx_t;

typedef struct
{
	uint8_t State;
	uint8_t Value;
	uint8_t Param;
} KeyState_t;

typedef struct
{
	uint8_t HandlerKey; /* 手控器传输的键值 */

	uint8_t MainKey; /* 映射后的主板键值 */

	uint8_t KeyPara; /* 按键参数，0xFF：使用手控器传输的参数  其他：重新映射参数     手控器参数有时候会和主板需要参数对应不上，在此映射，灵活处理 */
} HandlerKeyMap_t;

extern KeyRx_t KeyRx;

void KeyCom_Handler(void);
extern void DiyWork_Deal(u8 *data);
#endif
