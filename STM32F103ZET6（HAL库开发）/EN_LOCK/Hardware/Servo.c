#include "main.h"                  // Device header

#include "Servo.h"
#include  "faceID.h"

/**
 * @brief   舵机选选择任意角度
 * @param   angele ：选择的角度
 * @return  无
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
 * @brief   开门
 * @param   无
 * @return  无
 */

void Door_ON(void)
{
		Servo_Angle(180);
		HAL_Delay(600);
		Servo_Angle(0);
		HAL_Delay(600);
		face_init();
}
