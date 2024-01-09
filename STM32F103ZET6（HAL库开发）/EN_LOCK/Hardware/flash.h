#ifndef __FLASH_H
#define __FLASH_H
#include "main.h"
void WriteFlash(uint32_t addr,uint32_t *Data,uint32_t L);
void Flash_Read(uint32_t address, uint32_t *data, uint16_t length);
#define FLASH_SAVE_ADDR  (0x08060000) // 这地方不用改直接用这个地址。
typedef struct {
    float float_data[4];
    int int_data[4];
} DataToSave;								//这个是要保存数据的结构体
extern DataToSave data_to_save ;//这个是要保存的数据
void my_flash_write(void);
void my_flash_read(void);
#endif
