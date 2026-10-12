/*
 * usart_bitdef.h
 *
 * Contains USART peripheral bit position definitions
 * to keep stm32f407xx.h clean and organized.
 */

#ifndef INC_USART_BITDEF_H_
#define INC_USART_BITDEF_H_

/*===========================================================
 * USART_SR Register Bit Field Mapping
 * Address offset: 0x00, Reset value: 0x000C000C0
 *==========================================================*/
#define USART_SR_PE              0U      // Parity error
#define USART_SR_FE              1U      // Framing error
#define USART_SR_NF              2U      // Noise detected flag
#define USART_SR_ORE             3U      // Overrun error
#define USART_SR_IDLE            4U      // IDLE line detected
#define USART_SR_RXNE            5U      // Read data register not empty
#define USART_SR_TC              6U      // Transmission complete
#define USART_SR_TXE             7U      // Transmit data register empty
#define USART_SR_LBD             8U      // LIN break detection flag
#define USART_SR_CTS             9U      // CTS input changed flag
// Bits 31:10 Reserved

/*===========================================================
 * USART_DR Register Bit Field Mapping
 * Address offset: 0x04; Reset value: 0x00000000
 *==========================================================*/
#define USART_DR_DR              0U      // Data value, bits [8:0]
// Bits 31:9 Reserved

/*===========================================================
 * USART_BRR Register Bit Field Mapping
 * Address offset: 0x08; Reset value: 0x00000000
 *==========================================================*/
#define USART_BRR_DIV_FRACTION    0U      // Fraction of USARTDIV, bits [3:0]
#define USART_BRR_DIV_MANTISSA    4U      // Mantissa of USARTDIV, bits [15:4]
// Bits 31:16 Reserved
// Baud counters stop when TE (transmit) or RE (receive) is disabled, respectively.

/*===========================================================
 * USART_CR1 Register Bit Field Mapping
 * Address offset: 0x0C; Reset value: 0x00000000
 *==========================================================*/
#define USART_CR1_SBK             0U      // Send break
#define USART_CR1_RWU             1U      // Receiver wakeup
#define USART_CR1_RE              2U      // Receiver enable
#define USART_CR1_TE              3U      // Transmitter enable
#define USART_CR1_IDLEIE          4U      // IDLE interrupt enable
#define USART_CR1_RXNEIE          5U      // RXNE interrupt enable
#define USART_CR1_TCIE            6U      // Transmission complete interrupt enable
#define USART_CR1_TXEIE           7U      // TXE interrupt enable
#define USART_CR1_PEIE            8U      // PE interrupt enable
#define USART_CR1_PS              9U      // Parity selection
#define USART_CR1_PCE             10U     // Parity control enable
#define USART_CR1_WAKE            11U     // Wakeup method
#define USART_CR1_M               12U     // Word length
#define USART_CR1_UE              13U     // USART enable
// Bit 14 Reserved
#define USART_CR1_OVER8           15U     // Oversampling mode

/*===========================================================
 * USART_CR2 Register Bit Field Mapping
 * Address offset: 0x10; Reset value: 0x00000000
 *==========================================================*/
#define USART_CR2_ADD             0U      // Address of the USART node, bits [3:0]
// Bit 4 Reserved
#define USART_CR2_LBDL            5U      // LIN break detection length
#define USART_CR2_LBDIE           6U      // LIN break detection interrupt enable
// Bit 7 Reserved
#define USART_CR2_LBCL            8U      // Last bit clock pulse
#define USART_CR2_CPHA            9U      // Clock phase
#define USART_CR2_CPOL            10U     // Clock polarity
#define USART_CR2_CLKEN           11U     // Clock enable
#define USART_CR2_STOP            12U     // STOP bits, bits [13:12]
#define USART_CR2_LINEN           14U     // LIN mode enable
// Bit 15 Reserved
// Bits 31:16 Reserved

/*===========================================================
 * USART_CR3 Register Bit Field Mapping
 * Address offset: 0x14; Reset value: 0x00000000
 *==========================================================*/
#define USART_CR3_EIE             0U      // Error interrupt enable
#define USART_CR3_IREN            1U      // IrDA mode enable
#define USART_CR3_IRLP            2U      // IrDA low-power
#define USART_CR3_HDSEL           3U      // Half-duplex selection
#define USART_CR3_NACK            4U      // Smartcard NACK enable
#define USART_CR3_SCEN            5U      // Smartcard mode enable
#define USART_CR3_DMAR            6U      // DMA enable receiver
#define USART_CR3_DMAT            7U      // DMA enable transmitter
#define USART_CR3_RTSE            8U      // RTS enable
#define USART_CR3_CTSE            9U      // CTS enable
#define USART_CR3_CTSIE           10U     // CTS interrupt enable
#define USART_CR3_ONEBIT          11U     // One sample bit method enable
// Bits 15:12 Reserved
// Bits 31:16 Reserved

/*===========================================================
 * USART_GTPR Register Bit Field Mapping
 * Address offset: 0x18; Reset value: 0x00000000
 *==========================================================*/
#define USART_GTPR_PSC            0U      // Guard time and prescaler, bits [7:0]
#define USART_GTPR_GT             8U      // Guard time, bits [15:8]
// Bits 31:16 Reserved

#endif /* INC_USART_BITDEF_H_ */
