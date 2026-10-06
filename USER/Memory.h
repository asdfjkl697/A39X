#ifndef MEMORY_H
#define MEMORY_H

#include "config.h"
#include "main.h"

#define Max_Save_world sizeof(Save_struct) / sizeof(u16) // 当个存储字节数
#define Max_Save 6										 // @  最大存储个数 4->6 增加两个给自定义

#define Flash_Check_Time 150 // x * 100ms
extern uint16_t Flash_Check_Tick;

extern u8 DIY_Save_Flag;
extern bool Manual_Save_Flag;
extern u32 Manual_Save_tick;
extern u8 Manual_Save_AirState;
extern u8 Manual_Save_AirIntensity;
extern bool LoadSave_Flag;

typedef struct
{
	uint8_t MemoryBackHeat;
	uint8_t MemoryLegHeat;
	uint8_t MemoryShoulderHeat;
	uint8_t MemoryArmHeat;

	uint8_t MemoryBackHeatLevel;
	uint8_t MemoryLegHeatLevel;
	uint8_t MemoryShoulderHeatLevel;
	uint8_t MemoryArmHeatLevel;
} Save_Heat_t;

typedef struct
{
	u16 Save_ZeroState;
	u16 Save_BDDGCircle;
	u16 Save_LDDGCircle;
	u16 Save_AutoMode;
	u16 Save_Area;
	u16 Save_Wise;
	u16 Save_Speed;
	u16 Save_Width;
	u16 Save_Wise_M2;
	u16 Save_Speed_M2;
	u16 Save_Width_M2;
	u16 Save_Area_M2;
	u16 Save_BackPos_M2;
	u16 Save_AirState;
	u16 Save_AirIntensity;
	u16 Save_Roll;
	u16 Save_RollSpeed;
	u16 Save_LegKnead;
	u16 Save_LegKneadSpeed;
	Save_Heat_t Save_Heat;
	u16 Save_BackPos;
	u16 Save_Shouler;
	u16 Save_BackIntensity;
	u16 Save_AutoIntensity;
	u16 Save_Check;
} Save_struct;

typedef union
{
	Save_struct Save_struct_Value;
	u16 Save_Value[Max_Save_world];
} Union_Save;

#ifdef PART_USE_SUM_TIME
extern void Flash_PART_USE_read(void);
extern void PartUse_Process_S(void);
extern void PartUse_Process_Min(void);
#endif

extern void Flash_Parameter_read(void);
extern void Flash_Parameter_Write(void);
extern void memory_read(void);
extern void memory_write(void);
extern u8 HM_GetSaveMode(u8 nuser);
extern u8 HM_SaveMode(void);
extern u8 HM_DeleteSaveMode(u8 x);
extern u8 HM_DeleteSaveAll(void);
extern u8 User_Memory_Save(u8 x);
extern u8 User_Memory_Use(u8 x);
extern void User_Memory_Use_DIY(void);
#endif
