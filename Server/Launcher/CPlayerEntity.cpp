#include "stdafx.h"

int CPlayerEntity::Amount = 0;

void CPlayerEntity::Create(std::string Name, RakNetGUID GUID, SystemAddress Ip)
{
	CServerEntity newServerEntity;
	newServerEntity.SetType(newServerEntity.Player);

	Information.Name = Name;
	Information.Entity = newServerEntity.Create();
	Network.GUID = GUID;
	Network.Ip = Ip;

	Data.Vehicle.VehicleID = -1;
	Data.Vehicle.Seat = -1;
	
	g_Entities.push_back(newServerEntity);

	Amount++;

	std::cout << "[CPlayerEntity] Added Player: " << Information.Name << " [" << Network.Ip.ToString(false) << "]" << std::endl;
	std::cout << "[CPlayerEntity] Players Online: " << Amount << std::endl;

	Network.LastSyncSent = timeGetTime();
	Network.Synchronized = true;
}

void CPlayerEntity::Destroy()
{
	std::cout << "[CPlayerEntity] Removing Player: " << Information.Name << " [" << Network.Ip.ToString(false) << "]" << std::endl;

	Information = {};
	Statistics = {};
	Data = {};
	Network = {};

	Information.Entity = -1;
	Information.PlayerID = -1;

	Data.Vehicle.VehicleID = -1;
	Data.Vehicle.Seat = -1;

	Amount--;

	std::cout << "[CPlayerEntity] Players Online: " << Amount << std::endl;
}

void CPlayerEntity::Pulse()
{
	if (Network.LastSyncSent + (1000 / CServer::GetInstance()->GetSyncRate()) <= timeGetTime() && Information.Entity != -1 && Information.PlayerID != -1)
	{
		BitStream bitstream;
		bitstream.Write((unsigned char)ID_PACKET_PLAYER);
		bitstream.Write(Network.GUID);

		bitstream.Write(Information.Entity);
		bitstream.Write(Information.Name);

		bitstream.Write(Statistics.Score);

		bitstream.Write(Data.Model.Model);
		bitstream.Write(Data.Model.Type);

		bitstream.Write(Data.Weapon.Weapon);
		bitstream.Write(Data.Weapon.Reload);

		bitstream.Write(Data.ForwardSpeed);

		bitstream.Write(Data.Position.fX);
		bitstream.Write(Data.Position.fY);
		bitstream.Write(Data.Position.fZ);

		bitstream.Write(Data.Velocity.fX);
		bitstream.Write(Data.Velocity.fY);
		bitstream.Write(Data.Velocity.fZ);

		bitstream.Write(Data.Quaternion.fX);
		bitstream.Write(Data.Quaternion.fY);
		bitstream.Write(Data.Quaternion.fZ);
		bitstream.Write(Data.Quaternion.fW);

		bitstream.Write(Data.Vehicle.VehicleID);
		bitstream.Write(Data.Vehicle.Seat);

		CNetworkManager::instance()->GetInterface()->Send(&bitstream, MEDIUM_PRIORITY, UNRELIABLE_SEQUENCED, 0, UNASSIGNED_RAKNET_GUID, true);

		Network.LastSyncSent = timeGetTime();
	}
}

void CPlayerEntity::Update(Packet *packet)
{
	
	int lastVehicle = Data.Vehicle.VehicleID;
	int lastSeat = Data.Vehicle.Seat;

	BitStream bitstream(packet->data + 1, packet->length + 1, false);

	/*bitstream.Read(Information.Entity);
	bitstream.Read(Information.Name);*/
	
	bitstream.Read(Statistics.Score);

	bitstream.Read(Data.Model.Model);
	bitstream.Read(Data.Model.Type);

	bitstream.Read(Data.Weapon.Weapon);
	bitstream.Read(Data.Weapon.Reload);

	bitstream.Read(Data.ForwardSpeed);

	bitstream.Read(Data.Position.fX);
	bitstream.Read(Data.Position.fY);
	bitstream.Read(Data.Position.fZ);

	bitstream.Read(Data.Velocity.fX);
	bitstream.Read(Data.Velocity.fY);
	bitstream.Read(Data.Velocity.fZ);

	bitstream.Read(Data.Quaternion.fX);
	bitstream.Read(Data.Quaternion.fY);
	bitstream.Read(Data.Quaternion.fZ);
	bitstream.Read(Data.Quaternion.fW);

	bitstream.Read(Data.Vehicle.VehicleID);
	bitstream.Read(Data.Vehicle.Seat);

	Network.GUID = packet->guid;
	Network.Ip = packet->systemAddress;
	packet->deleteData;
	
	/*
	if (lastVehicle != Data.Vehicle.VehicleID)
	{
		for (int i = 0; i < g_Vehicles.size(); i++)
		{
			if (g_Vehicles[i].GetId() == Data.Vehicle.VehicleID && g_Vehicles[i].GetOccupant(Data.Vehicle.Seat) != Information.PlayerID)
			{
				g_Vehicles[i].SetOccupant(Data.Vehicle.Seat, Information.PlayerID);

				//OnPlayerEnterVehicle(player,vehicle,seat);
			}

			if (g_Vehicles[i].GetId() == lastVehicle && lastSeat != -1 && lastVehicle != -1)
			{
				g_Vehicles[i].SetOccupant(lastSeat, -1);
			}
		}
	}
	*/
}