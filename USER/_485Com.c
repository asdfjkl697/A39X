#include "_485Com.h"
#include "Leg.h"
#include "MassageLogic.h"

u32 MoveUp_Loss_tick = 0;
u32 ArmKey_Loss_tick = 0;
u32 Leg_Loss_tick = 0;

u8 MSG2MoveUp_Count = 0;
u8 LegTx_Count = 0;
u8 ArmKeyTx_Count = 0;

#ifdef USE_M2
u32 MoveDw_Loss_tick = 0;
u8 MSG2MoveDw_Count = 0;
#endif

u8 RS485_DriveCmd = 0; //* 发送中
u8 RS485_Resend = 0;

static u8 CRC_CheckSum_L8, CRC_CheckSum_H8;
u8 PartRxAddr = 0;
u8 PartRxCount = 0;
RxPro_S Rx485Var;

// 解析接收到的数据
void RS485_DataRx(u8 dat)
{
	u16 CRC_CheckSum;

	Rx485Var.RxBuffer[Rx485Var.RxCount] = dat;
	switch (Rx485Var.RxState)
	{
	default:
		break;
	case 0: // 解析帧头
		if (Rx485Var.RxBuffer[Rx485Var.RxCount] == _485_Com_Head0)
		{
			Rx485Var.RxCount++;
			Rx485Var.RxState++;
		}
		else
		{
			Rx485Var.RxCount = 0;
			Rx485Var.RxState = 0;
		}
		break;

	case 1: // 解析帧头
		if (Rx485Var.RxBuffer[Rx485Var.RxCount] == _485_Com_Head1)
		{
			Rx485Var.RxCount++;
			Rx485Var.RxState++;
		}
		else
		{
			Rx485Var.RxCount = 0;
			Rx485Var.RxState = 0;
		}
		break;

	case 2: // 解析目标地址
		if (Rx485Var.RxBuffer[Rx485Var.RxCount] == MainAddr)
		{
			Rx485Var.RxCount++;
			Rx485Var.RxState++;
		}
		else
		{
			Rx485Var.RxCount = 0;
			Rx485Var.RxState = 0;
		}
		break;

	case 3: // 解析源地址
		PartRxAddr = Rx485Var.RxBuffer[Rx485Var.RxCount];
		Rx485Var.RxCount++;
		Rx485Var.RxState++;
		break;

	case 4: // 解析序号
		PartRxCount = Rx485Var.RxBuffer[Rx485Var.RxCount];
		Rx485Var.RxCount++;
		Rx485Var.RxState++;
		break;

	case 5: // 解析命令
		if (Rx485Var.RxBuffer[Rx485Var.RxCount])
		{
			Rx485Var.RxDataCmdL8 = Rx485Var.RxBuffer[Rx485Var.RxCount];
			Rx485Var.RxCount++;
			Rx485Var.RxState++;
		}
		else
		{
			Rx485Var.RxCount = 0;
			Rx485Var.RxState = 0;
		}
		break;

	case 6: // 解析命令

		Rx485Var.RxDataCmdH8 = Rx485Var.RxBuffer[Rx485Var.RxCount];
		Rx485Var.RxDataCmd = Rx485Var.RxDataCmdH8 << 8 | Rx485Var.RxDataCmdL8;

		if (Rx485Var.RxDataCmd == 0x0001 || Rx485Var.RxDataCmd == 0x0002)
		{
			Rx485Var.RxCount++;
			Rx485Var.RxState++;
		}
		else
		{
			Rx485Var.RxCount = 0;
			Rx485Var.RxState = 0;
		}
		break;

	case 7: // 解析数据长度L8
		Rx485Var.RxDataLenL8 = Rx485Var.RxBuffer[Rx485Var.RxCount];

		Rx485Var.RxCount++;
		Rx485Var.RxState++;
		break;

	case 8: // 解析数据长度H8
		Rx485Var.RxDataLenH8 = Rx485Var.RxBuffer[Rx485Var.RxCount];
		Rx485Var.RxDataNum = Rx485Var.RxDataLenH8 << 8 | Rx485Var.RxDataLenL8;
		if (Rx485Var.RxDataNum > 0 && Rx485Var.RxDataNum < Max_RxBuffer_Len) //* 查map 有问题 修改成 数组最大长度  250724
		{
			Rx485Var.RxCount++;
			Rx485Var.RxState++;
		}
		else
		{
			Rx485Var.RxCount = 0;
			Rx485Var.RxState = 0;
		}
		break;

	case 9: // 解析数据
		if (Rx485Var.RxDataNum > 0)
		{
			Rx485Var.RxDataNum--;
			Rx485Var.RxCount++;
		}
		else
		{
			CRC_CheckSum_L8 = Rx485Var.RxBuffer[Rx485Var.RxCount]; // CRC校验值 低8位
			Rx485Var.RxCount++;
			Rx485Var.RxState++;
		}
		break;

	case 10: // CRC校验

		CRC_CheckSum_H8 = Rx485Var.RxBuffer[Rx485Var.RxCount]; // CRC校验值 高8位
		CRC_CheckSum = CRC_CheckSum_H8 << 8 | CRC_CheckSum_L8;

		if (CRC16_CCITT_FALSE(Rx485Var.RxBuffer, (Rx485Var.RxDataLenH8 << 8 | Rx485Var.RxDataLenL8) + 9) != CRC_CheckSum)
		{
			Rx485Var.RxState = 0;
			Rx485Var.RxCount = 0;
			break;
		}

		switch (PartRxAddr)
		{
		case MoveUpAddr:
			TICK_RESET(MoveUp_Loss_tick); // * 心跳计数清零
			if (PartRxCount != MSG2MoveUp_Count)
				break;
			MSG2MoveUp_Count++;
			TxPolling++;
			MoveUp_Updata_Clear();
			memcpy(&MSG4Move[MoveUp], &Rx485Var.RxBuffer[9], sizeof(MSG4Move_R));
			break;
		case LegAddr:
			TICK_RESET(Leg_Loss_tick); // * 心跳计数清零
			if (PartRxCount != LegTx_Count)
				break;
			LegTx_Count++;
			TxPolling++;
			memcpy(&LegRx, &Rx485Var.RxBuffer[9], sizeof(LegDataRx_S));
			break;
		case ArmKeyAddr:
			TICK_RESET(ArmKey_Loss_tick); // * 心跳计数清零
			if (PartRxCount != ArmKeyTx_Count)
				break;
			ArmKeyTx_Count++;
			TxPolling++;
			memcpy(&RxVar.RxBuffer[9], &Rx485Var.RxBuffer[9], sizeof(ArmKeyRx_S));
			ArmKey_Handler();
			break;
		case MoveDwAddr:
#ifdef USE_M2
			TICK_RESET(MoveDw_Loss_tick); // * 心跳计数清零
			if (PartRxCount != MSG2MoveDw_Count)
				break;
			MSG2MoveDw_Count++;
			TxPolling++;
			MoveDw_Updata_Clear();
			memcpy(&MSG4Move[MoveDw], &Rx485Var.RxBuffer[9], sizeof(MSG4Move_R));
#endif
			break;
		default:
			break;
		}

		Rx485Var.RxState = 0;
		Rx485Var.RxCount = 0;
		break;
	}
}

