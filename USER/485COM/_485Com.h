#ifndef _485COM_H
#define _485COM_H

#include "config.h"
// #include "Leg.h"
// #include "ArmKey.h"
// #include "MoveUp.h"
// #include "MoveDw.h"
// #include "main.h"
#include <string.h>

typedef struct
{

	uint8_t RxState;	  // 接收状态
	uint16_t  RxDataLen;     	//接收数据数量
	uint16_t RxDataNum;	  // 接收数据数量
	uint8_t RxDataLenL8;  // 接收数据长度L8
	uint8_t RxDataLenH8;  // 接收数据长度H8
	uint8_t RxDataCmdL8;  // 接收数据命令L8
	uint8_t RxDataCmdH8;  // 接收数据命令H8
	uint16_t RxDataCmd;	  // 接收数据命令
	uint8_t  RxDataParm;   		//接收数据参数
	uint8_t  RxDataNoPage;   		//接收数据包序号
	uint8_t RxCount;	  // 接收计数
	uint8_t RxBuffer[50]; // 接收缓存

} RxPro_S;

#define MainAddr 0x01
#define MoveUpAddr 0x02
#define MoveDwAddr 0x03
#define ArmKeyAddr 0x04
#define LegAddr 0x05
#define HandlerAddr 0x07

// 主板 发送驱动命令 0x0001     获取状态命令0x0002 （无数据）

// 驱动命令部件回复？

// 机芯接收数据 上

typedef struct
{

	uint8_t UserMode; // 0:part0有效 1：part1有效

	/******part0*******/
	uint8_t Mode;		 // 0;无效 1:定点 2：局部  3：全身   4：上半身  5：下半身
	uint8_t PartParamL8; // 局部时 当前位置为中心点设置参数
	uint8_t PartParamH8; // 局部时 当前位置为中心点设置参数
	uint8_t Wise;		 // 常规手法 (0:停止 1:正揉  2:反揉  3 拍打  4敲击（间隙拍打） 揉拍  指压1  6 指压2（二段式） 6 3D正向揉捏  7 3D反向揉捏 8 3D拍打  9 3D敲击  10 3D揉拍  10上揉搓 11 下揉搓 12 上揉搓配合3D+行走（肩胛骨按摩） 13 下揉搓配合3D+行走（肩胛骨按摩）
	uint8_t Speed : 4;	 // (0：默认 1~ x 档)
	uint8_t Width : 4;	 // (0：默认 1 ~ x 档)
						 //		uint8_t Walk;  //0:停止 1：上 2：下 3：上行多少圈 4：下行多少圈 5:指定位置 。。。 手法带行走动作该字节无效
						 //		uint8_t WalkParamL8; // 圈数  当State ：3 4 相对位置  ； 5：绝对位置（上限位零点）
						 //		uint8_t WalkParamH8; // 圈数  当State ：3 4 相对位置  ； 5：绝对位置（上限位零点）
						 //		uint8_t WalkSpeed;  //行走速度
	uint8_t _3DLevel;	 // 0：默认3档    1~5档

	/*******part1*******/

	// 行走电机
	struct
	{
		uint8_t State : 4; // 0:停止 1：上 2：下 3：上行多少圈 4：下行多少圈 5:指定位置
		uint8_t Speed : 4; // 速度1~5
		uint8_t Circle_L8; // 圈数  当State ：3 4 相对位置  ； 5：绝对位置（上限位零点）
		uint8_t Circle_H8; // 圈数  当State ：3 4 相对位置  ； 5：绝对位置（上限位零点）

	} Tread;

	// 揉捏电机
	struct
	{

		uint8_t State : 4; // 0:停止 1：正转 2：反转 3：正转多少圈 4：反转多少圈  5:正转到指定位置 6:反转到指定位置
		uint8_t Speed : 4; // 速度 1~5
		uint8_t Circle_L8; // 圈数低8位 0~x State 3 4相对位置 ； 5 6 绝对位置
		uint8_t Circle_H8; // 圈数高8位 0~x State 3 4相对位置 ； 5 6 绝对位置
	} Knead;

	// 拍打电机
	struct
	{
		uint8_t Speed; // 速度1~x
	} Tap;

	// 伸缩电机
	struct
	{
		uint8_t State : 4; // 0:停止 1：伸 2：缩 3：伸多少圈 4：缩多少圈 5:指定位置
		uint8_t Speed : 4; // 速度1~x
		uint8_t Circle_L8; // 圈数低8位 0~x  当State ：3 4 相对位置  ； 5：绝对位置（缩到底零点）
		uint8_t Circle_H8; // 圈数高8位 0~x  当State ：3 4 相对位置  ； 5：绝对位置（缩到底零点）
	} Flex;

	// struct
	// {
	// 	uint8_t Ex_Date; //* 上下机芯交换数据
	// } Exchange;

} MoveUpDataRx_S;

