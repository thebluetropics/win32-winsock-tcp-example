#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <ws2tcpip.h>
#include <winsock2.h>
#include <iostream>

int main() {
  WSADATA wsaData;
  int iResult;

  iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
  if (iResult != 0) {
    std::cout << "Err: WSAStartup failed." << std::endl;
    return 1;
  }

  addrinfo* result = NULL;
  addrinfo* ptr = NULL;
  addrinfo hints;

  ZeroMemory(&hints, sizeof(addrinfo));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;
  
  iResult = getaddrinfo("127.0.0.1", "3000", &hints, &result);
  if (iResult != 0) {
    std::cout << "Err: getaddrinfo failed." << std::endl;
    WSACleanup();
    return 1;
  }

  SOCKET clientSocket = INVALID_SOCKET;
  ptr = result;
  clientSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

  if (clientSocket == INVALID_SOCKET) {
    std::cout << "Err: socket creation failed." << std::endl;
    freeaddrinfo(result);
    WSACleanup();
    return 1;
  }

  iResult = connect(clientSocket, ptr->ai_addr, (int) ptr->ai_addrlen);
  if (iResult == SOCKET_ERROR) {
    closesocket(clientSocket);
    clientSocket = INVALID_SOCKET;
  }

  freeaddrinfo(result);

  if (clientSocket == INVALID_SOCKET) {
    std::cout << "Err: connect failed." << std::endl;
    WSACleanup();
    return 1;
  }
  
  return 0;
}
