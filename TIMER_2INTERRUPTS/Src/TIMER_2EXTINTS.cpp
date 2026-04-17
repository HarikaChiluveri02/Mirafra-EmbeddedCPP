#include <cstdint>
#include "stm32f407.h"
#include <cstdio>

inline volatile uint32_t *NVIC_ISER0 = reinterpret_cast<volatile uint32_t*>(0xE000E100);
inline volatile uint32_t *NVIC_ICPR0 = reinterpret_cast<volatile uint32_t*>(0xE000E280);

void LED_INIT()
{
	RCC->AHB1ENR|=(1<<3); ///GPIOD

	GPIOD->MODER&=~(3<<24); //PD12
	GPIOD->MODER|=(1<<24); //OUTPUT MODE

	GPIOD->ODR&=~(1<<12);
}

void TIM3_Init()
{
    RCC->APB1ENR |= (1 << 1);        // Enable TIM3 clock

    TIM3->PSC = 1599;
    TIM3->ARR = 9999;

    TIM3->CNT = 0;

    TIM3->EGR |= (1 << 0);           // update event
    TIM3->SR  &= ~(1 << 0);          // clear flag

    TIM3->DIER |= (1 << 0);          // enable update interrupt

    *NVIC_ICPR0 |= (1 << 29);        // clear pending interrupt
    *NVIC_ISER0 |= (1 << 29);        // enable TIM3 interrupt

}
void Button_EXTI0_Init()
{
    /* Enable clocks */
    RCC->AHB1ENR |= (1 << 0);   // GPIOA
    RCC->APB2ENR |= (1 << 14);  // SYSCFG

    /* PA0 as input */
    GPIOA->MODER &= ~(3 << (0 * 2));

    /* Pull-down */
    GPIOA->PUPDR &= ~(3 << (0 * 2));
    GPIOA->PUPDR |=  (2 << (0 * 2));  // Pull-down

    /* Map EXTI0 to PA0 */
    SYSCFG->EXTICR[0] &= ~(0xF << 0);  // 0000 = PA0

    /* Rising edge trigger */
    EXTI->RTSR |= (1 << 0);

    /* Unmask interrupt */
    EXTI->IMR |= (1 << 0);

    /* NVIC Enable (IRQ6 for EXTI0) */
    *((volatile uint32_t*)0xE000E100) |= (1 << 6);
}


void Button_EXTI1_Init()
{
    /* Enable clocks */
    RCC->AHB1ENR |= (1 << 0);   // GPIOA
    RCC->APB2ENR |= (1 << 14);  // SYSCFG

    /* PA1 as input */
    GPIOA->MODER &= ~(3 << (1 * 2));

    /* Pull-down */
    GPIOA->PUPDR &= ~(3 << (1 * 2));
    GPIOA->PUPDR |=  (2 << (1 * 2));  // Pull-down

    /* Map EXTI0 to PA0 */
    SYSCFG->EXTICR[0] &= ~(0xF << 4);  // 0000 = PA1 for EXTI1

    /* Rising edge trigger */
    EXTI->RTSR |= (1 << 1); //EXTI1

    /* Unmask interrupt */
    EXTI->IMR |= (1 << 1);  //EXTI1

    /* NVIC Enable (IRQ7 for EXTI1) */
    *((volatile uint32_t*)0xE000E100) |= (1 << 7);
}

extern "C" void EXTI0_IRQHandler(void)
{
    if (EXTI->PR & (1 << 0))  // Check pending
    {

    	 TIM3->CR1 |= (1 << 0);           // start timer

        EXTI->PR |= (1 << 0); // Clear pending
    }
}
extern "C" void EXTI1_IRQHandler(void)
{
    if (EXTI->PR & (1 << 1))  // Check pending
    {
    	printf("TIM3 stopped\r\n");
    	GPIOD->ODR &= ~(1 << 12);   // toggle LED
    	TIM3->CR1 &= ~(1 << 0);           // stop timer

        EXTI->PR |= (1 << 1); // Clear pending
    }
}
extern "C" void TIM3_IRQHandler(void)
{
    if(TIM3->SR & (1 << 0))
    {
    	 printf("TIM3 started\r\n");
    	 GPIOD->ODR ^= (1 << 12);   // toggle LED

    	  TIM3->SR &= ~(1 << 0);     // clear interrupt flag
    }
}

int main()
{
	LED_INIT();
	Button_EXTI0_Init();
	Button_EXTI1_Init();
	TIM3_Init();

	while(1)
	{

	}
}











