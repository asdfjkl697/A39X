#ifndef TABLE_H
#define TABLE_H

#include "config.h"
#include "MassageInit.h"
#include "AT.h"
#include <string.h>
#include "MassageEXEC.h"

#define IGNORE 0xFF

/* 推杆动作映射 */
#define FLEXST 0x00 //* 停止
#define FLEXDW 0x01
#define FLEXUP 0x02

#define BACKST 0x00
#define BACKUP 0x01
#define BACKDW 0x02
// @ 小腿升降正反确认 待修改 250208
#define LEGST 0x00
#define LEGUP 0x02
#define LEGDW 0x01

#define ZERO0 0x00
#define ZERO1 0x01
#define ZERO2 0x02
#define ZERO3 0x03

#define STRE0 0x04 /* 拉伸 全身-准备 */
#define STRE1 0x05 /* 拉伸 全身-拉伸 */
#define STRE2 0x06 /* 拉伸 小腿-准备 */
#define STRE3 0x07 /* 拉伸 小腿-拉伸 */
#define STRE4 0x08 /* 拉伸 全身-拉伸2 */
#define STRE5 0x09 /* 打包 */

#define STREZ1 0x0A /* ZERO1  */
#define STREZ2 0x0B /* ZERO2  */
#define STREZ3 0x0C /* ZERO2  */

/*【充气程序表】 -------------------------------------------------------------------*/
typedef enum
{
    FA_GAS_CLOSE = 0,
    FA_GAS_UPPERGAS,
    FA_GAS_LOWERGAS,
    FA_GAS_Full,
    FA_GAS_FullA,
    FA_GAS_FullB,
    FA_GAS_STRPREA,
    FA_GAS_STRPREB,
    FA_GAS_LC,
} GasTab_ID;

/*【子程序表】 -------------------------------------------------------------------*/
typedef enum
{
    SUBTB_CLOSE = 0,
    SUBTB_TEST,
    SUBTB_STRA, // 小腿拉伸
    SUBTB_STRB, // 小腿拉伸
    SUBTB_STRC, // 小腿拉伸
    SUBTB_STRD, // 小腿拉伸
    SUBTB_STRE, // 小腿拉伸
    SUBTB_STRF, // 小腿角度+长度拉伸
    SUBTB_STR1, // 角度拉伸
    SUBTB_STR2, // 角度拉伸
    SUBTB_STR3, // 角度拉伸
    SUBTB_STR4, // 角度拉伸
    SUBTB_STR5, // 角度拉伸
    SUBTB_STR6,
    SUBTB_STR7,
    SUBTB_STR8,
    SUBTB_STR9,
    SUBTB_FullMa1,
    SUBTB_UpperAuto21,
    SUBTB_WaistMaA,   // 腰部
    SUBTB_WaistMaB,   // 腰部
    SUBTB_WaistMaC,   // 腰部
    SUBTB_WaistMaD,   // 腰部
    SUBTB_WaistMaE,   // 顶腰
    SUBTB_BackMaA,    // 推拿背部
    SUBTB_BackMaB,    // 拍打背部
    SUBTB_BackMaC,    // 拍打背部
    SUBTB_BackMaD,    // 推拿背部
    SUBTB_BackMaE,    // 轻柔背部
    SUBTB_TNA,        // 推拿
    SUBTB_TNB,        // 推拿
    SUBTB_BackA,      // 背部
    SUBTB_BackB,      // 背部
    SUBTB_BackC,      // 背部拔筋
    SUBTB_Neck2BackA, // 颈到背
    SUBTB_Neck2BackB, // 颈到背
    SUBTB_Neck2BackC, // 颈到背
    SUBTB_Neck2BackD, // 颈到背
    SUBTB_Neck2BackE, // 颈到背
    SUBTB_Neck2BackF, // 颈到背
    SUBTB_Back2NeckA, // 背到颈
    SUBTB_Back2NeckB, // 背到颈
    SUBTB_JNNeckA,    // 夹捏颈部
    SUBTB_JNNeckB,    // 夹捏颈部
    SUBTB_JNNeckC,    // 夹捏颈部
    SUBTB_BJNeckA,    // 拔筋颈部
    SUBTB_BJNeckB,    // 拔筋颈部
    SUBTB_BJNeckC,    // 拔筋肩部
    SUBTB_JRNeckA,    // 提抓肩部
    SUBTB_PDBSHDA,    // 变速拍打肩部
    SUBTB_PDBSHDB,    // 变速拍打肩部
    SUBTB_TNSHDA,     // 搓肩
    SUBTB_TNSHDB,     // 推压肩
    SUBTB_TNSHDNeckA, // 推拿肩颈
    SUBTB_TNBACKA,    // 推拿背部
    SUBTB_TNBACKB,    // 推拿背部
    SUBTB_TNBACKC,    // 推拿背部
    SUBTB_HIPA,       // 臀部
    SUBTB_HIPB,       // 臀部
    SUBTB_HIPC,       // 臀部
    SUBTB_HIPD,       // 臀部
    SUBTB_HIPE,       // 臀部
} SubTab_ID;

