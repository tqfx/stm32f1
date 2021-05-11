
# STM32Cube_FW_F1_V1.8.3

## [stm32f1xx_ll_rtc.h](https://github.com/STMicroelectronics/STM32CubeF1/blob/master/Drivers/STM32F1xx_HAL_Driver/Inc/stm32f1xx_ll_rtc.h)

### Private variables

```c
LL_RTC_TimeTypeDef RTC_TimeStructure = {0};
LL_RTC_TimeTypeDef RTC_DateStructure = {0};

uint8_t EndOfMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
uint32_t timeCounter = 0;
uint8_t dateUpdate = 0;
uint8_t timeUpdate = 0;
```

### Configure RTC_DATE Structure

```c
/**
  * @brief  Configure RTC_DATE Structure
  * @param  fDate:  Date
  * @param  fMonth: Month
  * @param  fYear:  Year
  * @return None
  */
void RTC_DATE_Config(uint8_t fDate, uint8_t fMonth, uint8_t fYear)
{
  RTC_DateStructure.Seconds = fDate;
  RTC_DateStructure.Minutes = fMonth;
  RTC_DateStructure.Hours = fYear;
}
```

### Configure RTC_TIME Structure

```c
/**
  * @brief  Configure RTC_TIME Structure
  * @param  fHour: Hour
  * @param  fMin:  Minut
  * @param  fSec:  seconds
  * @return None
  */
void RTC_TIME_Config(uint8_t fHour, uint8_t fMin, uint8_t fSec)
{
  RTC_TimeStructure.Hours = fHour;
  RTC_TimeStructure.Minutes = fMin;
  RTC_TimeStructure.Seconds = fSec;

  LL_RTC_TIME_Init(RTC, LL_RTC_FORMAT_BCD, &RTC_TimeStructure);
}
```

### Update RTC_Date Structure

```c
/**
  * @brief  Update RTC_Date Structure
  * @param  None
  * @return None
  */
void RTC_DATE_structUpdate(void)
{
  /* Update DATE when Time is 00:00:00 */
  if ((timeCounter == 0U) & (dateUpdate != 0U))
  {
    dateUpdate = 0U;
    if (RTC_DateStructure.Seconds == EndOfMonth[RTC_DateStructure.Minutes - 1U])
    {
      RTC_DateStructure.Seconds = 1U;
      if (RTC_DateStructure.Minutes == 12U)
      {
        RTC_DateStructure.Minutes = 1U;
        RTC_DateStructure.Hours += 1U;
      }
      else
      {
        RTC_DateStructure.Minutes += 1U;
      }
    }
    else
    {
      RTC_DateStructure.Seconds += 1U;
    }
  }
}
```

### Update RTC_TIME Structure

```c
/**
  * @brief  Update RTC_TIME Structure
  * @param  None
  * @return None
  */
void RTC_TIME_structUpadate(void)
{
  if (timeUpdate != 0)
  {
    timeUpdate = 0;
    RTC_TimeStructure.Hours = (timeCounter / 3600);
    RTC_TimeStructure.Minutes = (timeCounter % 3600) / 60;
    RTC_TimeStructure.Seconds = (timeCounter % 3600) % 60;
  }
}
```

### Configure the current time and date

```c
/**
  * @brief  Configure the current time and date.
  * @param  None
  * @return None
  */
void Configure_RTC_Calendar(void)
{
  /*##-1- Disable RTC registers write protection ############################*/
  LL_RTC_DisableWriteProtection(RTC);

  /*##-2- Enter in initialization mode ######################################*/
  if (LL_RTC_EnterInitMode(RTC) != SUCCESS)
  {
  }

  /*##-3- Configure the Date ################################################*/
  /* Note: __LL_RTC_CONVERT_BIN2BCD helper macro can be used if user wants to*/
  /*       provide directly the decimal value:                               */
  /*       LL_RTC_DATE_Config(RTC, ,                                         */
  /*                          __LL_RTC_CONVERT_BIN2BCD(31), (...))           */
  /* Set Date: 31 December 2020 */
  RTC_DATE_Config(31, 12, 20);

  /*##-4- Configure the Time ################################################*/
  /* Set Time: 23:59:55 */
  RTC_TIME_Config(23, 59, 55);

  /* Enable Second Interrupt */
  LL_RTC_EnableIT_SEC(RTC);

  /*##-5- Exit of initialization mode #######################################*/
  if (LL_RTC_ExitInitMode(RTC) != SUCCESS)
  {
  }

  /*##-6- Enable RTC registers write protection #############################*/
  LL_RTC_EnableWriteProtection(RTC);
}
```

