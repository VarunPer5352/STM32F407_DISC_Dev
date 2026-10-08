/* Interrupt-driven PN532 application. Behavioral reference: main_pn532.c.
 * Build this file instead of the reference. No SPI byte or BSY busy waits.
 */
#include <stdint.h>
#include "main.h"
#include <string.h>

/******************************************************************************
 * PN532 SPI protocol definitions
 ******************************************************************************/
#define PN532_SPI_DATA_WRITE          0x01U // STM32 writes a PN532 command frame
#define PN532_SPI_STATUS_READ         0x02U // STM32 reads PN532 ready/busy status
#define PN532_SPI_DATA_READ           0x03U // STM32 clocks out ACK/response data from PN532

#define PN532_PREAMBLE                0x00U // First byte of every normal PN532 frame
#define PN532_START_CODE_1            0x00U // First byte of PN532 frame start sequence: 00 FF
#define PN532_START_CODE_2            0xFFU // Second byte of PN532 frame start sequence: 00 FF
#define PN532_POSTAMBLE               0x00U // Final byte appended after the frame checksum

#define PN532_HOST_TO_PN532           0xD4U // TFI value used for commands sent from STM32/host to PN532
#define PN532_PN532_TO_HOST           0xD5U // TFI value used for responses sent from PN532 back to STM32/host

#define PN532_CMD_GET_FIRMWARE        0x02U // GetFirmwareVersion: reads PN532 IC, firmware version, revision and feature support
#define PN532_CMD_SAM_CONFIGURATION   0x14U // SAMConfiguration: configures PN532 operating mode before NFC operations
#define PN532_CMD_IN_LIST_PASSIVE     0x4AU // InListPassiveTarget: searches for nearby passive NFC/RFID tags
#define PN532_CMD_IN_RELEASE          0x52U // InRelease: finish communication with the selected target(s)

#define PN532_READY_BIT               0x01U // Bit 0 of PN532 SPI status byte: 1 = ACK/response data is ready to read

#define PN532_MAX_FRAME_SIZE          64U // Maximum PN532 frame buffer used by this driver during initial bring-up
#define PN532_MAX_UID_SIZE            10U //Type-A UID can be 4, 7, or 10 bytes, so 10 is a sensible buffer
#define PN532_CMD_RF_CONFIGURATION    0x32U

// Various statuses from documnetation!
typedef enum{
    PN532_OK = 0,
    PN532_NO_TAG,
    PN532_ERR_TIMEOUT,
    PN532_ERR_ACK,
    PN532_ERR_FRAME,
    PN532_ERR_CHECKSUM,
    PN532_ERR_RESPONSE,
    PN532_ERR_BUFFER,
    PN532_ERR_SPI
} pn532_status_t;

/*
 * Debug variables: These are deliberately global/volatile so they can be watched directly from the STM32CubeIDE debugger.
 */
volatile pn532_status_t dbg_pn532_status = PN532_OK;
volatile pn532_status_t dbg_pn532_release_status = PN532_OK;
volatile uint8_t dbg_fw_ic = 0;
volatile uint8_t dbg_fw_version = 0;
volatile uint8_t dbg_fw_revision = 0;
volatile uint8_t dbg_fw_support = 0;
volatile uint8_t dbg_uid[PN532_MAX_UID_SIZE] = {0};
volatile uint8_t dbg_uid_length = 0;
volatile uint8_t dbg_pn532_raw_status = 0;
volatile uint8_t dbg_pn532_command = 0;
/* 1 = write, 2 = ACK wait, 3 = ACK read, 4 = response wait,
 * 5 = response read, 6 = transaction complete. Inspect after CS is high. */
volatile uint8_t dbg_pn532_phase = 0;
volatile uint8_t dbg_pn532_ack_status = 0;
volatile uint8_t dbg_pn532_response_status = 0;
volatile uint8_t dbg_pn532_ack[6] = {0};
volatile uint8_t dbg_pn532_bad_header[5] = {0};
volatile uint32_t dbg_pn532_status_ff_count = 0;
volatile uint32_t dbg_pn532_tag_count = 0;
volatile uint32_t dbg_pn532_no_tag_count = 0;

