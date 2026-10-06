#include "Motor.h"
#include "MassageLogic.h"

DDGMotor_Struct DDGVariable;

u8 LDDGOverLoadFlag, BDDGOverLoadFlag;
u8 nCurZeroState = 0;
u8 LegVoid = 0, LegAutoAdj = 0;
bool LegAdj_Flex_Flag = 0; // * 增加 250505
bool SIT_NONE_Flag = 0;	   // * 增加 复位动作 250616

const u8 ZeroRank[Max_ZeroRank][2] = {
	{DDG_BCircle_Zero0, DDG_LCircle_Zero0}, // 0零重力0
	{DDG_BCircle_Zero1, DDG_LCircle_Zero1}, // 1零重力1
	{DDG_BCircle_Zero2, DDG_LCircle_Zero2}, // 2零重力2
	{DDG_BCircle_Zero3, DDG_LCircle_Zero3}, // 3零重力3

	{DDG_BCircle_ST0, DDG_LCircle_ST0}, // 4拉伸 全身-准备
	{DDG_BCircle_ST1, DDG_LCircle_ST1}, // 5拉伸 全身-拉伸
	{DDG_BCircle_ST2, DDG_LCircle_ST2}, // 6拉伸 小腿-准备
	{DDG_BCircle_ST3, DDG_LCircle_ST3}, // 7拉伸 小腿-拉伸
	{DDG_BCircle_ST4, DDG_LCircle_ST4}, // 8拉伸 全身-拉伸2
	{DDG_BCircle_ST5, DDG_LCircle_ST5}, // 9打包

	{DDG_BCircle_ST6, DDG_LCircle_ST6}, // 零重力1
	{DDG_BCircle_ST7, DDG_LCircle_ST7}, // 零重力2
	{DDG_BCircle_ST8, DDG_LCircle_ST8}, // 零重力3

	{DDG_BCircle_DB, DDG_LCircle_DB}, // 3零重力3
};

#ifdef DDG_Type_Signal1
void DDG_Set_Stop(void)
{
	if (DDGVariable.UserState != DDG_UserState_Stop)
	{
		DDGVariable.UserState = DDG_UserState_Stop;
		nCurZeroState = 0x80;
#if (LEG_COM)
		LegVoid = 0; // 小腿自动避让
#endif				 // LEG_COM
	}
}

void DDG_Set_BackUp(void)
{
	DDGVariable.UserState = DDG_UserState_BackUp;
	nCurZeroState = 0x80;

#if (LEG_COM)
	LegAutoAdj = 1;
	LegVoid = 1; // 小腿自动避让
#endif			 // LEG_COM
}

void DDG_Set_BackDown(void)
{
	DDGVariable.UserState = DDG_UserState_BackDown;
	nCurZeroState = 0x80;

#if (LEG_COM)
	LegAutoAdj = 1;
	LegVoid = 0; // 小腿自动避让
#endif			 // LEG_COM
}

void DDG_Set_BackAction(uint8_t act)
{
	if (1 == act)
	{
		DDG_Set_BackUp();
	}
	else if (2 == act)
	{
		DDG_Set_BackDown();
	}
	else
	{
		DDG_Set_Stop();
	}
}

void DDG_Set_LegUp(void) //@ 往下
{
	DDGVariable.UserState = DDG_UserState_LegUp;
	nCurZeroState = 0x80;
#if (LEG_COM)
	LegAutoAdj = 1;
	LegVoid = 1; // 小腿自动避让
#endif			 // LEG_COM
}

void DDG_Set_LegDown(void) //@ 往上
{
	DDGVariable.UserState = DDG_UserState_LegDown;
	nCurZeroState = 0x80;
#if (LEG_COM)
	LegAutoAdj = 1;
	LegVoid = 0; // 小腿自动避让
#endif			 // LEG_COM
}

void DDG_Set_LegAction(uint8_t act)
{
	if (1 == act)
	{
		DDG_Set_LegUp();
	}
	else if (2 == act)
	{
		DDG_Set_LegDown();
	}
	else
	{
		DDG_Set_Stop();
	}
}

void DDG_Set_SitUp(void)
{
	DDGVariable.UserState = DDG_UserState_SitUp;
	nCurZeroState = 0x80;

#if (LEG_COM)
	LegAutoAdj = 1;
	LegVoid = 1; // 小腿自动避让
#endif			 // LEG_COM
}

void DDG_Set_SitDown(void)
{
	DDGVariable.UserState = DDG_UserState_SitDown;
	nCurZeroState = 0x80;

#if (LEG_COM)
	LegAutoAdj = 1;
	LegVoid = 0; // 小腿自动避让
#endif			 // LEG_COM
}

void DDG_Set_SitUpCircle(u8 x1, u8 x2)
{
	u32 temp;
	DDGVariable.UserState = DDG_UserState_SitUp_Circle;
	DDGVariable.BnLocalCircle = DDG_BCircle_Max * x1 / 100;

	temp = x2;
	temp *= DDG_LCircle_Max;
	temp /= 100;
	DDGVariable.LnLocalCircle = temp;

	nCurZeroState = 0x80;
#if (LEG_COM)
	LegAutoAdj = 1;
	LegVoid = 1; // 小腿自动避让
#endif			 // LEG_COM
}

void DDG_Set_SitDownCircle(u8 x1, u8 x2)
{
	u32 temp;
	DDGVariable.UserState = DDG_UserState_SitDown_Circle;
	DDGVariable.BnLocalCircle = DDG_BCircle_Max * x1 / 100;

	temp = x2;
	temp *= DDG_LCircle_Max;
	temp /= 100;
	DDGVariable.LnLocalCircle = temp;

	nCurZeroState = 0x80;

#if (LEG_COM)
	LegAutoAdj = 1;
	LegVoid = 0; // 小腿自动避让
#endif			 // LEG_COM
}

