#include "stdafx.h"

//#define TESTING

bool to_bool(std::string str)
{
	std::locale loc;
	for (std::string::size_type i = 0; i<str.length(); ++i)
		std::cout << std::tolower(str[i], loc);

	std::istringstream is(str);
	bool b;
	is >> std::boolalpha >> b;
	return b;
};

Checkpoint newCP;

// When Plugin gets loaded
extern "C" DLL_PUBLIC bool API_Initialize(void) 
{
	API::Server::PrintMessage(L"Gamemode Initializing...");

#ifdef TESTING
	API::Vehicle::Create(L"elegy", CVector3{ -3.0f, 6.0f, 73.0f }, 10.0f);
	API::Vehicle::Create(L"comet3", CVector3{ -6.0f, 8.0f, 73.0f }, 10.0f);
	API::Vehicle::Create(L"blazer5", CVector3{ -9.0f, 10.0f, 73.0f }, 10.0f);
	API::Vehicle::Create(L"voltic2", CVector3{ -12.0f, 12.0f, 73.0f }, 10.0f);
#else
	API::Vehicle::Create(L"elegy", CVector3{ 1533.53f, 3282.39f, 52.5f }, 195.0f);
	API::Vehicle::Create(L"comet3", CVector3{ 1527.65f, 3296.66f, 52.5f }, 195.0f);
	API::Vehicle::Create(L"blazer5", CVector3{ 1519.32f, 3280.2f, 52.5f }, 195.0f);
	API::Vehicle::Create(L"voltic2", CVector3{ 1516.84f, 3293.88f, 52.5f }, 195.0f);
#endif

	API::World::SetTime(13, 0, 0);

	int hour, minute, second;
	API::World::GetTime(&hour, &minute, &second);
	std::cout << "Time: " << hour << ":" << minute << ":" << second << std::endl;

#ifdef TESTING
	//API::NPC::Create(L"s_m_m_movspace_01", CVector3(0.0f, 0.0f, 74.0f), CVector3(0.0f, 0.0f, 90.0f));

	//API::Checkpoint::Create(CVector3(0.0f, 0.0f, 74.0f), CVector3(0.0f, 0.0f, 124.0f), 1, 5.0f, Color{ 255,0,0,255 }, 0);
#else
	API::NPC::Create(L"s_m_m_movspace_01", CVector3(1527.62f, 3274.39f, 53.0f), CVector3(0.0f, 0.0f, 90.0f));

	//API::Checkpoint::Create(CVector3(1527.62f, 3274.39f, 53.0f), CVector3(1527.62f, 3274.39f, 153.0f), 1, 5.0f, Color{ 255,0,0,255 }, 0);

	newCP.Create(CVector3(1527.62f, 3274.39f, 52.0f), CVector3(1527.62f, 3274.39f, 153.0f), 1, 5.0f, Color{ 255,0,0,255 }, 0);

	// Load Objects
	Json::Value root;
	Json::Reader reader;

	std::ifstream stream("./maps/objects.json", std::ifstream::binary);
	stream >> root;
	
	if (!reader.parse(stream, root, false))
	{
		// report to the user the failure and their locations in the document.
		std::cout << reader.getFormattedErrorMessages() << std::endl;;
	}
	
	const int objectcount = root["Map"]["Objects"]["MapObject"].size();

	for (int i = 0; i < objectcount; i++)
	{
		CVector3 position(
			(float)std::atof(root["Map"]["Objects"]["MapObject"][i]["Position"]["X"].asCString()),
			(float)std::atof(root["Map"]["Objects"]["MapObject"][i]["Position"]["Y"].asCString()),
			(float)std::atof(root["Map"]["Objects"]["MapObject"][i]["Position"]["Z"].asCString())
		);
		
		CVector3 rotation = {
			(float)std::atof(root["Map"]["Objects"]["MapObject"][i]["Rotation"]["X"].asCString()),
			(float)std::atof(root["Map"]["Objects"]["MapObject"][i]["Rotation"]["Y"].asCString()),
			(float)std::atof(root["Map"]["Objects"]["MapObject"][i]["Rotation"]["Z"].asCString()),
		};
		
		const int ent = API::Object::Create(atoi(root["Map"]["Objects"]["MapObject"][i]["Hash"].asCString()), position, rotation, to_bool(root["Map"]["Objects"]["MapObject"][i]["Dynamic"].asCString()));
		API::Object::SetTextureVariation(ent, 2);
	}
	// END Load Objects
	
#endif

	API::Server::PrintMessage(L"Gamemode Initialized!");
	return true;
}

// When plugin gets unloaded
extern "C" DLL_PUBLIC bool API_Close(void) 
{
	std::cout << "close" << std::endl;
	return true;
}

// Every server tick this gets called
extern "C" DLL_PUBLIC bool API_OnTick(void) 
{
	//API::Visual::ShowMessageAboveMap("I don't like people!", "CHAR_DEFAULT", 1, "Jack", "");
	return true;
}

// Player Connecting
extern "C" DLL_PUBLIC bool API_OnPlayerConnecting(const std::string guid)
{
	std::wstringstream oss;
	oss << L"Player connecting with [guid: " << guid.c_str() << L"]";
	API::Server::PrintMessage(oss.str());

	oss.str(std::wstring());
	oss.clear();

	oss << L"~p~A Player is connecting [" << guid.c_str() << L"]";
	API::Visual::ShowMessageAboveMap(oss.str(), L"CHAR_DEFAULT", 1, L"Server", L"Player Joining");
	return true;
}

// Player Connected
extern "C" DLL_PUBLIC bool API_OnPlayerConnected(int entity, int playerid)
{
	std::wstringstream oss;
	oss << L"~g~You Connected! ~o~[~w~ID: " << playerid << L"~o~]";
	API::Visual::ShowMessageAboveMapToPlayer(entity, oss.str().c_str(), L"CHAR_CREATOR_PORTRAITS", 1, L"Server", L"");

	//API::Player::SetModel(entity, "u_m_y_pogo_01");

#ifdef TESTING
	API::Entity::SetPosition(entity, CVector3(0.0f, 0.0f, 73.5f));
#else
	API::Entity::SetPosition(entity, CVector3(1527.62f, 3274.39f, 53.0f));
#endif

	CVector3 position = API::Entity::GetPosition(entity);
	
	oss.str(std::wstring());
	oss.clear();

	oss << L"~p~Position: " << position.x << L" " << position.y << L" " << position.z;
	API::Visual::ShowMessageAboveMapToPlayer(entity, oss.str().c_str(), L"CHAR_CREATOR_PORTRAITS", 5, L"Server", L"Position");

	newCP.Show(entity);

	return true;
}

// When a entity enters a checkpoint (only players right now)
extern "C" DLL_PUBLIC void API_OnEntityEnterCheckpoint(int checkpoint, int entity)
{
	API::Server::PrintMessage(L"OnEntityEnterCheckpoint");
}

// When a entity exits a checkpoint (only players right now)
extern "C" DLL_PUBLIC void API_OnEntityExitCheckpoint(int checkpoint, int entity)
{
	API::Server::PrintMessage(L"OnEntityExitCheckpoint");
}