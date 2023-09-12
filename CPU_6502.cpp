#include "CPU_6502.h"
#include <iomanip>


CPU_6502::CPU_6502(Bus* bus)
{
	this->lookup =
	{
		{"LDA", 0xA9, 2, &CPU_6502::LDA},
		{"STA", 0x8D, 2, &CPU_6502::STA},
		{"JSR", 0x20, 2, &CPU_6502::JSR},
		{"DEC", 0xC6, 2, &CPU_6502::DEC},
		{"JMP", 0x4C, 2, &CPU_6502::JMP},
		{"RTS", 0x60, 2, &CPU_6502::RTS},
	};

	/* Set to Default values */
	this->m_SP = STACK_DEFAULT_ADDRESS;	// Stack Pointer
	this->m_PC = 0x00;					// Program Counter
	this->m_STATUS = 0x00;				// Status Register
	this->m_REGISTER_A = 0x00;			// Acumulator Register
	this->m_REGISTER_X = 0x00;			// X Register
	this->m_REGISTER_Y = 0x00;			// Y Register

	this->m_bus = bus;
	std::cout << "CPU Power On!\n" << std::endl;
	std::cout << "Address:" << " R/W " << "DATA:" << std::endl;

	/* Starting from the address stored in the Reset Vector, create the first value for PC */
	uint16_t low_PC_byte = this->read(DEFAULT_RESET_VECTOR_ADDRESS);
	uint16_t high_PC_byte = this->read(DEFAULT_RESET_VECTOR_ADDRESS + 1);

	/* If the operation were made in HEX */
	//this->m_PC = (high_PC_byte << 8 | low_PC_byte);
	/* Othewise mimic the HEX value and calculate the coresponding DEC value: 256 = 16 ^ 2 */
	this->m_PC = high_PC_byte * 256 + low_PC_byte;
}

CPU_6502::~CPU_6502()
{
	delete this->m_bus;
	std::cout << "CPU Power Off!" << std::endl;
}

uint8_t CPU_6502::read(uint16_t temp_address)
{
	return this->m_bus->readBus(temp_address);
}

/* 6502 Have different buses for address and data - would use a single function for both */
void CPU_6502::write(uint16_t address, uint8_t data)
{
	return this->m_bus->writeBus(address, data);
}

void CPU_6502::reset()
{
	std::cout << "Resetting the microcontroller...\n" << std::endl;
	std::cout << "Address:" << " R/W " << "DATA:" << std::endl;
	/* Set the default values */
	this->m_REGISTER_A = 0;
	this->m_REGISTER_X = 0;
	this->m_REGISTER_Y = 0;
	this->m_SP = STACK_DEFAULT_ADDRESS;
	this->m_STATUS = 0x00;   /* | U; */

	/* Starting from the address stored in the Reset Vector, create the first value for PC */
	uint16_t low_PC_byte = this->read(DEFAULT_RESET_VECTOR_ADDRESS);
	uint16_t high_PC_byte = this->read(DEFAULT_RESET_VECTOR_ADDRESS + 1);	
	
	/* If the operation were made in HEX */
	//this->m_PC = (high_PC_byte << 8 | low_PC_byte);
	/* Othewise mimic the HEX value and calculate the coresponding DEC value: 256 = 16 ^ 2 */
	this->m_PC = high_PC_byte * 256 + low_PC_byte;

}

uint8_t CPU_6502::getOpcodeCycles(uint16_t opcode_to_find)
{
	for (const INSTRUCTION& instruction : this->lookup)
	{
		if (instruction.instruction_opcode == opcode_to_find) {
			return instruction.instruction_cycles_no;
			break;
		}
	}

	return 0;
}

/* Function used to return a pointer function if it is found in the lookup table */
//uint8_t(CPU_6502::* CPU_6502::getInstructionFunction(uint8_t opcode))() {
//	for (const INSTRUCTION& instr : lookup) {
//		if (instr.instruction_opcode == opcode) {
//			return instr.operation;
//		}
//	}
//	return nullptr; // Return nullptr if opcode not found
//}

