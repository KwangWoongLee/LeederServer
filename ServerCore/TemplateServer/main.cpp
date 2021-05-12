#include "stdafx.h"

#include "ServerLibrary.h"

int _tmain(int argc, _TCHAR* argv[])
{
	using namespace leeder;

	std::unique_ptr<IOCPServer> loginServer = std::make_unique<IOCPServer>();

	loginServer->Run();

	//SessionManager::GetInstance();

	//printf("------------------------------------------------------------");
	//printf("\n");



	//std::shared_ptr<Session> session;
	//SessionManager::GetInstance().AllocSession(session);


	//printf("Session Pool count %d's  usecount : %d \n", session->GetID(), session.use_count());

	//printf("------------------------------------------------------------");

	//printf("\nSession Alloc\n");

	//printf("\nSession Pool\n");

	//SessionManager::GetInstance().printSessionPool();

	//printf("\nSession List\n");


	//SessionManager::GetInstance().printSessionList();


	//printf("------------------------------------------------------------");

	//printf("\nSession Return\n");

	//SessionManager::GetInstance().ReturnSession(session);

	//SessionManager::GetInstance().printSessionPool();

	//SessionManager::GetInstance().printSessionList();


	return 0;
}