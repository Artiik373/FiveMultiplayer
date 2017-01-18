#include "stdafx.h"

CNetworkManager::CNetworkManager()
{
	// Get RakPeerInterface
	g_RakPeer = RakPeerInterface::GetInstance();

	// Get RPC4
	g_RPC = RPC4::GetInstance();

	// Attach RPC4 to RakPeerInterface
	g_RakPeer->AttachPlugin(g_RPC);

	// RakPeerInterface Settings
	g_RakPeer->SetSplitMessageProgressInterval(100);

	Logger::Msg("CNetworkManager::Constructed");

	g_ConnectionState = CONSTATE_DISC;
}

CNetworkManager::~CNetworkManager()
{
	// Stop RakNet, stops synchronization
	g_RakPeer->Shutdown(2000);

	// Detach RPC4 from RakPeerInterface
	g_RakPeer->DetachPlugin(g_RPC);

	// Destroy RPC4
	RPC4::DestroyInstance(g_RPC);

	// Destroy RakPeerInterface
	RakPeerInterface::DestroyInstance(g_RakPeer);

	Logger::Msg("CNetworkManager::Deconstructed");
}

bool CNetworkManager::Start()
{
	Logger::Msg("CNetworkManager::Started");

	// Return whether Startup worked or not
	return (g_RakPeer->Startup(1, &SocketDescriptor(), 1, THREAD_PRIORITY_NORMAL) == RAKNET_STARTED);
}

void CNetworkManager::Stop()
{
	// Disconnect if we're connected
	if (g_ConnectionState == CONSTATE_COND)
		Disconnect();

	// Stop our RakPeerInterface
	g_RakPeer->Shutdown(500);

	Logger::Msg("CNetworkManager::Stopped");
}

void CNetworkManager::Connect(const char *ip, const char *pass, int port)
{
	// Disconnect if we're already connected
	if (g_ConnectionState == CONSTATE_COND)
		Disconnect();

	// Set our last connection so we can connect again later and we set our state to connecting
	SetLastConnection(ip, pass, port);
	g_ConnectionState = CONSTATE_CONN;

	// Connect to the server, leaving the result
	int Result = g_RakPeer->Connect(ip, port, pass, sizeof(pass));

	// Check if connection failed, then set our state to failed
	if (Result != 0)
	{
		g_ConnectionState = CONSTATE_FAIL;
		Logger::Msg("Failed to connect, errorcode: %d", Result);
		return;
	}
}

void CNetworkManager::Disconnect()
{
	// Don't do anything if we're not connected
	if (g_ConnectionState == CONSTATE_DISC)
		return;

	// Stop RakPeer from accepting anymore incoming packets
	g_RakPeer->CloseConnection(g_SystemAddr, true);

	// Set our state to disconnected
	g_ConnectionState = CONSTATE_DISC;

	// Unregister RPCs
	g_Core->GetRPCManager()->UnregisterRPCMessages();

	// Reinitialize our RakPeerInterface
	Stop();
	Start();

	// Register RPCs
	g_Core->GetRPCManager()->RegisterRPCMessages();

	// Clean the server GUID
	g_SystemAddr = UNASSIGNED_SYSTEM_ADDRESS;

	// Remove all existing players
	for (int i = 0; i < g_Players.size(); i++) {
		g_Players[i].Destroy();
		g_Players.erase(g_Players.begin() + i);
	}
	// Shrink vector so size is correct.
	g_Players.shrink_to_fit();
	std::cout << "[CPlayerEntity] Players Online: " << g_Players.size() << std::endl;

	// Remove all existing vehicles
	for (int i = 0; i < g_Vehicles.size(); i++)
	{
		g_Vehicles[i].Destroy();
		g_Vehicles.erase(g_Vehicles.begin() + i);
	}
	// Shrink vector so size is correct.
	g_Vehicles.shrink_to_fit();
	std::cout << "[CPlayerEntity] Vehicles: " << g_Vehicles.size() << std::endl;

	Logger::Msg("CNetworkManager::Disconnected");
}

