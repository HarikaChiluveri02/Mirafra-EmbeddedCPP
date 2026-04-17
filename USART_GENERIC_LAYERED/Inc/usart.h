/*
 * usart.h
 *
 *  Created on: Apr 13, 2026
 *      Author: dell
 */

#ifndef USART_H_
#define USART_H_


#include <stdint.h>
#include "stm32f407.h"

typedef struct
{
    GPIO_TypeDef *port;
    uint8_t pin;
    uint8_t alternate_function;
}GPIO_PinConfig_t;

typedef struct
{
    uint32_t baudrate;

    GPIO_PinConfig_t tx_pin;
    GPIO_PinConfig_t rx_pin;

}USART_Config_t;

typedef struct
{
    USART_TypeDef *instance;
    USART_Config_t config;
}USART_Handle_t;


/* API */
void USART_Init(USART_Handle_t *husart);
void USART_SendChar(USART_Handle_t *husart, char data);
void USART_SendString(USART_Handle_t *husart, const char *str);
char USART_ReadChar(USART_Handle_t *husart);



#endif /* USART_H_ */
