#include "ROM_Memory.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>


ROM_Mem::ROM_Mem(const std::string& fileName)
	: m_fileName(fileName)
{
	this->m_file.open(m_fileName);
	if (m_file.is_open())
		std::cout << "ROM Memory Successfully Opened!" << std::endl;
}

ROM_Mem::~ROM_Mem()
{
	if (this->m_file.is_open())
	{
		this->m_file.close();
	}
}

void ROM_Mem::onBusRead(uint16_t address)
{
	if (address >= 0x8000 && address <= 0xFFFF)
	{
		/* If the ROM Memory (file) does not exists or it is unable to be open -> exit! */
		uint16_t value = NO_OPCODE;
		if (!this->m_file.is_open())
		{
			std::cout << "Failed to open the ROM Memory!" << std::endl;
			this->m_bus->setBusInfo(value); /* "no opcode" */
			return;
		}

		/* Find the nearest multiple of 16 (0x10 in hexadecimal) that is less than or equal to intValue */
		uint16_t ROM_Memory_line = address / 16 * 16;
		/* Calculate on which position is the needed byte */
		uint8_t ROM_Memory_remainder = address - ROM_Memory_line;

		/* The ROM Memory file have the following fields: [address] [4*space] [16 * bytes] */
		/* Example:                                       0000FFF0            EA EA EA EA EA EA EA EA EA EA EA EA 00 80 EA EA */
		std::stringstream ss;
		ss << std::uppercase << std::hex << ROM_Memory_line; // Set the stream to output in hexadecimal format
		std::string hexString = ss.str(); // Get the resulting hexadecimal string
		std::string searchAddressPattern = hexString + "    ";

		/* Reset the File Pointer - otherwise after the first iteration the program will jump over it */
		/* File Pointer will point to the end of the file */
		this->m_file.clear();  // Clear any error flags
		this->m_file.seekg(0); // Set the file pointer to the beginning

		std::string line;
		while (std::getline(this->m_file, line))
		{
			auto pattern_found = line.find(searchAddressPattern);
			if (pattern_found != std::string::npos)
			{
				std::string valueString = line.substr(pattern_found + searchAddressPattern.length(), 47);
				std::string ceva = valueString.substr(ROM_Memory_remainder * 3, 2);
				value = std::stoi(ceva, nullptr, 16);
				break;
			}
		}
		this->m_bus->setBusInfo(value);
	}
}

void ROM_Mem::onBusWrite(uint16_t address, uint8_t data)
{
	// nothing
}