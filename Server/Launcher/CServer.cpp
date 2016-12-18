#include "stdafx.h"

CAPI*				g_API;
CNetworkManager*	g_Network;

CServer* CServer::p_Instance = nullptr;

vector<CPlayerEntity> g_Players;

CServer::CServer()
{
	// Tell CServer we're using this particular class
	p_Instance = this;

	// Maximum amount of packets sent per second
	p_SyncRate = 10;
	// Unused, but it's supposed to limit the server FPS to 100.
	p_MaxFPS = 100;
	// If true, it shows the FPS in the console window. (windows only)
	p_ShowFPS = true;

	// Construct CNetworkManager
	g_Network = new CNetworkManager();

	// Construct CAPI
	g_API = new CAPI;

	// Construct CPlayerManager
	//g_Players = new CPlayerManager();

	cout << "[CServer] Constructed" << endl;
}


CServer::~CServer()
{
	SAFE_DELETE(g_Network);
	SAFE_DELETE(g_API);
	//SAFE_DELETE(g_Players);

	cout << "[CServer] Deconstructed" << endl;
}

bool CServer::Load(int argc, char ** argv)
{
#ifdef _WIN32
	// Set Window Title containing the modname, modversion and build type
	SetConsoleTitle(L"" INFO_NAME "(" INFO_VERSION ") - " INFO_BUILD);
#endif

	g_Network->Start();

	if (!g_API)
	{
		cout << "[CServer] invalid" << endl;
		getc(stdin);
		return 1;
	}

	// Load API.Lua plugin
#ifdef USEAPI
	if (!g_API->Load("plugins/API.Lua" LIBRARY_EXTENSION))
	{
		cout << "[CAPI]" << g_API->ModuleName() << " could not be loaded" << endl;
		getc(stdin);
		return false;
	}

	// Call Initialize function on our API
	if (!g_API->Initialize())
	{
		cout << "[CAPI]" << g_API->ModuleName() << " could not be initialized" << endl;
		getc(stdin);
		return false;
	}
#endif

	p_Active = true;
	return true;
}

void CServer::Stop()
{
	// Call Close function on our API
	if (!g_API->Close())
	{
		cout << "[CServer] " << g_API->ModuleName() << " could not be closed" << endl;
		getc(stdin);
	}
	p_Active = false;
}

void CServer::Process()
{
	// Keep CNetworkManager active
	g_Network->Pulse();

	// Pulse all players
	for (int i = 0; i < g_Players.size(); i++) {
		g_Players[i].Pulse();
	}

	// Show FPS in console window (windows only)
	if (p_ShowFPS) ShowFPS();
}

void CServer::ShowFPS()
{
#ifdef _WIN32
	unsigned long Time = timeGetTime();
	unsigned long TimeElapsed = (Time - p_LastFPSUpdateTime);

	if (TimeElapsed >= 1000)
	{
		p_FPS = p_FrameCount;

		char title[128];
		sprintf_s(title, "" INFO_NAME "(" INFO_VERSION ") - " INFO_BUILD " %dfps", p_FPS);
		SetConsoleTitleA(title);

		p_FrameCount = 0;
		p_LastFPSUpdateTime = Time;
	}
	p_FrameCount++;
#endif
}
