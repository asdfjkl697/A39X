/*************************************************************************************************
 * * FUJI OTA 程序 251219
 *
==================================================================================================*/
#include "FUJIOTA.h"
#include "FUJIApp.h"
#include "App.h"
#include <stdio.h>

#define OTA_VerSion 2
#define OTA_REC_MAX 540
#define OTA_REC_NUM 512
#define Send_Delay_Num 2
#define OTA_Req_Num 4

#define SECTOR_SIZE_AT 2048
#define OTA_PACK_SIZE 128

#define Mengdiao_OTAHead1 0XAA
#define Mengdiao_OTAHead2 0X55

u8 OTA_Step = 0;		  // @ OTA 标志和步骤 250815
u32 OTA_Timeout_Tick = 0; // @ OTA 超时 250815
// u8 OTA_REC_Data[OTA_REC_MAX];
// u16 OTA_REC_Count = 0;

// u16 OTA_REC_Long = 0; //* 包长度
u16 OTA_REC_Len = 0;  //* 有效长度
u32 OTA_REC_Size = 0; //* 累加长度
u32 OTA_addr = 0;
u16 OTA_REC_Cnt = 0;	  //* 包序号 260204
u32 OTA_REC_Checksum = 0; //* 实际累加和

u32 OTA_FW_Size = 0;	 //* 总长度
u16 OTA_FW_Checksum = 0; //* 累加和

u8 OTA_Err = 0;
u8 OTA_Req_Cnt = 0; //* 重发次数
u8 OTA_Forbid = 1;
u32 OTA_REC_Addr = 0;	//* 接收地址
u32 OTA_Check_Addr = 0; //* 校验地址
u16 OTA_Check_Cnt = 0;	//* 包序号校对 260204

u8 Send_Delay_cnt = 0;

// u8 add_checksum(const u8 *data, size_t length) // 累加和校验码
// {
// 	u8 checksum = 0;
// 	for (size_t i = 0; i < length; i++)
// 	{
// 		checksum += data[i];
// 	}
// 	return checksum;
// }

// 函数用于交换32位整数的字节序
uint32_t swap_endian_32(uint32_t val)
{
	return (val << 24) | ((val << 8) & 0x00FF0000) | ((val >> 8) & 0x0000FF00) | (val >> 24);
}

// // CRC16-CCITT算法实现
// uint16_t OTA_CRC = 0xffff;
// void CRC16_CCITT_OTA(const uint8_t *data, size_t length)
// {
// 	uint16_t poly = 0x1021; // 多项式
// 	while (length--)
// 	{
// 		OTA_CRC ^= (uint16_t)(*data++ << 8); // 将数据字节与OTA_CRC高8位异或，并左移8位
// 		for (uint8_t i = 0; i < 8; i++)
// 		{
// 			if (OTA_CRC & 0x8000)
// 			{									 // 判断最高位
// 				OTA_CRC = (OTA_CRC << 1) ^ poly; // 左移一位并与多项式异或
// 			}
// 			else
// 			{
// 				OTA_CRC <<= 1; // 仅左移一位
// 			}
// 		}
// 	}
// }

/*
=========================================================================================
===============================
** crc16半字查表法
**
=========================================================================================
===============================
*/
static const uint16_t CRC16_HalfWordTab[16] =
	{
		0x0000, 0x1021, 0x2042, 0x3063,
		0x4084, 0x50a5, 0x60c6, 0x70e7,
		0x8108, 0x9129, 0xa14a, 0xb16b,
		0xc18c, 0xd1ad, 0xe1ce, 0xf1ef};
