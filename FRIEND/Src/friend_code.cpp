#include <stdint.h>
#include "stm32f407.h"


class USART_Driver
{
public:

    void init()
    {
        RCC->AHB1ENR |= (1<<2);     // GPIOC clock
        RCC->APB1ENR |= (1<<18);    // USART3 clock

        GPIOC->MODER &= ~(3<<(10*2));
        GPIOC->MODER |=  (2<<(10*2));    // AF mode

        GPIOC->AFR[1] &= ~(0xF<<8);
        GPIOC->AFR[1] |=  (7<<8);        // AF7

        USART3->BRR = 0x0683;            // 9600 @16MHz

        USART3->CR1 |= (1<<3);           // TE
        USART3->CR1 |= (1<<13);          // UE
    }

    void write_char(char ch)
    {
        while(!(USART3->SR & (1<<7)));
        USART3->DR = ch;
        while(!(USART3->SR & (1<<6)));
    }

    void write_string(const char *str)
    {
        while(*str)
        {
            write_char(*str++);
        }
    }
};



class LED
{
private:
    uint32_t pin;

public:

    LED(uint32_t p)
    {
        pin = p;
    }

    void init()
    {
        RCC->AHB1ENR |= (1<<3);   // GPIOD clock

        GPIOD->MODER &= ~(3<<(pin*2));
        GPIOD->MODER |=  (1<<(pin*2));
    }

    void toggle()
    {
        GPIOD->ODR ^= (1<<pin);
    }

    friend void Timer_ISR_Helper();
};


class Timer
{
public:

    void init()
    {
        RCC->APB1ENR |= (1<<0);     // TIM2 clock

        TIM2->PSC = 1599;
        TIM2->ARR = 9999;           // ~500ms

        TIM2->DIER |= (1<<0);       // interrupt enable

        TIM2->EGR |= (1<<0);
        TIM2->SR  &= ~(1<<0);

        *((volatile uint32_t*)0xE000E100) |= (1<<28);

        TIM2->CR1 |= (1<<0);        // start timer
    }
};


/* ================= GLOBAL OBJECTS ================= */

LED led(12);
USART_Driver usart;
Timer timer;

/* interrupt counter */
volatile uint32_t interrupt_count = 0;


/* ================= FRIEND FUNCTION ================= */

void Timer_ISR_Helper()
{
    interrupt_count++;

    led.toggle();

    if(interrupt_count % 5 == 0)
    {
        usart.write_string("5 Timer Interrupts Completed\r\n");
    }
}


/* ================= INTERRUPT HANDLER ================= */

extern "C" void TIM2_IRQHandler()
{
    if(TIM2->SR & (1<<0))
    {
        TIM2->SR &= ~(1<<0);

        Timer_ISR_Helper();
    }
}


/* ================= MAIN ================= */

int main()
{
    led.init();

    usart.init();

    timer.init();

    usart.write_string("System Booted\r\n");

    while(1)
    {

    }
}
