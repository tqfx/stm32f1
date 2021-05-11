# STM32Cube_FW_F1_V1.8.3

## [stm32f1xx_ll_gpio.h](https://github.com/STMicroelectronics/STM32CubeF1/blob/master/Drivers/STM32F1xx_HAL_Driver/Inc/stm32f1xx_ll_gpio.h)

### The function configures GPIO

```c
/**
  * @brief  This function configures GPIO
  * @param  None
  * @return None
  */
void Configure_GPIO(void)
{
  LL_GPIO_InitTypeDef GPIO_InitStructure = {0};

  /* GPIO Ports Clock Enable */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOC);

  /* JTAG-DP Disabled and SW-DP Enabled */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOB);

  /* Configure IO in output push-pull mode to drive external*/
  GPIO_InitStructure.Pin = LL_GPIO_PIN_13;
  GPIO_InitStructure.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStructure.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStructure.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStructure.Pull = LL_GPIO_PULL_DOWN;

  /* Initialize GPIO_PORT port according to parameters defined in initialization structure. */
  if (LL_GPIO_Init(GPIOC, &GPIO_InitStructure) != SUCCESS)
  {
    /* Initialization Error */
    while (1)
    {
    }
  }

  GPIO_InitStruct.Pin = LL_GPIO_PIN_4|LL_GPIO_PIN_5|LL_GPIO_PIN_7;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  if (LL_GPIO_Init(GPIOA, &GPIO_InitStructure) != SUCCESS)
  {
    /* Initialization Error */
    while (1)
    {
    }
  }

  GPIO_InitStruct.Pin = LL_GPIO_PIN_6;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_FLOATING;
  if (LL_GPIO_Init(GPIOA, &GPIO_InitStructure) != SUCCESS)
  {
    /* Initialization Error */
    while (1)
    {
    }
  }
}
```

### Delay

```c
#define DelayClock 72000000

void delay_us(uint32_t us)
{
  uint32_t temp;
  SysTick->LOAD = us * (DelayClock / 8000000);
  SysTick->VAL = 0x00;
  SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
  do
  {
    temp = SysTick->CTRL;
  } while ((temp & 0x01) && !(temp & (1 << 16)));
  SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
  SysTick->VAL = 0X00;
}

void delay_ms(uint32_t ms)
{
  uint32_t temp;
  SysTick->LOAD = ms * 1000 * (DelayClock / 8000000);
  SysTick->VAL = 0x00;
  SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
  do
  {
    temp = SysTick->CTRL;
  } while ((temp & 0x01) && !(temp & (1 << 16)));
  SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
  SysTick->VAL = 0X00;
}
```

### SPI

```c
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

uint8_t SPI_WriteRead8(uint8_t txData)
{
    uint8_t rxData = 0U;

    for (uint8_t i = 0U; i < 8U; i++)
    {
        SPI_SCK_0;
        SPI_DELAY;
        //TransmitData

        if (txData & 0x80U)
            SPI_MOSI_1;
        else
            SPI_MOSI_0;

        txData <<= 1U;
        SPI_DELAY;

        SPI_SCK_1;
        SPI_DELAY;
        //ReceiveData
        rxData <<= 1U;
        if (SPI_READ_MISO)
            rxData |= 0x01U;

        SPI_DELAY;
    }
    SPI_SCK_1;

    return rxData;
}
```
