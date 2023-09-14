#include "RAM_Memory.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>


RAM_Mem::RAM_Mem(const std::string& fileName)
	: m_fileName(fileName)
{
	this->m_file.open(m_fileName);
	if (m_file.is_open())
		std::cout << "RAM Memory Successfully Opened!" << std::endl;
	this->m_currentPeripheral = false;
}

RAM_Mem::~RAM_Mem()
{
	if (this->m_file.is_open())
	{
		this->m_file.close();
	}
}

/* If the received address coresponds to the chip address, set data into RAM Memory */
void RAM_Mem::onBusWrite(uint16_t address, uint8_t data)
{
	/* 32 bytes of RAM memory128 bytes of memory */
	if (address >= 0x0100 && address <= 0x03FF && this->m_currentPeripheral == false)
	{
		/* The corresponding address is from the RAM Memory */
		this->m_currentPeripheral = true;
		this->m_currentAddress = address;
	}
	
	if(this->m_currentPeripheral)
	{
		/* If the RAM Memory (file) does not exists or it is unable to be open -> exit! */
		uint16_t value = NO_OPCODE;
		if (!this->m_file.is_open())
		{
			std::cout << "Failed to open the RAM Memory!" << std::endl;
			this->m_bus->setBusInfo(value); /* "no opcode" */
			return;
		}

		/* Find the nearest multiple of 8 that is less than or equal to intValue */
		uint16_t RAM_Memory_line = this->m_currentAddress / 8 * 8;
		/* Calculate on which position is the needed byte */
		uint8_t RAM_Memory_remainder = this->m_currentAddress - RAM_Memory_line;

		/* The ROM Memory file have the following fields: [address] [4*space] [16 * bytes] */
		/* Example:                                       0000FFF0            EA EA EA EA EA EA EA EA EA EA EA EA 00 80 EA EA */
		std::stringstream ss;
		ss << std::uppercase << std::hex << RAM_Memory_line; // Set the stream to output in hexadecimal format
		std::string hexString = ss.str(); // Get the resulting hexadecimal string
		std::string searchAddressPattern = hexString + "    ";

		/* Reset the File Pointer - otherwise after the first iteration the program will jump over it */
		/* File Pointer will point to the end of the file */
		this->m_file.clear();  // Clear any error flags
		this->m_file.seekg(0); // Set the file pointer to the beginning

		std::string line = "";
		bool line_modified = false;
		std::string inputFilePath = DEFAULT_RAM_MEMORY_FILE_PATH;
		uint8_t insertIndex = DEFAULT_RAM_MEMORY_FILE_PATH.length() - 4;
		std::string tempFilePath = DEFAULT_RAM_MEMORY_FILE_PATH;
		tempFilePath.insert(insertIndex, 1, '_');
		std::ofstream tempFile(tempFilePath);

		while (std::getline(this->m_file, line))
		{
			auto pattern_found = line.find(searchAddressPattern);
			if (pattern_found != std::string::npos)
			{				
				std::string valueString = line.substr(pattern_found + searchAddressPattern.length(), 47);
				setRAMValue(valueString, data, RAM_Memory_remainder);
				line = this->hexAddressToString(RAM_Memory_line) + "    ";
				line = line + valueString;
				line_modified = true;
			}
			tempFile << line << std::endl;
		}
		tempFile.close();

		if (line_modified)
		{
			this->m_file.close();
			std::remove(inputFilePath.c_str());
			std::rename(tempFilePath.c_str(), inputFilePath.c_str());
			this->m_file.open(inputFilePath);
		}

		/* Reset the flag for the next read */
		this->m_currentPeripheral = false;
	}
	else
	{
		/* Do Nothing */
	}
}

void RAM_Mem::setRAMValue(std::string& tempString, uint16_t data, uint8_t location)
{
	std::string result;
	std::string value;
	size_t currentIndex = 0;

	for (size_t i = 0; i < tempString.length(); i += 3) {
		if (currentIndex == location) {
			std::stringstream ss;
			ss << std::hex << static_cast<int>(data);
			ss >> value;
			if (value.length() <= 1)
				value.insert(0, " ");
			result += value;
		}
		else {
			result += tempString.substr(i, 2); // Extract the next two characters
		}

		// Add a space after each value (except the last one)
		if (i < tempString.length() - 3) {
			result += " ";
		}

		currentIndex++;
	}
	tempString = result;
}

void RAM_Mem::onBusRead(uint16_t address)
{
	if (address >= 0x000 && address <= 0x03FF)
	{
		/* If the RAM Memory (file) does not exists or it is unable to be open -> exit! */
		uint16_t value = NO_OPCODE;
		if (!this->m_file.is_open())
		{
			std::cout << "Failed to open the ROM Memory!" << std::endl;
			this->m_bus->setBusInfo(value); /* "no opcode" */
			return;
		}

		/* Find the nearest multiple of 16 (0x10 in hexadecimal) that is less than or equal to intValue */
		uint16_t RAM_Memory_line = address / 8 * 8;
		/* Calculate on which position is the needed byte */
		uint8_t RAM_Memory_remainder = address - RAM_Memory_line;

		/* The ROM Memory file have the following fields: [address] [4*space] [16 * bytes] */
		/* Example:                                       0000FFF0            EA EA EA EA EA EA EA EA EA EA EA EA 00 80 EA EA */
		std::stringstream ss;
		ss << std::uppercase << std::hex << RAM_Memory_line; // Set the stream to output in hexadecimal format
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
				std::string ceva = valueString.substr(RAM_Memory_remainder * 3, 2);
				value = std::stoi(ceva, nullptr, 16);
				break;
			}
		}
		this->m_bus->setBusInfo(value);
	}
}

std::string RAM_Mem::hexAddressToString(uint16_t address) {
	std::ostringstream oss;
	oss << std::hex << std::uppercase << std::setfill('0') << std::setw(8) << address;
	return oss.str();
}