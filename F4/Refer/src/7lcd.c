#include "7lcd.h"
#include "FONT.H"


_7LCDDev lcddev;
//LCD�Ļ�����ɫ�ͱ���ɫ	   
u16 POINT_COLOR=0x0000;	//������ɫ
u16 BACK_COLOR=0xFFFF;  //����ɫ 


u16 RecvData(void)
{										    	   
	return LCD->LCD_RAM;		 
}

//д�Ĵ���
//LCD_Reg:�Ĵ�����ַ
//LCD_RegValue:Ҫд�������
void _7LCDWriteReg(u8 LCD_Reg, u16 LCD_RegValue)
{	
	LCD->LCD_REG = LCD_Reg;		//д��Ҫд�ļĴ������	 
	LCD->LCD_RAM = LCD_RegValue;//д������	    		 
}	   
//LCD��������
//pwm:����ȼ�,0~63.Խ��Խ��.
void _7LCDBackLightSet(u8 pwm)
{	
	lcddev.sysreg&=~0X003F;					//���֮ǰ������
	lcddev.sysreg|=pwm&0X3F;				//�����µ�ֵ		
	_7LCDWriteReg(LCD_PREF,lcddev.sysreg);	//дLCD_PREF�Ĵ���	   	 	 
}
//ɨ�跽����,X���յ�����.
void _7LCDEndXSet(u16 x)
{													    		
	_7LCDWriteReg(LCD_END_X,x);				//����X������  	 	 
}
//���õ�ǰ��ʾ��
//layer:��ǰ��ʾ�� 
void _7LCDSetDisplayLayer(u16 layer)
{	 
	lcddev.sysreg&=~0X0E00;				   	//���֮ǰ������
	lcddev.sysreg|=(layer&0X07)<<9;			//�����µ�ֵ
	_7LCDWriteReg(LCD_PREF,lcddev.sysreg);	//дLCD_PREF�Ĵ���	   	 	 
} 
//���õ�ǰ������
//layer:��ǰ��ʾ�� 
void _7LCDSetOperateLayer(u16 layer)
{	 
	lcddev.sysreg&=~0X7000;					//���֮ǰ������
	lcddev.sysreg|=(layer&0X07)<<12;		//�����µ�ֵ
	_7LCDWriteReg(LCD_PREF,lcddev.sysreg);	//дLCD_PREF�Ĵ���	   	 	 
}
//����LCD���Զ�ɨ�跽��
//ע��:���ǵ�������,ֻ֧������ɨ������,��֧������ɨ������   	   
void _7LCDScanDir(u8 dir)
{			   
  switch(dir)
	{
		case L2R_U2D:	//������,���ϵ���
			_7LCDWriteReg(LCD_MIRROR,1);	//дLCD_PREF�Ĵ���
			break;
		case L2R_D2U:	//������,���µ���
			_7LCDWriteReg(LCD_MIRROR,3);	//дLCD_PREF�Ĵ���
			break;
		case R2L_U2D:	//���ҵ���,���ϵ���
			_7LCDWriteReg(LCD_MIRROR,0);	//дLCD_PREF�Ĵ���
			break;
		case R2L_D2U:	//���ҵ���,���µ���
			_7LCDWriteReg(LCD_MIRROR,2);	//дLCD_PREF�Ĵ���
			break;	
		default:		//����,Ĭ�ϴ�����,���ϵ���
			_7LCDWriteReg(LCD_MIRROR,1);	//дLCD_PREF�Ĵ���
			break;	 
	}
}   
//���ù��λ��
//Xpos:������
//Ypos:������
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{	 
	_7LCDWriteReg(lcddev.setycmd,Ypos);	//����Y���� 	 
	_7LCDWriteReg(lcddev.setxcmd,Xpos);	//����X���� 	 
}
//��ʼдGRAM
void _7LCD_WriteRAM_Prepare(void)
{
 	LCD->LCD_REG=lcddev.wramcmd;	  
}	 



//��ʾ�ַ���
//x,y:�������
//width,height:�����С  
//size:�����С
//*p:�ַ�����ʼ��ַ		  
void _7LCDShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p)
{         
	u8 x0=x;
	width+=x;
	height+=y;
    while((*p<='~')&&(*p>=' '))//�ж��ǲ��ǷǷ��ַ�!
    {       
        if(x>=width){x=x0;y+=size;}
        if(y>=height)break;//�˳�
        _7LCD.ShowChar(x,y,*p,size,0);
        x+=size/2;
        p++;
    }  
}

