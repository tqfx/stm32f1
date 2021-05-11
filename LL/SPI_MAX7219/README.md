
# STM32Cube_FW_F1_V1.8.3

## [stm32f1xx_ll_spi.h](https://github.com/STMicroelectronics/STM32CubeF1/blob/master/Drivers/STM32F1xx_HAL_Driver/Inc/stm32f1xx_ll_spi.h)

### This function configures SPI1

```c
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
```

```c
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
```

```c
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
```

```c
void Write_Max7219(uint8_t addr, uint8_t data)
{
    if (!LL_SPI_IsEnabled(MAX7129_SPI))
    {
        LL_SPI_Enable(MAX7129_SPI);
    }

    while (LL_SPI_IsActiveFlag_BSY(MAX7129_SPI)) //wait spi is free
    {
    }

    LL_GPIO_ResetOutputPin(MAX7129_PORT, MAX7129_CS); //ready to send

    if (LL_SPI_GetDataWidth(MAX7129_SPI) == LL_SPI_DATAWIDTH_8BIT) //8 bit
    {
        (void)SPI_WriteRead8(MAX7129_SPI, addr);
        (void)SPI_WriteRead8(MAX7129_SPI, data);
    }
    else
    {
        (void)SPI_WriteRead16(MAX7129_SPI, (addr << 8) | data);
    }

    while (LL_SPI_IsActiveFlag_BSY(MAX7129_SPI)) //wait transmit finished
    {
    }

    LL_GPIO_SetOutputPin(MAX7129_PORT, MAX7129_CS); //loading the data
}
```

```c
void Max7219_Init(void)
{
    Write_Max7219(0x09, 0x00); //09H 译码方式 高电平 BCD-B译码，低电平 字符码
    Write_Max7219(0x0A, 0x04); //0AH 亮度调节 D0~D3位 0~7 1~8
    Write_Max7219(0x0B, 0x07); //0BH 扫描界限 D0~D3位 0~7 1~8
    Write_Max7219(0x0C, 0x01); //0CH 停机寄存 D0 0 停机，D0 1 开机
    Write_Max7219(0x0F, 0x00); //0FH 显示测试 D0 0 正常，D0 1 测试(最大亮度)
}
```
