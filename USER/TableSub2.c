#include "Table.h"

//* BJB
const CoreMixOtherTabItem_t Sub2Tab_BJB[] = {
	// 拔筋背部
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 2},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH5, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_LOOP, ConMth_Loop, 0, 8, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 20, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_REV_ABS, MOVEUPWIDTH1, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_REV_ABS, MOVEUPWIDTH5, 0, 6},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_STOP, 0, 0, 0, 0},
};

//* LJA
const CoreMixOtherTabItem_t Sub2Tab_LJA[] = {
	// 向上推拿颈部回来3次
	// AC_M1Wise,	LJA,	0x84,
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, 30, 0, 2}, // * 修改 35-25 251022
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 10, 0, 2},

	{Part_LOOP, ConMth_Loop, 0, 4, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H0, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H2, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},
	{Part_STOP, 0, 0, 0, 0},
};

//* TLA
const CoreMixOtherTabItem_t Sub2Tab_TLA[] = {
	//  AC_M1Wise,TLA,0x84,
	{Part_LOOP, ConMth_Loop, 0, 6, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, 30, 0, 2},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_0, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 3, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH5, 0, 2},
	{MoveUp_Tread, TREAD_REV_CIRCLE, 5, 0, 8},
	{Part_EXEC, ConMth_MoveUp, 3, 30, 0},
	{Part_STOP, 0, 0, 0, 0},
};

//* TLB 提拉颈部
const CoreMixOtherTabItem_t Sub2Tab_TLB[] = {
	// 颈部提拉：渐进夹持 → 顶出提拉 → 打开后退释放 循环

	{Part_LOOP, ConMth_Loop, 0, 12, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1 + 5, 0, 4}, // * 接近
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H0, 0, 2}, // * 走到顶部
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 12, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 2}, // * 渐进合拢
	{Part_EXEC, ConMth_MoveUp, 15, 30, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 18, 0, 2}, // * 顶出提拉
	{Part_EXEC, ConMth_MoveUp, 25, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH3, 0, 2}, // * 打开
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 2}, // * 收回释放
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H1, 0, 2}, // * 后退脱离
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},

	{Part_STOP, 0, 0, 0, 0},
};

//* TLC 提拉颈部(强力度)
const CoreMixOtherTabItem_t Sub2Tab_TLC[] = {
	// 颈部提拉：渐进夹持 → 顶出提拉 → 打开后退释放 循环, 较强力度

	{Part_LOOP, ConMth_Loop, 0, 12, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1 + 5, 0, 5}, // * 接近
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H0, 0, 3}, // * 走到顶部
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 12, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 3}, // * 渐进合拢
	{Part_EXEC, ConMth_MoveUp, 10, 30, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 20, 0, 2}, // * 顶出提拉
	{Part_EXEC, ConMth_MoveUp, 20, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH3, 0, 3}, // * 打开
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 3}, // * 收回释放
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H1, 0, 3}, // * 后退脱离
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},

	{Part_STOP, 0, 0, 0, 0},
};

//* TLD 肩部揉按(舒适)
const CoreMixOtherTabItem_t Sub2Tab_TLD[] = {
	// 肩部揉按：渐进夹持 → 顶出揉按 → 打开释放 循环

	{Part_LOOP, ConMth_Loop, 0, 12, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1 + 5, 0, 4}, // * 接近
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 2}, // * 走到肩部
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 10, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 2}, // * 渐进合拢
	{Part_EXEC, ConMth_MoveUp, 15, 30, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 16, 0, 2}, // * 顶出揉按
	{Part_EXEC, ConMth_MoveUp, 25, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH3, 0, 2}, // * 打开
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 2}, // * 收回释放
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_1, 0, 2}, // * 上移脱离
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},

	{Part_STOP, 0, 0, 0, 0},
};

//* TLE 肩部揉按(强力度)
const CoreMixOtherTabItem_t Sub2Tab_TLE[] = {
	// 肩部揉按：渐进夹持 → 顶出揉按 → 打开释放 循环, 较强力度

	{Part_LOOP, ConMth_Loop, 0, 12, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1 + 5, 0, 5}, // * 接近
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 3}, // * 走到肩部
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 12, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 3}, // * 渐进合拢
	{Part_EXEC, ConMth_MoveUp, 10, 30, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 22, 0, 2}, // * 顶出揉按
	{Part_EXEC, ConMth_MoveUp, 20, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH3, 0, 3}, // * 打开
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 3}, // * 收回释放
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_1, 0, 3}, // * 上移脱离
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},

	{Part_STOP, 0, 0, 0, 0},
};

