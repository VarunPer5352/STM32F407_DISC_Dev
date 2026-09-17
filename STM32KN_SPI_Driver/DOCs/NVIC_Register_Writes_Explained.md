# Why NVIC register writes differ from ordinary variables

This guide uses your STM32F407 custom register-level drivers. No HAL is required. Your bit-position macros remain bit positions: use `1U << position` to construct a mask.

## 1. The key distinction: what C sends versus what hardware does

Your intuition is correct **for ordinary storage**:

```c
value = mask;      /* Replace the stored value. */
value |= mask;     /* Preserve existing ones and set the selected bits. */
value &= ~mask;    /* Preserve other bits and clear the selected bits. */
```

But a hardware register is an address connected to circuitry. That circuitry decides what a write means. Some registers store the supplied bits. Others interpret those bits as requests.

**Both `=` and `|=` write a whole value of the accessed width. `|=` does not send an instruction saying "change only this bit".**

For a simple register expression, this:

```c
REG |= mask;
```

performs these steps conceptually:

```c
uint32_t temporary = REG;  /* Read hardware. */
temporary = temporary | mask;
REG = temporary;           /* Write hardware. */
```

Those old bits are written back too. Whether that is harmless depends on the register.

Your custom structure definitions map names to addresses. They do not implement or override the hardware's write behavior.

## 2. Start with ordinary storage

Imagine an ordinary eight-bit variable:

```text
Old value:         0100 0000
Mask:              0001 0000

value = mask:      0001 0000   Old bit 6 is lost.
value |= mask:     0101 0000   Old bit 6 is preserved.
```

This is why read-modify-write is useful for ordinary configuration fields. For example, during SPI initialization while SPI is disabled:

```c
SPI2->CR1 |= (1U << SPI_CR1_LSBFIRST);
```

Your `SPI_CR1_LSBFIRST` is `7U`, so this constructs mask `0x80`. It preserves the other configuration bits while setting bit 7.

An assignment to `CR1` would instead supply the entire configuration word. That can be intentional when configuring everything together, but it is unsuitable when the intention is just to change LSBFIRST.

## 3. NVIC enable and disable registers are action interfaces

The NVIC keeps an interrupt-enable state internally. ISER and ICER provide two ways to change that same state:

| Register | Writing 1 | Writing 0 | Reading |
|---|---|---|---|
| ISER | Enable selected interrupt | No change | Current enable state |
| ICER | Disable selected interrupt | No change | Current enable state |

