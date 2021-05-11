
# STM32Cube_FW_F1_V1.8.3

## [stm32f1xx_ll_tim.h](https://github.com/STMicroelectronics/STM32CubeF1/blob/master/Drivers/STM32F1xx_HAL_Driver/Inc/stm32f1xx_ll_tim.h)

### Configures the timer to generate a PWM signal on the OC2 output

```c
/**
  * @brief  Configures the timer to generate a PWM signal on the OC2 output.
  * @param  None
  * @return None
  */
void Configure_TIMPWMOutput(void)
{
  LL_TIM_InitTypeDef TIM_InitStructure = {0};
  LL_TIM_OC_InitTypeDef TIM_OC_InitStructure = {0};
  LL_GPIO_InitTypeDef GPIO_InitStructure = {0};

  /*************************/
  /* GPIO AF configuration */
  /*************************/
  /* Enable the peripheral clock of GPIOs */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);

  /**TIM2 GPIO Configuration
    PA1     ------> TIM2_CH2
    */
  GPIO_InitStructure.Pin = LL_GPIO_PIN_1;
  GPIO_InitStructure.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStructure.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStructure.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  LL_GPIO_Init(GPIOA, &GPIO_InitStructure);

#if 0
  /***********************************************/
  /* Configure the NVIC to handle TIM2 interrupt */
  /***********************************************/
  NVIC_SetPriority(TIM2_IRQn, 0);
  NVIC_EnableIRQ(TIM2_IRQn);
#endif

  /******************************/
  /* Peripheral clocks enabling */
  /******************************/
  /* Enable the timer peripheral clock */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);

  /***************************/
  /* Time base configuration */
  /***************************/
  /* Set fields of initialization structure */
  /* TIM2CLK = SystemCoreClock / (APB prescaler & multiplier)               */

  TIM_InitStructure.Prescaler = 159;
  TIM_InitStructure.CounterMode = LL_TIM_COUNTERMODE_UP;
  TIM_InitStructure.Autoreload = 999;
  TIM_InitStructure.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
  TIM_InitStructure.RepetitionCounter = (uint8_t)0x00;

  /* Initialize TIM instance according to parameters defined in               */
  /* initialization structure.                                                */
  LL_TIM_Init(TIM2, &TIM_InitStructure);

  /* Enable TIM2_ARR register preload. Writing to or reading from the         */
  /* auto-reload register accesses the preload register. The content of the   */
  /* preload register are transferred into the shadow register at each update */
  /* event (UEV).                                                             */
  LL_TIM_EnableARRPreload(TIM2);

  /*********************************/
  /* Output waveform configuration */
  /*********************************/
  /* Set fields of initialization structure */
  /*  - Set compare value to half of the counter period (50% duty cycle ) */
  TIM_OC_InitStructure.OCMode = LL_TIM_OCMODE_PWM1;
  TIM_OC_InitStructure.OCState = LL_TIM_OCSTATE_DISABLE;
  TIM_OC_InitStructure.OCNState = LL_TIM_OCSTATE_DISABLE;
  TIM_OC_InitStructure.CompareValue = ((LL_TIM_GetAutoReload(TIM2) + 1) / 2);
  TIM_OC_InitStructure.OCPolarity = LL_TIM_OCPOLARITY_HIGH;
  TIM_OC_InitStructure.OCNPolarity = LL_TIM_OCPOLARITY_HIGH;
  TIM_OC_InitStructure.OCIdleState = LL_TIM_OCIDLESTATE_LOW;
  TIM_OC_InitStructure.OCNIdleState = LL_TIM_OCIDLESTATE_LOW;

  /* Initialize TIM instance according to parameters defined in               */
  /* initialization structure.                                                */
  LL_TIM_OC_Init(TIM2, LL_TIM_CHANNEL_CH2, &TIM_OC_InitStructure);

  /* Enable TIM2_CCR1 register preload. Read/Write operations access the      */
  /* preload register. TIM2_CCR1 preload value is loaded in the active        */
  /* at each update event.                                                    */
  LL_TIM_OC_EnablePreload(TIM2, LL_TIM_CHANNEL_CH2);

#if 0
  /**************************/
  /* TIM2 interrupts set-up */
  /**************************/
  /* Enable the capture/compare interrupt for channel 2*/
  LL_TIM_EnableIT_CC2(TIM2);
#endif

  /**********************************/
  /* Start output signal generation */
  /**********************************/
  /* Enable output channel 2 */
  LL_TIM_CC_EnableChannel(TIM2, LL_TIM_CHANNEL_CH2);

  /* Enable counter */
  LL_TIM_EnableCounter(TIM2);

#if 0
  /* Force update generation */
  LL_TIM_GenerateEvent_UPDATE(TIM2);
#endif
}
```
