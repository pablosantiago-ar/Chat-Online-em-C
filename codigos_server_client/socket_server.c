#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <process.h> // Biblioteca da begin_threadex() e _endthreadex()
#include <ws2tcpip.h> // Funções do TCP/IP
#pragma comment(lib, "Ws2_32.lib") // Usei pra garantir que a Ws2_32.lib fosse linkada

#define PORT 8888
#define TAMANHO_BUFFER 1024

void* __cdecl Servidor(void* arg)
{
    // Armazena buffer do cliente
    char buffer_do_cliente[TAMANHO_BUFFER];
    // Cast do ponteiro para um ponteiro para SOCKET (SOCKET é um unsigned int específico pra manipular os sockets)
    SOCKET sockEntrada = *(SOCKET*) arg; 
    // Loop infinito
    printf("Cliente conectado...\n");
    for (;;)
    {
        int bytesReceived = recv(sockEntrada, buffer_do_cliente, sizeof(buffer_do_cliente) - 1, 0);
        if (bytesReceived > 0)
        {
            // Adiciona o terminador de string

            buffer_do_cliente[bytesReceived] = '\0';
            if (strcmp(buffer_do_cliente, "quit") != 0)
            {
                // Se buffer != "quit", imprime a string "O cliente x foi conectado" estabelecida no buffer do cliente
                printf("%s\n", buffer_do_cliente);
            }
            else
            {
                // Encerra o descritor
                closesocket(sockEntrada);
                // Encerra a thread
                _endthreadex(0);
            }
        }
        else
        {
            // Se recv retornar 0, a conexão foi fechada
            closesocket(sockEntrada);
            _endthreadex(0);
        }
    }
    return 0;
}

SOCKET configuracaoServidor()
{
    // Cria o descritor
    SOCKET sockfd;
    // Declaração da estrutura
    struct sockaddr_in serverAddr;
    WSADATA wsaData;
    int result;

    // Inicializa o Winsock
    result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        printf("WSAStartup failed: %d\n", result);
        exit(1);
    }
    // Cria o socket
    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd == INVALID_SOCKET)
    {
        printf("Erro no Socket: %d\n", WSAGetLastError());
        WSACleanup();
        exit(1);
    }

    // Zera a estrutura
    memset(&serverAddr, 0, sizeof(serverAddr));
    // Seta a família
    serverAddr.sin_family = AF_INET;
    // Seta os IPs (INADDR_ANY) e define a porta
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(PORT);

    // Binda o socket para um ip e uma porta
    if (bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
    {
        printf("Erro no Bind: %d\n", WSAGetLastError());
        closesocket(sockfd);
        WSACleanup();
        exit(1);
    }

    // Declaração do listen(escuta) para até 5 clientes
    if (listen(sockfd, 5) == SOCKET_ERROR)
    {
        printf("Erro na escuta: %d\n", WSAGetLastError());
        closesocket(sockfd);
        WSACleanup();
        exit(1);
    }

    return sockfd;
}

int main()
{
    // Guarda o nome da máquina
    char hostname[NI_MAXHOST];
    // Declaração da estrutura
    struct sockaddr_in clienteAddr;
    int i;
    // Retorna da função e o descritor
    SOCKET sockfd = configuracaoServidor();

    // Pega informacoes da maquina
    gethostname(hostname,NI_MAXHOST); 
    // Pega o nome da maquina
    printf("*************\nServidor: Meu nome: %s\n",hostname);
    // Pega o ip da maquina e exibe
    PHOSTENT phe = gethostbyname(hostname);     
    
    for (i = 0; phe->h_addr_list[i] != 0; ++i) {
        struct in_addr addr;
        memcpy(&addr, phe->h_addr_list[i], sizeof(struct in_addr));
        printf("Servidor: Meu endereco IP (%d): %s\n",i, inet_ntoa(addr));        
        
    }
    // Loop infinito
    while (1)
    {
        SOCKET clienteSockfd;
        // Tamanho da estrutura
        unsigned int clntLen = sizeof(clienteAddr);
        // Declara uma thread (HANDLE substitui o pthread)
        HANDLE thread; 
        // Fica no aguardo da conexão do cliente
        clienteSockfd = accept(sockfd, (struct sockaddr*)&clienteAddr, &clntLen);
        if (clienteSockfd == INVALID_SOCKET)
        {
            printf("Erro no Accept: %d\n", WSAGetLastError());
            closesocket(sockfd);
            WSACleanup();
            exit(1);
        }
        // Inicializa a thread
        unsigned threadID;
        thread = (HANDLE)_beginthreadex(NULL, 0, (unsigned (__stdcall*)(void*))Servidor, (void*)&clienteSockfd, 0, &threadID);
        if (thread == NULL)
        {
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
