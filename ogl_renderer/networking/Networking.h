#pragma once
#define _CRT_SECURE_NO_WARNINGS //Allow old time functions
#include <iostream>
#include <fstream> //File stream
#include <sstream> //String stream for reading entire files
#include <../engine/Transform.h> //String stream for reading entire files
#include <../engine/Scene.h> //String stream for reading entire files
#include <string>
#include <vector>
#include <ctime> //For time functions
#include <winsock2.h> //Provides functions and definitions for network programming using Winsock
#include <ws2tcpip.h> //Contains additional functions for TCP/IP networking, for InetPtonW and other functions
#pragma comment(lib, "ws2_32.lib") //Links against the Winsock library
#include <thread>
#include "../engine/Diagnostics.h"
using namespace Diagnostics;
using namespace std;



namespace Networking
{
	void NetworkingTick();

	//Server
	void ServerHandleClientLoop(SOCKET clientSocket);
	int ServerWaitForAccept();
	void SetupServer();
	void CloseServer();
	void ServerTick();
	//Client
	void ClientTick();
	void SetupClient();
	void CloseClient();
	void SetSyncedTransforms(string message);
	void SendSyncedTransforms();
};
