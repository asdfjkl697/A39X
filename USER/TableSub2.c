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

	{Part_LOOP, ConMth_Loop, 0, 15, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH4, 0, 5},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 16, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0}, // 1
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1 + 2, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 20, 30, 0}, // 2
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_C0, 0, 2},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 20, 0, 1},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0}, // 3
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1 + 3, 0, 1},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0}, // 4
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH2, 0, 1},
	{Part_EXEC, ConMth_MoveUp, 15, 30, 0}, // 4
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1 + 3, 0, 1},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0}, // 4

	// {MoveUp_Tread, TREAD_REV_CIRCLE, 3, 0, 3},
	// {MoveUp_Flex, FLEX_FIX_CIRCLE, 5, 0, 2},
	// {Part_EXEC, ConMth_MoveUp, 30, 30, 0}, // 5

	{Part_STOP, 0, 0, 0, 0},
};

//* TLC 提拉颈部(强力度)
const CoreMixOtherTabItem_t Sub2Tab_TLC[] = {
	// 颈部提拉：渐进夹持 → 顶出提拉 → 打开后退释放 循环, 较强力度

	{Part_LOOP, ConMth_Loop, 0, 12, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1 + 5, 0, 5}, // * 接近
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H0, 0, 3},			  // * 走到顶部
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 12, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 3}, // * 渐进合拢
	{Part_EXEC, ConMth_MoveUp, 10, 30, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 20, 0, 2}, // * 顶出提拉
	{Part_EXEC, ConMth_MoveUp, 20, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH3, 0, 3}, // * 打开
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 3},			  // * 收回释放
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H1, 0, 3},		  // * 后退脱离
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},

	{Part_STOP, 0, 0, 0, 0},
};

//* TLD 肩部揉按(舒适)
const CoreMixOtherTabItem_t Sub2Tab_TLD[] = {
	// 肩部揉按：渐进夹持 → 顶出揉按 → 打开释放 循环

	{Part_LOOP, ConMth_Loop, 0, 12, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1 + 5, 0, 4}, // * 接近
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 2},			  // * 走到肩部
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 10, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 2}, // * 渐进合拢
	{Part_EXEC, ConMth_MoveUp, 15, 30, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 16, 0, 2}, // * 顶出揉按
	{Part_EXEC, ConMth_MoveUp, 25, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH3, 0, 2}, // * 打开
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 2},			  // * 收回释放
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_1, 0, 2},		  // * 上移脱离
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},

	{Part_STOP, 0, 0, 0, 0},
};

//* TLE 肩部揉按(强力度)
const CoreMixOtherTabItem_t Sub2Tab_TLE[] = {
	// 肩部揉按：渐进夹持 → 顶出揉按 → 打开释放 循环, 较强力度

	{Part_LOOP, ConMth_Loop, 0, 12, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1 + 5, 0, 5}, // * 接近
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 3},			  // * 走到肩部
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 12, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 3}, // * 渐进合拢
	{Part_EXEC, ConMth_MoveUp, 10, 30, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 22, 0, 2}, // * 顶出揉按
	{Part_EXEC, ConMth_MoveUp, 20, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH3, 0, 3}, // * 打开
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 3},			  // * 收回释放
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_1, 0, 3},		  // * 上移脱离
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},

	{Part_STOP, 0, 0, 0, 0},
};

//* TLF 腰部顶揉(舒适)
const CoreMixOtherTabItem_t Sub2Tab_TLF[] = {
	// 腰部顶揉：渐进夹持 → 顶出深揉 → 打开释放 循环

	{Part_LOOP, ConMth_Loop, 0, 12, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH2, 0, 4}, // * 接近
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_8, 0, 2},		  // * 走到腰部
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 15, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1 + 5, 0, 2}, // * 渐进合拢
	{Part_EXEC, ConMth_MoveUp, 15, 30, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 25, 0, 2}, // * 顶出深揉
	{Part_EXEC, ConMth_MoveUp, 25, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH3, 0, 2}, // * 打开
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 2},			  // * 收回释放
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_9, 0, 2},		  // * 下行脱离
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},

	{Part_STOP, 0, 0, 0, 0},
};

//* TLG 腰部顶揉(强力度)
const CoreMixOtherTabItem_t Sub2Tab_TLG[] = {
	// 腰部顶揉：渐进夹持 → 顶出深揉 → 打开释放 循环, 较强力度

	{Part_LOOP, ConMth_Loop, 0, 12, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH2, 0, 5}, // * 接近
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_8, 0, 3},		  // * 走到腰部
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 18, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1 + 5, 0, 3}, // * 渐进合拢
	{Part_EXEC, ConMth_MoveUp, 10, 30, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 30, 0, 2}, // * 顶出深揉
	{Part_EXEC, ConMth_MoveUp, 20, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH3, 0, 3}, // * 打开
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 3},			  // * 收回释放
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_9, 0, 3},		  // * 下行脱离
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},

	{Part_STOP, 0, 0, 0, 0},
};

