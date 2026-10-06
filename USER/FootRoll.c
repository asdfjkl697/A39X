#include "FootRoll.h"

#ifdef Debug_Mode
FootRollInfo_S Roll;
#else
static FootRollInfo_S Roll;
#endif

static RollAutoTable_S Table[8] =
	{

		{1, 50},
		{0, 10},
		{2, 50},
		{0, 10},
		{1, 100},
		{0, 20},
		{2, 100},
		{0, 20},

};

void FootRollProcess(void)
{
	//		if(Roll.CurMode==Roll.PreMode) return;

	Roll.PreMode = Roll.CurMode;
	switch (Roll.CurMode)
	{
	case 0: // 关闭
		RollStateSet(0);
		break;

	default:
	case 1: // 常开
		RollStateSet(1);
		break;

	case 2: // 自动
		switch (Roll.Step)
		{
		case 1:
			RollStateSet(Table[Roll.Count].Dir);
			TICK_RESET(Roll.Tick);
			Roll.Step++;
			break;

		case 2:
			if (TICK_COUNTER(Roll.Tick, Table[Roll.Count].Time))
			{
				if (++Roll.Count >= 8)
					Roll.Count = 0;
				TICK_RESET(Roll.Tick);
				Roll.Step = 1;
			}
			break;

		default:
			Roll.Step = 0;
			Roll.Count = 0;
			break;
		}
		break;
	}
}

void RollModeSet(u8 mode)
{
	Roll.CurMode = mode;
}

uint8_t RollModeGet(void)
{
	return Roll.CurMode;
}

void RollSpeedSet(u8 spd)
{
#if (LEG_COM == 0 && ROLLER_FOOT_EN)
	switch (spd)
	{
	case 0:
		FootRoll_PWM_Disable();
		break;
	case 1:
		FootRoll_PWM_Enable(40);
		break;
	case 2:
		FootRoll_PWM_Enable(80);
		break;
	case 3:
		FootRoll_PWM_Enable(120);
		break;
	default:
		break;
	}
#endif
	Roll.Speed = spd;
}

u8 RollSpeedGet(void)
{
	return Roll.Speed;
}

void RollStateSet(u8 sta)
{
	Roll.State = sta;
}

u8 RollStateGet(void)
{
	return Roll.State;
}
