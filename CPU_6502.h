#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include <iostream>
#include "Bus.h"
#include "Observer.h"

class Bus;

static const uint8_t HEX_INCREMENT = 16;
static const uint16_t RAM_STARTING_ADDRESS = 0x100;
static const uint16_t STACK_DEFAULT_ADDRESS = 0xFD;
static const uint16_t DEFAULT_RESET_VECTOR_ADDRESS = 0xFFFC; /* First PC address to be read after reset */

class CPU_6502
{
public:
	/* Constructor - Destructor */
	CPU_6502(Bus* bus);
	~CPU_6502();

	void run();
	/* CPU sends to the Bus an address (16 bits) and receives data (8 bits) */
	uint8_t read(uint16_t a);
	/* Cpu sends to the Bus some data and a specific address where to store it */
	void write(uint16_t address, uint8_t data);
	/* Reset function used to get the CPU in the initial state */
	void reset();
	/* Simulate the hardware clock sequence */
	/* Each instruction requires a number of clocks to be executed */
	void clock();
	/* Get specific no. of instructions: */
	uint8_t getOpcodeCycles(uint16_t);
	/* Return a pointer function to every Opcode specific execution */
	// uint8_t (CPU_6502::* getInstructionFunction(uint8_t opcode))();

	void executeInstruction(uint8_t opcode);

	uint8_t remaining_cycles = 0;
	uint8_t current_opcode = 0x00;
	uint8_t current_operand = 0x00;

	/* Instructions */
	uint8_t LDA();   uint8_t DEC();
	uint8_t STA();   uint8_t JMP();
	uint8_t JSR();   uint8_t RTS();

	struct INSTRUCTION
	{
		std::string instruction_name;
		uint8_t instruction_opcode;
		uint8_t instruction_cycles_no;
		/* Every instruction have a different exectuion */
		uint8_t(CPU_6502::*operation)(void) = nullptr;
	};

	std::vector<INSTRUCTION> lookup;


private:
	/* Registers */
	uint8_t m_SP;					// Stack Pointer
	uint16_t m_PC;					// Program Counter
	uint8_t m_STATUS;				// Status Register
	uint8_t m_REGISTER_A;			// Acumulator Register
	uint8_t m_REGISTER_X;			// X Register
	uint8_t m_REGISTER_Y;			// Y Register

	/* Reference to the Bus */
	Bus* m_bus;
};