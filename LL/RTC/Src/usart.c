/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* USART1 init function */

void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  LL_USART_InitTypeDef USART_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);

  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
  /**USART1 GPIO Configuration
  PA9   ------> USART1_TX
  PA10   ------> USART1_RX
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_9;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LL_GPIO_PIN_10;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_FLOATING;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  USART_InitStruct.BaudRate = 115200;
  USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
  USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
  USART_InitStruct.Parity = LL_USART_PARITY_NONE;
  USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
  USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
  USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
  LL_USART_Init(USART1, &USART_InitStruct);
  LL_USART_ConfigAsyncMode(USART1);
  LL_USART_Enable(USART1);
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/* USER CODE BEGIN 1 */
/**
 * @brief Send 8 bit data to the upward machine
 * @param data uint8_t 0~255
 * @return 0 failure ; 1 success
*/
uint8_t usart_transmit8(USART_TypeDef *USARTx, uint8_t data)
{
  static volatile uint16_t retry;

  retry = 0xFFF;
  while (!LL_USART_IsActiveFlag_TXE(USARTx))
  {
    if (!retry--)
      return 0U;
  }

  LL_USART_TransmitData8(USARTx, data);

  retry = 0xFFF;
  while (!LL_USART_IsActiveFlag_TC(USARTx))
  {
    if (!retry--)
      return 0U;
  }

  return 1U;
}
/**
 * @brief get 8 bit data from the upward machine
 * @param void
 * @return (uint8_t)data 0~255
*/
uint8_t usart_receive8(USART_TypeDef *USARTx)
{
  while (!LL_USART_IsActiveFlag_RXNE(USARTx))
  {
  }
  return LL_USART_ReceiveData8(USARTx);
}

#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#define GETCHAR_PROTOTYPE int __io_getchar(void)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#define GETCHAR_PROTOTYPE int fgetc(FILE *f)
#endif /* __GNUC__ */

#define CHAR_STREAM USART1
PUTCHAR_PROTOTYPE
{
  return usart_transmit8(CHAR_STREAM, ch);
}

GETCHAR_PROTOTYPE
{
  return usart_receive8(CHAR_STREAM);
}
#undef CHAR_STREAM

int _read(int file, char *ptr, int len)
{
  int DataIdx;

  for (DataIdx = 0; DataIdx < len; DataIdx++)
  {
    *ptr++ = __io_getchar();
  }

  return len;
}

int _write(int file, char *ptr, int len)
{
  int DataIdx;

  for (DataIdx = 0; DataIdx < len; DataIdx++)
  {
    __io_putchar(*ptr++);
  }
  return len;
}

uint8_t gets_s(char *buff, uint8_t bufsiz)
{
  uint8_t index = 0;
  while (index < bufsiz)
  {
    buff[index] = (char)usart_receive8(USART1);
    if (buff[index++] == '\n')
    {
      buff[index - 1] = 0;
      break;
    }
  }
  if (index == bufsiz)
  {
    buff[bufsiz - 1] = 0;
  }
  return index;
}

uint16_t puts_s(char *buff, uint16_t bufsiz)
{
  uint16_t index = 0;
  while (buff[index] && index < bufsiz)
  {
    index += usart_transmit8(USART1, (uint8_t)buff[index]);
  }
  (void)usart_transmit8(USART1, '\n');
  return index;
}

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