//m^n����
//����ֵ:m^n�η�.
u32 LCD_Pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}	
//��ʾ����,��λΪ0,������ʾ
//x,y:�������
//num:��ֵ(0~999999999);	 
//len:����(��Ҫ��ʾ��λ��)
//size:�����С
//mode:
//[7]:0,�����;1,���0.
//[6:1]:����
//[0]:0,�ǵ�����ʾ;1,������ʾ.
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

//��ʾ����,��λΪ0,����ʾ
//x,y :�������	 
//len :���ֵ�λ��
//size:�����С
//color:��ɫ 
//num:��ֵ(0~4294967295);	 
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
//��ָ��λ����ʾһ���ַ�
//x,y:��ʼ����
//num:Ҫ��ʾ���ַ�:" "--->"~"
//size:�����С 12/16
//mode:���ӷ�ʽ(1)���Ƿǵ��ӷ�ʽ(0)
void _7LCDShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode)
{  							  
    u8 temp,t1,t;
	u16 y0=y;
	u16 colortemp=POINT_COLOR;      			     
	//���ô���		   
	num=num-' ';//�õ�ƫ�ƺ��ֵ
	if(!mode) //�ǵ��ӷ�ʽ
	{
	    for(t=0;t<size;t++)
	    {   
			if(size==12)temp=asc2_1206[num][t];  //����1206����
			else temp=asc2_1608[num][t];		 //����1608���� 	                          
	        for(t1=0;t1<8;t1++)
			{			    
		        if(temp&0x80)POINT_COLOR=colortemp;
				else POINT_COLOR=BACK_COLOR;
				_7LCD.DrawPoint(x,y);	
				temp<<=1;
				y++;
				if(x>=lcddev.width){POINT_COLOR=colortemp;return;}//��������
				if((y-y0)==size)
				{
					y=y0;
					x++;
					if(x>=lcddev.width){POINT_COLOR=colortemp;return;}//��������
					break;
				}
			}  	 
	    }    
	}else//���ӷ�ʽ
	{
	    for(t=0;t<size;t++)
	    {   
			if(size==12)temp=asc2_1206[num][t];  //����1206����
			else temp=asc2_1608[num][t];		 //����1608���� 	                          
	        for(t1=0;t1<8;t1++)
			{			    
		        if(temp&0x80)_7LCD.DrawPoint(x,y); 
				temp<<=1;
				y++;
				if(x>=lcddev.height){POINT_COLOR=colortemp;return;}//��������
				if((y-y0)==size)
				{
					y=y0;
					x++;
					if(x>=lcddev.width){POINT_COLOR=colortemp;return;}//��������
					break;
				}
			}  	 
	    }     
	}
	POINT_COLOR=colortemp;	    	   	 	  
}  
//��ָ����������䵥����ɫ
//(sx,sy),(ex,ey):�����ζԽ�����,�����СΪ:(ex-sx+1)*(ey-sy+1)   
//color:Ҫ������ɫ
void _7LCDFill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color)
{          
	u16 i,j;
	u16 xlen=0;
	xlen=ex-sx+1;	   
	for(i=sy;i<=ey;i++)
	{
	 	LCD_SetCursor(sx,i);      				//���ù��λ�� 
		_7LCD_WriteRAM_Prepare();     			//��ʼд��GRAM	  
		for(j=0;j<xlen;j++)LCD->LCD_RAM=color;	//����LCD	    
	}
}  
//������	  
//(x1,y1),(x2,y2):���εĶԽ�����
void _7LCDDrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
	_7LCD.DrawLine(x1,y1,x2,y1);
	_7LCD.DrawLine(x1,y1,x1,y2);
	_7LCD.DrawLine(x1,y2,x2,y2);
	_7LCD.DrawLine(x2,y1,x2,y2);
}
//��ָ��λ�û�һ��ָ����С��Բ
//(x,y):���ĵ�
//r    :�뾶
void _7LCDDrawCircle(u16 x0,u16 y0,u8 r)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //�ж��¸���λ�õı�־
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
		//ʹ��Bresenham�㷨��Բ     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 						    
	}
} 									  

