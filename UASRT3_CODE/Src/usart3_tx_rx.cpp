#include <cstdint>
#include "stm32f407.h"
#include <iostream>
using namespace std;

char read_ch;
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
	GPIOC->MODER &= ~(3<<22);    //pc11 ->bit 22 23
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

void USART3_write(uint8_t ch)
{
    // Wait until DR empty
    while (!(USART3->SR & (1 << 7))); // TXE =0 while(!0) - while loop(true)
    //TXE=1 Data in DR register will be given to shift register

    USART3->DR = ch; // new data will be loaded to DR

    // Wait until transmission complete
    while (!(USART3->SR & (1 << 6))); // TC=0 while(!0) while(T) waiting
}
void USART3_write_str(const char* str)
{
	while(*str)
	{
		USART3_write(*str++);
		delayMs(500);
	}
}
uint8_t USART3_read(void)
{
	while (!(USART3->SR & (1 << 5))); // RXNE
   return USART3->DR;
}
void USART3_read_str(char *buffer)
{
    int i = 0;
    char ch;

    while(1)
    {
    	ch = USART3_read();
    	if(ch == '\r' || ch == '\n')   // Enter key pressed
        {
            buffer[i] = '\0';   // string end
            break;
        }
    	buffer[i++] = ch;
    }
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
/*
int main(void)
{
    USART3_init();   // Initialize UART

   while (1)
    {
        USART3_write('H');// 1 byte
        delayMs(500);
       USART3_write_str("Harika");
        delayMs(500);
   }
}*/
/*

int main()
{
    USART3_init();
    char str[50];
    while(1)
    {
        USART3_write_str("Enter name: ");
        USART3_read_str(str);
        USART3_write_str("\nYou typed: ");
        USART3_write_str(str);
        USART3_write_str("\n");
    }
}

*/

int main()
{
	USART3_init();
	while(1)
	{
		USART3_write('F');
		read_ch=USART3_read();
		if(read_ch == 'O')
		{
			toggle(12);
			off(14);
			delayMs(500);
		}
		else if(read_ch == 'F')
		{
			toggle(14);
			off(12);
			delayMs(500);
		}
		else
		{
			off(12);
			off(14);
			delayMs(500);
		}
	}
}





















