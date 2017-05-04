#include "stdafx.h"

void CRPCPlayer::PlayerModel(RakNet::BitStream *bitStream, RakNet::Packet *packet)
{
	std::cout << "CRPCPlayer::PlayerModel" << std::endl;
	RakWString model;

	bitStream->Read(model);

	//GamePed::SetPedModel(CLocalPlayer::GetPed(), CString::utf16ToUtf8(model.C_String()));
}

void CRPCPlayer::SetControllable(RakNet::BitStream *bitStream, RakNet::Packet *packet)
{
	std::cout << "CRPCPlayer::SetControllable" << std::endl;
	bool disablecontrols, frozen;

	bitStream->Read(disablecontrols);
	bitStream->Read(frozen);

	CLocalPlayer::SetControllable(disablecontrols);
	CLocalPlayer::SetFrozen(frozen);

	if(frozen)
		ENTITY::FREEZE_ENTITY_POSITION(CLocalPlayer::GetPed(), frozen);
}

void CRPCPlayer::Kick(RakNet::BitStream *bitStream, RakNet::Packet *packet)
{
	std::cout << "CRPCPlayer::Kick" << std::endl;
	RakString reason;

	bitStream->Read(reason);

	DirectXRenderer::kickedMessage = reason.C_String();
	DirectXRenderer::showKicked = true;

	DirectXRenderer::showCursor = true;
	CLocalPlayer::SetControllable(false);

	CNetworkManager::Disconnect();
}
