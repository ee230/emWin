#include "7lcd.h"
#include "FONT.H"


_7LCDDev lcddev;
//LCD的画笔颜色和背景色	   
u16 POINT_COLOR=0x0000;	//画笔颜色
u16 BACK_COLOR=0xFFFF;  //背景色 


u16 RecvData(void)
{										    	   
	return LCD->LCD_RAM;		 
}

//写寄存器
//LCD_Reg:寄存器地址
//LCD_RegValue:要写入的数据
void _7LCDWriteReg(u8 LCD_Reg, u16 LCD_RegValue)
{	
	LCD->LCD_REG = LCD_Reg;		//写入要写的寄存器序号	 
	LCD->LCD_RAM = LCD_RegValue;//写入数据	    		 
}	   
//LCD背光设置
//pwm:背光等级,0~63.越大越亮.
void _7LCDBackLightSet(u8 pwm)
{	
	lcddev.sysreg&=~0X003F;					//清除之前的设置
	lcddev.sysreg|=pwm&0X3F;				//设置新的值		
	_7LCDWriteReg(LCD_PREF,lcddev.sysreg);	//写LCD_PREF寄存器	   	 	 
}
//扫描方向上,X的终点坐标.
void _7LCDEndXSet(u16 x)
{													    		
	_7LCDWriteReg(LCD_END_X,x);				//设置X终坐标  	 	 
}
//设置当前显示层
//layer:当前显示层 
void _7LCDSetDisplayLayer(u16 layer)
{	 
	lcddev.sysreg&=~0X0E00;				   	//清除之前的设置
	lcddev.sysreg|=(layer&0X07)<<9;			//设置新的值
	_7LCDWriteReg(LCD_PREF,lcddev.sysreg);	//写LCD_PREF寄存器	   	 	 
} 
//设置当前操作层
//layer:当前显示层 
void _7LCDSetOperateLayer(u16 layer)
{	 
	lcddev.sysreg&=~0X7000;					//清除之前的设置
	lcddev.sysreg|=(layer&0X07)<<12;		//设置新的值
	_7LCDWriteReg(LCD_PREF,lcddev.sysreg);	//写LCD_PREF寄存器	   	 	 
}
//设置LCD的自动扫描方向
//注意:我们的驱动器,只支持左右扫描设置,不支持上下扫描设置   	   
void _7LCDScanDir(u8 dir)
{			   
  switch(dir)
	{
		case L2R_U2D:	//从左到右,从上到下
			_7LCDWriteReg(LCD_MIRROR,1);	//写LCD_PREF寄存器
			break;
		case L2R_D2U:	//从左到右,从下到上
			_7LCDWriteReg(LCD_MIRROR,3);	//写LCD_PREF寄存器
			break;
		case R2L_U2D:	//从右到左,从上到下
			_7LCDWriteReg(LCD_MIRROR,0);	//写LCD_PREF寄存器
			break;
		case R2L_D2U:	//从右到左,从下到上
			_7LCDWriteReg(LCD_MIRROR,2);	//写LCD_PREF寄存器
			break;	
		default:		//其他,默认从左到右,从上到下
			_7LCDWriteReg(LCD_MIRROR,1);	//写LCD_PREF寄存器
			break;	 
	}
}   
//设置光标位置
//Xpos:横坐标
//Ypos:纵坐标
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{	 
	_7LCDWriteReg(lcddev.setycmd,Ypos);	//设置Y坐标 	 
	_7LCDWriteReg(lcddev.setxcmd,Xpos);	//设置X坐标 	 
}
//开始写GRAM
void _7LCD_WriteRAM_Prepare(void)
{
 	LCD->LCD_REG=lcddev.wramcmd;	  
}	 



//显示字符串
//x,y:起点坐标
//width,height:区域大小  
//size:字体大小
//*p:字符串起始地址		  
void _7LCDShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p)
{         
	u8 x0=x;
	width+=x;
	height+=y;
    while((*p<='~')&&(*p>=' '))//判断是不是非法字符!
    {       
        if(x>=width){x=x0;y+=size;}
        if(y>=height)break;//退出
        _7LCD.ShowChar(x,y,*p,size,0);
        x+=size/2;
        p++;
    }  
}

