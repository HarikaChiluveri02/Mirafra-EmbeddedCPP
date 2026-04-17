#include <cstdint>
#include "stm32f407.h"
#include <iostream>
using namespace std;

enum class RCC_AHB1ENR_bits : uint32_t
{
GPIOA_EN =0,
GPIOB_EN =1,
GPIOC_EN =2,
GPIOD_EN =3
};

enum class GPIO_mode : uint32_t
{
INPUT  =0b00,
OUTPUT =0b01,
ALT    =0b10,
ANALOG =0b11
};

enum class Color :uint32_t
{
GREEN = 12,
ORANGE,
RED,
BLUE
};

//==============================
//  RAII: GPIO Clock Guard
// ==============================
class GPIOClockGuard
{
public:
    GPIOClockGuard()
    {
        RCC->AHB1ENR |= (1U << static_cast<uint32_t>(RCC_AHB1ENR_bits::GPIOD_EN)); // Enable GPIOD
        RCC->AHB1ENR |= (1U << static_cast<uint32_t>(RCC_AHB1ENR_bits::GPIOA_EN)); // Enable GPIOA
    }

    ~GPIOClockGuard()
    {
        RCC->AHB1ENR &= ~(1U << static_cast<uint32_t>(RCC_AHB1ENR_bits::GPIOD_EN)); // Disable GPIOD
        RCC->AHB1ENR &= ~(1U << static_cast<uint32_t>(RCC_AHB1ENR_bits::GPIOA_EN)); // Disable GPIOA
    }
};

// ==============================
//  GPIO Helper
// ==============================
class GPIO
{
public:

    static void set_output(Color pin)
    {
    	uint32_t p=static_cast<uint32_t>(pin);
        GPIOD->MODER &= ~(3U << (p * 2));
        GPIOD->MODER |=  (static_cast<std::uint32_t>(GPIO_mode::OUTPUT)<< (p * 2));
    }

    static void toggle(Color pin)
    {
        GPIOD->ODR ^= (1U << static_cast<uint32_t>(pin));
    }
    static void on(uint8_t pin)
    {
    	GPIOD->ODR|=(1U<<pin);
    }
    static void off(uint8_t pin)
    {
    	GPIOD->ODR&=~(1U<<pin);
    }
    static void set_input(uint8_t pin)
    {
    	GPIOA->MODER &=~(3U<<(pin*2));
    	GPIOA->MODER |=  (static_cast<std::uint32_t>(GPIO_mode::INPUT)<< (pin * 2));
    }
    static int read()
    {
    	return (GPIOA->IDR&1);
    }

};
// ==============================
//  Delay
// ==============================
void delay()
{
    for (std::uint32_t i = 0; i < 100000; ++i)
    {
        // Prevent optimization
    }
}

int main()
{
	GPIOClockGuard g;
	GPIO::set_output(Color::RED);
	GPIO::set_output(Color::GREEN);
    GPIO::set_input(0);

		while(true)
		{
			if(GPIO::read())
			{
				break;
			}
			GPIO::toggle(Color::RED);
			GPIO::toggle(Color::GREEN);
			delay();
		}
}



