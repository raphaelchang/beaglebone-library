#include "PinConfig.h"

std::map<std::pair<unsigned int, unsigned int>, unsigned int> PinConfig::gpioMap;
std::map<std::pair<unsigned int, unsigned int>, std::pair<unsigned int, unsigned int> > PinConfig::pruMap;
std::map<std::pair<unsigned int, unsigned int>, unsigned int> PinConfig::pwmMap;
std::map<std::pair<unsigned int, unsigned int>, unsigned int> PinConfig::uartMap;
std::map<std::pair<unsigned int, unsigned int>, unsigned int> PinConfig::i2cMap;
std::map<std::pair<unsigned int, unsigned int>, unsigned int> PinConfig::spiMap;
std::set<std::pair<unsigned int, unsigned int> > PinConfig::allocatedPins;

void PinConfig::SetupPinConfig()
{
	ocpDir = searchDirectory("/sys/devices/", "ocp.") + "/";
	
	gpioMap[make_pair<8, 3>] = 38;
	gpioMap[make_pair<8, 4>] = 39;
	gpioMap[make_pair<8, 5>] = 34;
	gpioMap[make_pair<8, 6>] = 35;
	gpioMap[make_pair<8, 7>] = 66;
	gpioMap[make_pair<8, 8>] = 67;
	gpioMap[make_pair<8, 9>] = 69;
	gpioMap[make_pair<8, 10>] = 68;
	gpioMap[make_pair<8, 11>] = 45;
	gpioMap[make_pair<8, 12>] = 44;
	gpioMap[make_pair<8, 13>] = 23;
	gpioMap[make_pair<8, 14>] = 26;
	gpioMap[make_pair<8, 15>] = 47;
	gpioMap[make_pair<8, 16>] = 46;
	gpioMap[make_pair<8, 17>] = 27;
	gpioMap[make_pair<8, 18>] = 65;
	gpioMap[make_pair<8, 19>] = 22;
	gpioMap[make_pair<8, 20>] = 63;
	gpioMap[make_pair<8, 21>] = 62;
	gpioMap[make_pair<8, 22>] = 37;
	gpioMap[make_pair<8, 23>] = 36;
	gpioMap[make_pair<8, 24>] = 33;
	gpioMap[make_pair<8, 25>] = 32;
	gpioMap[make_pair<8, 26>] = 61;
	gpioMap[make_pair<8, 27>] = 86;
	gpioMap[make_pair<8, 28>] = 88;
	gpioMap[make_pair<8, 29>] = 87;
	gpioMap[make_pair<8, 30>] = 89;
	gpioMap[make_pair<8, 31>] = 10;
	gpioMap[make_pair<8, 32>] = 11;
	gpioMap[make_pair<8, 33>] = 9;
	gpioMap[make_pair<8, 34>] = 81;
	gpioMap[make_pair<8, 35>] = 8;
	gpioMap[make_pair<8, 36>] = 80;
	gpioMap[make_pair<8, 37>] = 78;
	gpioMap[make_pair<8, 38>] = 79;
	gpioMap[make_pair<8, 39>] = 76;
	gpioMap[make_pair<8, 40>] = 77;
	gpioMap[make_pair<8, 41>] = 74;
	gpioMap[make_pair<8, 42>] = 75;
	gpioMap[make_pair<8, 43>] = 72;
	gpioMap[make_pair<8, 44>] = 73;
	gpioMap[make_pair<8, 45>] = 70;
	gpioMap[make_pair<8, 46>] = 71;
	
	gpioMap[make_pair<9, 11>] = 30;
	gpioMap[make_pair<9, 12>] = 60;
	gpioMap[make_pair<9, 13>] = 31;
	gpioMap[make_pair<9, 14>] = 50;
	gpioMap[make_pair<9, 15>] = 48;
	gpioMap[make_pair<9, 16>] = 51;
	gpioMap[make_pair<9, 17>] = 5;
	gpioMap[make_pair<9, 18>] = 4;
	gpioMap[make_pair<9, 21>] = 3;
	gpioMap[make_pair<9, 22>] = 2;
	gpioMap[make_pair<9, 23>] = 49;
	gpioMap[make_pair<9, 24>] = 15;
	gpioMap[make_pair<9, 25>] = 117;
	gpioMap[make_pair<9, 26>] = 14;
	gpioMap[make_pair<9, 27>] = 115;
	gpioMap[make_pair<9, 28>] = 123;
	gpioMap[make_pair<9, 29>] = 121;
	gpioMap[make_pair<9, 30>] = 122;
	gpioMap[make_pair<9, 31>] = 120;
	gpioMap[make_pair<9, 41>] = 20;
	gpioMap[make_pair<9, 42>] = 7;
	gpioMap[make_pair<9, 91>] = 116;
	gpioMap[make_pair<9, 92>] = 114;
	
	pruMap[make_pair<8, 15>] = make_pair<0, 15>;
	pruMap[make_pair<8, 16>] = make_pair<0, 14>;
	pruMap[make_pair<8, 20>] = make_pair<1, 13>;
	pruMap[make_pair<8, 21>] = make_pair<1, 12>;
	pruMap[make_pair<8, 27>] = make_pair<1, 8>;
	pruMap[make_pair<8, 28>] = make_pair<1, 10>;
	pruMap[make_pair<8, 29>] = make_pair<1, 9>;
	pruMap[make_pair<8, 39>] = make_pair<1, 6>;
	pruMap[make_pair<8, 40>] = make_pair<1, 7>;
	pruMap[make_pair<8, 41>] = make_pair<1, 4>;
	pruMap[make_pair<8, 42>] = make_pair<1, 5>;
	pruMap[make_pair<8, 43>] = make_pair<1, 2>;
	pruMap[make_pair<8, 44>] = make_pair<1, 3>;
	pruMap[make_pair<8, 45>] = make_pair<1, 0>;
	pruMap[make_pair<8, 46>] = make_pair<1, 1>;
	
	pruMap[make_pair<9, 24>] = make_pair<0, 16>;
	pruMap[make_pair<9, 25>] = make_pair<0, 7>;
	pruMap[make_pair<9, 26>] = make_pair<1, 16>;
	pruMap[make_pair<9, 27>] = make_pair<0, 5>;
	pruMap[make_pair<9, 28>] = make_pair<0, 3>;
	pruMap[make_pair<9, 29>] = make_pair<0, 1>;
	pruMap[make_pair<9, 30>] = make_pair<0, 2>;
	pruMap[make_pair<9, 31>] = make_pair<0, 0>;
	pruMap[make_pair<9, 41>] = make_pair<0, 16>;
	pruMap[make_pair<9, 91>] = make_pair<0, 6>;
	pruMap[make_pair<9, 92>] = make_pair<0, 4>;
	
	pwmMap[make_pair<8, 13>] = 6;
	pwmMap[make_pair<8, 19>] = 5;
	
	pwmMap[make_pair<9, 14>] = 3;
	pwmMap[make_pair<9, 16>] = 4;
	pwmMap[make_pair<9, 21>] = 1;
	pwmMap[make_pair<9, 22>] = 0;
	pwmMap[make_pair<9, 28>] = 7;
	pwmMap[make_pair<9, 42>] = 2;
}

bool PinConfig::validPin(unsigned int block, unsigned int pin)
{
	if (block != 8 && block != 9)
		return false;
	if (pin < 1 || (pin > 46 && pin != 91 && pin != 92))
		return false;
}