#ifndef __7LCD_H
#define __7LCD_H
#include "sys.h"


//-----------------LCD�˿ڶ���---------------- 
//#define	LCD_LED PBout(0) 	//LCD���ⲻ��IO�ڿ���,���������Լ�����,ͨ��ָ������   
//#define	LCD_RST PBout(15) 	//LCD��λ��,�Ϳ����干��,��������ΪPB0,��ʵ��û���õ�    
//LCD��ַ�ṹ��
typedef struct
{
	u16 LCD_REG;
	u16 LCD_RAM;
} LCD_TypeDef;
//ʹ��NOR/SRAM�� Bank1.sector4,��ַλHADDR[27,26]=11 A6��Ϊ�������������� 
//ע������ʱSTM32�ڲ�������һλ����! 111 1110=0X7E			    
#define LCD_BASE        ((u32)(0x6C000000 | 0x0000007E))
#define LCD             ((LCD_TypeDef *) LCD_BASE)

//ɨ�跽����
#define L2R_U2D  0 //������,���ϵ���
#define L2R_D2U  1 //������,���µ���
#define R2L_U2D  2 //���ҵ���,���ϵ���
#define R2L_D2U  3 //���ҵ���,���µ���

#define U2D_L2R  4 //���ϵ���,������
#define U2D_R2L  5 //���ϵ���,���ҵ���
#define D2U_L2R  6 //���µ���,������
#define D2U_R2L  7 //���µ���,���ҵ���	 

#define DFT_SCAN_DIR  L2R_U2D  //Ĭ�ϵ�ɨ�跽��
//////////////////////////////////////////////////////////////////////////////////
//LCD ָ���
#define LCD_CUR_Y  		0X00
#define LCD_CUR_X  		0X01
#define LCD_PIXELS 		0X02
#define LCD_END_X 		0X03
#define LCD_PREF 		0X05
#define LCD_MIRROR 		0X07

//LCD��Ҫ������
typedef struct  
{										    
	u16 width;			//LCD ���
	u16 height;			//LCD �߶�
	u16 id;				//LCD ID��ATK-7' TFTLCD��֧��id����
	u8  dir;			//���������������ƣ�0��������1��������	
	u16 sysreg;			//PREF�Ĵ��������õ�ǰ����ҳ����ʾҳ�ͱ���ȣ�ATK-7' TFTLCD�ݲ�֧�ֺ��������ã�ͳһΪ��������	
	u16	wramcmd;		//��ʼдgramָ��
	u16  setxcmd;		//����x����ָ��
	u16  setycmd;		//����y����ָ��  
}_7LCDDev; 	  
//LCD����
extern _7LCDDev lcddev;	//����LCD��Ҫ����



typedef struct//��ɫ����
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
