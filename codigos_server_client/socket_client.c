// bibliotecas utilizadas:
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma comment(lib, "Ws2_32.lib")

// constante utilizada
#define TAMANHO_BUFFER 1024

void zerar_string(char string[]){
    // criar um indice para percorrer a string;
    int i;
    // criação de um ponteiro para alocar o endereço da string;
    char *p;
    // variável para armazenar a quantidade de espaços da string;
    int cont;
    // recebimento do endereço da string
    p = &string[0];
    // armazenar a quantidade de espaços da string;
    cont = strlen(string);

    for(i=0; i<cont; i++){
        // toda a string recebe espaço;
        p[i] = ' ';
    }
}

int clientconfig()
{
    // Descritor do cliente
    SOCKET clienteSockfd;
    // Estrutura do Cliente
    struct sockaddr_in serv_addr;
    WSADATA wsaData;
    int result;

    // Inicializa o Winsock
    result = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (result != 0) {
        printf("WSAStartup failed: %d\n", result);
        exit(1);
    }

    // Socket familia TCP declarado SOCK_STREAM e criado
    clienteSockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clienteSockfd == INVALID_SOCKET) {
        printf("Erro no Socket: %d\n", WSAGetLastError());
        WSACleanup();
        exit(1);
    }

    // Zera a estrutura
    memset(&serv_addr, 0, sizeof(serv_addr));
    // Seta a familia
    serv_addr.sin_family = AF_INET;
    // Define o IP (o localhost)
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    // Define a porta de conexao (deve corresponder à do servidor)
    serv_addr.sin_port = htons(8888);  

    // Faz a conexao com o servidor
    if (connect(clienteSockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR) {
        printf("Erro no Connect: %d\n", WSAGetLastError());
        closesocket(clienteSockfd);
        WSACleanup();
        exit(1);
    }

    return clienteSockfd;
}

void Cliente(SOCKET clienteSockfd)
{
    // Buffer de dados a ser mandado para o servidor
    char buffersv[TAMANHO_BUFFER];
    char msg[150];
    char inicio[] = "O cliente ";
    char nome[30];
    char fim[] = " foi conectado.";

    printf("Servidor: Digite seu nome.\n");
    gets(nome);
    printf("Servidor: Conectado ao servidor.\n");
    printf("Servidor: Digite \"quit\" quando quiser encerrar a conexao.\n");
    strcpy(msg, inicio);
    strcat(msg, nome);
    strcat(msg, fim);
    send(clienteSockfd, msg, strlen(msg), 0);

    while(1){
        printf("%s: ", nome);
        zerar_string(msg);
        strcpy(msg, nome);
        strcpy(msg, ": ");
        fgets(msg, TAMANHO_BUFFER, stdin);

        send(clienteSockfd, msg, strlen(msg), 0);
    }

    // Encerra o descritor
    closesocket(clienteSockfd);
    WSACleanup();
}

int main()
{
    SOCKET descritorCliente = clientconfig();
    // Chama função do cliente passando o descritor
    Cliente(descritorCliente);
    return 0;
}
