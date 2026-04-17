#include <cstdint>
#include <iostream>
#include "stm32f407.h"
using namespace std;

void USART2_init()
{
	//Enable clock for usart2 and gpioa
	RCC->APB1ENR|= 1<<17; //enable usart2
	RCC->AHB1ENR|= 1<<0;  //enable gpioa

	//PA2 (Tx)
	GPIOA->MODER &=~(3<<4);        //pA2 ->bit 5 4
	GPIOA->MODER |=(1<<5);        //Alternate function : 1 0 (bit5 bit4)
	GPIOA->AFR[0] &=~(0XF<<8);   //AFRH->bit 8-11 for pA2
	GPIOA->AFR[0] |=(0X7<<8);    //Alternative function-7:0 1 1 1  (bit11 bit10 bit9 bit8)

	//PA3 (Rx)
	GPIOA->MODER &=~(3<<6);     //pA3 ->bit 7 6
	GPIOA->MODER |=(1<<7);      //Alternate function : 1 0 (bit7 bit6)
	GPIOA->AFR[0] &=~(0XF<<12);  //AFRH->bit 12-15 for pc11
	GPIOA->AFR[0] |=(0X7<<12);   //Alternative function-7:0 1 1 1  (bit11 bit10 bit9 bit8)

	//USART2 SETTINGS
	USART2->BRR = 0x0683;  //9600 Baud rate @16MHz(HSI) with 16 samples
	USART2->DR = 0;
	USART2->CR1 |= 1<<3;    //TE
	USART2->CR1 |= 1<<2;    //RE
	USART2->CR1 |= 1<<13;   //Enable USART
}

void USART2_write(uint8_t ch)
{
    // Wait until DR empty
    while (!(USART2->SR & (1 << 7))); // TXE =0 while(!0) - while loop(true)
    //TXE=1 Data in DR register will be given to shift register

    USART2->DR = ch; // new data will be loaded to DR

    // Wait until transmission complete
    while (!(USART2->SR & (1 << 6))); // TC=0 while(!0) while(T) waiting
}
void delayMs(int n)
{
    for (; n > 0; n--)
        for (volatile int i = 0; i < 2000; i++);
}

int main(void)
{
    USART2_init();   // Initialize UART

   while (1)
    {
        USART2_write('H');// 1 byte
        delayMs(500);
       /* USART3_write_str("Harika");
        delayMs(500);*/
   }
}
