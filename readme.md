<!DOCTYPE html>
<html>
<head>
    <title>6502 Microprocessor Emulator</title>
</head>
<body>
    <h1>6502 Microprocessor Emulator</h1>
    
    <h2>Project Overview</h2>
    <p>
        This project is an emulator for the 6502 microprocessor. The program is designed to execute (hexadecimal format) machine code.     
        It utilizes two key files: ROM_Memory.txt, which simulates the hardware ROM memory where the machine code is stored, and RAM_Memory.txt, which is instrumental in managing the program's state.
    </p>

    <h2>Resources</h2>
    <ul>
        <li><a href="https://eater.net/datasheets/w65c02s.pdf" target="_blank">6502 Datasheet</a></li>
        <li><a href="https://wiki.cdot.senecacollege.ca/wiki/6502#:~:text=The%206502%20is%20an%208,pages%20of%20256%20bytes%20each." target="_blank">6502 Wiki</a></li>
        <li><a href="https://sta.c64.org/cbm64mem.html" target="_blank">6502 Memory Map</a></li>
        <li><a href="http://www.6502.org/tutorials/6502opcodes.html" target="_blank">6502 OpCodes</a></li> 
        <li><a href="http://www.6502.org/tutorials/6502opcodes.html" target="_blank">Opcodes Explain in datail</a></li>
        <li><a href="https://www.pagetable.com/c64ref/6502/?tab=2#LDA" target="_blank">Opcodes Explain in datail</a></li>
    </ul>

    <h2>6502 Microprocessor Specifications</h2>
    <ul>
        <li>8-bit CPU</li>
        <li>able to access 64 KB of memory</li>
        <li>16-bit address bus</li>
        <li>8-bit data bus</li>
        <li>little endian (least significant byte first)</li>
    </ul>

    <h2>Memory Layout</h2>
    <ul>
        <li><strong>Zero Page:</strong> The first 256 bytes ($0x00 to $0xFF) of the CPU's addressable memory space used for fast access.</li>
        <li><strong>System Stack:</strong> The second page of memory ($0100 - $01FF).
            <ul>
                <li>The stack location is fixed and cannot be moved.</li>
                <li>Pushing bytes to the stack decrements the stack pointer, while pulling bytes increments it.</li>
                <li>There is no mechanism to detect stack overflow.</li>
            </ul>
        </li>
        <li><strong>Reserved Memory:</strong> Last 6 bytes of memory ($FFFA to $FFFF) are reserved for various purposes:
            <ul>
                <li>Interrupt handler ($FFFA/B)</li>
                <li>Power-on reset location ($FFFC/D)</li>
                <li>BRK/interrupt request handler ($FFFE/F)</li>
            </ul>
        </li>
    </ul>

    <h2>Registers</h2>
    <ul>
        <li><strong>Accumulator:</strong> 8-bit register used for arithmetic and logical operations.</li>
        <li><strong>Index Register X:</strong> 8-bit register, commonly used for counting or memory access offsets.</li>
        <li><strong>Index Register Y:</strong> 8-bit register, similar to Register X.</li>
        <li><strong>Program Counter (PC):</strong> 16-bit register that points to the next instruction to be executed.</li>
        <li><strong>Stack Pointer (SP):</strong> 8-bit register holding the low 8 bits of the next free location on the stack.</li>
        <li><strong>Processor Status (P):</strong> Special status register containing various flags (Carry, Zero, Interrupt Disable, Decimal Mode, Break Command, Overflow, Negative).</li>
    </ul>

    <h2>Processor Status Register (P)</h2>
    <pre>
        <code>
PROCESSOR STATUS REGISTER      <- BYTE
   7  6  5  4  3  2  1  0      <- BIT
   N  V     B  D  I  Z  C      <- FLAG
        </code>
    </pre>
    <ul>
        <li><strong>Carry Flag:</strong> Set if an overflow or underflow from bit 7 occurs.</li>
        <li><strong>Zero Flag:</strong> Set if the result of the last operation is zero.</li>
        <li><strong>Interrupt Disable:</strong> Set if the program has executed 'Set Interrupt Disable'. Processor won't respond to other interrupts until cleared.</li>
        <li><strong>Decimal Mode:</strong> Controls Binary Coded Decimal arithmetic.</li>
        <li><strong>Break Command:</strong> Set after executing BRK instruction and generating an interrupt for processing.</li>
        <li><strong>Overflow Flag:</strong> Set during arithmetic operations for invalid 2's complement results.</li>
        <li><strong>Negative Flag:</strong> Set if the last operation's bit 7 is one.</li>
    </ul>

    <h2>Execution Steps</h2>
    <ol>
        <li>Read a byte (from the ROM_Memory) and store in Program Counter.</li>
        <li>Decode the opcode using the lookup table, the cycle count and the function responsable for the execution.</li>
        <li>Read additional bytes as needed to complete the instruction.</li>
        <li>Execute the specific computation.</li>
        <li>Count cycles, wait, and complete.</li>
        <p>
            <b>To generate the ROM_Memory.txt and RAM_Memory.txt files -> run the python script "generate_memory.py" (don't forget to modify the paths! )
        </p>
    </ol>

    <h2>Images</h2>
    <!-- Add any relevant images here with appropriate HTML tags -->

</body>
</html>
