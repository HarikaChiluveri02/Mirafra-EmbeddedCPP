#include <cstdint>
#include <stdio.h>
#include "stm32f407.h"
using namespace std;

static uint8_t button_count;

void delay_ms(uint32_t time)
{
    while(time--)
    {
        for(volatile uint32_t i = 0; i < 5000; i++);
    }
}

void gpio_init()
{
	//GPIOA AND GPIOD CLOCK
	RCC->AHB1ENR |=(1<<0);
	RCC->AHB1ENR |=(1<<3);

    // PA0 → input
    GPIOA->MODER &= ~(3 << (0 * 2));

    // PD12–PD15 → output
    for(int i = 12; i <= 15; i++)
    {
        GPIOD->MODER &= ~(3 << (i * 2));
        GPIOD->MODER |=  (1 << (i * 2));
    }
}

class Observer
{
public:
	virtual void onButtonPressed()=0;
};

class Button
{
	Observer* observer[10];
	uint8_t pin;
	uint8_t count;
public:
	Button(uint8_t p):pin(p),count(0)
	{
		//constructor
	}
	void attach(Observer* obj)
	{
		if(count<10)
		{
			observer[count++]=obj;
		}
	}
	uint8_t read()
	{
		return ((GPIOA->IDR >> pin)&1)?1:0;
	}

	void poll()
	{
	    static uint8_t prev_status = 0;
	    uint8_t status = read();

	    if ((status == 1) && (prev_status == 0))
	    {
	        delay_ms(20);

	        if (GPIOA->IDR & (1 << 0))
	        {
	        	button_count++;
	            notify();
	        }
	    }

	    prev_status = status;
	}
private:
    void notify()
    {
        for(int i = 0; i < count; i++)
        {
            observer[i]->onButtonPressed();
        }
    }
};


class greenLed:public Observer
{
	uint8_t pin;
public:
	greenLed(uint8_t p):pin(p)
	{

	}
	void onButtonPressed() override
	{
		if(button_count%2==0 || button_count==10)
		{
		printf("Green Led ON\n");
		GPIOD->ODR |=(1<<pin);
		}
		else
		{
			GPIOD->ODR&=~(1<<pin);
		}
	}
};

class orangeLed:public Observer
{
	uint8_t pin;
public:
	orangeLed(uint8_t p):pin(p)
	{

	}
	void onButtonPressed() override
	{
		if(button_count%2!=0 || button_count==10)
		{
		printf("Orange Led ON\n");
		GPIOD->ODR |=(1<<pin);
		}
		else
		{
		GPIOD->ODR&=~(1<<pin);
		}
	}
};

class redLed:public Observer
{
	uint8_t pin;
public:
	redLed(uint8_t p):pin(p)
	{

	}
	void onButtonPressed() override
	{
		if(button_count%2==0 || button_count==10)
		{
		printf("red Led ON\n");
		GPIOD->ODR |=(1<<pin);
		}
		else
		{
		GPIOD->ODR&=~(1<<pin);
		}
	}
};

class blueLed:public Observer
{
	uint8_t pin;
public:
	blueLed(uint8_t p):pin(p)
	{

	}
	void onButtonPressed() override
	{
		if(button_count%2!=0 || button_count==10)
		{
		printf("Blue Led ON\n");
		GPIOD->ODR |=(1<<pin);
		}
		else
		{
			GPIOD->ODR&=~(1<<pin);
		}
	}
};


int main()
{
	gpio_init();

	Button b(0);
	greenLed g(12);
	orangeLed o(13);
	redLed r(14);
	blueLed bl(15);

	b.attach(&g);
	b.attach(&o);
	b.attach(&r);
	b.attach(&bl);

	 while(1)
	 {
		 b.poll();
	 }

}






























