//****************************************************************************
// Comunicacao de Dados
// Professor: Gustavo Kunzel
// Servidor TCP simples 
// Implementa um servidor TCP que aguarda o recebimento de pacotes de 
// diferentes clientes em uma porta
// Nota: a biblioteca -lws2_32 deve ser incluida no linker para funcionar
//     no projeto do DEV já foi incluido atraves das opcoes de projeto
//****************************************************************************
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
// **************************************************************************
// definicoes iniciais
#define TAMANHO_BUFFER 1024          // tamanho do buffer de recepcao e envio
#define DEFAULT_PORT "8888"        // porta de comunicacao que sera usada
// **************************************************************************

// esvaziar a string
void zerar_string(char string[]){
    int i;
    // criar contador para receber o endereço de string
    char *p;
    // alocação do endereço
    p = &string[0];
    for(i=0; i<TAMANHO_BUFFER; i++){
        // esvaziar a string
        p[i] = '\0';
    }

}

int __cdecl main(void) {
    // variaveis
    WSADATA wsaData;   // variavel para o winsock                                                        
    int iResult;       // variavel de status
    // sockets para conexao com o servidor
    SOCKET ListenSocket = INVALID_SOCKET;
    SOCKET ClientSocket = INVALID_SOCKET;
    // estrutura de enderecos
    struct addrinfo *result = NULL;
    struct addrinfo hints;
    // buffers de recepcao e envio
    int iSendResult;
    char recvbuf[TAMANHO_BUFFER];
    int recvbuflen = TAMANHO_BUFFER;     // comprimento do buffer de recepcao  
      

    // inicializa o winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup falhou com o erro: %d\n", iResult);
        system("pause");
        return 1;
    }

     // pega informacoes da maquina
    char hostname[NI_MAXHOST];        // guarda nome da maquina
    gethostname(hostname,NI_MAXHOST); // pega nome da maquina
    printf("*************\nServidor: Meu nome: %s\n",hostname);

    // pega o ip da maquina e exibe
    PHOSTENT phe = gethostbyname(hostname);     
    int i;
    for (i = 0; phe->h_addr_list[i] != 0; ++i) {
        struct in_addr addr;
        memcpy(&addr, phe->h_addr_list[i], sizeof(struct in_addr));
        printf("Servidor: Meu endereco IP (%d): %s\n",i, inet_ntoa(addr));        
        
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve o endereco do servidor e a porta 
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if ( iResult != 0 ) {
        printf("getaddrinfo falhou com erro: %d\n", iResult);
        WSACleanup();
        system("pause");
        return 1;
    }

    // Cria um socket para conexao com o servidor
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        printf("socket falhou com erro: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        system("pause");
        return 1;
    }

    // Configura o socket TCP do servidor
    iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind falhou com erro: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        system("pause");
        return 1;
    }


    freeaddrinfo(result);

    // Escuta aguardando um cliente 
    printf("*************\nServidor: Estou aguardando um cliente na porta %s...\n", DEFAULT_PORT);
    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        printf("listen falhou com erro: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        system("pause");
        return 1;
    }

    // Aceita o socket com o cliente
    ClientSocket = accept(ListenSocket, NULL, NULL);
    if (ClientSocket == INVALID_SOCKET) {
        printf("accept falhou com erro: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        system("pause");
        return 1;
    }

    // printf("Cliente aceito: %s\n",inet_ntoa(addr));

    // No longer need server socket
    closesocket(ListenSocket);

    // *********************************************************************
    // Estabeleceu conexao com o cliente e pode trocar dados
    do {

        iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0) {
            printf("Mensagem recebida: \"%s\"\n", recvbuf);

            // esvaziar a string
            zerar_string(recvbuf);

            recvbuf[strcspn(recvbuf, "\n")] = 0;

            /*
            // Echo the buffer back to the sender
            iSendResult = send( ClientSocket, recvbuf, iResult, 0 );
            if (iSendResult == SOCKET_ERROR) {
                printf("send failed with error: %d\n", WSAGetLastError());
                closesocket(ClientSocket);
                WSACleanup();
                system("pause");
                return 1;
            }
            printf("Bytes sent: %d\n", iSendResult);
            */
        }
        else if (iResult == 0)
            printf("Fechando conexao...\n");
        else  {
            printf("recv falhou com o erro: %d\n", WSAGetLastError());
            closesocket(ClientSocket);
            WSACleanup();
            system("pause");
            return 1;
        }

    } while (iResult > 0);

    // *********************************************************************
    // Encerra a conexao
    iResult = shutdown(ClientSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ClientSocket);
        WSACleanup();
        system("pause");
        return 1;
    }

    // cleanup
    printf("Encerrando socket...");
    closesocket(ClientSocket);
    WSACleanup();

    system("pause");
    return 0;
}
