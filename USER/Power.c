#include "Power.h"

#ifdef Debug_Mode
PowerInfo_S Power;
#else
static PowerInfo_S Power;
#endif

void PowerInit(void)
{
	memset(&Power, 0, sizeof(PowerInfo_S));
	_5VA_ON();
	_5VB_ON();
	_5VC_ON();
}

u8 PowerStateGet(void)
{

	return Power.State;
}

static u8 Power_Flag = 0;
void PowerProcess(void)
{
	Power_Flag = 0;

	if (WorkStateGet() != WorkState_Wait) // 工作状态判断
		Power_Flag = 1;

	if (DDGVariable.UserState != DDG_UserState_Stop) // 电动杆状态判断
		Power_Flag = 1;

#if (LEG_COM)
	// * 小腿伸缩状态判断
	if ((GetLegDataRx()->Flex.State != DDG_UserState_Stop) && (GetLegDataRx()->Flex.Place != 2)) // * 修改 250411
		Power_Flag = 1;
#endif // LEG_COM

	// Power_Flag = 1; //* 测试 250404

	switch (Power.State)
	{

	default:
	case PowerOff:
		if (Power_Flag)
		{
			if (WorkStateGet() != WorkState_Wait)
				_5VC_ON(); // * 手控电源
			_5VA_ON();
			_5VB_ON();
			_24V_ON();
			Power.State = PowerStart;
			TICK_RESET(Power.Tick);
		}

		break;

	case PowerStart:

		if (TICK_COUNTER(Power.Tick, PowerStartTime))
		{
			Power.State = PowerReady;
		}

		break;

	case PowerReady:

		if (Power_Flag == 0) // 没有负载15s后断电
		{
			if (TICK_COUNTER(Power.Tick, 15000))
			{
				_24V_OFF();
				_5VC_OFF(); //* 关闭手控电源

				Handle_Dispaly_Step = 0; // todo 增加防止断电5秒内上电手控器bug 260616
#ifdef USE_SOUNDWAVE
				BREDR_Iint_Flag = 0; // todo 蓝牙开关重新初始化 260825
#endif
				if (nLowPower == 0) // todo add 260701
				{
					Massage_Flag.Reset = 1;
					_5VA_OFF(); // * 不能关 机芯计数有问题 等优化 250407
					_5VB_OFF(); // * 蓝牙 不能关
				}
				Power.State = PowerOff;
			}
		}
		else
			TICK_RESET(Power.Tick);

		break;
	}
}
