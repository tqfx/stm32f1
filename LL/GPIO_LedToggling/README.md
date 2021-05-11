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
}
```

### System Clock Configuration

```c
/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 72000000
  *            HCLK(Hz)                       = 72000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 2
  *            APB2 Prescaler                 = 1
  *            HSE Frequency(Hz)              = 8000000
  *            PLLMUL                         = 9
  *            Flash Latency(WS)              = 2
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  /* Set FLASH latency */
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);
  while (LL_FLASH_GetLatency() != LL_FLASH_LATENCY_2)
  {
  }

  /* Enable HSE oscillator */
  LL_RCC_HSE_Enable();
  while (LL_RCC_HSE_IsReady() != 1)
  {
  }

  /* Main PLL configuration and activation */
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE_DIV_1, LL_RCC_PLL_MUL_9);

  LL_RCC_PLL_Enable();
  while (LL_RCC_PLL_IsReady() != 1)
  {
  }

  /* Sysclk activation on the main PLL */
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
  while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {
  }

  /* Set APB1 & APB2 prescaler*/
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

  /* Set systick to 1ms in using frequency set to 72MHz */
  LL_Init1msTick(72000000);

  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  LL_SetSystemCoreClock(72000000);
}
```
