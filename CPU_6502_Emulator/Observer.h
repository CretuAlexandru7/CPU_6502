#pragma once
#include <cstdint>

class Observer
{
public:
	/* Pure virtual functions */
	virtual void onBusRead(uint16_t address) = 0;
	virtual void onBusWrite(uint16_t address, uint8_t data) = 0;
};
