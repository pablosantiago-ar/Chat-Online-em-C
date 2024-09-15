
#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <process.h> // Biblioteca da _beginthreadex() e _endthreadex()
#include <ws2tcpip.h> // Biblioteca TCP/IP
#include <conio.h>
#pragma comment(lib, "Ws2_32.lib")

#define PORT 8888
#define TAMANHO_BUFFER 1024
#define MAX_CLIENTES 10
#define HISTORICO_ARQUIVO "historico.txt" //cria o arquivo do historico

typedef struct {
    SOCKET socket;
    char nome[30];
} Cliente;

Cliente clientes[MAX_CLIENTES];
int num_clientes = 0;

// Função para enviar uma mensagem para todos os clientes, exceto o cliente excluído
void BroadcastMessage(const char* mensagem, SOCKET socket_excluir) {
    FILE *historico;

    //escreve no arquivo
    historico = fopen(HISTORICO_ARQUIVO, "a");
    if(historico != NULL){
        fprintf(historico, "%s", mensagem);
        fclose(historico);
    }

    // Envia a mensagem para todos os clientes conectados
    for (int i = 0; i < num_clientes; i++) {
        if (clientes[i].socket != socket_excluir) {
            send(clientes[i].socket, mensagem, strlen(mensagem), 0);
        }
    }
}

// Função para enviar o histórico de mensagens para um novo cliente
void EnviarHistorico(SOCKET sockfd){
    char linha[TAMANHO_BUFFER];
    FILE *historico = fopen(HISTORICO_ARQUIVO, "r");

    if(historico != NULL) {
        while(fgets(linha, sizeof(linha), historico) != NULL){
            send(sockfd, linha, strlen(linha), 0);
        }
        fclose(historico);
    }
}

// Função para apagar o arquivo de histórico
void ApagarHistorico() {
    if (remove(HISTORICO_ARQUIVO) == 0) {
        printf("Arquivo do historico foi apagado com sucesso.\n");
    } else {
        printf("Erro ao apagar o arquivo de histórico");
    }
}

// Função que será executada em uma thread para gerenciar cada cliente
unsigned __stdcall Servidor(void* arg) {
    char buffer[TAMANHO_BUFFER];
    SOCKET sockEntrada = *(SOCKET*)arg; // Cast do ponteiro para um ponteiro SOCKET
    int index_cliente = -1;

    // Receber nome do cliente
    int bytesReceived = recv(sockEntrada, buffer, sizeof(buffer) - 1, 0);
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0';

        // Adicionar cliente à lista
        for (int i = 0; i < MAX_CLIENTES; i++) {
            if (clientes[i].socket == 0) {
                clientes[i].socket = sockEntrada;
                strcpy(clientes[i].nome, buffer);
                num_clientes++;
                index_cliente = i;
                break;
            }
        }
        if (index_cliente == -1) {
            printf("Número máximo de clientes alcançado.\n");
            closesocket(sockEntrada);
            _endthreadex(0);
        }
    } else {
        closesocket(sockEntrada);
        _endthreadex(0);
    }

    printf("Cliente %s conectado...\n", clientes[index_cliente].nome);


    // Notificar outros clientes sobre a nova conexão
    char mensagem[TAMANHO_BUFFER];
    snprintf(mensagem, sizeof(mensagem), "O cliente %s foi conectado.\n", clientes[index_cliente].nome);
    BroadcastMessage(mensagem, sockEntrada);

    while (1) {
        bytesReceived = recv(sockEntrada, buffer, sizeof(buffer) - 1, 0);
        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';

            // Imprimir a mensagem recebida no terminal do servidor
            printf("Mensagem recebida de %s: %s\n", clientes[index_cliente].nome, buffer);

            if (strcmp(buffer, "quit") == 0) {
                // Remover cliente da lista
                closesocket(sockEntrada);
                num_clientes--;
                for (int i = index_cliente; i < num_clientes; i++) {
                    clientes[i] = clientes[i + 1];
                }
                clientes[num_clientes].socket = 0; // Limpar o slot final
                snprintf(mensagem, sizeof(mensagem), "O cliente %s desconectou.\n", clientes[index_cliente].nome);
                BroadcastMessage(mensagem, INVALID_SOCKET);
                _endthreadex(0);
            } else if (strcmp(buffer, "/APAGAR_HISTORICO") == 0) {
                // Comando para apagar histórico
                ApagarHistorico();
                snprintf(mensagem, sizeof(mensagem), "O historico foi apagado.\n");
                BroadcastMessage(mensagem, INVALID_SOCKET);
            } else {
                // Enviar mensagem para todos os clientes
                snprintf(mensagem, sizeof(mensagem), "%s: %s\n", clientes[index_cliente].nome, buffer);
                BroadcastMessage(mensagem, sockEntrada);
            }
        } else {
            // Conexão fechada
            closesocket(sockEntrada);
            num_clientes--;
            for (int i = index_cliente; i < num_clientes; i++) {
                clientes[i] = clientes[i + 1];
            }
            clientes[num_clientes].socket = 0; // Limpar o slot final
            snprintf(mensagem, sizeof(mensagem), "O cliente %s desconectou.\n", clientes[index_cliente].nome);
            BroadcastMessage(mensagem, INVALID_SOCKET);
            _endthreadex(0);
        }
    }
    return 0;
}

