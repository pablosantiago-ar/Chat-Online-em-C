#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma comment(lib, "Ws2_32.lib")

#define TAMANHO_BUFFER 1024

// Função para zerar uma string, substituindo caracteres por espaços
void zerar_string(char string[]) {
    int i;
    int cont = strlen(string);
    for (i = 0; i < cont; i++) {
        string[i] = ' ';
    }
}

// Função para configurar a conexão do cliente
SOCKET clientconfig() {
    SOCKET clienteSockfd;
    struct sockaddr_in serv_addr;
    WSADATA wsaData;
    int result;

    // Inicializa Winsock
    result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        printf("WSAStartup failed: %d\n", result);
        exit(1);
    }

    // Cria o socket do cliente
    clienteSockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clienteSockfd == INVALID_SOCKET) {
        printf("Erro no Socket: %d\n", WSAGetLastError());
        WSACleanup();
        exit(1);
    }
    
    // Configura a estrutura do endereço do servidor
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // IP do servidor (localhost)
    serv_addr.sin_port = htons(8888); // Porta do servidor

    // Conecta ao servidor
    if (connect(clienteSockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR) {
        printf("Erro no Connect: %d\n", WSAGetLastError());
        closesocket(clienteSockfd);
        WSACleanup();
        exit(1);
    }

    return clienteSockfd;
}

// Função principal do cliente
void Cliente(SOCKET clienteSockfd) {
    char msg[TAMANHO_BUFFER];
    char nome[30];
    
    // Receber e enviar o nome do cliente
    printf("Digite seu nome: ");
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = '\0';  // Remove a nova linha no final

    // Enviar nome para o servidor
    send(clienteSockfd, nome, strlen(nome), 0);

    while (1) {
        // Receber e exibir mensagens do servidor
        int bytesReceived = recv(clienteSockfd, msg, sizeof(msg) - 1, 0);
        if (bytesReceived > 0) {
            msg[bytesReceived] = '\0';
            printf("%s", msg);
        }

        // Enviar mensagens para o servidor
        printf("%s: ", nome);
        zerar_string(msg);
        fgets(msg, sizeof(msg), stdin);
        msg[strcspn(msg, "\n")] = '\0';  // Remove a nova linha no final

        if (strcmp(msg, "APAGAR_HISTORICO") == 0) {
            // Enviar comando para apagar o histórico
            send(clienteSockfd, msg, strlen(msg), 0);
        } else if (strcmp(msg, "quit") == 0) {
            // Enviar comando de desconexão
            send(clienteSockfd, msg, strlen(msg), 0);
            break;
        } else {
            // Enviar mensagem normal
            send(clienteSockfd, msg, strlen(msg), 0);
        }
    }
    
    // Encerra o socket
    closesocket(clienteSockfd);
    WSACleanup();
}

int main() {
    // Chama a função do cliente passando o descritor
    SOCKET clienteSockfd = clientconfig();
    Cliente(clienteSockfd);
    return 0;
}
