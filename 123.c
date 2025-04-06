#include "App.h"

u8 App_Update_Cnt = 0;
bool App_Update_Flag = 0;

u8 AT_Auto16[AutoLoad_SIZE] = {0};
u8 AT_Auto17[AutoLoad_SIZE] = {0};
u8 AT_Auto18[AutoLoad_SIZE] = {0};
u8 AT_Auto19[AutoLoad_SIZE] = {0};

//u8 *AT_Auto16;

u8 flash_read_AutoOnline( uint32_t read_addr, uint8_t* p_buffer, uint16_t num_read )
{
    uint16_t i;
    if( *( uint8_t* )( read_addr ) == 0xFF )
    {
        return 0;
    }
    for( i = 0; i < num_read; i++ )
    {
        p_buffer[i] = *( uint8_t* )( read_addr );
        read_addr += 1;
    }
    return 1;
}

#define SECTOR_SIZE 2048 /* this parameter depends on the specific model of the chip */

/**
* @brief  check flash upgrade flag.
* @param  none
* @retval none
*/
FlagStatus flash_upgrade_flag_read( void )
{
    if( ( *( u32* )OTA_UPGRADE_FLAG_ADDR ) == OTA_UPGRADE_FLAG )
    {
        return SET;
    }
    else
    {
        return RESET;
    }
}

/**
* @brief  write flash upgrade flag.
* @param  none
* @retval none
*/
void write_upgrade_flag( void )
{
    fmc_unlock();
    fmc_page_erase( OTA_UPGRADE_FLAG_ADDR );
    fmc_word_program( OTA_UPGRADE_FLAG_ADDR, OTA_UPGRADE_FLAG );
    fmc_lock();
}

/**
* @brief  ota init.
* @param  none
* @retval none
*/
void ota_init( void )
{
    if( ( *( uint32_t* )OTA_UPGRADE_FLAG_ADDR ) == OTA_UPGRADE_FLAG )
    {
        fmc_unlock();
        fmc_page_erase( OTA_UPGRADE_FLAG_ADDR );
        fmc_lock();
    }
}

/**
* @}
*/

/**
* @}
*/

ErrStatus app_ota_erase( u32 write_addr, u32 num_sector )
{
    ErrStatus rtn = SUCCESS;
    fmc_unlock();
    for( uint32_t i = 0; i < num_sector; i++ )
    {
        fmc_page_erase( write_addr + ( SECTOR_SIZE * i ) );
    }
    fmc_lock();
    return rtn;
}

ErrStatus app_ota_write( u32 write_addr, u8* p_buffer, u16 num_write )
{
    if( ( write_addr % 4 ) != 0 )   //|| writeAddr < AP_ADDR_MIN || writeAddr > AP_ADDR_MAX ) // || (writeAddr + numByteToWrite) > AP_ADDR_MAX
    {
        return ERROR;
    }
    if( memcmp( ( uint8_t* )write_addr, p_buffer, num_write ) == 0 )         //???????,?????
    {
        return SUCCESS;
    }
    uint16_t leftLen = 0;
    uint16_t u16Loop = 0;
    uint32_t temp = 0;
    fmc_unlock();
    while( 1 )
    {
        if( u16Loop < num_write )
        {
            temp = 0;
            leftLen = num_write - u16Loop;
            if( leftLen >= 4 )
            {
                memcpy( &temp, p_buffer, 4 );
                fmc_word_program( write_addr, temp );
                /* clear all pending flags */
                fmc_flag_clear( FMC_FLAG_BANK0_END );
                fmc_flag_clear( FMC_FLAG_BANK0_WPERR );
                fmc_flag_clear( FMC_FLAG_BANK0_PGERR );
                write_addr += 4;
                p_buffer += 4;
                u16Loop += 4;
            }
            else
            {
                memcpy( &temp, p_buffer, leftLen );
                fmc_word_program( write_addr, temp );
                /* clear all pending flags */
                fmc_flag_clear( FMC_FLAG_BANK0_END );
                fmc_flag_clear( FMC_FLAG_BANK0_WPERR );
                fmc_flag_clear( FMC_FLAG_BANK0_PGERR );
                break;
            }
        }
        else
        {
            break;
        }
    }
    fmc_lock();
    return SUCCESS;
}

u16 TM_ACK_Value = 0; // ????
u8 ota_state = 0;
u16 ota_package = 0;
u32 ota_size_num = 0; //??
u32 ota_size_package = 0; //???
u32 ota_addr = 0;
u16 ota_over_time = 0;
u32 ota_size_cnt = 0;

u8 AutoLoad_state = 0;
u16 AutoLoad_package = 0;
u32 AutoLoad_size_num = 0; //??
u32 AutoLoad_size_package = 0; //???
u32 AutoLoad_addr = 0;
u16 AutoLoad_over_time = 0;
u16 AutoLoad_size_cnt[4] = {0};
u8 AutoLoad_TempCnt = 0;

#define AutoLoad_state_stop 0
#define AutoLoad_state_start 1
#define AutoLoad_state_Data 2
#define AutoLoad_state_end 3
#define AutoLoad_over_time_default 10

#define ota_state_stop 0
#define ota_state_start 1
#define ota_state_Data 2
#define ota_state_end 3
#define ota_over_time_default 10

static u8 ota_found_str( const char* str, const char* compare_str )
{
    while( *str > 0 )
    {
        if( *compare_str != *str )
        {
            return 0;
        }
        str++;
        compare_str++;
        if( *compare_str == 0 )
        {
            return 1;
        }
    }
    return 0;
}

void Send_App_Data( u8* nBuf, u8 nLen )
{
    // Arm_App_Update(nBuf, nLen);
}

u8 Display_Data_F1[20] = {0};
//u8 Display_Data_F1Con[20]={0};
u8 Display_Data_F2[20] = {0};
//u8 Display_Data_F2Con[20]={0};

