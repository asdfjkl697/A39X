#include "NOI.h"

// uint8_t NOI_Step=0;
// uint32_t NOI_Tick=0;

// void NOI_Process(void){

//	switch(NOI_Step){
//
//
//		case 0:
//
//					NOI_ON();
//					FS_ON();

//					TICK_RESET(NOI_Tick);
//					NOI_Step++;
//

//
//		break;
//
//
//		case 1:
//
//					if(TICK_COUNTER(NOI_Tick,2000)){
//
//												NOI_OFF();
//									FS_OFF();

//							TICK_RESET(NOI_Tick);
//							NOI_Step++;
//					}
//
//
//		break;
//
//		case 2:
//
//					if(TICK_COUNTER(NOI_Tick,1000)){
//
//								NOI_Step=0;
//
//					}
//
//
//		break;
//
//
//
//
//
//	}
//
//
//}
