#include<stdint.h>
#include "stm32f407.h"

uint8_t status;
static uint8_t counter;

// PA0 Switch
//void count_action(void);
void delay_ms(uint32_t time)
{
    while(time--)
    {
        for(volatile uint32_t i = 0; i < 5000; i++);
    }
}

int main(void)
{

	        RCC->AHB1ENR |= (1 << 3); // RCC->AHB1ENR set bit 3 to enable GPIOD clock
		    RCC->AHB1ENR |= (1 << 0); // RCC->AHB1ENR set bit 0 to enable GPIOA clock

		    GPIOD->MODER &= ~((3 << 24) | (3 << 26) | (3 << 28) | (3 << 30)); // cleared the bits 25 and 14,26 and 27,28 and 29, 30 and 31
		    GPIOD->MODER |= ((1 << 24) | (1 << 26) | (1 << 28) | (1 << 30));
		    GPIOA->MODER &= ~(3 << 0); // input(reset mode) -00,PA0 pin0 bits are 0 & 1

	//gpio_init();


		    uint8_t prev_status = 0;

		    while (1)
		    {
		        status = GPIOA->IDR & (1 << 0);

		        if ((status == 1) && (prev_status == 0))
		        {
		            delay_ms(20);  // debounce

		            if (GPIOA->IDR & (1 << 0))
		            {
		                counter++;
		            }
		        }

		        prev_status = status;
		    }
}
