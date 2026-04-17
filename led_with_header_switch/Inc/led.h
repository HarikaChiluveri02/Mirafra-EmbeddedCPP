#ifndef LED_H_
#define LED_H_

#include "stm32f407.h"
#include <stdint.h>

typedef enum
{
    OFF = 0,
    ON  = 1
}LED_State;

class LED
{
private:
    GPIO_TypeDef *pGPIO;
    uint8_t pin;
    uint32_t voltage;
    uint32_t current;
    LED_State state;

public:

    LED(GPIO_TypeDef *GPIOx,uint8_t p,uint32_t v,uint32_t c,LED_State s);

    void on();
    void off();
    void toggle();

    void delay();

    uint32_t power();

    void reset();

};

#endif