/******************************************************************************************
*******************************
* 函数名称 : CRC16_Cal
* 功能描述 : 计算CRC16
* 输入参数 : pdata -- 指向要计算的数据
 len -- 要计算的长度
 crc -- 计算初值
* 返回参数 : 计算后的CRC值
* 补充说明 : 无
*****************************************************************************************
********************************/
uint16_t OTA_CRC = 0;
uint16_t CRC16_Cal(uint8_t *pdata, uint32_t len, uint16_t crc)
{
	uint8_t crc_H4;
	while (len--)
	{
		crc_H4 = (uint8_t)(crc >> 12);						/* 暂存CRC的高四位 */
		crc <<= 4;											/* CRC左移4位，相当于取CRC的低12位）*/
		crc ^= CRC16_HalfWordTab[crc_H4 ^ (*pdata >> 4)];	/* CRC的高4位和本字节的前半字节相加后查表计算CRC，然后加上上一次CRC的余数 */
		crc_H4 = (uint8_t)(crc >> 12);						/* 暂存CRC的高4位 */
		crc <<= 4;											/* CRC左移4位， 相当于CRC的低12位） */
		crc ^= CRC16_HalfWordTab[crc_H4 ^ (*pdata & 0x0f)]; /* CRC的高4位和本字节的后半字节相加后查表计算CRC，然后再加上上一次CRC的余数 */
		pdata++;
	}
	return (crc);
}

// #define CONNECT_BLUETOOTH // todo 切换蓝牙 260207
#define OTASend_Delay_Time 1 // IDLE已接管帧检测, 发送仅需1周期缓冲
bool FUJU_OTASend_Flag = 0;
u8 FUJU_OTASend_len = 0;
u8 FUJU_OTASend_data[MaxSendLen] = {0};
u8 FUJU_OTASend_Delay = 0;
void FUJIOTA_Send_Pro()
{
	u8 i, len;
	if (FUJU_OTASend_Flag == 0)
		return;
	if (FUJU_OTASend_Delay > 0)
	{
		FUJU_OTASend_Delay--;
		return;
	}
	FUJU_OTASend_data[0] = 0x7e;
	FUJU_OTASend_data[1] = 0x12; // 目标地址
	FUJU_OTASend_data[2] = 0x01; // 源地址
	FUJU_OTASend_data[3] = 0x01; // 协议id
	len = FUJU_OTASend_data[7] + 3;
	FUJU_OTASend_data[4 + len] = add_checksum(&FUJU_OTASend_data[1], len + 3);
	i = zy_z(&FUJU_OTASend_data[1], len + 4); // 转义
	FUJU_OTASend_len = i + len + 6;
	FUJU_OTASend_data[FUJU_OTASend_len - 1] = 0x7e;

	EUSART2_Send_Data(FUJU_OTASend_data, FUJU_OTASend_len); // @ 260303

	FUJU_OTASend_Flag = 0;
}

