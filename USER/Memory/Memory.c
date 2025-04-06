#include "Memory.h"
#include "MoveLogic.h"
#include "FMC/fmc.h" //* 增加
// 记忆模式

Union_Save Union_Save_Value[Max_Save];

#define Max_Flash_Parameter 54
uint16_t Flash_Check_Tick;
uint16_t Flash_Parameter_Buf[Max_Flash_Parameter + 2];

u8 DIY_Save_Flag = 0;
bool Manual_Save_Flag = 0;
u32 Manual_Save_tick = 0;
u8 Manual_Save_AirState = 0;
u8 Manual_Save_AirIntensity = 0;
// u8 Manual_Save_Roll = 0;
// u8 Manual_Save_RollSpeed = 0;
// u8 Manual_Save_LegKnead = 0;

void Flash_Parameter_read(void)
{
  // u8 i;
  GDFLASH_Read(SYSTEM_PARA_ADDR, Flash_Parameter_Buf, Max_Flash_Parameter + 2);

  if (Flash_Parameter_Buf[Max_Flash_Parameter] == 0x5aa5 &&
      Flash_Parameter_Buf[Max_Flash_Parameter + 1] == 0xfeef) // 有数据
  {
    nBlueState = Flash_Parameter_Buf[0];
    // nVoiceState = Flash_Parameter_Buf[1];
    nVolume = Flash_Parameter_Buf[2];
    if (nVolume > 15)
      nVolume = 8;
    // nBlueAutoCon = Flash_Parameter_Buf[3];
    UnionState.Strack = Flash_Parameter_Buf[3]; // todo 修改
    Dispaly_Language = Flash_Parameter_Buf[4];
    // nLigntSw = Flash_Parameter_Buf[5];
    // nAutoUp = Flash_Parameter_Buf[6];
    // nLedSensor = Flash_Parameter_Buf[7];

    Memorystate = Flash_Parameter_Buf[8] & 0x0f; //* add

    // PwrLowModeFlag = Flash_Parameter_Buf[52]; //* 屏蔽

    // nScreen = Flash_Parameter_Buf[53];
    // if (nScreen > 9)
    //   nScreen = 9;
  }
  else
  {
    // nBlueState = 1;
    // nVoiceState = 1;
    nVolume = 8;
    // nScreen = 9;
    // nBlueAutoCon = 0;
    UnionState.Strack = 0xff; // todo 默认全打开
    // nAutoUp = 0;
    // nLedSensor = 0;
    Dispaly_Language = DefaultLanguage;
    // nLigntSw = 1;
    // for (i = 1; i < 21; i++)
    // {
    //   SetFavoContent(i, 0);
    // }

    // PwrLowModeFlag = 0xFFFF;
  }

  // RecFlashUse();

#ifdef RecPreSaveEnable
  RecPreFlashRead();
#endif
  memory_read();

  //  User_custom_Read();
}

void Flash_Parameter_Write(void)
{
  // u8 i;
  if (
      // nBlueState != Flash_Parameter_Buf[0] ||
      // nVoiceState != Flash_Parameter_Buf[1] ||
      nVolume != Flash_Parameter_Buf[2] ||
      UnionState.Strack != Flash_Parameter_Buf[3] ||
      Dispaly_Language != Flash_Parameter_Buf[4] ||
      // nLigntSw != Flash_Parameter_Buf[5] ||
      // nAutoUp != Flash_Parameter_Buf[6] ||
      // nLedSensor != Flash_Parameter_Buf[7] ||
      Memorystate != Flash_Parameter_Buf[8] //* add
  )
  {
    // Flash_Parameter_Buf[0] = nBlueState;
    // Flash_Parameter_Buf[1] = nVoiceState;
    Flash_Parameter_Buf[2] = nVolume;
    Flash_Parameter_Buf[3] = UnionState.Strack;
    Flash_Parameter_Buf[4] = Dispaly_Language;
    // Flash_Parameter_Buf[5] = nLigntSw;
    // Flash_Parameter_Buf[6] = nAutoUp;
    // Flash_Parameter_Buf[7] = nLedSensor;
    Flash_Parameter_Buf[8] = Memorystate; //* add

    // Flash_Parameter_Buf[52] = PwrLowModeFlag;
    // Flash_Parameter_Buf[53] = nScreen;
    Flash_Parameter_Buf[54] = 0x5aa5;
    Flash_Parameter_Buf[55] = 0xfeef;

    GDFLASH_Write(SYSTEM_PARA_ADDR, Flash_Parameter_Buf, Max_Flash_Parameter + 2);
  }
}

