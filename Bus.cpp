#include "Bus.h"

// Initialize the static member variable
Bus* Bus::m_busInstance = nullptr;

Bus::Bus()
{
	this-> m_current_bus_info = 0;
}

Bus::~Bus()
{
	// Release any resources held by the Bus class, if necessary
	// For example, you can release the memory used by observers in m_vPeripheralObs vector.
	for (Observer* observer : m_vPeripheralObs)
	{
		delete observer;
	}
	m_vPeripheralObs.clear();
}

/* A new peripheral added in the vector = a new peripheral attached to the Bus */
void Bus::attachNewPeripheralToBus(Observer* new_observer_peripheral)
{
	this->m_vPeripheralObs.push_back(new_observer_peripheral);
}

/* Remove peripheral from the Peripherals Observers Vector */
void Bus::detachPeripheralFromBus(Observer* observer_peripheral)
{
	auto it = std::find(this->m_vPeripheralObs.begin(), this->m_vPeripheralObs.end(), observer_peripheral);
	if (it != this->m_vPeripheralObs.end())
	{
		this->m_vPeripheralObs.erase(it);
	}
}

void Bus::notifyPeripheralRead(uint16_t address) {
	for (Observer* observer : m_vPeripheralObs) {
		observer->onBusRead(address);
	}
}

void Bus::notifyPeripheralWrite(uint16_t address, uint8_t data) {
	for (Observer* observer : m_vPeripheralObs) {
		observer->onBusWrite(address, data);
	}
}

uint8_t Bus::readBus(uint16_t address)
{
	this->notifyPeripheralRead(address);
	std::cout << std::hex << " " << address << "     r   " << this->m_current_bus_info << std::endl;
	return this->m_current_bus_info;
}

void Bus::writeBus(uint16_t address, uint8_t data)
{
	this->notifyPeripheralWrite(address, data);
	this->m_current_bus_info = data;
	std::cout << std::hex << " " << address << "      w   " << this->m_current_bus_info << std::endl;
}


void Bus::setBusInfo(uint16_t tempInfo)
{
	Bus* Main_Bus = Bus::GetInstance();
	Main_Bus->m_current_bus_info = tempInfo;
}