//* TLH 背部推揉(舒适)
const CoreMixOtherTabItem_t Sub2Tab_TLH[] = {
	// 背部推揉：渐进夹持 → 顶出推揉 → 打开释放 循环

	{Part_LOOP, ConMth_Loop, 0, 12, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1 + 5, 0, 4}, // * 接近
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_5, 0, 2},			  // * 走到中背
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 10, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 2}, // * 渐进合拢
	{Part_EXEC, ConMth_MoveUp, 15, 30, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 14, 0, 2}, // * 顶出推揉
	{Part_EXEC, ConMth_MoveUp, 25, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH3, 0, 2}, // * 打开
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 2},			  // * 收回释放
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_6, 0, 2},		  // * 下行脱离
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},

	{Part_STOP, 0, 0, 0, 0},
};

//* TLI 背部推揉(强力度)
const CoreMixOtherTabItem_t Sub2Tab_TLI[] = {
	// 背部推揉：渐进夹持 → 顶出推揉 → 打开释放 循环, 较强力度

	{Part_LOOP, ConMth_Loop, 0, 12, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1 + 5, 0, 5}, // * 接近
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_5, 0, 3},			  // * 走到中背
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 12, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 3}, // * 渐进合拢
	{Part_EXEC, ConMth_MoveUp, 10, 30, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 20, 0, 2}, // * 顶出推揉
	{Part_EXEC, ConMth_MoveUp, 20, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH3, 0, 3}, // * 打开
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 3},			  // * 收回释放
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_6, 0, 3},		  // * 下行脱离
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},

	{Part_STOP, 0, 0, 0, 0},
};

//* PLSE 脉冲揉捏(快揉爆发+停顿呼吸)
const CoreMixOtherTabItem_t Sub2Tab_PLSE[] = {
	// 脉冲体感：高速揉捏3圈→停顿1.5s→反向3圈→停顿→重复

	{Part_LOOP, ConMth_Loop, 0, 8, 0},
	{MoveUp_Knead, KNEAD_FWD_CNT, 3, 0, 6}, // * 高速正揉3圈
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_REV_CNT, 3, 0, 6}, // * 高速反揉3圈
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_STOP, 0, 0, 0},   // * 停顿
	{Part_EXEC, ConMth_MoveUp, 15, 30, 0}, // * 呼吸1.5秒

	{Part_STOP, 0, 0, 0, 0},
};

//* TKSTG 拍揉交替(拍→揉→拍→揉 错落有致)
const CoreMixOtherTabItem_t Sub2Tab_TKSTG[] = {
	// 交替体感：拍打和揉捏不同时，轮流出现

	{Part_LOOP, ConMth_Loop, 0, 10, 0},
	{MoveUp_Tap, FLAP_ACT, 0, 0, 3},		// * 拍打
	{Part_EXEC, ConMth_MoveUp, 15, 30, 0},	// * 拍1.5秒
	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},		// * 停拍
	{MoveUp_Knead, KNEAD_FWD_CNT, 3, 0, 4}, // * 正揉3圈
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_REV_CNT, 3, 0, 4}, // * 反揉3圈
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_STOP, 0, 0, 0, 0},
};

//* RMPUP 渐快揉捏(慢→中→快 层层递进)
const CoreMixOtherTabItem_t Sub2Tab_RMPUP[] = {
	// 加速体感：揉捏速度逐级攀升，产生期待感

	{Part_LOOP, ConMth_Loop, 0, 10, 0},
	{MoveUp_Knead, KNEAD_FWD_CNT, 2, 0, 2}, // * 慢速2圈
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_FWD_CNT, 3, 0, 4}, // * 中速3圈
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_FWD_CNT, 3, 0, 6}, // * 快速3圈
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_STOP, 0, 0, 0},   // * 停顿
	{Part_EXEC, ConMth_MoveUp, 10, 30, 0}, // * 呼吸1秒

	{Part_STOP, 0, 0, 0, 0},
};

