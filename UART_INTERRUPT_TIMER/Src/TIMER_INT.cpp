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

void USART3_Init(void)
{
    RCC->AHB1ENR |= (1 << 2);        // GPIOC clock
    RCC->APB1ENR |= (1 << 18);       // USART3 clock

    GPIOC->MODER &= ~(3 << 20);
    GPIOC->MODER |=  (2 << 20);      // PC10 alternate function

    GPIOC->AFR[1] &= ~(0xF << 8);
    GPIOC->AFR[1] |=  (7 << 8);      // AF7 USART3_TX

    USART3->BRR = 0x0683;            // 9600 baud

    USART3->CR1 |= (1 << 3);         // TX enable
    USART3->CR1 |= (1 << 13);        // USART enable
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

void USART3_write(uint8_t ch)
{
    while(!(USART3->SR & (1 << 7)));
    USART3->DR = ch;

    while(!(USART3->SR & (1 << 6)));
}

void USART3_write_string(const char* str)
{
    while(*str)
    {
        USART3_write(*str++);
    }
}

extern "C" void TIM3_IRQHandler(void)
{
    if(TIM3->SR & (1 << 0))
    {
        TIM3->SR &= ~(1 << 0);     // clear interrupt flag

        GPIOD->ODR ^= (1 << 12);   // toggle LED

        USART3_write_string("TIM3 interrupt fired — 2 second!\r\n");
    }
}

int main(void)
{
    LED_Init();
    USART3_Init();
    TIM3_Init();

    USART3_write_string("System started!\r\n");
    USART3_write_string("TIM3 interrupt every 2 second...\r\n");

    while(1)
    {

    }
}
