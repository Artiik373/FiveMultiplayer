#include "stdafx.h"

#include "RPCManager.h"

CConfig*			g_Config;

CServer* CServer::p_Instance = nullptr;

std::vector<CServerEntity>	g_Entities;
std::vector<CPlayerEntity>	g_Players;
std::vector<CVehicleEntity>	g_Vehicles;
std::vector<CObjectEntity>	g_Objects;
std::vector<CNPCEntity>		g_Npcs;
std::vector<CCheckpointEntity>		g_Checkpoints;
std::vector<CAPI>			g_ApiModules;

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

	// Construct CConfig
	g_Config = new CConfig();

	// Construct CNetworkManager
	s_NetworkManager = std::unique_ptr<CNetworkManager>(new CNetworkManager);

	// Construct CWorld
	s_World = std::unique_ptr<CWorld>(new CWorld);

	std::cout << "[CServer] Constructed" << std::endl;
}


CServer::~CServer()
{
	SAFE_DELETE(g_Config);

	std::cout << "[CServer] Deconstructed" << std::endl;
}

#define USEAPI

bool CServer::Load(int argc, char ** argv)
{

#ifdef _WIN32
	// Set Window Title containing the modname, modversion and build type
	SetConsoleTitle(L"" INFO_NAME "(" INFO_VERSION ") - " INFO_BUILD);
#endif

	if (!g_Config)
	{
		std::cout << "[CConfig] Invalid" << std::endl;
		getc(stdin);
		return 1;
	}

	if (!g_Config->Read())
	{
		std::cout << "[CConfig] Could not read config file" << std::endl;
		getc(stdin);
		return 1;
	}

	if (!GetNetworkManager()->Start())
	{
		std::cout << "[CNetworkManager] Could not be started" << std::endl;
		getc(stdin);
		return 1;
	}

	// Register RPG Messages
	RPC::Manager::RegisterRPCMessages();
	
	// Split the plugin string for each defined plugin and insert into the pool.
	CAPI NewModule;
	std::string module = "./plugin/";
	
	for (int c = 0; c < g_Config->GetLanguage().size(); c++)
	{
		if (g_Config->GetLanguage()[c] != ' ')
		{
			module.push_back(g_Config->GetLanguage()[c]);
		}
		else
		{
			module += LIBRARY_EXTENSION;
			NewModule.SetModuleName(module);
			g_ApiModules.push_back(NewModule);
			module = "./plugin/";
		}

		if (c == g_Config->GetLanguage().size() - 1)
		{
			module += LIBRARY_EXTENSION;
			NewModule.SetModuleName(module);
			g_ApiModules.push_back(NewModule);
		}
	}

#ifdef USEAPI
	// Load plugin modules
	for (int i = 0; i < g_ApiModules.size(); i++)
	{
		if (!g_ApiModules[i].Load(g_ApiModules[i].ModuleName().c_str()))
		{
			g_ApiModules.erase(g_ApiModules.begin() + i);
		}
	}
	
	g_ApiModules.shrink_to_fit();

	// Call Initialize function on our API
	for (int i = 0; i < g_ApiModules.size(); i++)
	{
		if (!g_ApiModules[i].Initialize())
		{
			std::cout << "[CAPI] " << g_ApiModules[i].ModuleName() << " could not be initialized" << std::endl;
		}
	}
#endif
	
	p_Active = true;
	return true;
}

void CServer::Stop()
{
	// Call Close function on our API
	for (int i = 0; i < g_ApiModules.size(); i++)
	{
		if (!g_ApiModules[i].Close())
		{
			std::cout << "[CServer] " << g_ApiModules[i].ModuleName() << " could not be closed" << std::endl;
			getc(stdin);
		}
	}
	p_Active = false;
}

void CServer::Process()
{
	// Keep CNetworkManager active
	GetNetworkManager()->Pulse();

	// Pulse all players
	for (int i = 0; i < g_Players.size(); i++) {
		g_Players[i].Pulse();
	}

	// Pulse all vehicles
	for (int i = 0; i < g_Vehicles.size(); i++)
	{
		g_Vehicles[i].Pulse();
	}

	for (int i = 0; i < g_Objects.size(); i++)
	{
		g_Objects[i].Pulse();
	}

	for (int i = 0; i < g_ApiModules.size(); i++)
	{
		g_ApiModules[i].OnTick();
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

#ifndef _WIN32
unsigned int timeGetTime()
{
	struct timeval now;
	gettimeofday(&now, NULL);
	return now.tv_usec / 1000;
}
#endif