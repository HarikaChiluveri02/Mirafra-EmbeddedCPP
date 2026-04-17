#include "switch.h"

SWITCH::SWITCH(GPIO_TypeDef *GPIOx,uint8_t p)
{
    pGPIO = GPIOx;
    pin = p;

    if(pGPIO == GPIOA)
        RCC->AHB1ENR |= (1<<0);

    pGPIO->MODER &= ~(3<<(pin*2));
}

uint8_t SWITCH::read()
{
    if((pGPIO->IDR >> pin) & 1)
        return 1;
    else
        return 0;
}
