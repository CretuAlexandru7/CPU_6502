#pragma once
#include <array>
#include <vector>
#include <cstdint>
#include "CPU_6502.h"
#include "Observer.h"


class Bus
{
public:
	
	/* Unable to make a clone - Singleton */
	Bus(Bus& other) = delete;
	/* Unable to assign the isntance - Singleton */
	void operator=(const Bus&) = delete;

	/* Single Point Access to the Singleton Class */
	static Bus* GetInstance()
	{
		if (!m_busInstance)
		{
			m_busInstance = new Bus();
		}
		return m_busInstance;
	}

	~Bus();
	/* Attach / Detach peripheral from the bus (vector of observers) */
	void attachNewPeripheralToBus(Observer* observer);
	void detachPeripheralFromBus(Observer* observer);
	/* Read / Write data from / to the Bus */
	uint8_t readBus(uint16_t address);
	void writeBus(uint16_t address, uint8_t data);

	/* Notify Peripherals that a Read or a Write operation will take place */
	void notifyPeripheralRead(uint16_t address);
	void notifyPeripheralWrite(uint16_t address, uint8_t data);

	void setBusInfo(uint16_t tempInfo);

protected:
	Bus();
private:
	static Bus* m_busInstance;
	/* RAM, ROM and other chips attached to the Bus */
	std::vector<Observer*> m_vPeripheralObs;
	/* Current / Actual state of the bus (what is written at this moment in time) */
	uint16_t m_current_bus_info;
};