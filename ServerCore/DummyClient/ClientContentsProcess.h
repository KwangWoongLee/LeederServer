#pragma once
#include "stdafx.h"

namespace leeder
{

	using clientPacketfunc = std::function<void(std::shared_ptr<Client>&, std::shared_ptr<Packet>&)>;


	class ClientContentsProcess
	{
	public:
		ClientContentsProcess();
		~ClientContentsProcess();

		void Init(XMLDocument* config);

		void	PutPackage(std::shared_ptr<ClientPackage>&& package);
		std::shared_ptr<ClientPackage> GetPackage();

		void	ProcessPackage(std::shared_ptr<ClientPackage>& package);

		size_t	GetThreadCount() { return mThreadCount; }

	protected:
		void RegistFunction(ePacketType type, clientPacketfunc func);

	private:
		void	RegistDefaultFunction();


		ThreadSafeQueue<std::shared_ptr<ClientPackage>>	mPackageQueue;
		std::vector<std::unique_ptr<Thread>>		mProcessThreadPool;
		size_t										mThreadCount;
		std::unordered_map<ePacketType, clientPacketfunc>		mProcessFunctionMap;

	};


}