// 机芯发送数据 上

typedef struct
{
	// 行走电机
	struct
	{
		uint8_t State : 8;	  //	0:停止 1：上行  2；下行  3.完成当前动作    （主板发送获取命令，等待下一条驱动命令更新）  
		uint8_t Place : 8;	  // 当前位置 0：导轨中 1：上限位 2：下限位 。。。			0x08：肩位检测成功   0x10：肩位检测失败  //todo 4->8
		uint8_t CurCircle_L8; // 当前圈数 低8位
		uint8_t CurCircle_H8; // 当前圈数 高8位
		uint8_t Current_L8;	  // 电流低8位 mA
		uint8_t Current_H8;	  // 电流高8位 mA
	} Tread;
	// 揉捏电机
	struct
	{

		uint8_t State;		  // 0:停止 1：正转中 2：反转中 3.完成当前动作    （主板发送获取命令，等待下一条驱动命令更新）
		uint8_t CurCircle_L8; // 当前圈数 低8位
		uint8_t CurCircle_H8; // 当前圈数 高8位
		uint8_t Current_L8;	  // 电流低8位 mA
		uint8_t Current_H8;	  // 电流高8位 mA

	} Knead;

	// 拍打电机
	struct
	{
		//					uint8_t State;  //0:停止  3.完成当前动作    （主板发送获取命令，等待下一条驱动命令更新）
		uint8_t Current_L8; // 电流低8位 mA
		uint8_t Current_H8; // 电流高8位 mA
	} Tap;

	// 伸缩电机
	struct
	{

		uint8_t State : 4;	  // 0:停止 1：伸出 2：回缩 3：完成       （主板发送获取命令，等待下一条驱动命令更新）
		uint8_t Place : 4;	  // 当前位置 0：有伸出 1：伸到顶 2：缩到底 。。。
		uint8_t CurCircle_L8; // 当前圈数 低8位
		uint8_t CurCircle_H8; // 当前圈数 高8位
		uint8_t Current_L8;	  // 电流低8位 mA
		uint8_t Current_H8;	  // 电流高8位 mA

	} Flex;

	// 32位 每个bit代表其中一个错误
	uint8_t ErrorCode[4];

} MoveUpDataTx_S;

// 机芯接收数据 下

