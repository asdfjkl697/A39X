#ifndef MOVEBOTH_H
#define MOVEBOTH_H

#include "config.h"
#include "_485Com.h"
#include "MassageInit.h"
#include <string.h>

// #define MOVEUPWIDTH1 35
// #define MOVEUPWIDTH2 45
// #define MOVEUPWIDTH3 52
// #define MOVEUPWIDTH4 57
// #define MOVEUPWIDTH5 65

#define MOVEUPWIDTH1 40 // 6
#define MOVEUPWIDTH2 49 // 7
#define MOVEUPWIDTH3 55 // 8
#define MOVEUPWIDTH4 61 // 9
#define MOVEUPWIDTH5 3	// A = 0
#define MOVEUPWIDTH6 12 // 1
#define MOVEUPWIDTH7 18 // 2
#define MOVEUPWIDTH8 23 // 3
#define MOVEUPWIDTH9 29 // 4
#define MOVEUPWIDTHA 34 // 5

// #define MOVEDWWIDTH1 34
// #define MOVEDWWIDTH2 43
// #define MOVEDWWIDTH3 47
// #define MOVEDWWIDTH4 51
// #define MOVEDWWIDTH5 57

#define MOVEDWWIDTH1 34
#define MOVEDWWIDTH2 43
#define MOVEDWWIDTH3 50
#define MOVEDWWIDTH4 57
#define MOVEDWWIDTH5 63
#define MOVEDWWIDTH6 6
#define MOVEDWWIDTH7 12
#define MOVEDWWIDTH8 18
#define MOVEDWWIDTH9 24
#define MOVEDWWIDTHA 30

typedef enum
{
	TREAD_STOP,						// 行走停止, para1,para2, speed无效
	TREAD_FWD,						// 行走上行, para1,para2无效, speed有效
	TREAD_REV,						// 行走下行, para1,para2无效，speed有效
	TREAD_FWD_CIRCLE,				// 以当前位置上行指定圈数，para1, para2合并圈数, speed有效
	TREAD_REV_CIRCLE,				// 以当前位置下行指定圈数，para1, para2合并圈数, speed有效
	TREAD_FIX_CIRCLE,				// 行走指定位置，para1, para2合并圈数, speed有效
	TREAD_FIX_RATIO,				// 行走指定位置，para1为百分比, para2无效, speed有效
	TREAD_BACK_AND_FORTH_CUR_POINT, // 以当前位置为中点往复走，para1, para2合并圈数, para1为低字节，para2为高字节
	TREAD_BACK_AND_FORTH,			// 按照设置的上下点往复走, para1:上行到位位置（百分比），para2:下行到位位置（百分比），speed有效
									// 先走到上行设定位置，再走到下行设定位置
	TREAD_FOLLOW_KNEAD1,			// 跟随揉捏，揉捏正转，行走向上走
	TREAD_FOLLOW_KNEAD2,			// 跟随揉捏，揉捏反转，行走向上走
	TREAD_FOLLOW_FLEX1,				// 跟随伸缩，伸缩顶出，行走向上走
	TREAD_FOLLOW_FLEX2,				// 跟随伸缩，伸缩顶出，行走向下走
	TREAD_FIND_SDR,					// 找肩位
	TREAD_MOVE_SDR,					// 移动到肩位
	TREAD_END
} TreadCmd;

typedef enum
{
	KNEAD_STOP,		  // 揉捏停⽌
	KNEAD_FWD,		  // 揉捏正转，para1 para2⽆效，speed有效,
	KNEAD_FWD_CLK,	  // 揉捏正转，时间模式，para1为时间(100ms)
	KNEAD_FWD_CNT,	  // 揉捏正转，次数模式，para1为次数
	KNEAD_FWD_ABS,	  // 揉捏正转，绝对模式，para1为绝对位置
	KNEAD_REV,		  // 揉捏反转，para1 para2⽆效，speed有效,
	KNEAD_REV_CLK,	  // 揉捏反转，时间模式，para1为时间(100ms)
	KNEAD_REV_CNT,	  // 揉捏反转，次数模式，para1为次数
	KNEAD_REV_ABS,	  // 揉捏反转，绝对模式，para1为绝对位置
	KNEAD_FIX_CIRCLE, // 指定位置，para1:指定⽬标位置, para2⽆效，speed有效
	KNEAD_BACK_FORTH, // 往复运动，以当前位置为起始位置，para1为起始位置到结束位置增量，
					  // para2为次数，0xFF为持续循环，speed⾼4位，低4位
	KNEAD_1F2R,		  // 2点之间往复运动，start先正转到end，再反转回start
					  // start_point为para1低4位，end_point为para1⾼4位，para2为次数，
					  // speed低4位为start到end的速度，⾼4位为end到start的速度
	KNEAD_1R2F,		  // 2点之间往复运动，start先反转到end，再正转回start
					  // start_point为para1低4位，end_point为para1⾼4位，para2为次数，
					  // speed低4位为start到end的速度，⾼4位为end到start的速度
	KNEAD_1F2F,		  // 2点之间往复运动，start先正转到end，再正转回start，停留200ms
					  // start_point为para1低4位，end_point为para1⾼4位，para2为次数，
					  // speed低4位为start到end的速度，⾼4位为end到start的速度
	KNEAD_1R2R,		  // 2点之间往复运动，start先反转到end，再反转回start，停留200ms
					  // start_point为para1低4位，end_point为para1⾼4位，para2为次数，
					  // speed低4位为start到end的速度，⾼4位为end到start的速度
	KNEAD_END

} KneadCmd;

