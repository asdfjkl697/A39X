#include "_485Com.h"
// u8 Debug_thread_cnt=0;
void _485_ComTx(u8 taraddr, u8 souraddr, u8 count, u16 cmd, u16 len, u8 *data)
{

	u8 i, Temp[100];

	if (COM4.B_TX_busy) /* 当正在有模块通过485发送信息时，新的信息被屏蔽 */
	{
		//			COM4.B_TX_busy++;
		//			if(COM4.B_TX_busy > 5)    /* 当占用485的时间太长，则默认发送失败，进行下一数据帧的接收 */
		//			{
		//				COM4.B_TX_busy = 0;
		//			}
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