// Função para configurar o servidor
SOCKET configuracaoServidor() {
    SOCKET sockfd;
    struct sockaddr_in serverAddr;
    WSADATA wsaData;
    int result;

    result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        printf("WSAStartup failed: %d\n", result);
        exit(1);
    }

    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd == INVALID_SOCKET) {
        printf("Erro no Socket: %d\n", WSAGetLastError());
        WSACleanup();
        exit(1);
    }
    
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(PORT);

    if (bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        printf("Erro no Bind: %d\n", WSAGetLastError());
        closesocket(sockfd);
        WSACleanup();
        exit(1);
    }

    if (listen(sockfd, 5) == SOCKET_ERROR) {
        printf("Erro na escuta: %d\n", WSAGetLastError());
        closesocket(sockfd);
        WSACleanup();
        exit(1);
    }

    return sockfd;
}

int main() {
    char hostname[NI_MAXHOST];
    struct sockaddr_in clienteAddr;
    int i;
    SOCKET sockfd = configuracaoServidor();

    gethostname(hostname, NI_MAXHOST);
    printf("*************\nServidor: Meu nome: %s\n", hostname);
    PHOSTENT phe = gethostbyname(hostname);
    for (i = 0; phe->h_addr_list[i] != 0; ++i) {
        struct in_addr addr;
        memcpy(&addr, phe->h_addr_list[i], sizeof(struct in_addr));
        printf("Servidor: Meu endereco IP (%d): %s\n", i, inet_ntoa(addr));
    }

    while (1) {
        SOCKET clienteSockfd;
        unsigned int clntLen = sizeof(clienteAddr);
        HANDLE thread;

        clienteSockfd = accept(sockfd, (struct sockaddr*)&clienteAddr, &clntLen);
        if (clienteSockfd == INVALID_SOCKET) {
            printf("Erro no Accept: %d\n", WSAGetLastError());
            closesocket(sockfd);
            WSACleanup();
            exit(1);
        }

        // Inicializa a thread
        unsigned threadID;
        thread = (HANDLE)_beginthreadex(NULL, 0, Servidor, (void*)&clienteSockfd, 0, &threadID);
        if (thread == NULL) {
            printf("Erro na Thread: %d\n", GetLastError());
            closesocket(clienteSockfd);
            closesocket(sockfd);
            WSACleanup();
            exit(1);
        }
        CloseHandle(thread);
    }

    closesocket(sockfd);
    WSACleanup();
    return 0;
}
