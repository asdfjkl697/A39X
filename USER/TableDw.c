#include "Table.h"

#ifndef USE_M2_4D

//* 补充动作
const CoreMixOtherTabItem_t MoveDwTab_0_RPZ[] = {

	{MoveDw_Knead, KNEAD_FIX_CIRCLE, MOVEDWWIDTH3, 0, 6},
	{MoveDw_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 2},
	{MoveDw_Tap, FLAP_ACT, 0, 0, 5},
	{Part_EXEC, ConMth_MoveDw, 0, 30, 0},

	{MoveDw_Knead, KNEAD_REV_CNT, 4, 0, 5},
	{Part_EXEC, ConMth_MoveDw, 0, 30, 0},

	// {MoveDw_Knead, KNEAD_FWD_CNT, 5, 0, 4},
	// {MoveDw_TreadB, TREAD_FIX_CIRCLE, LB_1, 0, 6},
	// {MoveDw_Tap, FLAP_ACT, 0, 0, 4},
	// {Part_EXEC, ConMth_MoveDw, 0, 30, 0},

	{MoveDw_Knead, KNEAD_REV, 0, 0, 5},
	{MoveDw_TreadB, TREAD_FIX_CIRCLE, LB_0, 0, 2},
	{MoveDw_Tap, FLAP_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveDw, 0, 30, 0},

	// {MoveDw_Knead, KNEAD_FIX_CIRCLE, MOVEDWWIDTH2, 0, 6},
	// {Part_EXEC, ConMth_MoveDw, 100, 30, 0},

	// {MoveDw_Knead, KNEAD_REV_CNT, 5, 0, 3},
	// {MoveDw_Tap, FLAP_STOP, 0, 0, 5},
	// {Part_EXEC, ConMth_MoveDw, 0, 30, 0},

	{Part_EXEC, ConMth_AC, 0, 0, 0}, //* 复位标志
};