//* INWAV 力度波浪(深→浅→深 如呼吸起伏)
const CoreMixOtherTabItem_t Sub2Tab_INWAV[] = {
	// 波浪体感：Flex深度交替，像潮汐呼吸

	{Part_LOOP, ConMth_Loop, 0, 8, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 22, 0, 3}, // * 深层顶出
	{Part_EXEC, ConMth_MoveUp, 25, 30, 0},	  // * 保持2.5秒
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 8, 0, 3},  // * 浅层收回
	{Part_EXEC, ConMth_MoveUp, 20, 30, 0},	  // * 保持2秒
	{MoveUp_Knead, KNEAD_FWD_CNT, 3, 0, 4},	  // * 揉捏过渡
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_STOP, 0, 0, 0, 0},
};

//* TNFN 推拿风池穴
const CoreMixOtherTabItem_t Sub2Tab_TNFN[] = {
	// 推拿风池穴：窄幅揉捏+上下推按 LB_H0↔LB_1 风池穴区域
	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, 22, 0, 2}, // * 窄幅揉捏贴近

	{Part_LOOP, ConMth_Loop, 0, 6, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_C0, 0, 8}, // * 推到风池穴(颅底)
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 16, 0, 2},		// * 顶出深压风池
	{Part_EXEC, ConMth_MoveUp, 12, 30, 0},			// * 保持1.2s
	{MoveUp_Tread, TREAD_REV_CIRCLE, 20, 0, 8},		// * 推到颈上放松
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 16, 0, 2},		// * 收回过渡
	{Part_EXEC, ConMth_MoveUp, 8, 30, 0},			// * 保持0.8s
	{Part_STOP, 0, 0, 0, 0},
};

//* RNFN 揉捏风池穴
const CoreMixOtherTabItem_t Sub2Tab_RNFN[] = {
	// 揉捏风池穴：定点渐进揉捏 LB_H0 风池穴 舒适力度
	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	// {MoveUp_Knead, KNEAD_FIX_CIRCLE, 52, 0, 2},		// * 窄幅揉捏贴近
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_C0, 0, 2}, // * 走到风池穴
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 10, 0, 2},		// * 顶出深压风池
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},			// * 保持1.2s

	{Part_LOOP, ConMth_Loop, 0, 8, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_C0, 0, 4}, // * 走到风池穴
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, 52, 0, 4},		// * 接近宽度
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 10, 0, 4},		// * 初始顶出
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},			// * 到达
	{MoveUp_Tread, TREAD_FWD_CIRCLE, 4, 0, 4},		// *
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, 60, 0, 4},		// todo 修改 260730
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 12, 0, 4},		// * 顶出深压风池
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},			// * 夹持1.5s

	{Part_STOP, 0, 0, 0, 0},
};

