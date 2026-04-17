#include <cstdint>
#include "stm32f407.h"

char str[50];
char ch;

void delay()
{
	for(volatile uint32_t i=0;i<200000;i++)
	{
		__asm volatile("nop");
	}
}
void gpiod_init()
{
	//cfg pd12 - pd15 for leds
		GPIOD->MODER &=~(3<<24);  //PD12 -> bit25 bit24
		GPIOD->MODER |=(1<<24);   //OUTPUT FUNCTION : 0 1 (bit25 bit24)

		GPIOD->MODER &=~(3<<26);
		GPIOD->MODER |=(1<<26);

		GPIOD->MODER &=~(3<<28);  //PD14 -> bit29 bit28
		GPIOD->MODER |=(1<<28);   //OUTPUT FUNCTION : 0 1 (bit29 bit18)

		GPIOD->MODER &=~(3<<30);
		GPIOD->MODER |=(1<<30);
}

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

void USART3_init()
{
	//Enable the clock for USART3 and GPIOC
	RCC->AHB1ENR |= 1<<2;    //GPIOC
	RCC->AHB1ENR |= 1<<3;    //GPIOD for led pins
	RCC->APB1ENR |= 1<<18;   //USART3

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
	GPIOD->ODR|=(1<<pin);
}
void off(uint8_t pin)
{
	GPIOD->ODR &=~(1<<pin);
}
void toggle(uint8_t pin)
{
	GPIOD->ODR^=(1<<pin);
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
void USART2_write_str(const char* str)
{
	while(*str)
	{
		USART2_write(*str++);
		delay();
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
    while(1)
    {
    	ch = USART3_read();
    	if(ch=='R' || ch=='r')
    	{
    		toggle(14);
    		delay();
    	}
    	else if(ch=='G' || ch=='g')
    	{
    		toggle(12);
    		delay();
    	}
    	else if(ch=='O' || ch=='o')
    	{
    		toggle(13);
    		delay();
    	}
    	else if(ch=='B' || ch=='b')
    	{
    		toggle(15);
    		delay();
    	}

    	if(ch == '\r' || ch == '\n')   // Enter key pressed
        {
            buffer[i] = '\0';   // string end
            break;
        }
    	buffer[i++] = ch;
    }
}

int main()
{
	USART2_init();
	USART3_init();
	gpiod_init();

	while(1)
	{
		USART2_write_str("HARIKA\n");
		USART3_read_str(str);
	}

}