//m^n函数
//返回值:m^n次方.
u32 LCD_Pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}	
//显示数字,高位为0,还是显示
//x,y:起点坐标
//num:数值(0~999999999);	 
//len:长度(即要显示的位数)
//size:字体大小
//mode:
//[7]:0,不填充;1,填充0.
//[6:1]:保留
//[0]:0,非叠加显示;1,叠加显示.
void _7LCDShowxNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode)
{  
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				if(mode&0X80)_7LCD.ShowChar(x+(size/2)*t,y,'0',size,mode&0X01);  
				else _7LCD.ShowChar(x+(size/2)*t,y,' ',size,mode&0X01);  
 				continue;
			}else enshow=1; 
		 	 
		}
		_7LCD.ShowChar(x+(size/2)*t,y,temp+'0',size,mode&0X01); 
	}
} 

//显示数字,高位为0,则不显示
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//color:颜色 
//num:数值(0~4294967295);	 
void _7LCDShowNum(u16 x,u16 y,u32 num,u8 len,u8 size)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				_7LCD.ShowChar(x+(size/2)*t,y,' ',size,0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	_7LCD.ShowChar(x+(size/2)*t,y,temp+'0',size,0); 
	}
} 
//在指定位置显示一个字符
//x,y:起始坐标
//num:要显示的字符:" "--->"~"
//size:字体大小 12/16
//mode:叠加方式(1)还是非叠加方式(0)
void _7LCDShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode)
{  							  
    u8 temp,t1,t;
	u16 y0=y;
	u16 colortemp=POINT_COLOR;      			     
	//设置窗口		   
	num=num-' ';//得到偏移后的值
	if(!mode) //非叠加方式
	{
	    for(t=0;t<size;t++)
	    {   
			if(size==12)temp=asc2_1206[num][t];  //调用1206字体
			else temp=asc2_1608[num][t];		 //调用1608字体 	                          
	        for(t1=0;t1<8;t1++)
			{			    
		        if(temp&0x80)POINT_COLOR=colortemp;
				else POINT_COLOR=BACK_COLOR;
				_7LCD.DrawPoint(x,y);	
				temp<<=1;
				y++;
				if(x>=lcddev.width){POINT_COLOR=colortemp;return;}//超区域了
				if((y-y0)==size)
				{
					y=y0;
					x++;
					if(x>=lcddev.width){POINT_COLOR=colortemp;return;}//超区域了
					break;
				}
			}  	 
	    }    
	}else//叠加方式
	{
	    for(t=0;t<size;t++)
	    {   
			if(size==12)temp=asc2_1206[num][t];  //调用1206字体
			else temp=asc2_1608[num][t];		 //调用1608字体 	                          
	        for(t1=0;t1<8;t1++)
			{			    
		        if(temp&0x80)_7LCD.DrawPoint(x,y); 
				temp<<=1;
				y++;
				if(x>=lcddev.height){POINT_COLOR=colortemp;return;}//超区域了
				if((y-y0)==size)
				{
					y=y0;
					x++;
					if(x>=lcddev.width){POINT_COLOR=colortemp;return;}//超区域了
					break;
				}
			}  	 
	    }     
	}
	POINT_COLOR=colortemp;	    	   	 	  
}  
//在指定区域内填充单个颜色
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)   
//color:要填充的颜色
void _7LCDFill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color)
{          
	u16 i,j;
	u16 xlen=0;
	xlen=ex-sx+1;	   
	for(i=sy;i<=ey;i++)
	{
	 	LCD_SetCursor(sx,i);      				//设置光标位置 
		_7LCD_WriteRAM_Prepare();     			//开始写入GRAM	  
		for(j=0;j<xlen;j++)LCD->LCD_RAM=color;	//送入LCD	    
	}
}  
//画矩形	  
//(x1,y1),(x2,y2):矩形的对角坐标
void _7LCDDrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
	_7LCD.DrawLine(x1,y1,x2,y1);
	_7LCD.DrawLine(x1,y1,x1,y2);
	_7LCD.DrawLine(x1,y2,x2,y2);
	_7LCD.DrawLine(x2,y1,x2,y2);
}
//在指定位置画一个指定大小的圆
//(x,y):中心点
//r    :半径
void _7LCDDrawCircle(u16 x0,u16 y0,u8 r)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //判断下个点位置的标志
	while(a<=b)
	{
		_7LCD.DrawPoint(x0+a,y0-b);             //5
 		_7LCD.DrawPoint(x0+b,y0-a);             //0           
		_7LCD.DrawPoint(x0+b,y0+a);             //4               
		_7LCD.DrawPoint(x0+a,y0+b);             //6 
		_7LCD.DrawPoint(x0-a,y0+b);             //1       
 		_7LCD.DrawPoint(x0-b,y0+a);             
		_7LCD.DrawPoint(x0-a,y0-b);             //2             
  	_7LCD.DrawPoint(x0-b,y0-a);             //7     	         
		a++;
		//使用Bresenham算法画圆     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 						    
	}
} 									  

