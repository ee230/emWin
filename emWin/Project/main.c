#include "sys.h"
#include "holl.h"
#include "tftlcd.h"



int main(void)
{ 
	u16 val;
	Stm32_Clock_Init(336,8,2,7);
	delay_init(168);
	uart_init(84,115200);
	Holl_Init();
	lcd.Init();
	
	while(1)
	{
		//val=Get_Adc(Holl1_CH5);
		
		delay_ms(500);
	}
}
