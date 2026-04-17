#include "led.h"

LED::LED(GPIO_TypeDef *GPIOx,uint8_t p,uint32_t v,uint32_t c,LED_State s)
{
    pGPIO = GPIOx;
    pin = p;
    voltage = v;
    current = c;
    state = s;

    if(pGPIO == GPIOD)
        RCC->AHB1ENR |= (1<<3);

    pGPIO->MODER &= ~(3<<(pin*2));
    pGPIO->MODER |=  (1<<(pin*2));
}

void LED::on()
{
    pGPIO->ODR |= (1<<pin);
    state = ON;
}

void LED::off()
{
    pGPIO->ODR &= ~(1<<pin);
    state = OFF;
}

void LED::toggle()
{
    pGPIO->ODR ^= (1<<pin);

    if(state == ON)
        state = OFF;
    else
        state = ON;
}

void LED::delay()
{
    for(volatile uint32_t i=0;i<300000;i++);
}

uint32_t LED::power()
{
    return (voltage * current);
}

void LED::reset()
{
    voltage = 0;
    current = 0;
}
