#include "main.h"
#include "Serial_screen.h"
#include <stdio.h>
#include <string.h>
#include "FaceID.h"
#include "Servo.h"

uint8_t RX_data1[100];


/*
@brief     	�Ӵ��������յ�ָ�� ���ݲ�ͬ�ķ���ֵȷ��ִ�еĹ��� 
@param      void
@return    
						 5 : ָ�ƽ���
						 3 ���沿���� 
						 4 : �沿ע��
						 2 ��ָ��ע�� 
						 1 : �����ɹ�
						 8 : ����ʧ��
*/
int Serial_Screen(void)
{
    static int flag = -1;
		int count=0;
		memset(RX_data1, 0, sizeof(RX_data1)); //��ʼ������Ϊ0
		HAL_UART_AbortReceive(&huart2);
	
    HAL_UART_Receive(&huart2, RX_data1, 1, 200);

    if (memcmp(RX_data1, "\x22", 1) ==0)  //�������
    {
			  flag =1;
				memset(RX_data1, 0, sizeof(RX_data1)); //��ʼ������Ϊ0
				while(flag)
				{
					  HAL_UART_Receive(&huart2, RX_data1, 1, 200);
					if(memcmp(RX_data1, "\x33", 1)==0)
					{
							memset(RX_data1, 0, sizeof(RX_data1)); //��ʼ������Ϊ0
						  count=0;
							flag=-1;
							return 1;
					}
					count++;
					HAL_Delay(100);
					if(count==100)
					{
						memset(RX_data1, 0, sizeof(RX_data1)); //��ʼ������Ϊ0
						 flag=-1;
						 count=0;
					}
				}
    }
		else if(memcmp(RX_data1, "\x11", 1)==0) //ָ��ע��
		{
				memset(RX_data1, 0, sizeof(RX_data1)); //��ʼ������Ϊ0
				return 2;
		}
		else if(memcmp(RX_data1, "\x44", 1)==0) //����ע��
		{
				memset(RX_data1, 0, sizeof(RX_data1)); //��ʼ������Ϊ0
				return 4;
		}
		else if(memcmp(RX_data1, "\x55", 1)==0) //����ʶ��
		{
				memset(RX_data1, 0, sizeof(RX_data1)); //��ʼ������Ϊ0
				return 3;
		}
		else if(memcmp(RX_data1, "\x66", 1)==0) //ָ�ƽ���
		{
				memset(RX_data1, 0, sizeof(RX_data1)); //��ʼ������Ϊ0
				return 5;
		}
		return 0;
}