void memory_read(void)
{
  u8 i, j;
  u16 nCheckSum;
  GDFLASH_Read(USER_PROGRAM_ADDR, Union_Save_Value[0].Save_Value, (Max_Save_world * Max_Save));

  // 检验校验和,不正确禁止使用
  for (i = 0; i < Max_Save; i++)
  {
    nCheckSum = 0;

    for (j = 0; j < Max_Save_world; j++)
      nCheckSum += Union_Save_Value[i].Save_Value[j];

    if (nCheckSum != 0)
      Union_Save_Value[i].Save_struct_Value.Save_AutoMode = 0xff;
  }
}

void memory_write(void)
{
  GDFLASH_Write(USER_PROGRAM_ADDR, Union_Save_Value[0].Save_Value, Max_Save_world * Max_Save);
}

// 鸿蒙调取收藏
u8 HM_GetSaveMode(u8 nuser)
{
  u8 i;

  if (nuser >= Max_Save)
    return 0;

  i = Union_Save_Value[nuser].Save_struct_Value.Save_AutoMode;

  if (i <= Max_AutoMode)
    return (i + 1);

  return 0;
}

u8 HM_SaveMode(void)
{
  u8 i;

  for (i = 0; i < Max_Save; i++)
  {
    if (Union_Save_Value[i].Save_struct_Value.Save_AutoMode > Max_AutoMode)
    {
      return User_Memory_Save(i);
    }

    if (Union_Save_Value[i].Save_struct_Value.Save_AutoMode == AutoModeGet()) //* 修改
      return 0;
  }
  return 0;
}

u8 HM_DeleteSaveMode(u8 x)
{
  u8 i, j;
  u16 *np1;

  if (x >= Max_Save)
    return 0;

  // if (CollectorSwitch == x + 1)
  // 	CollectorSwitch = 0;

  if (Union_Save_Value[x].Save_struct_Value.Save_AutoMode <= Max_AutoMode)
  {
    // 储存项目左移
    if (x == (Max_Save - 1))
    {
      Union_Save_Value[x].Save_struct_Value.Save_AutoMode = 0xff;
    }
    else
    {
      np1 = Union_Save_Value[x].Save_Value;

      for (i = x; i < Max_Save - 1; i++)
      {
        for (j = 0; j < Max_Save_world; j++)
        {
          *np1 = *(np1 + Max_Save_world);
          np1++;
        }

        // if (CollectorSwitch == i + 2)
        // 	CollectorSwitch--;

        Union_Save_Value[i + 1].Save_struct_Value.Save_AutoMode = 0xff;
      }
    }
    memory_write();
    return 1;
  }
  return 0;
}

u8 HM_DeleteSaveAll(void)
{
  u8 i;
  for (i = 0; i < Max_Save; i++)
    Union_Save_Value[i].Save_struct_Value.Save_AutoMode = 0xff;
  memory_write();
  return 1;
}