void DDG_Set_StretchUpC(u8 x1, u8 x2)
{

	u32 temp;
	DDGVariable.UserState = DDG_UserState_StretchUp;
	DDGVariable.BnLocalCircle = DDG_BCircle_Max * x1 / 100;

	temp = x2;
	temp *= DDG_LCircle_Max;
	temp /= 100;
	DDGVariable.LnLocalCircle = temp;

	//	DDGVariable.LnLocalCircle = DDG_LCircle_Max * x2 / 100;
	nCurZeroState = 0x80;

#if (LEG_COM)
	LegAutoAdj = 0;
	LegVoid = 1; // * 修改 0->1 小腿自动避让
#endif			 // LEG_COM
}

void DDG_Set_StretchDownC(u8 x1, u8 x2)
{
	u32 temp;

	DDGVariable.UserState = DDG_UserState_StretchDown;
	DDGVariable.BnLocalCircle = DDG_BCircle_Max * x1 / 100;
	//	DDGVariable.LnLocalCircle = DDG_LCircle_Max * x2 / 100;

	temp = x2;
	temp *= DDG_LCircle_Max;
	temp /= 100;
	DDGVariable.LnLocalCircle = temp;

	nCurZeroState = 0x80;

#if (LEG_COM)
	LegAutoAdj = 0;
	LegVoid = 0; // 小腿自动避让
#endif			 // LEG_COM
}

void DDG_Set_SitCircle(u8 x1, u8 x2)
{

	u32 temp;
	DDGVariable.nCurUserState = 0xff;

	DDGVariable.UserState = DDG_UserState_Sit_Circle;
	DDGVariable.BCircleSet = DDG_BCircle_Max * x1 / 100;

	temp = x2;
	temp *= DDG_LCircle_Max;
	temp /= 100;
	DDGVariable.LCircleSet = temp;

	//	DDGVariable.LCircleSet = DDG_LCircle_Max * x2 / 100;
	nCurZeroState = 0x80;

#if (LEG_COM)
	// LegAutoAdj = 1; //@ 用于自动程序内 不找脚 250915
	LegVoid = 1; // 小腿自动避让
#endif			 // LEG_COM
}

void DDG_Set_Zero(u8 x)
{

	u32 temp;
	if (x == nCurZeroState || x >= Max_ZeroRank)
		return;

	if (x == AUTO_SIT_NONE)
	{
		DDGVariable.nCurUserState = 0xff;
		DDGVariable.UserState = DDG_UserState_SitUp;
		// DDGVariable.BCircleSet = 0; //* 屏蔽 250208
		// DDGVariable.LCircleSet = DDG_LCircle_Max; //* 修改
		SIT_NONE_Flag = 1; // * 增加 250616
		FootFlexStateSet(FlexUp);
		FootFlexSpeedSet(5);
	}
	else
	{
		DDGVariable.nCurUserState = 0xff;
		DDGVariable.UserState = DDG_UserState_Sit_Circle;
		DDGVariable.BCircleSet = DDG_BCircle_Max * (ZeroRank[x][0]) / 100;

		temp = ZeroRank[x][1]; // * 修改
		temp *= DDG_LCircle_Max;
		temp /= 100;
		DDGVariable.LCircleSet = temp;

		// DDGVariable.LCircleSet = DDG_LCircle_Max * (ZeroRank[x][1]) / 100;
	}

#if (LEG_COM)
	LegAutoAdj = 1;
#endif // LEG_COM

	if (x >= Max_Zero)
	{
		nCurZeroState = 0x80;
#if (LEG_COM)
		LegAutoAdj = 0; // * 拉伸不找脚 250224
		LegVoid = 0;	// 小腿自动避让
#endif					// LEG_COM
	}
	else
	{
		nCurZeroState = x;
#if (LEG_COM)
		LegVoid = 1; // 小腿自动避让
#endif				 // LEG_COM
	}
}