void SystemInit(void)
{
#if (__FPU_PRESENT == 1U) && (__FPU_USED == 1U)
    SCB->CPACR |= ((3UL << (10U * 2U)) | (3UL << (11U * 2U)));
    __DSB();
    __ISB();
#endif
}

void delay_nop(uint32_t count)
{
    while (count--)
    {
        __asm volatile ("nop");
    }
}

/******************************************************************************
 * @brief      Handles unrecoverable application errors.
 *
 * @details    Disables interrupts and traps the CPU in an infinite loop,
 *             allowing the debugger to halt execution and inspect the system
 *             state at the point of failure.
 *
 * @return     None
 ******************************************************************************/
void Error_Handler(void)
{
    __disable_irq();

    while (1)
    {
        __NOP();
    }
}

volatile float dbg_adc_raw = 2048.0f;
volatile float dbg_vref = 3.3f;
volatile float dbg_adc_voltage = 0.0f;
volatile float dbg_sensor_offset = 1.65f;
volatile float dbg_sensor_sensitivity = 0.330f;
volatile float dbg_sensor_value = 0.0f;
volatile float dbg_filtered_value = 0.0f;
volatile uint32_t dbg_fpu_counter = 0;

SPI_Handle_t spi2_t;
GPIO_Handle_t spi_gpio_t;

void spi2_gpio_init(GPIO_Handle_t *spi_gpio_t)
{
	spi_gpio_t->pGPIOx_addr = GPIOB;
	spi_gpio_t->pin_config.Mode = GPIO_MODE_ALTFN;
	spi_gpio_t->pin_config.Alternate = GPIO_AF5_SPI2;
	spi_gpio_t->pin_config.OPType = GPIO_OP_PP;
	spi_gpio_t->pin_config.Pull = GPIO_NOPULL;
	spi_gpio_t->pin_config.Speed = GPIO_SPEED_FREQ_HIGH;

	spi_gpio_t->pin_config.Pin = GPIO_PIN_13; // Config of SCLK-PB13
	gpio_init(spi_gpio_t);

	spi_gpio_t->pin_config.Pin = GPIO_PIN_15; // Config of MOSI-PB15
	gpio_init(spi_gpio_t);

	spi_gpio_t->pin_config.Pin = GPIO_PIN_14; // Config of MISO-PB14
	gpio_init(spi_gpio_t);

	// PB12 as manual CS GPIO, not AF
	spi_gpio_t->pin_config.Mode = GPIO_MODE_OUTPUT;
	spi_gpio_t->pin_config.Alternate = 0;
	spi_gpio_t->pin_config.Pin = GPIO_PIN_12; // Config of NSS-PB12
	gpio_init(spi_gpio_t);

    gpio_set_pin_level(GPIOB, GPIO_PIN_12, 1); // CS idle high
}

void spi2_periph_init(SPI_Handle_t *spi2_t)
{
	spi2_t->Instance = SPI2;
	spi2_t->Init.Direction = SPI_COM_FD;
	spi2_t->Init.Mode = SPI_MODE_MASTER;
	spi2_t->Init.BaudRatePrescaler = SPI_CLK_DIV4; // 4MHz sclk as pn532 is rated for max 5MHz
	spi2_t->Init.DataSize = BYTE_FRAME_LEN; // 8Bit mode
	spi2_t->Init.CLKPhase = SPI_CPHA_LOW;
	spi2_t->Init.CLKPolarity = SPI_CPOL_LOW;
	spi2_t->Init.NSS = SPI_SSM_ENABLED;

	spi_init(spi2_t); // This sets the settings while spi2 is disabled

    /* PN532 SPI transfers every byte LSB-first. */
    SPI2->CR1 |= (1U << SPI_CR1_LSBFIRST); // As per our custom codebase this macros is not bit masked just position of that bit in reg !

	spi_ssi_state(spi2_t->Instance, ENABLE);
    spi_set_state(spi2_t->Instance, ENABLE);
    spi_irq_config(SPI2_IRQn, 5U, ENABLE);
}

/* This project leaves RCC at reset: HSI/HCLK/PCLK1 = 16 MHz, SPI2 = 4 MHz.
 * Change this together with the clock setup if a PLL is introduced. */