### Configure RTC

```c
/**
  * @brief  Configure RTC.
  * @note   Peripheral configuration is minimal configuration from reset values.
  *         Thus, some useless LL unitary functions calls below are provided as
  *         commented examples - setting is default configuration from reset.
  * @param  None
  * @return None
  */
void Configure_RTC(void)
{
  if (LL_RCC_GetRTCClockSource() != LL_RCC_RTC_CLKSOURCE_LSE)
  {
    LL_RCC_ForceBackupDomainReset();
    LL_RCC_ReleaseBackupDomainReset();
    LL_RCC_SetRTCClockSource(LL_RCC_RTC_CLKSOURCE_LSE);
  }
  /*##-1- Enable RTC peripheral Clocks #######################################*/
  /* Enable RTC Clock */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_BKP);
  LL_PWR_EnableBkUpAccess();
  LL_RCC_EnableRTC();

  /*##-2- Disable RTC registers write protection ##############################*/
  LL_RTC_DisableWriteProtection(RTC);

  /*##-3- Enter in initialization mode #######################################*/
  if (LL_RTC_EnterInitMode(RTC) != SUCCESS)
  {
  }

  /*##-4- Configure RTC ######################################################*/
  /* Configure RTC prescaler */
  /* Set Asynch Prediv (value according to source clock) */
  LL_RTC_InitTypeDef RTC_InitStructure = {0};
  LL_RTC_StructInit(&RTC_InitStructure);
  RTC_InitStructure.OutPutSource = LL_RTC_CALIB_OUTPUT_SECOND;
  LL_RTC_Init(RTC, &RTC_InitStructure);

  /* RTC_Alarm Interrupt Configuration: EXTI configuration */
  LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_17);
  LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_17);

  /*##-5- Configure the NVIC for RTC Alarm ###############################*/
  NVIC_SetPriority(RTC_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
  NVIC_EnableIRQ(RTC_IRQn);

  /*##-6- Exit of initialization mode #######################################*/
  if (LL_RTC_ExitInitMode(RTC) != SUCCESS)
  {
  }

  /*##-7- Enable RTC registers write protection #############################*/
  LL_RTC_EnableWriteProtection(RTC);
}
```

### Calendar callback

```c
/**
  * @brief  Calendar callback
  * @param  None
  * @return None
  */
void Calendar_Callback(void)
{
  timeCounter = LL_RTC_TIME_Get(RTC);
  timeUpdate = 1;
  /* Reset RTC Counter when Time is 23:59:59 */
  if (timeCounter == 0x0001517FU)
  {
    dateUpdate = 1;

    LL_RTC_DisableWriteProtection(RTC);

    if (LL_RTC_EnterInitMode(RTC) != SUCCESS)
    {
    }

    LL_RTC_TIME_Set(RTC, 0x0U);
    /* Wait until last write operation on RTC registers has finished */
    LL_RTC_WaitForSynchro(RTC);

    if (LL_RTC_ExitInitMode(RTC) != SUCCESS)
    {
    }

    LL_RTC_EnableWriteProtection(RTC);
  }
}
```

### Display the current time and date

```c
#include <stdio.h>
/**
  * @brief  Display the current time and date.
  * @param  None
  * @return None
  */
void Show_RTC_Calendar(void)
{
  RTC_TIME_structUpadate();
  RTC_DATE_structUpdate();

  /* Note: need to convert in decimal value in using __LL_RTC_CONVERT_BCD2BIN helper macro */
  /* Display date Format : yy-mm-dd */
  printf("%.2d-%.2d-%.2d", (2000 + RTC_DateStructure.Hours),
         RTC_DateStructure.Seconds,
         RTC_DateStructure.Minutes);
  putchar(' ');
  /* Display time Format : hh:mm:ss */
  printf("%.2d:%.2d:%.2d", RTC_TimeStructure.Hours,
         RTC_TimeStructure.Minutes,
         RTC_TimeStructure.Seconds);
  printf("\r\n");
}
```
