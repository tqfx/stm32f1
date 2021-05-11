
#include "ULN2003.h"

void ULN_Init(void)
{
    LL_GPIO_SetOutputPin(ULN_PORT1, ULN_PIN1);
    LL_GPIO_SetOutputPin(ULN_PORT2, ULN_PIN2);
    LL_GPIO_SetOutputPin(ULN_PORT3, ULN_PIN3);
    LL_GPIO_SetOutputPin(ULN_PORT4, ULN_PIN4);
}

void ULN_Run(int _ms, int _dir)
{
    if (_dir)
    {
        LL_GPIO_SetOutputPin(ULN_PORT1, ULN_PIN1);
        LL_GPIO_ResetOutputPin(ULN_PORT2, ULN_PIN2);
        LL_mDelay(_ms);
        LL_GPIO_SetOutputPin(ULN_PORT2, ULN_PIN2);
        LL_GPIO_ResetOutputPin(ULN_PORT3, ULN_PIN3);
        LL_mDelay(_ms);
        LL_GPIO_SetOutputPin(ULN_PORT3, ULN_PIN3);
        LL_GPIO_ResetOutputPin(ULN_PORT4, ULN_PIN4);
        LL_mDelay(_ms);
        LL_GPIO_SetOutputPin(ULN_PORT4, ULN_PIN4);
        LL_GPIO_ResetOutputPin(ULN_PORT1, ULN_PIN1);
        LL_mDelay(_ms);
    }
    else
    {
        LL_GPIO_SetOutputPin(ULN_PORT1, ULN_PIN1);
        LL_GPIO_ResetOutputPin(ULN_PORT4, ULN_PIN4);
        LL_mDelay(_ms);
        LL_GPIO_SetOutputPin(ULN_PORT4, ULN_PIN4);
        LL_GPIO_ResetOutputPin(ULN_PORT3, ULN_PIN3);
        LL_mDelay(_ms);
        LL_GPIO_SetOutputPin(ULN_PORT3, ULN_PIN3);
        LL_GPIO_ResetOutputPin(ULN_PORT2, ULN_PIN2);
        LL_mDelay(_ms);
        LL_GPIO_SetOutputPin(ULN_PORT2, ULN_PIN2);
        LL_GPIO_ResetOutputPin(ULN_PORT1, ULN_PIN1);
        LL_mDelay(_ms);
    }
}