#include "stdafx.h"

CVehicleEntity::CVehicleEntity()
{
	Game.Created = false; 
	Game.Vehicle = NULL; 
	Network.Assigned = RakNetGUID(12345);
	
	for (int i = 0; i < SizeOfArray(Occupants); i++) 
	{ 
		Occupants[i] = -1; 
	}
}

void CVehicleEntity::Create(int entity)
{
	Information.Id	= entity;

	CServerEntity newServerEntity;
	newServerEntity.SetId(entity);
	newServerEntity.SetType(newServerEntity.Vehicle);
	g_Entities.push_back(newServerEntity);

	std::cout << "[CVehicleEntity] Added Vehicle: " << Information.Id << std::endl;
}

void CVehicleEntity::CreateVehicle()
{
	Hash model = GAMEPLAY::GET_HASH_KEY((char*)Data.Model.c_str());
	if (!STREAMING::IS_MODEL_IN_CDIMAGE(model) || !STREAMING::IS_MODEL_VALID(model))
		model = GAMEPLAY::GET_HASH_KEY("dilettante");

	STREAMING::REQUEST_COLLISION_AT_COORD(Data.Position.fX, Data.Position.fY, Data.Position.fZ);
	STREAMING::REQUEST_MODEL(model);
	while (!STREAMING::HAS_MODEL_LOADED(model))
		WAIT(0);

	Game.Vehicle = VEHICLE::CREATE_VEHICLE(model, Data.Position.fX, Data.Position.fY, Data.Position.fZ, Data.Heading, FALSE, TRUE);

	ENTITY::FREEZE_ENTITY_POSITION(Game.Vehicle, TRUE);
	ENTITY::SET_ENTITY_COORDS_NO_OFFSET(Game.Vehicle, Data.Position.fX, Data.Position.fY, Data.Position.fZ, FALSE, FALSE, FALSE);
	
	ENTITY::SET_ENTITY_LOAD_COLLISION_FLAG(Game.Vehicle, TRUE);
	ENTITY::SET_ENTITY_COLLISION(Game.Vehicle, TRUE, FALSE);
	ENTITY::SET_ENTITY_QUATERNION(Game.Vehicle, Data.Quaternion.fX, Data.Quaternion.fY, Data.Quaternion.fZ, Data.Quaternion.fW);

	VEHICLE::SET_VEHICLE_MOD_KIT(Game.Vehicle, 0);
	//VEHICLE::SET_VEHICLE_COLOURS(Game.Vehicle, color1, color2);
	VEHICLE::SET_TAXI_LIGHTS(Game.Vehicle, TRUE);
	VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(Game.Vehicle, "FiveMP");

	const int Class = VEHICLE::GET_VEHICLE_CLASS(Game.Vehicle);
	if (Class == 18 || Class == 17 || Class == 15 || Class == 16 || Class == 20 || Class == 14)
	{
		VEHICLE::SET_VEHICLE_MOD(Game.Vehicle, 48, 0, 0);
		VEHICLE::SET_VEHICLE_LIVERY(Game.Vehicle, 0);
	}

	DECORATOR::DECOR_REGISTER("FiveMP_Vehicle", 2);
	DECORATOR::DECOR_SET_BOOL(Game.Vehicle, "FiveMP_Vehicle", TRUE);

	ENTITY::FREEZE_ENTITY_POSITION(Game.Vehicle, FALSE);
	ENTITY::SET_ENTITY_DYNAMIC(Game.Vehicle, TRUE);
	std::cout << "[CVehicleEntity] Created Vehicle" << std::endl;
	Game.Created = true;

	RequestData();
}

// Gets the data thats only needed once thats not synced constantly
void CVehicleEntity::RequestData()
{
	RakNet::BitStream sData;
	sData.Write(Information.Id);
	g_Core->GetNetworkManager()->GetRPC().Signal("RequestEntityData", &sData, HIGH_PRIORITY, RELIABLE_ORDERED, 0, g_Core->GetNetworkManager()->GetSystemAddress(), false, false);
}

void CVehicleEntity::Destroy()
{
	std::cout << "[CVehicleEntity] Removing Vehicle: " << Information.Id << std::endl;

	VEHICLE::DELETE_VEHICLE(&Game.Vehicle);
	Game.Created = false;
	UI::REMOVE_BLIP(&Game.Blip);

	Game = {};
	Information = {};
	Data = {};
	Network = {};
	InterpolationData = {};

	Information.Id = -1;
}

