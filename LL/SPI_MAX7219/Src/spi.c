/**
  ******************************************************************************
  * @file    spi.c
  * @brief   This file provides code for the configuration
  *          of the SPI instances.
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

/* Includes ------------------------------------------------------------------*/
#include "spi.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* SPI1 init function */
void MX_SPI1_Init(void)
{
  LL_SPI_InitTypeDef SPI_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SPI1);

  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
  /**SPI1 GPIO Configuration
  PA5   ------> SPI1_SCK
  PA6   ------> SPI1_MISO
  PA7   ------> SPI1_MOSI
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_5 | LL_GPIO_PIN_7;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LL_GPIO_PIN_6;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_FLOATING;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  SPI_InitStruct.TransferDirection = LL_SPI_FULL_DUPLEX;
  SPI_InitStruct.Mode = LL_SPI_MODE_MASTER;
  SPI_InitStruct.DataWidth = LL_SPI_DATAWIDTH_8BIT;
  SPI_InitStruct.ClockPolarity = LL_SPI_POLARITY_HIGH;
  SPI_InitStruct.ClockPhase = LL_SPI_PHASE_2EDGE;
  SPI_InitStruct.NSS = LL_SPI_NSS_SOFT;
  SPI_InitStruct.BaudRate = LL_SPI_BAUDRATEPRESCALER_DIV256;
  SPI_InitStruct.BitOrder = LL_SPI_MSB_FIRST;
  SPI_InitStruct.CRCCalculation = LL_SPI_CRCCALCULATION_DISABLE;
  SPI_InitStruct.CRCPoly = 10;
  LL_SPI_Init(SPI1, &SPI_InitStruct);
}

/* USER CODE BEGIN 1 */

/**
  * @brief This function configures SPI1.
  * @note  This function is used to :
  *        -1- Enables GPIO clock and configures the SPI1 pins.
  *        -2- Configure SPI1 functional parameters.
  * @note   Peripheral configuration is minimal configuration from reset values.
  *         Thus, some useless LL unitary functions calls below are provided as
  *         commented examples - setting is default configuration from reset.
  * @param  None
  * @retval None
  */
void Configure_SPI(void)
{
  /* (1) Enables GPIO clock and configures the SPI1 pins ********************/
  /* Enable the peripheral clock of GPIOA */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);

  /**SPI1 GPIO Configuration
  PA5   ------> SPI1_SCK
  PA6   ------> SPI1_MISO
  PA7   ------> SPI1_MOSI
  */
  LL_GPIO_InitTypeDef GPIO_InitStructure = {0};
  GPIO_InitStructure.Pin = LL_GPIO_PIN_5 | LL_GPIO_PIN_7;
  GPIO_InitStructure.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStructure.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStructure.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  LL_GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.Pin = LL_GPIO_PIN_6;
  GPIO_InitStructure.Mode = LL_GPIO_MODE_FLOATING;
  LL_GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* (2) Configure SPI1 functional parameters ********************************/
  /* Enable the peripheral clock of SPI1 */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SPI1);

  /* Configure SPI1 communication */
  LL_SPI_InitTypeDef SPI_InitStructure = {0};
  SPI_InitStructure.TransferDirection = LL_SPI_FULL_DUPLEX;
  SPI_InitStructure.Mode = LL_SPI_MODE_MASTER;
  SPI_InitStructure.DataWidth = LL_SPI_DATAWIDTH_8BIT;
  SPI_InitStructure.ClockPolarity = LL_SPI_POLARITY_HIGH;
  SPI_InitStructure.ClockPhase = LL_SPI_PHASE_2EDGE;
  SPI_InitStructure.NSS = LL_SPI_NSS_SOFT;
  SPI_InitStructure.BaudRate = LL_SPI_BAUDRATEPRESCALER_DIV256;
  SPI_InitStructure.BitOrder = LL_SPI_MSB_FIRST;
  SPI_InitStructure.CRCCalculation = LL_SPI_CRCCALCULATION_DISABLE;
  SPI_InitStructure.CRCPoly = 7;
  LL_SPI_Init(SPI1, &SPI_InitStructure);
}

/**
  * @brief  This function Activate SPI1
  * @param  None
  * @retval None
  */
void Activate_SPI(void)
{
  /* Enable SPI1 */
  LL_SPI_Enable(SPI1);
}

#define RETRY_MAX 0xFF

uint8_t SPI_WriteRead8(SPI_TypeDef *SPIx, uint8_t data)
{
  static volatile uint8_t retry = 0;

  retry = RETRY_MAX;
  while (!LL_SPI_IsActiveFlag_TXE(SPIx))
  {
    if (!retry--)
    {
      return 0;
    }
  }

  LL_SPI_TransmitData8(SPIx, data);

  retry = RETRY_MAX;
  while (!LL_SPI_IsActiveFlag_RXNE(SPIx))
  {
    if (!retry--)
    {
      return 0;
    }
  }

  return LL_SPI_ReceiveData8(SPIx);
}

uint16_t SPI_WriteRead16(SPI_TypeDef *SPIx, uint16_t data)
{
  static volatile uint8_t retry = 0;

  retry = RETRY_MAX;
  while (!LL_SPI_IsActiveFlag_TXE(SPIx))
  {
    if (!retry--)
    {
      return 0;
    }
  }

  LL_SPI_TransmitData16(SPIx, data);

  retry = RETRY_MAX;
  while (!LL_SPI_IsActiveFlag_RXNE(SPIx))
  {
    if (!retry--)
    {
      return 0;
    }
  }

  return LL_SPI_ReceiveData16(SPIx);
}

#undef RETRY_MAX

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
