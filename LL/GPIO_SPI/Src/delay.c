#include "delay.h"

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