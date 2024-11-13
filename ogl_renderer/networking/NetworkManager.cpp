#include "NetworkManager.h"

namespace NetworkManager
{
    //const PCWSTR IP = L"127.0.0.1";//Public IP of the server
    ////LocalHost : 127.0.0.1:8888
    ////My Pub Ip : 192.168.1.36:5555

    //const int PORT = 8888;//Port of the server
    //const int BUFFER_SIZE = 1024;

    vector<SOCKET> clientArray;
    Scene* syncedScene;
    WSADATA wsaData;
    SOCKET serverSocket;
    SOCKET clientSocket;
    sockaddr_in clientAddr;
    sockaddr_in serverAddr;
    NetAuthority netAuth;
    string incomingMessage="";

    //A thread is created running this function per socket connected
    //This function recieves incoming packets from clients, and redistributes them to all other clients.
    void NetworkManager::ServerClientListener() {
        //Log("ServerClientListener started", FOREGROUND_GREEN);
        
        Log("ServerClientListener started #" + to_string(clientArray.size()), FOREGROUND_GREEN);

        while (1 > 0) {//Wait to recieve message
            char buffer[1024];
            int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
            buffer[bytesRead] = '\0';
            Log("Received | " + *buffer, FOREGROUND_INTENSITY);
            //writeToSaveFile(buffer);
            // Broadcast the message to all other clients
            //for (int i = 0;i < clientArray.size(); i++) {
            //    SetColor(FOREGROUND_BLUE);
            //    cout << "Broadcasting message to client #" << i << endl;
            //    send(clientArray[i], buffer, bytesRead, 0);
            //}
        }
        cout << "ending client handler" << endl;
        // Remove client from list and close socket
        closesocket(clientSocket);
        clientArray.erase(remove(clientArray.begin(), clientArray.end(), clientSocket), clientArray.end());
    }


