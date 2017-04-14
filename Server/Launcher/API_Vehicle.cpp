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
		if (modType >= 0 && modType <= 49)
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
			std::wcout << ThisNamespace << L"GetMod Invalid modType " << modType << L", must be 0 to 49." << std::endl;
		}
	}

	void Vehicle::SetMod(const int entity, const int modType, const int modIndex)
	{
		if (modType >= 0 && modType <= 49)
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
			std::wcout << ThisNamespace << L"SetMod Invalid modType " << modType << L", must be 0 to 49." << std::endl;
		}
	}

	const bool Vehicle::GetEngineState(const int entity)
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
						return g_Vehicles[i].GetEngineState();
					}
				}
				break;
			default:
				std::wcout << ThisNamespace << L"GetEngineState Entity " << entity << L" is not of type Vehicle." << std::endl;
				break;
			}
		}
		else
		{
			std::wcout << ThisNamespace << L"GetEngineState Invalid Entity: " << entity << std::endl;
		}
	}

	void Vehicle::SetEngineState(const int entity, const bool state)
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
						g_Vehicles[i].SetEngineState(state);

						RakNet::BitStream sData;
						sData.Write(entity);
						sData.Write(state);

						g_Server->GetNetworkManager()->GetRPC().Signal("SetEngineState", &sData, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true, false);
						return;
					}
				}
				break;
			default:
				std::wcout << ThisNamespace << L"SetEngineState Entity " << entity << L" is not of type Vehicle." << std::endl;
				break;
			}
		}
		else
		{
			std::wcout << ThisNamespace << L"SetEngineState Invalid Entity: " << entity << std::endl;
		}
	}

	const int Vehicle::GetDoorsLockState(const int entity)
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
						return g_Vehicles[i].GetDoorsLockState();
					}
				}
				break;
			default:
				std::wcout << ThisNamespace << L"GetDoorsLockState Entity " << entity << L" is not of type Vehicle." << std::endl;
				break;
			}
		}
		else
		{
			std::wcout << ThisNamespace << L"GetDoorsLockState Invalid Entity: " << entity << std::endl;
		}
	}

	void Vehicle::SetDoorsLockState(const int entity, const int state)
	{
		if (state <= 4 && state >= 0)
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
							g_Vehicles[i].SetDoorsLockState(state);

							RakNet::BitStream sData;
							sData.Write(entity);
							sData.Write(state);

							g_Server->GetNetworkManager()->GetRPC().Signal("SetDoorsLockState", &sData, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true, false);
							return;
						}
					}
					break;
				default:
					std::wcout << ThisNamespace << L"SetDoorsLockState Entity " << entity << L" is not of type Vehicle." << std::endl;
					break;
				}
			}
			else
			{
				std::wcout << ThisNamespace << L"SetDoorsLockState Invalid Entity: " << entity << std::endl;
			}
		}
		else
		{
			std::wcout << ThisNamespace << L"SetDoorsLockState Invalid Lock State, Valid states are 0 to 4." << entity << std::endl;
		}
	}

	void Vehicle::SetDoorsLockState(const int entity, const int state, const int player)
	{
		if (state <= 4 && state >= 0)
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
							const int pIndex = ServerEntity::GetIndex(player);

							if (pIndex > -1)
							{
								switch (g_Entities[pIndex].GetType())
								{
								case CServerEntity::Player:
									for (int p = 0; p < g_Players.size(); p++)
									{
										if (player == g_Players[p].GetId())
										{
											RakNet::BitStream sData;
											sData.Write(entity);
											sData.Write(state);

											g_Server->GetNetworkManager()->GetRPC().Signal("SetDoorsLockState", &sData, HIGH_PRIORITY, RELIABLE_ORDERED, 0, g_Players[p].GetGUID(), false, false);
											return;
										}
									}
									break;
								default:
									std::wcout << ThisNamespace << L"SetDoorsLockState Entity " << player << L" is not of type Player." << std::endl;
									break;
								}
							}
							else
							{
								std::wcout << ThisNamespace << L"SetDoorsLockState Invalid Entity(player): " << player << std::endl;
							}
						}
					}
					break;
				default:
					std::wcout << ThisNamespace << L"SetDoorsLockState Entity(entity) " << entity << L" is not of type Vehicle." << std::endl;
					break;
				}
			}
			else
			{
				std::wcout << ThisNamespace << L"SetDoorsLockState Invalid Entity(entity): " << entity << std::endl;
			}
		}
		else
		{
			std::wcout << ThisNamespace << L"SetDoorsLockState Invalid Lock State, Valid states are 0 to 4." << entity << std::endl;
		}
	}

	const std::wstring Vehicle::GetModel(const int entity)
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
						return g_Vehicles[i].GetModel();
					}
				}
				break;
			default:
				std::wcout << ThisNamespace << L"GetModel Entity " << entity << L" is not of type Vehicle." << std::endl;
				break;
			}
		}
		else
		{
			std::wcout << ThisNamespace << L"GetModel Invalid Entity: " << entity << std::endl;
		}
	}

	const int Vehicle::GetNumberPlateStyle(const int entity)
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
						return g_Vehicles[i].GetNumberPlateStyle();
					}
				}
				break;
			default:
				std::wcout << ThisNamespace << L"SetNumberPlateStyle Entity " << entity << L" is not of type Vehicle." << std::endl;
				break;
			}
		}
		else
		{
			std::wcout << ThisNamespace << L"SetNumberPlateStyle Invalid Entity: " << entity << std::endl;
		}
	}

	void Vehicle::SetNumberPlateStyle(const int entity, const int style)
	{
		if (style >= 0 && style <= 5)
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
							g_Vehicles[i].SetNumberPlateStyle(style);

							RakNet::BitStream sData;
							sData.Write(entity);
							sData.Write(style);

							g_Server->GetNetworkManager()->GetRPC().Signal("SetNumberPlateStyle", &sData, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true, false);
							return;
						}
					}
					break;
				default:
					std::wcout << ThisNamespace << L"SetNumberPlateStyle Entity " << entity << L" is not of type Vehicle." << std::endl;
					break;
				}
			}
			else
			{
				std::wcout << ThisNamespace << L"SetNumberPlateStyle Invalid Entity: " << entity << std::endl;
			}
		}
		else
		{
			std::wcout << ThisNamespace << L"SetNumberPlateStyle Invalid Style " << style << L", Valid styles are 0-5." << std::endl;
		}
	}

	const bool Vehicle::GetExtra(const int entity, const int extra)
	{
		if (extra >= 1 && extra <= 14)
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
							return g_Vehicles[i].GetExtra(extra);
						}
					}
					break;
				default:
					std::wcout << ThisNamespace << L"GetVehicleExtra Entity " << entity << L" is not of type Vehicle." << std::endl;
					break;
				}
			}
			else
			{
				std::wcout << ThisNamespace << L"GetVehicleExtra Invalid Entity: " << entity << std::endl;
			}
		}
		else
		{
			std::wcout << ThisNamespace << L"GetVehicleExtra Invalid Extra: " << extra << L", Must be 1-14." << std::endl;
		}
	}

	void Vehicle::SetExtra(const int entity, const int extra, const bool toggle)
	{
		if (extra >= 1 && extra <= 14)
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
							g_Vehicles[i].SetExtra(extra, toggle);

							RakNet::BitStream sData;
							sData.Write(entity);
							sData.Write(extra);
							sData.Write(toggle);

							g_Server->GetNetworkManager()->GetRPC().Signal("SetExtra", &sData, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true, false);
							return;
						}
					}
					break;
				default:
					std::wcout << ThisNamespace << L"GetVehicleExtra Entity " << entity << L" is not of type Vehicle." << std::endl;
					break;
				}
			}
			else
			{
				std::wcout << ThisNamespace << L"GetVehicleExtra Invalid Entity: " << entity << std::endl;
			}
		}
		else
		{
			std::wcout << ThisNamespace << L"GetVehicleExtra Invalid Extra: " << extra << L", Must be 1-14." << std::endl;
		}
	}
}
