#include "Utility.h"
#include <dirent.h>

using namespace Utility;

std::string searchDirectory(std::string directory, std::string prefix)
{
	std::string str;
	DIR *path;
	dirent *entry;

	path = opendir(directory.c_str());
	if( path != NULL  )
	{
		while( (entry = readdir(path)) != NULL)
		{
			if( entry->d_name[0] == '.')
			{
				continue;
			}

			str = entry->d_name;
			if(strstr(entry->d_name, prefix.c_str()) != NULL )
			{
				closedir(path);
				return str;
			}
		}
	}
	closedir(path);

	return "";
}
