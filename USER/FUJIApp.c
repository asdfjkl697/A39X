#include "FUJIApp.h"
#include "FUJIOTA.h"
#include "Display.h"

#define OH_Flag 0x7E
#define OH_Esc 0x5E // @ 不同于670T 251124
// #define OH_Max 12

// extern bool Uart3_Seng_Flag = 0;

#define UART2_RE_TimeOUT_MAX 20 // IDLE中断为主, 200ms超时仅作异常兜底
void Uart2_Timer10ms(void)
{
    UART2_RE_TIMEOUT++;
    if (UART2_RE_TIMEOUT >= UART2_RE_TimeOUT_MAX)
    {
        UART2_RE_TIMEOUT = 0;
        UART2_DATAPRO_SIGE = 1;
    }
}

u8 add_checksum(const u8 *data, size_t length) // 累加和校验码
{
    u8 checksum = 0;
    for (size_t i = 0; i < length; i++)
    {
        checksum += data[i];
    }
    return checksum;
}

// CRC16-CCITT算法实现
uint16_t CRC16_CCITT(const uint8_t *data, size_t length)
{
    uint16_t crc = 0xFFFF;  // 初始值
    uint16_t poly = 0x1021; // 多项式
    while (length--)
    {
        crc ^= (uint16_t)(*data++ << 8); // 将数据字节与CRC高8位异或，并左移8位
        for (uint8_t i = 0; i < 8; i++)
        {
            if (crc & 0x8000)
            {                            // 判断最高位
                crc = (crc << 1) ^ poly; // 左移一位并与多项式异或
            }
            else
            {
                crc <<= 1; // 仅左移一位
            }
        }
    }
    return crc; // 返回CRC校验码
}

u8 zy_z(u8 *Array, u16 Len)
{
    u8 gg = 0;
    u16 k = 0;
    u16 c = 0;
    for (k = 0; k < Len; k++)
    {
        if (Array[k] == OH_Flag)
        {
            for (c = Len + 1; c > k + 1; c--)
            {
                Array[c] = Array[c - 1];
            }
            Array[k] = OH_Esc;
            Array[k + 1] = 0x7d; //* md 251124
            k++;
            Len++;
            gg++;
        }
        if (Array[k] == OH_Esc)
        {
            for (c = Len + 1; c > k + 1; c--)
            {
                Array[c] = Array[c - 1];
            }
            Array[k] = OH_Esc;
            Array[k + 1] = 0x5d;
            k++;
            Len++;
            gg++;
        }
    }
    return gg;
}

u16 zy_f(u8 *Array, u16 Len)
{
    u8 temp_n = 0;
    u16 k = 0;
    u16 c = 0;
    for (k = 0; k < Len; k++)
    {
        if ((Array[k] == OH_Esc) && (Array[k + 1] == 0x7d))
        {
            for (c = k + 1; c < Len; c++)
            {
                Array[c] = Array[c + 1];
            }
            Array[k] = OH_Flag;
            temp_n++;
            Len--;
        }
        if ((Array[k] == OH_Esc) && (Array[k + 1] == 0x5d))
        {
            for (c = k + 1; c < Len; c++)
            {
                Array[c] = Array[c + 1];
            }
            Array[k] = OH_Esc;
            temp_n++;
            Len--;
        }
    }
    return Len;
}

//*在buf1的0-num内找sige,找到0个返回0，找到1个返回1，找个2个返回2
u8 Findsige(u8 *buf, u16 num, u8 sige, u16 *Start, u16 *End)
{
    u16 i, j;
    for (i = 0; i < num; i++)
    {
        if (buf[i] == sige) // 找第一个sige
        {
            *Start = i;
            if (i == num)
            {
                return 1;
            }
            for (j = i + 1; j < num; j++)
            {
                if (buf[j] == sige) // 找第二个sige
                {
                    *End = j;
                    return 2;
                }
            }
        }
    }
    return 0;
}

void EUSART2_Send_Data(u8 *send_data, u8 send_len)
{
    u8 i;
    for (i = 0; i < send_len; i++)
        EUSART2_Write(send_data[i]); // 数据体
}

