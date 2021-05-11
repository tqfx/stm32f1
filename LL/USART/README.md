
# STM32Cube_FW_F1_V1.8.3

## [stm32f1xx_ll_usart.h](https://github.com/STMicroelectronics/STM32CubeF1/blob/master/Drivers/STM32F1xx_HAL_Driver/Inc/stm32f1xx_ll_usart.h)

### The function configures USARTx Instance

```c
/**
  * @brief  This function configures USARTx Instance.
  * @note   This function is used to :
  *         -1- Enable GPIO clock and configures the USART pins.
  *         -2- Configure USART functional parameters.
  *         -3- Enable USART.
  * @param  None
  * @return None
  */
void Configure_USART(void)
{
  LL_USART_InitTypeDef USART_InitStructure = {0};
  LL_GPIO_InitTypeDef GPIO_InitStructure = {0};

  /* (1) Enable GPIO clock and configures the USART pins *********************/

  /* Enable the peripheral clock of GPIO Port */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);

  /* Enable USART peripheral clock *******************************************/
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);

  /**USART1 GPIO Configuration
  PA9   ------> USART1_TX
  PA10  ------> USART1_RX
  */

  /* Configure Tx Pin as : Alternate function, High Speed, Push pull, Pull up */
  GPIO_InitStructure.Pin = LL_GPIO_PIN_9;
  GPIO_InitStructure.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStructure.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStructure.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  LL_GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Configure Rx Pin as : Input Floating function, High Speed, Pull up */
  GPIO_InitStructure.Pin = LL_GPIO_PIN_10;
  GPIO_InitStructure.Mode = LL_GPIO_MODE_FLOATING;
  LL_GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* (2) Configure USART functional parameters ********************************/

  /* Set Baudrate to 115200 */
  USART_InitStructure.BaudRate = 115200;
  /* 8 data bit, 1 start bit, 1 stop bit, no parity */
  USART_InitStructure.DataWidth = LL_USART_DATAWIDTH_8B;
  USART_InitStructure.StopBits = LL_USART_STOPBITS_1;
  USART_InitStructure.Parity = LL_USART_PARITY_NONE;
  USART_InitStructure.TransferDirection = LL_USART_DIRECTION_TX_RX;
  /* No Hardware Flow control */
  USART_InitStructure.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
  USART_InitStructure.OverSampling = LL_USART_OVERSAMPLING_16;
  LL_USART_Init(USART1, &USART_InitStructure);
  LL_USART_ConfigAsyncMode(USART1);

  /* (3) Enable USART *********************************************************/
  LL_USART_Enable(USART1);
}
```

### Transmit or Receive 8 bit data

```c
/**
 * @brief Send 8 bit data to the upward machine
 * @param data uint8_t 0~255
 * @return 0 failure ; 1 success
*/
uint8_t USART_Transmit8(USART_TypeDef *USARTx, uint8_t data)
{
  static volatile uint16_t retry;

  retry = 0xFFF;
  while (!LL_USART_IsActiveFlag_TXE(USARTx))
  {
    if (!retry--)
    {
      return 0U;
    }
  }

  LL_USART_TransmitData8(USARTx, data);

  retry = 0xFFF;
  while (!LL_USART_IsActiveFlag_TC(USARTx))
  {
    if (!retry--)
    {
      return 0U;
    }
  }

  return 1U;
}

/**
 * @brief get 8 bit data from the upward machine
 * @param void
 * @return (uint8_t)data 0~255
*/
uint8_t USART_Receive8(USART_TypeDef *USARTx)
{
  while (!LL_USART_IsActiveFlag_RXNE(USARTx))
  {
  }
  return LL_USART_ReceiveData8(USARTx);
}
```

```c
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#define GETCHAR_PROTOTYPE int __io_getchar(void)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#define GETCHAR_PROTOTYPE int fgetc(FILE *f)
#endif /* __GNUC__ */

#define CHAR_STREAM USART1
PUTCHAR_PROTOTYPE
{
  return USART_Transmit8(CHAR_STREAM, ch);
}

GETCHAR_PROTOTYPE
{
  return USART_Receive8(CHAR_STREAM);
}
#undef CHAR_STREAM

int _read(int file, char *ptr, int len)
{
  int DataIdx;

  for (DataIdx = 0; DataIdx < len; DataIdx++)
  {
    *ptr++ = __io_getchar();
  }

  return len;
}

int _write(int file, char *ptr, int len)
{
  int DataIdx;

  for (DataIdx = 0; DataIdx < len; DataIdx++)
  {
    __io_putchar(*ptr++);
  }
  return len;
}
```

```c
/**
 * @brief gets
 * @param buff
 * @param bufsiz
 * @return count of characters
*/
uint8_t gets_s(char *buff, uint8_t bufsiz)
{
  uint8_t index = 0;
  while (index < bufsiz)
  {
    buff[index] = (char)USART_Receive8(USART1);
    if (buff[index++] == '\n')
    {
      buff[index - 1] = 0;
      break;
    }
  }
  if (index == bufsiz)
  {
    buff[bufsiz - 1] = 0;
  }
  return index;
}

/**
 * @brief puts
 * @param buff
 * @param bufsiz
 * @return count of characters
*/
uint16_t puts_s(char *buff, uint16_t bufsiz)
{
  uint16_t index = 0;
  while (buff[index] && index < bufsiz)
  {
    index += USART_Transmit8(USART1, (uint8_t)buff[index]);
  }
  (void)USART_Transmit8(USART1, '\n');
  return index;
}
```
