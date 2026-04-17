#include <cstdint>
#include <iostream>
#include "stm32f407.h"

using namespace std;

class GPIO
{
public:
	static void clock_enable()
	{
		RCC->AHB1ENR|=1U<<3;
		RCC->AHB1ENR|=1U<<0;
	}

	static void set_output(uint8_t pin)
	{
		GPIOD->MODER &= ~(3U << (pin * 2));
		GPIOD->MODER |=  (1U << (pin * 2));
	}
	static void set_input(uint8_t pin)
	{
		GPIOA->MODER &=~(3U<<(pin*2));
	}
	static void set(uint32_t pin)
	{
		GPIOD->BSRR|=(1<<pin);
	}
	static void reset(uint32_t pin)
	{
	    GPIOD->BSRR|=(1<<(16+pin));
	}
	static void toggle(std::uint32_t pin)
	{
		if (GPIOD->ODR & (1U << pin))
		{
	    		 reset(pin);
	    }
		else
	    	 {
	    		 set(pin);
	    	 }
	 }
	static uint32_t read()
	{
		return(( GPIOA->IDR>>0)&1);
	}
};

class led
{

	uint8_t* led_pin;
public:
	led(uint8_t pin)
	{
		led_pin=new uint8_t(pin);
	}
	void toggle()
	{
		GPIO::toggle(*led_pin);
	}
	void on()
	{
		GPIO::set(*led_pin);
	}
	void off()
	{
		GPIO::reset(*led_pin);
	}
	~led()
	{
		delete led_pin;
	}
};

void delay()
{
	for(volatile uint32_t i=0;i<100000;i++)
	{
		__asm volatile("nop");
	}
}
int main()
{
	constexpr uint8_t LED_PIN=13;
	constexpr uint8_t sw_pin=0;
	GPIO::clock_enable();
	GPIO::set_output(LED_PIN);
	GPIO::set_input(sw_pin);
	{
	led LED(LED_PIN);

	while(true)
	{
		if(GPIO::read())
		{
			break;
		}
		LED.toggle();
		delay();
	}
	}
}



