u16 nPreLDDGCircle = 0, nPreBDDGCircle = 0;
#ifdef Debug_Mode
u16 BDD_Switch_time = 0, LDD_Switch_time = 0;
#else
static u16 BDD_Switch_time = 0, LDD_Switch_time = 0;
#endif
static void DDG_Motor_Control(DDGMotor_Struct *nVariable)
{
#if (LEG_COM)
	u8 i;
#endif

	/* 小腿升降推杆触地动作 */
	if (GetLegDataRx()->Singal.Touch) /* 腿部信号接线混乱，后续修改后取消注释 */
	{
	}

	if (nVariable->BPlace == DDG_Place_Top)
	{
		if (nVariable->BNextMotorState == DDG_MotorState_Up)
			nVariable->BnLocalCircle = 0;

		nVariable->BnCurCircle = 0;

		if (nVariable->BSignalInit == 0)
			nVariable->BSignalInit = 1;
	}
	else if (nVariable->BPlace == DDG_Place_Bot)
	{
		nVariable->BnCurCircle = DDG_BCircle_Max;

		if (nVariable->BNextMotorState == DDG_MotorState_Down)
			nVariable->BnLocalCircle = 0;

		if (nVariable->BSignalInit == 0)
			nVariable->BSignalInit = 1;
	}

	if (nVariable->LPlace == DDG_Place_Top)
	{
		nVariable->LnCurCircle = 0;

		if (nVariable->LNextMotorState == DDG_MotorState_Up)
			nVariable->LnLocalCircle = 0;

		if (nVariable->LSignalInit == 0)
			nVariable->LSignalInit = 1;
	}
	else if (nVariable->LPlace == DDG_Place_Bot)
	{
		nVariable->LnCurCircle = DDG_LCircle_Max;

		if (nVariable->LNextMotorState == DDG_MotorState_Down)
			nVariable->LnLocalCircle = 0;

		if (nVariable->LSignalInit == 0)
			nVariable->LSignalInit = 1;
	}

	// 防夹处理
	//  if (Touth_State && nVariable->UserState)
	//  	nVariable->UserState = DDG_UserState_Stop;

#if (LEG_COM)
	if (LegActionSuccess == GetLegDataRx()->Flex.State) //* 小腿伸缩标志位 250504
	{
		MChair.Leg.FootFinding = 0;
		FootFlexStateSet(FlexStop);
		FootFlexSpeedSet(0);
	}
#endif

	switch (nVariable->UserState)
	{
	default:
		if (nVariable->nCurUserState != DDG_UserState_Stop)
		{
			nVariable->nCurUserState = DDG_UserState_Stop;
			nVariable->BNextMotorState = DDG_MotorState_Stop;
			nVariable->LNextMotorState = DDG_MotorState_Stop;
			nVariable->DDG_Pt = 0;

#if (LEG_COM)
			// 自动脚长检查触发
			i = 1;
			if (nPreLDDGCircle < 2 && (GetLegDataRx()->Flex.State != LegActionStop))
			{
				i = 0;
			}

			if (nVariable->LnCurCircle < 20 && nVariable->BnCurCircle < 20) // * 找脚条件LnCurCircle 10->20
				i = 0;

			if ((1 == MChair.CFG.Bit.NoFootFindFlag) || (WorkState_Reset == WorkStateGet()) || (WorkStateGet() == WorkState_Wait)) // * 修改 待机也不找脚 250427
				i = 0;

			if (KeyPush.FootFlexKeyState) // * 优先手动调节 手动模式不找脚 250416
				i = 0;

			if (Leg_Touch_Flag) // * 触地不找脚 250513
				i = 0;
#ifdef LOCK_AUTOLEG_AUTONE
			if (AutoModeGet() <= 1 && WorkStateGet() == WorkState_Work) // * 手动与自动程序1不找脚 250716
				i = 0;
#endif
			if (i && LegAutoAdj)
			{
				LegAdj_Flex_Flag = 1; // * 增加 250505
				MChair.Leg.FootFinding = 1;
				FootFlexStateSet(FootFind);
				FootFlexSpeedSet(5); /* 找脚默认速度最大 */
			}

			nPreBDDGCircle = 0;
			nPreLDDGCircle = 0;
			LegAutoAdj = 0;
#endif // LEG_COM
		}
		break;
	case DDG_UserState_BackUp:
		if (nVariable->nCurUserState != nVariable->UserState)
		{
			nVariable->nCurUserState = nVariable->UserState;
			nVariable->BNextMotorState = DDG_MotorState_Up;
			nVariable->LNextMotorState = DDG_MotorState_Stop;
			nVariable->DDG_Pt = 0;
		}

#if (LEG_COM)
		if (LegVoid)
		{
			if (nVariable->DDG_Pt == 0x80)
			{
				if (GetLegDataRx()->Flex.State == FlexStop)
				{
					LegAutoAdj = 1;
					nVariable->DDG_Pt = 0x40;
					nVariable->BNextMotorState = DDG_MotorState_Up;
					nVariable->LNextMotorState = DDG_MotorState_Stop;
				}
				else
					break;
			}
			else
			{
				if (!GetLegDataRx()->Singal.Top && GetLegDataRx()->Singal.Touch && nVariable->DDG_Pt < 0x40)
				{
					nVariable->DDG_Pt = 0x80;
					FootFlexStateSet(FlexUp);
					nVariable->BNextMotorState = DDG_MotorState_Stop;
					nVariable->LNextMotorState = DDG_MotorState_Stop;
					break;
				}
			}
		}
#endif // LEG_COM

		if (nVariable->BPlace == DDG_Place_Top || (nVariable->BEor & 0x0c))
		{
			nVariable->BNextMotorState = DDG_MotorState_Stop;
			nVariable->LNextMotorState = DDG_MotorState_Stop;
			nVariable->UserState = DDG_UserState_Stop;
			nVariable->nCurUserState = 255;
		}
		break;
	case DDG_UserState_BackDown:
		if (nVariable->nCurUserState != nVariable->UserState)
		{
			nVariable->nCurUserState = nVariable->UserState;
			nVariable->BNextMotorState = DDG_MotorState_Down;
			nVariable->LNextMotorState = DDG_MotorState_Stop;
			nVariable->DDG_Pt = 0;
		}
		if (nVariable->BPlace == DDG_Place_Bot || (nVariable->BEor & 0x0c))
		{
			nVariable->BNextMotorState = DDG_MotorState_Stop;
			nVariable->LNextMotorState = DDG_MotorState_Stop;
			nVariable->UserState = DDG_UserState_Stop;
			nVariable->nCurUserState = 255;
		}
		break;
	case DDG_UserState_LegUp:
		if (nVariable->nCurUserState != nVariable->UserState)
		{
			nVariable->nCurUserState = nVariable->UserState;
			nVariable->BNextMotorState = DDG_MotorState_Stop;
			nVariable->LNextMotorState = DDG_MotorState_Up;
			nVariable->DDG_Pt = 0;
		}

#if (LEG_COM)
		if (LegVoid)
		{
			if (nVariable->DDG_Pt == 0x80)
			{
				if (GetLegDataRx()->Flex.State == FlexStop)
				{
					LegAutoAdj = 1;
					nVariable->DDG_Pt = 0x40;
					nVariable->BNextMotorState = DDG_MotorState_Stop;
					nVariable->LNextMotorState = DDG_MotorState_Up;
				}
				else
					break;
			}
			else
			{
				if (!GetLegDataRx()->Singal.Top && GetLegDataRx()->Singal.Touch && nVariable->DDG_Pt < 0x40)
				{
					nVariable->DDG_Pt = 0x80;
					FootFlexStateSet(FlexUp);
					nVariable->BNextMotorState = DDG_MotorState_Stop;
					nVariable->LNextMotorState = DDG_MotorState_Stop;
					break;
				}
			}
		}
#endif // LEG_COM

		if (nVariable->LPlace == DDG_Place_Top || (nVariable->LEor & 0x0c))
		{
			nVariable->BNextMotorState = DDG_MotorState_Stop;
			nVariable->LNextMotorState = DDG_MotorState_Stop;
			nVariable->UserState = DDG_UserState_Stop;
			nVariable->nCurUserState = 255;
		}
		break;
	case DDG_UserState_LegDown:
		if (nVariable->nCurUserState != nVariable->UserState)
		{
			nVariable->nCurUserState = nVariable->UserState;
			nVariable->BNextMotorState = DDG_MotorState_Stop;
			nVariable->LNextMotorState = DDG_MotorState_Down;
			nVariable->DDG_Pt = 0;
		}

		if (nVariable->LPlace == DDG_Place_Bot || (nVariable->LEor & 0x0c))
		{
			nVariable->BNextMotorState = DDG_MotorState_Stop;
			nVariable->LNextMotorState = DDG_MotorState_Stop;
			nVariable->UserState = DDG_UserState_Stop;
			nVariable->nCurUserState = 255;
		}
		break;

	case DDG_UserState_SitUp:
		if (nVariable->nCurUserState != nVariable->UserState)
		{
			nVariable->nCurUserState = nVariable->UserState;
			nVariable->DDG_Pt = 0;
			nVariable->BNextMotorState = DDG_MotorState_Up;
			nVariable->LNextMotorState = DDG_MotorState_Up;
		}

#if (LEG_COM)
		if (LegVoid)
		{
			if (nVariable->DDG_Pt == 0x80)
			{
				if (GetLegDataRx()->Flex.State == FlexStop)
				{
					LegAutoAdj = 1;
					nVariable->DDG_Pt = 0x40;
					nVariable->BNextMotorState = DDG_MotorState_Up;
					nVariable->LNextMotorState = DDG_MotorState_Up;
				}
				else
					break;
			}
			else
			{
				if (!GetLegDataRx()->Singal.Top && GetLegDataRx()->Singal.Touch && nVariable->DDG_Pt < 0x40)
				{
					nVariable->DDG_Pt = 0x80;
					FootFlexStateSet(FlexUp);
					nVariable->BNextMotorState = DDG_MotorState_Stop;
					nVariable->LNextMotorState = DDG_MotorState_Stop;
					break;
				}
			}
		}
#endif // LEG_COM

		if (nVariable->BPlace == DDG_Place_Top || (nVariable->BEor & 0x0c))
			nVariable->BNextMotorState = DDG_MotorState_Stop;

		if (nVariable->LPlace == DDG_Place_Top || (nVariable->LEor & 0x0c))
			nVariable->LNextMotorState = DDG_MotorState_Stop;

		if (nVariable->BNextMotorState == DDG_MotorState_Stop &&
			nVariable->LNextMotorState == DDG_MotorState_Stop)
		{
			nVariable->nCurUserState = 255;
			nVariable->UserState = DDG_UserState_Stop;
		}
		break;

	case DDG_UserState_SitDown:
		if (nVariable->nCurUserState != nVariable->UserState)
		{
			nVariable->nCurUserState = nVariable->UserState;
			nVariable->BNextMotorState = DDG_MotorState_Down;
			nVariable->LNextMotorState = DDG_MotorState_Down;
			nVariable->DDG_Pt = 0;
		}
		if (nVariable->BPlace == DDG_Place_Bot || (nVariable->BEor & 0x0c))
			nVariable->BNextMotorState = DDG_MotorState_Stop;

		if (nVariable->LPlace == DDG_Place_Bot || (nVariable->LEor & 0x0c))
			nVariable->LNextMotorState = DDG_MotorState_Stop;

		if (nVariable->BNextMotorState == DDG_MotorState_Stop &&
			nVariable->LNextMotorState == DDG_MotorState_Stop)
		{
			nVariable->nCurUserState = 255;
			nVariable->UserState = DDG_UserState_Stop;
		}
		break;

	case DDG_UserState_SitUp_Circle:
	case DDG_UserState_StretchUp:
	case DDG_UserState_SitDown_Circle:
	case DDG_UserState_StretchDown:
		if (nVariable->nCurUserState != nVariable->UserState)
		{
			nVariable->nCurUserState = nVariable->UserState;

			nVariable->DDG_Pt = 0;

			if (nVariable->nCurUserState == DDG_UserState_SitUp_Circle ||
				nVariable->nCurUserState == DDG_UserState_StretchUp)
				nVariable->BNextMotorState = DDG_MotorState_Up, nVariable->DDG_Pt |= 1;
			else
				nVariable->BNextMotorState = DDG_MotorState_Down;

			if (nVariable->nCurUserState == DDG_UserState_SitUp_Circle ||
				nVariable->nCurUserState == DDG_UserState_StretchDown)
				nVariable->LNextMotorState = DDG_MotorState_Up, nVariable->DDG_Pt |= 2;
			else
				nVariable->LNextMotorState = DDG_MotorState_Down;
		}

#if (LEG_COM)
		if (LegVoid)
		{
			if (nVariable->DDG_Pt == 0x80)
			{
				if (GetLegDataRx()->Flex.State == FlexStop)
				{
					LegAutoAdj = 1;
					nVariable->DDG_Pt = 0x40;
				}
				else
					break;
			}
			else
			{
				if (!GetLegDataRx()->Singal.Top && GetLegDataRx()->Singal.Touch && nVariable->DDG_Pt < 0x40)
				{
					nVariable->DDG_Pt = 0x80;
					FootFlexStateSet(FlexUp);
					nVariable->BNextMotorState = DDG_MotorState_Stop;
					nVariable->LNextMotorState = DDG_MotorState_Stop;
					break;
				}
			}
		}
#endif // LEG_COM

		if (nVariable->BnLocalCircle == 0 || (nVariable->BEor & 0x0c))
			nVariable->BNextMotorState = DDG_MotorState_Stop;

		if (nVariable->LnLocalCircle == 0 || (nVariable->LEor & 0x0c))
			nVariable->LNextMotorState = DDG_MotorState_Stop;

		if (nVariable->DDG_Pt & 0x01)
		{
			if (nVariable->BPlace == DDG_Place_Top)
				nVariable->BNextMotorState = DDG_MotorState_Stop;
		}
		else
		{
			if (nVariable->BPlace == DDG_Place_Bot)
				nVariable->BNextMotorState = DDG_MotorState_Stop;
		}

		if (nVariable->DDG_Pt & 0x02)
		{
			if (nVariable->LPlace == DDG_Place_Top)
				nVariable->LNextMotorState = DDG_MotorState_Stop;
		}
		else
		{
			if (nVariable->LPlace == DDG_Place_Bot)
				nVariable->LNextMotorState = DDG_MotorState_Stop;
		}

		if (nVariable->BNextMotorState == DDG_MotorState_Stop &&
			nVariable->LNextMotorState == DDG_MotorState_Stop)
		{
			nVariable->nCurUserState = 255;
			nVariable->UserState = DDG_UserState_Stop;
		}

		break;

	case DDG_UserState_Sit_Circle:
		if (nVariable->nCurUserState != nVariable->UserState)
		{
			nVariable->nCurUserState = nVariable->UserState;
			nVariable->DDG_Pt = 0;
		}

#if (LEG_COM)
		if (LegVoid)
		{
			if (nVariable->DDG_Pt == 0x80)
			{
				if (GetLegDataRx()->Flex.State == FlexStop)
				{
					nVariable->DDG_Pt = 0x40;
				}
				else
					break;
			}
			else
			{
				if (!GetLegDataRx()->Singal.Top && GetLegDataRx()->Singal.Touch && nVariable->DDG_Pt < 0x40)
				{
					nVariable->DDG_Pt = 0x80;
					FootFlexStateSet(FlexUp);
					nVariable->BNextMotorState = DDG_MotorState_Stop;
					nVariable->LNextMotorState = DDG_MotorState_Stop;
					break;
				}
			}
		}
#endif // LEG_COM

		if (nVariable->BSignalInit)
		{
			if (!(nVariable->DDG_Pt & 0x01))
			{
				nVariable->DDG_Pt |= 0x01;

				if (nVariable->BnCurCircle > nVariable->BCircleSet)
				{
					nVariable->BnLocalCircle = nVariable->BnCurCircle - nVariable->BCircleSet;
					nVariable->BNextMotorState = DDG_MotorState_Up;
				}
				else if (nVariable->BnCurCircle < nVariable->BCircleSet)
				{
					nVariable->BnLocalCircle = nVariable->BCircleSet - nVariable->BnCurCircle;
					nVariable->BNextMotorState = DDG_MotorState_Down;
				}
				else
					nVariable->BNextMotorState = DDG_MotorState_Stop;
			}
			else
			{
				if (nVariable->BnLocalCircle == 0)
					nVariable->BNextMotorState = DDG_MotorState_Stop;
			}
		}
		else
			nVariable->BNextMotorState = DDG_MotorState_Up;

		if (nVariable->LSignalInit)
		{
			if (!(nVariable->DDG_Pt & 0x02))
			{
				nVariable->DDG_Pt |= 0x02;
				if (nVariable->LnCurCircle > nVariable->LCircleSet)
				{
					nVariable->LnLocalCircle = nVariable->LnCurCircle - nVariable->LCircleSet;
					nVariable->LNextMotorState = DDG_MotorState_Up;
				}
				else if (nVariable->LnCurCircle < nVariable->LCircleSet)
				{
					nVariable->LnLocalCircle = nVariable->LCircleSet - nVariable->LnCurCircle;
					nVariable->LNextMotorState = DDG_MotorState_Down;
				}
				else
					nVariable->LNextMotorState = DDG_MotorState_Stop;
			}
			else
			{
				if (nVariable->LnLocalCircle == 0)
					nVariable->LNextMotorState = DDG_MotorState_Stop;
			}
		}
		else
			nVariable->LNextMotorState = DDG_MotorState_Up;

		if (nVariable->BEor & 0x0c)
			nVariable->BNextMotorState = DDG_MotorState_Stop;
		if (nVariable->LEor & 0x0c)
			nVariable->LNextMotorState = DDG_MotorState_Stop;

		if (nVariable->LNextMotorState == DDG_MotorState_Stop &&
			nVariable->BNextMotorState == DDG_MotorState_Stop)
		{
			nVariable->nCurUserState = 255;
			nVariable->UserState = DDG_UserState_Stop;
		}

		break;
	}

	switch (nVariable->BMotorState)
	{
	default:
		if (nVariable->BStopTime < DDG_Stop_Time)
		{
			nVariable->BStopTime++;
			nVariable->BSignalOutTime = 0;
			nVariable->BCircleOutTime = 5;
			nVariable->BnCurSpeed = 0;
			BDDG_PWM_Disable();
		}
		else if (nVariable->BStopTime < DDG_Stop_Time1)
		{
			nVariable->BStopTime++;
			BDDG_Dir_Set_Stop();
		}
		else
		{
			if (nVariable->BNextMotorState == DDG_MotorState_Up &&
				nVariable->BPlace != DDG_Place_Top &&
				nVariable->BPlace != DDG_Place_Eor &&
				BDDGOverLoadFlag == 0 && WorkStateGet() != WorkState_Pause)
			{
				nVariable->BMotorState = DDG_MotorState_Up;
				DDGVariable.BDir = DDG_MotorState_Up;
				nVariable->BStopTime = 0;
				BDD_Switch_time = 0;
				if (nVariable->BPlace == DDG_Place_Bot)
					nVariable->BPlace = DDG_Place_NULL;
				BDDG_Dir_Set_Up();
			}
			else if (nVariable->BNextMotorState == DDG_MotorState_Down &&
					 nVariable->BPlace != DDG_Place_Bot &&
					 nVariable->BPlace != DDG_Place_Eor &&
					 BDDGOverLoadFlag == 0 && WorkStateGet() != WorkState_Pause)
			{
				nVariable->BMotorState = DDG_MotorState_Down;
				DDGVariable.BDir = DDG_MotorState_Down;
				nVariable->BStopTime = 0;
				BDD_Switch_time = 0;
				if (nVariable->BPlace == DDG_Place_Top)
					nVariable->BPlace = DDG_Place_NULL;
				BDDG_Dir_Set_Down();
			}
		}
		break;
	case DDG_MotorState_Up:
		if (nVariable->BNextMotorState != DDG_MotorState_Up ||
			nVariable->BPlace == DDG_Place_Top ||
			nVariable->BPlace == DDG_Place_Eor ||
			nVariable->BSignalOutTime >= DDG_Tread_OutTime ||
			BDDGOverLoadFlag == 1 || WorkStateGet() == WorkState_Pause)
		{
			BDDG_PWM_Disable();
			nVariable->BMotorState = DDG_MotorState_Stop;
		}
		else
		{
			nVariable->BSignalOutTime++;
			if (nVariable->BCircleOutTime < DDG_Circle_OutTime)
				nVariable->BCircleOutTime++;
			if (nVariable->BnCurSpeed < BDDG_Speed)
			{
				nVariable->BnCurSpeed += 4;
				if (nVariable->BnCurSpeed > BDDG_Speed)
					nVariable->BnCurSpeed = BDDG_Speed;
				BDDG_PWM_Enable(nVariable->BnCurSpeed);
			}

			if (BackDDG_Current <= DDG_StopCurrent)
			{
				if (++BDD_Switch_time > 180)
				{
					BDD_Switch_time = 0;
					nVariable->BPlace = DDG_Place_Top;
				}
			}
			else
				BDD_Switch_time = 0;
		}
		break;
	case DDG_MotorState_Down:
		if (nVariable->BNextMotorState != DDG_MotorState_Down ||
			nVariable->BPlace == DDG_Place_Bot ||
			nVariable->BPlace == DDG_Place_Eor ||
			nVariable->BSignalOutTime >= DDG_Tread_OutTime ||
			BDDGOverLoadFlag == 1 || WorkStateGet() == WorkState_Pause)
		{
			BDDG_PWM_Disable();
			nVariable->BMotorState = DDG_MotorState_Stop;
		}
		else
		{
			nVariable->BSignalOutTime++;
			if (nVariable->BCircleOutTime < DDG_Circle_OutTime)
				nVariable->BCircleOutTime++;
			if (nVariable->BnCurSpeed < BDDG_Speed)
			{
				nVariable->BnCurSpeed += 4;
				if (nVariable->BnCurSpeed > BDDG_Speed)
					nVariable->BnCurSpeed = BDDG_Speed;
				BDDG_PWM_Enable(nVariable->BnCurSpeed);
			}

			if (BackDDG_Current <= DDG_StopCurrent)
			{
				if (++BDD_Switch_time > 180)
				{
					BDD_Switch_time = 0;
					nVariable->BPlace = DDG_Place_Bot;
				}
			}
			else
				BDD_Switch_time = 0;
		}
		break;
	}

	switch (nVariable->LMotorState)
	{
	default:
		if (nVariable->LStopTime < DDG_Stop_Time)
		{
			nVariable->LStopTime++;
			nVariable->LSignalOutTime = 0;
			nVariable->LCircleOutTime = 5;
			nVariable->LnCurSpeed = 0;
			LDDG_PWM_Disable();
		}
		else if (nVariable->LStopTime < DDG_Stop_Time1)
		{
			nVariable->LStopTime++;
			LDDG_Dir_Set_Stop();
		}
		else
		{
			if (nVariable->LNextMotorState == DDG_MotorState_Up &&
				nVariable->LPlace != DDG_Place_Top &&
				nVariable->LPlace != DDG_Place_Eor &&
				LDDGOverLoadFlag == 0 && WorkStateGet() != WorkState_Pause)
			{
				nVariable->LMotorState = DDG_MotorState_Up;
				DDGVariable.LDir = DDG_MotorState_Up;
				nVariable->LStopTime = 0;

				LDD_Switch_time = 0;
				if (nVariable->LPlace == DDG_Place_Bot)
					nVariable->LPlace = DDG_Place_NULL;

				LDDG_Dir_Set_Up();
			}
			else if (nVariable->LNextMotorState == DDG_MotorState_Down &&
					 nVariable->LPlace != DDG_Place_Bot &&
					 nVariable->LPlace != DDG_Place_Eor &&
					 LDDGOverLoadFlag == 0 && WorkStateGet() != WorkState_Pause)
			{
				nVariable->LMotorState = DDG_MotorState_Down;
				DDGVariable.LDir = DDG_MotorState_Down;
				nVariable->LStopTime = 0;

				LDD_Switch_time = 0;
				if (nVariable->LPlace == DDG_Place_Top)
					nVariable->LPlace = DDG_Place_NULL;

				LDDG_Dir_Set_Down();
			}
		}
		break;
	case DDG_MotorState_Up: // * 增加小腿触底判断 250513  增加 DDG_Get_LnCurCircle > 10 250616
		if (nVariable->LNextMotorState != DDG_MotorState_Up ||
			nVariable->LPlace == DDG_Place_Top ||
			nVariable->LPlace == DDG_Place_Eor ||
			nVariable->LSignalOutTime >= DDG_Tread_OutTime ||
			LDDGOverLoadFlag == 1 ||
			(Leg_Touch_Flag && (DDG_Get_LnCurCircle > 20) && WorkStateGet() != WorkState_Wait && WorkStateGet() != WorkState_Reset) ||
			WorkStateGet() == WorkState_Pause)
		{
			LDDG_PWM_Disable();
			nVariable->LMotorState = DDG_MotorState_Stop;
		}
		else
		{
			nVariable->LSignalOutTime++;
			if (nVariable->LCircleOutTime < DDG_Circle_OutTime)
				nVariable->LCircleOutTime++;
			if (nVariable->LnCurSpeed < LDDG_Speed)
			{
				nVariable->LnCurSpeed += 4;
				if (nVariable->LnCurSpeed > LDDG_Speed)
					nVariable->LnCurSpeed = LDDG_Speed;
				LDDG_PWM_Enable(nVariable->LnCurSpeed);
			}

			if (LegDDG_Current <= DDG_StopCurrent)
			{
				if (++LDD_Switch_time > 180)
				{
					LDD_Switch_time = 0;
					nVariable->LPlace = DDG_Place_Top;
				}
			}
			else
				LDD_Switch_time = 0;
		}
		break;

	case DDG_MotorState_Down:
		if (nVariable->LNextMotorState != DDG_MotorState_Down ||
			nVariable->LPlace == DDG_Place_Bot ||
			nVariable->LPlace == DDG_Place_Eor ||
			nVariable->LSignalOutTime >= DDG_Tread_OutTime ||
			LDDGOverLoadFlag == 1 ||
			WorkStateGet() == WorkState_Pause)
		{
			LDDG_PWM_Disable();
			nVariable->LMotorState = DDG_MotorState_Stop;
		}
		else
		{
			nVariable->LSignalOutTime++;
			if (nVariable->LCircleOutTime < DDG_Circle_OutTime)
				nVariable->LCircleOutTime++;
			if (nVariable->LnCurSpeed < LDDG_Speed)
			{
				nVariable->LnCurSpeed += 4;
				if (nVariable->LnCurSpeed > LDDG_Speed)
					nVariable->LnCurSpeed = LDDG_Speed;
				LDDG_PWM_Enable(nVariable->LnCurSpeed);
			}

			if (LegDDG_Current <= DDG_StopCurrent)
			{
				if (++LDD_Switch_time > 180)
				{
					LDD_Switch_time = 0;
					nVariable->LPlace = DDG_Place_Bot;
				}
			}
			else
				LDD_Switch_time = 0;
		}
		break;
	}

	if (nVariable->BEor & DDG_Eor_Circle)
	{
		if (nVariable->BCircleOutTime < 5)
			nVariable->BEor &= ~DDG_Eor_Circle;
	}
	else
	{
		if (nVariable->BCircleOutTime >= DDG_Circle_OutTime)
			nVariable->BEor |= DDG_Eor_Circle;
	}

	if (nVariable->BEor & DDG_Eor_OutTime)
	{
		if (nVariable->BSignalOutTime < DDG_Tread_OutTime)
			nVariable->BEor &= ~DDG_Eor_OutTime;
	}
	else
	{
		if (nVariable->BSignalOutTime >= DDG_Tread_OutTime)
			nVariable->BEor |= DDG_Eor_OutTime;
	}

	if (nVariable->LEor & DDG_Eor_Circle)
	{
		if (nVariable->LCircleOutTime < 5)
			nVariable->LEor &= ~DDG_Eor_Circle;
	}
	else
	{
		if (nVariable->LCircleOutTime >= DDG_Circle_OutTime)
			nVariable->LEor |= DDG_Eor_Circle;
	}

	if (nVariable->LEor & DDG_Eor_OutTime)
	{
		if (nVariable->LSignalOutTime < DDG_Tread_OutTime)
			nVariable->LEor &= ~DDG_Eor_OutTime;
	}
	else
	{
		if (nVariable->LSignalOutTime >= DDG_Tread_OutTime)
			nVariable->LEor |= DDG_Eor_OutTime;
	}
}