void CVehicleEntity::Pulse()
{
	if (Game.Created && Information.Id != -1)
	{
		int t_CurrentVehicle = GamePed::GetVehicleID(g_Core->GetLocalPlayer()->GetPed());

		// Assignment System
		if (Network.Assigned != RakNetGUID(12345) && Network.Assigned == UNASSIGNED_RAKNET_GUID && Occupants[0] == -1)
		{
			if ((g_Core->GetLocalPlayer()->GetPos() - Data.Position).Length() < 50.0f)
			{
				Network.Assigned = g_Core->GetNetworkManager()->GetInterface()->GetMyGUID();

				RakNet::BitStream sData;
				sData.Write(Information.Id);
				g_Core->GetNetworkManager()->GetRPC().Signal("TakeEntityAssignment", &sData, HIGH_PRIORITY, RELIABLE_ORDERED, 0, g_Core->GetNetworkManager()->GetSystemAddress(), false, false);
			}
		}
		else
		{
			if (g_Core->GetNetworkManager()->GetInterface()->GetMyGUID() == Network.Assigned)
			{
				if ((g_Core->GetLocalPlayer()->GetPos() - Data.Position).Length() > 50.0f)
				{
					Network.Assigned = UNASSIGNED_RAKNET_GUID;

					RakNet::BitStream sData;
					sData.Write(Information.Id);
					g_Core->GetNetworkManager()->GetRPC().Signal("DropEntityAssignment", &sData, HIGH_PRIORITY, RELIABLE_ORDERED, 0, g_Core->GetNetworkManager()->GetSystemAddress(), false, false);
				}
			}
		}

		// Sync
		if ((t_CurrentVehicle != Information.Id || (t_CurrentVehicle == Information.Id && g_Core->GetLocalPlayer()->GetSeat() != 0)) && g_Core->GetNetworkManager()->GetInterface()->GetMyGUID() != Network.Assigned)
		{
			Interpolate();
		}
		else 
		{
			if ((t_CurrentVehicle == Information.Id || g_Core->GetNetworkManager()->GetInterface()->GetMyGUID() == Network.Assigned))
			{
				Vector3 Coordinates = ENTITY::GET_ENTITY_COORDS(Game.Vehicle, ENTITY::IS_ENTITY_DEAD(Game.Vehicle));
				ENTITY::GET_ENTITY_QUATERNION(Game.Vehicle, &Data.Quaternion.fX, &Data.Quaternion.fY, &Data.Quaternion.fZ, &Data.Quaternion.fW);
				Vector3 Velocity = ENTITY::GET_ENTITY_VELOCITY(Game.Vehicle);

				Data.ForwardSpeed = ENTITY::GET_ENTITY_SPEED(Game.Vehicle);
				Data.Position = { Coordinates.x, Coordinates.y, Coordinates.z };
				Data.Velocity = { Velocity.x, Velocity.y, Velocity.z };

				Data.Gear = vdata.GetGearCurr(Game.Vehicle);
				Data.RPM = vdata.GetCurrentRPM(Game.Vehicle);
				Data.Throttle = vdata.GetThrottle(Game.Vehicle);
				Data.ThrottleP = vdata.GetThrottleP(Game.Vehicle);
				Data.BrakeP = vdata.GetBrakeP(Game.Vehicle);
				Data.SteeringAngle = vdata.GetSteeringAngle(Game.Vehicle);

				BitStream bitstream;
				bitstream.Write((unsigned char)ID_PACKET_VEHICLE);

				bitstream.Write(Information.Id);

				bitstream.Write(Data.Position.fX);
				bitstream.Write(Data.Position.fY);
				bitstream.Write(Data.Position.fZ);

				bitstream.Write(Data.ForwardSpeed);

				bitstream.Write(Data.Gear);
				bitstream.Write(Data.RPM);
				bitstream.Write(Data.Throttle);
				bitstream.Write(Data.ThrottleP);
				bitstream.Write(Data.BrakeP);
				bitstream.Write(Data.SteeringAngle);

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
	}
}

void CVehicleEntity::Update(Packet * packet)
{
	BitStream bitstream(packet->data + 1, packet->length + 1, false);

	bitstream.Read(Information.Id);

	RakString model;
	bitstream.Read(model);
	Data.Model = model.C_String();

	bitstream.Read(Data.Heading);

	bitstream.Read(Data.Position.fX);
	bitstream.Read(Data.Position.fY);
	bitstream.Read(Data.Position.fZ);

	bitstream.Read(Data.ForwardSpeed);

	bitstream.Read(Data.Gear);
	bitstream.Read(Data.RPM);
	bitstream.Read(Data.Throttle);
	bitstream.Read(Data.ThrottleP);
	bitstream.Read(Data.BrakeP);
	bitstream.Read(Data.SteeringAngle);

	bitstream.Read(Data.Velocity.fX);
	bitstream.Read(Data.Velocity.fY);
	bitstream.Read(Data.Velocity.fZ);

	bitstream.Read(Data.Quaternion.fX);
	bitstream.Read(Data.Quaternion.fY);
	bitstream.Read(Data.Quaternion.fZ);
	bitstream.Read(Data.Quaternion.fW);

	for (int i = 0; i < SizeOfArray(Occupants); i++)
	{
		bitstream.Read(Occupants[i]);
	}

	
	if (!Game.Created)
		CreateVehicle();

	if (GamePed::GetVehicleID(g_Core->GetLocalPlayer()->GetPed()) != Information.Id) {
		UpdateTargetPosition();
		UpdateTargetData();
		//UpdateTargetRotation();

		Network.LastSyncReceived = timeGetTime();
	}
}

void CVehicleEntity::Interpolate()
{
	SetTargetPosition();
	//SetTargetRotation();
}

void CVehicleEntity::UpdateTargetPosition()
{
	if (Game.Created) {
		unsigned long CurrentTime = timeGetTime();
		unsigned int InterpolationTime = CurrentTime - Network.LastSyncReceived;

		// Get our position
		Vector3 Coordinates = ENTITY::GET_ENTITY_COORDS(Game.Vehicle, ENTITY::IS_ENTITY_DEAD(Game.Vehicle));
		CVector3 CurrentPosition = { Coordinates.x, Coordinates.y, Coordinates.z };

		// Set the target position
		CVector3 TargetPosition = { Data.Position.fX, Data.Position.fY, Data.Position.fZ };
		InterpolationData.Position.Target = TargetPosition;

		// Calculate the relative error
		InterpolationData.Position.Error = TargetPosition - CurrentPosition;

		// Get the interpolation interval
		InterpolationData.Position.StartTime = CurrentTime;
		InterpolationData.Position.FinishTime = (CurrentTime + InterpolationTime);

		// Initialize the interpolation
		InterpolationData.Position.LastAlpha = 0.0f;
	}
}

void CVehicleEntity::SetTargetPosition()
{
	if (InterpolationData.Position.FinishTime != 0 && Game.Created) {
		// Get our position
		Vector3 Coordinates = ENTITY::GET_ENTITY_COORDS(Game.Vehicle, ENTITY::IS_ENTITY_DEAD(Game.Vehicle));
		CVector3 vecCurrentPosition = { Coordinates.x, Coordinates.y, Coordinates.z };

		// Get the factor of time spent from the interpolation start to the current time.
		unsigned long CurrentTime = timeGetTime();
		float fAlpha = Math::Unlerp(InterpolationData.Position.StartTime, CurrentTime, InterpolationData.Position.FinishTime);

		// Don't let it overcompensate the error
		fAlpha = Math::Clamp(0.0f, fAlpha, 1.0f);

		// Get the current error portion to compensate
		float fCurrentAlpha = (fAlpha - InterpolationData.Position.LastAlpha);
		InterpolationData.Position.LastAlpha = fAlpha;

		// Apply the error compensation
		CVector3 vecCompensation = Math::Lerp(CVector3(), fCurrentAlpha, InterpolationData.Position.Error);

		// If we finished compensating the error, finish it for the next pulse
		if (fAlpha == 1.0f)
			InterpolationData.Position.FinishTime = 0;

		// Calculate the new position
		CVector3 vecNewPosition = vecCurrentPosition + vecCompensation;

		// Check if the distance to interpolate is too far
		if ((vecCurrentPosition - InterpolationData.Position.Target).Length() > 750.0f)
		{
			// Abort all interpolation
			InterpolationData.Position.FinishTime = 0;
			vecNewPosition = InterpolationData.Position.Target;
		}

		// Set our new position
		ENTITY::SET_ENTITY_COORDS_NO_OFFSET(Game.Vehicle, vecNewPosition.fX, vecNewPosition.fY, vecNewPosition.fZ, false, false, false);
		ENTITY::SET_ENTITY_VELOCITY(Game.Vehicle, Data.Velocity.fX, Data.Velocity.fY, Data.Velocity.fZ);
		ENTITY::SET_ENTITY_QUATERNION(Game.Vehicle, Data.Quaternion.fX, Data.Quaternion.fY, Data.Quaternion.fZ, Data.Quaternion.fW);
	}
}

void CVehicleEntity::UpdateTargetRotation()
{
	unsigned int interpolationtime = timeGetTime() - Network.LastSyncReceived;
	unsigned long ulTime = timeGetTime();

	// Get our position
	CVector3 vecLocalRotation;
	float unusedw;
	ENTITY::GET_ENTITY_QUATERNION(Game.Vehicle, &vecLocalRotation.fX, &vecLocalRotation.fY, &vecLocalRotation.fZ, &unusedw);

	// Set the target rotation
	CVector3 vecRotation = { Data.Quaternion.fX, Data.Quaternion.fY, Data.Quaternion.fZ };
	InterpolationData.Rotation.Target = vecRotation;

	// Get the error
	InterpolationData.Rotation.Error = Math::GetOffsetDegrees(vecLocalRotation, vecRotation);
	InterpolationData.Rotation.Error *= Math::Lerp < const float >(0.40f, Math::UnlerpClamped(100, interpolationtime, 400), 1.0f);

	// Get the interpolation interval
	InterpolationData.Rotation.StartTime = ulTime;
	InterpolationData.Rotation.FinishTime = (ulTime + interpolationtime);

	// Initialize the interpolation
	InterpolationData.Rotation.LastAlpha = 0.0f;
}

void CVehicleEntity::SetTargetRotation()
{
	if (InterpolationData.Rotation.FinishTime != 0) {
		CVector3 vecCurrentRotation;

		// Get our rotation
		float unusedw;
		ENTITY::GET_ENTITY_QUATERNION(Game.Vehicle, &vecCurrentRotation.fX, &vecCurrentRotation.fY, &vecCurrentRotation.fZ, &unusedw);

		// Get the factor of time spent from the interpolation start to the current time.
		unsigned long ulCurrentTime = timeGetTime();
		float fAlpha = Math::Unlerp(InterpolationData.Rotation.StartTime, ulCurrentTime, InterpolationData.Rotation.FinishTime);

		// Don't let it overcompensate the error
		fAlpha = Math::Clamp(0.0f, fAlpha, 1.0f);

		// Get the current error portion to compensate
		float fCurrentAlpha = (fAlpha - InterpolationData.Rotation.LastAlpha);
		InterpolationData.Rotation.LastAlpha = fAlpha;

		// Apply the error compensation
		CVector3 vecCompensation = Math::Lerp(CVector3(), fCurrentAlpha, InterpolationData.Rotation.Error);

		// If we finished compensating the error, finish it for the next pulse
		if (fAlpha == 1.0f)
			InterpolationData.Rotation.FinishTime = 0;

		// Calculate the new position
		CVector3 vecNewRotation = vecCurrentRotation + vecCompensation;

		// Set our new position
		ENTITY::SET_ENTITY_QUATERNION(Game.Vehicle, vecNewRotation.fX, vecNewRotation.fY, vecNewRotation.fZ, unusedw);
	}
}

void CVehicleEntity::UpdateTargetData()
{
	vdata.SetGearCurr(Game.Vehicle, Data.Gear);
	vdata.SetCurrentRPM(Game.Vehicle, Data.RPM);
	vdata.SetThrottle(Game.Vehicle, Data.Throttle);
	vdata.SetThrottleP(Game.Vehicle, Data.ThrottleP);
	vdata.SetBrakeP(Game.Vehicle, Data.BrakeP);
	vdata.SetSteeringAngle(Game.Vehicle, Data.SteeringAngle);
}


Vehicle CVehicleEntity::getClosestVehicleFromPedPos(Ped ped, int maxDistance)
{
	Vector3 ppos = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(ped, 0.0, 0.0, -1.0f);
	CVector3 playerPos = CVector3(ppos.x, ppos.y, ppos.z);

	Vehicle vehicle = 0;
	float lastMaxDistance = maxDistance;

	if (!g_Vehicles.empty())
	{
		for (int i = 0; i < g_Vehicles.size(); i++)
		{
			if (g_Vehicles[i].GetId() != -1)
			{
				Vector3 vpos = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(g_Vehicles[i].GetEntity(), 0.0, 0.0, 0.0f);
				CVector3 vehiclePos = CVector3(vpos.x, vpos.y, vpos.z);

				float distance = Math::GetDistanceBetweenPoints3D(ppos.x, ppos.y, ppos.z, vpos.x, vpos.y, vpos.z);
				if (distance < lastMaxDistance)
				{
					lastMaxDistance = distance;
					vehicle = g_Vehicles[i].GetEntity();
				}

			}
		}
	}

	return vehicle;
}
