#include "main.h"                  // Device header

#include "Servo.h"
#include  "faceID.h"

/**
 * @brief   ���ѡѡ������Ƕ�
 * @param   angele ��ѡ��ĽǶ�
 * @return  ��
 */
void Servo_Angle(int angle)
{
		switch(angle)
	{
		case 0:
			__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1, 500);
		case 45:
			__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1, 1000);
		case 90:
			__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1, 1500);
		case 135:
			__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1, 2000);
		case 180:
			__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1, 2500);
		break;
	}
	uint32_t i=0;
	i=angle*11+500;
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,i);
}


/**
 * @brief   ����
 * @param   ��
 * @return  ��
 */

void Door_ON(void)
{
		Servo_Angle(180);
		HAL_Delay(600);
		Servo_Angle(0);
		HAL_Delay(600);
		face_init();
}
