#ifndef UTILITY_H_
#define UTILITY_H_

#include <string>

namespace Utility
{
	std::string searchDirectory(std::string directory, std::string prefix);
	template<typename T>
	inline T lexical_cast(std::string value)
	{
		T temp;
		std::stringstream sstream(value);
		sstream >> std::boolalpha >> temp;
		return temp;
	}
	template<typename T>
	inline std::string lexical_cast(T value)
	{
		return std::string(static_cast<std::ostringstream*>(&(std::ostringstream() << value))->str());
	}
}

#endif