const CoreMixOtherTabItem_t MoveDwTab_FullA[] = {
	{MoveDw_Knead, KNEAD_FWD, 0, 0, 6},
	{MoveDw_TreadB, TREAD_FIX_CIRCLE, LB_H6, 0, 6},
	{MoveDw_Tap, FLAP_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveDw, 0, 30, 0},

	//* 捶打 +
	{MoveDw_Tap, FLAP_ACT, 0, 0, 4},
	{Part_LOOP, 0, 2, 12, 0},
	{MoveDw_Knead, KNEAD_FWD, 0, 0, 6},
	{MoveDw_TreadB, TREAD_FIX_CIRCLE, LB_3, 0, 2},
	{Part_EXEC, ConMth_MoveDw, 0, 30, 0},
	{MoveDw_Knead, KNEAD_1F2R, 0x97, 5, 0x62},
	{MoveDw_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 4},
	{Part_EXEC, ConMth_MoveDw, 0, 30, 0},
	{MoveDw_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH3, 0, 4},
	{Part_EXEC, ConMth_MoveDw, 0, 30, 0},
	{MoveDw_TreadB, TREAD_FIX_CIRCLE, LB_3, 0, 2},
	{Part_EXEC, ConMth_MoveDw, 0, 30, 0},
	{MoveDw_TreadB, TREAD_FIX_CIRCLE, LB_6, 0, 2}, //@ H6->6
	{Part_EXEC, ConMth_MoveDw, 0, 30, 0},
		
	{Part_LOOP, 0, 2, 7, 0},
	{MoveDw_Tap, FLAP_STOP, 0, 0, 0},
	{MoveDw_Knead, KNEAD_REV, 0, 0, 6},
	{MoveDw_TreadB, TREAD_FIX_CIRCLE, LB_0, 0, 6},
	{Part_EXEC, ConMth_MoveDw, 0, 30, 0},
	{MoveDw_Knead, KNEAD_FWD, 0, 0, 6},
	{MoveDw_TreadB, TREAD_FIX_CIRCLE, LB_6, 0, 6},
	{Part_EXEC, ConMth_MoveDw, 0, 30, 0},

	// {MoveDw_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 4}, //@ 待确认 260130
	// {MoveDw_Tap, FLAP_STOP, 0, 0, 0},
	// {Part_LOOP, 0, 5, 4, 0},
	// {MoveDw_Tread, TREAD_REV_CIRCLE, 20, 0, 3},
	// {Part_EXEC, ConMth_MoveDw, 0, 30, 0},
	// {MoveDw_Tread, TREAD_FWD_CIRCLE, 10, 0, 3},
	// {Part_EXEC, ConMth_MoveDw, 0, 30, 0},
	
	// {MoveDw_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH3, 0, 4}, //@ 待确认 260130
	// {MoveDw_Tap, FLAP_STOP, 0, 0, 0},
	// {Part_LOOP, 0, 8, 4, 0},
	// {MoveDw_Tread, TREAD_FWD_CIRCLE, 30, 0, 3},
	// {Part_EXEC, ConMth_MoveDw, 0, 30, 0},
	// {MoveDw_Tread, TREAD_REV_CIRCLE, 10, 0, 3},
	// {Part_EXEC, ConMth_MoveDw, 0, 30, 0},

	// {MoveDw_TreadB, TREAD_FIX_CIRCLE, LB_H0, 0, 4},
	// // {MoveDw_Tap, FLAP_STOP, 0, 0, 0},
	// {Part_EXEC, ConMth_MoveDw, 0, 30, 0},

	// {MoveDw_Knead, KNEAD_FWD_CNT, 5, 0, 5},
	// {MoveDw_Tap, FLAP_ACT, 0, 0, 5},
	// {Part_EXEC, ConMth_MoveDw, 0, 30, 0},

	{MoveDw_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH3, 0, 4},
	{MoveDw_TreadB, TREAD_FIX_CIRCLE, LB_0, 0, 4},
	{MoveDw_Tap, FLAP_ACT, 0, 0, 4},
	{Part_EXEC, ConMth_MoveDw, 80, 30, 0},

	{MoveDw_TreadB, TREAD_FIX_CIRCLE, LB_H0, 0, 4},
	{Part_EXEC, ConMth_MoveDw, 0, 30, 0},

	{MoveDw_Knead, KNEAD_REV_CNT, 5, 0, 5},
	{MoveDw_Tap, FLAP_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveDw, 0, 30, 0},

	{Part_EXEC, ConMth_AC, 0, 0, 0}, //* 复位标志
};

