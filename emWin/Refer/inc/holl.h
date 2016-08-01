#ifndef _HOLL_H_
#define _HOLL_H_
#include "stm32f4xx.h"


#define Holl1_CH5  		5 		 	//Í¨µÀ5	   	

void Holl_Init(void);
u16 Get_Adc(u8 ch);
u16 Get_Adc_Average(u8 ch,u8 times);

#endif