// void exint_config(void) // 用外部中断检测电动杆脉冲数
//{
//	exint_init_type exint_init_struct;

//	crm_periph_clock_enable(CRM_IOMUX_PERIPH_CLOCK, TRUE);
//	crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);
//	crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, TRUE);

//	gpio_exint_line_config(GPIO_PORT_SOURCE_GPIOB, GPIO_PINS_SOURCE3);
//	gpio_exint_line_config(GPIO_PORT_SOURCE_GPIOA, GPIO_PINS_SOURCE15);

//	exint_default_para_init(&exint_init_struct);
//	exint_init_struct.line_enable = TRUE;
//	exint_init_struct.line_mode = EXINT_LINE_INTERRUPUT;
//	exint_init_struct.line_select = EXINT_LINE_3;
//	exint_init_struct.line_polarity = EXINT_TRIGGER_BOTH_EDGE;
//	exint_init(&exint_init_struct);

//	exint_init_struct.line_select = EXINT_LINE_15;
//	exint_init(&exint_init_struct);

//	nvic_irq_enable(EXINT3_IRQn, 1, 0);
//	nvic_irq_enable(EXINT15_10_IRQn, 1, 0);
//}

/**
 * @brief  exint0 interrupt handler
 * @param  none
 * @retval none
 */

