#include "stdafx.h"

namespace API
{
	const char *Object::ThisNamespace = "API::Object";

	const int Object::Create(std::string model, CVector3 position, CVector3 rotation, bool dynamic)
	{
		CObjectEntity newObject;
		newObject.Create(model, position, rotation, dynamic);
		g_Objects.push_back(newObject);

		RakNet::BitStream sData;
		sData.Write(newObject.GetId());
		sData.Write(false);
		sData.Write(RakString(model.c_str()));
		sData.Write(position.fX);
		sData.Write(position.fY);
		sData.Write(position.fZ);
		sData.Write(rotation.fX);
		sData.Write(rotation.fY);
		sData.Write(rotation.fZ);
		sData.Write(dynamic);

		g_Server->GetNetworkManager()->GetRPC().Signal("CreateObject", &sData, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true, false);

		return newObject.GetId();
	}

	const int Object::Create(int hash, CVector3 position, CVector3 rotation, bool dynamic)
	{
		CObjectEntity newObject;
		newObject.Create(hash, position, rotation, dynamic);
		g_Objects.push_back(newObject);

		RakNet::BitStream sData;
		sData.Write(newObject.GetId());
		sData.Write(true);
		sData.Write(hash);
		sData.Write(position.fX);
		sData.Write(position.fY);
		sData.Write(position.fZ);
		sData.Write(rotation.fX);
		sData.Write(rotation.fY);
		sData.Write(rotation.fZ);
		sData.Write(dynamic);

		g_Server->GetNetworkManager()->GetRPC().Signal("CreateObject", &sData, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true, false);

		return newObject.GetId();
	}

	const void Object::SetTextureVariation(const int entity, const int textureindex)
	{
		for (int i = 1; i < g_Entities.size(); i++)
		{
			if (g_Entities[i].GetId() == entity)
			{
				switch (g_Entities[i].GetType())
				{
				case CServerEntity::Object:
					for (int i = 0; i < g_Objects.size(); i++)
					{
						if (g_Objects[i].GetId() == entity)
						{
							g_Objects[i].SetTextureVariation(textureindex);

							RakNet::BitStream sData;
							sData.Write(entity);
							sData.Write(textureindex);

							g_Server->GetNetworkManager()->GetRPC().Signal("SetTextureVariation", &sData, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true, false);
							return;
						}
					}
					break;
				default:
					std::cout << "[" << ThisNamespace << "::SetTextureVariation] Entity " << entity << " is not a Object." << std::endl;
					break;
				}
			}
		}
	}
}
