#include <cstdint>
#include "stm32f407.h"

class GPIO
{
public:
    static void enable_clock()
    {
        RCC->AHB1ENR |= (1U << 3);
        RCC->AHB1ENR|=(1U<<0);
    }
    static void set_input(){
    	GPIOA->IDR&=~(3<<0);
    }

    static void set_output(std::uint32_t pin)
    {
        GPIOD->MODER &= ~(3U << (pin * 2));
        GPIOD->MODER |=  (1U << (pin * 2));
    }
    static uint32_t read(std::uint32_t button){
    	return (GPIOA->IDR>>button)&1;
    }
    static void set(uint32_t pin){
    	GPIOD->BSRR|=(1<<pin);
    }
    static void reset(uint32_t pin){
    	GPIOD->BSRR|=(1<<(16+pin));
    }
    static void toggle(std::uint32_t pin)
    {
    	 if (GPIOD->ODR & (1U << pin)) {
    		 reset(pin);
    }
    	 else{
    		 set(pin);
    	 }
    }
};

class LED
{
    std::uint32_t pin;

public:
    explicit LED(std::uint32_t p) : pin(p)
    {
        GPIO::set_output(pin);
    }

    void toggle()
    {
        GPIO::toggle(pin);
    }
};
void delay()
{
    for (volatile std::uint32_t i = 0; i < 100000; ++i);
}

int main()
{
    constexpr std::uint32_t LED_PIN = 12;
    constexpr std::uint32_t Sw_Pin = 0;
    GPIO::enable_clock();
    GPIO::set_input();
    LED led(LED_PIN);
    while (true)
    {
    	if(GPIO::read(Sw_Pin)==1){
        led.toggle();
        delay();
    }
    }
}