u8 User_Memory_Save(u8 x)
{
  Save_struct *Flash_Parameter_Buf;
  u16 *np;
  u16 nCheckSum;

  u8 i;

  i = 0;

  if (x >= Max_Save)
    return 0;

  Flash_Parameter_Buf = &(Union_Save_Value[x].Save_struct_Value);

  np = Union_Save_Value[x].Save_Value;

  if (AutoModeGet() != Flash_Parameter_Buf->Save_AutoMode)
  {
    i = 1;
    Flash_Parameter_Buf->Save_AutoMode = AutoModeGet();
    // if (Flash_Parameter_Buf->Save_AutoMode == 0)
    //   Flash_Parameter_Buf->Save_AutoMode = Max_AutoMode + 1; //*手动模式 不能是0
  }

  if (AutoModeGet() == 0) // 手动模式
  {
    if (Flash_Parameter_Buf->Save_Wise != MassWiseGet(MoveUp))
    {
      Flash_Parameter_Buf->Save_Wise = MassWiseGet(MoveUp);
      i = 1;
    }

    if (Flash_Parameter_Buf->Save_Wise_M2 != MassWiseGet(MoveDw))
    {
      Flash_Parameter_Buf->Save_Wise_M2 = MassWiseGet(MoveDw);
      i = 1;
    }

    if (Flash_Parameter_Buf->Save_Area != MassAreaGet(MoveUp))
    {
      Flash_Parameter_Buf->Save_Area = MassAreaGet(MoveUp);
      i = 1;
    }

    if (Flash_Parameter_Buf->Save_Area_M2 != MassAreaGet(MoveDw))
    {
      Flash_Parameter_Buf->Save_Area_M2 = MassAreaGet(MoveDw);
      i = 1;
    }

    if (Flash_Parameter_Buf->Save_Speed != MassSpeedGet(MoveUp))
    {
      Flash_Parameter_Buf->Save_Speed = MassSpeedGet(MoveUp);
      i = 1;
    }

    if (Flash_Parameter_Buf->Save_Speed_M2 != MassSpeedGet(MoveDw))
    {
      Flash_Parameter_Buf->Save_Speed_M2 = MassSpeedGet(MoveDw);
      i = 1;
    }

    if (Flash_Parameter_Buf->Save_Width != MassWidthGet(MoveUp))
    {
      Flash_Parameter_Buf->Save_Width = MassWidthGet(MoveUp);
      i = 1;
    }

    if (Flash_Parameter_Buf->Save_Width_M2 != MassWidthGet(MoveDw))
    {
      Flash_Parameter_Buf->Save_Width_M2 = MassWidthGet(MoveDw);
      i = 1;
    }

    if (Flash_Parameter_Buf->Save_BackPos != 0 /*ManualArea_Initial*/) // 手动模式初始位置
    {
      Flash_Parameter_Buf->Save_BackPos = 0; // ManualArea_Initial;
      i = 1;
    }

    if (Flash_Parameter_Buf->Save_BackPos_M2 != 0 /*CurM2TreadCircle*/) // 手动模式初始位置
    {
      Flash_Parameter_Buf->Save_BackPos_M2 = 0; // CurM2TreadCircle;
      i = 1;
    }
  }

  if (nCurZeroState <= 3)
  {
    if (Flash_Parameter_Buf->Save_ZeroState != nCurZeroState)
    {
      Flash_Parameter_Buf->Save_ZeroState = nCurZeroState;
      i = 1;
    }
  }
  else
  {
    if (Flash_Parameter_Buf->Save_BDDGCircle != DDG_Get_BnCurCircle ||
        Flash_Parameter_Buf->Save_LDDGCircle != DDG_Get_LnCurCircle)
    {

      Flash_Parameter_Buf->Save_ZeroState = 0x80;
      i = 1;
      Flash_Parameter_Buf->Save_BDDGCircle = DDG_Get_BnCurCircle;
      Flash_Parameter_Buf->Save_LDDGCircle = DDG_Get_LnCurCircle;
    }
  }

  if (Flash_Parameter_Buf->Save_AirState != AirModeGet())
  {
    Flash_Parameter_Buf->Save_AirState = AirModeGet();
    i = 1;
  }

  if (Flash_Parameter_Buf->Save_AirIntensity != AirIntensityGet())
  {
    Flash_Parameter_Buf->Save_AirIntensity = AirIntensityGet();
    i = 1;
  }

  if (Flash_Parameter_Buf->Save_Roll != RollModeGet())
  {
    Flash_Parameter_Buf->Save_Roll = RollModeGet();
    i = 1;
  }

  if (Flash_Parameter_Buf->Save_RollSpeed != RollSpeedGet()) // todo 增加 250322
  {
    Flash_Parameter_Buf->Save_RollSpeed = RollSpeedGet();
    i = 1;
  }

  if (Flash_Parameter_Buf->Save_LegKnead != KneadModeGet())
  {
    Flash_Parameter_Buf->Save_LegKnead = KneadModeGet();
    i = 1;
  }
  // #if(HEAT_BACK || HEAT_LEG || HEAT_SHOULDER || HEAT_ARM || HEAT_PJ)
#if (HEAT_BACK)
  Flash_Parameter_Buf->Save_Heat.MemoryBackHeat = HeatStateGet(BackHeat);
  Flash_Parameter_Buf->Save_Heat.MemoryBackHeatLevel = HeatLevelGet(BackHeat);
#endif

#if (HEAT_LEG)
  Flash_Parameter_Buf->Save_Heat.MemoryLegHeat = HeatStateGet(LegHeat);
  Flash_Parameter_Buf->Save_Heat.MemoryLegHeatLevel = HeatLevelGet(LegHeat);
#endif

#if (HEAT_SHOULDER)
  Flash_Parameter_Buf->Save_Heat.MemoryLegHeat = HeatStateGet(ShoulderHeat);
  Flash_Parameter_Buf->Save_Heat.MemoryLegHeatLevel = HeatLevelGet(ShoulderHeat);
#endif

#if (HEAT_ARM)
  Flash_Parameter_Buf->Save_Heat.MemoryLegHeat = HeatStateGet(ArmHeat);
  Flash_Parameter_Buf->Save_Heat.MemoryLegHeatLevel = HeatLevelGet(ArmHeat);
#endif

#if (HEAT_PJ)
  Flash_Parameter_Buf->Save_Heat.MemoryLegHeat = HeatStateGet(PJHeat);
  Flash_Parameter_Buf->Save_Heat.MemoryLegHeatLevel = HeatLevelGet(PJHeat);
#endif

  if (Flash_Parameter_Buf->Save_Shouler != BodyArea[LB_2]) //* 修改
  {
    Flash_Parameter_Buf->Save_Shouler = BodyArea[LB_2];
    i = 1;
  }

  if (Flash_Parameter_Buf->Save_BackIntensity != nBackIntensity)
  {
    Flash_Parameter_Buf->Save_BackIntensity = nBackIntensity;
    i = 1;
  }

  if (Flash_Parameter_Buf->Save_AutoIntensity != UserIntensityGet()) // todo 增加
  {
    Flash_Parameter_Buf->Save_AutoIntensity = UserIntensityGet();
    i = 1;
  }

  if (i == 1)
  {
    nCheckSum = 0;

    for (i = 0; i < (Max_Save_world - 1); i++)
      nCheckSum += np[i];
    np[Max_Save_world - 1] = (u16)0 - nCheckSum;

    memory_write();
    return 1;
  }
  else
  {
    return 0;
  }
}

