#include <usart.h>

void delay()
{
	for(volatile uint32_t i=0;i<100000;i++);
}
USART_Handle_t husart2;

int main()
{
    husart2.instance = USART2;

    husart2.config.baudrate = 9600;

    husart2.config.tx_pin.port = GPIOA;
    husart2.config.tx_pin.pin = 2;
    husart2.config.tx_pin.alternate_function = 7;

    husart2.config.rx_pin.port = GPIOA;
    husart2.config.rx_pin.pin = 3;
    husart2.config.rx_pin.alternate_function = 7;

    USART_Init(&husart2);

    while(1)
    {
        USART_SendString(&husart2,"Generic USART driver\r\n");
        delay();
        USART_SendString(&husart2,"Harika\r\n");
    }
}