//* TLF 腰部顶揉(舒适)
const CoreMixOtherTabItem_t Sub2Tab_TLF[] = {
	// 腰部顶揉：渐进夹持 → 顶出深揉 → 打开释放 循环

	{Part_LOOP, ConMth_Loop, 0, 12, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH2, 0, 4}, // * 接近
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_8, 0, 2}, // * 走到腰部
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 15, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1 + 5, 0, 2}, // * 渐进合拢
	{Part_EXEC, ConMth_MoveUp, 15, 30, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 25, 0, 2}, // * 顶出深揉
	{Part_EXEC, ConMth_MoveUp, 25, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH3, 0, 2}, // * 打开
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 2}, // * 收回释放
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_9, 0, 2}, // * 下行脱离
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},

	{Part_STOP, 0, 0, 0, 0},
};

//* TLG 腰部顶揉(强力度)
const CoreMixOtherTabItem_t Sub2Tab_TLG[] = {
	// 腰部顶揉：渐进夹持 → 顶出深揉 → 打开释放 循环, 较强力度

	{Part_LOOP, ConMth_Loop, 0, 12, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH2, 0, 5}, // * 接近
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_8, 0, 3}, // * 走到腰部
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 18, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1 + 5, 0, 3}, // * 渐进合拢
	{Part_EXEC, ConMth_MoveUp, 10, 30, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 30, 0, 2}, // * 顶出深揉
	{Part_EXEC, ConMth_MoveUp, 20, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH3, 0, 3}, // * 打开
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 3}, // * 收回释放
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_9, 0, 3}, // * 下行脱离
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},

	{Part_STOP, 0, 0, 0, 0},
};

//* TLH 背部推揉(舒适)
const CoreMixOtherTabItem_t Sub2Tab_TLH[] = {
	// 背部推揉：渐进夹持 → 顶出推揉 → 打开释放 循环

	{Part_LOOP, ConMth_Loop, 0, 12, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1 + 5, 0, 4}, // * 接近
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_5, 0, 2}, // * 走到中背
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 10, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 2}, // * 渐进合拢
	{Part_EXEC, ConMth_MoveUp, 15, 30, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 14, 0, 2}, // * 顶出推揉
	{Part_EXEC, ConMth_MoveUp, 25, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH3, 0, 2}, // * 打开
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 2}, // * 收回释放
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_6, 0, 2}, // * 下行脱离
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},

	{Part_STOP, 0, 0, 0, 0},
};

//* TLI 背部推揉(强力度)
const CoreMixOtherTabItem_t Sub2Tab_TLI[] = {
	// 背部推揉：渐进夹持 → 顶出推揉 → 打开释放 循环, 较强力度

	{Part_LOOP, ConMth_Loop, 0, 12, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1 + 5, 0, 5}, // * 接近
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_5, 0, 3}, // * 走到中背
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 12, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 3}, // * 渐进合拢
	{Part_EXEC, ConMth_MoveUp, 10, 30, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 20, 0, 2}, // * 顶出推揉
	{Part_EXEC, ConMth_MoveUp, 20, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH3, 0, 3}, // * 打开
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 3}, // * 收回释放
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_6, 0, 3}, // * 下行脱离
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},

	{Part_STOP, 0, 0, 0, 0},
};

//* 子程序工作模式索引表
ActionTabAttr_t Sub2Mode_Addr[] = {
	{SUB2TB_CLOSE, NULL},
	{SUB2TB_BJB, (void *)Sub2Tab_BJB},
	{SUB2TB_LJA, (void *)Sub2Tab_LJA},
	{SUB2TB_TLA, (void *)Sub2Tab_TLA},
	{SUB2TB_TLB, (void *)Sub2Tab_TLB}, // * 提拉颈部
	{SUB2TB_TLC, (void *)Sub2Tab_TLC}, // * 提拉颈部(强力度)
	{SUB2TB_TLD, (void *)Sub2Tab_TLD}, // * 肩部揉按(舒适)
	{SUB2TB_TLE, (void *)Sub2Tab_TLE}, // * 肩部揉按(强力度)
	{SUB2TB_TLF, (void *)Sub2Tab_TLF}, // * 腰部顶揉(舒适)
	{SUB2TB_TLG, (void *)Sub2Tab_TLG}, // * 腰部顶揉(强力度)
	{SUB2TB_TLH, (void *)Sub2Tab_TLH}, // * 背部推揉(舒适)
	{SUB2TB_TLI, (void *)Sub2Tab_TLI}, // * 背部推揉(强力度)
};

#define Sub2ModeMaxAddr sizeof(Sub2Mode_Addr) / sizeof(Sub2Mode_Addr[0])

void Sub2MassMode_Load(u8 ID)
{
	u8 *temp;

	if (ID >= Sub2ModeMaxAddr)
		return;

	temp = Sub2Mode_Addr[ID].pTab;

	if (ID == SUB2TB_CLOSE)
	{
		AT_TableStop(Table_Sub2);
	}
	else
	{
		AT_TableStart(Table_Sub2, temp);
		AtTabSet_ID(Table_Sub2, ID);
	}
}