typedef enum
{
	FLEX_STOP,			// 伸缩停止, para1,para2,speed无效
	FLEX_FWD,			// 伸缩正转, para1,para2,speed有效
	FLEX_REV,			// 伸缩反转, para1,para2,speed有效
	FLEX_FWD_CIRCLE,	// 以当前位置正转指定圈数，para1, para2合并圈数, speed有效
	FLEX_REV_CIRCLE,	// 以当前位置反转指定圈数，para1, para2合并圈数, speed有效
	FLEX_FIX_CIRCLE,	// 伸缩指定位置，para1, para2合并圈数, speed有效
	FLEX_BACK_FORTH,	// 顶出伸缩，para1为第一段顶出位置，停留0.5s，speed有效
	FLEX_BACK_FORTH2,	// 二段式顶出，para1第一段顶出位置，para2为第二段顶出位置，停留0.5s，speed有效
	FLEX_FOLLOW_KNEAD1, // 跟随揉捏，揉捏正转，伸缩顶出
	FLEX_FOLLOW_KNEAD2, // 跟随揉捏，揉捏反转，伸缩顶出
	FLEX_FOLLOW_KNEAD3, // 内圈顶出
	FLEX_FOLLOW_TREAD1, // 跟随行走，行走向上走，伸缩顶出
	FLEX_FOLLOW_TREAD2, // 跟随行走，行走向下走，伸缩顶出
	FLEX_END
} FlexCmd;

typedef enum
{
	FLAP_STOP,	  // 拍打停⽌
	FLAP_ACT,	  // 持续拍打，para1:0⽆限模式，para2⽆效，speed有效,
	FLAP_ACT_CLK, // para1为时间(100ms)
	FLAP_BS,	  // 变速拍打
} FlapCmd;

typedef enum
{
	FOLW_STOP = 0,	//* 跟随停⽌
	FOLW_COPY,		//* 复制动作
	FOLW_SPACE,		//* 距离跟随
	FOLW_WIDTH,		//* 设定宽度
	FOLW_FIX2KNEAD, //* 设定揉捏
} MoveFollowCmd;

// 主机参数
typedef struct MasterMotorMsg
{
	uint8_t Update;	  // 更新标志
	uint8_t Method;	  // 方法
	uint8_t Para1;	  // 参数1
	uint8_t Para2;	  // 参数2
	uint8_t Speed;	  // 速度
} master_motor_msg_t; // 主机参数

// 从机参数
typedef struct SlaveMotorMsg
{
	uint8_t Finish;	  // 完成标志
	uint8_t Res;	  // 预留
	uint8_t Signal;	  // 限位信号
	uint8_t CircleL;  // 当前位置低字节
	uint8_t CircleH;  // 当前位置高字节
	uint8_t CurrentL; // 电流低字节
	uint8_t CurrentH; // 电流高字节
} slave_motor_msg_t;

// 机芯接收数据 上
typedef struct
{
	uint8_t UserMode; //* 协议版本 0:part0有效 1：part1有效 2:测试协议

	master_motor_msg_t Tread; // 行走电机

	master_motor_msg_t Knead; // 揉捏电机

	master_motor_msg_t Flex; // 伸缩电机

	master_motor_msg_t Tap; // 拍打电机

	uint8_t Res[10]; // 预留

} MSG2Move_S;

// 机芯接收数据 回
typedef struct
{
	slave_motor_msg_t Tread; // 行走电机

	slave_motor_msg_t Knead; // 揉捏电机

	slave_motor_msg_t Flex; // 伸缩电机

	slave_motor_msg_t Tap; // 拍打电机

	uint8_t ErrorCode[4]; // 32位 每个bit代表其中一个错误

	uint8_t Res[6]; // * 预留
} MSG4Move_R;

typedef struct
{
	uint8_t Tread_p : 1;
	uint8_t Knead_p : 1;
	uint8_t Flex_p : 1;
	uint8_t Tap_p : 1;
	uint8_t Speed : 1;
	uint8_t Width : 1;
	uint8_t Inten : 1;
	uint8_t Wise;
	uint8_t Area;
} Move_Flag_T;

typedef struct
{
	uint8_t Reset : 1;
	uint8_t BodyCheck : 1;
	uint8_t DwFinish : 1;
	uint8_t HealthPrepare : 1;
} Massage_Flag_T;

typedef struct
{
	uint8_t Tread_cmd;
	uint8_t Knead_cmd;
	uint8_t Tap_cmd;
	uint8_t Flex_cmd;
} Move_Follow_Cmd_T;

extern MSG2Move_S MSG2Move[2];
extern MSG4Move_R MSG4Move[2];
extern Move_Flag_T Move_Flag[2];
extern Massage_Flag_T Massage_Flag;
extern Move_Follow_Cmd_T MoveFolw_Cmd;
extern u16 Move_Follow_Space;

MSG4Move_R *GetMoveDataTx(u8 whi);
extern void MSGMoveInit(void);
extern u16 MoveCurTreadCircleGet(u8 whi);

extern void MoveUp_Manual_Pause(void);
extern void MoveUp_Manual_Deal(void);
extern void MoveUp_Updata_Clear(void);
extern void MoveUp_Updata_Set(u8 code);
#ifdef USE_M2
extern void MoveDw_Manual_Pause(void);
extern void MoveDw_Updata_Clear(void);
extern void MoveDw_Updata_Set(u8 code);
#endif
extern void MSG2MoveUp_Deal(void);

extern void MSG2MoveDw_Deal(void);

extern void Part_Date_Set(u8 part, u8 update, u8 method, u8 para1, u8 para2, u8 speed);

extern void CoreRest(void);

#endif
