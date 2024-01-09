#include "flash.h"
#include <stdio.h>
#include <string.h>
/*FLASH写入程序*/
void WriteFlash(uint32_t addr,uint32_t *Data,uint32_t L)
{
	uint32_t i=0;
	/* 1/4解锁FLASH*/
	HAL_FLASH_Unlock();
	/* 2/4擦除FLASH*/
	/*初始化FLASH_EraseInitTypeDef*/
	/*擦除方式页擦除FLASH_TYPEERASE_PAGES，块擦除FLASH_TYPEERASE_MASSERASE*/
	/*擦除页数*/
	/*擦除地址*/
	FLASH_EraseInitTypeDef FlashSet;
	FlashSet.TypeErase = FLASH_TYPEERASE_PAGES;
	FlashSet.PageAddress = addr;
	FlashSet.NbPages = 1;
	/*设置PageError，调用擦除函数*/
	uint32_t PageError = 0;
	HAL_FLASHEx_Erase(&FlashSet, &PageError);
	/* 3/4对FLASH烧写*/
	for(i=0;i<L;i++)
	{
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, addr+4*i, Data[i]);
	}
	/* 4/4锁住FLASH*/
	HAL_FLASH_Lock();
}

void Flash_Read(uint32_t address, uint32_t *data, uint16_t length)
{
    uint16_t i;
    for (i = 0; i < length; i++) {
        data[i] = *(__IO uint32_t *)(address + (i * 4)); // 以字为单位读取Flash
    }
}
//
DataToSave data_to_save ;//这个是要保存的数据
void my_flash_write(void)
{
	// 将数据转换为uint32_t数组以便写入Flash
	uint32_t flash_data[sizeof(DataToSave) / sizeof(uint32_t)];
	memcpy(flash_data, &data_to_save, sizeof(DataToSave));

	// 将数据写入Flash
	WriteFlash(FLASH_SAVE_ADDR, flash_data, sizeof(DataToSave) / sizeof(uint32_t));
}
int fff;//这是用来测试的
void my_flash_read(void)
{
	// 将读取到的数据转换回原始结构体
	DataToSave data_read;
	// 从Flash中读取数据
	uint32_t flash_data_read[sizeof(DataToSave) / sizeof(uint32_t)];
	Flash_Read(FLASH_SAVE_ADDR, flash_data_read, sizeof(DataToSave) / sizeof(uint32_t));
	memcpy(&data_read, flash_data_read, sizeof(DataToSave));
	// 现在，data_read.float_data 和 data_read.int_data 中分别包含从Flash中读取的float和int数据
	//读取完以后的数据可以从这里拿出！！！！
	fff=data_read.int_data[0];
}
