#include "CalfKnead.h"

#ifdef Debug_Mode
CalfKneadInfo_S Knead;
#else
static CalfKneadInfo_S Knead;
#endif

static KneadAutoTable_S Table[] =
	{

		{1, 1, 30},
		{1, 1, 30},
		{1, 2, 30},
		{1, 2, 30},
		{0, 0, 20},
		{1, 3, 50},
		{1, 3, 50},
		{1, 4, 50},
		{1, 4, 50},
		{1, 5, 50},
		{1, 5, 50},
		{0, 0, 40},
		{1, 4, 30},
		{1, 3, 30},
		{1, 4, 30},
		{1, 5, 30},
		{0, 0, 20},
		{1, 5, 30},
		{1, 4, 30},
		{1, 3, 30},
		{1, 4, 30},
		{1, 5, 30},
		{0, 0, 30},

};

void CalfKneadProcess(void)
{

	//		if(Knead.CurMode==Knead.PreMode)
	//			return;

	Knead.PreMode = Knead.CurMode;

	switch (Knead.CurMode)
	{
	case 0: // 关闭
		KneadStateSet(0);
		break;

	default:
	case 1: // 常开
		KneadStateSet(1);
		break;

	case 2: // 自动
		switch (Knead.Step)
		{
		case 1:
			KneadStateSet(Table[Knead.Count].Dir);
			KneadSpeedSet(Table[Knead.Count].Speed);
			TICK_RESET(Knead.Tick);
			Knead.Step++;
			break;

		case 2:
			if (TICK_COUNTER(Knead.Tick, Table[Knead.Count].Time))
			{
				if (++Knead.Count >= 8)
					Knead.Count = 0;
				TICK_RESET(Knead.Tick);
				Knead.Step = 1;
			}
			break;

		default:
			Knead.Step = 0;
			Knead.Count = 0;
			break;
		}
		break;
	}

	if (KeyPush.KeyCalfKnead) // todo 手动腿部揉捏与腿部气压关联补丁 260625
	{
		// if (AirT_Work.RunFlag[AirT_CalfSide] == 1)
		if (AirT_Work.RunFlag[AirT_CalfSide] == 1 && KneadModeGet())
		{
			KneadSpeedSet(1);
		}
		else
		{
			KneadSpeedSet(0);
		}
	}
}

void KneadModeSet(u8 mode)
{
	Knead.CurMode = mode;
}

uint8_t KneadModeGet(void)
{
	return Knead.CurMode;
}

void KneadSpeedSet(u8 spd)
{
#if (LEG_COM == 0 && ROLLER_CALVES_EN)
	switch (spd)
	{
	case 0:
		CalfKnead_Pwm(0);
		break;
	case 1:
		CalfKnead_Pwm(100);
		break;
	default:
		break;
	}
#endif
	Knead.Speed = spd;
}

u8 KneadSpeedGet(void)
{
	return Knead.Speed;
}

void KneadStateSet(u8 sta)
{
	Knead.State = sta;
}

u8 KneadStateGet(void)
{
	return Knead.State;
}
