
# STM32Cube_FW_F1_V1.8.3

## [stm32f1xx_ll_exti.h](https://github.com/STMicroelectronics/STM32CubeF1/blob/master/Drivers/STM32F1xx_HAL_Driver/Inc/stm32f1xx_ll_exti.h)

### The function configures EXTI Line as Push-Button

```c
/**
  * @brief  This function configures EXTI Line as Push-Button
  * @param  None
  * @retval None
  */
void Configure_EXTI(void)
{
  LL_EXTI_InitTypeDef EXTI_InitStructure = {0};

  /* -1- GPIO Config */
  /* Enable GPIO Clock (to be able to program the configuration registers) */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);

  /* Configure IO */
  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_0, LL_GPIO_MODE_FLOATING);

  /* -2- Connect External Line to the GPIO*/
  LL_GPIO_AF_SetEXTISource(LL_GPIO_AF_EXTI_PORTA, LL_GPIO_AF_EXTI_LINE0);

  /*-3- Enable a falling trigger EXTI line 0 Interrupt */
  /* Set fields of initialization structure */
  EXTI_InitStructure.Line_0_31 = LL_EXTI_LINE_0;
  EXTI_InitStructure.LineCommand = ENABLE;
  EXTI_InitStructure.Mode = LL_EXTI_MODE_IT;
  EXTI_InitStructure.Trigger = LL_EXTI_TRIGGER_FALLING;

  /* Initialize EXTI according to parameters defined in initialization structure. */
  if (LL_EXTI_Init(&EXTI_InitStructure) != SUCCESS)
  {
    /* Initialization Error */
    while (1)
    {
    }
  }

  /*-4- Configure NVIC for EXTI0_IRQn */
  NVIC_SetPriority(EXTI0_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
  NVIC_EnableIRQ(EXTI0_IRQn);
}
```

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
