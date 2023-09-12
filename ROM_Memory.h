#pragma once
#include <fstream>
#include <string.h>
#include <iostream>
#include "Bus.h"
#include "Observer.h"

const std::string DEFAULT_ROM_MEMORY_FILE_PATH = "D:/Desktop/ROM_Memory.txt";

class Bus;

/* Using a python script we will make a raw file which will mimic the ROM memory (~32kb of memory) */
class ROM_Mem : public Observer
{
public:
	ROM_Mem(const std::string& fileName);
	~ROM_Mem();

	void onBusRead(uint16_t address);
	/* Read Only Memory - don't need this function to be implemented */
	void onBusWrite(uint16_t address, uint8_t data);

private:
	Bus* m_bus;
	std::string m_fileName;
	std::ifstream m_file;
};