#include "main.h"
#include "FaceID.h"
#include "keyboard.h"

#define ROW_NUM 4
#define COL_NUM 4

GPIO_TypeDef* row_ports[ROW_NUM] = {GPIOD, GPIOD, GPIOD, GPIOD};  // 连接到 PD8, PD9, PD10, PD11
uint16_t row_pins[ROW_NUM] = {GPIO_PIN_8, GPIO_PIN_9, GPIO_PIN_10, GPIO_PIN_11};

GPIO_TypeDef* col_ports[COL_NUM] = {GPIOD, GPIOD, GPIOD, GPIOD};  // 连接到 PD12, PD13, PD14, PD15
uint16_t col_pins[COL_NUM] = {GPIO_PIN_12, GPIO_PIN_13, GPIO_PIN_14, GPIO_PIN_15};


uint8_t key_val[4][4] = {
    {'1', '4', '7', '*'},
    {'2', '5', '8', '0'},
    {'3', '6', '9', '#'},
    {'A', 'B', 'C', 'D'}
};

uint8_t Password[]={'1','4','0','9','6','6'};

typedef struct {
    uint8_t row;
    uint8_t col;
} KeyInfo;

// 用于跟踪按键的按下状态
uint8_t keyState[ROW_NUM][COL_NUM] = {0};

void InitKeypad(void)
{
    // 配置行为推挽输出
    for (int i = 0; i < ROW_NUM; ++i) {
        GPIO_InitTypeDef GPIO_InitStruct = {0};
        GPIO_InitStruct.Pin = row_pins[i];
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        HAL_GPIO_Init(row_ports[i], &GPIO_InitStruct);
    }

    // 配置列为上拉输入
    for (int i = 0; i < COL_NUM; ++i) {
        GPIO_InitTypeDef GPIO_InitStruct = {0};
        GPIO_InitStruct.Pin = col_pins[i];
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        HAL_GPIO_Init(col_ports[i], &GPIO_InitStruct);
    }
}

void SendKeyOverUART(uint8_t key)
{
    // 中止当前的发送操作
    HAL_UART_AbortTransmit(&huart1);
    // 等待确保缓冲区已经清空
    HAL_Delay(1);
    // 发送键值到串口
    HAL_UART_Transmit(&huart1, &key, 1, HAL_MAX_DELAY);
}

uint8_t ReadKeypad(void)
{
	 HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10| GPIO_PIN_11, GPIO_PIN_SET);
    // 扫描每一行
    for (int row = 0; row < ROW_NUM; ++row) {
        // 设置当前行为低电平
        HAL_GPIO_WritePin(row_ports[row], row_pins[row], GPIO_PIN_RESET);
		
        // 检查每一列
        for (int col = 0; col < COL_NUM; ++col) {
            if (HAL_GPIO_ReadPin(col_ports[col], col_pins[col]) == GPIO_PIN_RESET) {
                HAL_Delay(50);  // 按键消抖
                // 检测到按键按下
                if (keyState[row][col] == 0) {
                    uint8_t key = key_val[row][col];
					__HAL_UART_FLUSH_DRREGISTER(&huart1);
                    SendKeyOverUART(key);
                    // 标记按键已按下
                    keyState[row][col] = 1;

                    // 返回按键的值
                    return key;
                }
            } else {
                // 按键释放，重置状态
                keyState[row][col] = 0;
            }
        }
        // 恢复当前行为高电平
        HAL_GPIO_WritePin(row_ports[row], row_pins[row], GPIO_PIN_SET);
    }

    // 没有按键按下
    return 0;
}
