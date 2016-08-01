#ifndef __7LCD_H
#define __7LCD_H
#include "sys.h"


//-----------------LCD端口定义---------------- 
//#define	LCD_LED PBout(0) 	//LCD背光不由IO口控制,有驱动器自己控制,通过指令设置   
//#define	LCD_RST PBout(15) 	//LCD复位口,和开发板共用,这里设置为PB0,其实并没有用到    
//LCD地址结构体
typedef struct
{
	u16 LCD_REG;
	u16 LCD_RAM;
} LCD_TypeDef;
//使用NOR/SRAM的 Bank1.sector4,地址位HADDR[27,26]=11 A6作为数据命令区分线 
//注意设置时STM32内部会右移一位对其! 111 1110=0X7E			    
#define LCD_BASE        ((u32)(0x6C000000 | 0x0000007E))
#define LCD             ((LCD_TypeDef *) LCD_BASE)

//扫描方向定义
#define L2R_U2D  0 //从左到右,从上到下
#define L2R_D2U  1 //从左到右,从下到上
#define R2L_U2D  2 //从右到左,从上到下
#define R2L_D2U  3 //从右到左,从下到上

#define U2D_L2R  4 //从上到下,从左到右
#define U2D_R2L  5 //从上到下,从右到左
#define D2U_L2R  6 //从下到上,从左到右
#define D2U_R2L  7 //从下到上,从右到左	 

#define DFT_SCAN_DIR  L2R_U2D  //默认的扫描方向
//////////////////////////////////////////////////////////////////////////////////
//LCD 指令定义
#define LCD_CUR_Y  		0X00
#define LCD_CUR_X  		0X01
#define LCD_PIXELS 		0X02
#define LCD_END_X 		0X03
#define LCD_PREF 		0X05
#define LCD_MIRROR 		0X07

//LCD重要参数集
typedef struct  
{										    
	u16 width;			//LCD 宽度
	u16 height;			//LCD 高度
	u16 id;				//LCD ID（ATK-7' TFTLCD不支持id读）
	u8  dir;			//横屏还是竖屏控制：0，竖屏；1，横屏。	
	u16 sysreg;			//PREF寄存器，设置当前操作页，显示页和背光等（ATK-7' TFTLCD暂不支持横竖屏设置，统一为横屏）。	
	u16	wramcmd;		//开始写gram指令
	u16  setxcmd;		//设置x坐标指令
	u16  setycmd;		//设置y坐标指令  
}_7LCDDev; 	  
//LCD参数
extern _7LCDDev lcddev;	//管理LCD重要参数



typedef struct//颜色定义
{
	u16 White;
	u16 Black;
	u16 Blue;
	u16 Bred;
	u16 Gred;
	u16 Gblue;
	u16 Red;
	u16 Magenta;
	u16 Green;
	u16 Cyan;
	u16 Yellow;
	u16 Brown;
	u16 Brred;
	u16 Gray;
	
	u16 Darkblue;
	u16 Lightblue;
	u16 Grayblue;
	
	u16 Lightgreen;
	u16 Lgray;
	u16 Lgrayblue;
	u16 Lbblue;
}ColorBast;

extern const ColorBast Color;


typedef struct
{
	void (*Init)(void);
	void (*Clear)(u16 color);
	void (*DrawPoint)(u16 x,u16 y);
	u16 (*ReadPoint)(u16 x,u16 y);
	void (*DrawLine)(u16 x1, u16 y1, u16 x2, u16 y2);
	void (*DrawCircle)(u16 x0,u16 y0,u8 r);
	void (*DrawRectangle)(u16 x1, u16 y1, u16 x2, u16 y2);
	void (*Fill)(u16 sx,u16 sy,u16 ex,u16 ey,u16 color);
	void (*ShowChar)(u16 x,u16 y,u8 num,u8 size,u8 mode);
	void (*ShowNum)(u16 x,u16 y,u32 num,u8 len,u8 size);
	void (*ShowxNum)(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode);
	void (*ShowString)(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p);
	u16 *PC;
}_7LCDBast;

extern const _7LCDBast _7LCD;

#endif