void App_Update_State( void )
{
    // static u8 App_Update_Cnt, App_Update_Cnt1;
    u32 i;
    u8 Display_Data[20];
    // APP Update
    switch( App_Update_Cnt )
    {
        case 0:
            Display_Data_F1[0] = 0xAA;
            Display_Data_F1[1] = 0x55;
            Display_Data_F1[2] = 0x02;
            Display_Data_F1[3] = 0xe5;
            Display_Data_F1[4] = 0xF1;
            Display_Data_F1[5] = 0x01;
            Display_Data_F1[6] = 0x00;
            Display_Data_F1[7] = 0x09;
            if( ota_state == ota_state_start )
            {
                Display_Data_F1[8] = 0x0D;
            }
            else
            {
                Display_Data_F1[8] = WorkStateGet();
            }
            //????
            Display_Data_F1[9] = WorkMinGet() - 1;
            Display_Data_F1[10] = WorkSecGet();
            Display_Data_F1[11] = ( ( 10 - ShoulderValue ) << 4 ) | ( BackPosUpGet() );
            Display_Data_F1[12] = AutoModeGet();
            if( AutoModeGet() >= 16 && AutoModeGet() <= 19 )
            {
                Display_Data_F1[12] = 209 + AutoModeGet();
            }
            //????
            if( AutoModeGet() == 0 )
            {
                if( MoveUpState.ManualArea_ID == Area_Upper )
                {
                    Display_Data_F1[13] = 3;
                }
                else if( MoveUpState.ManualArea_ID == Area_Lower )
                {
                    Display_Data_F1[13] = 4;
                }
                else if( MoveUpState.ManualArea_ID == Area_Full )
                {
                    Display_Data_F1[13] = 2;
                }
                else if( MoveUpState.ManualArea_ID == Area_Fixed )
                {
                    Display_Data_F1[13] = 1;
                }
                else if( MoveUpState.ManualArea_ID == Area_Part )
                {
                    Display_Data_F1[13] = 5;
                }
            }
            else
            {
                Display_Data_F1[13] = 0;
            }
            //???????
            switch( MassWiseGet( MoveUp ) )
            {
                default:
                case TZ://??
                    Display_Data_F1[14] = 0;
                    break;
                case RNZ://??
                    Display_Data_F1[14] = 1;
                    break;
                case PD://??2
                    Display_Data_F1[14] = 2;
                    break;
                case RPZ://????
                    Display_Data_F1[14] = 4;
                    break;
                case PDB://??
                    Display_Data_F1[14] = 8;
                    break;
                case ZYS://??
                    Display_Data_F1[14] = 5;
                    break;
                case D3RNZ://3D??
                    Display_Data_F1[14] = 0x20;
                    break;
                case D3PD://3D??
                    Display_Data_F1[14] = 0x21;
                    break;
                case D3RPZ://3D????
                    Display_Data_F1[14] = 0x22;
                    break;
                case ZYD://3D??
                    Display_Data_F1[14] = 0x24;
                    break;
            }
            //??????????
            Display_Data_F1[15] = ( MassSpeedGet( MoveUp ) << 4 ) | MassWidthGet( MoveUp );
            /*??????????????
            */
            if( AirPartGet().Part2.Bit.Ankle )
            {
                Shoulder_Sac = 1;
            }
            else
            {
                Shoulder_Sac = 0;
            }
            if( AirPartGet().Part2.Bit.Ankle )
            {
                Arm1_Sac = 1;
            }
            else
            {
                Arm1_Sac = 0;
            }
            if( AirPartGet().Part2.Bit.Ankle )
            {
                Arm2_Sac = 1;
            }
            else
            {
                Arm2_Sac = 0;
            }
            // if ((UpperValveData & TLR) && Pump_State > 0)
            //  Back_Sac = 1;
            if( AirPartGet().Part2.Bit.Ankle )
            {
                Leg1_Sac = 1;
                // Leg2_Sac = 1;
            }
            else
            {
                Leg1_Sac = 0;
                // Leg2_Sac = 0;
            }
            if( AirPartGet().Part2.Bit.Ankle )
            {
                Leg2_Sac = 1;
            }
            else
            {
                Leg2_Sac = 0;
            }
            Display_Data_F1[16] = UnionState1.Strack;
            Display_Data_F1[17] = 0x00;
            Display_Data_F1[18] = 0x0D;
            Display_Data_F1[19] = 0x0A;
            //  if(TxF1Enable() || TxF2Enable()|| App_Update_Flag){
            for( u8 i = 0; i < 20; i++ )
            {
                EUSART2_Write( Display_Data_F1[i] );
            }
            //  }
            App_Update_Cnt++;
            break;
        case 1:
            Display_Data_F2[0] = 0xAA;
            Display_Data_F2[1] = 0x55;
            Display_Data_F2[2] = 0x02;
            Display_Data_F2[3] = 0xe5;
            Display_Data_F2[4] = 0xF2;
            Display_Data_F2[5] = 0x01;
            Display_Data_F2[6] = 0x00;
            Display_Data_F2[7] = 0x09;
            i = nBackIntensity + 1;
            i <<= 4;
            i |= AirIntensityGet();
            Display_Data_F2[8] = i;
            //???????????????????,??????
            if( AirModeGet() == Area_Upper )
            {
                Display_Data_F2[9] = AirIntensityGet() << 4;
                Display_Data_F2[9] &= 0xf0;
            }
            else if( AirModeGet() == AirModeLower )
            {
                Display_Data_F2[9] = AirIntensityGet();
                Display_Data_F2[9] &= 0x0f;
            }
            else if( AirModeGet() == AirModeFull )
            {
                Display_Data_F2[9] = ( AirIntensityGet() << 4 ) ;  //+ AirIntensity;
                Display_Data_F2[9] |= AirIntensityGet();
            }
            else
            {
                Display_Data_F2[9] = 0;
            }
            i = RollSpeedGet();
            //nLegKnead????????
            Display_Data_F2[10] = ( i << 4 ) | 0; //nLegKnead;
            i = 0;
            //????
            if( HeatStateGet( BackHeat ) )
            {
                i |= 0x80;
            }
            Display_Data_F2[11] = i;
            i = 0;
            //????
            if( HeatStateGet( LegHeat ) )
            {
                i |= 0x01;
            }
            //????
            if( nLCState )
            {
                i |= 0x40;
            }
            //????
            if( nVoiceControlSw )
            {
                i |= 0x08;
            }
            //????
            if( nBlueState )
            {
                i |= 0x02;
            }
            // if (nIonState)
            // i |= 0x04;
            Display_Data_F2[12] = i;
            Display_Data_F2[13] = 0;
            Display_Data_F2[14] = 0;
            Display_Data_F2[15] = 0;
            Display_Data_F2[16] = 0;
            Display_Data_F2[17] = 0;
            Display_Data_F2[18] = 0x0D;
            Display_Data_F2[19] = 0x0A;
            //  if(TxF1Enable() || TxF2Enable()|| App_Update_Flag){
            //
            //  for(u8 i=0;i<20;i++){
            //
            //  Display_Data_F1Con[i]=Display_Data_F1[i];
            //  }
            //
            //  for(u8 i=0;i<20;i++){
            //
            //  Display_Data_F2Con[i]=Display_Data_F2[i];
            //
            //  }
            for( u8 i = 0; i < 20; i++ )
            {
                EUSART2_Write( Display_Data_F2[i] );
            }
            App_Update_Flag = 0;
            //  }
            App_Update_Cnt = 0;
            // Send_App_Data(Display_Data, 13);
            break;
        case 2:
            Display_Data[0] = 0xAA;
            Display_Data[1] = 0x55;
            Display_Data[2] = 0x02;
            Display_Data[3] = 0xe5;
            Display_Data[4] = 0xF5;
            Display_Data[5] = 0x01;
            Display_Data[6] = 0x00;
            Display_Data[7] = 0x09;
            Display_Data[8] = ( u32 )( ota_size_cnt * 100 ) / ota_size_num;
            Display_Data[9] = 0;
            Display_Data[10] = 0;
            Display_Data[11] = 0;
            Display_Data[12] = 0;
            Display_Data[13] = 0;
            Display_Data[14] = 0;
            Display_Data[15] = 0;
            Display_Data[16] = 0;
            Display_Data[17] = 0;
            Display_Data[18] = 0x0D;
            Display_Data[19] = 0x0A;
            for( u8 i = 0; i < 20; i++ )
            {
                EUSART2_Write( Display_Data[i] );
            }
            App_Update_Cnt++;
            break;
        default:
            App_Update_Cnt = 0;
            break;
    }
}