const CoreMixOtherTabItem_t MoveDwTab_FullB[] = {
	{MoveDw_Knead, KNEAD_FWD, 0, 0, 6},
	{MoveDw_TreadB, TREAD_FIX_CIRCLE, LB_H6, 0, 6},
	{MoveDw_Tap, FLAP_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveDw, 0, 30, 0},

	//* 捶打 +
	{MoveDw_Tap, FLAP_ACT, 0, 0, 4},
	{Part_LOOP, 0, 2, 12, 0},
	{MoveDw_Knead, KNEAD_FWD, 0, 0, 6},
	{MoveDw_TreadB, TREAD_FIX_CIRCLE, LB_3, 0, 2},
	{Part_EXEC, ConMth_MoveDw, 0, 30, 0},
	{MoveDw_Knead, KNEAD_1F2R, 0x97, 5, 0x62},
	{MoveDw_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 4},
	{Part_EXEC, ConMth_MoveDw, 0, 30, 0},
	{MoveDw_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH3, 0, 4},
	{Part_EXEC, ConMth_MoveDw, 0, 30, 0},
	{MoveDw_TreadB, TREAD_FIX_CIRCLE, LB_3, 0, 2},
	{Part_EXEC, ConMth_MoveDw, 0, 30, 0},
	{MoveDw_TreadB, TREAD_FIX_CIRCLE, LB_6, 0, 2}, //@ H6->6
	{Part_EXEC, ConMth_MoveDw, 0, 30, 0},

	{MoveDw_Tap, FLAP_STOP, 0, 0, 0},
	{MoveDw_Knead, KNEAD_FWD, 0, 0, 6},
	{MoveDw_TreadB, TREAD_FIX_CIRCLE, LB_3, 0, 1},
	{Part_EXEC, ConMth_MoveDw, 0, 30, 0},
	{MoveDw_Knead, KNEAD_1F2R, 0xa6, 5, 0x62}, //@ md 260201
	// {MoveDw_Knead, KNEAD_FWD_CNT, 5, 0, 3}, // * 普通
	{MoveDw_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 4},
	{Part_EXEC, ConMth_MoveDw, 0, 30, 0},
	{MoveDw_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH3, 0, 4},
	{Part_EXEC, ConMth_MoveDw, 0, 30, 0},
	{MoveDw_TreadB, TREAD_FIX_CIRCLE, LB_3, 0, 2},
	{Part_EXEC, ConMth_MoveDw, 0, 30, 0},

	{MoveDw_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 4}, //@ 待确认 260130
	{MoveDw_Tap, FLAP_STOP, 0, 0, 0},
	{Part_LOOP, 0, 5, 4, 0},
	{MoveDw_Tread, TREAD_REV_CIRCLE, 20, 0, 3},
	{Part_EXEC, ConMth_MoveDw, 0, 30, 0},
	{MoveDw_Tread, TREAD_FWD_CIRCLE, 10, 0, 3},
	{Part_EXEC, ConMth_MoveDw, 0, 30, 0},
	
	{MoveDw_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH3, 0, 4}, //@ 待确认 260130
	{MoveDw_Tap, FLAP_STOP, 0, 0, 0},
	{Part_LOOP, 0, 8, 4, 0},
	{MoveDw_Tread, TREAD_FWD_CIRCLE, 30, 0, 3},
	{Part_EXEC, ConMth_MoveDw, 0, 30, 0},
	{MoveDw_Tread, TREAD_REV_CIRCLE, 10, 0, 3},
	{Part_EXEC, ConMth_MoveDw, 0, 30, 0},

	{MoveDw_TreadB, TREAD_FIX_CIRCLE, LB_H0, 0, 4},
	// {MoveDw_Tap, FLAP_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveDw, 0, 30, 0},

	{MoveDw_Knead, KNEAD_FWD_CNT, 5, 0, 5},
	{MoveDw_Tap, FLAP_ACT, 0, 0, 5},
	{Part_EXEC, ConMth_MoveDw, 0, 30, 0},

	{MoveDw_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH3, 0, 4},
	{MoveDw_TreadB, TREAD_FIX_CIRCLE, LB_0, 0, 4},
	// {MoveDw_Tap, FLAP_ACT, 0, 0, 5},
	{Part_EXEC, ConMth_MoveDw, 80, 30, 0},

	{MoveDw_TreadB, TREAD_FIX_CIRCLE, LB_H0, 0, 4},
	{Part_EXEC, ConMth_MoveDw, 0, 30, 0},

	{MoveDw_Knead, KNEAD_REV_CNT, 5, 0, 5},
	{MoveDw_Tap, FLAP_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveDw, 0, 30, 0},

	{Part_EXEC, ConMth_AC, 0, 0, 0}, //* 复位标志
};

