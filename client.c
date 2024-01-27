#include <stdio.h>
#include <winsock2.h>

// Definiranje na port i adresa na koja treba da se povrzi
#define PORT 8080
#define SERVER_ADDR "127.0.0.1"

int main() {
    // winsock biblioteka
    WSADATA wsa;
    // Mesto kaj sho ke se chuvaat informacii za socketot
    SOCKET client_sock;
    // Adresa na serverot
    struct sockaddr_in server;
    // Poraka koja ke se pushta do serverot
    char message[] = "Hello from Mihail Siljanovski INKI745";
    // poraki koi ke se primat od serverot
    char buffer[1024] = {0};
    // goleminata na podatoko
    int recv_size;

    // Inicijaliziranje na Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("WSAStartup failed. Error Code : %d", WSAGetLastError());
        return 1;
    }

    // Kreiranje na socket na klientot za komunikacija
    if ((client_sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Could not create socket : %d", WSAGetLastError());
        return 1;
    }

    // Struktura na serverot: adresa, port 
    server.sin_addr.s_addr = inet_addr(SERVER_ADDR);
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);

    // Konektiranje do serverot
    if (connect(client_sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        printf("Connection failed");
        return 1;
    }
    printf("Connected to server\n");

    // Pushtanje podatoci
    send(client_sock, message, strlen(message), 0);

    // Primanje podatoci
    recv_size = recv(client_sock, buffer, sizeof(buffer), 0);
    printf("Server response: %s\n", buffer);

    // Zatvoranje
    closesocket(client_sock);
    WSACleanup();
    return 0;
}