#define PN532_CORE_HZ       16000000U
#define PN532_SPI_TIMEOUT_MS 20U
#define PN532_ACK_TIMEOUT_MS 200U
#define PN532_COMMAND_TIMEOUT_MS 500U
#define PN532_TARGET_TIMEOUT_MS 5000U
#define PN532_SCAN_PAUSE_MS 100U

static volatile uint32_t tick_ms;
void SysTick_Handler(void) { tick_ms++; }
void SPI2_IRQHandler(void) { spi_irq_handle(&spi2_t); }

static uint8_t elapsed(uint32_t now, uint32_t since, uint32_t interval)
{
    return (uint32_t)(now - since) >= interval;
}

typedef enum { 
    IO_IDLE,
    IO_GAP, 
    IO_SETUP, 
    IO_ACTIVE 
} io_state_t;

static struct {
    io_state_t state;
    uint8_t tx[PN532_MAX_FRAME_SIZE + 1U];
    uint8_t rx[PN532_MAX_FRAME_SIZE + 1U];
    uint16_t size;
    uint8_t hold_cs;
    uint32_t since;
    uint32_t last_cs_high;
} io;

typedef enum {
    PN_IDLE, 
    PN_WRITE, 
    PN_ACK_WAIT, 
    PN_ACK_STATUS, 
    PN_ACK_READ,
    PN_RESPONSE_WAIT, 
    PN_RESPONSE_STATUS, 
    PN_HEADER, 
    PN_BODY, 
    PN_DONE
} pn_state_t;

static struct {
    pn_state_t state;
    pn532_status_t result;
    uint8_t command;
    uint8_t frame[PN532_MAX_FRAME_SIZE];
    uint8_t response[32];
    uint8_t response_len;
    uint8_t response_capacity;
    uint32_t ready_since;
    uint32_t poll_since;
    uint32_t response_timeout;
} pn;

static void cs_high(uint32_t now)
{
    gpio_set_pin_level(GPIOB, GPIO_PIN_12, 1);
    io.last_cs_high = now;
}

/* Called only from foreground after the previous transfer has finished.
 * tx/rx belong to io, and are not reused until io_poll reports completion. */
static void io_begin(uint16_t size, uint8_t hold_cs, uint8_t continuation, uint32_t now)
{
    io.size = size;
    io.hold_cs = hold_cs;
    io.since = now;
    io.state = continuation ? IO_SETUP : IO_GAP;
    /* Continuation retains CS and needs no additional setup delay. */
    if (continuation) io.since = now - 1U;
}

/* 0 = pending, 1 = finished, -1 = SPI error, -2 = transfer timeout.
 * Check BSY once per visit; never spin waiting for a flag or a timer. */
static int io_poll(uint32_t now)
{
    if (io.state == IO_GAP)
    {
        if (!elapsed(now, io.last_cs_high, 1U)) return 0;
        gpio_set_pin_level(GPIOB, GPIO_PIN_12, 0);
        io.since = now;
        io.state = IO_SETUP;
        return 0;
    }
    if (io.state == IO_SETUP)
    {
        if (!elapsed(now, io.since, 1U)) return 0;
        if (!spi_transfer_full_duplex_it(&spi2_t, io.tx, io.rx, io.size)) return -1;
        io.since = now;
        io.state = IO_ACTIVE;
        return 0;
    }
    if (io.state == IO_ACTIVE)
    {
        SPI_StateTypeDef state = spi_transfer_poll(&spi2_t);
        if (state == SPI_STATE_ERROR) return -1;
        if (state == SPI_STATE_READY)
        {
            __DMB(); /* Acquire received buffer after driver completion. */
            if (!io.hold_cs) cs_high(now);
            io.state = IO_IDLE;
            return 1;
        }
        if (elapsed(now, io.since, PN532_SPI_TIMEOUT_MS)) return -2;
    }
    return 0;
}

static void pn_finish(pn532_status_t result, uint32_t now)
{
    if (result != PN532_OK)
    {
        /* Stop ISR buffer access, then reset SPI to stop clocks even if BSY
         * is stuck. No new command is sent after a failure (reference policy).
         * The device may still have a pending command: do not blindly retry. */
        spi_transfer_abort(&spi2_t);
        spi_deinit(SPI2);
    }
    cs_high(now);
    io.state = IO_IDLE;
    pn.result = result;
    pn.state = PN_DONE;
    if (result == PN532_OK) dbg_pn532_phase = 6;
}

