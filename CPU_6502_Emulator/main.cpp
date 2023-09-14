/* This program represents a good example / exercise to understand how does the first famillies of microprocessors works, and what happens under the hood: */
/* 
 The final goal would be to mimic (emulate) the behavior of the 6502 microprocessor, and be able to execute the following code:
 (the memory / the steps would be available for the user in the console)


; Initialize A register to 6
A9 06    ; LDA #$06

; Decrement A register
C6       ; DEC A

; Store A in RAM memory at address $0100
8D 00 01 ; STA $0100

; Jump (JSR) to label "lbl" (address $800A)
20 77 80 ; JSR $8077

; Continue after executing subroutine

; Jump (JMP) to address $8000
4C 00 80 ; JMP $8000

; Subroutine "lbl" starts at address $800A

lbl:
; Load A register with 11
A9 0B    ; LDA #$0B

; Decrement A register
C6       ; DEC A

; Store A in RAM memory at the next available address ($0101)
8D 01 01 ; STA $0101

; Return from subroutine
60       ; RTS

*/

#include <iostream>
#include "CPU_6502.h"
#include "ROM_Memory.h"
#include "RAM_Memory.h"


int main()
{
	ROM_Mem Main_Rom(DEFAULT_ROM_MEMORY_FILE_PATH);
	RAM_Mem Main_Ram(DEFAULT_RAM_MEMORY_FILE_PATH);
	Bus* Main_Bus = Bus::GetInstance();
	Main_Bus->attachNewPeripheralToBus(&Main_Rom);
	Main_Bus->attachNewPeripheralToBus(&Main_Ram);
	CPU_6502* CPU_Emulator = new CPU_6502(Main_Bus);

	/* tested CPU_Emulator->reset(); */
	// CPU_Emulator->reset();

	CPU_Emulator->run();

	return 0;
}