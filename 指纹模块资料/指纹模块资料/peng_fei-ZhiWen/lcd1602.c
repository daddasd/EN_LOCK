#include <reg52.h>
#include <intrins.h>

#include"lcd1602.h"


/***********1602Һ����ʾ�����ӳ���****************/
void lcddelay(unsigned int z)
{
	unsigned int x,y;
	for(x=z;x>0;x--)
		for(y=110;y>0;y--);
}

 void  LCD_Write(bit style, unsigned char input)
{
	LcdRs=style;
	P0=input; 
	lcddelay(5);
	LcdEn=1;
	lcddelay(5);	
	LcdEn=0;	
}


void LCD_SetDisplay(unsigned char DisplayMode)
{
	LCD_Write(LCD_COMMAND, 0x08|DisplayMode);	
}



void LCD_SetInput(unsigned char InputMode)
{
	LCD_Write(LCD_COMMAND, 0x04|InputMode);
}

//��ʼ��LCD************************************************************
void LCD_Initial()
{
	LcdEn=0;
	LCD_Write(LCD_COMMAND,0x38);           //8λ���ݶ˿�,2����ʾ,5*7����
	LCD_Write(LCD_COMMAND,0x38);
	LCD_SetDisplay(LCD_SHOW|LCD_NO_CURSOR);    //������ʾ, �޹��
	LCD_Write(LCD_COMMAND,LCD_CLEAR_SCREEN);   //����
	LCD_SetInput(LCD_AC_UP|LCD_NO_MOVE);       //AC����, ���治��
}

//Һ���ַ������λ��************************
void GotoXY(unsigned char x, unsigned char y)
{
	if(y==0)
		LCD_Write(LCD_COMMAND,0x80|x);
	if(y==1)
		LCD_Write(LCD_COMMAND,0x80|(x-0x40));
}

//���ַ������Һ����ʾ
void Print(unsigned char *str)
{
	while(*str!='\0')
	{
		LCD_Write(LCD_DATA,*str);
		str++;
	}
}