void OTA_Handler(u8 *OtaData, u16 len)
{
	u8 cmd, rec_packid, object_len, object_cmd, firmware_type; //* packid
	u8 object_len_tx = 0;

	u8 ota_ack[10], ota_len;
	u16 temp16;
	u32 temp;

	TICK_RESET(OTA_Timeout_Tick); // @ OTA超时复位
	rec_packid = OtaData[4];
	cmd = OtaData[5];
	object_len = OtaData[7];
	object_cmd = OtaData[8];

	ota_ack[0] = rec_packid;
	ota_ack[1] = cmd;
	ota_ack[2] = 0;			 // * 应答码
	ota_ack[4] = object_cmd; // * 对象命令
	if (cmd == 0x02)
	{
		if (object_cmd == 0x4d)
		{
			object_len_tx = 4; // * 对象发送长度
			ota_ack[3] = object_len_tx;
			ota_ack[5] = (OTA_Step > 0) ? 1 : 0; // * IAP标志位
			ota_ack[6] = (OTA_Step > 0) ? 1 : 0; // * 升级中
			memcpy(&FUJU_OTASend_data[4], &ota_ack[0], object_len_tx + 3);
			FUJU_OTASend_Flag = 1;
			FUJU_OTASend_Delay = OTASend_Delay_Time;
		}
		else if (object_cmd == 0x3d)
		{
			object_len_tx = strlen(device_Model) + 2;
			ota_ack[3] = object_len_tx;
			memcpy(&ota_ack[5], device_Model, strlen(device_Model));
			memcpy(&FUJU_OTASend_data[4], &ota_ack[0], object_len_tx + 3);
			FUJU_OTASend_Flag = 1;
			FUJU_OTASend_Delay = OTASend_Delay_Time;
		}
		else
			return;
		// FUJIOTA_ACK(ota_ack, object_len_tx + 3);
		return;
	}

	u8 module_id;
	if (cmd == 0xf0)
	{
		switch (object_cmd)
		{
		default:
			break;
		case 1:
			object_len_tx = 2;
			ota_ack[3] = object_len_tx;
			memcpy(&FUJU_OTASend_data[4], &ota_ack[0], object_len_tx + 3);
			FUJU_OTASend_Flag = 1;
			FUJU_OTASend_Delay = OTASend_Delay_Time;
			break;
		case 2:
			object_len_tx = 2;
			ota_ack[3] = object_len_tx;
			memcpy(&FUJU_OTASend_data[4], &ota_ack[0], object_len_tx + 3);
			FUJU_OTASend_Flag = 1;
			FUJU_OTASend_Delay = OTASend_Delay_Time;
			OTA_Step = 1;
			break;
		case 3:
			module_id = OtaData[9];
			if (module_id != 1) // * 只有中心板升级
				break;
			object_len_tx = 5;
			ota_ack[3] = object_len_tx;
			ota_ack[5] = (APP_SIZE / OTA_PACK_SIZE) & 0xff;
			ota_ack[6] = (APP_SIZE / OTA_PACK_SIZE) >> 8;
			ota_ack[7] = OTA_PACK_SIZE; //* 包长度
			memcpy(&FUJU_OTASend_data[4], &ota_ack[0], object_len_tx + 3);
			FUJU_OTASend_Flag = 1;
			FUJU_OTASend_Delay = OTASend_Delay_Time;
			OTA_Step = 2;
			break;
		case 4:
			temp16 = OtaData[30];
			temp16 = temp16 << 8 | OtaData[29];
			OTA_FW_Checksum = temp16;
			if ((app_ota_erase(TEMPLATE_START_ADDR, APP_SIZE / SECTOR_SIZE_AT) == SUCCESS))
			{
				OTA_addr = TEMPLATE_START_ADDR;
				OTA_REC_Size = 0;
				OTA_REC_Addr = 0;
				OTA_Check_Addr = 0;
				OTA_REC_Cnt = 0;
				OTA_Check_Cnt = 0;
				// OTA_CRC = 0xffff; //* OTA crc 复位
				OTA_CRC = 0; //* OTA crc 复位
				OTA_Forbid = 1;
				OTA_Step = 1;
				OTA_Req_Cnt = 0;
				OTA_Err = 0;
				// todo 添加复位功能 260204
			}
			else
			{
				OTA_Req_Cnt++;
				OTA_Forbid = 2; //* 禁止OTA
			}

			object_len_tx = 5;
			ota_ack[3] = object_len_tx;
			ota_ack[5] = OTA_Forbid;		   //* 文件正确
			ota_ack[6] = OTA_Check_Cnt & 0xff; //* 期望序号
			ota_ack[7] = OTA_Check_Cnt >> 8;
			memcpy(&FUJU_OTASend_data[4], &ota_ack[0], object_len_tx + 3);
			FUJU_OTASend_Flag = 1;
			FUJU_OTASend_Delay = OTASend_Delay_Time;
			OTA_Step = 3;
			break;
		case 5:
			temp16 = OtaData[10];
			temp16 = temp16 << 8 | OtaData[9];
			OTA_REC_Cnt = temp16;
			if (OTA_REC_Cnt != OTA_Check_Cnt)
			{
				object_len_tx = 4;
				ota_ack[3] = object_len_tx;
				ota_ack[5] = OTA_Check_Cnt & 0xff; //* 期望序号
				ota_ack[6] = OTA_Check_Cnt >> 8;

				memcpy(&FUJU_OTASend_data[4], &ota_ack[0], object_len_tx + 3);
				FUJU_OTASend_Flag = 1;
				FUJU_OTASend_Delay = OTASend_Delay_Time;
				OTA_Step = 4;

				OTA_Req_Cnt++;
				break;
			}
			OTA_REC_Len = object_len - 4;
			if (app_ota_write(OTA_addr, &OtaData[11], OTA_REC_Len) == SUCCESS)
			{
				OTA_addr += OTA_REC_Len;
				OTA_REC_Size += OTA_REC_Len;
				// CRC16_CCITT_OTA(&OtaData[11], OTA_REC_Len);
				OTA_CRC = CRC16_Cal(&OtaData[11], OTA_REC_Len, OTA_CRC); // @ 新CRC计算 260205

				OTA_Check_Cnt++; // @ 下一包序号 260204
				OTA_Req_Cnt = 0;
			}
			else
			{
				OTA_Req_Cnt++;
			}

			object_len_tx = 4;
			ota_ack[3] = object_len_tx;
			ota_ack[5] = OTA_Check_Cnt & 0xff; //* 期望序号
			ota_ack[6] = OTA_Check_Cnt >> 8;
			memcpy(&FUJU_OTASend_data[4], &ota_ack[0], object_len_tx + 3);
			FUJU_OTASend_Flag = 1;
			FUJU_OTASend_Delay = OTASend_Delay_Time;
			OTA_Step = 4;
			break;
		case 6:
			memcpy(&OTA_FW_Size, &OtaData[9], sizeof(uint32_t));
			object_len_tx = 4;
			ota_ack[3] = object_len_tx;
			ota_ack[5] = OTA_CRC & 0xff;
			ota_ack[6] = OTA_CRC >> 8;
			memcpy(&FUJU_OTASend_data[4], &ota_ack[0], object_len_tx + 3);
			FUJU_OTASend_Flag = 1;
			FUJU_OTASend_Delay = OTASend_Delay_Time;
			if (OTA_CRC == OTA_FW_Checksum)
				OTA_Step = 5;
			else
				OTA_Err = 4;
			break;
		case 7:
			object_len_tx = 3;
			ota_ack[3] = object_len_tx;
			ota_ack[5] = OtaData[9]; // * 停止/继续升级 260207
			memcpy(&FUJU_OTASend_data[4], &ota_ack[0], object_len_tx + 3);
			FUJU_OTASend_Flag = 1;
			FUJU_OTASend_Delay = OTASend_Delay_Time;
			break;
		case 0x10:
			object_len_tx = 2;
			ota_ack[3] = object_len_tx;
			memcpy(&FUJU_OTASend_data[4], &ota_ack[0], object_len_tx + 3);
			FUJU_OTASend_Flag = 1;
			FUJU_OTASend_Delay = OTASend_Delay_Time;
			OTA_Step = 0; // * 取消升级
			break;
		case 0x20:
			object_len_tx = 4;
			ota_ack[3] = object_len_tx;
			ota_ack[5] = 1; // * 模块1 主板
			if (OTA_Err == 4)
			{
				ota_ack[6] = 4; // * 升级失败
			}
			else if (OTA_Step == 0)
			{
				ota_ack[6] = 3;
			}
			else if (OTA_Step < 5)
			{
				ota_ack[6] = 1;
			}
			else if (OTA_Step == 5)
			{
				ota_ack[6] = 2;
			}
			else
			{
				ota_ack[6] = 0x0f; // * 升级失败
			}
			memcpy(&FUJU_OTASend_data[4], &ota_ack[0], object_len_tx + 3);
			FUJU_OTASend_Flag = 1;
			FUJU_OTASend_Delay = OTASend_Delay_Time;
			break;
		case 0x30:
			object_len_tx = 2;
			ota_ack[3] = object_len_tx;
			memcpy(&FUJU_OTASend_data[4], &ota_ack[0], object_len_tx + 3);
			FUJU_OTASend_Flag = 1;
			FUJU_OTASend_Delay = OTASend_Delay_Time;
			OTA_Step = 0; // * 取消升级
			break;
		}
		// FUJIOTA_ACK(ota_ack, object_len_tx + 3);
	}
}

//* 程序更新处理
void OTA_Upgrade_Pro(void) // @ 10ms
{
	FUJIOTA_Send_Pro();

	if (OTA_Step == 5)
	{
		if (TICK_COUNTER(OTA_Timeout_Tick, 1500))
		{
			OTA_Step = 0;
			write_upgrade_flag(); //* 写更新标志
			NVIC_SystemReset(); // 系统复位
		}
	}
	else if (OTA_Step > 0)
	{
		if (TICK_COUNTER(OTA_Timeout_Tick, 30000) || OTA_Req_Cnt > OTA_Req_Num) //* 30S OTA timeout 请求出错次数超过
		{
			OTA_Step = 0;
		}
	}
}
