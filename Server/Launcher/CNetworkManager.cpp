#include "stdafx.h"

CNetworkManager::CNetworkManager()
{
	// Get RakPeerInterface
	g_RakPeer = RakPeerInterface::GetInstance();
	
	// Retrieve instances of RPC4, DirectoryDeltaTransfer and FileListTransfer
	g_RPC = RPC4::GetInstance();
	g_DirTransfer = DirectoryDeltaTransfer::GetInstance();
	g_FileTransfer = FileListTransfer::GetInstance();
	
	// Attach RPC4, DirectoryDeltaTransfer and FileListTransfer to RakPeerInterface
	g_RakPeer->AttachPlugin(g_RPC);
	g_RakPeer->AttachPlugin(g_DirTransfer);
	g_RakPeer->AttachPlugin(g_FileTransfer);

	// Set the right transfer method
	g_DirTransfer->SetFileListTransferPlugin(g_FileTransfer);

	// Set our transfer directory and shared folder
	g_DirTransfer->SetApplicationDirectory(".//");
	g_DirTransfer->AddUploadsFromSubdirectory("clientplugins");

	// RakPeerInterface Settings
	g_RakPeer->SetSplitMessageProgressInterval(100);

	std::cout << "[CNetworkManager] Constructed" << std::endl;
}


CNetworkManager::~CNetworkManager()
{
	// Stop RakNet, stops synchronization
	g_RakPeer->Shutdown(2000);

	// Detach RPC4, DirectoryDeltaTransfer and FileListTransfer from RakPeerInterface
	g_RakPeer->DetachPlugin(g_RPC);
	g_RakPeer->DetachPlugin(g_DirTransfer);
	g_RakPeer->DetachPlugin(g_FileTransfer);

	// Destroy RPC4, DirectoryDeltaTransfer and FileListTransfer
	RPC4::DestroyInstance(g_RPC);
	DirectoryDeltaTransfer::DestroyInstance(g_DirTransfer);
	FileListTransfer::DestroyInstance(g_FileTransfer);

	// Destroy RakPeerInterface
	RakPeerInterface::DestroyInstance(g_RakPeer);

	std::cout << "[CNetworkManager] Deconstructed" << std::endl;
}

bool CNetworkManager::Start()
{
	std::cout << "[CNetworkManager] Starting..." << std::endl;
	SocketDescriptor socketDescriptor;
	socketDescriptor.port = g_Config->GetPort();

	int Startup = g_RakPeer->Startup(g_Config->GetMaxPlayers(), &socketDescriptor, 1, 0);
	if (!Startup)
	{
		// Set all connection data after interface has started
		g_RakPeer->SetMaximumIncomingConnections(g_Config->GetMaxPlayers());
		g_RakPeer->SetIncomingPassword(g_Config->GetPassword().c_str(), sizeof(g_Config->GetPassword().c_str()));
		g_RakPeer->SetLimitIPConnectionFrequency(true);
		g_RakPeer->SetTimeoutTime(15000, UNASSIGNED_SYSTEM_ADDRESS);

		std::cout << "[CNetworkManager] Successfully started" << std::endl;
		std::cout << "[CNetworkManager] " << g_DirTransfer->GetNumberOfFilesForUpload() << " files suitable for upload to clients" << std::endl;

		// Send data to the masterlist, so others can see that the server is online.
		PulseMaster();
		return true;
	}
	std::cout << "[CNetworkManager] Startup error " << Startup << std::endl;
	return false;
}