void _485_ComRx(void)
{
	u8 data;
	while (COM4.RX_read != COM4.RX_write)
	{
		data = EUART4_Read();
		RS485_DataRx(data);
	}
}

#ifdef USE_M2
#if (LEG_COM)
#define TxChMax 4
#else
#define TxChMax 3
#endif
#else
#if (LEG_COM)
#define TxChMax 3
#else
#define TxChMax 2
#endif
#endif

uint8_t TxPolling = 0;
uint8_t TxPollingPre;
uint8_t TxKeepCnt = 0;

bool RS485_Idle_Flag = 0;
u8 RS485_Delay = 0;
u8 RS485_Idle_Cnt = 0;
u16 Debug_TxKeepCnt = 0; // @ 测试重发次数 251007
u16 Debug_TxKeepCntA = 0;
void RS485_Poll_Reset(void) // * add 251104
{
	RS485_Idle_Cnt = 20;
	TxPolling = 0;
}
void RS485_Polling(void)
{
	if (++RS485_Idle_Cnt > 20)
	{
		RS485_Idle_Cnt = 0;
		RS485_Idle_Flag = 0;
	}
	else if (TxPolling >= TxChMax && RS485_Idle_Flag == 0)
		RS485_Idle_Flag = 1;

	if (RS485_Idle_Flag)
	{
		return;
	}

	if (TxPollingPre == TxPolling && RS485_Delay > 0) // @ 重发间隔2*5ms
	{
		RS485_Delay--;
		return;
	}

	/*485轮询通讯*/
	if (TxPolling >= TxChMax)
		TxPolling = 0;

	if (TxPollingPre != TxPolling)
	{
		TxKeepCnt = 0;
		TxPollingPre = TxPolling;
	}
	else
	{
		TxKeepCnt++;	   /* 若只有一个通讯设备，该处屏蔽，一般不需要屏蔽 */
		Debug_TxKeepCnt++; //* 测试用
	}

	if (TxKeepCnt > 2) // * 重发3次
	{
		TxPolling++;
		TxKeepCnt = 0;
		Debug_TxKeepCntA++; //* 测试用
	}
	else if (TxKeepCnt > 0)
	{
		if ((TxPolling == 0 && TICK_COUNTER(MoveUp_Loss_tick, 3000)) ||
#if defined USE_M2 && (LEG_COM)
			(TxPolling == 1 && TICK_COUNTER(MoveDw_Loss_tick, 3000)) ||
			(TxPolling == 2 && TICK_COUNTER(Leg_Loss_tick, 5000)) ||
			(TxPolling == 3 && TICK_COUNTER(ArmKey_Loss_tick, 5000))
#elif (LEG_COM)
			(TxPolling == 1 && TICK_COUNTER(Leg_Loss_tick, 5000)) ||
			(TxPolling == 2 && TICK_COUNTER(ArmKey_Loss_tick, 5000))
#elif defined USE_M2
			(TxPolling == 1 && TICK_COUNTER(MoveDw_Loss_tick, 3000)) ||
			(TxPolling == 2 && TICK_COUNTER(ArmKey_Loss_tick, 5000))
#else
			(TxPolling == 1 && TICK_COUNTER(ArmKey_Loss_tick, 5000))
#endif
		)
		{
			TxPolling++;
			TxPollingPre = TxPolling; // @ add for resend bug 260423
			TxKeepCnt = 0;
		}
	}

	RS485_Delay = 1;
	switch (TxPolling)
	{
	case 0:
		MSG2MoveUp_Deal();
		if (MoveUpDw_Loss_Flag)
			RS485_Delay = 2;
		break;
#if defined USE_M2 && (LEG_COM)
	case 1:
		MSG2MoveDw_Deal();
		if (MoveUpDw_Loss_Flag)
			RS485_Delay = 2;
		break;
	case 2:
		LegDataTx();
		RS485_Delay = 2; // @ 间隔时间不同 251206
		break;
	case 3:
		ArmKeyDataTx();
		break;
#elif (LEG_COM)
	case 1:
		LegDataTx();
		RS485_Delay = 2;
		break;
	case 2:
		ArmKeyDataTx();
		break;
#elif defined USE_M2
	case 1:
		MSG2MoveDw_Deal();
		break;
	case 2:
		ArmKeyDataTx();
		break;
#else
	case 1:
		ArmKeyDataTx();
		break;
#endif
	default:
		break;
	}
}