/*【二级子程序表】 -------------------------------------------------------------------*/
typedef enum
{
    SUB2TB_CLOSE = 0,
    SUB2TB_BJB,
    SUB2TB_LJA, // 推拿颈部
    SUB2TB_TLA,
    SUB2TB_TLB,   // 提拉颈部
    SUB2TB_TLC,   // 提拉颈部(强力度)
    SUB2TB_TLD,   // 肩部揉按(舒适)
    SUB2TB_TLE,   // 肩部揉按(强力度)
    SUB2TB_TLF,   // 腰部顶揉(舒适)
    SUB2TB_TLG,   // 腰部顶揉(强力度)
    SUB2TB_TLH,   // 背部推揉(舒适)
    SUB2TB_TLI,   // 背部推揉(强力度)
    SUB2TB_PLSE,  // 脉冲揉捏(快揉+停顿)
    SUB2TB_TKSTG, // 拍揉交替(拍→揉→拍→揉)
    SUB2TB_RMPUP, // 渐快揉捏(慢→中→快)
    SUB2TB_INWAV, // 力度波浪(深→浅→深循环)
    SUB2TB_TNFN,  // 推拿风池穴
    SUB2TB_RNFN,  // 揉捏风池穴
    SUB2TB_ZFRN,
    SUB2TB_ZFRNA,
    SUB2TB_ZFRNB,
    SUB2TB_ZFRNC,
    SUB2TB_ZFRND,
    SUB2TB_ZFRNE,
    SUB2TB_ZFRNF,
    SUB2TB_ZFRNG,
    SUB2TB_ZFRNT,
    SUB2TB_DYD,
} Sub2Tab_ID;

/*【下机芯程序表】 -------------------------------------------------------------------*/
typedef enum
{
    DWTB_CLOSE = 0,
    DWTB_0RPZ,
    DWTB_FullA,
    DWTB_FullB,
#ifdef USE_M2_4D
    DWTB_FullC,
    DWTB_FullD,
#endif
    DWTB_SWPD1,
    DWTB_WastTN,
    DWTB_MassWast,
    DWTB_ButtTN,
} DwTab_ID;

/*【主程序表】 --------------------------------------------------------------*/
typedef enum
{
    FA_AUTO_CLOSE = 0,
    FA_AUTO_AUTO1,
    FA_AUTO_AUTO2,
    FA_AUTO_AUTO3,
    FA_AUTO_AUTO4,
    FA_AUTO_AUTO5,
    FA_AUTO_AUTO6,
    FA_AUTO_AUTO7,
    FA_AUTO_AUTO8,
    FA_AUTO_AUTO9,
    FA_AUTO_AUTO10,
    FA_AUTO_AUTO11,
    FA_AUTO_AUTO12,
    FA_AUTO_AUTO13,
    FA_AUTO_AUTO14,
    FA_AUTO_AUTO15,
    FA_AUTO_AUTO16,
    FA_AUTO_AUTO17,
    FA_AUTO_AUTO18,
    FA_AUTO_AUTO19,
    FA_AUTO_AUTO20,

    FA_AUTO_AUTO21,
    FA_AUTO_AUTO22,
    FA_AUTO_AUTO23,
    FA_AUTO_AUTO24,
    FA_AUTO_AUTO25,

    A_LC,
    A_PACK,
    A_Reset,
    A_BodyCheck,

    Update_Enable = 255,
} AutoMode_ID;

extern void MassMode_Load(u8 ID);

extern void SubMassMode_Load(u8 ID);
extern void Sub2MassMode_Load(u8 ID);
extern void MoveDwMassMode_Load(u8 ID);
extern void AirMode_Load(uint8_t ID);

#endif