// 捶打脊椎上下到腰部
const CoreMixOtherTabItem_t MoveDwTab_SpineWaistPD1[] = {

	{MoveDw_Knead, KNEAD_FIX_CIRCLE, MOVEDWWIDTH2, 0, 6},
	{MoveDw_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 2},
	{MoveDw_Tap, FLAP_ACT, 0, 0, 6},
	{Part_EXEC, ConMth_MoveDw, 5, 30, 0},

	{MoveDw_TreadB, TREAD_FIX_CIRCLE, LB_7, 0, 2},
	{Part_EXEC, ConMth_MoveDw, 5, 30, 0},
	{MoveDw_TreadB, TREAD_FIX_CIRCLE, LB_4, 0, 2},
	{Part_EXEC, ConMth_MoveDw, 5, 30, 0},

	{MoveDw_Knead, KNEAD_FIX_CIRCLE, MOVEDWWIDTH4, 0, 6},
	{MoveDw_Tap, FLAP_ACT, 0, 0, 8},
	{MoveDw_TreadB, TREAD_FIX_CIRCLE, LB_7, 0, 2},
	{Part_EXEC, ConMth_MoveDw, 5, 30, 0},
	{MoveDw_TreadB, TREAD_FIX_CIRCLE, LB_4, 0, 2},
	{Part_EXEC, ConMth_MoveDw, 5, 30, 0},

	{MoveDw_Knead, KNEAD_FWD, 0, 0, 6},
	{MoveDw_Tap, FLAP_STOP, 0, 0, 0},
	{MoveDw_TreadB, TREAD_FIX_CIRCLE, LB_7, 0, 2},
	{Part_EXEC, ConMth_MoveDw, 5, 30, 0},
	{MoveDw_TreadB, TREAD_FIX_CIRCLE, LB_4, 0, 2},
	{Part_EXEC, ConMth_MoveDw, 5, 30, 0},

	// {MoveDw_Tap, FLAP_ACT, 0, 0, 6},
	// {MoveDw_Knead, KNEAD_REV, 0, 0, 6},
	// {MoveDw_TreadB, TREAD_FIX_CIRCLE, LB_1, 0, 2},
	// {Part_EXEC, ConMth_MoveDw, 5, 30, 0},
	// {MoveDw_Knead, KNEAD_FWD_CNT, 3, 0, 6},
	// {Part_EXEC, ConMth_MoveDw, 5, 30, 0},
	// {MoveDw_Knead, KNEAD_REV, 0, 0, 6},
	// {MoveDw_TreadB, TREAD_FIX_CIRCLE, LB_0, 0, 2},
	// {Part_EXEC, ConMth_MoveDw, 5, 30, 0},

	{Part_EXEC, ConMth_AC, 0, 0, 0}, // * 结束 可以发送结束标志
};

const CoreMixOtherTabItem_t M2AT_WastTN_Addr[] = {
	// AC_M2_General,	1,	1,	LY3,	M3,	0,	TZ,	S2,	W0,	0,	MSJ,	20,	CMVA,
	{MoveDw_Knead, KNEAD_FIX_CIRCLE, MOVEDWWIDTH1, 0, 4},
	{MoveDw_Tap, FLAP_STOP, 0, 0, 0},
	{MoveDw_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 6},
	{Part_EXEC, ConMth_MoveDw, 20, 30, 0},

	// AC_M2_General,	6,	2,	LY3,	M3,	0,	TZ,	S2,	W0,	4,	MSJ,	5,	CMVA,
	// LQS_UP,	M3,	8,	TZ,	S2,	W0,	4,	MSJ,	5,	CMVA,
	// LY3,	M3,	0,	TZ,	S2,	W2,	4,	MSJ,	5,	CMVA,
	// LQS_UP,	M3,	8,	TZ,	S2,	W2,	4,	MSJ,	5,	CMVA,
	// LY3,	M3,	0,	TZ,	S2,	W4,	4,	MSJ,	5,	CMVA,
	// LQS_UP,	M3,	8,	TZ,	S2,	W4,	4,	MSJ,	5,	CMVA,
	{Part_LOOP, 0, 2, 15, 0}, //* 循环次数2 条数4
	{MoveDw_Knead, KNEAD_FIX_CIRCLE, MOVEDWWIDTH1, 0, 4},
	{MoveDw_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 6},
	{Part_EXEC, ConMth_MoveDw, 5, 30, 0},
	{MoveDw_Tread, TREAD_REV_CIRCLE, 8, 0, 6},
	{Part_EXEC, ConMth_MoveDw, 5, 30, 0},
	{MoveDw_Knead, KNEAD_FIX_CIRCLE, MOVEDWWIDTH3, 0, 4},
	{MoveDw_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 6},
	{Part_EXEC, ConMth_MoveDw, 5, 30, 0},
	{MoveDw_Tread, TREAD_REV_CIRCLE, 8, 0, 6},
	{Part_EXEC, ConMth_MoveDw, 5, 30, 0},
	{MoveDw_Knead, KNEAD_FIX_CIRCLE, MOVEDWWIDTH5, 0, 4},
	{MoveDw_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 6},
	{Part_EXEC, ConMth_MoveDw, 5, 30, 0},
	{MoveDw_Tread, TREAD_REV_CIRCLE, 8, 0, 6},
	{Part_EXEC, ConMth_MoveDw, 5, 30, 0},

	// AC_M2_General,	1,	1,	LDW,	M3,	0,	TZ,	S2,	W0,	0,	MSJ,	5,	CMVA,
	{MoveDw_Knead, KNEAD_FIX_CIRCLE, MOVEDWWIDTH1, 0, 4},
	{MoveDw_TreadB, TREAD_FIX_CIRCLE, LB_0, 0, 6},

	// AC_M2Stop,
	{Part_EXEC, ConMth_AC, 0, 0, 0}, // * 结束 可以发送结束标志
};