    int NetworkManager::ServerWaitNewClients() {//Continously wait for new connections
        Log("ServerWaitNewClients started", FOREGROUND_GREEN);
        int clientAddrLen = sizeof(clientAddr);
        clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientAddrLen);
        if (clientSocket == INVALID_SOCKET) {
            Log("Accept failed", FOREGROUND_RED);
            return 1;
        }
        Log("New client connected, starting ServerClientListener for them", FOREGROUND_BLUE);
        clientArray.push_back(clientSocket);
        thread t = thread(&NetworkManager::ServerClientListener);
        t.detach();
        return 0;
    }
    void NetworkManager::ServerTick() {
        Log("Starting ServerTick", FOREGROUND_GREEN);
        while (netAuth == NetAuthority::Server) {
            if (syncedScene != nullptr) {
                //Send out synced transforms
                string message = "t";
                
                for (int i = 0; i < syncedScene->gameObjects.size(); i++) {
                    Transform* transform = syncedScene->gameObjects[i]->transform;
                    vec3 pos = transform->GetGlobalPosition();
                    message += to_string(i);
                    message += "X";
                    message += std::to_string(pos.x);
                    message += "Y";
                    message += std::to_string(pos.y);
                    message += "Z";
                    message += std::to_string(pos.z);
                    message += "|";//END OF OBJ
                }
                message += "]";//END MESSAGE
                const char* buff = message.c_str();
                for (int i = 0; i < clientArray.size(); i++) {
                    send(clientArray[i], buff, sizeof(&buff), 0);
                }
                Log("[SERVERTICK THREAD] : Sending message : " + message, FOREGROUND_GREEN);
                //Output : "t1X1.00000Y1.00000Z1.00000"
            }
        }
        Log("Leaving out of ServerTick", FOREGROUND_RED);
    }

    void NetworkManager::EvaluateIncomingMessage() {

        int xStart = 0;
        int yStart = 0;
        int zStart = 0;
        int end = 0;

        for (int i = 0; i < incomingMessage.length(); i++) {
            if (incomingMessage[i] == 'X') { xStart = i; }
            else if (incomingMessage[i] == 'Y') { yStart = i; }
            else if (incomingMessage[i] == 'Z') { zStart = i; }
            else if (incomingMessage[i] == '|') { end = i; }
        }
        if (xStart == 0 || yStart == 0 || zStart == 0 || end == 0) {
            Log("IncomingMessage did not meet reqs", FOREGROUND_RED);
            return;
        }
        else { Log("Found X at " + to_string(xStart) + "Found Y at " + to_string(yStart) + "Found Z at " + to_string(zStart)); }
        vec3 vec;
        string x;
        for (int i = xStart; i < yStart; i++) {
            x[i - xStart] = incomingMessage[i];
        }
        string y;
        for (int i = yStart; i < zStart; i++) {
            y[i - yStart] = incomingMessage[i];
        }
        string z;
        for (int i = zStart; i < end; i++) {
            z[i - zStart] = incomingMessage[i];
        }
        vec.x = stoi(x);
        vec.y = stoi(y);
        vec.z = stoi(z);
        Log("Recieved vector : ", FOREGROUND_INTENSITY);
        LogVec3(vec);
    }
    void NetworkManager::SetupServer() {


        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            Log("WSA Startup failed", FOREGROUND_RED);
            return;
        }

        serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (serverSocket == INVALID_SOCKET) {
            Log("Socket creation failed", FOREGROUND_RED);
            WSACleanup();
            return;
        }

        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = INADDR_ANY;
        serverAddr.sin_port = htons(8888);

        if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
            Log("Bind failed", FOREGROUND_RED);

            closesocket(serverSocket);
            WSACleanup();
            return;
        }

        if (listen(serverSocket, 5) == SOCKET_ERROR) {
            Log("Listen failed", FOREGROUND_RED);
            closesocket(serverSocket);
            WSACleanup();
            return;
        }
        Log("Server started, netAuth set to Server", FOREGROUND_GREEN);

        netAuth = NetAuthority::Server;
        //while (true) {//Server loop for listening for clients trying to join, then creating a thread to handle messsages recieved
        //    ServerTick(serverSocket);
        //}
        thread(&NetworkManager::ServerWaitNewClients).detach();//Thread for accepting new clients
        thread(&NetworkManager::ServerTick).detach();//Thread for sending out server info
        

    }
    void NetworkManager::CloseServer() {
        Log("Closing server", FOREGROUND_BLUE);

        closesocket(serverSocket);
        WSACleanup();
        netAuth = NetAuthority::Offline;
    }

    void NetworkManager::ClientTick() {
        while (netAuth == NetAuthority::Client) {
            //Send stuff
        //send(clientSocket, message.c_str(), message.length(), 0);//Send to server

        //Recieve stuff
        //If we recieved, print to console
            char recvBuffer[1024];
            int bytesRead = recv(clientSocket, recvBuffer, sizeof(recvBuffer), 0);
            if (bytesRead > 0) {
                recvBuffer[bytesRead] = '\0';
                incomingMessage += string(recvBuffer);
                Log("[CLIENTTICK THREAD] : Received : " + string(recvBuffer), FOREGROUND_GREEN);
            }
        }
        Log("Breaking out of client tick", FOREGROUND_RED);
    }
    //Initialize a client
    void NetworkManager::SetupClient() {

        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            Log("WSAStartup failed", FOREGROUND_RED);
            return;
        }

        clientSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (clientSocket == INVALID_SOCKET) {
            Log("Socket creation failed", FOREGROUND_RED);
            WSACleanup();
            return;
        }

        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(8888);

        //Convert IP address to wide string and use InetPtonW
        if (InetPtonW(AF_INET, (PCWSTR)L"127.0.0.1", &serverAddr.sin_addr) != 1) {
            Log("InetPtonW failed", FOREGROUND_RED);
            closesocket(clientSocket);
            WSACleanup();
            return;
        }

        if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
            Log("Connect failed", FOREGROUND_RED);
            closesocket(clientSocket);
            WSACleanup();
            return;
        }


        string joinedMessage = "Player connected to the server.";
        //string joinedMessage = (getTime() + username + " connected to the server.");
        send(clientSocket, joinedMessage.c_str(), joinedMessage.length(), 0);
        netAuth = NetAuthority::Client;
        //while (true) {//Client loop
        //    ClientLoop(clientSocket);
        //}
        thread t = thread(&NetworkManager::ClientTick);
        t.detach();

    }
    void NetworkManager::CloseClient() {

        Log("Closing client", FOREGROUND_BLUE);
        closesocket(clientSocket);
        WSACleanup();
        netAuth = NetAuthority::Offline;
    }

    //void NetworkManager::NetworkingTick() {
    //    //if (netAuth == Offline || syncedScene == nullptr) { return; }
    //    //if (netAuth == Server) { ServerTick(); }
    //    //else if (netAuth == Client) {}
    //}
}