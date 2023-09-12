#pragma once
#include <fstream>
#include <string.h>
#include <iostream>
#include <iomanip>
#include "Bus.h"
#include "Observer.h"

/* RAM Memory -> 0x000 -> 0x03FF (arbitrary - 3 times the stack dimension) */
/* Stack Memory -> 0x0100 -> 0x01FF (harcoded in the microprocessor) */
const std::string DEFAULT_RAM_MEMORY_FILE_PATH = "D:/Desktop/RAM_Memory.txt";

class Bus;

/* Using a python script we will make a raw file which will mimic the ROM memory (~32kb of memory) */
class RAM_Mem : public Observer
{
public:
	RAM_Mem(const std::string& fileName);
	~RAM_Mem();

	void onBusRead(uint16_t address);
	void onBusWrite(uint16_t address, uint8_t data);
	void setRAMValue(std::string& tempString, uint16_t data, uint8_t location);
	std::string  hexAddressToString(uint16_t a);

private:
	Bus* m_bus;
	std::string m_fileName;
	std::ifstream m_file;
	/* Flag veriable used to acknowledge that a write command will follow */
	bool m_currentPeripheral;
	uint16_t m_currentAddress;
};
