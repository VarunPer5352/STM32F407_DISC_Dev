#ifndef INC_USART_DRIVER_H_
#define INC_USART_DRIVER_H_

#include "stm32f407xx.h"

typedef struct
{
    uint32_t  USART_Mode;
    uint32_t USART_Baud;
    uint32_t  USART_NoOfStopBits;
    uint32_t  USART_WordLength;
    uint32_t  USART_ParityControl;
    uint32_t  USART_HWFlowControl;
} USART_Config_t;

typedef struct
{
    USART_RegDef_t *pUSARTx;
    USART_Config_t USART_Config;
} USART_Handle_t;


#endif /* INC_USART_DRIVER_H_ */