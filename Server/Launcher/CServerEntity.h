#pragma once
/*
 * The global server entity ids
*/
class CServerEntity {
public:
	enum Type
	{
		Player = 0,
		Vehicle = 1,
		Object = 2
	};
private:
	struct Entity {
		int Id;
		Type type;
	} Data;

public:
	CServerEntity() { }
	~CServerEntity() { }

	int			Create();
	void		Destroy();

	Type		GetType() { return Data.type; }
	void		SetType(Type type);

	int			GetId() { return Data.Id; }
	void		SetId(int entity) { Data.Id = entity;  }

	CVector3	GetPosition();
	void		SetPosition(CVector3 position);

	CVector4	GetQuaternion();
	void		SetQuaternion(CVector4 quaternion);
};
extern std::vector<CServerEntity> g_Entities;

namespace ServerEntity
{
	int			GetIndex(int entity);
	bool		IsValid(int entity);
	RakNetGUID	GetAssignee(int entity);
	void		SetAssignee(int entity, RakNetGUID assignee);
	void		RequestData(int entity, RakNetGUID requester);
}