/*
 * spi_bitdef.h
 *
 *  Created on: 08-Jul-2026
 *      Author: varun.s.patil
 */

#ifndef INC_SPI_BITDEF_H_
#define INC_SPI_BITDEF_H_

/*
 * Purpose:
 * Contains SPI peripheral bit position and bit mask definitions
 * to keep stm32f407xx.h clean and organized.
 */

/*===========================================================
 * SPI_CR1 Register Bit Field Mapping
 *==========================================================*/
#define SPI_CR1_CPHA             0U
#define SPI_CR1_CPOL             1U
#define SPI_CR1_MSTR             2U
#define SPI_CR1_BR               3U
#define SPI_CR1_SPE              6U
#define SPI_CR1_LSBFIRST         7U
#define SPI_CR1_SSI              8U
#define SPI_CR1_SSM              9U
#define SPI_CR1_RXONLY          10U
#define SPI_CR1_DFF             11U
#define SPI_CR1_CRCNEXT         12U
#define SPI_CR1_CRCEN           13U
#define SPI_CR1_BIDIOE          14U
#define SPI_CR1_BIDIMODE        15U

/*===========================================================
 * SPI_CR2 Register Bit Field Mapping
 *==========================================================*/
#define SPI_CR2_RXDMAEN          0U
#define SPI_CR2_TXDMAEN          1U
#define SPI_CR2_SSOE             2U
// Bit 3 Reserved
#define SPI_CR2_FRF              4U
#define SPI_CR2_ERRIE            5U
#define SPI_CR2_RXNEIE           6U
#define SPI_CR2_TXEIE            7U


/*===========================================================
 * SPI_SR Register Bit Field Mapping
 *==========================================================*/
#define SPI_SR_RXNE              0U
#define SPI_SR_TXE               1U
#define SPI_SR_CHSIDE            2U
#define SPI_SR_UDR               3U
#define SPI_SR_CRCERR            4U
#define SPI_SR_MODF              5U
#define SPI_SR_OVR               6U
#define SPI_SR_BSY               7U
#define SPI_SR_FRE               8U


/*===========================================================
 * SPI_DR Register Bit Field Mapping
 *==========================================================*/
#define SPI_DR_DR                0U      // DR[15:0]


/*===========================================================
 * SPI_CRCPR Register Bit Field Mapping
 *==========================================================*/
#define SPI_CRCPR_CRCPOLY        0U      // CRCPOLY[15:0]


/*===========================================================
 * SPI_RXCRCR Register Bit Field Mapping
 *==========================================================*/
#define SPI_RXCRCR_RXCRC         0U      // RxCRC[15:0]


/*===========================================================
 * SPI_TXCRCR Register Bit Field Mapping
 *==========================================================*/
#define SPI_TXCRCR_TXCRC         0U      // TxCRC[15:0]


/*===========================================================
 * SPI_I2SCFGR Register Bit Field Mapping
 *==========================================================*/
#define SPI_I2SCFGR_CHLEN        0U
#define SPI_I2SCFGR_DATLEN       1U      // DATLEN[1:0]
#define SPI_I2SCFGR_CKPOL        3U
#define SPI_I2SCFGR_I2SSTD       4U      // I2SSTD[1:0]
#define SPI_I2SCFGR_PCMSYNC      7U
// Bit 8 Reserved
#define SPI_I2SCFGR_I2SCFG       9U      // I2SCFG[1:0]
#define SPI_I2SCFGR_I2SE         10U
#define SPI_I2SCFGR_I2SMOD       11U


/*===========================================================
 * SPI_I2SPR Register Bit Field Mapping
 *==========================================================*/
#define SPI_I2SPR_I2SDIV         0U      // I2SDIV[7:0]
#define SPI_I2SPR_ODD            8U
#define SPI_I2SPR_MCKOE          9U

#endif /* INC_SPI_BITDEF_H_ */
