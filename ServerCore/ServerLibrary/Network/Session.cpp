#include "stdafx.h"
#include "Session.h"

namespace leeder
{
Session::Session()
	:mType(eSessionType::CLIENT)
{
}
Session::~Session()
{
}
void Session::Reset()
{
}
void Session::OnAccept()
{
}
void Session::OnClose()
{

}
}