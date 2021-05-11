
# STM32Cube_FW_F1_V1.8.3

## [stm32f1xx_ll_dma.h](https://github.com/STMicroelectronics/STM32CubeF1/blob/master/Drivers/STM32F1xx_HAL_Driver/Inc/stm32f1xx_ll_dma.h)

### The function configures the DMA Channels for TX and RX transfers

```c
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
```

### the function initiates TX and RX DMA transfers by enabling DMA channels

```c
#define BUFFLEN 256U
uint8_t Usart1_RxBuff[BUFFLEN];
uint8_t Usart1_TxBuff[BUFFLEN];

/**
  * @brief  This function initiates TX and RX DMA transfers by enabling DMA channels
  * @param  None
  * @retval None
  */
void Activate_DMA(void)
{
  LL_USART_EnableDMAReq_TX(USART1);
  LL_DMA_ConfigAddresses(DMA1, LL_DMA_CHANNEL_4,
                         (uint32_t)(Usart1_TxBuff),
                         LL_USART_DMA_GetRegAddr(USART1),
                         LL_DMA_GetDataTransferDirection(DMA1, LL_DMA_CHANNEL_4));
  LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_4, BUFFLEN);
  LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_4);

  LL_USART_EnableDMAReq_RX(USART1);
  LL_DMA_ConfigAddresses(DMA1, LL_DMA_CHANNEL_5,
                         LL_USART_DMA_GetRegAddr(USART1),
                         (uint32_t)(Usart1_RxBuff),
                         LL_DMA_GetDataTransferDirection(DMA1, LL_DMA_CHANNEL_5));
  LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_5, BUFFLEN);
  LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_5);
}
```

### IRQHandler

```c
/**
  * @brief This function handles DMA1 channel4 global interrupt.
  */
void DMA1_Channel4_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel4_IRQn 0 */

  /* USER CODE END DMA1_Channel4_IRQn 0 */

  /* USER CODE BEGIN DMA1_Channel4_IRQn 1 */

  LL_DMA_ClearFlag_GI4(DMA1);

  DMA1_Channel4_Callback();

  /* USER CODE END DMA1_Channel4_IRQn 1 */
}

/**
  * @brief This function handles DMA1 channel5 global interrupt.
  */
void DMA1_Channel5_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel5_IRQn 0 */

  /* USER CODE END DMA1_Channel5_IRQn 0 */

  /* USER CODE BEGIN DMA1_Channel5_IRQn 1 */

  if (LL_DMA_IsActiveFlag_HT5(DMA1))
  {
    LL_DMA_ClearFlag_HT5(DMA1);
  }
  else if (LL_DMA_IsActiveFlag_TC5(DMA1))
  {
    LL_DMA_ClearFlag_TC5(DMA1);
  }

  LL_DMA_ClearFlag_GI5(DMA1);

  DMA1_Channel5_Callback(LL_DMA_GetDataLength(DMA1, LL_DMA_CHANNEL_5));

  /* USER CODE END DMA1_Channel5_IRQn 1 */
}
```

### Callback

```c
void DMA1_Channel4_Callback(void)
{
  LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_4);
  LL_DMA_ConfigAddresses(DMA1, LL_DMA_CHANNEL_4,
                         (uint32_t)(Usart1_TxBuff),
                         LL_USART_DMA_GetRegAddr(USART1),
                         LL_DMA_GetDataTransferDirection(DMA1, LL_DMA_CHANNEL_4));
  LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_4, BUFFLEN);
}

void DMA1_Channel5_Callback(uint32_t Lenth)
{
  static uint16_t Remain = BUFFLEN;

  for (uint16_t i = BUFFLEN - Remain; i != BUFFLEN - Lenth;)
  {
    Usart1_TxBuff[i] = Usart1_RxBuff[i];
    if (++i == BUFFLEN)
    {
      LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_4);
      i = 0;
    }
  }

  Remain = Lenth;
}
```