/* Copies parameters before returning; callers may use temporary parameters.
 * A completed result must be consumed (state set to IDLE) before starting. */
static uint8_t pn_start(uint8_t command, const uint8_t *data, uint8_t count, uint8_t capacity, uint32_t timeout, uint32_t now)
{
    if (pn.state != PN_IDLE) return 0;
    pn.response_len = 0;
    pn.command = command;
    pn.response_capacity = capacity;
    pn.response_timeout = timeout;
    dbg_pn532_command = command;
    dbg_pn532_phase = 1;
    dbg_pn532_ack_status = dbg_pn532_response_status = 0;
    if (((uint16_t)count + 9U > PN532_MAX_FRAME_SIZE) || (count && !data) || (capacity > sizeof(pn.response)))
    {
        pn_finish(PN532_ERR_BUFFER, now);
        return 1;
    }
    uint8_t len = (uint8_t)(count + 2U);
    uint8_t sum = (uint8_t)(PN532_HOST_TO_PN532 + command);
    io.tx[0] = PN532_SPI_DATA_WRITE;
    io.tx[1] = PN532_PREAMBLE;
    io.tx[2] = PN532_START_CODE_1;
    io.tx[3] = PN532_START_CODE_2;
    io.tx[4] = len;
    io.tx[5] = (uint8_t)(0U - len);
    io.tx[6] = PN532_HOST_TO_PN532;
    io.tx[7] = command;
    for (uint8_t i = 0; i < count; i++)
    {
        io.tx[8U + i] = data[i];
        sum += data[i];
    }
    io.tx[8U + count] = (uint8_t)(0U - sum);
    io.tx[9U + count] = PN532_POSTAMBLE;
    io_begin((uint16_t)count + 10U, 0, 0, now);
    pn.state = PN_WRITE;
    return 1;
}

static pn532_status_t pn_validate_response(void)
{
    const uint8_t *frame = pn.frame;
    uint8_t len = frame[3];
    if ((len == 1U) && (frame[5] == 0x7FU)) return PN532_ERR_RESPONSE;
    if (len < 2U) return PN532_ERR_FRAME;
    uint8_t sum = 0;
    for (uint8_t i = 0; i < len; i++) sum += frame[5U + i];
    sum += frame[5U + len];
    if (sum) return PN532_ERR_CHECKSUM;
    if (frame[6U + len] != PN532_POSTAMBLE) return PN532_ERR_FRAME;
    if ((frame[5] != PN532_PN532_TO_HOST) ||
        (frame[6] != (uint8_t)(pn.command + 1U))) return PN532_ERR_RESPONSE;
    uint8_t payload = (uint8_t)(len - 2U);
    if (payload > pn.response_capacity) return PN532_ERR_BUFFER;
    memcpy(pn.response, &frame[7], payload);
    pn.response_len = payload;
    return PN532_OK;
}

