/*
 * spi_driver.h
 *
 *  Created on: Mar 10, 2026
 *      Author: varun.s.patil
 */

#ifndef INC_SPI_DRIVER_H_
#define INC_SPI_DRIVER_H_

#include "stm32f407xx.h"

/************************************************************************/
/* A Configuration Structure for SPI pin holding all its settings */
/************************************************************************/
/**
  * @brief  SPI Configuration Structure definition
  */
typedef struct
{
  uint32_t Mode;                 /* Master / Slave */                                 /*!< Specifies the SPI operating mode.This parameter can be a value of @ref SPI_Mode */
  uint32_t Direction;            /* Full duplex / Half duplex or Bus Config */        /*!< Specifies the SPI bidirectional mode state.This parameter can be a value of @ref SPI_Direction */
  uint32_t DataSize;             /* 8-bit / 16-bit */                                 /*!< Specifies the SPI data size.This parameter can be a value of @ref SPI_Data_Size */
  uint32_t CLKPolarity;          /* CPOL */                                           /*!< Specifies the serial clock steady state.This parameter can be a value of @ref SPI_Clock_Polarity */
  uint32_t CLKPhase;             /* CPHA */                                           /*!< Specifies the clock active edge for the bit capture.This parameter can be a value of @ref SPI_Clock_Phase */
  uint32_t NSS;                  /* Hardware / Software NSS or SSM*/                  /*!< Specifies whether the NSS signal is managed byhardware (NSS pin) or by software using the SSI bit.This parameter can be a value of @ref SPI_Slave_Select_management */
  uint32_t BaudRatePrescaler;    /* Clock divider */                                  /*!< Specifies the Baud Rate prescaler value which will be used to configure the transmit and receive SCK clock.This parameter can be a value of @ref SPI_BaudRate_Prescaler @note The communication clock is derived from the master clock. The slave clock does not need to be set. */
  uint32_t FirstBit;             /* MSB / LSB */                                      /*!< Specifies whether data transfers start from MSB or LSB bit.This parameter can be a value of @ref SPI_MSB_LSB_transmission */
  uint32_t TIMode;               /* TI mode */                                        /*!< Specifies if the TI mode is enabled or not. This parameter can be a value of @ref SPI_TI_mode */
  uint32_t CRCCalculation;       /* CRC enable */                                     /*!< Specifies if the CRC calculation is enabled or not. This parameter can be a value of @ref SPI_CRC_Calculation */
  uint32_t CRCPolynomial;        /* CRC polynomial */                                 /*!< Specifies the polynomial used for the CRC calculation.This parameter must be an odd number between Min_Data = 1 and Max_Data = 65535 */
} SPI_Config_t;

/**
  * @brief SPI State structure definition
  */
typedef enum
{
  SPI_STATE_RESET      = 0x00U,    /*!< Peripheral not Initialized                         */
  SPI_STATE_READY      = 0x01U,    /*!< Peripheral Initialized and ready for use           */
  SPI_STATE_BUSY       = 0x02U,    /*!< an internal process is ongoing                     */
  SPI_STATE_BUSY_TX    = 0x03U,    /*!< Data Transmission process is ongoing               */
  SPI_STATE_BUSY_RX    = 0x04U,    /*!< Data Reception process is ongoing                  */
  SPI_STATE_BUSY_TX_RX = 0x05U,    /*!< Data Transmission and Reception process is ongoing */
  SPI_STATE_ERROR      = 0x06U,    /*!< SPI error state                                    */
  SPI_STATE_ABORT      = 0x07U     /*!< SPI abort is ongoing                               */
} SPI_StateTypeDef;

/**
  * @brief  SPI handle Structure definition
  */
typedef struct __SPI_HandleTypeDef
{
  SPI_RegDef_t                *Instance;      /*!< SPI registers base address SPI1 to SPI4              */
  SPI_Config_t            Init;           /*!< SPI communication parameters i.e. Config Settings            */
  const uint8_t              *pTxBuffPtr;    /*!< Pointer to SPI Tx transfer Buffer        */
  uint16_t                   TxXferSize;     /*!< SPI Tx Transfer size                     */
  __IO uint16_t              TxXferCount;    /*!< SPI Tx Transfer Counter                  */
  uint8_t                    *pRxBuffPtr;    /*!< Pointer to SPI Rx transfer Buffer        */
  uint16_t                   RxXferSize;     /*!< SPI Rx Transfer size                     */
  __IO uint16_t              RxXferCount;    /*!< SPI Rx Transfer Counter                  */
  void (*RxISR)(struct __SPI_HandleTypeDef *hspi);   /*!< function pointer on Rx ISR       */
  void (*TxISR)(struct __SPI_HandleTypeDef *hspi);   /*!< function pointer on Tx ISR       */
//   DMA_HandleTypeDef          *hdmatx;        /*!< SPI Tx DMA Handle parameters   <WILL PORT IN FUTURE>          */
//   DMA_HandleTypeDef          *hdmarx;        /*!< SPI Rx DMA Handle parameters   <WILL PORT IN FUTURE>             */
//   HAL_LockTypeDef            Lock;           /*!< Locking object     <WILL PORT IN FUTURE>                           */
  __IO SPI_StateTypeDef  State;          /*!< SPI communication state                  */
  __IO uint32_t              ErrorCode;      /*!< SPI Error code                           */
} SPI_HandleTypeDef;

#endif /* INC_SPI_DRIVER_H_ */
