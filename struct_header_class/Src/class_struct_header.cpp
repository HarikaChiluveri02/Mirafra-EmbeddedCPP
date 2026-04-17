#include <stdint.h>
#include <iostream>
using namespace std;
typedef struct
{
    volatile uint32_t MODER; //4 byte 0x00
    volatile uint32_t OTYPER;// 0x04
    volatile uint32_t OSPEEDR;
    volatile uint32_t PUPDR;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
} GPIO_RegDef_t;

typedef struct
{
    volatile uint32_t CR;
    volatile uint32_t PLLCFGR;
    volatile uint32_t CFGR;
    volatile uint32_t CIR;
    volatile uint32_t AHB1RSTR;
    volatile uint32_t AHB2RSTR;
    volatile uint32_t AHB3RSTR;
    uint32_t RESERVED0;
    volatile uint32_t APB1RSTR;
    volatile uint32_t APB2RSTR;
    uint32_t RESERVED1[2];
    volatile uint32_t AHB1ENR;
} RCC_RegDef_t;

#define RCC_BASE   0x40023800
#define GPIOD_BASE 0x40020C00
#define GPIOA_BASE 0x40020000

#define RCC   ((RCC_RegDef_t*) RCC_BASE)// type casting
#define GPIOD ((GPIO_RegDef_t*) GPIOD_BASE)
#define GPIOA ((GPIO_RegDef_t*) GPIOA_BASE)

class LED
{
GPIO_RegDef_t *pGPIO;
uint8_t pin;
public:
LED(GPIO_RegDef_t* GPIO,uint8_t p)
{
pGPIO=GPIO;
pin=p;
if(pGPIO==GPIOD)
{
RCC->AHB1ENR|=(1<<3);
}
pGPIO->MODER&=~(3<<(pin*2));
pGPIO->MODER|=(1<<(pin*2));

}
void Ledon()
{
pGPIO->ODR|=(1<<pin);
}
void Ledoff()
{
pGPIO->ODR&=~(1<<pin);
}
void Toggle()
{
pGPIO->ODR^=(1<<pin);
}
void delay()
{
for(volatile uint32_t i=0;i<300000;i++);
}
};
class SWITCH
{
	uint8_t p;
	GPIO_RegDef_t* pGPIO;
public:
	SWITCH(GPIO_RegDef_t* gpio,uint8_t pin)
	{
		pGPIO=gpio;
		p=pin;

		if(pGPIO==GPIOA)
			RCC->AHB1ENR|=(1<<0); //Enable GPIOA

		pGPIO->MODER&=~(3<<(p*2));
	}
	int read()
	{
		return ((pGPIO->IDR>>p)&1);
	}
};

int main()
{
LED led12(GPIOD,12);
LED led14(GPIOD,14);
SWITCH s(GPIOA,0);
while(1)
{
	if(s.read())
	{
		led14.Ledoff();
		led12.Toggle();
		led12.delay();
    }
	else
	{
		led12.Ledoff();
		led14.Toggle();
		led14.delay();
	}
}
}
