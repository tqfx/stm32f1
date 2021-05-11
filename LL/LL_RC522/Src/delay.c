#include "delay.h"

void delay_us(uint32_t _us)
{
  uint32_t temp;
  SysTick->LOAD = _us * (DelayClock / 8000000);
  SysTick->VAL = 0x00;
  SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
  do
  {
    temp = SysTick->CTRL;
  } while ((temp & 0x01) && !(temp & (1 << 16)));
  SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
  SysTick->VAL = 0X00;
}

void delay_ms(uint32_t _ms)
{
  uint32_t temp;
  SysTick->LOAD = _ms * 1000 * (DelayClock / 8000000);
  SysTick->VAL = 0x00;
  SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
  do
  {
    temp = SysTick->CTRL;
  } while ((temp & 0x01) && !(temp & (1 << 16)));
  SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
  SysTick->VAL = 0X00;
}