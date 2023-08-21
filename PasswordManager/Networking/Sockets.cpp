#include "pch.h"
#pragma comment(lib, "ws2_32.lib")
#include "Client.h"
#include "Sockets.h"
sockaddr_in HInt;
SOCKET Sock = socket(AF_INET, SOCK_STREAM, 0);
void CreateSockets()
{
    WSADATA data;
    WORD ver = MAKEWORD(2, 2);
    int wsResult = WSAStartup(ver, &data);
    if (wsResult != 0)
    {
        return;
    }

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET)
    {
        WSACleanup();
        return;
    }

    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(51000);
    inet_pton(AF_INET, "127.0.0.1", &hint.sin_addr);
    bind(sock, (sockaddr*)&hint, sizeof(hint));
    listen(sock, SOMAXCONN);
    HInt = hint;
    Sock = sock;
    std::thread acceptclientsthread(AcceptClients);
    acceptclientsthread.join();
}
void AcceptClients()
{
    while (true)
    {
        int size = sizeof(HInt);
        SOCKET socket;
        if ((socket = accept(Sock, (SOCKADDR*)&HInt, &size)) != INVALID_SOCKET) 
        {
            Client* client = new Client(socket);

        }
    }
}