static void pn_poll(uint32_t now)
{
    static const uint8_t ack[] = { 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00 };
    if ((pn.state == PN_IDLE) || (pn.state == PN_DONE)) return;
    if ((pn.state == PN_ACK_WAIT) || (pn.state == PN_RESPONSE_WAIT) || (pn.state == PN_ACK_STATUS) || (pn.state == PN_RESPONSE_STATUS))
    {
        uint8_t ack_phase = (pn.state == PN_ACK_WAIT) || (pn.state == PN_ACK_STATUS);
        if (elapsed(now, pn.ready_since,
                    ack_phase ? PN532_ACK_TIMEOUT_MS : pn.response_timeout))
        {
            pn_finish(PN532_ERR_TIMEOUT, now);
            return;
        }
        if ((pn.state == PN_ACK_WAIT) || (pn.state == PN_RESPONSE_WAIT))
        {
            if (!elapsed(now, pn.poll_since, 1U)) return;
            io.tx[0] = PN532_SPI_STATUS_READ;
            io.tx[1] = 0xFF;
            io_begin(2, 0, 0, now);
            pn.state = ack_phase ? PN_ACK_STATUS : PN_RESPONSE_STATUS;
            return;
        }
    }
    int result = io_poll(now);
    if (result < 0)
    {
        pn_finish(result == -2 ? PN532_ERR_TIMEOUT : PN532_ERR_SPI, now);
        return;
    }
    if (!result) return;
    switch (pn.state)
    {
    case PN_WRITE:
        pn.ready_since = pn.poll_since = now;
        dbg_pn532_phase = 2;
        pn.state = PN_ACK_WAIT;
        break;
    case PN_ACK_STATUS:
    case PN_RESPONSE_STATUS:
    {
        uint8_t ack_phase = pn.state == PN_ACK_STATUS;
        dbg_pn532_raw_status = io.rx[1];
        if (io.rx[1] == 0xFFU) dbg_pn532_status_ff_count++;
        if (ack_phase) dbg_pn532_ack_status = io.rx[1];
        else dbg_pn532_response_status = io.rx[1];
        if (!(io.rx[1] & PN532_READY_BIT))
        {
            pn.poll_since = now;
            pn.state = ack_phase ? PN_ACK_WAIT : PN_RESPONSE_WAIT;
            break;
        }
        memset(io.tx, 0, sizeof(io.tx));
        io.tx[0] = PN532_SPI_DATA_READ;
        /* Header: operation + 5 bytes. Keep CS low for the remaining body. */
        io_begin(ack_phase ? 7U : 6U, !ack_phase, 0, now);
        dbg_pn532_phase = ack_phase ? 3U : 5U;
        pn.state = ack_phase ? PN_ACK_READ : PN_HEADER;
        break;
    }
    case PN_ACK_READ:
        for (uint8_t i = 0; i < sizeof(ack); i++) dbg_pn532_ack[i] = io.rx[1U + i];
        if (memcmp(&io.rx[1], ack, sizeof(ack)))
        {
            pn_finish(PN532_ERR_ACK, now);
            break;
        }
        pn.ready_since = pn.poll_since = now;
        dbg_pn532_phase = 4;
        pn.state = PN_RESPONSE_WAIT;
        break;
    case PN_HEADER:
        memcpy(pn.frame, &io.rx[1], 5);
        if ((pn.frame[0] != 0) || (pn.frame[1] != 0) || (pn.frame[2] != 0xFFU))
        {
            for (uint8_t i = 0; i < 5U; i++) dbg_pn532_bad_header[i] = pn.frame[i];
            pn_finish(PN532_ERR_FRAME, now);
        }
        else if ((uint8_t)(pn.frame[3] + pn.frame[4]) != 0)
            pn_finish(PN532_ERR_CHECKSUM, now);
        else if ((uint16_t)pn.frame[3] + 7U > sizeof(pn.frame))
            pn_finish(PN532_ERR_BUFFER, now);
        else
        {
            memset(io.tx, 0, sizeof(io.tx));
            io_begin((uint16_t)pn.frame[3] + 2U, 0, 1, now);
            pn.state = PN_BODY;
        }
        break;
    case PN_BODY:
        memcpy(&pn.frame[5], io.rx, (uint16_t)pn.frame[3] + 2U);
        pn_finish(pn_validate_response(), now);
        break;
    default:
        pn_finish(PN532_ERR_FRAME, now);
        break;
    }
}

typedef enum { 
    APP_WAKE, 
    APP_SAM, 
    APP_RF, 
    APP_FIRMWARE, 
    APP_SCAN,           
    APP_RELEASE, 
    APP_PAUSE, 
    APP_FAILED 
} app_state_t;

static app_state_t app = APP_WAKE;
static uint32_t app_since;

static pn532_status_t consume_target(void)
{
    if (pn.response_len < 1U) return PN532_ERR_RESPONSE;
    if (!pn.response[0])
    {
        dbg_uid_length = 0;
        dbg_pn532_no_tag_count++;
        return PN532_NO_TAG;
    }
    if (pn.response_len < 6U) return PN532_ERR_RESPONSE;
    uint8_t len = pn.response[5];
    if (len > PN532_MAX_UID_SIZE) return PN532_ERR_BUFFER;
    if (pn.response_len < (uint8_t)(6U + len)) return PN532_ERR_RESPONSE;
    memset((void *)dbg_uid, 0, sizeof(dbg_uid));
    for (uint8_t i = 0; i < len; i++) dbg_uid[i] = pn.response[6U + i];
    dbg_uid_length = len;
    dbg_pn532_tag_count++;
    return PN532_OK;
}

