#include <cstdint>
#include "stm32f407.h"
using namespace std;

class GPIO
{
public:
    static void enable_clock()
    {
        RCC->AHB1ENR |= (1U << 3);  // Enable GPIOD clock
        RCC->AHB1ENR |= (1U<<0);
    }

    static void set_output(std::uint32_t pin)
    {
        GPIOD->MODER &= ~(3U << (pin * 2));
        GPIOD->MODER |=  (1U << (pin * 2));
    }

    static void toggle(std::uint32_t pin)
    {
        GPIOD->ODR ^= (1U << pin);
    }
    static void on(uint32_t pin)
    {
    	GPIOD->ODR|=(1U<<pin);
    }
    static void off(uint32_t pin)
    {
    	GPIOD->ODR&=~(1U<<pin);
    }
    static void set_input(uint32_t pin)
    {
    	GPIOA->MODER &=~(3U<<(pin*2));
    }
    static int read()
    {
    	return (GPIOA->IDR&1);
    }
};
template<uint8_t pin>
class LED
{
public:
	static LED& instance()noexcept
	{
		static LED instance;
		return instance;
	}

	LED(const LED&)=delete;
	LED& operator=(const LED&)=delete;

	LED(LED&&)=delete;
	LED& operator=(LED&&)=delete;

    void toggle()
    {
        GPIO::toggle(pin);
    }
    void on()noexcept
    {
    	GPIO::on(pin);
    }
    void off()noexcept
    {
    	GPIO::off(pin);
    }

private:
    LED()
    {
    	GPIO::enable_clock();
    	GPIO::set_output(pin);
    }
};

void delay()
{
	for(volatile uint32_t i=0;i<100000;i++)
	{
		__asm volatile("NOP");
	}
}

int main()
{
	auto& led1=LED<12>::instance();
	auto& led2=LED<12>::instance();
	auto& led3=LED<14>::instance();

	while(true)
	{
		led1.on();
		delay();

		led2.off();
		delay();

		led1.on();
		delay();

		led3.on();
		delay();
		led2.off();
	}
}



















