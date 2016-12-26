#pragma once

class CPlayerEntity {
private:
	struct PlayerInfo {
		int			Id;
		string		Name;
	} Information;

	struct PlayerStats {
		int			Score;
	} Statistics;

	struct PlayerData {
		float		ForwardSpeed;
		CVector3	Position;
		CVector3	Velocity;
		CVector4	Quaternion;
	} Data;

	struct PlayerNetwork {
		unsigned long LastSyncSent;
		bool Synchronized;
		RakNetGUID GUID;
		SystemAddress Ip;
	} Network;

public:
	static int Amount;

	CPlayerEntity() { };
	~CPlayerEntity() { };

	void Create(string Name, RakNetGUID GUID, SystemAddress Ip);
	void Destroy();

	void Pulse();
	void Update(Packet *packet);

	int GetId() { return Information.Id; };
	string GetUsername() { return Information.Name; };

	int GetScore() { return Statistics.Score; };

	CVector3 GetPosition() { return Data.Position; };
	CVector4 GetQuaternion() { return Data.Quaternion; }

	bool GetSynchronized()	{ return Network.Synchronized; }
	RakNetGUID GetGUID()	{ return Network.GUID; };
	SystemAddress GetIp()	{ return Network.Ip; }

	PlayerInfo SetInfo(PlayerInfo newinfo) { Information = newinfo; }
	PlayerStats SetStats(PlayerStats newstats) { Statistics = newstats; }
	PlayerData SetData(PlayerData newdata) { Data = newdata; }

	PlayerInfo GetInfo() { return Information; }
	PlayerStats GetStats() { return Statistics; }
	PlayerData GetData() { return Data; }
};
extern vector<CPlayerEntity> g_Players;