void _485_ComTx(u8 taraddr, u8 souraddr, u8 count, u16 cmd, u16 len, u8 *data)
{

	u8 i, Temp[100];

	if (COM4.B_TX_busy) /* 当正在有模块通过485发送信息时，新的信息被屏蔽 */
	{
		return;
	}

	Temp[0] = _485_Com_Head0;
	Temp[1] = _485_Com_Head1;
	Temp[2] = taraddr;
	Temp[3] = souraddr;
	Temp[4] = count;
	Temp[5] = cmd & 0x00FF;
	Temp[6] = (cmd & 0xFF00) >> 8;
	Temp[7] = len & 0x00FF;
	Temp[8] = (len & 0xFF00) >> 8;

	for (i = 0; i < len; i++)
	{

		Temp[9 + i] = data[i];
	}
	Temp[9 + len] = CRC16_CCITT_FALSE(Temp, 9 + len) & 0x00FF;
	Temp[9 + len + 1] = (CRC16_CCITT_FALSE(Temp, 9 + len) & 0xFF00) >> 8;

	for (i = 0; i < (9 + len + 2); i++)
	{

		EUART4_Write(Temp[i]);
	}
	// 发送使能
	COM4.B_TX_busy = 1;
	gpio_bit_set(GPIOD, GPIO_PIN_0);
	//		    delay_1ms(1);
	usart_interrupt_enable(UART4, USART_INT_TBE);
}

unsigned short CRC16_CCITT_FALSE(unsigned char *puchMsg, unsigned int usDataLen)
{
	unsigned short wCRCin = 0xFFFF;
	unsigned short wCPoly = 0x1021;
	unsigned char wChar = 0;
	u8 i;
	while (usDataLen--)
	{
		wChar = *(puchMsg++);
		wCRCin ^= (wChar << 8);

		for (i = 0; i < 8; i++)
		{
			if (wCRCin & 0x8000)
			{
				wCRCin = (wCRCin << 1) ^ wCPoly;
			}
			else
			{
				wCRCin = wCRCin << 1;
			}
		}
	}
	return (wCRCin);
}
