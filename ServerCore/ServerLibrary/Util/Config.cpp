#include "stdafx.h"
#include "Config.h"


namespace leeder
{

bool loadConfig(XMLDocument* config, const char* path)
{
	if (!config->LoadFile(path)) {
		printf("config.xml cannot be found");
		return false;
	}
	return true;
}


}