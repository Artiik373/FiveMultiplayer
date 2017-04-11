#include "stdafx.h"

namespace API
{
	const char *Vehicle::ThisNamespace = "API::Vehicle::";

	const int Vehicle::Create(const std::wstring model, const CVector3 position, const float heading)
	{
		CVehicleEntity newVehicle;
		newVehicle.Create(model, position, heading);
		g_Vehicles.push_back(newVehicle);

		return newVehicle.GetId();
	}
	const int Vehicle::Create(const std::wstring model, const CVector3 position, const CVector3 rotation)
	{
		CVehicleEntity newVehicle;
		newVehicle.Create(model, position, rotation);
		g_Vehicles.push_back(newVehicle);

		return newVehicle.GetId();
	}

	void Vehicle::SetColor(const int entity, const int layer, const int painttype, const int color)
	{
		if (layer == 1 || layer == 2)
		{
			const int index = ServerEntity::GetIndex(entity);
			if (index > -1) {
				switch (g_Entities[index].GetType())
				{
				case CServerEntity::Vehicle:
					for (int i = 0; i < g_Vehicles.size(); i++)
					{
						if (g_Vehicles[i].GetId() == entity)
						{
							g_Vehicles[i].SetColor(layer, painttype, color);

							RakNet::BitStream sData;
							sData.Write(entity);
							sData.Write(layer);
							sData.Write(painttype);
							sData.Write(color);

							g_Server->GetNetworkManager()->GetRPC().Signal("SetStandardColor", &sData, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true, false);
							return;
						}
					}
					break;
				default:
					std::wcout << ThisNamespace << L"SetColor Entity " << entity << L" is not of type Vehicle." << std::endl;
					break;
				}
			}
			else {
				std::wcout << ThisNamespace << L"SetColor Invalid Entity: " << entity << std::endl;
			}
		}
		else {
			std::wcout << ThisNamespace << L"SetColor Invalid Layer, Layers are 1 or 2." << std::endl;
		}
	}

	void Vehicle::SetColor(const int entity, const int layer, const Color color)
	{
		if (layer == 1 || layer == 2)
		{
			const int index = ServerEntity::GetIndex(entity);
			if (index > -1) {
				switch (g_Entities[index].GetType())
				{
				case CServerEntity::Vehicle:
					for (int i = 0; i < g_Vehicles.size(); i++)
					{
						if (g_Vehicles[i].GetId() == entity)
						{
							g_Vehicles[i].SetColor(layer, color);

							RakNet::BitStream sData;
							sData.Write(entity);
							sData.Write(layer);
							sData.Write(color.Red);
							sData.Write(color.Green);
							sData.Write(color.Blue);

							g_Server->GetNetworkManager()->GetRPC().Signal("SetCustomColor", &sData, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true, false);
							return;
						}
					}
					break;
				default:
					std::wcout << ThisNamespace << L"SetColor Entity " << entity << L" is not of type Vehicle." << std::endl;
					break;
				}
			}
			else {
				std::wcout << ThisNamespace << L"SetColor Invalid Entity: " << entity << std::endl;
			}
		}
		else {
			std::wcout << ThisNamespace << L"SetColor Invalid Layer, Layers are 1 or 2." << std::endl;
		}
	}

	const std::wstring Vehicle::GetNumberPlate(const int entity)
	{
		const int index = ServerEntity::GetIndex(entity);
		if (index > -1) {
			switch (g_Entities[index].GetType())
			{
			case CServerEntity::Vehicle:
				for (int i = 0; i < g_Vehicles.size(); i++)
				{
					if (g_Vehicles[i].GetId() == entity)
					{
						return g_Vehicles[i].GetNumberPlate();
					}
				}
				break;
			default:
				std::wcout << ThisNamespace << L"GetNumberPlate Entity " << entity << L" is not of type Vehicle." << std::endl;
				break;
			}
		}
		else {
			std::wcout << ThisNamespace << L"GetNumberPlate Invalid Entity: " << entity << std::endl;
		}
	}

	void Vehicle::SetNumberPlate(const int entity, std::wstring plate)
	{
		const int index = ServerEntity::GetIndex(entity);
		if (index > -1) {
			switch (g_Entities[index].GetType())
			{
			case CServerEntity::Vehicle:
				for (int i = 0; i < g_Vehicles.size(); i++)
				{
					if (g_Vehicles[i].GetId() == entity)
					{
						if (plate.size() > 8)
							plate = plate.substr(0, 8);

						g_Vehicles[i].SetNumberPlate(plate);

						RakNet::BitStream sData;
						sData.Write(entity);
						sData.Write(RakWString(plate.c_str()));

						g_Server->GetNetworkManager()->GetRPC().Signal("SetNumberPlate", &sData, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true, false);
						return;
					}
				}
				break;
			default:
				std::wcout << ThisNamespace << L"SetNumberPlate Entity " << entity << L" is not of type Vehicle." << std::endl;
				break;
			}
		}
		else {
			std::wcout << ThisNamespace << L"SetNumberPlate Invalid Entity: " << entity << std::endl;
		}
	}

	const int Vehicle::GetMod(const int entity, const int modType)
	{
		if (modType > -1 && modType < 49)
		{
			const int index = ServerEntity::GetIndex(entity);
			if (index > -1) 
			{
				switch (g_Entities[index].GetType())
				{
				case CServerEntity::Vehicle:
					for (int i = 0; i < g_Vehicles.size(); i++)
					{
						if (g_Vehicles[i].GetId() == entity)
						{
							return g_Vehicles[i].GetMod(modType);
						}
					}
					break;
				default:
					std::wcout << ThisNamespace << L"GetMod Entity " << entity << L" is not of type Vehicle." << std::endl;
					break;
				}
			}
			else 
			{
				std::wcout << ThisNamespace << L"GetMod Invalid Entity: " << entity << std::endl;
			}
		}
		else
		{
			std::wcout << ThisNamespace << L"GetMod Invalid modType " << modType << L", must be 0 to 48." << std::endl;
		}
	}

	void Vehicle::SetMod(const int entity, const int modType, const int modIndex)
	{
		if (modType > -1 && modType < 49)
		{
			const int index = ServerEntity::GetIndex(entity);
			if (index > -1)
			{
				switch (g_Entities[index].GetType())
				{
				case CServerEntity::Vehicle:
					for (int i = 0; i < g_Vehicles.size(); i++)
					{
						if (g_Vehicles[i].GetId() == entity)
						{
							g_Vehicles[i].SetMod(modType, modIndex);

							RakNet::BitStream sData;
							sData.Write(entity);
							sData.Write(modType);
							sData.Write(modIndex);

							g_Server->GetNetworkManager()->GetRPC().Signal("SetMod", &sData, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true, false);
							return;
						}
					}
					break;
				default:
					std::wcout << ThisNamespace << L"SetMod Entity " << entity << L" is not of type Vehicle." << std::endl;
					break;
				}
			}
			else
			{
				std::wcout << ThisNamespace << L"SetMod Invalid Entity: " << entity << std::endl;
			}
		}
		else
		{
			std::wcout << ThisNamespace << L"SetMod Invalid modType " << modType << L", must be 0 to 48." << std::endl;
		}
	}
}