// void EXINT15_10_IRQHandler(void) // 靠背电动杆计数
//{
//	if (exint_flag_get(EXINT_LINE_15) != RESET)
//	{
//		exint_flag_clear(EXINT_LINE_15);

//		if (DDGVariable.BDir == DDG_MotorState_Up)
//		{
//			DDGVariable.BCircleOutTime = 0;
//			if (DDGVariable.BnCurCircle > 0)
//				DDGVariable.BnCurCircle--;
//			if (DDGVariable.BnLocalCircle > 0)
//				DDGVariable.BnLocalCircle--;
//			nPreBDDGCircle++;
//		}
//		else if (DDGVariable.BDir == DDG_MotorState_Down)
//		{

//			DDGVariable.BCircleOutTime = 0;
//			if (DDGVariable.BSignalInit)
//				DDGVariable.BnCurCircle++;
//			if (DDGVariable.BnLocalCircle > 0)
//				DDGVariable.BnLocalCircle--;
//			nPreBDDGCircle++;
//		}
//	}
//}

// void EXINT3_IRQHandler(void) // 小腿电动杆计数
//{
//	if (exint_flag_get(EXINT_LINE_3) != RESET)
//	{
//		exint_flag_clear(EXINT_LINE_3);

//		if (DDGVariable.LDir == DDG_MotorState_Up)
//		{
//			DDGVariable.LCircleOutTime = 0;
//			if (DDGVariable.LnCurCircle)
//				DDGVariable.LnCurCircle--;
//			if (DDGVariable.LnLocalCircle > 0)
//				DDGVariable.LnLocalCircle--;
//			nPreLDDGCircle++;
//		}
//		else if (DDGVariable.LDir == DDG_MotorState_Down)
//		{
//			DDGVariable.LCircleOutTime = 0;
//			if (DDGVariable.LSignalInit)
//				DDGVariable.LnCurCircle++;
//			if (DDGVariable.LnLocalCircle > 0)
//				DDGVariable.LnLocalCircle--;
//			nPreLDDGCircle++;
//		}
//	}
//}

