#ifndef PIN_CONFIG_H_
#define PIN_CONFIG_H_

#include <string>
#include <map>
#include <set>

class PinConfig
{
public:
	static void SetupPinConfig();
	static int ConfigurePin(unsigned int block, unsigned int pin, std::string mode);
	static bool AllocatePin(unsigned int block, unsigned int pin);
	
private:
	static bool validPin(unsigned int block, unsigned int pin);
	static std::string ocpDir;
	static std::map<std::pair<unsigned int, unsigned int>, unsigned int> gpioMap;
	static std::map<std::pair<unsigned int, unsigned int>, std::pair<unsigned int, unsigned int> > pruMap;
	static std::map<std::pair<unsigned int, unsigned int>, unsigned int> pwmMap;
	static std::map<std::pair<unsigned int, unsigned int>, unsigned int> uartMap;
	static std::map<std::pair<unsigned int, unsigned int>, unsigned int> i2cMap;
	static std::map<std::pair<unsigned int, unsigned int>, unsigned int> spiMap;
	static std::set<std::pair<unsigned int, unsigned int> > allocatedPins;
};

#endif