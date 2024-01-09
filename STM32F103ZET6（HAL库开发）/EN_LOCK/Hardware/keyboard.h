#ifndef _KEYBOARD_H
#define _KEYBOARD_H



void InitKeypad(void);
void SendKeyOverUART(uint8_t key);
uint8_t ReadKeypad(void);


#endif

