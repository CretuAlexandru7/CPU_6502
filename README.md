# 6502 Microprocessor Emulator

## Project Overview

 This project is an emulator for the 6502 microprocessor. The program is designed to execute (hexadecimal format) machine code.     
 It utilizes two key files: 
- **ROM_Memory.txt**: which simulates the hardware ROM memory where the machine code is stored;
- **RAM_Memory.txt**: which is instrumental in managing the program's state;

## Resources

- [6502 Datasheet](https://eater.net/datasheets/w65c02s.pdf)
- [6502 Wiki](https://wiki.cdot.senecacollege.ca/wiki/6502#:~:text=The%206502%20is%20an%208,pages%20of%20256%20bytes%20each.)
- [6502 Memory Map](https://sta.c64.org/cbm64mem.html)
- [6502 OpCodes](http://www.6502.org/tutorials/6502opcodes.html)
- [Opcodes Explained in Detail](https://www.pagetable.com/c64ref/6502/?tab=2#LDA)

## 6502 Microprocessor Specifications

- 8-bit CPU
- Able to access 64 KB of memory
- 16-bit address bus
- 8-bit data bus
- Little endian (least significant byte first)

## Memory Layout

- **Zero Page:** The first 256 bytes ($0x00 to $0xFF) of the CPU's addressable memory space used for fast access.
- **System Stack:** The second page of memory ($0100 - $01FF).
    - The stack location is fixed and cannot be moved.
    - Pushing bytes to the stack decrements the stack pointer, while pulling bytes increments it.
    - There is no mechanism to detect stack overflow.
- **Reserved Memory:** Last 6 bytes of memory ($FFFA to $FFFF) are reserved for various purposes:
    - Interrupt handler ($FFFA/B)
    - Power-on reset location ($FFFC/D)
    - BRK/interrupt request handler ($FFFE/F)

## Registers

- **Accumulator:** 8-bit register used for arithmetic and logical operations.
- **Index Register X:** 8-bit register, commonly used for counting or memory access offsets.
- **Index Register Y:** 8-bit register, similar to Register X.
- **Program Counter (PC):** 16-bit register that points to the next instruction to be executed.
- **Stack Pointer (SP):** 8-bit register holding the low 8 bits of the next free location on the stack.
- **Processor Status (P):** Special status register containing various flags (Carry, Zero, Interrupt Disable, Decimal Mode, Break Command, Overflow, Negative).

## Processor Status Register (P)

<pre>
PROCESSOR STATUS REGISTER      <- BYTE
   7  6  5  4  3  2  1  0      <- BIT
   N  V     B  D  I  Z  C      <- FLAG
</pre>
     

- **Carry Flag:** Set if an overflow or underflow from bit 7 occurs.
- **Zero Flag:** Set if the result of the last operation is zero.
- **Interrupt Disable:** Set if the program has executed 'Set Interrupt Disable'. Processor won't respond to other interrupts until cleared.
- **Decimal Mode:** Controls Binary Coded Decimal arithmetic.
- **Break Command:** Set after executing BRK instruction and generating an interrupt for processing.
- **Overflow Flag:** Set during arithmetic operations for invalid 2's complement results.
- **Negative Flag:** Set if the last operation's bit 7 is one.

## Execution Steps

1. Read a byte (from the ROM_Memory) and store in Program Counter.
2. Decode the opcode using the lookup table, the cycle count, and the function responsible for the execution.
3. Read additional bytes as needed to complete the instruction.
4. Execute the specific computation.
5. Count cycles, wait, and complete.

**To generate the `ROM_Memory.txt` and `RAM_Memory.txt` files, run the Python script "generate_memory.py" (don't forget to modify the paths!).**

## Images

<!-- Add any relevant images here with appropriate Markdown image tags -->


