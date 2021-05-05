#include "stdafx.h"
#include "Config.h"


namespace leeder
{

bool loadConfig(XMLDocument* config)
{
	if (!config->LoadFile("../TemplateServer/config.xml")) {
		printf("config.xml cannot be found");
		return false;
	}
	return true;
}


}