const CoreMixOtherTabItem_t M2AT_MassWast_Addr[] = {
	// AC_M2_General,	7,	1,	LY3,	M3,	0,	TZ,	S2,	W0,	0,	MSJ,	20,	CMVA,
	// LTZ,	M3,	0,	TZ,	S2,	W0,	2,	MSJ,	20,	CMVA,
	// LTZ,	M3,	0,	RNZ,	S2,	W0,	2,	MCY,	3,	CMVA,
	// LTZ,	M3,	0,	RNF,	S2,	W0,	2,	MCY,	3,	CMVA,
	// LY2,	M3,	0,	TZ,	S2,	W0,	3,	MSJ,	10,	CMVA,
	// LTZ,	M3,	0,	RNZ,	S2,	W0,	3,	MCY,	3,	CMVA,
	// LTZ,	M3,	0,	RNF,	S2,	W0,	3,	MCY,	3,	CMVA,
	{MoveDw_Knead, KNEAD_FIX_CIRCLE, MOVEDWWIDTH1, 0, 4},
	{MoveDw_Tap, FLAP_STOP, 0, 0, 0},
	{MoveDw_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 6},
	{Part_EXEC, ConMth_MoveDw, 20, 30, 0},
	{MoveDw_Knead, KNEAD_FWD_CNT, 3, 0, 4},
	{Part_EXEC, ConMth_MoveDw, 5, 30, 0},
	{MoveDw_Knead, KNEAD_REV_CNT, 3, 0, 4},
	{Part_EXEC, ConMth_MoveDw, 5, 30, 0},
	{MoveDw_Knead, KNEAD_FIX_CIRCLE, MOVEDWWIDTH1, 0, 4},
	{Part_EXEC, ConMth_MoveDw, 10, 30, 0},
	{MoveDw_Knead, KNEAD_FWD_CNT, 3, 0, 4},
	{Part_EXEC, ConMth_MoveDw, 5, 30, 0},
	{MoveDw_Knead, KNEAD_REV_CNT, 3, 0, 4},
	{Part_EXEC, ConMth_MoveDw, 5, 30, 0},

	// AC_M2_General,	6,	2,	LY3,	M3,	0,	TZ,	S2,	W0,	4,	MSJ,	5,	CMVA,
	// LY2,	M3,	0,	TZ,	S2,	W0,	4,	MSJ,	5,	CMVA,
	// LY3,	M3,	0,	TZ,	S2,	W2,	4,	MSJ,	5,	CMVA,
	// LY2,	M3,	0,	TZ,	S2,	W2,	4,	MSJ,	5,	CMVA,
	// LY3,	M3,	0,	TZ,	S2,	W4,	4,	MSJ,	5,	CMVA,
	// LY2,	M3,	0,	TZ,	S2,	W4,	4,	MSJ,	5,	CMVA,
	{Part_LOOP, 0, 2, 15, 0}, //* 循环次数2 条数4
	{MoveDw_Knead, KNEAD_FIX_CIRCLE, MOVEDWWIDTH1, 0, 4},
	{MoveDw_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 6},
	{Part_EXEC, ConMth_MoveDw, 5, 30, 0},
	{MoveDw_TreadB, TREAD_FIX_CIRCLE, LB_3, 0, 6},
	{Part_EXEC, ConMth_MoveDw, 5, 30, 0},
	{MoveDw_Knead, KNEAD_FIX_CIRCLE, MOVEDWWIDTH3, 0, 4},
	{MoveDw_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 6},
	{Part_EXEC, ConMth_MoveDw, 5, 30, 0},
	{MoveDw_TreadB, TREAD_FIX_CIRCLE, LB_3, 0, 6},
	{Part_EXEC, ConMth_MoveDw, 5, 30, 0},
	{MoveDw_Knead, KNEAD_FIX_CIRCLE, MOVEDWWIDTH5, 0, 4},
	{MoveDw_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 6},
	{Part_EXEC, ConMth_MoveDw, 5, 30, 0},
	{MoveDw_TreadB, TREAD_FIX_CIRCLE, LB_3, 0, 6},
	{Part_EXEC, ConMth_MoveDw, 5, 30, 0},

	// AC_M2_General,	2,	2,	LY3,	M3,	0,	RNF,	S2,	W0,	3,	MCY,	3,	CMVA,
	// LY2,	M3,	0,	RNZFS,	S2,	W0,	3,	MCY,	5,	CMVA,
	{Part_LOOP, 0, 2, 6, 0},
	{MoveDw_Knead, KNEAD_REV_CNT, 3, 0, 4},
	{MoveDw_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 6},
	{Part_EXEC, ConMth_MoveDw, 5, 30, 0},
	{MoveDw_Knead, KNEAD_1F2R, 0x93, 5, 0x44},
	{MoveDw_TreadB, TREAD_FIX_CIRCLE, LB_3, 0, 6},
	{Part_EXEC, ConMth_MoveDw, 5, 30, 0},

	// AC_M2_General,	1,	1,	LDW,	M3,	0,	TZ,	S2,	W0,	0,	MSJ,	5,	CMVA,
	{MoveDw_Knead, KNEAD_FIX_CIRCLE, MOVEDWWIDTH1, 0, 4},
	{MoveDw_TreadB, TREAD_FIX_CIRCLE, LB_0, 0, 6},
	{Part_EXEC, ConMth_MoveDw, 5, 30, 0},

	// AC_M2Stop,
	{Part_EXEC, ConMth_AC, 0, 0, 0}, // * 结束 可以发送结束标志
};

