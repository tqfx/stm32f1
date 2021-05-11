/**
  ******************************************************************************
  * @file    dma.c
  * @brief   This file provides code for the configuration
  *          of all the requested memory to memory DMA transfers.
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
#include "dma.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure DMA                                                              */
/*----------------------------------------------------------------------------*/

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/**
  * Enable DMA controller clock
  */
void MX_DMA_Init(void)
{

  /* Init with LL driver */
  /* DMA controller clock enable */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);

  /* DMA interrupt init */
  /* DMA1_Channel1_IRQn interrupt configuration */
  NVIC_SetPriority(DMA1_Channel1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
  NVIC_EnableIRQ(DMA1_Channel1_IRQn);
}

/* USER CODE BEGIN 2 */

/**
  * @brief  This function configures DMA for transfer of data from ADC
  * @param  None
  * @retval None
  */
void Configure_DMA(void)
{
  /*## Configuration of NVIC #################################################*/
  /* Configure NVIC to enable DMA interruptions */
  NVIC_SetPriority(DMA1_Channel1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 1, 0)); /* DMA IRQ lower priority than ADC IRQ */
  NVIC_EnableIRQ(DMA1_Channel1_IRQn);

  /*## Configuration of DMA ##################################################*/
  /* Enable the peripheral clock of DMA */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);

  /* Configure the DMA transfer */
  /*  - DMA transfer in circular mode to match with ADC configuration:        */
  /*    DMA unlimited requests.                                               */
  /*  - DMA transfer from ADC without address increment.                      */
  /*  - DMA transfer to memory with address increment.                        */
  /*  - DMA transfer from ADC by half-word to match with ADC configuration:   */
  /*    ADC resolution 12 bits.                                               */
  /*  - DMA transfer to memory by half-word to match with ADC conversion data */
  /*    buffer variable type: half-word.                                      */
  LL_DMA_ConfigTransfer(DMA1,
                        LL_DMA_CHANNEL_1,
                        LL_DMA_DIRECTION_PERIPH_TO_MEMORY |
                            LL_DMA_MODE_CIRCULAR |
                            LL_DMA_PERIPH_NOINCREMENT |
                            LL_DMA_MEMORY_INCREMENT |
                            LL_DMA_PDATAALIGN_HALFWORD |
                            LL_DMA_MDATAALIGN_HALFWORD |
                            LL_DMA_PRIORITY_HIGH);

#if 0
  /* Enable DMA transfer interruption: transfer complete */
  LL_DMA_EnableIT_TC(DMA1,
                     LL_DMA_CHANNEL_1);

  /* Enable DMA transfer interruption: half transfer */
  LL_DMA_EnableIT_HT(DMA1,
                     LL_DMA_CHANNEL_1);

  /* Enable DMA transfer interruption: transfer error */
  LL_DMA_EnableIT_TE(DMA1,
                     LL_DMA_CHANNEL_1);

  /*## Activation of DMA #####################################################*/
  /* Enable the DMA transfer */
  LL_DMA_EnableChannel(DMA1,
                       LL_DMA_CHANNEL_1);
#endif
}
/* USER CODE END 2 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