//画线
//x1,y1:起点坐标
//x2,y2:终点坐标  
void _7LCDDrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出 
	{  
		_7LCD.DrawPoint(uRow,uCol);
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}
//读取个某点的颜色值	 
//x,y:坐标
//返回值:此点的颜色
u16 _7LCDReadPoint(u16 x,u16 y)
{
 	u16 t=0;
 	LCD_SetCursor(x,y);	    
	while(t<0X1FFF)
	{	
		if(LCD->LCD_REG&0x0001)break; 
		t++;		   
	}					   
	return LCD->LCD_RAM; 	
}
//画点
//x,y:坐标
//POINT_COLOR:此点的颜色
void _7LCDDrawPoint(u16 x,u16 y)
{
	LCD->LCD_REG=lcddev.setycmd;
	LCD->LCD_RAM=y; 
 	LCD->LCD_REG=lcddev.setxcmd; 
	LCD->LCD_RAM=x; 
	LCD->LCD_REG=lcddev.wramcmd; 
	LCD->LCD_RAM=POINT_COLOR; 
}
//清屏函数
//color:要清屏的填充色
static void _7LCDClear(u16 color)
{
	u32 index=0;      
	u32 totalpoint=lcddev.width;
	totalpoint*=lcddev.height; 	//得到总点数
	LCD_SetCursor(0x00,0x0000);	//设置光标位置 
	_7LCD_WriteRAM_Prepare();     //开始写入GRAM	 	  
	for(index=0;index<totalpoint;index++)
	{
		LCD->LCD_RAM=color;	   
	}
} 
static void _7LCDInit()
{
	u16 i=0;									  
	RCC->AHB1ENR|=0XF<<3;    	//使能PD,PE,PF,PG时钟  
	RCC->AHB1ENR|=1<<1;     	//使能PB时钟  
	RCC->AHB3ENR|=1<<0;     	//使能FSMC时钟  
	GPIO_Set(GPIOB,PIN15,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_50M,GPIO_PUPD_PU);							//PB15 推挽输出,控制背光
	GPIO_Set(GPIOD,(3<<0)|(3<<4)|(7<<8)|(3<<14),GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PU);	//PD0,1,4,5,8,9,10,14,15 AF OUT
	GPIO_Set(GPIOE,(0X1FF<<7),GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PU);						//PE7~15,AF OUT
	GPIO_Set(GPIOF,PIN12,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PU);							//PF12,FSMC_A6
	GPIO_Set(GPIOG,PIN12,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PU);							//PG12,FSMC_NE4
 	GPIO_AF_Set(GPIOD,0,12);	//PD0,AF12
 	GPIO_AF_Set(GPIOD,1,12);	//PD1,AF12
 	GPIO_AF_Set(GPIOD,4,12);	//PD4,AF12
 	GPIO_AF_Set(GPIOD,5,12);	//PD5,AF12 
 	GPIO_AF_Set(GPIOD,8,12);	//PD8,AF12
 	GPIO_AF_Set(GPIOD,9,12);	//PD9,AF12
 	GPIO_AF_Set(GPIOD,10,12);	//PD10,AF12 
 	GPIO_AF_Set(GPIOD,14,12);	//PD14,AF12
 	GPIO_AF_Set(GPIOD,15,12);	//PD15,AF12
	
 	GPIO_AF_Set(GPIOE,7,12);	//PE7,AF12
 	GPIO_AF_Set(GPIOE,8,12);	//PE8,AF12
 	GPIO_AF_Set(GPIOE,9,12);	//PE9,AF12
 	GPIO_AF_Set(GPIOE,10,12);	//PE10,AF12
 	GPIO_AF_Set(GPIOE,11,12);	//PE11,AF12
 	GPIO_AF_Set(GPIOE,12,12);	//PE12,AF12
 	GPIO_AF_Set(GPIOE,13,12);	//PE13,AF12
 	GPIO_AF_Set(GPIOE,14,12);	//PE14,AF12
 	GPIO_AF_Set(GPIOE,15,12);	//PE15,AF12
	
 	GPIO_AF_Set(GPIOF,12,12);	//PF12,AF12
 	GPIO_AF_Set(GPIOG,12,12);	//PG12,AF12
	 
	//寄存器清零
	//bank1有NE1~4,每一个有一个BCR+TCR，所以总共八个寄存器。
	//这里我们使用NE1 ，也就对应BTCR[0],[1]。				    
	FSMC_Bank1->BTCR[6]=0X00000000;
	FSMC_Bank1->BTCR[7]=0X00000000;
	FSMC_Bank1E->BWTR[6]=0X00000000;
	//操作BCR寄存器	使用异步模式
	FSMC_Bank1->BTCR[6]|=1<<12;		//存储器写使能
	FSMC_Bank1->BTCR[6]|=1<<14;		//读写使用不同的时序
	FSMC_Bank1->BTCR[6]|=1<<4; 		//存储器数据宽度为16bit 	    
	//操作BTR寄存器	
	//读时序控制寄存器 							    
	FSMC_Bank1->BTCR[7]|=0<<28;		//模式A 	 						  	 
	FSMC_Bank1->BTCR[7]|=6<<0; 		//地址建立时间(ADDSET)为6个HCLK 1/168M=6ns*6=36ns	 
	FSMC_Bank1->BTCR[7]|=5<<8;  	//数据保存时间(DATAST)为6个HCLK	=6*6=36ns
	//写时序控制寄存器  
	FSMC_Bank1E->BWTR[6]|=0<<28; 	//模式A 	 							    
	FSMC_Bank1E->BWTR[6]|=6<<0;		//地址建立时间(ADDSET)为6个HCLK=36ns  
	FSMC_Bank1E->BWTR[6]|=5<<8; 	//数据保存时间(DATAST)为6ns*6个HCLK=36ns
	//使能BANK1,区域4
	FSMC_Bank1->BTCR[6]|=1<<0;		//使能BANK1，区域1	     
  	//LCD_RST=0;	//探索者STM32F407开发板的RESET和LCD的RST共用，并未用普通IO控制复位
	delay_ms(100);
	//LCD_RST=1;	   
	while(i<0X1FFF) 
	{
		if(RecvData()&0x0001)break;//等待控制器准备好
		i++;
	} 
  lcddev.setxcmd=LCD_CUR_X;	//设置写X坐标指令
 	lcddev.setycmd=LCD_CUR_Y;	//设置写Y坐标指令
 	lcddev.wramcmd=LCD_PIXELS;	//设置写入GRAM的指令
 	lcddev.width=800;			//设置宽度
 	lcddev.height=480;			//设置高度
	_7LCDScanDir(L2R_U2D);		//设置默认扫描方向.	 
	_7LCDSetDisplayLayer(0);	 	//显示层为0
	_7LCDSetOperateLayer(0);		//操作层也为0
	_7LCDEndXSet(799);			//x终点坐标为800
	_7LCDBackLightSet(63);		//背光设置为最亮
	_7LCDClear(Color.Black);			//清屏
}



const ColorBast Color=
{
	0xFFFF,
	0x0000,
	0x001F,
	0XF81F,
	0XFFE0,
	0X07FF,
	0xF800,
	0xF81F,
	0x07E0,
	0x7FFF,
	0xFFE0,
	0XBC40,
	0XFC07,
	0X8430,
	0X01CF,
	0X7D7C,
	0X5458,
	0X841F,
	0XC618,
	0XA651,
	0X2B12,
};
const _7LCDBast _7LCD=
{
	_7LCDInit,
	_7LCDClear,
	_7LCDDrawPoint,
	_7LCDReadPoint,
	_7LCDDrawLine,
	_7LCDDrawCircle,
	_7LCDDrawRectangle,
	_7LCDFill,
	_7LCDShowChar,
	_7LCDShowNum,
	_7LCDShowxNum,
	_7LCDShowString,
	(u16 *)&POINT_COLOR,
};


