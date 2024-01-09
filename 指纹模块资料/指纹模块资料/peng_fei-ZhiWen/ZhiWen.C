#include <reg52.h>
#include <intrins.h>
#include"lcd1602.h"


sbit buzzer=P2^0;  //蜂鸣器	低电平打开蜂鸣器

sbit red=   P1^0;  //红色灯 表示 录入 模式 
sbit green= P1^1;  //绿色灯 表示 识别 模式

sbit k1=P3^2; //清除所有指纹（10个指纹清除）
sbit k2=P3^3; //模式识别转换
sbit k3=P3^4; //录入一次指纹

#define Max_User 10
#define error 2
   
unsigned int 	SaveNumber=0,searchnum=0;
unsigned int  	SearchNumber=0;		
unsigned int 	clk0=0;

unsigned char str[6]={0,0,0,0,0,'\0'};

unsigned char code Identify[16]="Lock:   Identify";
unsigned char code Input[16]   ="Lock:   Input   ";

bit modeflag=0,clearallflag=0,changeflag=0;

// modeflag=0 为识别模式 modeflag=1为录入指纹模式
//每成功录入一次指纹 SaveNumber++

//////////////////////////////////////常用指令定义/////////////////////////////

//Verify  Password   ：验证设备握手口令
unsigned char code VPWD[16]={16,0X01 ,0Xff,0xff,0xff,0xff, 0x01,0,7,0x13,0x00,0x00,0x00,0x00,0x00,0x1b};	 //回送12个

//设置设备握手口令
unsigned char code STWD[16]={16,0X01 ,0Xff,0xff,0xff,0xff, 0x01,0,7,0x12,0x00,0x00,0x00,0x00,0x00,0x1a};	 //回送12个

//GetImage           ：探测手指并从传感器上读入图像
unsigned char code GIMG[14]={12, 0X01 ,0Xff,0xff,0xff,0xff, 0x01, 0,3,1,0x00,0x05};	//回送12个

//Gen Templet1        ：根据原始图像生成指纹特征1
unsigned char code GENT1[14]={13,0X01 ,0Xff,0xff,0xff,0xff,0x01,0,4,2,1,0x00,0x08};	//回送12个

//Gen Templet2        ：根据原始图像生成指纹特征2
unsigned char code GENT2[14]={13,0X01 ,0Xff,0xff,0xff,0xff,0x01,0,4,2,2,0x00,0x09}; //回送12个	

	
//Search Finger      ：以CharBufferA或CharBufferB中的特征文件搜索整个或部分指纹库
unsigned char code SEAT[18]={17,  0X01 ,0Xff,0xff,0xff,0xff, 0x01,   0,8, 4,1,0,0,    0,0x65,  0x00,0x73};	//回送16个


//Merge Templet      ;将CharBufferA与CharBufferB中的特征文件合并生成模板，结果存于ModelBuffer。
unsigned char code MERG[14]={12,  0X01 ,0Xff,0xff,0xff,0xff, 0x01,  0,3,5 , 0x00,0x09};//回送12个	

//Store Templet      ：将ModelBuffer中的文件储存到flash指纹库中
unsigned char code STOR[16]={15,  0X01 ,0Xff,0xff,0xff,0xff, 0x01,  0,6,6,2,     0x00,0x00,     0x00,0x0f}; //回送12个

//Read Note
unsigned char code RDNT[14]={13,0X01 ,0Xff,0xff,0xff,0xff, 0x01, 0,4,0x19,  0, 0x00,0x1e};

//Clear Note
unsigned char code DENT[46]={45,0X01 ,0Xff,0xff,0xff,0xff, 0x01, 0,36,0x18,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0x00,0x3d};

//DEL one templet
unsigned char code DELE_one[16]={16, 0X01 ,0Xff,0xff,0xff,0xff, 0x01,   0,7,  0x0c,    0x00,0x00, 0,1, 0x00,0x15};

