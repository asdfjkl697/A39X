#include "FootFlex.h"
#include "Leg.h"

static FootFlexInfo_S Flex;

void FootFlexSpeedSet(u8 spd)
{
	Flex.Speed = spd;
}

u8 FootFlexSpeedGet(void)
{
	return Flex.Speed;
}

void FootFlexStateSet(FootFlexState_E sta)
{
	Flex.State = sta;
}

FootFlexState_E FootFlexStateGet(void)
{
	return Flex.State;
}

void FootFlexCircleSet(u16 cir)
{
	Flex.Circle = cir;
}

u16 FootFlexCircleGet(void)
{

	return Flex.Circle;
}
