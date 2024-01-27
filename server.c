#include <stdio.h>
#include <winsock2.h>

// Definirame Port na koja serverot ke "slusha"
#define PORT 8080

int main() {
    WSADATA wsa;
    SOCKET server_sock, client_sock;
    struct sockaddr_in server, client;
    int recv_size;
    char buffer[1024] = {0};
    
    // Inicijalizirame Winsock, socket za windows operativen sistem
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("WSAStartup failed. Error Code : %d", WSAGetLastError());
        return 1;
    }

    // Creairame socket
    if ((server_sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        // Ako ne uspej da se kreira
        printf("Could not create socket : %d", WSAGetLastError());
        return 1;
    }

    // IPv4 protokol
    server.sin_family = AF_INET;
    // ja pretstavuva ip adresata na serverot, koga ke se povrzi "slusha" za site dostapni mrezi
    server.sin_addr.s_addr = INADDR_ANY;
    // ja pretvora portata vo network byte na koj shto ke "slusha"
    server.sin_port = htons(PORT);

    // Povrzuvanje na socket so odredena ip adresa na koj serverot ke "slusha"
    if (bind(server_sock, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR) {
        printf("Bind failed with error code : %d", WSAGetLastError());
        return 1;
    }

    // Pochnuva da "slusha" za konekcija 
    listen(server_sock, 3);
    printf("Server listening on port %d...\n", PORT);

    // Prifakja konekcija
    int c = sizeof(struct sockaddr_in);
    // pointer do lokacijata na adresata na klientot
    client_sock = accept(server_sock, (struct sockaddr *)&client, &c);
    // Ako ne uspee konekcijata
    if (client_sock == INVALID_SOCKET) {
        printf("Accept failed with error code : %d", WSAGetLastError());
        return 1;
    }

    // Prima podatoci i vraka podatoci
    while ((recv_size = recv(client_sock, buffer, sizeof(buffer), 0)) > 0) {
        printf("Client sent: %s\n", buffer);
        send(client_sock, "Hello from the server!", strlen("Hello from the server!"), 0);
        memset(buffer, 0, sizeof(buffer));
    }

    // Ako se pojavi greshka pri primanje podatok
    if (recv_size == SOCKET_ERROR) {
        printf("Receive failed with error code : %d", WSAGetLastError());
    }

    // Zatvoranje
    closesocket(server_sock);
    closesocket(client_sock);
    WSACleanup();
    return 0;
}
