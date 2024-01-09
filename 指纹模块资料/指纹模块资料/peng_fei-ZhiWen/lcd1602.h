#ifndef __LCD1602__HH__
#define __LCD1602__HH__

//向LCD写入命令或数据************************************************************
#define LCD_COMMAND			0      // Command
#define LCD_DATA			1      // Data
#define LCD_CLEAR_SCREEN	0x01      // 清屏
#define LCD_HOMING  		0x02      // 光标返回原点

//设置显示模式************************************************************
#define LCD_SHOW			0x04    //显示开
#define LCD_HIDE			0x00    //显示关	  
#define LCD_CURSOR			0x02 	//显示光标
#define LCD_NO_CURSOR		        0x00    //无光标		     
#define LCD_FLASH			0x01    //光标闪动
#define LCD_NO_FLASH		        0x00    //光标不闪动

//设置输入模式************************************************************
#define LCD_AC_UP			0x02
#define LCD_AC_DOWN			0x00      // default
#define LCD_MOVE			0x01      // 画面可平移
#define LCD_NO_MOVE			0x00      //default


//1602液晶端口定义 ******************************************************
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

void Print(unsigned char *str);	//将字符输出到液晶显示


#endif
