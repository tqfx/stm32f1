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
  /* DMA1_Channel4_IRQn interrupt configuration */
  NVIC_SetPriority(DMA1_Channel4_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
  NVIC_EnableIRQ(DMA1_Channel4_IRQn);
  /* DMA1_Channel5_IRQn interrupt configuration */
  NVIC_SetPriority(DMA1_Channel5_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
  NVIC_EnableIRQ(DMA1_Channel5_IRQn);
}

/* USER CODE BEGIN 2 */
void USART_DMA_Config(void)
{
  LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_4, sizeof(uint8_t) * BUFFLEN);
  LL_DMA_SetPeriphAddress(DMA1, LL_DMA_CHANNEL_4, (uint32_t)(&USART1->DR));
  LL_DMA_SetMemoryAddress(DMA1, LL_DMA_CHANNEL_4, (uint32_t)(&Usart1_TxBuff));
  LL_USART_EnableDMAReq_TX(USART1);

  LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_5, sizeof(uint8_t) * BUFFLEN);
  LL_DMA_SetPeriphAddress(DMA1, LL_DMA_CHANNEL_5, (uint32_t)(&USART1->DR));
  LL_DMA_SetMemoryAddress(DMA1, LL_DMA_CHANNEL_5, (uint32_t)(&Usart1_RxBuff));
  LL_USART_EnableDMAReq_RX(USART1);
}

/**
  * @brief  This function initiates TX and RX DMA transfers by enabling DMA channels
  * @param  None
  * @retval None
  */
void StartTransfers(void)
{
  /* Enable DMA RX Interrupt */
  LL_USART_EnableDMAReq_RX(USART1);

  /* Enable DMA TX Interrupt */
  LL_USART_EnableDMAReq_TX(USART1);

  /* Enable DMA Channel Rx */
  LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_5);

  /* Enable DMA Channel Tx */
  LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_4);
}

/**
  * @brief  This function configures the DMA Channels for TX and RX transfers
  * @note   This function is used to :
  *         -1- Enable DMA1 clock
  *         -2- Configure NVIC for DMA transfer complete/error interrupts 
  *         -3- Configure DMA TX channel functional parameters
  *         -4- Configure DMA RX channel functional parameters
  *         -5- Enable transfer complete/error interrupts
  * @param  None
  * @retval None
  */
void Configure_DMA(void)
{
  /* DMA1 used for USART1 Transmission and Reception
   */
  /* (1) Enable the clock of DMA1 */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);

  /* (2) Configure NVIC for DMA transfer complete/error interrupts */
  NVIC_SetPriority(DMA1_Channel4_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
  NVIC_EnableIRQ(DMA1_Channel4_IRQn);
  NVIC_SetPriority(DMA1_Channel5_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
  NVIC_EnableIRQ(DMA1_Channel5_IRQn);

  /* (3) Configure the DMA functional parameters for transmission */
  LL_DMA_ConfigTransfer(DMA1, LL_DMA_CHANNEL_4,
                        LL_DMA_DIRECTION_MEMORY_TO_PERIPH |
                            LL_DMA_PRIORITY_HIGH |
                            LL_DMA_MODE_NORMAL |
                            LL_DMA_PERIPH_NOINCREMENT |
                            LL_DMA_MEMORY_INCREMENT |
                            LL_DMA_PDATAALIGN_BYTE |
                            LL_DMA_MDATAALIGN_BYTE);

  /* (4) Configure the DMA functional parameters for reception */
  LL_DMA_ConfigTransfer(DMA1, LL_DMA_CHANNEL_5,
                        LL_DMA_DIRECTION_PERIPH_TO_MEMORY |
                            LL_DMA_PRIORITY_HIGH |
                            LL_DMA_MODE_CIRCULAR |
                            LL_DMA_PERIPH_NOINCREMENT |
                            LL_DMA_MEMORY_INCREMENT |
                            LL_DMA_PDATAALIGN_BYTE |
                            LL_DMA_MDATAALIGN_BYTE);

  /* (5) Enable DMA transfer complete/error interrupts  */
  LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_4);
  //LL_DMA_EnableIT_TE(DMA1, LL_DMA_CHANNEL_4);
  LL_DMA_EnableIT_HT(DMA1, LL_DMA_CHANNEL_5);
  LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_5);
  //LL_DMA_EnableIT_TE(DMA1, LL_DMA_CHANNEL_5);
}
/* USER CODE END 2 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
