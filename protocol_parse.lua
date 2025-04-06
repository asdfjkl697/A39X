-- protocol_parse.lua

function Normal_Display_Data_Update(packet)
	Now_Data = string.format("packet[2] = 0x%02X",packet[2])  --使用自由协议时候必须加上这一行，否则会数据处理不正常
	-- 只保留显示数据 前9字节包含帧头和命令信息 最后2字节校验
	j = (#(packet))-9-2
	for i=0,j do
		packet[i] = packet[i+9]
		-- Now_Data = string.format("packet[%d] = 0x%02X",i,packet[i])  
		-- print(Now_Data)
	end

	nTestFlag = 0
	-- 工作状态
	nPreWorkState = packet[0];
	--按摩时间 分
	nPreTiming_min = packet[1];
	--按摩时间 秒
	nPreTiming_s = packet[2];
	--上机械手位置
	nPreMovementPosition_Upper = packet[3] & 0x0f;
	--下机械手位置
	nPreMovementPosition_Lower = packet[3]>>4;
	--机械手 M1 M2
	nPreHandCheck = packet[4] & 0x0f;
	--肩位置挡位
	nPreShoulderNo = packet[4] >>4;
	--自动模式
	nPreAutoMode = packet[5];
	--声音处理 - 按键音
	nPreBeepState = packet[6];
	--4D强度
	nPre4DIntensity = packet[7] & 0x0f;
	--按摩速度
	nPreMassageSpeed = packet[7] >> 4;
	--按摩区域
	nPreAreaValue = packet[8] & 0x0f;
	--按摩宽度
	nPreWidthValue = packet[8] >> 4;
	--滚轮速度
	nPreRollState = packet[9] & 0x0f;
	--零重力
	nPreZeroState = packet[9] >> 4;
	--气压强度
	nPreAirStrength = packet[10] & 0x0f;
	--气压模式
	nPreAirMode = packet[10] >> 4;
	--背部加热
	nPreBackHeatState = packet[11] &0x01;
	--小腿加热
	nPreLegsHeatState = (packet[11]>>1) & 0x01;
	--披肩加热
	nPrePJHeatState = (packet[11] >>2) & 0x01;
	--气囊状态
	nPreAirBit = packet[12];
	--蓝牙
	nPreBlueToothState = (packet[13] >> 1) & 0x01;
	--声控
	nPreVoiceState = (packet[13] >> 2) & 0x01;
	--老化
	nPreLCState = (packet[13] >> 3) & 0x01;
	--小腿揉搓
	nPreCalfKnead = (packet[13] >> 4) & 0x01;
	--负离子
	nPreIon = (packet[13] >> 5) & 0x01;
	--按摩强度 轻柔-舒缓-强劲
	nPreMassageStrength = packet[14];
	--错误代码
	nPreErrorCode = packet[15];
	--序列号
	nPreIDCode = (packet[19]<<24)|(packet[18]<<16)|(packet[17]<<8)|(packet[16]);
	--手法
	nPreWiseValue = packet[20];
	--心率		
	nPreHeartRate = packet[21];
	--血氧
	nPreSop2 = packet[22];
	--微循环
	nPreMicLoop = packet[23];
	--疲劳等级
	nPreTiredStat = packet[24];
	--疲劳检测进度条 0-24
	nPreCountDown = packet[25];
	--语音播报
	nPreVoicePlay = packet[26]>>7;
	--音量
	nPreVolume = packet[26] & 0x7f;
	--LED灯
	nPreLedMode = packet[27]&0x01;
	--背景音乐
	nPreBGM = (packet[27]>>1)&0x01;
	--按摩椅自动复位
	nPreAutoReset = (packet[27]>>2)&0x01;
	--语言
	nPreLanguage = packet[28];
	--存储状态
	nPreSaveState = packet[29];

	DisplayDataUpdate(0xF1)
end