//����
//x1,y1:�������
//x2,y2:�յ�����  
void _7LCDDrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1; //������������ 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //���õ������� 
	else if(delta_x==0)incx=0;//��ֱ�� 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//ˮƽ�� 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //ѡȡ�������������� 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//������� 
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
//��ȡ��ĳ�����ɫֵ	 
//x,y:����
//����ֵ:�˵����ɫ
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
//����
//x,y:����
//POINT_COLOR:�˵����ɫ
void _7LCDDrawPoint(u16 x,u16 y)
{
	LCD->LCD_REG=lcddev.setycmd;
	LCD->LCD_RAM=y; 
 	LCD->LCD_REG=lcddev.setxcmd; 
	LCD->LCD_RAM=x; 
	LCD->LCD_REG=lcddev.wramcmd; 
	LCD->LCD_RAM=POINT_COLOR; 
}
//��������
//color:Ҫ���������ɫ
static void _7LCDClear(u16 color)
{
	u32 index=0;      
	u32 totalpoint=lcddev.width;
	totalpoint*=lcddev.height; 	//�õ��ܵ���
	LCD_SetCursor(0x00,0x0000);	//���ù��λ�� 
	_7LCD_WriteRAM_Prepare();     //��ʼд��GRAM	 	  
	for(index=0;index<totalpoint;index++)
	{
		LCD->LCD_RAM=color;	   
	}
} 
static void _7LCDInit()
{
	u16 i=0;									  
	RCC->AHB1ENR|=0XF<<3;    	//ʹ��PD,PE,PF,PGʱ��  
	RCC->AHB1ENR|=1<<1;     	//ʹ��PBʱ��  
	RCC->AHB3ENR|=1<<0;     	//ʹ��FSMCʱ��  
	GPIO_Set(GPIOB,PIN15,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_50M,GPIO_PUPD_PU);							//PB15 �������,���Ʊ���
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
	 
	//�Ĵ�������
	//bank1��NE1~4,ÿһ����һ��BCR+TCR�������ܹ��˸��Ĵ�����
	//��������ʹ��NE1 ��Ҳ�Ͷ�ӦBTCR[0],[1]��				    
	FSMC_Bank1->BTCR[6]=0X00000000;
	FSMC_Bank1->BTCR[7]=0X00000000;
	FSMC_Bank1E->BWTR[6]=0X00000000;
	//����BCR�Ĵ���	ʹ���첽ģʽ
	FSMC_Bank1->BTCR[6]|=1<<12;		//�洢��дʹ��
	FSMC_Bank1->BTCR[6]|=1<<14;		//��дʹ�ò�ͬ��ʱ��
	FSMC_Bank1->BTCR[6]|=1<<4; 		//�洢�����ݿ��Ϊ16bit 	    
	//����BTR�Ĵ���	
	//��ʱ����ƼĴ��� 							    
	FSMC_Bank1->BTCR[7]|=0<<28;		//ģʽA 	 						  	 
	FSMC_Bank1->BTCR[7]|=6<<0; 		//��ַ����ʱ��(ADDSET)Ϊ6��HCLK 1/168M=6ns*6=36ns	 
	FSMC_Bank1->BTCR[7]|=5<<8;  	//���ݱ���ʱ��(DATAST)Ϊ6��HCLK	=6*6=36ns
	//дʱ����ƼĴ���  
	FSMC_Bank1E->BWTR[6]|=0<<28; 	//ģʽA 	 							    
	FSMC_Bank1E->BWTR[6]|=6<<0;		//��ַ����ʱ��(ADDSET)Ϊ6��HCLK=36ns  
	FSMC_Bank1E->BWTR[6]|=5<<8; 	//���ݱ���ʱ��(DATAST)Ϊ6ns*6��HCLK=36ns
	//ʹ��BANK1,����4
	FSMC_Bank1->BTCR[6]|=1<<0;		//ʹ��BANK1������1	     
  	//LCD_RST=0;	//̽����STM32F407�������RESET��LCD��RST���ã���δ����ͨIO���Ƹ�λ
	delay_ms(100);
	//LCD_RST=1;	   
	while(i<0X1FFF) 
	{
		if(RecvData()&0x0001)break;//�ȴ�������׼����
		i++;
	} 
  lcddev.setxcmd=LCD_CUR_X;	//����дX����ָ��
 	lcddev.setycmd=LCD_CUR_Y;	//����дY����ָ��
 	lcddev.wramcmd=LCD_PIXELS;	//����д��GRAM��ָ��
 	lcddev.width=800;			//���ÿ��
 	lcddev.height=480;			//���ø߶�
	_7LCDScanDir(L2R_U2D);		//����Ĭ��ɨ�跽��.	 
	_7LCDSetDisplayLayer(0);	 	//��ʾ��Ϊ0
	_7LCDSetOperateLayer(0);		//������ҲΪ0
	_7LCDEndXSet(799);			//x�յ�����Ϊ800
	_7LCDBackLightSet(63);		//��������Ϊ����
	_7LCDClear(Color.Black);			//����
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


