#include <cstdint>
#include <stm32f407.h>

static int8_t read_ch;

void delayMs(int n) {
    for (; n > 0; n--)
        for (volatile int i = 0; i < 2000; i++);
}

class USART {
private:
    USART() { USART3_init(); } // Constructor handles init

public:
    static USART& instance() noexcept {
        static USART instance;
        return instance;
    }

    // Delete copy/move operations
    USART(const USART&) = delete;
    USART& operator=(const USART&) = delete;
    USART(USART&&)=delete;
    USART& operator=(USART&&)=delete;

    void USART3_init() {
        // Enable Clocks: GPIOC (TX/RX), GPIOD (LEDs), USART3
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN | RCC_AHB1ENR_GPIODEN;
        RCC->APB1ENR |= RCC_APB1ENR_USART3EN;

        // Configure PD12, PD13, PD14 as Output for LEDs
        GPIOD->MODER &= ~( (3 << 24) | (3 << 26) | (3 << 28) );
        GPIOD->MODER |=  ( (1 << 24) | (1 << 26) | (1 << 28) );

        // Configure PC10 (TX) and PC11 (RX) as Alternate Function
        GPIOC->MODER &= ~( (3 << 20) | (3 << 22) );
        GPIOC->MODER |=  ( (2 << 20) | (2 << 22) ); // AF mode is 10 binary

        // Map PC10/PC11 to AF7 (USART3)
        GPIOC->AFR[1] &= ~( (0xF << 8) | (0xF << 12) );
        GPIOC->AFR[1] |=  ( (7 << 8) | (7 << 12) );

        // USART3 Settings: 9600 Baud @ 16MHz
        USART3->BRR = 0x0683;
        USART3->CR1 |= (USART_CR1_TE | USART_CR1_RE | USART_CR1_UE);
    }

    void USART3_write(char ch) {
        while (!(USART3->SR & USART_SR_TXE)); // Wait for TX Empty
        USART3->DR = (ch & 0xFF);
    }

    void USART3_write(const char* str) {
        while (*str) {
            USART3_write(*str++);
        }
    }

    uint8_t USART3_read() {
        while (!(USART3->SR & USART_SR_RXNE)); // Wait for RX Not Empty
        return USART3->DR;
    }
};

int main() {
    USART& uart = USART::instance();

    while (1) {
        uart.USART3_write("H");
        read_ch=uart.USART3_read();
        GPIOD->ODR |= (1 << 13);  // LED ON
        delayMs(500);
        GPIOD->ODR &= ~(1 << 13); // LED OFF
        delayMs(500);
    }
}


