void CNetworkManager::Pulse()
{
	Packet *g_Packet = NULL;
	while (g_Packet = g_RakPeer->Receive())
	{
		BitStream g_BitStream(g_Packet->data + 1, g_Packet->length + 1, false);
		switch (g_Packet->data[0])
		{
			case ID_NEW_INCOMING_CONNECTION:
			{
				// API::Network::OnPlayerConnecting Execute
				for (int i = 0; i < g_ApiModules.size(); i++)
				{
					void *Instance = g_ApiModules[i].GetInstance();
					if (!API::Network::OnPlayerConnecting(Instance, g_Packet->guid.ToString()))
					{
						g_RakPeer->CloseConnection(g_Packet->systemAddress, true);
						return;
					}
				}

				int playerID = -1;
				if (!g_Players.empty())
				{
					for (int i = 0; i < g_Players.size(); i++)
					{
						if (g_Players[i].GetPlayerID() == -1)
						{
							playerID = g_Players.size();
							CPlayerEntity newPlayer;
							newPlayer.Create("User", g_Packet->guid, g_Packet->systemAddress);
							newPlayer.SetPlayerID(playerID);
							g_Players[i] = newPlayer;
							g_Players[i].SetPlayerID(i);
							playerID = i;

							g_Entities[g_Players[i].GetEntity()].SetEntity(&g_Players[i]);
							break;
						}
					}
				}

				if (playerID == -1)
				{
					playerID = g_Players.size();
					CPlayerEntity newPlayer;
					newPlayer.Create("User", g_Packet->guid, g_Packet->systemAddress);
					newPlayer.SetPlayerID(playerID);
					g_Players.push_back(newPlayer);

					g_Entities[g_Players[playerID].GetEntity()].SetEntity(&g_Players[playerID]);
				}

				NetworkSync::SyncServerWorld(g_Packet->guid);
				
				// API::Network::OnPlayerConnected Execute
				for (int i = 0; i < g_ApiModules.size(); i++)
				{
					void *Instance = g_ApiModules[i].GetInstance();
					API::Network::OnPlayerConnected(Instance, g_Players[playerID].GetEntity(), g_Players[playerID].GetPlayerID());
				}
				
				PulseMaster();
				break;
			}
			case ID_DISCONNECTION_NOTIFICATION:
			{
				for (int i = 0; i < g_Players.size(); i++) {
					if (g_Players[i].GetGUID() == g_Packet->guid)
					{
						if (g_Players[i].GetVehicle() != -1)
						{
							for (int v = 0; v < g_Vehicles.size(); v++)
							{
								if (g_Players[i].GetVehicle() == g_Vehicles[i].GetId())
								{
									g_Vehicles[i].SetOccupant(g_Players[i].GetVehicleSeat(), -1);
								}

								if (g_Vehicles[i].GetAssignee() == g_Packet->guid)
								{
									ServerEntity::SetAssignee(g_Vehicles[i].GetId(), UNASSIGNED_RAKNET_GUID);
								}
							}
						}

						g_Players[i].Destroy();
						break;
					}
				}
				PulseMaster();
				break;
			}
			case ID_CONNECTION_LOST:
			{
				for (int i = 0; i < g_Players.size(); i++)
				{
					if (g_Players[i].GetGUID() == g_Packet->guid)
					{
						if (g_Players[i].GetVehicle() != -1)
						{
							for (int v = 0; v < g_Vehicles.size(); v++)
							{
								if (g_Players[i].GetVehicle() == g_Vehicles[i].GetId())
								{
									g_Vehicles[i].SetOccupant(g_Players[i].GetVehicleSeat(), -1);
								}

								if (g_Vehicles[i].GetAssignee() == g_Packet->guid)
								{
									ServerEntity::SetAssignee(g_Vehicles[i].GetId(), UNASSIGNED_RAKNET_GUID);
								}
							}
						}

						g_Players[i].Destroy();
					}
				}
				PulseMaster();
				break;
			}
			case ID_PACKET_PLAYER:
			{
				for (int i = 0; i < g_Players.size(); i++) {
					if (g_Players[i].GetGUID() == g_Packet->guid) {
						g_Players[i].Update(g_Packet);
					}
				}
				break;
			}
			case ID_PACKET_VEHICLE:
			{
				int t_Id;
				g_BitStream.Read(t_Id);

				for (int i = 0; i < g_Vehicles.size(); i++) {
					if (g_Vehicles[i].GetId() == t_Id) {
						g_Vehicles[i].Update(g_Packet);
					}
				}
				break;
			}
			std::cout << g_Packet->data[0] << std::endl;
		}
		g_RakPeer->DeallocatePacket(g_Packet);
	}
	
	if (clock() - p_LastMasterUpdate > (120 * 1000)) {
		PulseMaster();
		p_LastMasterUpdate = clock();
	}
}

void CNetworkManager::PulseMaster()
{
	if (g_Config->GetAnnounce()) {
		std::string playerList;

		if (!g_Players.empty()) {
			for (int p = 0; p < g_Players.size(); p++) {
				if (g_Players[p].GetEntity() != -1) {
					std::ostringstream oss;
					oss << "{\"id\":" << g_Players[p].GetPlayerID() << ",\"name\":\"" << g_Players[p].GetUsername() << "\"}";
					std::string player = oss.str();

					if (p < g_Players.size() - 1)
						player.push_back(',');

					playerList.append(player);
				}
			}
		}

		CURL *hnd = curl_easy_init();

		struct curl_slist *headers = NULL;
		char content[1024];

		std::sprintf(content, "Content: {\"port\":%d, \"name\":\"%s\", \"players\":{\"amount\":%d, \"max\":%d, \"list\":[%s]}}", g_Config->GetPort(), g_Config->GetServerName().c_str(), CPlayerEntity::Amount, g_Config->GetMaxPlayers(), playerList.c_str());
		headers = curl_slist_append(headers, "content-type: application/x-www-form-urlencoded");
		headers = curl_slist_append(headers, "cache-control: no-cache");
		headers = curl_slist_append(headers, content);
		headers = curl_slist_append(headers, "authorization: FiveMP Token 13478817f618329e");

		curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "POST");
		curl_easy_setopt(hnd, CURLOPT_URL, "http://176.31.142.113:7001/v2/servers");
		curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, headers);
		curl_easy_setopt(hnd, CURLOPT_NOBODY, 1);

		CURLcode ret = curl_easy_perform(hnd);
	}
}