void Motor_Init(void) //
{
	// exint_config();

	DDGVariable.UserState = DDG_UserState_Stop;
	DDGVariable.nCurUserState = DDG_UserState_Stop;
	DDGVariable.LMotorState = DDG_MotorState_Stop;
	DDGVariable.LNextMotorState = DDG_MotorState_Stop;
	DDGVariable.LStopTime = 0;
	DDGVariable.LSignal = 0xff;
	DDGVariable.LSignalInit = 0;
	DDGVariable.LPlace = DDG_Place_NULL;
	DDGVariable.LnCurCircle = 0;
	DDGVariable.LCircleSet = 0;
	DDGVariable.LnLocalCircle = 0;
	DDGVariable.BMotorState = DDG_MotorState_Stop;
	DDGVariable.BNextMotorState = DDG_MotorState_Stop;
	DDGVariable.BStopTime = 0;
	DDGVariable.BSignal = 0xff;
	DDGVariable.BSignalInit = 0;
	DDGVariable.BPlace = DDG_Place_NULL;
	DDGVariable.BnCurCircle = 0;
	DDGVariable.BCircleSet = 0;
	DDGVariable.BnLocalCircle = 0;
	DDGVariable.BEor = DDG_Eor_NULL;
	DDGVariable.LEor = DDG_Eor_NULL;
	DDGVariable.BSignalOutTime = 0;
	DDGVariable.BCircleOutTime = 5;
	DDGVariable.LSignalOutTime = 0;
	DDGVariable.LCircleOutTime = 5;
	DDGVariable.BnCurSpeed = 0;
	DDGVariable.LnCurSpeed = 0;

	DDGVariable.DDG_Pt = 0;
	DDGVariable.LDir = 0;
	DDGVariable.BDir = 0;

	DDGVariable.LOverLoadCnt = 0;
	DDGVariable.BOverLoadCnt = 0;
	DDGVariable.LOverLoadTick = 0;
	DDGVariable.BOverLoadTick = 0;
	LDDGOverLoadFlag = 0;
	BDDGOverLoadFlag = 0;
}
#endif // DDG_Type

