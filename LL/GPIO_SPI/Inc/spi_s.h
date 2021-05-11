#ifndef __SPI_S_H__
#define __SPI_S_H__

#include "main.h"
#include "delay.h"

#define SPI_SCK_PORT GPIOA
#define SPI_SCK_PIN LL_GPIO_PIN_5
#define SPI_MISO_PORT GPIOA
#define SPI_MISO_PIN LL_GPIO_PIN_6
#define SPI_MOSI_PORT GPIOA
#define SPI_MOSI_PIN LL_GPIO_PIN_7

#define SPI_SCK_1 LL_GPIO_SetOutputPin(SPI_SCK_PORT, SPI_SCK_PIN)         /* SCK = 1 */
#define SPI_SCK_0 LL_GPIO_ResetOutputPin(SPI_SCK_PORT, SPI_SCK_PIN)       /* SCK = 0 */
#define SPI_MOSI_1 LL_GPIO_SetOutputPin(SPI_MOSI_PORT, SPI_MOSI_PIN)      /* MOSI = 1 */
#define SPI_MOSI_0 LL_GPIO_ResetOutputPin(SPI_MOSI_PORT, SPI_MOSI_PIN)    /* MOSI = 0 */
#define SPI_READ_MISO LL_GPIO_IsOutputPinSet(SPI_MISO_PORT, SPI_MISO_PIN) /* read input */
#define SPI_DELAY delay_us(1)

uint8_t SPI_WriteRead8(uint8_t txData);

#endif