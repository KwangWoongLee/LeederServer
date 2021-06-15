#pragma once
#include "stdafx.h"

//미리 컴파일된 헤더

#include "ServerLibrary.h"

using leeder::Singleton;
using leeder::Thread;
using leeder::ThreadSafeQueue;
using leeder::Clock;

using leeder::Session;
using leeder::IOCPSession;

using leeder::eInputType;
using leeder::Input;


using leeder::eObjectState;
using leeder::eObjectType;
using leeder::ObjectInfo;

using leeder::GameObject;
using leeder::Packet;
using leeder::ePacketType;


using leeder::Package;

using leeder::ContentsProcess;



using leeder::XMLDocument;
using leeder::XMLElement;
using leeder::BUF_SIZE;

using leeder::loadConfig;

#include "User.h"

#include "World.h"

#include "NetworkManager.h"

#include "DummyTestProcess.h"