void CNetworkManager::Pulse()
{
	// Don't do anything if we're disconnected
	if (g_ConnectionState == CONSTATE_DISC || g_ConnectionState == CONSTATE_FAIL)
		return;

	Packet *g_Packet = NULL;

	while (g_Packet = g_RakPeer->Receive())
	{
		BitStream g_BitStream(g_Packet->data + 1, g_Packet->length + 1, false);

		switch (g_Packet->data[0])
		{
			case ID_UNCONNECTED_PONG:
			{
				break;
			}
			case ID_ADVERTISE_SYSTEM:
			{
				break;
			}
			case ID_DOWNLOAD_PROGRESS:
			{
				break;
			}
			case ID_IP_RECENTLY_CONNECTED:
			{
				Logger::Msg("Failed to connect, recently connected", g_Packet->data[0]);
				break;
			}
			case ID_INCOMPATIBLE_PROTOCOL_VERSION:
			{
				Logger::Msg("Failed to connect, incompatible protocol version", g_Packet->data[0]);
				break;
			}
			case ID_ALREADY_CONNECTED:
			{
				Logger::Msg("Failed to connect, already connected", g_Packet->data[0]);
				break;
			}
			case ID_NO_FREE_INCOMING_CONNECTIONS:
			{
				Logger::Msg("Failed to connect, max client", g_Packet->data[0]);
				break;
			}
			case ID_INVALID_PASSWORD:
			{
				Logger::Msg("Failed to connect, invalid password", g_Packet->data[0]);
				break;
			}
			case ID_CONNECTION_ATTEMPT_FAILED:
			{
				Logger::Msg("Failed to connect, failed", g_Packet->data[0]);
				break;
			}
			case ID_CONNECTION_BANNED:
			{
				Logger::Msg("Failed to connect, banned", g_Packet->data[0]);
				break;
			}
			case ID_CONNECTION_REQUEST_ACCEPTED:
			{
				// Set the server Adress
				g_SystemAddr = g_Packet->systemAddress;

				// Set our state to connected
				g_ConnectionState = CONSTATE_COND;

				// Set our last packet update so it sends our own packets too
				g_Core->GetLocalPlayer()->SetLastSync(timeGetTime());

				Logger::Msg("CNetworkManager::Connected");
				break;
			}
			case ID_CONNECTION_LOST:
			{
				Disconnect();
				Logger::Msg("CNetworkManager::ConnectionLost");
				break;
			}
			case ID_PACKET_PLAYER:
			{
				RakNetGUID tempGUID;
				g_BitStream.Read(tempGUID);

				bool exist = false;

				if (!g_Players.empty()) {
					for (int i = 0; i < g_Players.size(); i++) {
						if (std::strcmp(g_Players[i].GetGUID().ToString(), tempGUID.ToString()) == 0) {
							g_Players[i].Update(g_Packet);
							exist = true;
							i = (int)g_Players.size();
						}
					}
				}
				if (!exist) {
					int entityId;
					g_BitStream.Read(entityId);

					CPlayerEntity newPlayer;
					newPlayer.Create("User", tempGUID, entityId);
					g_Players.push_back(newPlayer);

					std::cout << "[CPlayerEntity] Players Online: " << g_Players.size() << std::endl;
				}
				break;
			}
			case ID_PACKET_VEHICLE:
			{
				int t_Id;
				g_BitStream.Read(t_Id);

				bool t_Existing = false;

				if (!g_Vehicles.empty())
				{
					for (int i = 0; i < g_Vehicles.size(); i++)
					{
						if (g_Vehicles[i].GetId() == t_Id)
						{
							g_Vehicles[i].Update(g_Packet);
							t_Existing = true;
							i = (int)g_Vehicles.size();
						}
					}
				}
				if (!t_Existing)
				{
					CVehicleEntity newVehicle;
					newVehicle.Create(t_Id);
					g_Vehicles.push_back(newVehicle);

					std::cout << "[CPlayerEntity] Vehicle Count: " << g_Vehicles.size() << std::endl;
				}
				break;
			}
			Logger::Msg("%d", g_Packet->data[0]);
		}
		g_RakPeer->DeallocatePacket(g_Packet);
	}
}

