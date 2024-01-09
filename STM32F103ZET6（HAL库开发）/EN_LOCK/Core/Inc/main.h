/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define screen_TX_Pin GPIO_PIN_2
#define screen_TX_GPIO_Port GPIOA
#define screen_RX_Pin GPIO_PIN_3
#define screen_RX_GPIO_Port GPIOA
#define Servo_Pin GPIO_PIN_9
#define Servo_GPIO_Port GPIOE
#define AS608_TX_Pin GPIO_PIN_10
#define AS608_TX_GPIO_Port GPIOB
#define AS608_RX_Pin GPIO_PIN_11
#define AS608_RX_GPIO_Port GPIOB
#define Finger_WAK_Pin GPIO_PIN_12
#define Finger_WAK_GPIO_Port GPIOB
#define FACE_TX_Pin GPIO_PIN_9
#define FACE_TX_GPIO_Port GPIOA
#define FACE_RX_Pin GPIO_PIN_10
#define FACE_RX_GPIO_Port GPIOA
#define RC522_RST_Pin GPIO_PIN_15
#define RC522_RST_GPIO_Port GPIOA
#define RC522_CS_Pin GPIO_PIN_8
#define RC522_CS_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
//#define RC522_RST_Pin GPIO_PIN_11
//#define RC522_RST_GPIO_Port GPIOA
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