typedef struct
{

	uint8_t UserMode; // 0:part0有效 1：part1有效

	/******part0*******/
	uint8_t Mode;		 // 0;无效 1:定点 2：局部  3：全身   4：上半身  5：下半身
	uint8_t PartParamL8; // 局部时 参数
	uint8_t PartParamH8; // 局部时 参数
	uint8_t Wise;		 // 常规手法 (0:停止 1:正揉  2:反揉  3 拍打  4敲击（间隙拍打） 揉拍  指压1  6 指压2（二段式）   10上揉搓 11 下揉搓 12 上揉搓配合+行走（肩胛骨按摩） 13 下揉搓配合+行走（肩胛骨按摩）....
	uint8_t Speed : 4;	 // (0：默认 1~ x 档)
	uint8_t Width : 4;	 // (0：默认 1 ~ x 档)
						 //		uint8_t Walk;  //0:停止 1：上 2：下 3：上行多少圈 4：下行多少圈 5:指定位置 。。。 手法带行走动作该字节无效
						 //		uint8_t WalkParamL8; // 圈数  当State ：3 4 相对位置  ； 5：绝对位置（上限位零点）
						 //		uint8_t WalkParamH8; // 圈数  当State ：3 4 相对位置  ； 5：绝对位置（上限位零点）

	/*********part1*********/

	// 手法关联电机数据无效

	// 行走电机
	struct
	{
		uint8_t State : 4; // 0:停止 1：上 2：下 3：上行多少圈 4：下行多少圈 5:指定位置
		uint8_t Speed : 4; // 速度1~5
		uint8_t Circle_L8; // 圈数  当State ：3 4 相对位置  ； 5：绝对位置（上限位零点）
		uint8_t Circle_H8; // 圈数  当State ：3 4 相对位置  ； 5：绝对位置（上限位零点）

	} Tread;

	// 揉捏电机
	struct
	{

		uint8_t State : 4; // 0:停止 1：正转 2：反转 3：正转多少圈 4：反转多少圈  5:正转到指定位置 6:反转到指定位置 7:自动手法
		uint8_t Speed : 4; // 速度 1~5
		uint8_t Circle_L8; // 圈数低8位 0~x State 3 4相对位置 ； 5 6 绝对位置
		uint8_t Circle_H8; // 圈数高8位 0~x State 3 4相对位置 ； 5 6 绝对位置
	} Knead;

	// 拍打电机
	struct
	{
		uint8_t Speed; // 速度1~x
	} Tap;

	// struct
	// {
	// 	uint8_t Ex_Date; //* 上下机芯交换数据
	// } Exchange;
} MoveDwDataRx_S;

// 机芯发送数据 下

typedef struct
{

	// 行走电机
	struct
	{
		uint8_t State : 8;	  //	0:停止 1：上行  2；下行  3.完成当前动作     （主板发送获取命令，等待下一条驱动命令更新）
		uint8_t Place : 8;	  // 当前位置 0：导轨中 1：上限位 2：下限位 。。。
		uint8_t CurCircle_L8; // 当前圈数 低8位
		uint8_t CurCircle_H8; // 当前圈数 高8位
		uint8_t Current_L8;	  // 电流低8位 mA
		uint8_t Current_H8;	  // 电流高8位 mA
	} Tread;
	// 揉捏电机
	struct
	{

		uint8_t State;		  // 0:停止 1：正转中 2：反转中 3.完成当前动作     （主板发送获取命令，等待下一条驱动命令更新）
		uint8_t CurCircle_L8; // 当前圈数 低8位
		uint8_t CurCircle_H8; // 当前圈数 高8位
		uint8_t Current_L8;	  // 电流低8位 mA
		uint8_t Current_H8;	  // 电流高8位 mA

	} Knead;

	// 拍打电机
	struct
	{
		uint8_t State;		// 0:停止  3.完成当前动作    （主板发送获取命令，等待下一条驱动命令更新）
		uint8_t Current_L8; // 电流低8位 mA
		uint8_t Current_H8; // 电流高8位 mA
	} Tap;

	// 32位 每个bit代表其中一个错误
	uint8_t ErrorCode[4];

} MoveDwDataTx_S;

// 小腿接收数据

typedef struct
{

	// 行走电机
	struct
	{
		uint8_t State : 4; /// 0:停止 1：正转 2：反转  预留（3；自动1  4自动2  。。。。）
		uint8_t Speed : 4; // 速度1~5

	} Roll;

	// 揉捏电机
	struct
	{

		uint8_t State : 4; // 0:停止 1：正转 2：反转 预留（3；自动1  4自动2  。。。。）
		uint8_t Speed : 4; // 速度 1~5

	} Knead1;

	// 滚刮电机
	struct
	{

		uint8_t State : 4; // 0:停止 1：正转 2：反转 预留（3；自动1  4自动2  。。。。）
		uint8_t Speed : 4; // 速度 1~5

	} Knead2;

	// 加热
	struct
	{

		uint8_t State : 4; // 0:停止 1：加热  预留（2；自动1  3自动2  。。。。）
		uint8_t Level : 4; // 档位 1~5

	} Heat;

	// 气阀
	union
	{

		u8 Valves;
		struct
		{
			uint8_t Gas1 : 1; //
			uint8_t Gas2 : 1; //
			uint8_t Gas3 : 1; //
			uint8_t Gas4 : 1; //
			uint8_t Gas5 : 1; //
			uint8_t Gas6 : 1; //
			uint8_t Gas7 : 1; //
			uint8_t Gas8 : 1; //
		} Valve;

	} AirValve;

	// 伸缩电机
	struct
	{
		uint8_t State : 4; // 0:停止 1：伸 2：缩 3：伸多少圈 4：缩多少圈 5:指定位置 6：找脚(最大速度)
		uint8_t Speed : 4; // State 1~5 ：速度1~x	 6：找脚补偿量 单位:s
		uint8_t Circle_L8; // 圈数低8位 0~x  当State ：3 4 相对位置  ； 5：绝对位置（缩到底零点） 6 无效
		uint8_t Circle_H8; // 圈数高8位 0~x  当State ：3 4 相对位置  ； 5：绝对位置（缩到底零点） 6 无效
	} Flex;

} LegDataTx_S;

