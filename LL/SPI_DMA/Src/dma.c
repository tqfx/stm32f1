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
  /* DMA1_Channel2_IRQn interrupt configuration */
  NVIC_SetPriority(DMA1_Channel2_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  NVIC_EnableIRQ(DMA1_Channel2_IRQn);
  /* DMA1_Channel3_IRQn interrupt configuration */
  NVIC_SetPriority(DMA1_Channel3_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  NVIC_EnableIRQ(DMA1_Channel3_IRQn);

}

/* USER CODE BEGIN 2 */

/**
  * @brief This function configures the DMA Channels for SPI1
  * @note  This function is used to :
  *        -1- Enable DMA1 clock
  *        -2- Configure NVIC for DMA1 transfer complete/error interrupts
  *        -3- Configure the DMA1_Channel2 functional parameters
  *        -4- Configure the DMA1_Channel3 functional parameters
  *        -5- Enable DMA1 interrupts complete/error
  * @param   None
  * @retval  None
  */
void Configure_DMA(void)
{
  /* DMA1 used for SPI1 Transmission
   * DMA1 used for SPI1 Reception
   */
  /* (1) Enable the clock of DMA1 and DMA1 */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);

  /* (2) Configure NVIC for DMA transfer complete/error interrupts */
  NVIC_SetPriority(DMA1_Channel2_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
  NVIC_EnableIRQ(DMA1_Channel2_IRQn);
  NVIC_SetPriority(DMA1_Channel3_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
  NVIC_EnableIRQ(DMA1_Channel3_IRQn);

  /* (3) Configure the DMA1_Channel2 functional parameters */
  LL_DMA_ConfigTransfer(DMA1,
                        LL_DMA_CHANNEL_2,
                        LL_DMA_DIRECTION_PERIPH_TO_MEMORY |
                            LL_DMA_PRIORITY_HIGH |
                            LL_DMA_MODE_CIRCULAR |
                            LL_DMA_PERIPH_NOINCREMENT |
                            LL_DMA_MEMORY_INCREMENT |
                            LL_DMA_PDATAALIGN_BYTE |
                            LL_DMA_MDATAALIGN_BYTE);

  /* (4) Configure the DMA1_Channel3 functional parameters */
  LL_DMA_ConfigTransfer(DMA1,
                        LL_DMA_CHANNEL_3,
                        LL_DMA_DIRECTION_MEMORY_TO_PERIPH |
                            LL_DMA_PRIORITY_HIGH |
                            LL_DMA_MODE_NORMAL |
                            LL_DMA_PERIPH_NOINCREMENT |
                            LL_DMA_MEMORY_INCREMENT |
                            LL_DMA_PDATAALIGN_BYTE |
                            LL_DMA_MDATAALIGN_BYTE);

  /* (5) Enable DMA interrupts complete/error */
  LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_2);
  LL_DMA_EnableIT_TE(DMA1, LL_DMA_CHANNEL_2);
  LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_3);
  LL_DMA_EnableIT_TE(DMA1, LL_DMA_CHANNEL_3);
}

/* USER CODE END 2 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
