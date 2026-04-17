#include <stdint.h>
#include "stm32f407.h"

int main(void)
{
    // Enable clocks
    RCC->AHB1ENR |= (1 << 0); // GPIOA
    RCC->AHB1ENR |= (1 << 3); // GPIOD

    // PA0 as input
    GPIOA->MODER &= ~(3 << 0);

    // PD12 as output
    GPIOD->MODER &= ~(3 << 24);
    GPIOD->MODER |=  (1 << 24);

    while (1)
    {
        uint8_t sensor = (GPIOA->IDR & (1 << 0)) ? 1 : 0;

        if(sensor == 0)   // Object detected
        {
            GPIOD->ODR |= (1 << 12);  // LED ON
        }
        else
        {
            GPIOD->ODR &= ~(1 << 12); // LED OFF
        }
    }
}