// 小腿发送数据

typedef struct
{

	// 滚轮电机
	struct
	{

		uint8_t State;		// 0:停止 1：正转中 2：反转中 3.完成当前动作  （主板发送获取命令）
		uint8_t Current_L8; // 电流低8位 mA
		uint8_t Current_H8; // 电流高8位 mA

	} Roll;

	// 揉搓电机
	struct
	{
		uint8_t State;		// 0:停止 1：正转中 2：反转中 3.完成当前动作   （主板发送获取命令）
		uint8_t Current_L8; // 电流低8位 mA
		uint8_t Current_H8; // 电流高8位 mA
	} Knead1;

	// 滚刮电机
	struct
	{
		uint8_t State;		// 0:停止 1：正转中 2：反转中 3.完成当前动作   （主板发送获取命令）
		uint8_t Current_L8; // 电流低8位 mA
		uint8_t Current_H8; // 电流高8位 mA
	} Knead2;

	// 加热
	struct
	{
		uint8_t State;		// 0:停止 1：加热中 2：加热中 3.完成当前动作   （主板发送获取命令）
		uint8_t Current_L8; // 电流低8位 mA
		uint8_t Current_H8; // 电流高8位 mA
	} Heat;

	// 伸缩电机
	struct
	{

		uint8_t State : 4;	  // 0:停止 1：伸出 2：回缩 3：完成    （主板发送获取命令）
		uint8_t Place : 4;	  // 当前位置 0：有伸出 1：伸到顶 2：缩到底 。。。
		uint8_t CurCircle_L8; // 当前圈数 低8位
		uint8_t CurCircle_H8; // 当前圈数 高8位
		uint8_t Current_L8;	  // 电流低8位 mA
		uint8_t Current_H8;	  // 电流高8位 mA

	} Flex;

	// 加热
	struct
	{
		uint8_t Top : 1;	 // 上限位
		uint8_t Bot : 1;	 // 下限位
		uint8_t Foot : 1;	 // 找脚
		uint8_t Touch : 1;	 // 触地
		uint8_t Protect : 1; // 防夹
		uint8_t Res : 3;
	} Singal;

	// 32位 每个bit代表其中一个错误
	uint8_t ErrorCode[4];

} LegDataRx_S;

// 扶手按键接收数据

typedef struct
{

	uint8_t LedMode;  // 0：灭 ;  1: 常亮   2：亮 Ledpara1x0.1s 灭 Ledpara2x0.1s
	uint8_t Ledpara1; // 参数1	 	LedMode为2有效
	uint8_t Ledpara2; // 参数2		LedMode为2有效

} ArmKeyRx_S;

// 扶手按键发送数据  200ms一次主动发送

typedef struct
{

	uint8_t State; // 按键状态 0: 空闲 1：按下  2：松开
	uint8_t Value; // 按键键值

} ArmKeyTx_S;

#define _485_Com_Head0 0x55
#define _485_Com_Head1 0xAA

u16 CRC16_CCITT_FALSE(u8 *puchMsg, u32 usDataLen);
void _485_ComTx(u8 taraddr, u8 souraddr, u8 count, u16 cmd, u16 len, u8 *data);

#endif