u8 User_Memory_Use(u8 x)
{
  u8 i;
  Save_struct *nUse;

  if (x >= Max_Save)
    return 0;

  nUse = &(Union_Save_Value[x].Save_struct_Value);

  if (nUse->Save_AutoMode > Max_AutoMode) // todo +1 21为手动
    return 0;                             // 调用失败

  //    Air_Stre_State = 0;
  if (AutoModeGet() > 0 && nUse->Save_AutoMode == 0) // todo 自动转手动 250323
  {
    UserFunc_Set(UserFunc_ManualMode, 0);
  }

  AutoModeSet(nUse->Save_AutoMode);
  AutoPreModeSet(0); /* 将Pre参数清零，不管当前运行的自动程序和调用的自动程序是否是同一个，都能触发切换逻辑 */

  if (AutoModeGet() > 0)
  {
    AirModeSet(nUse->Save_AirState);
    AirIntensitySet(nUse->Save_AirIntensity);
  }
  else
  {
    // AirModeSet(AirModeClose);
    // RollSpeedSet(0);
    // RollModeSet(0);
    // RollStateSet(0);
    // KneadModeSet(0);
    // KneadStateSet(0);
    // KneadSpeedSet(0);
    // TICK_RESET(Manual_Save_tick);
    // Manual_Save_Flag = 1; // todo 打补丁 250323
    // Manual_Save_AirState = nUse->Save_AirState;
    // Manual_Save_Roll = nUse->Save_Roll;
    // Manual_Save_RollSpeed = nUse->Save_RollSpeed;
    // Manual_Save_LegKnead = nUse->Save_LegKnead;

    Manual_Save_AirState = nUse->Save_AirState;
    Manual_Save_AirIntensity = nUse->Save_AirIntensity;
    AirModeSet(AirModeClose);
    TICK_RESET(Manual_Save_tick);
    Manual_Save_Flag = 1; // todo 打补丁 250323

    RollModeSet(nUse->Save_Roll);
    RollStateSet(nUse->Save_Roll);
    RollSpeedSet(nUse->Save_RollSpeed);
    KneadModeSet(nUse->Save_LegKnead);
    KneadStateSet(nUse->Save_LegKnead);
    KneadSpeedSet(nUse->Save_LegKnead);
    KeyPush.FootRollKeyState = 1;
    KeyPush.KeyCalfKnead = 1;
    LegTx_DriveCmd = 1;

    MassWiseSet(MoveUp, (ManualWise_E)nUse->Save_Wise);
    MassSpeedSet(MoveUp, nUse->Save_Speed);
    MassWidthSet(MoveUp, nUse->Save_Width);
    MassAreaSet(MoveUp, (MassageArea_E)nUse->Save_Area); //* 修改

#if (USE_M2)
    // M2At_Load(M2AT_Manual, FALSE);
    MassWiseSet(MoveDw, (ManualWise_E)nUse->Save_Wise_M2);
    MassSpeedSet(MoveDw, nUse->Save_Speed_M2);
    MassWidthSet(MoveDw, nUse->Save_Width_M2);
    MassAreaSet(MoveDw, (MassageArea_E)nUse->Save_Area_M2);
//				M2TreadUserState = TreadUserState_Fixed;
//				M2TreadParams.B16 = nUse->Save_BackPos_M2;
#endif // USE_M2
  }

  i = nUse->Save_ZeroState;

  if (i <= 3)
  {
    DDG_Set_Zero(i);
  }
  else
  {
    // DDG_Set_SitCircle(nUse->Save_BDDGCircle, nUse->Save_LDDGCircle);
    DDGVariable.UserState = DDG_UserState_Sit_Circle;
    DDGVariable.BCircleSet = nUse->Save_BDDGCircle;
    DDGVariable.LCircleSet = nUse->Save_LDDGCircle;

    nCurZeroState = 0x80;

#if (LEG_COM)
    LegAutoAdj = 1;
    LegVoid = 1; // 小腿自动避让
#endif           // LEG_COM
  }

#if (0 == AUTO_AIRBYTABLE)
  AirModeSet((AirMode_E)nUse->Save_AirState);
#endif

  // if (AutoModeGet() == 0)
  // {
  //   AirModeSet((AirMode_E)nUse->Save_AirState);
  // }
  UserIntensitySet(nUse->Save_AutoIntensity); // todo 增加综合强度
  nBackIntensity = nUse->Save_BackIntensity;

  // RollModeSet(nUse->Save_Roll);
  // RollSpeedSet(nUse->Save_RollSpeed);
  // KneadModeSet(nUse->Save_LegKnead);

#if (HEAT_BACK)
  HeatStateSet(BackHeat, nUse->Save_Heat.MemoryBackHeat, nUse->Save_Heat.MemoryBackHeatLevel);
#endif

#if (HEAT_LEG)
  HeatStateSet(LegHeat, nUse->Save_Heat.MemoryLegHeat, nUse->Save_Heat.MemoryLegHeatLevel);
#endif

#if (HEAT_SHOULDER)
  HeatStateSet(ShoulderHeat, nUse->Save_Heat.MemoryShoulderHeat, nUse->Save_Heat.MemoryShoulderHeatLevel);
#endif

#if (HEAT_ARM)
  HeatStateSet(ArmHeat, nUse->Save_Heat.MemoryArmHeat, nUse->Save_Heat.MemoryArmHeatLevel);
#endif

#if (HEAT_PJ)
  HeatStateSet(PJHeat, nUse->Save_Heat.MemoryPJHeat, nUse->Save_Heat.MemoryPJHeatLevel);
#endif

  BodyPlaceCal(nUse->Save_Shouler);

  return 1;
}

