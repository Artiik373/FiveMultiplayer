#include "stdafx.h"

void CRPCEntity::Destroy(RakNet::BitStream *bitStream, RakNet::Packet *packet)
{
	int entity, type;

	bitStream->Read(entity);
	bitStream->Read(type);

	if (entity != -1)
	{
		for (int i = 0; i < g_Entities.size(); i++)
		{
			if (g_Entities[i].GetId() == entity)
			{
				g_Entities[i].Destroy();
			}
		}
	}
}

void CRPCEntity::SetPosition(RakNet::BitStream *bitStream, RakNet::Packet *packet)
{
	int entity;
	CVector3 position;

	bitStream->Read(entity);
	bitStream->Read(position.fX);
	bitStream->Read(position.fY);
	bitStream->Read(position.fZ);

	if (entity != -1)
	{
		for (int i = 0; i < g_Entities.size(); i++)
		{
			if(entity == g_Entities[i].GetId())
				return g_Entities[i].SetPosition(position);
		}
	}
	else
	{
		STREAMING::LOAD_SCENE(position.fX, position.fY, position.fZ);
		ENTITY::SET_ENTITY_COORDS(g_Core->GetLocalPlayer()->GetPed(), position.fX, position.fY, position.fZ, false, false, false, false);
	}
}

void CRPCEntity::TakeEntityAssignment(RakNet::BitStream *bitStream, RakNet::Packet *packet)
{
	int entity;
	RakNetGUID guid;

	bitStream->Read(entity);
	bitStream->Read(guid);

	if (ServerEntity::IsValid(entity))
	{
		ServerEntity::SetAssignee(entity, guid);
	}
}

void CRPCEntity::DropEntityAssignment(RakNet::BitStream *bitStream, RakNet::Packet *packet)
{
	int entity;

	bitStream->Read(entity);

	if (ServerEntity::IsValid(entity))
	{
		ServerEntity::SetAssignee(entity, UNASSIGNED_RAKNET_GUID);
	}
	
}