static void app_poll(uint32_t now)
{
    static const uint8_t sam[] = { 0x01, 0x14, 0x01 };
    static const uint8_t rf[] = { 0x05, 0x00, 0x00, 0x00 };
    static const uint8_t scan[] = { 0x01, 0x00 };
    static const uint8_t release[] = { 0x00 };
    if (app == APP_FAILED) return; /* Preserve the first failure for debugging. */
    if (app == APP_WAKE)
    {
        if (!elapsed(now, app_since, 10U)) return; // Basically, the first time app since before while(1), time and this now should be atleast 10ms if not return back
        cs_high(now);
        app = APP_SAM;
    }
    if (app == APP_PAUSE)
    {
        if (!elapsed(now, app_since, PN532_SCAN_PAUSE_MS)) return;
        app = APP_SCAN;
    }
    if (pn.state == PN_DONE)
    {
        pn532_status_t status = pn.result;
        if (status == PN532_OK)
        {
            switch (app)
            {
            case APP_SAM: app = APP_RF; break;
            case APP_RF: app = APP_FIRMWARE; break;
            case APP_FIRMWARE:
                if (pn.response_len != 4U) status = PN532_ERR_RESPONSE;
                else
                {
                    dbg_fw_ic = pn.response[0];
                    dbg_fw_version = pn.response[1];
                    dbg_fw_revision = pn.response[2];
                    dbg_fw_support = pn.response[3];
                    app = APP_SCAN;
                }
                break;
            case APP_SCAN:
                status = consume_target();
                if (status == PN532_OK) app = APP_RELEASE;
                else if (status == PN532_NO_TAG) { app = APP_PAUSE; app_since = now; }
                break;
            case APP_RELEASE:
                if ((pn.response_len != 1U) || pn.response[0]) status = PN532_ERR_RESPONSE;
                dbg_pn532_release_status = status;
                if (status == PN532_OK) { app = APP_PAUSE; app_since = now; }
                break;
            default: status = PN532_ERR_RESPONSE; break;
            }
        }
        else if (app == APP_RELEASE) dbg_pn532_release_status = status;
        dbg_pn532_status = status;
        if ((status != PN532_OK) && (status != PN532_NO_TAG))
        {
            dbg_uid_length = 0;
            app = APP_FAILED;
            return;
        }
        pn.state = PN_IDLE;
    }
    if (pn.state != PN_IDLE) return;
    switch (app)
    {
        case APP_SAM:
        {
            (void)pn_start(PN532_CMD_SAM_CONFIGURATION, sam, sizeof(sam), 0, PN532_COMMAND_TIMEOUT_MS, now);
            break;
        }
        case APP_RF:
        {
            (void)pn_start(PN532_CMD_RF_CONFIGURATION, rf, sizeof(rf), 0, PN532_COMMAND_TIMEOUT_MS, now);
            break;
        }
        case APP_FIRMWARE:
        {
            (void)pn_start(PN532_CMD_GET_FIRMWARE, NULL, 0, 4, PN532_COMMAND_TIMEOUT_MS, now);
            break;
        }
        case APP_SCAN:
        {
            (void)pn_start(PN532_CMD_IN_LIST_PASSIVE, scan, sizeof(scan), 32, PN532_TARGET_TIMEOUT_MS, now);
            break;
        }
        case APP_RELEASE:
        {
            (void)pn_start(PN532_CMD_IN_RELEASE, release, sizeof(release), 1, PN532_COMMAND_TIMEOUT_MS, now);
            break;
        }
        default: 
            break;
    }
}

int main(void)
{
    spi2_gpio_init(&spi_gpio_t);
    spi2_periph_init(&spi2_t);
    spi2_t.State = SPI_STATE_READY;
    
    if (SysTick_Config(PN532_CORE_HZ / 1000U)) { Error_Handler(); } // This sets systick timer to generate interrupt per 1ms
    gpio_set_pin_level(GPIOB, GPIO_PIN_12, 0); /* Original CS wake pulse. */
    app_since = tick_ms;

    while(1)
    {
        uint32_t now = tick_ms;
        pn_poll(now);
        app_poll(now);
        /* Other bounded foreground work can run here during every wait. */
    }
}
