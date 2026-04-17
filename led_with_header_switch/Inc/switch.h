#ifndef SWITCH_H_
#define SWITCH_H_

#include "stm32f407.h"
#include <stdint.h>

class SWITCH
{
private:
    GPIO_TypeDef *pGPIO;
    uint8_t pin;

public:

    SWITCH(GPIO_TypeDef *GPIOx,uint8_t p);

    uint8_t read();
};

#endif
