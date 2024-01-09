#ifndef _SERVO_H
#define _SERVO_H

#include "main.h"                  // Device header

#define Door_On  Door_ON()

extern TIM_HandleTypeDef htim1;

void Servo_Angle(int angle);
void Door_ON(void);


#endif

