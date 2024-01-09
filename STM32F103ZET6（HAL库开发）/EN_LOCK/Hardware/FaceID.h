#ifndef _FACEID_H
#define _FACEID_H

extern UART_HandleTypeDef huart1;

void face_init(void);
int Face_registration(void);
int Face_verification(void);


#endif