const CoreMixOtherTabItem_t M2AT_ButtTN_Addr[] = {
	// 腿部，臀部 分点揉捏
	// AC_M2_General,	1,	1,	LDW,	M3,	0,	TZ,	S2,	W0,	0,	MSJ,	20,	CMVA,
	{MoveDw_Knead, KNEAD_FIX_CIRCLE, MOVEDWWIDTH1, 0, 4},
	{MoveDw_Tap, FLAP_STOP, 0, 0, 0},
	{MoveDw_TreadB, TREAD_FIX_CIRCLE, LB_0, 0, 6},
	{Part_EXEC, ConMth_MoveDw, 20, 30, 0},

	// 臀部推拿
	// AC_M2_General,	6,	2,	LDW,	M3,	0,	TZ,	S2,	W0,	4,	MSJ,	5,	CMVA,
	// LBU1,	M3,	0,	TZ,	S2,	W0,	4,	MSJ,	5,	CMVA,
	// LDW,	M3,	0,	TZ,	S2,	W2,	4,	MSJ,	5,	CMVA,
	// LBU1,	M3,	0,	TZ,	S2,	W2,	4,	MSJ,	5,	CMVA,
	// LDW,	M3,	0,	TZ,	S2,	W4,	4,	MSJ,	5,	CMVA,
	// LBU1,	M3,	0,	TZ,	S2,	W4,	4,	MSJ,	5,	CMVA,
	{Part_LOOP, 0, 2, 15, 0},
	{MoveDw_Knead, KNEAD_FIX_CIRCLE, MOVEDWWIDTH1, 0, 4},
	{MoveDw_TreadB, TREAD_FIX_CIRCLE, LB_0, 0, 6},
	{Part_EXEC, ConMth_MoveDw, 5, 30, 0},
	{MoveDw_TreadB, TREAD_FIX_CIRCLE, LB_1, 0, 6},
	{Part_EXEC, ConMth_MoveDw, 5, 30, 0},
	{MoveDw_Knead, KNEAD_FIX_CIRCLE, MOVEDWWIDTH3, 0, 4},
	{MoveDw_TreadB, TREAD_FIX_CIRCLE, LB_0, 0, 6},
	{Part_EXEC, ConMth_MoveDw, 5, 30, 0},
	{MoveDw_TreadB, TREAD_FIX_CIRCLE, LB_1, 0, 6},
	{Part_EXEC, ConMth_MoveDw, 5, 30, 0},
	{MoveDw_Knead, KNEAD_FIX_CIRCLE, MOVEDWWIDTH5, 0, 4},
	{MoveDw_TreadB, TREAD_FIX_CIRCLE, LB_0, 0, 6},
	{Part_EXEC, ConMth_MoveDw, 5, 30, 0},
	{MoveDw_TreadB, TREAD_FIX_CIRCLE, LB_1, 0, 6},
	{Part_EXEC, ConMth_MoveDw, 5, 30, 0},

	// AC_M2_General,	1,	1,	LDW,	M3,	0,	TZ,	S2,	W0,	0,	MSJ,	200,	CMVA,
	{MoveDw_Knead, KNEAD_FIX_CIRCLE, MOVEDWWIDTH1, 0, 4},
	{MoveDw_TreadB, TREAD_FIX_CIRCLE, LB_0, 0, 6},
	{Part_EXEC, ConMth_MoveDw, 200, 30, 0},

	// AC_M2Stop,
	{Part_EXEC, ConMth_AC, 0, 0, 0}, // * 结束 可以发送结束标志
};