#ifndef Handle_Old_Version
void User_Memory_Use_DIY(void) // todo 读取自定义数值
{
  u8 i, temp;
  Save_struct *nUse;

  nUse = &(Union_Save_Value[4].Save_struct_Value);

  temp = M1M2_to_Handlekey(nUse->Save_Wise);
  DIY_packet[0] = 0x10 + temp;
  i = nUse->Save_Area;
  DIY_packet[1] = (i << 4) + nUse->Save_AirState;
  if (temp == 2 || temp == 4 || temp == 7 || temp == 9) //* 宽度值与手法有关
    i = 0;
  else
    i = nUse->Save_Width + 1; //* 宽度转义 + 1
  DIY_packet[2] = (i << 4) + nUse->Save_Speed;
  i = nUse->Save_AirIntensity;
  DIY_packet[3] = (i << 4) + nUse->Save_BackIntensity + 1; //* 4D+1

  i = nUse->Save_RollSpeed << 4; // todo 需要保存滚轮速度
  if (nUse->Save_Heat.MemoryLegHeat)
    i |= 1;
  else
    i &= 0xfe;
  if (nUse->Save_Heat.MemoryBackHeat)
    i |= 2;
  else
    i &= 0xfd;
  if (nUse->Save_LegKnead)
    i |= 4;
  else
    i &= 0xfb;

  DIY_packet[4] = i;

  nUse = &(Union_Save_Value[5].Save_struct_Value);

  temp = M1M2_to_Handlekey(nUse->Save_Wise);
  DIY_packet[5] = 0x20 + temp;
  i = nUse->Save_Area;
  DIY_packet[6] = (i << 4) + nUse->Save_AirState;
  if (temp == 2 || temp == 4 || temp == 7 || temp == 9) //* 宽度值与手法有关
    i = 0;
  else
    i = nUse->Save_Width + 1; //* 宽度转义 + 1
  DIY_packet[7] = (i << 4) + nUse->Save_Speed;
  i = nUse->Save_AirIntensity;
  DIY_packet[8] = (i << 4) + nUse->Save_BackIntensity + 1;

  i = nUse->Save_Roll << 4;
  if (nUse->Save_Heat.MemoryLegHeat)
    i |= 1;
  else
    i &= 0xfe;
  if (nUse->Save_Heat.MemoryBackHeat)
    i |= 2;
  else
    i &= 0xfd;
  if (nUse->Save_LegKnead)
    i |= 4;
  else
    i &= 0xfb;
  DIY_packet[9] = i;
}
#endif
