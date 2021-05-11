#ifndef __MAX7219_H__
#define __MAX7219_H__

#include "main.h"
#include "spi.h"

#define MAX7129_PORT GPIOA
#define MAX7129_CS LL_GPIO_PIN_4
#define MAX7129_SPI SPI1

void Write_Max7219(uint8_t addr, uint8_t data);
void Max7119_Display8(uint8_t data[8]);
void Max7219_Init(void);

#endif