They do not behave like two independent RAM variables. In particular, an ICER read reports enabled interrupts, not a history of disable requests. ST documents these rules in [PM0214, sections 4.3.2 and 4.3.3](https://www.st.com/resource/en/programming_manual/DM00046982-.pdf).

Conceptually, if `enabled` represents the hardware's internal state:

```text
Write data to ISER: enabled becomes enabled OR data
Write data to ICER: enabled becomes enabled AND NOT data
```

These are explanations of the hardware, not extra operations you must implement in C.

## 4. Your exact example: SPI2 and USART2

In your STM32F407 device definitions:

| Interrupt | IRQ number | Bank: IRQ / 32 | Bit: IRQ % 32 |
|---|---:|---:|---:|
| SPI2 | 36 | 1 | 4 |
| USART2 | 38 | 1 | 6 |

Both use bank 1. Showing just its lowest eight bits makes the arithmetic easier to see.

### Enable SPI2, then enable USART2

Assume neither is enabled initially:

```c
NVIC->ISER[1] = (1U << 4);  /* Enable SPI2. */
NVIC->ISER[1] = (1U << 6);  /* Enable USART2. */
```

```text
Initial enabled state:     0000 0000
First write data:          0001 0000
Enabled after first:      0001 0000

Second write data:         0100 0000
Enabled after second:     0101 0000
```

The second write sends zero for SPI2's bit. On ISER, zero means leave it alone. **SPI2 remains enabled.**

### Disable only SPI2: correct

```c
NVIC->ICER[1] = (1U << 4);
```

```text
Enabled before:           0101 0000
Disable request:          0001 0000
Enabled after:            0100 0000
```

Only SPI2 is disabled. USART2 keeps running.

### Disable only SPI2 using |=: incorrect

Starting again with both enabled:

```c
NVIC->ICER[1] |= (1U << 4);  /* Wrong for this purpose. */
```

```text
Read ICER:                0101 0000
OR with SPI2 mask:        0001 0000
Value written to ICER:    0101 0000
Enabled after:            0000 0000
```

Both ones become disable requests. USART2 is disabled too, even though you only mentioned SPI2 in the expression.

This is why a bug can remain invisible when testing only one interrupt, then appear when another peripheral starts using interrupts.

### Does |= on ISER cause the same problem?

In the simple case, it enables the requested interrupt and rewrites enable requests for interrupts already enabled. That usually leaves the intended state, but the read is unnecessary. Use direct assignment to express the exact enable request. A read-modify-write also creates an unnecessary opportunity for interference if another execution context changes the state between the read and write.

### Does writing zero to ICER disable everything?

No:

```c
NVIC->ICER[1] = 0U;  /* No disable requests: changes nothing. */
```

Likewise, clearing a bit with `ICER &= ~mask` is not how you disable its interrupt. The selected bit needs a written one to trigger disabling.

Your current driver correctly uses:

```c
NVIC->ISER[IRQ_number / 32] = (1U << (IRQ_number % 32));
NVIC->ICER[IRQ_number / 32] = (1U << (IRQ_number % 32));
```

These are alternatives for enable and disable, not two lines to execute together. Your local [CMSIS implementation](../Inc/core_cm4.h) also uses direct assignments in `__NVIC_EnableIRQ()` and `__NVIC_DisableIRQ()`.

## 5. Not every NVIC register behaves this way

Priority storage is different. Your `NVIC->IP[]` exposes one byte per interrupt. STM32F407 implements the upper four priority bits, giving priority values 0 through 15; lower numbers mean higher urgency.

```c
NVIC->IP[SPI2_IRQn] = (uint8_t)(3U << 4);
NVIC->IP[USART2_IRQn] = (uint8_t)(5U << 4);
```

These write different byte locations. Assigning SPI2's priority does not overwrite USART2's priority.

`|=` would be wrong when replacing an existing priority:

```text
Old encoded priority 5:    0101 0000
New encoded priority 2:    0010 0000
OR result:                0111 0000  = priority 7, not 2
```

Use assignment to replace that byte. See your device header and [PM0214 section 4.3.7](https://www.st.com/resource/en/programming_manual/DM00046982-.pdf) for priority layout.

## 6. Other STM32 registers also have special behavior

NVIC registers are not the only examples. The peripheral register descriptions in your [STM32F407 reference manual](RefManual_STM32F407Disc.pdf) describe each field's access rules.

### EXTI pending register: write one to clear

Suppose EXTI lines 2 and 5 both have pending events. You are handling line 2:

```c
EXTI->PR = (1U << 2);  /* Clear only line 2's pending flag. */
```

```text
Pending before:           0010 0100
Clear request:            0000 0100
Pending after:            0010 0000
```

But this is wrong:

```c
EXTI->PR |= (1U << 2);
```

It reads both pending ones and writes both back, clearing both flags. Line 5's event can be lost before its handler deals with it. Your current `gpio_irq_handle()` correctly uses assignment.

The label for this behavior is **write-one-to-clear**, often abbreviated W1C or shown as `rc_w1` in ST documentation.

### GPIO BSRR: separate set and reset requests

BSRR provides output actions without reading the output state first:

```c
GPIOB->BSRR = (1U << 12);          /* Set PB12 high. */
GPIOB->BSRR = (1U << (12U + 16U)); /* Reset PB12 low. */
```

Bits 0 through 15 request setting pins; bits 16 through 31 request resetting pins. Zero bits leave the corresponding output unchanged. Use assignment, not `|=`. These examples are separate operations.

Compare `GPIOB->ODR`: that holds output latch values, so assigning an entire value there changes all writable output bits. Two addresses belonging to the same GPIO peripheral can have different write behavior.

### SPI flags: sometimes the required operation is a read

For STM32F407 SPI, RXNE is cleared by reading the receive data through DR. Overrun clearing requires the documented sequence of reading DR, then SR.

Consequently, this is not a general solution:

```c
SPI2->SR &= ~(1U << SPI_SR_RXNE);  /* Not how RXNE is cleared. */
```

Use the prescribed data access or clearing sequence for the particular flag. Reading a peripheral register can itself have effects; even debugger register views need care with such registers.

### More categories you will encounter

| Access behavior | Meaning | Example |
|---|---|---|
| Ordinary read/write | Supplied bits replace stored fields | SPI configuration fields |
| Write-one-to-set | Ones request setting; zeros do nothing | NVIC ISER, ISPR |
| Write-one-to-clear | Ones request clearing; zeros do nothing | NVIC ICER, ICPR; EXTI PR |
| Write-zero-to-clear | Zeros clear selected flags; ones preserve them | Certain timer SR flags |
| Read-only | Software observes hardware state | SPI BSY |
| Action on data access | Reads/writes consume or supply data | SPI DR |
| Separate set/reset fields | One write requests individual output changes | GPIO BSRR |

Do not apply one rule to every status register, or even every bit in a register. Mixed access types and reserved bits require checking the individual field descriptions. For W0C fields, do not blindly copy the W1C recipe.

## 7. What volatile does and does not do

Your register members are volatile so accesses occur as required by the C implementation rather than being treated as ordinary cached variables.

But volatile does not:

- Change a register's electrical or hardware behavior.
- Turn `|=` into a single-bit write.
- Make a read-modify-write sequence atomic.
- Prevent an interrupt handler from changing state between the read and write.

For an ordinary R/W register shared between main code and an ISR, preserving bits with `|=` still needs appropriate coordination. A dedicated action register such as BSRR avoids that particular read-modify-write sequence.

## 8. How to decide what to write

1. Find the exact register and field in the reference or programming manual.
2. Read both the read behavior and the write behavior. A broad "R/W" label alone is insufficient.
3. For ordinary configuration fields, preserve unrelated fields when updating part of a register.
4. For write-one actions, write a mask containing only the requested actions.
5. For flags cleared by reads or sequences, perform that documented sequence.
6. Respect access widths, reserved bits, and restrictions on changing settings while a peripheral runs.

For your current code, remember these concrete examples:

```c
/* Ordinary configuration: preserve other bits, during initialization. */
SPI2->CR1 |= (1U << SPI_CR1_LSBFIRST);

/* Action: enable only the selected interrupt. */
NVIC->ISER[SPI2_IRQn / 32] = (1U << (SPI2_IRQn % 32));

/* Action: disable only the selected interrupt. */
NVIC->ICER[SPI2_IRQn / 32] = (1U << (SPI2_IRQn % 32));

/* Action: acknowledge only the selected EXTI event. */
EXTI->PR = (1U << pin_number);

/* Ordinary priority byte: replace this interrupt's priority. */
NVIC->IP[SPI2_IRQn] = (uint8_t)(3U << 4);
```

## References in this project

- [Your SPI driver](../KN_drivers/src/spi_driver.c): `spi_irq_config()`.
- [Your GPIO driver](../KN_drivers/src/gpio_driver.c): `gpio_irq_config()` and `gpio_irq_handle()`.
- [Local CMSIS core header](../Inc/core_cm4.h): NVIC enable, disable, and priority implementations.
- [Your STM32F407 device definitions](../KN_drivers/inc/stm32f407xx.h): interrupt numbers and implemented priority bits.
- [ST PM0214](https://www.st.com/resource/en/programming_manual/DM00046982-.pdf): NVIC register descriptions, section 4.3.
- [Local ST reference manual](RefManual_STM32F407Disc.pdf): GPIO BSRR/ODR, EXTI PR, SPI SR/DR, and timer status-field descriptions.
