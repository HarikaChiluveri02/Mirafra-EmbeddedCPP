#include <cstdint>
#include <cstdio>
#include "stm32f407.h"

void GPIO_init()
{
	RCC->AHB1ENR|=(1<<3); //GPIOD clock

	GPIOD->MODER &=~(3<<24);  //CLEAR
	GPIOD->MODER |=(1<<24);  //CFG AS OUTPUT
}

void USART2_Init()
{
	RCC->AHB1ENR |= (1<<0);   // GPIOA clock
	RCC->APB1ENR |= (1<<17);  // USART2 clock

	GPIOA->MODER &= ~(3<<(2*2));
	GPIOA->MODER |=  (2<<(2*2));   // PA2 AF mode

	GPIOA->MODER &= ~(3<<(3*2));
	GPIOA->MODER |=  (2<<(3*2));   // PA3 AF mode

	GPIOA->AFR[0] |= (7<<(2*4));   // AF7 for USART
	GPIOA->AFR[0] |= (7<<(3*4));

	USART2->BRR = 0x0683;  // Baudrate 9600 (example)
	USART2->CR1 |= (1<<2); // RE → Receiver enable
	USART2->CR1 |= (1<<3); // TE → Transmitter enable
	USART2->CR1 |= (1<<13); // UE → USART enable

	USART2->CR1 |= (1<<5);   // RXNEIE → RX interrupt enable
	*(volatile uint32_t*)0xE000E100 |= (1<<38); // USART2 IRQ enable
}
extern "C" void USART2_IRQHandler(void)
{
    if(USART2->SR & (1<<5))      // RXNE flag
    {
        char data = USART2->DR;  // Read received character

        if(data == 'A')
        {
            GPIOD->ODR ^= (1<<12);  // Toggle LED
        }
    }
}
void USART2_write(uint8_t ch)
{
	while(!(USART2->SR & (1<<7)));
	USART2->DR=ch;
	while(!(USART2->SR &(1<<6)));
}
int main()
{
	GPIO_init();
	USART2_Init();
	while(1)
	{

	}
}









