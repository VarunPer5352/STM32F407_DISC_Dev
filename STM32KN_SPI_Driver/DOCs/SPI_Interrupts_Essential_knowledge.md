# INTERRUPT EVENT TYPE-01: TRANSMIT TX BUFFER READY TO BE LOADED!
### TXE and TXEIE — SPI transmit interrupt

- **TXE (Transmit buffer empty)** is controlled by hardware, whether interrupts are enabled or not.
  - **1:** The transmit buffer can accept data.
  - Writing data to `DR` clears TXE.
  - When hardware moves that data into the shift register, TXE becomes **1** again.
  - It stays **1** until another write; it doesn’t toggle continuously by itself.

- **TXEIE (TXE interrupt enable)** is controlled by software.
  - **0:** No interrupt request from TXE.
  - **1:** When TXE is also **1**, SPI requests an interrupt through the NVIC.

**Process:**

```text
Software enables TXEIE
        ↓
TXE = 1 → interrupt requested
        ↓
ISR writes next data item into DR → TXE clears
        ↓
Hardware moves data into shift register → TXE sets again
        ↓
Repeat until all data items are supplied
        ↓
Software disables TXEIE to stop further TXE interrupts
```

**TXE means buffer space is available—not that transmission on MOSI has finished.**

# ----------------------------------------------------------------------------
VVI-Misc:
**Both can contain your logic. The difference is who calls them.**

| Function | Who calls it? | Purpose |
|---|---|---|
| **Interrupt handler / ISR** | CPU enters it through the interrupt vector | Respond to a hardware interrupt |
| **Callback** | Driver or library calls a function you supplied | Notify your application about an event |

They often work together:

```text
Hardware interrupt
        ↓
Interrupt handler runs
        ↓
Driver services the hardware
        ↓
Driver calls your registered callback
        ↓
Your application reacts
```

So your memories fit:

- **`TC3_Handler()`** could be the actual timer interrupt entry point, where you wrote your logic directly.
- **UART `RXCB`** could be a callback registered with the UART driver, which calls it when a receive event occurs. The exact event depends on that driver.

In your current GPIO setup:

```text
EXTI0_IRQHandler()       ← actual ISR
    gpio_irq_handle(0)  ← ordinary helper that clears the flag
    Your LED logic      ← application action directly inside the ISR
```

You could move the LED logic into a callback, but **you don’t need a callback just because you use interrupts**.

Also, the name alone doesn’t decide: your `gpio_irq_handle()` is called “handle,” but the CPU doesn’t enter it directly. And a callback called from an ISR still executes in interrupt context.


Think of the chain:

```text
1. Configure SPI + NVIC
       |
       |   "CPU is ready to respond"
       v
2. Start a specific transfer
       |
       |   "Here is my buffer, send these N bytes"
       v
3. Enable TXE interrupt
       |
       v
4. SPI hardware has TXE = 1
       |
       v
5. IRQ fires
       |
       v
6. ISR writes first byte to SPI->DR
       |
       v
7. Hardware shifts it
       |
       v
8. TXE becomes 1 again
       |
       v
9. IRQ fires again
       |
       v
10. ISR writes next byte
```

# ------------------------------------------------------------------------------
**When you're NOT transmitting anything, what is TXE usually?**

`TXE = 1`, because the transmit buffer is empty and ready to accept data.

So imagine the transfer finishes:

```text
TxXferCount = 0
TX buffer empty -> TXE = 1
TXEIE still     = 1
```

Therefore:

```text
TXE && TXEIE
 1  &&   1
     ↓
interrupt request!
```

ISR runs, finds nothing to send, returns...

But TXE is **still 1** because nobody wrote another byte to DR.

So:

```text
IRQ -> ISR -> return
        ↓
TXE still 1 + TXEIE still 1
        ↓
IRQ again
        ↓
ISR again
...
```

You can basically create an **interrupt storm** and waste the CPU.

That's why the pattern is:

```text
Starting TX:
    TXEIE = 1

Last byte has been supplied:
    TXEIE = 0
```

Notice we don't clear **TXE** ourselves. We disable our interest in TXE by clearing **TXEIE**.

Small question: when `spi_transfer_data_it()` enables `TXEIE`, and TXE was already `1` because SPI was idle, what do you expect to happen immediately?