//DEL templet      ;清空指纹库
unsigned char code DELE_all[12]={12,0X01 ,0Xff,0xff,0xff,0xff, 0x01, 0,3,  0x0d,0x00,0x11};

//////////////////////////////////////常用指令定义-------结束///////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////宏定义////////////////////////////////////////////////////////////////////////
#define FALSE  0
#define TURE   1
#define ERROR  1001

//状态定义表
#define on    	1
#define off   	0

#define MAX_NUMBER    63 
#define _Nop()  _nop_()


//////////////////////////////////////宏定义------------结束////////////////////////////////////////////////////////

unsigned char 		  FifoNumber=0; 
xdata unsigned char   FIFO[MAX_NUMBER+1]={0};

/*********1毫秒延时程序**********/
void delay1ms(unsigned int t)
{
	unsigned int i,j;
	for(i=0;i<t;i++)
	   for(j=0;j<120;j++)
	   ;
}


void TxdByte(unsigned char dat)//串口发送信息,通过查询方式发送一个字符
{
    TI = 0;		 //让TI＝0
    SBUF = dat;	 //读入数据
    while(!TI);	 //等待发送完毕
    TI = 0;		 //清零
}

bit Command(unsigned char *p,unsigned char MaxTime) //命令解析,给模块发送一个命令
{
  	unsigned char count=0,tmpdat=0,temp=0,i=0,package=0,flag=0,checksum=0;
  		
	bit result=0, start=0,stop=0;

  	TxdByte(0xef);//数据包包头识别码
   	TxdByte(0x01);//数据包包头识别码
  	i=*p;         //数组的第“0”个元素、里面存放了本数组的长度，把这个长度给变量i，方便进行操作
  	p++; 
	p++;
  	for (count=i-1; count!=1;count--)  //Sent command String
	  	{
	 		temp=*p++;//取第个“1”个元素的内容，然后发送 
	    	TxdByte(temp);//将数据发送出去
		}  
  	result=TURE;//发送完成,结果为真 (真为1)   	
  	FifoNumber=0;
  	for (count=MAX_NUMBER+1; count!=0; count--)//清空所有FIFO[]数组里面的内容，写入0X00
		 FIFO[count-1]=0x00;  
	
  	if (result)   
   	{		
     	result=FALSE;
      	start =FALSE;
   		stop  =FALSE;
       	count=0;
       	clk0=0;	//清零CL0计数
		
       	do /////////////////////////////do的内容////////////////////////////////
		{	
			restart0:				
	         	if (RI==1)//如果接收到数据
		      	{ 				
			  		tmpdat=SBUF;//先把接收到的数据放到tmpdat中
	            	RI=0;
	            	if ((tmpdat==0xef)&&(start==FALSE))//这个数据为第一个传回来的数据，也就是“指令应答”的第一个字节
		            	{ 
							count=0;
					    	FIFO[0]=tmpdat;//读入第一个应答字节(0XEF)，存在第“0”个元素中    
							flag=1;	
							goto 
								restart0;//可以用中断方式进行			
				        	
		             	}
					if(flag==1)//第一个字节已经回来，所以flag==1成立
						{  
							if(tmpdat!=0x01)  //接收数据错误，将重新从缓冲区接收数据
							{  	
								flag=0;//接收应答失败
								result=FALSE;
		      					start =FALSE;
		   						stop=FALSE;
		       					count=0;
								goto 
									restart0;					
							
							}
							//如果成功接收到0xef01，可以开始接收数据
							flag=2;//flag=2;表示应答成功，可以开始接收数据了
							count++;//现在count=1;
							FIFO[count]=tmpdat;//读入第二个应答字节（0X01），存在第“1”个元素中    
							start=TURE;	//应答成功可以开始接收数据
							    goto 
									restart0;	
						}                  
	             	if((flag==2)&&(start==TURE))//flag=2;表示应答成功，可以开始接收数据了
		             	{	   	  					 
					   		count++; //数据元素下标++
				            FIFO[count]=tmpdat;//存入数据
							if(count>=6)
								{
									checksum=FIFO[count]+checksum; //计算校验和
								}
							if(count==8)
								{ 
									package=FIFO[7]*0X100+FIFO[8];	//计算包长度							
									stop= TURE;
								}
							if(stop)
								{						
									if(count==package+8)
									{
										checksum=checksum-FIFO[count-1];
										if(checksum!=FIFO[count]&0xff) 
											result=FALSE; //校验失败,置结果标志为0							
										else 
											result=TURE;
										flag=0;
										break;
									} 
								}
		             	}
	 	       	}
          }/////////////////////////////do的内容----------------结束////////////////////////////////
		   
		while ((clk0<=MaxTime)&&(count<=MAX_NUMBER)&&(changeflag==0)); //由定时器以及最大接收数据来控制，保证不会在此一直循环
		  
        FifoNumber=count;	//保存接收到的数据个数
	 }
   return (result);
}