//* 正反揉捏
const CoreMixOtherTabItem_t Sub2Tab_ZFRN[] = {
	{Part_LOOP, ConMth_Loop, 0, 4, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 20, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH5, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{Part_STOP, 0, 0, 0, 0},
};

//* 正反揉捏 正揉1
const CoreMixOtherTabItem_t Sub2Tab_ZFRNT[] = {
	{Part_LOOP, ConMth_Loop, 0, 4, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH5, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{Part_STOP, 0, 0, 0, 0},
};

//* 正反揉捏A 窄中揉捏
const CoreMixOtherTabItem_t Sub2Tab_ZFRNA[] = {
	{Part_LOOP, ConMth_Loop, 0, 4, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 20, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH3, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{Part_STOP, 0, 0, 0, 0},
};

//* 正反揉捏B 宽中揉捏
const CoreMixOtherTabItem_t Sub2Tab_ZFRNB[] = {
	{Part_LOOP, ConMth_Loop, 0, 4, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH3, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 20, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH5, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{Part_STOP, 0, 0, 0, 0},
};

//* 正反揉捏C 窄揉
const CoreMixOtherTabItem_t Sub2Tab_ZFRNC[] = {
	{Part_LOOP, ConMth_Loop, 0, 4, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 20, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH2, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{Part_STOP, 0, 0, 0, 0},
};

//* 正反揉捏D 指压窄揉
const CoreMixOtherTabItem_t Sub2Tab_ZFRND[] = {
	{Part_LOOP, ConMth_Loop, 0, 16, 0},
	{MoveUp_Flex, FLEX_FWD_CIRCLE, 4, 0, 4},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 20, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH2, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 20, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH2, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 20, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH2, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Flex, FLEX_REV_CIRCLE, 4, 0, 4},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH5, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{Part_STOP, 0, 0, 0, 0},
};

//* 正反揉捏E 指压窄揉1
const CoreMixOtherTabItem_t Sub2Tab_ZFRNE[] = {
	{Part_LOOP, ConMth_Loop, 0, 16, 0},
	{MoveUp_Flex, FLEX_FWD_CIRCLE, 4, 0, 4},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 20, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH2, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 20, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH2, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 20, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH2, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Flex, FLEX_REV_CIRCLE, 4, 0, 4},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH3, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{Part_STOP, 0, 0, 0, 0},
};

//* 正反揉捏F 指压中揉
const CoreMixOtherTabItem_t Sub2Tab_ZFRNF[] = {
	{Part_LOOP, ConMth_Loop, 0, 16, 0},
	{MoveUp_Flex, FLEX_FWD_CIRCLE, 4, 0, 4},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 20, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH3, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 20, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH3, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 20, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH3, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Flex, FLEX_REV_CIRCLE, 4, 0, 4},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH5, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{Part_STOP, 0, 0, 0, 0},
};

//* 正反揉捏G 指压宽揉
const CoreMixOtherTabItem_t Sub2Tab_ZFRNG[] = {
	{Part_LOOP, ConMth_Loop, 0, 16, 0},
	{MoveUp_Flex, FLEX_FWD_CIRCLE, 4, 0, 4},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH3, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 20, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH5, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH3, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 20, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH5, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH3, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 20, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH5, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Flex, FLEX_REV_CIRCLE, 4, 0, 4},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH5, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{Part_STOP, 0, 0, 0, 0},
};

//* 正反揉捏G 指压宽揉
const CoreMixOtherTabItem_t Sub2Tab_DYD[] = {
	{Part_LOOP, ConMth_Loop, 0, 4, 0},
	{MoveUp_Flex, FLEX_FWD_CIRCLE, 4, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 25, 30, 0},
	{MoveUp_Flex, FLEX_REV_CIRCLE, 4, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{Part_STOP, 0, 0, 0, 0},
};

//* 子程序工作模式索引表
ActionTabAttr_t Sub2Mode_Addr[] = {
	{SUB2TB_CLOSE, NULL},
	{SUB2TB_BJB, (void *)Sub2Tab_BJB},
	{SUB2TB_LJA, (void *)Sub2Tab_LJA},
	{SUB2TB_TLA, (void *)Sub2Tab_TLA},
	{SUB2TB_TLB, (void *)Sub2Tab_TLB},	   // * 提拉颈部
	{SUB2TB_TLC, (void *)Sub2Tab_TLC},	   // * 提拉颈部(强力度)
	{SUB2TB_TLD, (void *)Sub2Tab_TLD},	   // * 肩部揉按(舒适)
	{SUB2TB_TLE, (void *)Sub2Tab_TLE},	   // * 肩部揉按(强力度)
	{SUB2TB_TLF, (void *)Sub2Tab_TLF},	   // * 腰部顶揉(舒适)
	{SUB2TB_TLG, (void *)Sub2Tab_TLG},	   // * 腰部顶揉(强力度)
	{SUB2TB_TLH, (void *)Sub2Tab_TLH},	   // * 背部推揉(舒适)
	{SUB2TB_TLI, (void *)Sub2Tab_TLI},	   // * 背部推揉(强力度)
	{SUB2TB_PLSE, (void *)Sub2Tab_PLSE},   // * 脉冲揉捏(快揉+停顿)
	{SUB2TB_TKSTG, (void *)Sub2Tab_TKSTG}, // * 拍揉交替(拍→揉→拍→揉)
	{SUB2TB_RMPUP, (void *)Sub2Tab_RMPUP}, // * 渐快揉捏(慢→中→快)
	{SUB2TB_INWAV, (void *)Sub2Tab_INWAV}, // * 力度波浪(深→浅→深)
	{SUB2TB_TNFN, (void *)Sub2Tab_TNFN},   // * 推拿风池穴
	{SUB2TB_RNFN, (void *)Sub2Tab_RNFN},   // * 揉捏风池穴
	{SUB2TB_ZFRN, (void *)Sub2Tab_ZFRN},
	{SUB2TB_ZFRNA, (void *)Sub2Tab_ZFRNA},
	{SUB2TB_ZFRNB, (void *)Sub2Tab_ZFRNB},
	{SUB2TB_ZFRNC, (void *)Sub2Tab_ZFRNC},
	{SUB2TB_ZFRND, (void *)Sub2Tab_ZFRND},
	{SUB2TB_ZFRNE, (void *)Sub2Tab_ZFRNE},
	{SUB2TB_ZFRNF, (void *)Sub2Tab_ZFRNF},
	{SUB2TB_ZFRNG, (void *)Sub2Tab_ZFRNG},
	{SUB2TB_ZFRNT, (void *)Sub2Tab_ZFRNT},
	{SUB2TB_DYD, (void *)Sub2Tab_DYD},
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

