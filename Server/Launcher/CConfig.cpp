#include "stdafx.h"

bool CConfig::Read()
{
	INIReader Config("Server.Config.ini");

	if (Config.ParseError() < 0) {
		std::cout << "Can't load 'Server.Core.ini'\n";
#ifdef NDEBUG
		return false;
	}
	else 
	{
#endif
		cout << "[CConfig] Loaded config" << endl;

		Connection.Ip		= Config.Get("Connection", "ip", "127.0.0.1");
		Connection.Port		= Config.GetInteger("Connection", "port", 2322);
		Connection.Pass		= Config.Get("Connection", "pass", "default");
		Connection.Announce = Config.GetBoolean("Connection", "announce", false);

		Limits.MaxPlayers	= Config.GetInteger("Limits", "players", 50);

		Information.Name = Config.Get("Information", "Name", "FiveMP Server (" INFO_VERSION ")");

		Plugins.Language = Config.Get("Plugins", "lang", "API.Lua");
		Plugins.Additional = Config.Get("Plugins", "etc", "");

		cout << "[CConfig] Read config" << endl;
		return true;
	}
	return false;
}