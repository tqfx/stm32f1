/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define RETRY_MAX 0xFFF
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
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

void LL_I2C_Write8(I2C_TypeDef *I2Cx, uint8_t _addr, uint8_t _reg, uint8_t _data)
{
  uint16_t retry = RETRY_MAX;

  while (LL_I2C_IsActiveFlag_BUSY(I2Cx))
  {
    if (!retry--)
    {
      printf("\rI2C_WBUSY\r\n");
      return;
    }
  }

  LL_I2C_AcknowledgeNextData(I2Cx, LL_I2C_ACK);
  LL_I2C_GenerateStartCondition(I2Cx);

  retry = RETRY_MAX;
  while (!LL_I2C_IsActiveFlag_SB(I2Cx))
  {
    if (!retry--)
    {
      printf("\rI2C_WSB\r\n");
      return;
    }
  }

  LL_I2C_TransmitData8(I2Cx, _addr & (uint8_t)0xFE);

  retry = RETRY_MAX;
  while (!LL_I2C_IsActiveFlag_ADDR(I2Cx))
  {
    if (!retry--)
    {
      printf("\rI2C_WADDR\r\n");
      return;
    }
  }

  LL_I2C_ClearFlag_ADDR(I2Cx);

  retry = RETRY_MAX;
  while (!LL_I2C_IsActiveFlag_TXE(I2Cx))
  {
    if (!retry--)
    {
      printf("\rI2C_WTXE\r\n");
      return;
    }
  }

  LL_I2C_TransmitData8(I2Cx, _reg);

  retry = RETRY_MAX;
  while (!LL_I2C_IsActiveFlag_TXE(I2Cx))
  {
    if (!retry--)
    {
      printf("\rI2C_WTXE2\r\n");
      return;
    }
  }

  LL_I2C_TransmitData8(I2Cx, _data);

  retry = RETRY_MAX;
  while (!LL_I2C_IsActiveFlag_BTF(I2Cx))
  {
    if (!retry--)
    {
      printf("\rI2C_WBTF\r\n");
      return;
    }
  }

  LL_I2C_GenerateStopCondition(I2Cx);
}

uint8_t LL_I2C_Read8(I2C_TypeDef *I2Cx, uint8_t _addr, uint8_t _reg)
{
  uint16_t retry = RETRY_MAX;

  while (LL_I2C_IsActiveFlag_BUSY(I2Cx))
  {
    if (!retry--)
    {
      printf("\rI2C_RBUSY\r\n");
      return 0x00;
    }
  }

  LL_I2C_AcknowledgeNextData(I2Cx, LL_I2C_ACK);
  LL_I2C_GenerateStartCondition(I2Cx);

  retry = RETRY_MAX;
  while (!LL_I2C_IsActiveFlag_SB(I2Cx))
  {
    if (!retry--)
    {
      printf("\rI2C_RSB\r\n");
      return 0x00;
    }
  }

  LL_I2C_TransmitData8(I2Cx, _addr);

  retry = RETRY_MAX;
  while (!LL_I2C_IsActiveFlag_ADDR(I2Cx))
  {
    if (!retry--)
    {
      printf("\rI2C_RADDR\r\n");
      return 0x00;
    }
  }

  LL_I2C_ClearFlag_ADDR(I2Cx);

  retry = RETRY_MAX;
  while (!LL_I2C_IsActiveFlag_TXE(I2Cx))
  {
    if (!retry--)
    {
      printf("\rI2C_RTXE\r\n");
      return 0x00;
    }
  }

  LL_I2C_TransmitData8(I2Cx, _reg);

  retry = RETRY_MAX;
  while (!LL_I2C_IsActiveFlag_TXE(I2Cx))
  {
    if (!retry--)
    {
      printf("\rI2C_RTXE2\r\n");
      return 0x00;
    }
  }

  LL_I2C_GenerateStopCondition(I2Cx);
  LL_I2C_GenerateStartCondition(I2Cx);

  retry = RETRY_MAX;
  while (!LL_I2C_IsActiveFlag_SB(I2Cx))
  {
    if (!retry--)
    {
      printf("\rI2C_RSB2\r\n");
      return 0x00;
    }
  }

  LL_I2C_TransmitData8(I2Cx, _addr | 0x01);

  retry = RETRY_MAX;
  while (!LL_I2C_IsActiveFlag_ADDR(I2Cx))
  {
    if (!retry--)
    {
      printf("\rI2C_RADDR2\r\n");
      return 0x00;
    }
  }

  LL_I2C_AcknowledgeNextData(I2Cx, LL_I2C_NACK);
  LL_I2C_ClearFlag_ADDR(I2Cx);
  LL_I2C_GenerateStopCondition(I2Cx);

  retry = RETRY_MAX;
  while (!LL_I2C_IsActiveFlag_RXNE(I2Cx))
  {
    if (!retry--)
    {
      printf("\rI2C_RRXNE\r\n");
      return 0x00;
    }
  }

  return LL_I2C_ReceiveData8(I2Cx);
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */

  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_AFIO);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  /* System interrupt init*/

  /** NOJTAG: JTAG-DP Disabled and SW-DP Enabled
  */
  LL_GPIO_AF_Remap_SWJ_NOJTAG();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  uint8_t addr = 0x90;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);
  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_0)
  {
  }
  LL_RCC_HSI_SetCalibTrimming(16);
  LL_RCC_HSI_Enable();

   /* Wait till HSI is ready */
  while(LL_RCC_HSI_IsReady() != 1)
  {

  }
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI)
  {

  }
  LL_Init1msTick(8000000);
  LL_SetSystemCoreClock(8000000);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
