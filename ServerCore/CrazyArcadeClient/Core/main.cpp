#include "stdafx.h"

int _tmain(int argc, _TCHAR* argv[])
{
    std::unique_ptr<Client> client = std::make_unique<Client>();

    if (client->Init())
        client->Run();


    return 0;


};