void Motor_Func_Update(void)
{
	DDG_Motor_Control(&DDGVariable);
}

static u8 MotorStep = 0;
static u8 MotorTick = 0;
static u32 LMotorFaultTick = 0; // * 小腿电动杆错误判断
bool LMotorFaultFlag = 0;
static u32 BMotorFaultTick = 0; // * 靠背电动杆错误判断
static bool BMotorFaultFlag = 0;
u8 BMotorFaultCnt = 0;	 //* 靠背电动杆故障次数
u8 BMotorMaxCurrent = 0; //* 靠背电动杆最大电流
static u8 LMotorFaultCnt = 0;	 //* 靠背电动杆故障次数
static u32 PumpFaultTick = 0;	 // * 气泵错误判断
static bool PumpFaultFlag = 0;
static u8 PumpFaultCnt = 0;	// * 气泵故障次数
void MotorProcess(void) //* 5ms 时基
{
	switch (MotorStep)
	{
	default:
		break;
	case 0:
		DDG_Motor_Control(&DDGVariable);
		TICK_RESET(MotorTick);
		MotorStep++;
		break;

	case 1:
		if (TICK_COUNTER(MotorTick, 5))
		{
			MotorStep = 0;
		}
		break;
	}

	if (SIT_NONE_Flag) // * 增加 250616
	{
		if (DDGVariable.LnCurCircle < 10 || DDGVariable.UserState != DDG_UserState_SitUp) // * 增加 250701
			SIT_NONE_Flag = 0;
	}

	if (BDDGCircle_Fault() == 0) // *  增加 8840故障复位 250308
	{
		if (BMotorFaultFlag == 0)
		{
			BMotorFaultFlag = 1;
			TICK_RESET(BMotorFaultTick);
		}
		else
		{
			if (TICK_COUNTER(BMotorFaultTick, 6000))
			{
				BMotorFaultFlag = 0;
			}
			else if (TICK_COUNTER(BMotorFaultTick, 4000))
			{
				BDDG_RESET_HIGH;
			}
			else if (TICK_COUNTER(BMotorFaultTick, 3000))
			{
				BDDG_RESET_LOW;
			}
		}
	}
	else
	{
		if (BMotorFaultFlag)
		{
			BMotorFaultFlag = 0;
			if (TICK_COUNTER(BMotorFaultTick, 3000))
				BMotorFaultCnt++;
		}
	}

	if (LDDGCircle_Fault() == 0) // *  增加 8840故障复位  250723
	{
		if (LMotorFaultFlag == 0)
		{
			LMotorFaultFlag = 1;
			TICK_RESET(LMotorFaultTick);
		}
		else
		{
			if (TICK_COUNTER(LMotorFaultTick, 6000))
			{
				LMotorFaultFlag = 0;
			}
			else if (TICK_COUNTER(LMotorFaultTick, 4000))
			{
				LDDG_RESET_HIGH;
			}
			else if (TICK_COUNTER(LMotorFaultTick, 3000))
			{
				LDDG_RESET_LOW;
			}
		}
	}
	else
	{
		if (LMotorFaultFlag)
		{
			LMotorFaultFlag = 0;
			if (TICK_COUNTER(LMotorFaultTick, 3000))
				LMotorFaultCnt++;
		}
	}

	if (PUMPCircle_Fault() == 0) // *  增加 8840故障复位 250322
	{
		if (PumpFaultFlag == 0)
		{
			PumpFaultFlag = 1;
			TICK_RESET(PumpFaultTick);
		}
		else
		{
			if (TICK_COUNTER(PumpFaultTick, 6000))
			{
				PumpFaultFlag = 0;
			}
			else if (TICK_COUNTER(PumpFaultTick, 4000))
			{
				PUMP_RESET_HIGH;
			}
			else if (TICK_COUNTER(PumpFaultTick, 3000))
			{
				PUMP_RESET_LOW;
			}
		}
	}
	else
	{
		if (PumpFaultFlag)
		{
			PumpFaultFlag = 0;
			if (TICK_COUNTER(PumpFaultTick, 3000))
				PumpFaultCnt++;
		}
	}
}
