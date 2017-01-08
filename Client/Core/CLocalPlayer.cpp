#include "stdafx.h"

CLocalPlayer::CLocalPlayer()
{
	Game.Player		= PLAYER::PLAYER_ID();
	Game.Ped		= PLAYER::GET_PLAYER_PED(Game.Player);
}

CLocalPlayer::~CLocalPlayer()
{
	Game.Player		= NULL;
	Game.Ped		= NULL;
}

void CLocalPlayer::Pulse()
{
	unsigned long ulCurrentTime = timeGetTime();
	if (ulCurrentTime >= Network.LastSyncSent + (1000.0f / 50))
	{
		if (g_Core->GetNetworkManager()->g_ConnectionState != CONSTATE_COND)
			return;

		Vector3 Coordinates = ENTITY::GET_ENTITY_COORDS(Game.Ped, ENTITY::IS_ENTITY_DEAD(Game.Ped));
		ENTITY::GET_ENTITY_QUATERNION(Game.Ped, &Data.Quaternion.fX, &Data.Quaternion.fY, &Data.Quaternion.fZ, &Data.Quaternion.fW);
		Vector3 Velocity = ENTITY::GET_ENTITY_VELOCITY(Game.Ped);

		Data.Model.Model = ENTITY::GET_ENTITY_MODEL(Game.Ped);
		Data.Model.Type = PED::GET_PED_TYPE(Game.Ped);

		WEAPON::GET_CURRENT_PED_WEAPON(Game.Ped, &Data.Weapon.Weapon, true);
		Data.Weapon.Reload = (bool)WEAPON::_IS_PED_RELOADING(Game.Ped);

		Data.ForwardSpeed = ENTITY::GET_ENTITY_SPEED(Game.Ped);
		Data.Position = { Coordinates.x, Coordinates.y, Coordinates.z };
		Data.Velocity = { Velocity.x, Velocity.y, Velocity.z };

		BitStream bitstream;
		bitstream.Write((unsigned char)ID_PACKET_PLAYER);

		bitstream.Write(Information.Id);
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

		g_Core->GetNetworkManager()->GetInterface()->Send(&bitstream, MEDIUM_PRIORITY, UNRELIABLE_SEQUENCED, 0, g_Core->GetNetworkManager()->GetSystemAddress(), false);

		Network.LastSyncSent = timeGetTime();
	}
}

int CLocalPlayer::GetVehicleID()
{
	Vehicle t_CurrentVehicle = PED::GET_VEHICLE_PED_IS_IN(Game.Ped, false);

	if (Game.LastVehicle == t_CurrentVehicle)
		return Game.LastVehicle;

	for (int i = 0; i < g_Vehicles.size(); i++)
	{
		if (g_Vehicles[i].GetEntity() == t_CurrentVehicle)
			return g_Vehicles[i].GetId();
	}

	return -1;
}
