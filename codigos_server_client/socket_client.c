//****************************************************************************
// Comunicacao de Dados
// Professor: Gustavo Kunzel
// Cliente TCP simples 
// Implementa um cliente TCP que envia pacotes para servidores
// diferentes em uma porta
// Nota: a biblioteca -lws2_32 deve ser incluida no linker para funcionar
//     no projeto do DEV já foi incluido atraves das opcoes de projeto
//****************************************************************************

// implementa um cliente
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
// **************************************************************************
// definicoes iniciais
#define TAMANHO_BUFFER 512          // tamanho do buffer de recepcao
#define DEFAULT_PORT "8888"        // porta de comunicacao
#define IP_SERVIDOR "127.0.0.1"     // ip do servidor
// **************************************************************************

int main(int argc, char **argv) 
{
    // variaveis
    WSADATA wsaData;                  // variavel para o winsock 
    int iResult;                      // variavel de status
    // sockets para conexao com o servidor
    SOCKET ConnectSocket = INVALID_SOCKET;    
    // estrutura de enderecos
    struct addrinfo *result = NULL,
                    *ptr = NULL,
                    hints;    
    // buffers de recepcao e envio                   
    char sendbuf[TAMANHO_BUFFER];     
    char recvbuf[TAMANHO_BUFFER];     
    int recvbuflen = TAMANHO_BUFFER;  // comprimento do buffer de recepcao    

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
    printf("*************\nCliente: Meu nome: %s\n",hostname);

    // pega o ip da maquina e exibe
    PHOSTENT phe = gethostbyname(hostname);     
    int i;
    for (i = 0; phe->h_addr_list[i] != 0; ++i) {
        struct in_addr addr;
        memcpy(&addr, phe->h_addr_list[i], sizeof(struct in_addr));
        printf("Cliente: Meu endereco IP (%d): %s\n",i,inet_ntoa(addr));        
        
    }

    // inicializa o tipo de conexao
    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;    // datagramas TCP
    hints.ai_protocol = IPPROTO_TCP;

    // resolve o endereco e porta do servidor
    iResult = getaddrinfo(IP_SERVIDOR, DEFAULT_PORT, &hints, &result);
    if ( iResult != 0 ) {
        printf("getaddrinfo falhou com o erro: %d\n", iResult);
        WSACleanup();
        system("pause");
        return 1;
    }

    // tenta conectar ao endereco do servidor
    printf("*************\nTentando se conectar ao servidor de endereco %s na porta %s...\n", IP_SERVIDOR, DEFAULT_PORT);
    for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {

        // cria um socket para se conectar ao servidor
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            printf("Socket falhou com o erro: %ld\n", WSAGetLastError());
            WSACleanup();
            system("pause");
            return 1;
        }

        // tenta conecta ao servidor
        iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        printf("Nao foi possivel se conectar ao servidor!\n");
        system("pause");
        WSACleanup();
        return 1;
    }    

    // *********************************************************************
    // Estabeleceu conexao com o servidor e pode trocar dados

    i = 0;
    while(!kbhit()) {
        printf("Enviando \"Sou o cliente %s e envio o recado no. %04d\"...\n",hostname,i);                     

        // monta msg com um numero e o nome da maquina em forma de string
        sprintf(sendbuf,"Sou o cliente %s e envio o recado no. %04d",hostname,i);
        // envia a mensagem
        iResult = send( ConnectSocket, sendbuf, (int)strlen(sendbuf), 0 );
        if (iResult == SOCKET_ERROR) {
            printf("Falhou o envio com o erro: %d\n", WSAGetLastError());
            closesocket(ConnectSocket);
            WSACleanup();
            system("pause");
            return 1;
        }

        i ++;
        Sleep(1000);

    }

    // *********************************************************************
    // Encerra a conexao
    iResult = shutdown(ConnectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("Falhou encerrar a conexao com o erro: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        system("pause");
        return 1;
    }

     // Receive until the peer closes the connection
 /*   do {

        iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
        if ( iResult > 0 )
            printf("Bytes received: %d\n", iResult);
        else if ( iResult == 0 )
            printf("Connection closed\n");
        else
            printf("recv failed with error: %d\n", WSAGetLastError());

    } while( iResult > 0 );

*/

    // cleanup
    printf("Encerrando socket...");
    closesocket(ConnectSocket);
    WSACleanup();

    system("pause");
    return 0;
}