bit VefPSW(void)//验证设备握手口令,成功返回1     
{
 	unsigned char  count=0;
	while (1)
   	{
     	if(Command(VPWD,20) && (FifoNumber==11) && (FIFO[9]==0x00))  
			return(1);
     	count++;
   	  	if (count>=2)//如果不成功，再验证一次，如果两次不成功，返回失败
	        {  
	            return(0);   
	 	     }
	}
}


void Clear_All(void) //清空所有指纹信息，清空前首先验证和指纹模块通讯是否正常  
{
	if(VefPSW()!=1)//验证失败
	{			
         	
	}
	else		  //验证成功
	{	                 
   		Command(DELE_all,50); //清空指纹库
   		Command(DENT,20); 	  //清空用户记事本
	}
}

unsigned char ImgProcess(unsigned char BUFID)  //发获取图像并生成特征文件，存入BUFID中//输入参数为缓冲区号  
{	
    if(Command(GIMG,89) && (FifoNumber==11) && (FIFO[9]==0x00))  
    {
		if(BUFID==1)      
		{	   			
      		if(Command(GENT1,60) && (FifoNumber==11) && (FIFO[9]==0x00))  
        	{							
				return 1;
			}  
        	else
        	{
  				return 0;
         	}
		 }
		else if(BUFID==2)
		{
		  	if(Command(GENT2,60) && (FifoNumber==11) && (FIFO[9]==0x00))  
        	{				
				return 1;
			}  
        	else
        	{
  				return 0;
         	}			
		}
    }
    else
    {
      return 0;       
    }
	return 0;             
}

bit Searchfinger(void)//搜索指纹(发送搜索命令、以及根据返回值确定是否存在)        
{		
   	if(Command(SEAT,60) && (FifoNumber==15) && (FIFO[9]==0x00) )  
    {
		SearchNumber=FIFO[10]*0x100+FIFO[11];//搜索到的页码
		//MatchScore=FIFO[12]*0x100+FIFO[13]   可以在此计算得分，从而进行安全级别设定，本程序忽略
	   	return 1;
	}     
    else             
    {
       	return 0;
    }  
}


unsigned int search(void)//搜索用户 
{
 	unsigned int SearchBuf=0,i=0;
  	while (i<20)
    {
	   if(modeflag==0)//在识别模式下
	   {
	   	 if (ImgProcess(1)==1)//首先读入一次指纹  
       	 {
       		SearchBuf=Searchfinger();//进行指纹比对，如果搜索到，返回搜索到的指纹序号
       		if(SearchBuf==1)         //成功搜索到指纹号
       		{
       			return SearchNumber; 
       		}
       		else  //如果没有搜索到指纹号
			{
			   return 1001; //由于只能录入1000个指纹，这时候如果返回255表示识别错误，或者这个指纹没有登记
			}     
       	}
	   }
		else
		{
		   return 0;
		}
		i++;	
    }
   return 0;
}

