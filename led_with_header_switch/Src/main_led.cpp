#include "led.h"
#include "switch.h"

#define POWER_THRESHOLD 10

int main()
{

    LED led12(GPIOD,12,5,3,OFF);
    LED led13(GPIOD,13,5,3,OFF);
    LED led14(GPIOD,14,5,3,OFF);
    LED led15(GPIOD,15,5,3,OFF);

    SWITCH sw(GPIOA,0);

    while(1)
    {

        if(sw.read())
        {
            led12.reset();
        }

        uint32_t power = led12.power();

        if(power > POWER_THRESHOLD)
        {
            led12.toggle();
            led13.toggle();
            led12.delay();
        }
        else
        {
            led14.toggle();
            led15.toggle();
            led14.delay();
        }
    }
}
