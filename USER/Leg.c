#include "Leg.h"

LegDataTx_S LegTx;
LegDataRx_S LegRx;

Leg_State_t Leg_State;

void LegInit(void)
{
	memset(&LegRx, 0, sizeof(LegDataRx_S));
	memset(&LegTx, 0, sizeof(LegDataTx_S));
}

void LegDataTx(void) //* 200ms 时基
{
	u8 LegDataUpdate[sizeof(LegDataTx_S)] = {0};

	LegTx.Roll.State = RollStateGet();
	LegTx.Roll.Speed = RollSpeedGet();
	LegTx.Knead1.State = KneadStateGet(); /* 腿部滚刮和揉搓状态一致 */
	LegTx.Knead1.Speed = KneadSpeedGet();
	LegTx.Knead2.State = KneadStateGet();
	LegTx.Knead2.Speed = KneadSpeedGet();

	LegTx.Flex.State = FootFlexStateGet();
	LegTx.Flex.Speed = FootFlexSpeedGet();
	LegTx.Flex.Circle_L8 = FootFlexCircleGet();
	LegTx.Flex.Circle_H8 = FootFlexCircleGet() >> 8;

	if (WorkState_Pause == WorkStateGet())
	{
		memset(&LegTx, 0, sizeof(LegDataTx_S));
	}

	memcpy(&LegDataUpdate, &LegTx, sizeof(LegDataUpdate) / sizeof(LegDataUpdate)[0]);
	_485_ComTx(LegAddr, MainAddr, LegTx_Count, 0x0001, sizeof(LegDataUpdate) / sizeof(LegDataUpdate)[0], LegDataUpdate);
}

LegDataRx_S *GetLegDataRx(void)
{
	return &LegRx;
}

void LegRollStateSet(uint8_t sta, uint8_t spd)
{
	LegTx.Roll.State = sta;
	LegTx.Roll.Speed = spd;
}

void LegKneadStateSet(uint8_t sta, uint8_t spd)
{
	LegTx.Knead1.State = sta;
	LegTx.Knead2.State = sta;

	LegTx.Knead1.Speed = spd;
	LegTx.Knead2.Speed = spd;
}

void LegHeatStateSet(uint8_t sta, uint8_t level)
{
	LegTx.Heat.State = sta;
	LegTx.Heat.Level = level;
}

void LegAirStateSet(uint8_t air)
{
	LegTx.AirValve.Valve.Gas1 = (air & 0x01) ? 1 : 0;
	LegTx.AirValve.Valve.Gas2 = (air & 0x02) ? 1 : 0;
	LegTx.AirValve.Valve.Gas3 = (air & 0x04) ? 1 : 0;
	LegTx.AirValve.Valve.Gas4 = (air & 0x08) ? 1 : 0;
	LegTx.AirValve.Valve.Gas5 = (air & 0x10) ? 1 : 0;
	LegTx.AirValve.Valve.Gas6 = (air & 0x20) ? 1 : 0;
	LegTx.AirValve.Valve.Gas7 = (air & 0x40) ? 1 : 0;
	LegTx.AirValve.Valve.Gas8 = (air & 0x80) ? 1 : 0;
}

void LegFlexStateSet(uint8_t sta, uint8_t spd, uint16_t circle)
{
	LegTx.Flex.State = sta;
	LegTx.Flex.Speed = spd;
	LegTx.Flex.Circle_L8 = circle;
	LegTx.Flex.Circle_H8 = circle >> 8;
}