bit savefingure(unsigned char ID)//保存指纹
{
	 unsigned char i=0;
	 //现在开始进行存储指纹模板的操作
     for (i=0;i<16;i++)	//保存指纹信息
	 	{
			FIFO[i]=STOR[i];
		}  
		
	 FIFO[11]=ID&0xFF00; 
     FIFO[12]=ID&0xFF; //把指纹模板存放的PAGE_ID也就是FLASH的位置
     FIFO[14]=FIFO[14]+ID;	//校验和
     if (Command(FIFO,70)==1)//不成功返回0	//此处进行存放指纹模板的命
	 	{return(1);}
     else
	 	{return(0);} //不成功返回0
}



unsigned char enroll(void) //采集两次指纹，生成1个 指纹模板
{
 	unsigned char temp=0,count=0;
  	while(1)
  	{
  	 	temp=ImgProcess(1); //生成特征1    
   		if (temp==1)//生成特征文件成功             
      	{  		      
        	break;
       	}
     	else                   
       	{
        	if (temp==0)//采集指纹没有成功
          	{ 
				count++;
            	if (count>=40)//如果采集了40次，还不成功，直接采集失败，直接退出enroll函数－－－－返回0  
					return(0);
            }
        }
   	}
	//采集第一个特征成功 
   	count=0;  
  	buzzer=0;
	delay1ms(100);
	buzzer=1;
	delay1ms(2000);//延时2S开始采集下一个特征

	//开始采集第二个特征 
 	while(1)
  	{
   		temp=ImgProcess(2); //生成特征2    
   		if (temp==1)//生成特征文件2成功
      	{
        	if (  (Command(MERG,40)&& (FifoNumber==11) && (FIFO[9]==0x00))==0  ) //合并不成功返回0，成功返回1
			{
				return 0;
			}	
			else//特征文件合并生成模板，结果存于ModelBuffer
			{
				 buzzer=0;
	             delay1ms(100);
	             buzzer=1;
				 delay1ms(100);
				 buzzer=0;
	             delay1ms(100);	//响两声，表示生成一个模板成功
	             buzzer=1;
        	     return 1;
			}	
        }
      	else      
       	{	
        	if (temp==1)//采集指纹没有成功
        	{
				count++;
				
				if (count>=25) 
				
				return(0);
       		}
     	}
   	}
}


void shownum(unsigned int dat)
{
	unsigned int numdat=0;
	numdat=dat;
	GotoXY(0,1);//x,y
	Print("Finger:");
	GotoXY(0,8);//x,y
	if(numdat<1000)//指纹模块只能存1000个指纹
	{
	    str[0]=0x30+numdat/100;      //百 位转换成ASCII码
		str[1]=0x30+(numdat%100)/10; //十 位转换成ASCII码
		str[2]=0x30+numdat%10;		 //个 位转换成ASCII码
		str[3]=' ';
		str[4]=' ';	
		Print(str);
	}
	if(numdat==ERROR)//表示识别错误
	{
	   Print("Error");
	}
	
	
}

void changeState(void)
{

   GotoXY(0,0);//x,y
   if(modeflag==0)
   {
   	Print(Identify);//显示 识别模式  Indentity
	green=0;
	red=1;
   }
   else
   {
   	Print(Input);   //显示  输入模式 Input
	green=1;
	red=0;
   }
   changeflag=0;
   shownum(0);		
}




