#ifndef __LCD1602__HH__
#define __LCD1602__HH__

//��LCDд�����������************************************************************
#define LCD_COMMAND			0      // Command
#define LCD_DATA			1      // Data
#define LCD_CLEAR_SCREEN	0x01      // ����
#define LCD_HOMING  		0x02      // ��귵��ԭ��

//������ʾģʽ************************************************************
#define LCD_SHOW			0x04    //��ʾ��
#define LCD_HIDE			0x00    //��ʾ��	  
#define LCD_CURSOR			0x02 	//��ʾ���
#define LCD_NO_CURSOR		        0x00    //�޹��		     
#define LCD_FLASH			0x01    //�������
#define LCD_NO_FLASH		        0x00    //��겻����

//��������ģʽ************************************************************
#define LCD_AC_UP			0x02
#define LCD_AC_DOWN			0x00      // default
#define LCD_MOVE			0x01      // �����ƽ��
#define LCD_NO_MOVE			0x00      //default


//1602Һ���˿ڶ��� ******************************************************
sbit LcdRs= P2^7;												  
sbit LcdRw= P2^6;
sbit LcdEn= P2^5;


sbit  ACC0 = ACC^0;
sbit  ACC7 = ACC^7;


unsigned char LCD_Wait(void);

void LCD_Write(bit style, unsigned char input);

void lcddelay(unsigned int z);

void LCD_Write(bit style, unsigned char input);

void LCD_SetDisplay(unsigned char DisplayMode);

void LCD_SetInput(unsigned char InputMode);

void LCD_Initial(void);

void GotoXY(unsigned char x, unsigned char y);

void Print(unsigned char *str);	//���ַ������Һ����ʾ


#endif
