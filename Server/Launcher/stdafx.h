#pragma once

// Windows/UNIX
#ifdef _WIN32
#include <winsock2.h>
#include <Windows.h>
#else
#define _BSD_SOURCE

#include <dlfcn.h>
#include <sys/time.h>
#endif
#include <stdio.h>

// C++
#include <iostream>
#include <ctime>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>
#include <map>
#include <cctype>
#include <cstdlib>
#include <cstdio>
#include <functional>
using namespace std;

// Shared
#include <Common.h>
#include <CVector3.h>
#include <CVector4.h>

// Vendor
	// Vendor.RakNet
	#include <RakNet/RakPeerInterface.h>
	#include <RakNet/BitStream.h>
	#include <RakNet/MessageIdentifiers.h>
	#include <RakNet/RPC4Plugin.h>
	#include <RakNet/DirectoryDeltaTransfer.h>
	#include <RakNet/FileListTransfer.h>
	#include <RakNet/IncrementalReadInterface.h>
	using namespace RakNet;

	// Vendor.LibCurl
	#include <LibCurl/curl/curl.h>
// End of Vendor

// Server.Launcher
	// Include
	#include "IniReader.h"
	#include "ini.h"

	// Server
	#include "CServer.h"
	#include "CConfig.h"

	// API
	#include "CAPI.h"
	#include "API_Server.h"

	// Network
	#include "CNetworkManager.h"
	#include "CPlayerEntity.h"

// End of Server.Launcher