//u8 TxF1Enable(){

//
//  for(u8 i=0;i<20;i++){
//
//  if(Display_Data_F1[i]==Display_Data_F1Con[i])
//
//  continue;
//
//  else{
//
//  return 1;
//  }

//  }
//
//  return 0;

//}

//u8 TxF2Enable(){

//  for(u8 i=0;i<20;i++){
//
//  if(Display_Data_F2[i]==Display_Data_F2Con[i])
//
//  continue;
//
//  else{
//
//  return 1;
//  }

//  }
//
//  return 0;

//}

void App_Ack( /*u16 TM_ACK_Value*/ )
{
    u8 Display_Data[25];
    //??
    if( TM_ACK_Value & TM_ACK_Model_App )
    {
        TM_ACK_Value &= ~TM_ACK_Model_App;
        Display_Data[0] = 0xAA;
        Display_Data[1] = 0x55;
        Display_Data[2] = TM_Device_App;
        Display_Data[3] = 0xe4;
        Display_Data[4] = 0x01;
        Display_Data[5] = 0x01;
        Display_Data[6] = 0x00;
        Display_Data[7] = 0x08;
        Display_Data[8] = 0x00;
        Display_Data[9] = 'A';
        Display_Data[10] = '3';
        Display_Data[11] = '9';
        Display_Data[12] = '1';
        Display_Data[13] = 'S';
        Display_Data[14] = '-';
        Display_Data[15] = '1';
        Display_Data[16] = 0;
        Display_Data[17] = 0x0D;
        Display_Data[18] = 0x0A;
        for( u8 i = 0; i < 19; i++ )
        {
            EUSART2_Write( Display_Data[i] );
        }
        // Send_App_Data(Display_Data, 11);
        return;
    }
    if( TM_ACK_Value & TM_ACK_Model_Server )
    {
        TM_ACK_Value &= ~TM_ACK_Model_Server;
        Display_Data[0] = 0xAA;
        Display_Data[1] = 0x55;
        Display_Data[2] = TM_Device_Server;
        Display_Data[3] = 0xF1;
        Display_Data[4] = 0x01;
        Display_Data[5] = 0x01;
        Display_Data[6] = 0x00;
        Display_Data[7] = 0x08;
        Display_Data[8] = 0x00;
        Display_Data[9] = 'A';
        Display_Data[10] = '3';
        Display_Data[11] = '9';
        Display_Data[12] = '1';
        Display_Data[13] = 'S';
        Display_Data[14] = '-';
        Display_Data[15] = '1';
        Display_Data[16] = 0;
        Display_Data[17] = 0x0D;
        Display_Data[18] = 0x0A;
        for( u8 i = 0; i < 19; i++ )
        {
            EUSART2_Write( Display_Data[i] );
        }
        // Send_App_Data(Display_Data, 11);
        return;
    }
    //?????
    if( TM_ACK_Value & TM_ACK_Version_Server )
    {
        TM_ACK_Value &= ~TM_ACK_Version_Server;
        Display_Data[0] = 0xAA;
        Display_Data[1] = 0x55;
        Display_Data[2] = TM_Device_Server;
        Display_Data[3] = 0xf1;
        Display_Data[4] = 0x00;
        Display_Data[5] = 0x01;
        Display_Data[6] = 0x00;
        Display_Data[7] = 0x0D;
        Display_Data[8] = 0;
        Display_Data[9] = UserCode_data >> 24;
        Display_Data[10] = UserCode_data >> 16;
        Display_Data[11] = UserCode_data >> 8;
        Display_Data[12] = UserCode_data;
        Display_Data[13] = 0; // 1.0 ????
        Display_Data[14] = 1;
        Display_Data[15] = 0; // 1.0 ????
        Display_Data[16] = 1;
        Display_Data[17] = 0; // ERR
        Display_Data[18] = 0;
        Display_Data[19] = 0;
        Display_Data[20] = 0;
        Display_Data[19] = 0;
        Display_Data[20] = 0x0D;
        Display_Data[21] = 0x0A;
        for( u8 i = 0; i < 22; i++ )
        {
            EUSART2_Write( Display_Data[i] );
        }
        // Send_App_Data(Display_Data, 17);
        return;
    }
    if( TM_ACK_Value & TM_ACK_Valid_Sever )
    {
        TM_ACK_Value &= ~TM_ACK_Valid_Sever;
        Display_Data[0] = 0xAA;
        Display_Data[1] = 0x55;
        Display_Data[2] = TM_Device_Server;
        Display_Data[3] = 0Xe6; // ??
        Display_Data[4] = 0X01; // ??
        Display_Data[5] = 0X00; // ??
        Display_Data[6] = 0X00; // ??
        Display_Data[7] = 0X00; // ??
        Display_Data[8] = Display_Data[2] + Display_Data[3] +
                          Display_Data[4] + Display_Data[5] + Display_Data[6] +
                          Display_Data[7]; //
        Display_Data[9] = 0X0D; // ??
        Display_Data[10] = 0X0A; // ??
        // if (Arm_Version > 0)//????????200????
        // {
        // Display_Data[4] = (u8)(OTA_PACKAGE_SIZE >> 8);
        // Display_Data[5] = (u8)OTA_PACKAGE_SIZE;
        // }
        // else
        // {
        // Display_Data[4] = 0;
        // Display_Data[5] = 60;
        // }
        // Display_Data[6] = (u8)(OTA_PACKAGE_DELAY >> 8);
        // Display_Data[7] = (u8)OTA_PACKAGE_DELAY;
        // Send_App_Data(Display_Data, 8);
        for( u8 i = 0; i < 11; i++ )
        {
            EUSART2_Write( Display_Data[i] );
        }
        // Enter_Pause();
        // ota_over_time = 20; // 20/5= 4S
    }
    if( TM_ACK_Value & TM_ACK_DisValid_Sever )
    {
        // Display_Data[0] = TM_Device_Server;
        // Display_Data[1] = 0Xe6; // ??
        // Display_Data[2] = 0X00; // ??
        // Display_Data[3] = 0X00; // ??
        // Display_Data[4] = 0x00; //
        // Send_App_Data(Display_Data, 5);
        TM_ACK_Value &= ~TM_ACK_Valid_Sever;
        Display_Data[0] = 0xAA;
        Display_Data[1] = 0x55;
        Display_Data[2] = TM_Device_Server;
        Display_Data[3] = 0Xe6; // ??
        Display_Data[4] = 0X00; // ??
        Display_Data[5] = 0X00; // ??
        Display_Data[6] = 0X00; // ??
        Display_Data[7] = 0X00; // ??
        Display_Data[8] = Display_Data[2] + Display_Data[3] +
                          Display_Data[4] + Display_Data[5] + Display_Data[6] +
                          Display_Data[7];; // ??
        Display_Data[9] = 0X0D; // ??
        Display_Data[10] = 0X0A; // ??
        for( u8 i = 0; i < 11; i++ )
        {
            EUSART2_Write( Display_Data[i] );
        }
        // ota_state = ota_state_stop;
        // ota_package = 0;
        // ota_size = 0;
        // ota_addr = 0;
        // Exit_Pause();
    }
}

