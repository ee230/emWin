#include "sys.h"
#include "7lcd.h"


int main(void)
{ 
	u16 val;
	Stm32_Clock_Init(336,8,2,7);
	delay_init(168);
	uart_init(84,115200);
	_7LCD.Init();
	_7LCD.Clear(Color.White);
	*_7LCD.PC=Color.Bred;
	_7LCD.DrawCircle(20,20,10);
	*_7LCD.PC=Color.Black;
	_7LCD.DrawLine(100,100,200,200);
	_7LCD.Fill(50,50,100,100,Color.Blue);
	_7LCD.ShowString(200,400,200,20,16,"ABCDEFG");
	_7LCD.ShowxNum(200,220,200,3,16,1);
	*_7LCD.PC=Color.Gray;
	_7LCD.DrawRectangle(220,200,300,300);
	
	
	while(1)
	{
		
		printf("%d\r\n",val);
		
		delay_ms(500);
	}
}
