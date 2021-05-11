#ifndef __ULN2003_H__
#define __ULN2003_H__

#include "main.h"

#define ULN_PORT1 GPIOB
#define ULN_PIN1 LL_GPIO_PIN_12
#define ULN_PORT2 GPIOB
#define ULN_PIN2 LL_GPIO_PIN_13
#define ULN_PORT3 GPIOB
#define ULN_PIN3 LL_GPIO_PIN_14
#define ULN_PORT4 GPIOB
#define ULN_PIN4 LL_GPIO_PIN_15

void ULN_Init(void);
void ULN_Run(int _ms, int _dir);

#endif