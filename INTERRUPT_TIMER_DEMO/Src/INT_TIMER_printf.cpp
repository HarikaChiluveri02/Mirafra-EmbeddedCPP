#include <stdint.h>
#include "stm32f407.h"

inline volatile uint32_t *NVIC_ISER0 = reinterpret_cast<volatile uint32_t*>(0xE000E100);
inline volatile uint32_t *NVIC_ICPR0 = reinterpret_cast<volatile uint32_t*>(0xE000E280);

void LED_Init(void)
{
    RCC->AHB1ENR |= (1 << 3);        // Enable GPIOD clock

    GPIOD->MODER &= ~(3 << (12 * 2));
    GPIOD->MODER |=  (1 << (12 * 2));   // PD12 output

    GPIOD->ODR &= ~(1 << 12);
}

void TIM3_Init(void)
{
    RCC->APB1ENR |= (1 << 1);        // Enable TIM3 clock

    TIM3->PSC = 1599;
    TIM3->ARR = 19999;

    TIM3->CNT = 0;

    TIM3->EGR |= (1 << 0);           // update event
    TIM3->SR  &= ~(1 << 0);          // clear flag

    TIM3->DIER |= (1 << 0);          // enable update interrupt

    *NVIC_ICPR0 |= (1 << 29);        // clear pending interrupt
    *NVIC_ISER0 |= (1 << 29);        // enable TIM3 interrupt

    TIM3->CR1 |= (1 << 0);           // start timer
}


extern "C" void TIM3_IRQHandler(void)
{
    if(TIM3->SR & (1 << 0))
    {
        TIM3->SR &= ~(1 << 0);     // clear interrupt flag

        GPIOD->ODR ^= (1 << 12);   // toggle LED

        printf("TIM3 interrupt fired — 2 second!\r\n");
    }
}


int main(void)
{
    LED_Init();
    USART3_Init();
    TIM3_Init();

    printf("System started!\r\n");
    printf("TIM3 interrupt every 2 second...\r\n");

    while(1)
    {

    }
}