void main(void)//主函数
{
	unsigned char i=0;

	ET0=1;     //定时器0开中断
	TL0=0x97;  //17ms的初值
	TH0=0xBD;
   delay1ms(5);
   LcdRw=0;      //只对液晶进行写操作，不进行读操作
   delay1ms(5);
   LCD_Initial();//液晶初始化
   delay1ms(5); 
   GotoXY(0,0);//x,y
   Print(Identify);

    //串口初始化 
    SCON=0x50;   //UART方式1:8位UART;   REN=1:允许接收 
    PCON=0x00;   //SMOD=0:波特率不加倍 
    TMOD=0x21;   //T1方式2,用于UART波特率
    TH1=0xFD; 
    TL1=0xFD;   //UART波特率设置:9600
    TR1=1; 
    TR0=1;// 开定时器0
	
	IT0=0;//中断0低电平中断
	IT1=1;//中断1低电平中断
	EX0=1;//开中断0
	EX1=1;//开中断1
    EA=1;

	

	for(i=0;i<6;i++)//开始握手6次，如果没有一次成功，表示模块通信不正常。只要成功就跳出此循环
	{
		if(VefPSW())//与模块握手通过，绿灯亮起。进入识别模式
		  {
				green=0; //读一次
				buzzer=0;
				shownum(0);
		        delay1ms(300);
		        buzzer=1;
				break;	
		  }
	    else
		 {
			red=1; 
			green=1;
			break;

		 }
	}

    while(1)
	{
		
		if(k3==0)//录入一个指纹
		{
	  	delay1ms(10);	
	  	if(k3==0)//如果仍为低电平，表示按键有效
		  	{
		  	  while(k3==0);//等待松手
			  if(VefPSW()==1&&modeflag==1&&SaveNumber<1000)//与模块握手通过
				  {
				  	  if(enroll()==1)//采集两次，生成1个指纹模板成功
					  	{
						   if(savefingure(SaveNumber+1)==1)//保存也成功
						   	{
								SaveNumber++;//加一次
								shownum(SaveNumber);
							}
	
						}
				  }
				else
				{
						buzzer=0; 
						for(i=0;i<8;i++)
						{
						  delay1ms(100);
						  red=~red;
						}
						red=0;
						buzzer=1;		
				}  	
		  	}
					
		}

		if(modeflag==0)//为识别模式
		{
			  searchnum=search();
			  if(searchnum>=1&&searchnum<=1000)//最多只能存储1000个指纹
			  {
			  	    //显示搜索到的指纹号
					shownum(searchnum);
					
			       //蜂鸣器响一声
			       buzzer=0;
			       delay1ms(100);
			       buzzer=1;
			  }
			  if(searchnum==ERROR)//表示放上了没有登记的指纹或者识别错误
			  {
			      shownum(searchnum);
				   //蜂鸣器响三声
			       buzzer=0;delay1ms(100); buzzer=1;delay1ms(100);
			       buzzer=0;delay1ms(100); buzzer=1;delay1ms(100);
			       buzzer=0;delay1ms(100); buzzer=1;
			  }
		}

		if(clearallflag==1)
		{
			clearallflag=0;
			Clear_All();
		    red=0;	     //红色灯亮
		    green=1;
			//蜂鸣器长响一次，表示清除所有指纹结束
			modeflag=1;//进入录入指纹模式
			GotoXY(0,0);//x,y
			Print(Input);
			shownum(0);
			buzzer=0;
			delay1ms(800);
			buzzer=1;
			SaveNumber=0;
		}

		if(changeflag==1)
		{
		   changeState();
		   changeflag=0;
		}

	}////////////////////////////while(1)结束////////////////////////////////
	
}


void int0(void) interrupt 0//中断0，清除所有指纹
{
	if(k1==0) //清除所有指纹
	{
		delay1ms(10);
		if(k1==0)//如果仍为低电平，表示按键有效
		{
			while(k1==0);//等待松手	
			clearallflag=1;
		}	
	}	
} 

void  Timer0(void) interrupt 1//定时器0中断函数
{
 	TL0=0x97;
	TH0=0xBD;
	clk0++;   //延时17ms
}


void int1(void) interrupt 2//中断1，模式转换
{
	if(k2==0)//模式转换 其中用modeflag 来标志,默认从第1个指纹开始录入
	{
	  delay1ms(10);	
	  if(k2==0)//如果仍为低电平，表示按键有效
	  	{
			while(k2==0);//等待松手
			modeflag=~modeflag;//0表示录入指纹  1表示识别指纹
			changeflag=1;//表示模式有变化
		}	
	}	
}







