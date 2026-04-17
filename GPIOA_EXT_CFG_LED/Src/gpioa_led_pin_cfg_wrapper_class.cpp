#include <cstdint>
#include <type_traits>

// =======================================================
// 🔹 1. Generic Register Wrapper
// =======================================================
// Purpose:
// - Access memory-mapped registers safely
// - Replace macros like *(volatile uint32_t*)addr
// - Zero runtime cost (inlined by compiler)
template<std::uintptr_t address>
struct Register
{
    static volatile uint32_t& value()
    {
        return *reinterpret_cast<volatile uint32_t*>(address);
    }
};

// =======================================================
// 🔹 2. Peripheral Base Addresses (STM32F407)
// =======================================================
constexpr std::uintptr_t RCC_BASE   = 0x40023800;
constexpr std::uintptr_t GPIOA_BASE = 0x40020000;
constexpr std::uintptr_t GPIOB_BASE = 0x40020400;
constexpr std::uintptr_t GPIOC_BASE = 0x40020800;
constexpr std::uintptr_t GPIOD_BASE = 0x40020C00;
constexpr std::uintptr_t GPIOE_BASE = 0x40021000;
// =======================================================
// 🔹 3. RCC (Clock Control) Wrapper
// =======================================================
// Controls enabling/disabling peripheral clocks
struct RCC
{
    // AHB1 Peripheral Clock Enable Register
    static auto& AHB1ENR()
    {
        return Register<RCC_BASE + 0x30>::value();
    }
};

// =======================================================
// 🔹 4. GPIO Port Wrapper (Generic)
// =======================================================
// Represents one GPIO peripheral (A, B, C, D, etc.)
template<std::uintptr_t base>
struct GPIO_Port
{
    // Mode register (input/output/alternate/analog)
    static auto& MODER() { return Register<base + 0x00>::value(); }

    // Output type register (push-pull/open-drain)
    static auto& OTYPER(){ return Register<base + 0x04>::value(); }

    // Output speed register
    static auto& OSPEEDR(){ return Register<base + 0x08>::value(); }

    // Pull-up / Pull-down register
    static auto& PUPDR() { return Register<base + 0x0C>::value(); }

    // Input data register
    static auto& IDR()   { return Register<base + 0x10>::value(); }

    // Output data register
    static auto& ODR()   { return Register<base + 0x14>::value(); }

    // Bit Set/Reset register (atomic operations)
    static auto& BSRR()  { return Register<base + 0x18>::value(); }
};

// =======================================================
// 🔹 5. GPIO Driver (Low-Level)
// =======================================================
// Template parameters:
// - Port → which GPIO (A/B/C/D)
// - Pin  → which pin (0–15)
//
// This class directly interacts with hardware


using GPIOA = GPIO_Port<GPIOA_BASE>;
using GPIOB = GPIO_Port<GPIOB_BASE>;
using GPIOC = GPIO_Port<GPIOC_BASE>;
using GPIOD = GPIO_Port<GPIOD_BASE>;
using GPIOE = GPIO_Port<GPIOE_BASE>;

// ===================================
template<typename Port, uint8_t pin>
class GPIO
{
public:
    // Compile-time safety check
    static_assert(pin < 16, "Invalid GPIO pin");

    // Enable clock for GPIO (GPIOD example)
    static void enable_clock()
       {
           if constexpr (std::is_same_v<Port, GPIOA>)
               RCC::AHB1ENR() |= (1U << 0);
           else if constexpr (std::is_same_v<Port, GPIOB>)
               RCC::AHB1ENR() |= (1U << 1);
           else if constexpr (std::is_same_v<Port, GPIOC>)
               RCC::AHB1ENR() |= (1U << 2);
           else if constexpr (std::is_same_v<Port, GPIOD>)
               RCC::AHB1ENR() |= (1U << 3);
           else if constexpr (std::is_same_v<Port, GPIOE>)
               RCC::AHB1ENR() |= (1U << 4);
       }
    static void set_input()
    {
    	Port::MODER()&= ~(3U << (pin*2));
    }

    // Configure pin as output
    static void set_output()
    {
        // Clear 2 bits for pin
        Port::MODER() &= ~(3U << (pin * 2));

        // Set as output (01)
        Port::MODER() |=  (1U << (pin * 2));
    }

    // Set pin HIGH (atomic)
    static void set()
    {
        Port::BSRR() = (1U << pin);
    }

    // Set pin LOW (atomic)
    static void reset()
    {
        Port::BSRR() = (1U << (pin + 16));
    }

    // Read pin state
    static bool read()
    {
        return (Port::IDR() & (1U << pin));
    }

    // Toggle pin (safe logic)
    static void toggle()
    {
        if (read())
            reset();
        else
            set();
    }
};


//========================================================
// 🔹 7. High-Level Abstraction (LED)
// =======================================================
// Represents a real-world device
// Does NOT know about registers
template<typename GPIO_PIN,uint8_t st>
class LED
{
public:
    LED()
    {
    	if(st==0)
    	{
        GPIO_PIN::set_output();
    	}
    	else
    	{
    		GPIO_PIN::set_input();
    	}
    }

    void on()
    {
        GPIO_PIN::set();
    }

    void off()
    {
        GPIO_PIN::reset();
    }

    void toggle()
    {
        GPIO_PIN::toggle();
    }
    bool read()
    {
    	return GPIO_PIN::read();
    }
};

// =======================================================
// 🔹 8. Delay Function (Simple)
// =======================================================
void delay()
{
    for (volatile uint32_t i = 0; i < 200000; i++)
    {
        __asm volatile("nop");
    }
}

// =======================================================
// 🔹 9. Main Application
// =======================================================
int main()
{
    // Create type for LED pin (PD12)

    using porte5 = GPIO<GPIOE, 5>;
    using portb7 = GPIO<GPIOB, 7>;
    using porta1 = GPIO<GPIOA ,1>;
    // Enable hardware clock
    porte5::enable_clock();
    portb7::enable_clock();
    porta1::enable_clock();
    // Create LED object
    LED<porte5,0> led1;
    LED<portb7,0> led2;
    LED<porta1,1> sw;
    // Infinite loop
    while (true)
    {
    	if(sw.read()==1)
    	{
        led1.toggle();
        delay();
    	}
    	else
    	{
        led2.toggle();
        delay();
    	}
    }
}
