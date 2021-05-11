#include "MAX7219.h"
#include <stdio.h>

void Write_Max7219(uint8_t addr, uint8_t data)
{
    TxBuffer[0] = addr;
    TxBuffer[1] = data;

    LL_GPIO_ResetOutputPin(MAX7129_PORT, MAX7129_CS);
    LL_DMA_EnableChannel(MAX7129_DMA, MAX7129_DMA_CHANNEL);
}

void Max7119_Display8(uint8_t data[8])
{
    for (uint8_t i = 0; i < 8; i++)
    {
        Write_Max7219(0x01 + i, data[i]);
    }
}

void Max7219_Init(void)
{
    Write_Max7219(0x09, 0x00); //09H 译码方式 高电平 BCD-B译码，低电平 字符码
    Write_Max7219(0x0A, 0x04); //0AH 亮度调节 D0~D3位 0~7 1~8
    Write_Max7219(0x0B, 0x07); //0BH 扫描界限 D0~D3位 0~7 1~8
    Write_Max7219(0x0C, 0x01); //0CH 停机寄存 D0 0 停机，D0 1 开机
    Write_Max7219(0x0F, 0x00); //0FH 显示测试 D0 0 正常，D0 1 测试(最大亮度)
}