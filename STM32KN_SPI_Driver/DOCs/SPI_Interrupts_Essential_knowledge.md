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