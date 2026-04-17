#include <cstdint>
#include "stm32f407.h"
#include <iostream>
using namespace std;

//==============================
//  RAII: GPIO Clock Guard
// ==============================
class GPIOClockGuard
{
public:
    GPIOClockGuard()
    {
        RCC->AHB1ENR |= (1U << 3); // Enable GPIOD
        RCC->AHB1ENR |= (1U << 0); // Enable GPIOA
    }

    ~GPIOClockGuard()
    {
        RCC->AHB1ENR &= ~(1U << 3); // Disable GPIOD
        RCC->AHB1ENR &= ~(1U << 0); // Disable GPIOA
    }
};

// ==============================
//  GPIO Helper
// ==============================
class GPIO
{
public:

    static void set_output(uint8_t pin)
    {
        GPIOD->MODER &= ~(3U << (pin * 2));
        GPIOD->MODER |=  (1U << (pin * 2));
    }

    static void toggle(uint8_t pin)
    {
        GPIOD->ODR ^= (1U << pin);
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
    }
    static int read()
    {
    	return (GPIOA->IDR&1);
    }

};
/*
class GPIO
{
public:
    static void set_output(volatile std::uint32_t& moder, std::uint32_t pin)
    {
        moder &= ~(3U << (pin * 2));
        moder |=  (1U << (pin * 2)); // Output mode
    }

    static void set_input(volatile std::uint32_t& moder, std::uint32_t pin)
    {
        moder &= ~(3U << (pin * 2)); // Input mode (00)
    }

    static void toggle(volatile std::uint32_t& odr, std::uint32_t pin)
    {
        odr ^= (1U << pin);
    }

    static bool read(volatile std::uint32_t& idr, std::uint32_t pin)
    {
        return (idr & (1U << pin));
    }
};
*/

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
	constexpr uint8_t LED_PIN = 12;
	constexpr uint8_t SW_PIN = 0;

	{
		GPIOClockGuard g;

		GPIO::set_output(LED_PIN);
		GPIO::set_input(SW_PIN);

		while(true)
		{
			if(GPIO::read())
			{
				break;
			}
			GPIO::toggle(LED_PIN);
			delay();
		}
	}

}

/*
 int main()
 {
    constexpr uint8_t LED_PIN = 12;
	constexpr uint8_t SW_PIN = 0;

	{
		GPIOClockGuard g;

		GPIO::set_output(GPIOD->ODR,LED_PIN);
		GPIO::set_input(GPIOA->IDR,SW_PIN);

		while(true)
		{

		if(GPIO::read(GPIOA->IDR,SW_PIN)))
		{
		break;
		}

		GPIO::toggle(GPIOD->ODR,LED_PIN);
		delay();
		}
	}
 }
 */