u8 FUJI_IAP_FLAG = 0;              // * IAP 标志
u8 FUJI_OTA_FLAG = 0;              // * OTA 标志
CDP_DataFormat0x3F_t FUJI_RunTime; // * add 251125
bool FUJU_Send_Flag = 0;
u8 FUJU_Send_len = 0;
u8 FUJU_Send_data[MaxSendLen] = {0};
void FUJU_Send_Pro(void)
{
    u8 i;
    u8 len = FUJU_Send_data[7];
    FUJU_Send_data[0] = 0x7e;
    FUJU_Send_data[1] = 0x12; // 目标地址
    FUJU_Send_data[2] = 0x01; // 源地址
    FUJU_Send_data[3] = 0x01; // 协议id

    FUJU_Send_data[6] = 0x00; // 应答码
    FUJU_Send_data[7 + len] = add_checksum(&FUJU_Send_data[1], len + 6);
    i = zy_z(&FUJU_Send_data[1], len + 7); // 转义
    FUJU_Send_len = i + len + 9;
    FUJU_Send_data[FUJU_Send_len - 1] = 0x7e;
    EUSART2_Send_Data(FUJU_Send_data, FUJU_Send_len); // @ 260303
}

// @ 实际指令操作在这里 251015
void FUJI_Handler(u8 *OHData1, u16 len)
{
    // u8 para_id;
    u8 i, size;
    u16 temp;
    CDP_DataFormat0x21_t heart_data;

    bool app_key_state = 0;
    u8 address = 0;
    u8 command = 0;
    u8 rec_packid = 0; //* packid
    u8 DataLen = 0;
    u8 DataID = 0;
    u8 KeyVal = 0;
    u8 AddtionData[2] = {0};

    address = OHData1[1];
    if (address != 1) // 通信地址不对
    {
        return;
    }

    rec_packid = OHData1[4];
    command = OHData1[5]; //* 命令码
    DataLen = OHData1[7]; //* 内容长度
    DataID = OHData1[8];

    if (command == 0x02 || command == 0xf0)
    {
        OTA_Handler(OHData1, len);
        return;
    }
}

// u8 test_buf[254] = {0};
u16 FUJI_DataPro(u8 *OHData, u16 len)
{
    u16 FrameStart = 0, FrameEnd = 0;
    u16 DataLEN = 0;
    u8 Check = 0;
    u8 checktemp = 0;

    if (Findsige(OHData, len, 0x7e, &FrameStart, &FrameEnd) < 2) // 没有两个0x7e
    {
        return 0;
    }

    DataLEN = zy_f(&OHData[FrameStart + 1], (FrameEnd - FrameStart - 1)); // 转义
    DataLEN += 2;

    // if (DataLEN > 40 && OHData[FrameStart + 8] != 4) // todo test 260715
    //     checktemp = OHData[FrameStart + DataLEN - 2];

    // memcpy(test_buf, &OHData[FrameStart], DataLEN); // todo test 260715

    checktemp = OHData[FrameStart + DataLEN - 2];

    Check = add_checksum(&OHData[FrameStart + 1], (DataLEN - 3));

    if (Check == checktemp) //* 校验
    {
        FUJI_Handler(&OHData[FrameStart], DataLEN); // * 打包数组
    }
    return FrameEnd + 1; // 返回消费的字节数（从 buffer 开头到第二个 0x7E）
}

void FUJIApp_Recieve(void) // 在os_task_BlueRx的while循环之后调用
{
    u16 consumed;
    if (UART2_DATAPRO_SIGE)
    {
        UART2_DATAPRO_SIGE = 0;

        // 循环处理所有帧，逐个消费
        while (COM2.RX_write > 0)
        {
            consumed = FUJI_DataPro(RX2_Buffer, COM2.RX_write);
            if (consumed == 0)
            {
                break; // 无完整帧
            }
            // 只消费已处理的帧，保留剩余数据
            if (consumed < COM2.RX_write)
            {
                // 将剩余数据前移
                memmove(RX2_Buffer, &RX2_Buffer[consumed], COM2.RX_write - consumed);
            }
            COM2.RX_write -= consumed;
        }
        COM2.RX_read = COM2.RX_write; // 同步读写指针

        if (COM2.RX_write == 0)
        {
            UART2_RE_TIMEOUT = 0;
        }
    }
}

void FUJIApp_Deal_10ms(void) // @ 修改成 5ms
{
    Uart2_Timer10ms();
    OTA_Upgrade_Pro(); //* 升级处理 251224
}
