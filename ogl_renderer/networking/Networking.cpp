#include "Networking.h"

enum NetAuthority { Offline, Server, Client };

const PCWSTR IP = L"127.0.0.1";//Public IP of the server
//LocalHost : 127.0.0.1:8888
//My Pub Ip : 192.168.1.36:5555

const int PORT = 8888;//Port of the server
const int BUFFER_SIZE = 1024;

vector<SOCKET> clientArray;
vector<Transform*> syncedTransforms;
WSADATA wsaData;
SOCKET serverSocket;
SOCKET clientSocket;
sockaddr_in clientAddr;
sockaddr_in serverAddr;
NetAuthority netAuth;

//A thread is created running this function per socket connected
//This function recieves incoming packets from clients, and redistributes them to all other clients.
void Networking::ServerHandleClientLoop(SOCKET clientSocket) {
    SetColor(FOREGROUND_BLUE);
    cout << "Starting client loop #" << clientArray.size() << endl;

    while (1 > 0) {//Wait to recieve message
        char buffer[BUFFER_SIZE];
        int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        buffer[bytesRead] = '\0';
        SetColor(FOREGROUND_INTENSITY);
        cout << "Received | " << buffer << endl;//Log message
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
int Networking::ServerWaitForAccept() {//Continously wait for new connections
    int clientAddrLen = sizeof(clientAddr);
    clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientAddrLen);
    if (clientSocket == INVALID_SOCKET) {
        Log("Accept failed", FOREGROUND_RED);
        return 1;
    }
    Log(GetTimeAsString() + "New client connected", FOREGROUND_BLUE);
    clientArray.push_back(clientSocket);
    thread t = thread(ServerHandleClientLoop, ref(clientSocket));
    t.detach();
    return 0;
}
void Networking::ServerTick(){
    SendSyncedTransforms();
}
void Networking::SendSyncedTransforms() {
    string message = "";
    message += syncedTransforms.size();
    for (int i = 0; i < syncedTransforms.size(); i++) {
        Transform* transform = syncedTransforms[i];
        vec3 pos = transform->GetPosition();
        message += "X" + std::to_string(pos.x) + "Y" + std::to_string(pos.y) + "Z" + std::to_string(pos.z);

    }
    message += "|";
    const char* buff = message.c_str();
    for (int i = 0; i < clientArray.size(); i++) {
        send(clientArray[i], buff, sizeof(&buff), 0);
    } 
}
void Networking::SetSyncedTransforms(string message) {

    Log("Syncing " + to_string(message[0]) + " Transforms", FOREGROUND_INTENSITY);
    for (int i = 0; i < message[0]; i++) {
        int xStart = 0;
        int yStart = 0;
        int zStart = 0;
        int end = 0;
        for (int i = 0; i < message.length(); i++) {
            if (message[i] == 'X') { xStart = i; }
            else if (message[i] == 'Y') { yStart = i; }
            else if (message[i] == 'Z') { zStart = i; }
            else if (message[i] == '|') { end = i; }
        }
        vec3 vec;
        string x;
        for (int i = xStart; i < yStart; i++) {
            x[i - xStart] = message[i];
        }
        string y;
        for (int i = yStart; i < zStart; i++) {
            y[i - yStart] = message[i];
        }
        string z;
        for (int i = zStart; i < end; i++) {
            z[i - zStart] = message[i];
        }
        vec.x = stoi(x);
        vec.y = stoi(y);
        vec.z = stoi(z);
        Log("Synced Obj : " + to_string(message[0]) + " : ", FOREGROUND_INTENSITY);
        LogVec3(vec);
    }
}
void Networking::SetupServer() {


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
    serverAddr.sin_port = htons(PORT);

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
    Log("Server on Port ", FOREGROUND_GREEN);

    netAuth = NetAuthority::Server;
    //while (true) {//Server loop for listening for clients trying to join, then creating a thread to handle messsages recieved
    //    ServerTick(serverSocket);
    //}
    thread t = thread(ServerWaitForAccept);
    t.detach();
    
}
void Networking::CloseServer() {
    Log("Closing server", FOREGROUND_BLUE);

    closesocket(serverSocket);
    WSACleanup();
    netAuth = NetAuthority::Offline;
}

void Networking::ClientTick() {
    //string message;
    //cout << " : ";
    //getline(cin, message);//Read message
    //ClearLine(1);//Clear input, because we will also recieve the message we send and dont want to see it twice
    //message = GetTimeAsString() + " : " + message;//Final message
    //send(clientSocket, message.c_str(), message.length(), 0);//Send to server

    ////If we recieved, print to console
    //char recvBuffer[BUFFER_SIZE];
    //int bytesRead = recv(clientSocket, recvBuffer, sizeof(recvBuffer), 0);
    //if (bytesRead > 0) {
    //    recvBuffer[bytesRead] = '\0';
    //    cout << recvBuffer;
    //}
}
//Initialize a client
void Networking::SetupClient() {

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
    serverAddr.sin_port = htons(PORT);

    //Convert IP address to wide string and use InetPtonW
    if (InetPtonW(AF_INET, IP, &serverAddr.sin_addr) != 1) {
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
    
}
void Networking::CloseClient() {
    
    Log("Closing client", FOREGROUND_BLUE);
    closesocket(clientSocket);
    WSACleanup();
    netAuth = NetAuthority::Offline;
}

void Networking::NetworkingTick() {
    if (netAuth == Offline) { return; }
    if (netAuth == Server) {  }
    else if (netAuth == Client) { ClientTick(); }
}