//* 下机芯工作模式索引表
ActionTabAttr_t MoveDwMode_Addr[] = {
	{DWTB_CLOSE, NULL},
	{DWTB_0RPZ, (void *)MoveDwTab_0_RPZ},
	{DWTB_FullA, (void *)MoveDwTab_FullA},
	{DWTB_FullB, (void *)MoveDwTab_FullB},
	{DWTB_SWPD1, (void *)MoveDwTab_SpineWaistPD1},
	{DWTB_WastTN, (void *)M2AT_WastTN_Addr},
	{DWTB_MassWast, (void *)M2AT_MassWast_Addr},
	{DWTB_ButtTN, (void *)M2AT_ButtTN_Addr},
};

#define MoveDwModeMaxAddr sizeof(MoveDwMode_Addr) / sizeof(MoveDwMode_Addr[0])

void MoveDwMassMode_Load(u8 ID)
{
	u8 *temp;

	if (ID >= MoveDwModeMaxAddr)
		return;

	temp = MoveDwMode_Addr[ID].pTab;

	if (ID == DWTB_CLOSE)
	{
		AT_TableStop(Table_MoveDw);
	}
	else
	{
		AT_TableStart(Table_MoveDw, temp);
		AtTabSet_ID(Table_MoveDw, ID);
	}
}

#endif // USE_M2_4D
