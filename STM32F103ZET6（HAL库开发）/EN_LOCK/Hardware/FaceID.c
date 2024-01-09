#include "main.h"
#include "FaceID.h"
#include <stdio.h>
#include <string.h>


uint8_t FACE_REGISTRATION[]={0XEF,0XAA,0X13,0X00,0X00,0X00,0X00,0X13}; //����ע��                            
uint8_t ON_BACKLIGHT[]={0XEF,0XAA,0XC1,0X00,0x00,0x00,0x01,0X01,0XC3}; //����ʾ
uint8_t OFF_BACKLIGHT[]={0XEF,0XAA,0XC1,0X00,0x00,0x00,0x01,0X00,0XC2};//�ر���ʾ
uint8_t RECOGNITION[]={0xEF,0xAA,0x12,0x00,0x00,0x00,0x00,0x12};       //ʶ����֤
uint8_t JUDGE[10];                                                     //�ж��Ƿ�ɹ�
uint8_t RX_data[100];

void face_init(void)
{
	__HAL_UART_FLUSH_DRREGISTER(&huart1);
	memset(RX_data, 0, sizeof(RX_data)); //��ʼ������Ϊ0
	HAL_UART_Transmit(&huart1,OFF_BACKLIGHT,sizeof(OFF_BACKLIGHT), 200); //�ر���ʾ
}

/*
@brief     	����ʶ��ע��
@param      void
@return     1 : �ɹ�  0 ��ʧ��  2���ظ�ע�������ԭ��
*/
int Face_registration(void)
{
	__HAL_UART_FLUSH_DRREGISTER(&huart1);
	HAL_Delay(1500);
	memset(RX_data, 0, sizeof(RX_data)); //��ʼ������Ϊ0
	HAL_UART_Transmit(&huart1,ON_BACKLIGHT,sizeof(ON_BACKLIGHT), 200); //����ʾ
	HAL_UART_Receive(&huart1,RX_data,10,2000);
	memset(RX_data, 0, sizeof(RX_data)); //��ʼ������Ϊ0
	HAL_Delay(10);
	HAL_UART_AbortReceive(&huart1);
	HAL_UART_Transmit(&huart1,FACE_REGISTRATION,sizeof(FACE_REGISTRATION), 100);//����ע��
	HAL_Delay(10);
	HAL_UART_Receive(&huart1,RX_data,10,2000);
	if(memcmp(RX_data,"\xEF\xAA\x00\x00\x00\x00\x04\x13\x00\x00", 10) == 0 ) //ע��ɹ�
		return 1;
	else if(memcmp(RX_data,"\xEF\xAA\x00\x00\x00\x00\x02\x13\x01\x16", 10) == 0 ) //ע��ʧ��
		return 0;
	else
		return 2;
}

/*
@brief     	������֤
@param      void
@return     1 : �ɹ�  0 ��ʧ�� ��������ԭ��
*/

int Face_verification(void)
{
	__HAL_UART_FLUSH_DRREGISTER(&huart1);
	HAL_Delay(1500);
	memset(RX_data, 0, 100); //��ʼ������Ϊ0
	HAL_UART_Transmit(&huart1,ON_BACKLIGHT,sizeof(ON_BACKLIGHT), 200); //����ʾ
	HAL_UART_Receive(&huart1,RX_data,10,2000);
	memset(RX_data, 0, sizeof(RX_data)); //��ʼ������Ϊ0
	HAL_Delay(10);
	HAL_UART_AbortReceive(&huart1);
	HAL_UART_Transmit(&huart1,RECOGNITION,sizeof(RECOGNITION),200);//����ʶ��
	HAL_Delay(10);
	HAL_UART_Receive(&huart1,RX_data,9,2000);
	if(memcmp(RX_data,"\xEF\xAA\x00\x00\x00\x00\x04\x12\x00",9) == 0 ) //ʶ��ɹ�
	{
		memset(RX_data, 0, 100); //��ʼ������Ϊ0
		return 1;
	}
	else 
		return 0;
}
