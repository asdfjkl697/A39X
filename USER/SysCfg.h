/*
 *文件描述：功能开关状态定义
 *
 */

#define USE_Health_Check 1
#define Health_Check_Main (USE_Health_Check & 1)        // 通过主板连接心率模块
#define Health_Check_Key_OneLine (USE_Health_Check & 0) // 通过单线通信快捷按键板连接

// #define USART1_DMA

// #define DEBUG_FLAG //todo 调试用

#define AUTO_AIRBYTABLE 1 /* 运行自动程序时，气囊动作通过自动程序表格控制 */

#define USE_M2 1

#define USE_CSD_MSUIC 0

#define USE_BLUE_APP 1

//@ 老化配置
// #define J0201_LC
// #define J0201_LC_2
// #define A391S_LC

//* 调试模式
#define Debug_Mode

//* 手控老协议 过渡用 250321
// #define Handle_Old_Version 

#define ENABLE_ABAB_STOP
//* 是否使用A390PCB
// #define A390PCB  
//@ 以下定义机型
 #define A391S_1
//#define A391S_5
// #define A391S_6
// #define A390S_1_Haier
// #define A390S_2
// #define A390S_5A 
// #define A390S_5B

#ifdef A391S_1
#define LEG_COM 1
#define P0230_LEG
#define A391_ARMKEY
#define LED2_PWM
#elif defined A391S_5
#define LEG_COM 1
#define P0230_LEG
#define A391_ARMKEY
#define LED2_PWM
#elif defined A391S_6
#define LEG_COM 1
#define P0230_LEG
#define A391_ARMKEY
#define LED2_PWM
#define ENGLISH_VOICE
#elif defined A390S_1
#define LEG_COM 1
#define P0230_LEG
#define A390_ARMKEY
#elif defined A390S_2
#define LEG_COM 1
#define P0230_LEG
#define A390_ARMKEY
#elif defined A390S_5A
#define LEG_COM 1
#define P0230_LEG
#define A390_ARMKEY
#elif defined A390S_5B
#define LEG_COM 0
#define P0153_LEG
#define A390_ARMKEY
#else
#define LEG_COM 1
#endif


/*-----------------------------------加热模块----------------------------------------------------*/
#define HEAT_BACK 1     /* 背部加热 */
#define HEAT_LEG 1      /* 腿部加热 */
#define HEAT_SHOULDER 0 /* 肩膀加热 */
#define HEAT_ARM 0      /* 手臂加热 */
#define HEAT_PJ 0       /* 披肩加热 */

/*-----------------------------------滚轮模块----------------------------------------------------*/
#define ROLLER_CALVES_EN 1 /* 腿滚 一般和腿部揉搓一起控制 */
#define ROLLER_FOOT_EN 1   /* 脚滚 */
#define ROLLER_ARM_EN 0    /* 手滚 */

/*------------------------------------手控器------------------------------------------------------*/
#define DCI_CTRL_EN 1
#define USE_S0307 (DCI_CTRL_EN & 0)

/*-----------------------------------外设通讯-------------------------------------------------------*/
#define DCI_ARMKEY_EN 1
#define DCI_APP_EN 0
#define DCI_VOICE_EN 1