ErrStatus flash_check( u32 app_addr )
{
    /* check app starting address whether 0x08xxxxxx */
    if( ( ( *( u32* )( app_addr + 4 ) ) & 0xFF000000 ) != 0x08000000 )
    {
        return ERROR;
    }
    /* check the address of stack */
    if( ( ( *( u32* )app_addr ) - 0x20000000 ) <= ( SRAM_SIZE * 1024 ) )
    {
        return SUCCESS;
    }
    return ERROR;
}

void App_Cmd_Handler( u8* nbuf, u16 nLen )
{
    // u8 nDevice = nbuf[0];
    // u8 nCmd = nbuf[1];
    // u8 nParam = nbuf[2];
    u16 i;
    u8 nDevice = nbuf[2];
    u8 nCmd = nbuf[3];
    u8 nParam = nbuf[4];
    if( nDevice == TM_Device_Server ) // ???
    {
        switch( nCmd )
        {
            case 0xf1: // ????
                // App_Update_Cnt = 2, App_Update_Cnt1 = 3;
                // App_Ack(TM_ACK_Version_Server);
                App_init();
                TM_ACK_Value |= TM_ACK_Version_Server | TM_ACK_Model_Server;
                // App_Update_Flag=1;
                // App_Update_Cnt=0;
                break;
            case 0xf4:              // ??
                if( nParam == 0x01 ) // start
                {
                    if( nPowerOn )
                    {
                        UserFunc_Set( UserFunc_PowerOff, 1 ); // ??
                    }
                    // if (ota_state != ota_state_start)
                    // {
                    ota_size_num = ( ( u32 )nbuf[8] << 24 ) +
                                   ( ( u32 )nbuf[9] << 16 ) +
                                   ( ( u32 )nbuf[10] << 8 ) +
                                   ( ( u32 )nbuf[11] );
                    if( ota_size_num <= APP_SIZE &&
                            ( app_ota_erase( TEMPLATE_START_ADDR, APP_SIZE / SECTOR_SIZE ) == SUCCESS ) )
                    {
                        ota_state = ota_state_start;
                        ota_addr = TEMPLATE_START_ADDR;
                        ota_package = 0;
                        ota_size_cnt = 0;
                        ota_over_time = ota_over_time_default;
                        Pt_Reset_ID( TASK_ID_APP );
                        App_Update_Cnt = 0;
                        App_Update_State();
                    }
                    else
                    {
                        TM_ACK_Value = TM_ACK_DisValid_Sever;
                        App_Ack();
                    }
                }
                else if( nParam == 0x02 ) // data
                {
                    if( ota_state == ota_state_start )
                    {
                        ota_package++;
                        if( ( ota_package & 0x00ff ) ==
                                nbuf[5]/* && ota_size_num >= nLen*/ )
                        {
                            if( app_ota_write( ota_addr, &nbuf[8], nLen ) == SUCCESS )
                            {
                                ota_addr += nLen;
                                // ota_size_num -= nLen;
                                for( i = 0; i < nLen; i++ )
                                {
                                    ota_size_cnt++;
                                }
                                ota_over_time = ota_over_time_default;
                                Pt_Reset_ID( TASK_ID_APP );
                                App_Update_Cnt = 2;
                                App_Update_State();
                            }
                            else
                            {
                                TM_ACK_Value = TM_ACK_DisValid_Sever;
                                App_Ack();
                            }
                        }
                        else
                        {
                            TM_ACK_Value = TM_ACK_DisValid_Sever;
                            App_Ack();
                        }
                    }
                }
                else if( nParam == 0x03 ) // end
                {
                    if( ota_state == ota_state_start )
                    {
                        ota_state = ota_state_end;
                        ota_over_time = ota_over_time_default;
                        TM_ACK_Value = TM_ACK_Valid_Sever;
                        App_Ack();
                        write_upgrade_flag(); // ?????
                    }
                    else
                    {
                        ota_state = ota_state_stop;
                    }
                    // if (ota_state != ota_state_Data ||
                    // ota_size != 0 ||
                    // flash_check(TEMPLATE_START_ADDR) == ERROR)
                    // {
                    // App_Ack(TM_ACK_DisValid_Sever);
                    // return;
                    // }
                    // u32 flash_addr = TEMPLATE_START_ADDR;
                    // u32 flash_check = 0;
                    // u8 mode_check = 0;
                    // if (nLen >= 4)
                    // {
                    // while (flash_addr < ota_addr)
                    // {
                    // if (mode_check == 0) // ????
                    // mode_check = ota_found_str(((char *)flash_addr), OTA_MODE);
                    // flash_check += *((u8 *)flash_addr);
                    // flash_addr++;
                    // }
                    // u32 ota_check = ((u32)nbuf[4] << 24) +
                    // ((u32)nbuf[5] << 16) +
                    // ((u32)nbuf[6] << 8) +
                    // ((u32)nbuf[7]);
                    // if (ota_check != flash_check)
                    // {
                    // App_Ack(TM_ACK_DisValid_Sever);
                    // return;
                    // }
                    // }
                    // if (nLen >= 5 && nbuf[12] == 1 && mode_check == 0) // ????
                    // {
                    // App_Ack(TM_ACK_DisValid_Sever);
                    // return;
                    // }
                    // write_upgrade_flag();
                    // App_Ack(TM_ACK_Valid_Sever);
                    // APP_WaitCompleted();
                    // nvic_system_reset();
                }
                break;
            case 0xF5:
                if( nParam == 0x01 ) // start
                {
                    //  if (nPowerOn)
                    // {
                    // UserFunc_Set(UserFunc_PowerOff, UserFunc_PowerOff_KeepLying); // ??
                    // }
                    // if (ota_state != ota_state_start)
                    // {
                    // AutoLoad_size_num = ((u32)nbuf[8] << 24) +
                    // ((u32)nbuf[9] << 16) +
                    // ((u32)nbuf[10] << 8) +
                    // ((u32)nbuf[11]);
                    AutoLoad_size_num =
                        ( ( u32 )nbuf[6] << 8 ) +
                        ( ( u32 )nbuf[7] );
                    AutoLoad_TempCnt = nbuf[5] - 1;
                    if( AutoLoad_size_num <= AutoLoad_SIZE &&
                            ( app_ota_erase( AutoLoad_START_ADDR + ( AutoLoad_ADDR_Offset *
                                             AutoLoad_TempCnt ), AutoLoad_SIZE / SECTOR_SIZE ) == SUCCESS ) )
                    {
                        AutoLoad_state = AutoLoad_state_start;
                        AutoLoad_addr = AutoLoad_START_ADDR + ( AutoLoad_ADDR_Offset
                                                                * ( ( ( u32 )nbuf[5] ) - 1 ) );
                        AutoLoad_package = 0;
                        AutoLoad_size_cnt[AutoLoad_TempCnt] = 0;
                        AutoLoad_over_time = AutoLoad_over_time_default;
                        TM_ACK_Value = TM_ACK_Valid_Sever;
                        App_Ack();
                        Pt_Reset_ID( TASK_ID_APP );
                        //  App_Update_Cnt=0;
                        //  App_Update_State();
                    }
                    else
                    {
                        TM_ACK_Value = TM_ACK_DisValid_Sever;
                        App_Ack();
                    }
                }
                else if( nParam == 0x02 ) // data
                {
                    if( AutoLoad_state == AutoLoad_state_start )
                    {
                        AutoLoad_package++;
                        if( ( AutoLoad_package & 0x00ff ) ==
                                nbuf[5]/* && ota_size_num >= nLen*/ )
                        {
                            if( app_ota_write( AutoLoad_addr, &nbuf[8], nLen ) == SUCCESS )
                            {
                                AutoLoad_addr += nLen;
                                // ota_size_num -= nLen;
                                for( i = 0; i < nLen; i++ )
                                {
                                    AutoLoad_size_cnt[AutoLoad_TempCnt]++;
                                }
                                TM_ACK_Value = TM_ACK_Valid_Sever;
                                App_Ack();
                                AutoLoad_over_time = AutoLoad_over_time_default;
                                Pt_Reset_ID( TASK_ID_APP );
                                //  App_Update_Cnt=2;
                                //  App_Update_State();
                            }
                            else
                            {
                                TM_ACK_Value = TM_ACK_DisValid_Sever;
                                App_Ack();
                            }
                        }
                        else
                        {
                            TM_ACK_Value = TM_ACK_DisValid_Sever;
                            App_Ack();
                        }
                    }
                }
                else if( nParam == 0x03 ) // end
                {
                    if( AutoLoad_state == AutoLoad_state_start )
                    {
                        AutoLoad_state = AutoLoad_state_end;
                        AutoLoad_over_time = AutoLoad_over_time_default;
                        TM_ACK_Value = TM_ACK_Valid_Sever;
                        App_Ack();
                        //  write_upgrade_flag(); // ?????
                    }
                    else
                    {
                        AutoLoad_state = AutoLoad_state_stop;
                    }
                }
                break;
        }
    }
    else if( nDevice == TM_Device_App ) // app
    {
        u16 i;
        Key.State = nbuf[5];
        Key.Param = nParam;
        Key.Value = 0;
        switch( nCmd )
        {
            case 0xe4: // ????
                // App_Update_Cnt = 2, App_Update_Cnt1 = 0;
                App_init();
                TM_ACK_Value |= TM_ACK_Model_App;
                // App_Ack(TM_ACK_Model_App);
                //  App_Update_Flag=1;
                //  App_Update_Cnt=0;
                break;
            case 0x30: // ???
                Key.Value = Key_Power;
                break;
            case 0x31: // ?????
                Key.Value = Key_Shoulder;
                break;
            case 0x32: // ????
                if( Key.Param == 0x01 )
                {
                    Key.Value = Key_ShoulderAdj_Jump;
                }
                else if( Key.Param == 0x02 )
                {
                    Key.Value = Key_BodyCheck_Jump;
                }
                else if( Key.Param == 0x03 )
                {
                    Key.Value = key_AutoLyJump;
                }
                else if( Key.Param == 0x04 )
                {
                    Key.Value = Key_BodyCheck_Jump;
                }
                break;
            // case 0x33:
            // case 0x38: // ????
            // Key.Value = Key_MasMode;
            // App_Update_Cnt = 0;
            // App_Update_Cnt1 = 0;
            // break;
            case 0x34: // ??
                Key.Value = Key_Pause;
                break;
            case 0x35: // ??
                // Key.Value = Key_Timing;
                if( ( WorkStateGet() == WorkState_Work ) ||
                        ( WorkStateGet() == WorkState_WaitFunc ) )
                {
                    i = WorkMinGet();
                    i *= 60;
                    i += WorkSecGet();
                    switch( Key.Param )
                    {
                        //???
                        case 0:
                            //                            i += 600;
                            //                            if(i > 2400)
                            //                            {
                            //                                i = 2400;
                            //                            }
                            if( i > 300 )
                            {
                                i -= 300;
                            }
                            break;
                        //???
                        case 1:
                            i += 300;
                            if( i > 2400 )
                            {
                                i = 2400;
                            }
                            break;
                        //                        case 2:
                        //                            if(i > 300)
                        //                            {
                        //                                i -= 300;
                        //                            }
                        //                            break;
                        //                        case 3:
                        //                            if(i > 600)
                        //                            {
                        //                                i -= 600;
                        //                            }
                        //                            break;
                        default:
                            break;
                    }
                    WorkMinSet( i / 60 );
                    WorkSecSet( i % 60 );
                    Buzzer = SOUND_ACK;
                }
                break;
            case 0x3e: // ????,????
                if( Key.Param == 0x10 )
                {
                    Key.Value = Key_Player;
                    Key.Param = 0;
                }
                else if( Key.Param == 0x11 )
                {
                    Key.Value = Key_Player;
                    Key.Param = 1;
                }
                break;
            case 0x3f: // ??
                Key.Value = Key_Volume;
                if( Key.Param > 15 )
                {
                    Key.Param = 15;
                }
                break;
            case 0x40: // ????
                if( nParam == 0xe1 )
                {
                    //Key.Value = Key_Memory_Use;
                    if( flash_read_AutoOnline( 0x8035000, AT_Auto16, AutoLoad_size_cnt[0] ) )
                    {
                        Key.Value = Key_AutoMode;
                        Key.Param = 16;
                    }
                    else
                    {
                        Buzzer = SOUND_ALARM;
                    }
                    // Key.Param = 1;
                }
                else if( nParam == 0xe2 )
                {
                    // Key.Value = Key_Memory_Use;
                    // Key.Param = 2;
                    if( flash_read_AutoOnline( 0x8035800, AT_Auto17, AutoLoad_size_cnt[1] ) )
                    {
                        Key.Value = Key_AutoMode;
                        Key.Param = 17;
                    }
                    else
                    {
                        Buzzer = SOUND_ALARM;
                    }
                }
                else if( nParam == 0xe3 )
                {
                    if( flash_read_AutoOnline( 0x8036000, AT_Auto18, AutoLoad_size_cnt[2] ) )
                    {
                        Key.Value = Key_AutoMode;
                        Key.Param = 18;
                    }
                    else
                    {
                        Buzzer = SOUND_ALARM;
                    }
                    // Key.Value = Key_Memory_Use;
                    // Key.Param = 3;
                }
                else if( nParam == 0xe4 )
                {
                    if( flash_read_AutoOnline( 0x8036800, AT_Auto19, AutoLoad_size_cnt[3] ) )
                    {
                        Key.Value = Key_AutoMode;
                        Key.Param = 19;
                    }
                    else
                    {
                        Buzzer = SOUND_ALARM;
                    }
                }
                else
                {
                    Key.Value = Key_AutoMode;
                }
                break;
            case 0x41: // ????
                if( ( WorkStateGet() == WorkState_Work ) ||
                        ( WorkStateGet() == WorkState_WaitFunc ) ||
                        ( AutoModeGet() != 0 ) )
                {
                    // if (MoveWhichGet())
                    // {
                    // Buzzer = SOUND_ALARM;
                    // return;
                    // }
                    if( MassWiseGet( MoveUp ) == JRZ )
                    {
                        Buzzer = SOUND_ALARM;
                        return;
                    }
                    //                    if(MoveWhichGet())
                    //                    {
                    //
                    //                      switch(Key.Param)
                    //                      {
                    //                          default:
                    //                          case 1://??
                    //                              MoveUpState.ManualArea_ID = 4;
                    //                          break;
                    //                          case 2://??
                    //                              MoveUpState.ManualArea_ID = 5;
                    //                          break;
                    //                          case 3://??
                    //                              MoveUpState.ManualArea_ID = 3;
                    //                          break;
                    //                          case 4://???
                    //                              MoveUpState.ManualArea_ID = 1;
                    //                          break;
                    //                          case 5://???
                    //                              MoveUpState.ManualArea_ID = 2;
                    //                          break;
                    //
                    //                      }
                    ////                        MoveUpState.ManualArea_ID = Key.Param;
                    //                        MassAreaSet(MoveDw, (MassageArea_E)MoveUpState.ManualArea_ID);
                    //                        MoveDwTx_DriveCmd = 1;
                    //                    }
                    //                    else
                    {
                        switch( Key.Param )
                        {
                            default:
                            case 1://??
                                MoveUpState.ManualArea_ID = 4;
                                break;
                            case 2://??
                                MoveUpState.ManualArea_ID = 5;
                                break;
                            case 3://??
                                MoveUpState.ManualArea_ID = 3;
                                break;
                            case 4://???
                                MoveUpState.ManualArea_ID = 1;
                                break;
                            case 5://???
                                MoveUpState.ManualArea_ID = 2;
                                break;
                        }
                        //                        MoveUpState.ManualArea_ID = Key.Param;
                        MassAreaSet( MoveUp, ( MassageArea_E )MoveUpState.ManualArea_ID );
                        MassArea_PartParamSet( MoveUp,
                                               0xABAB ); // todo ???? 250301
                        MoveUpTx_DriveCmd = 1;
                    }
                    // if (AutoModeGet() != 0)
                    // UserFunc_Set(UserFunc_ManualMode, 0);
                    Buzzer = SOUND_ACK;
                }
                break;
            case 0x42://?????
                if( ( WorkStateGet() == WorkState_Work ) ||
                        ( WorkStateGet() == WorkState_WaitFunc ) )
                {
                    if( AutoModeGet() > 0 )
                    {
                        UserFunc_Set( UserFunc_ManualMode, 0 );
                        i = 0;
                    }
                    else
                    {
                        if( MoveWhichGet() )
                        {
                            i = MassWiseGet( MoveDw );
                        }
                        else
                        {
                            i = MassWiseGet( MoveUp );
                        }
                    }
                    if( /*Manual_Select==0 &&*/ Key.Param == 6 /*i <= JRZFB && i >= JRZ*/ )
                    {
                        MoveUpTreadStateSet( TreadState_Stop, 0, 0 );
                        KeyPush.TreadKeyState = TreadState_Stop;
                    }
                    switch( Key.Param )
                    {
                        default: // ??
                            i = TZ;
                            break;
                        case 0: // ??1 ??
                            i = TZ;
                            break;
                        case 1: // ??
                            i = RNZ;
                            break;
                        case 2: // ??2
                            // i = ZYS; // todo ?? ????
                            i = PD;
                            break;
                        case 4: // ????
                            i = RPZ;
                            break;
                        case 8: // ??
                            i = PDB;
                            break;
                        case 5: // ??
                            i = ZYS;
                            break;
                        case 0x20: // 3D??
                            i = D3RNZ;
                            break;
                        case 0x21: // 3D??
                            i = D3PD;
                            // i = RNZB;
                            break;
                        case 0x22: // 3D????
                            i = D3RPZ;
                            // i = RNFB;
                            break;
                        case 0x24: // 3D??
                            i = ZYD;
                            // i = TJA;
                            break;
                    }
                    //                    if(MoveWhichGet())
                    //                    {
                    //                        if(0 == MassSpeedGet(MoveDw))
                    //                        {
                    //                            MassSpeedSet(MoveDw, 3);
                    //                        }
                    //                        MassWiseSet(MoveDw, (ManualWise_E)i);
                    //                        MoveDwTx_DriveCmd = 1;
                    //                    }
                    //                    else
                    {
                        if( 0 == MassSpeedGet( MoveUp ) )
                        {
                            MassSpeedSet( MoveUp, 3 );
                        }
                        MassWiseSet( MoveUp, ( ManualWise_E )i );
                        MoveUpTx_DriveCmd = 1;
                    }
                    KeyPush.WiseKeyState = 1;
                    Buzzer = SOUND_ACK;
                }
                break;
            case 0x43://?????
                if( WorkStateGet() == WorkState_Work && AutoModeGet() == 0 )
                {
                    if( MoveWhichGet() )
                    {
                        if( MassWiseGet( MoveDw ) == TZ )
                        {
                            Buzzer = SOUND_ALARM;
                            return;
                        }
                        i = MassSpeedGet( MoveDw );
                        if( Key.Param == 0x02 )
                        {
                            if( i > 1 )
                            {
                                MassSpeedSet( MoveDw, i - 1 );
                                Buzzer = SOUND_ACK; //* ???
                            }
                            else
                            {
                                Buzzer = SOUND_ALARM;
                            }
                        }
                        if( Key.Param == 0x01 )
                        {
                            if( i < 5 )
                            {
                                MassSpeedSet( MoveDw, i + 1 );
                                Buzzer = SOUND_ACK;
                            }
                            else
                            {
                                Buzzer = SOUND_ALARM;
                            }
                        }
                        MoveDwTx_DriveCmd = 1;
                        return;
                    }
                    if( MassWiseGet( MoveUp ) == ZYD ||
                            MassWiseGet( MoveUp ) == ZYS || MassWiseGet( MoveUp ) == TZ ||
                            MassWiseGet( MoveUp ) == ZY )
                    {
                        Buzzer = SOUND_ALARM;
                        return;
                    }
                    i = MassSpeedGet( MoveUp );
                    if( Key.Param == 0x02 )
                    {
                        if( i > 1 )
                        {
                            MassSpeedSet( MoveUp, i - 1 );
                            Buzzer = SOUND_ACK; //* ???
                        }
                        else
                        {
                            Buzzer = SOUND_ALARM;
                        }
                    }
                    if( Key.Param == 0x01 )
                    {
                        if( i < 5 )
                        {
                            MassSpeedSet( MoveUp, i + 1 );
                            Buzzer = SOUND_ACK; //* ???
                        }
                        else
                        {
                            Buzzer = SOUND_ALARM;
                        }
                    }
                    MoveUpTx_DriveCmd = 1;
                }
                break;
            case 0x44://?????
                if( ( ( WorkStateGet() == WorkState_Work ) ||
                        ( WorkStateGet() == WorkState_WaitFunc ) ) &&
                        AutoModeGet() == 0 )
                {
                    if( MoveWhichGet() )
                    {
                        if( ( RNZ == MassWiseGet( MoveDw ) ) ||
                                ( RPZ == MassWiseGet( MoveDw ) ) ||
                                ( JRZ == MassWiseGet( MoveDw ) ) )
                        {
                            Buzzer = SOUND_ALARM;
                            break;
                        }
                        i = MassWidthGet( MoveDw );
                        if( Key.Param == 0x02 ) //-
                        {
                            if( i > 0 )
                            {
                                MassWidthSet( MoveDw, i - 1 );
                                Buzzer = SOUND_ACK;
                            }
                            else
                            {
                                Buzzer = SOUND_ALARM;
                            }
                        }
                        else if( Key.Param == 0x01 ) //+
                        {
                            if( i < 4 )
                            {
                                MassWidthSet( MoveDw, i + 1 );
                                Buzzer = SOUND_ACK;
                            }
                            else
                            {
                                Buzzer = SOUND_ALARM;
                            }
                        }
                        MoveDwTx_DriveCmd = 1;
                        return;
                    }
                    if( ( RNZ == MassWiseGet( MoveUp ) ) ||
                            ( RPZ == MassWiseGet( MoveUp ) ) ||
                            ( JRZ == MassWiseGet( MoveUp ) ) )
                    {
                        Buzzer = SOUND_ALARM;
                        break;
                    }
                    i = MassWidthGet( MoveUp );
                    if( Key.Param == 0x02 )
                    {
                        if( i > 0 ) //* ???
                        {
                            MassWidthSet( MoveUp, i - 1 );
                            Buzzer = SOUND_ACK;
                        }
                        else
                        {
                            Buzzer = SOUND_ALARM;
                        }
                    }
                    else if( Key.Param == 0x01 )
                    {
                        if( i < 4 ) // todo ?? 5-4 250213
                        {
                            MassWidthSet( MoveUp, i + 1 );
                            Buzzer = SOUND_ACK;
                        }
                        else
                        {
                            Buzzer = SOUND_ALARM;
                        }
                    }
                    MoveUpTx_DriveCmd = 1;
                }
                break;
            case 0x45://?????
                /*
                if(Key.Param == 0x20)
                {
                Key.Param = nBackIntensity;

                if(Key.Param < 4)
                {
                Key.Value = Key_BackStrength;
                Key.Param++;
                }
                else
                {
                Buzzer = SOUND_ALARM;
                }
                }
                else if(Key.Param == 0x10)
                {
                Key.Param = nBackIntensity;

                if(Key.Param > 0)
                {
                Key.Value = Key_BackStrength;
                Key.Param--;
                }
                else
                {
                Buzzer = SOUND_ALARM;
                }
                }
                else
                {
                Key.Param = nBackIntensity;

                if(Key.Param == 4)
                {
                Key.Param = 0;
                }
                else
                {
                Key.Param++;
                }

                Key.Value = Key_BackStrength;
                }
                */
                if( ( WorkStateGet() == WorkState_Work ) || ( WorkStateGet() == WorkState_WaitFunc ) )
                {
                    if( MoveWhichGet() )
                    {
                        Buzzer = SOUND_ALARM; /* ???????4D?? */
                    }
                    else
                    {
                        if( nBackIntensity > 5 )
                        {
                            nBackIntensity = 1;
                        }
                        else
                        {
                            nBackIntensity++;
                        }
                    }
                    Buzzer = SOUND_ACK;
                    if( AutoModeGet() == 0 ) // todo ????????3D????????????????
                    {
                        MassIntensitySet( nBackIntensity << 1 );
                    }
                    MoveUpTx_DriveCmd = 1;
                    break;
                }
                break;
            case 0x46: // ????,????????
                // Key.Value = Key_UpperAir;
                // Key.Param = 1;
                //            // Key.Param = nCurAirState;
                // if (WorkStateGet == WorkState_Work)
                // {
                // if (Air_Stre_State > 0)
                // {
                // Buzzer = SOUND_ALARM;
                // break;
                // }
                // if (nCurAirState == Air_State_All)
                // Air_Set_Mode(Air_State_Stop);
                // else
                // Air_Set_Mode(Air_State_All);
                // Buzzer = SOUND_ACK;
                // }
                //                Key.Param = AirIntensityGet();
                Key.Value = Key_UpperAirStrength;
                Key.Param = 3;//Key_UpperAirStrength????,???3???????????
                //                if(Key.Param < 5)
                //                {
                //                    Key.Param++;
                //                }
                //                else
                //                {
                //                    Key.Param = 1;
                //                }
                break;
            case 0x47: // ???????
                /*
                if(Key.Param == 0x20)
                {
                Key.Param = AirIntensityGet();

                if(Key.Param < 5)
                {
                Key.Param++;
                Key.Value = Key_UpperAirStrength;
                }
                else
                {
                Buzzer = SOUND_ALARM;
                }
                }
                else if(Key.Param == 0x10)
                {
                Key.Param = AirIntensityGet();

                if(Key.Param > 1)
                {
                Key.Param--;
                Key.Value = Key_UpperAirStrength;
                }
                else
                {
                Buzzer = SOUND_ALARM;
                }
                }
                else if(Key.Param == 0x30)
                {
                Key.Value = Key_UpperAir;
                }
                */
                if( Key.Param == 0x30 )
                {
                    switch( AirModeGet() )
                    {
                        default:
                        case AirModeClose://
                            AirModeSet( AirModeUpper );
                            break;
                        case AirModeUpper://
                            AirModeSet( AirModeClose );
                            break;
                        case AirModeLower://
                            AirModeSet( AirModeFull );
                            break;
                        case AirModeFull://
                            AirModeSet( AirModeLower );
                            break;
                    }
                }
                break;
            case 0x48: // ???????
                //                if(Key.Param == 0x30)
                //                {
                //                    Key.Value = Key_LowerAir;
                //                }
                if( Key.Param == 0x30 )
                {
                    switch( AirModeGet() )
                    {
                        default:
                        case AirModeClose://
                            AirModeSet( AirModeLower );
                            break;
                        case AirModeUpper://
                            AirModeSet( AirModeFull );
                            break;
                        case AirModeLower://
                            AirModeSet( AirModeClose );
                            break;
                        case AirModeFull://
                            AirModeSet( AirModeUpper );
                            break;
                    }
                }
                break;
            case 0x4c:
                Key.Value = Key_BackHeat;
                // Key.Param = !nUpperHeat;
                break;
            case 0x4e:
                Key.Value = Key_LegsHeat;
                // Key.Param = !nLowerHeat;
                break;
            case 0x4f: // ROLL
                if( ( WorkStateGet() == WorkState_Work ) ||
                        ( WorkStateGet() == WorkState_WaitFunc ) )
                {
                    i = RollSpeedGet();
                    if( i < 3 )
                    {
                        RollSpeedSet( ++i );
                    }
                    else
                    {
                        RollSpeedSet( 0 );
                    }
                    if( RollSpeedGet() )
                    {
                        RollModeSet( 1 );
                        RollStateSet( 1 );
                    }
                    else
                    {
                        RollModeSet( 0 );
                        RollStateSet( 0 );
                    }
                    KeyPush.FootRollKeyState = 1;
                    LegTx_DriveCmd = 1;
                    Buzzer = SOUND_ACK; //* ??
                }
                break;
            case 0x4a:
                Key.Value = Key_LegsKnead;
                break;
            case 0x50:
                Key.Value = Key_Lying;
                break;
            case 0x51:
            case 0x54:
                Key.Value = Key_LegsFlex;
                break;
            case 0x52:
                Key.Value = Key_BackLying;
                break;
            case 0x53:
                Key.Value = Key_LegsUpDown;
                break;
            case 0x55:
                Key.Value = Key_Lying;
                break;
            case 0x56:
                Key.Value = Key_Zero;
                break;
            case 0x57:
                Key.Value = Key_BackTread;
                break;
            case 0x58:
                Key.Value = Key_Voice;
                Key.Param = !nVoiceControlSw;
                break;
            case 0x59://?????
                // Key.Value = Key_Voice;
                // Key.Param = 0x10;
                if( nVoiceControlSw )
                {
                    Voice_WakeUp();
                }
                else
                {
                    Buzzer = SOUND_ALARM;
                }
                break;
            case 0x5A: // ???
                Key.Value = Key_Ion;
                Key.Param = !nIonState;
                break;
            case 0x5b:
                // if(Key.State == 2)
                // {
                // if(nBlueState)
                // {
                // nBlueState = 0;
                // PrintString5("AT+FUN=BREDR=DIS\r\n");
                // }
                // else
                // {
                // nBlueState = 1;
                // PrintString5("AT+FUN=BREDR=EN\r\n");
                // }
                // Flash_Check_Tick = Flash_Check_Time;
                // Buzzer = SOUND_ACK;
                // }
                // nBlueState = !nBlueState;
                // Flash_Check_Tick = Flash_Check_Time;
                // Buzzer = SOUND_ACK;
                Key.Value = Key_Blue;
                break;
            case 0x5e:
                Key.Value = Key_Health;
                break;
            case 0xe0:
                Key.Param >>= 4;
                if( Key.Param >= 1 && Key.Param <= 3 )
                {
                    Key.Value = Key_Memory_Save;
                }
                break;
            default:
                break;
        }
        if( Key.Value > 0 )
        {
            KeyFunc();
        }
    }
}