// Function to find and execute the function based on opcode
void CPU_6502::executeInstruction(uint8_t opcode) {
	for (const INSTRUCTION& instr : lookup) {
		if (instr.instruction_opcode == opcode) {
			(this->*instr.operation)(); // Call the found function
			return;
		}
	}
	std::cout << "Instruction not found!" << std::endl;
}

void CPU_6502::clock()
{
	/* When there are no more cycles -> increment de PC */
	if (this->remaining_cycles == 0)
	{
		this->current_opcode = this->read(this->m_PC);
		/* Increment the Program Counter */
		this->m_PC = this->m_PC + 1;
		/* Every instruction have a different no. of clock cycles */
		/* First cycle is already executed - the first one fetches the opcode of the instruction */
		this->remaining_cycles = this->getOpcodeCycles(this->current_opcode);
	}
	else
	{
		this->executeInstruction(this->current_opcode);
	}
	--this->remaining_cycles;
}

void CPU_6502::run()
{
	/* After Poweing On the Microprocessor -> read from ROM instructions: */
	while (true)
	{
		clock();
	}
}


uint8_t CPU_6502::LDA()
{
	/* This instruction will take 2 cycles: */
	/* - one cycle to fetch the opcode */
	/* - one cycle to fetch the operand (next byte) */
	this->m_REGISTER_A = this->read(this->m_PC);
	/* Increment the Program Counter*/
	this->m_PC = this->m_PC + 1;
	/* Set corresponding flags */
	/* Set Z if A == 0 */
	/* Set N if bit 7 of A is set */

	return 0;
}

uint8_t CPU_6502::STA()
{
	/* Store Accumulator at the (next fetched) specific memory address */
	uint16_t temp_address;
	/* Get the highr part of the address */
	temp_address = this->m_bus->readBus(this->m_PC); /* higher part of the address */
	/* Get the lower part of the address */
	this->m_PC = this->m_PC + 1;
	/* Address is little endian. This line calculates the DEC value of the address: */
	temp_address = temp_address + (this->m_bus->readBus(this->m_PC)) * 256;

	/* Write at specific memory address, coresponding data */
	this->write(temp_address, this->m_REGISTER_A);
	this->m_PC = this->m_PC + 1;

	return 0;
}

uint8_t CPU_6502::JSR()
{
	uint16_t temp_address;
	/* Get the highr part of the address */
	temp_address = this->m_bus->readBus(this->m_PC); /* higher part of the address */
	/* Get the lower part of the address */
	this->m_PC = this->m_PC + 1;
	/* Address is little endian. This line calculates the DEC value of the address: */
	temp_address = temp_address + (this->m_bus->readBus(this->m_PC)) * 256;
	
	this->m_PC += 1;
	this->m_SP = STACK_DEFAULT_ADDRESS;
	this->write(RAM_STARTING_ADDRESS + this->m_SP, (this->m_PC >> 8) & 0x00FF);
	this->m_SP -= 1;
	this->write(RAM_STARTING_ADDRESS + this->m_SP, this->m_PC & 0x00FF);
	this->m_SP -= 1;

	this->m_PC = temp_address;

	return 0;
}

uint8_t CPU_6502::DEC()
{
	/* CE Opcode used to decrement the A register*/
	this->m_REGISTER_A = this->m_REGISTER_A - 1;
	return 0;
}

uint8_t CPU_6502::JMP()
{
	uint16_t temp_address = 0x00;
	temp_address = this->read(this->m_PC);
	this->m_PC += 1;
	temp_address = temp_address + this->read(this->m_PC) * 256;

	this->m_PC = temp_address;

	return 0;
}

uint8_t CPU_6502::RTS()
{
	this->m_SP += 1;
	this->m_PC = (uint16_t)this->read(RAM_STARTING_ADDRESS + this->m_SP);
	this->m_SP += 1;
	this->m_PC |= (uint16_t)this->read(RAM_STARTING_ADDRESS + this->m_SP) << 8;

	return 0;
}