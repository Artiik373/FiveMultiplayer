#pragma once

class CNPCEntity
{
private:
	struct GameInfo
	{
		Ped			Npc;
		bool		Created;

		Blip		Blip;
	} Game;


	struct NPCData
	{
		int					Id;

		struct ModelData
		{
			std::string		Model;
			int				Type;
		} Model;

		struct WeaponData
		{
			unsigned long	Weapon;
			bool			Reload;
		} Weapon;

		struct Vehicle
		{
			int			VehicleID;
			int			Seat;
		} Vehicle;

		struct Component
		{
			int drawableid = -1;
			int textureid = -1;
			int paletteid = -1;
		} ModelComponents[12];

		struct HeadBlend
		{
			int shapeFirst = 0;
			int shapeSecond = 0;
			int shapeThird = 0;
			int skinFirst = 0;
			int skinSecond = 0;
			int skinThird = 0;
			float shapeMix = 0;
			float skinMix = 0;
			float thirdMix = 0;
		} ModelHeadBlend;

		struct HeadOverlay
		{
			int index = 0;
			float opacity = 0.0f;
			int colorType = 0;
			int colorID = 0;
			int secondColorID = 0;
		} ModelHeadOverlay[13];

		struct Prop
		{
			int drawableid = 0;
			int textureid = 0;
		} ModelProp[3];

		struct Feature
		{
			float scale = 0.0f;
		} ModelFaceFeature[20];

		float				ForwardSpeed;

		CVector3			Position;
		CVector3			Velocity;
		CVector3			Rotation;
	} Data;

	struct Network
	{
		std::chrono::time_point<std::chrono::system_clock> LastSyncSent;
		bool				Synchronized;
	} Network;

public:
	static int Amount;

	CNPCEntity();
	~CNPCEntity() {}

	bool Create(const int entity, const std::string model, const CVector3 position, const CVector3 rotation);
	bool CreateNpc();
	void RequestData();
	void Destroy();
	void Delete();

	void Pulse();
	void Update(Packet *packet);

	int				GetId() { return Data.Id; }
	void			SetId(const int playerid) { Data.Id = playerid; }

	const Entity	GetEntity() { return Game.Npc; }

	bool			IsCreated() { return Game.Created; }

	int				GetVehicle() { return Data.Vehicle.VehicleID; }
	int				GetVehicleSeat() { return Data.Vehicle.Seat; }

	CVector3		GetPosition() { return Data.Position; }
	void			SetPosition(const CVector3 position) { Data.Position = position; }

	CVector3		GetRotation() { return Data.Rotation; }
	void			SetRotation(const CVector3 rotation) { Data.Rotation = rotation; }

	const std::string GetModel() { return Data.Model.Model; }
	void			SetModel(const std::string model) { Data.Model.Model = model; }

	NPCData::Component GetModelComponent(const int index) { return Data.ModelComponents[index]; }
	void			SetModelComponent(const int index, const int drawableid, const int textureid, const int paletteid);

	NPCData::HeadBlend GetModelHeadBlend() { return Data.ModelHeadBlend; }
	void			SetModelHeadBlend(const int shapeFirst, const float shapeMix, const int shapeSecond, const int shapeThird, const int skinFirst, const float skinMix, const int skinSecond, const int skinThird, const float thirdMix);

	NPCData::HeadOverlay GetModelHeadOverlay(const int index) { return Data.ModelHeadOverlay[index]; }
	void			SetModelHeadOverlay(const int index, const int type, const int colorType, const int colorID, const int secondColorID, const float opacity);

	NPCData::Prop	GetModelProp(const int index) { return Data.ModelProp[index]; }
	void			SetModelProp(const int index, const int drawableid, const int textureid);

	float			GetModelFaceFeature(const int index) { return Data.ModelFaceFeature[index].scale; }
	void			SetModelFaceFeature(const int index, const float scale);

	bool			GetSynchronized() { return Network.Synchronized; }

	void			RequestData(RakNetGUID requester);

	NPCData		SetData(NPCData newdata) { Data = newdata; }

	NPCData		GetData() { return Data; }
};
extern std::vector<CNPCEntity> g_Npcs;