void os_task_app( OS_TASK_ID nTask_ID ) // app??
{
    // APP??????
    Pt_Start
    if( ota_over_time > 0 )
    {
        Pt_Wait_Tick( 10 ); // 50mS
        TM_ACK_Value = TM_ACK_Valid_Sever;
        App_Ack();
    }
    while( 1 )
    {
        Pt_Wait_Tick( 50 ); // 250ms/5ms = 50
        if( ota_over_time > 0 )
        {
            ota_over_time--;
            if( ota_over_time == 0 )
            {
                if( ota_state == ota_state_end ) // ????????????????
                {
                    NVIC_SystemReset(); // ????
                    //UserFunc_Set(UserFunc_PowerOn, 0);
                }
                else
                {
                    TM_ACK_Value = TM_ACK_DisValid_Sever;
                    App_Ack();
                }
                ota_state = ota_state_stop;
            }
        }
        else
        {
            if( TM_ACK_Value )
            {
                App_Ack();
            }
            else
            {
                App_Update_State();
            }
        }
    }
    Pt_End
}

#define AppComHead1 0xAA
#define AppComHead2 0x55
#define AppComTial1 0x0D
#define AppComTial2 0x0A

static RxPro_S RxVar;

void AppCom( u8 dat )
{
    RxVar.RxBuffer[RxVar.RxCount] = dat;
    switch( RxVar.RxState )
    {
        case 0://????1
            if( RxVar.RxBuffer[RxVar.RxCount] == AppComHead1 )
            {
                RxVar.RxCount++;
                RxVar.RxState++;
            }
            else
            {
                RxVar.RxCount = 0;
                RxVar.RxState = 0;
            }
            break;
        case 1://????2
            if( RxVar.RxBuffer[RxVar.RxCount] == AppComHead2 )
            {
                RxVar.RxCount++;
                RxVar.RxState++;
            }
            else
            {
                RxVar.RxCount = 0;
                RxVar.RxState = 0;
            }
            break;
        case 2://??????
            if( RxVar.RxBuffer[RxVar.RxCount] == 0x02 )
            {
                RxVar.RxDataNum = 6;
                RxVar.RxCount++;
                RxVar.RxState++;
            }
            //???
            else if( RxVar.RxBuffer[RxVar.RxCount] == 0x03 )
            {
                RxVar.RxCount++;
                RxVar.RxState = 5;
            }
            else
            {
                RxVar.RxCount = 0;
                RxVar.RxState = 0;
            }
            break;
        case 3://????
            if( RxVar.RxDataNum > 0 )
            {
                RxVar.RxDataNum--;
                RxVar.RxCount++;
            }
            else
            {
                if( RxVar.RxBuffer[RxVar.RxCount] == AppComTial1 )
                {
                    RxVar.RxCount++;
                    RxVar.RxState++;
                }
                else
                {
                    RxVar.RxCount = 0;
                    RxVar.RxState = 0;
                }
            }
            break;
        case 4: //????
            if( RxVar.RxBuffer[RxVar.RxCount] == AppComTial2 )
            {
                App_Cmd_Handler( RxVar.RxBuffer, RxVar.RxDataLen );
            }
            RxVar.RxState = 0;
            RxVar.RxCount = 0;
            break;
        case 5:  //??
            RxVar.RxDataCmd = RxVar.RxBuffer[RxVar.RxCount];
            if( RxVar.RxDataCmd == 0xF1 )
            {
                RxVar.RxDataNum = 5;
                RxVar.RxCount++;
                RxVar.RxState = 3;
            }
            else if( RxVar.RxDataCmd == 0xF4 || RxVar.RxDataCmd == 0xF5 )
            {
                RxVar.RxState++;
                RxVar.RxCount++;
            }
            break;
        /*****????????********/
        case 6:
            RxVar.RxDataParm = RxVar.RxBuffer[RxVar.RxCount];
            if( RxVar.RxDataParm == 0x01 )
            {
                RxVar.RxDataNum = 8;
                RxVar.RxCount++;
                RxVar.RxState = 3;
            }
            else if( RxVar.RxDataParm == 0x02 )
            {
                RxVar.RxState++;
                RxVar.RxCount++;
            }
            else if( RxVar.RxDataParm == 0x03 )
            {
                RxVar.RxDataNum = 4;
                RxVar.RxCount++;
                RxVar.RxState = 3;
            }
            break;
        case 7:
            RxVar.RxDataNoPage = RxVar.RxBuffer[RxVar.RxCount];
            RxVar.RxState++;
            RxVar.RxCount++;
            break;
        case 8:
            RxVar.RxDataLenH8 = RxVar.RxBuffer[RxVar.RxCount];
            RxVar.RxState++;
            RxVar.RxCount++;
            break;
        case 9:
            RxVar.RxDataLenL8 = RxVar.RxBuffer[RxVar.RxCount];
            RxVar.RxDataLen = ( RxVar.RxDataLenH8 << 8 | RxVar.RxDataLenL8 );
            RxVar.RxDataNum = RxVar.RxDataLen + 1;
            RxVar.RxState = 3;
            RxVar.RxCount++;
            break;
    }
}

void App_init( void )
{
    TM_ACK_Value = TM_ACK_NULL; // ???????
    ota_state = 0;
    ota_over_time = 0;
    Pt_Reset_ID( TASK_ID_APP );
    //Pt_Pause_ID(TASK_ID_APP);
}
