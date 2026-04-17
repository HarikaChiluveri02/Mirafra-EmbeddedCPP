#include <cstdint>
#include <iostream>
#include "stm32f407.h"
using namespace std;

void delayMs(int n)
{
    for (; n > 0; n--)
        for (volatile int i = 0; i < 2000; i++);
}
void USART3_init()
{
	//Enable the clock for USART3 and GPIOC
	RCC->AHB1ENR |= 1<<2;    //GPIOC
	RCC->AHB1ENR |= 1<<3;    //GPIOD for led pins
	RCC->APB1ENR |= 1<<18;   //USART3


	//cfg pd12 and pd14 for leds
	GPIOD->MODER &=~(3<<24);  //PD12 -> bit25 bit24
	GPIOD->MODER |=(1<<24);   //OUTPUT FUNCTION : 0 1 (bit25 bit24)

	GPIOD->MODER &=~(3<<28);  //PD14 -> bit29 bit28
	GPIOD->MODER |=(1<<28);   //OUTPUT FUNCTION : 0 1 (bit29 bit18)


	//PC10 for tx
	GPIOC->MODER &= ~(3<<20);   //pc10 ->bit 20 21
	GPIOC->MODER |= (1<<21);    //Alternate function : 1 0 (bit21 bit20)
	GPIOC->AFR[1] &= ~(0xf<<8); //AFRH->bit 8-11 for pc10
	GPIOC->AFR[1] |= (0x7<<8);  //Alternative function-7:0 1 1 1  (bit11 bit10 bit9 bit8)

	//PC11 for rx
	GPIOC->MODER &= ~(3<<22);    //pc10 ->bit 22 23
	GPIOC->MODER |= (1<<23);     //Alternate function : 1 0 (bit23 bit22)
	GPIOC->AFR[1] &= ~(0xf<<12); //AFRH->bit 12-15 for pc11
	GPIOC->AFR[1] |= (0x7<<12);  //Alternative function-7:0 1 1 1  (bit11 bit10 bit9 bit8)

	//USART3 SETTINGS
	USART3->BRR = 0x0683;  //9600 Baud rate @16MHz(HSI) with 16 samples
	USART3->DR = 0;
	USART3->CR1 |= 1<<3;    //TE
	USART3->CR1 |= 1<<2;    //RE
	USART3->CR1 |= 1<<13;   //Enable USART
}


void on(uint8_t pin)
{
	GPIOD->ODR |=(1<<pin);
}
void off(uint8_t pin)
{
	GPIOD->ODR&=~(1<<pin);
}
void toggle(uint8_t pin)
{
	GPIOD->ODR ^=(1<<pin);
}




int main()
{
	USART3_init();
	printf("Printing from uart_write\n");

	while(1)
	{
		toggle(12);
		delayMs(500);
		printf("Green led is toggling\n");
		delayMs(500);
		toggle(14);
		delayMs(500);
		printf("red led is toggling\n");

	}
}










