#ifndef INC_USART_DRIVER_H_
#define INC_USART_DRIVER_H_

#include "stm32f407xx.h"
#include "usart_bitdef.h"

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

/************************************************************************/
/* USART operating mode */
#define USART_MODE_ONLY_TX       0U
#define USART_MODE_ONLY_RX       1U
#define USART_MODE_TXRX          2U

/* Standard baud-rate selections (bits per second) */
#define USART_STD_BAUD_1200      1200U
#define USART_STD_BAUD_2400      2400U
#define USART_STD_BAUD_9600      9600U
#define USART_STD_BAUD_19200     19200U
#define USART_STD_BAUD_38400     38400U
#define USART_STD_BAUD_57600     57600U
#define USART_STD_BAUD_115200    115200U
#define USART_STD_BAUD_230400    230400U
#define USART_STD_BAUD_460800    460800U
#define USART_STD_BAUD_921600    921600U
#define USART_STD_BAUD_2M        2000000U
#define USART_STD_BAUD_3M        3000000U

/* Parity control */
#define USART_PARITY_EN_ODD      2U
#define USART_PARITY_EN_EVEN     1U
#define USART_PARITY_DISABLE     0U

/* USART word length */
#define USART_WORDLEN_8BITS      0U
#define USART_WORDLEN_9BITS      1U

/* Number of stop bits */
#define USART_STOPBITS_1         0U
#define USART_STOPBITS_0_5       1U
#define USART_STOPBITS_2         2U
#define USART_STOPBITS_1_5       3U

/* Hardware flow-control selection */
#define USART_HW_FLOW_CTRL_NONE  0U
#define USART_HW_FLOW_CTRL_CTS   1U
#define USART_HW_FLOW_CTRL_RTS   2U
#define USART_HW_FLOW_CTRL_CTS_RTS 3U

/* Function prototypes for USART-specific APIs */
/************************************************************************/
/**
 * API to control clock enabling & disabling for a USART peripheral
 */
void usart_clk_ctrl(USART_RegDef_t *pUSARTx_addr, uint8_t state);

/**
 * API's to init/deinit a USART pin as per settings!
 */
void usart_init(USART_Handle_t *pUSARTx_handle);
void usart_deinit(USART_RegDef_t *pUSARTx_addr);

/**
 * API's to send/receive {Blocking Mode}
 */
void usart_send_data(USART_RegDef_t *pUSARTx_addr, uint8_t *pTX_buffer, uint32_t data_len);
void usart_receive_data(USART_RegDef_t *pUSARTx_addr, uint8_t *pRX_buffer, uint32_t data_len);
uint8_t usart_send_data_it(USART_Handle_t *pUSARTx_handle, uint8_t *pTX_buffer, uint32_t data_len);
uint8_t usart_receive_data_it(USART_Handle_t *pUSARTx_handle, uint8_t *pRX_buffer, uint32_t data_len);

/**
 * API's for Interrupt Handling
 */
void usart_irq_config(IRQn_Type IRQ_number, uint8_t IRQ_priority, uint8_t state);
void usart_irq_handle(USART_Handle_t *pUSARTx_handle);
void usart_peripheral_ctrl(USART_RegDef_t *pUSARTx_addr, uint8_t state);
uint8_t usart_get_flag_status(USART_RegDef_t *pUSARTx_addr, uint32_t flag_name);
void usart_clear_flag(USART_RegDef_t *pUSARTx_addr, uint16_t status_flag_name);
void usart_application_event_callback(USART_Handle_t *pUSARTx_handle, uint8_t app_event);

/**
 * Future & Advanced API's for tasks!
 */

#endif /* INC_USART_DRIVER_H_ */