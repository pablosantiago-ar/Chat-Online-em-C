#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
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

// Função de exibição do menu
int menu(){
    int op;

    printf("BEM-VINDO A TELA INICIAL!\n\n");
    printf("CHAT ONLINE EM C\n");
    printf("PROFESSOR: Lucas Silva\n");
    printf("AUTORES: Diego, Gustavo, Joao Pedro, Maria Fernanda, Pablo, Pedro Paulo, Rafael\n\n\n");

    printf("O que voce deseja fazer?\n\n");
    printf("1- Entrar no chat\n");
    printf("2- Acessar o menu de comandos\n");
    
    scanf("%d", &op);
    return op;
}

// Função para limpar o terminal.
void limpar_tela(){
    system("cls");
}

// Função para imprimir o emoji unicode
void imprimeicone(char* n){
	char str[1000];
    sprintf(str,"powershell.exe -Command \"[System.Char]::ConvertFromUtf32([System.Convert]::toInt32('%s',16))\"", n);
	system(str);
}

// Menu que exibe as opções de emojis
void exibir_menu_emoticons() {
    char str[10];
    strcpy(str, "1F600");
    printf("1.");
    imprimeicone(str);
    printf("\n");
    zerar_string(str);
    strcpy(str, "1F614");
    printf("2.");
    imprimeicone(str);
    printf("\n");
    zerar_string(str);
    strcpy(str, "1F609");
    printf("3.");
	imprimeicone(str);
    printf("\n");
    zerar_string(str);    
    strcpy(str, "1F601");
    printf("4.");
	imprimeicone(str);
    printf("\n");
    zerar_string(str);
    strcpy(str, "1F636");
    printf("5.");
	imprimeicone(str);
    printf("\n");
    zerar_string(str);
    strcpy(str, "1F61B");
    printf("6.");
	imprimeicone(str);
    printf("\n");
    zerar_string(str);
    printf("Escolha um emoji: ");
}

// Retorna no chat o emoji escolhido
char* obter_emoticon(int escolha) {
    switch (escolha) {
        case 1: return ":-)";
        case 2: return ":-(";
        case 3: return ";-)";
        case 4: return ":-D";
        case 5: return ":-O";
        case 6: return ":-P";
        default: return "Nao achei um emoji :(";
    }
}

// Menu dos comandos do chat;
int menu_comandos(){
    printf("##### BEM-VINDO AO MENU DE COMANDOS #####\n\n");
    printf("Use o \"/APAGAR_HISTORICO\" para limpar o historico de mensagens\n");
    printf("Use o \"/emoticons\" para mandar algum emoji\n");
    printf("Use o \"/quit\" para voltar ao menu principal\n");
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
    char *emoticon = "";
    int escolha_emoticon;
    int op;

    while(1){
        limpar_tela();
        // exibe o menu inicial;
        op = menu();

        if(op==1){
            limpar_tela();
            // Receber e enviar o nome do cliente
            getchar();
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

                if (strcmp(msg, "/APAGAR_HISTORICO") == 0) {
                    // Enviar comando para apagar o histórico
                    send(clienteSockfd, msg, strlen(msg), 0);

                } else if (strcmp(msg, "/quit") == 0) {
                    // Enviar comando de desconexão
                    strcpy(msg, "O usuario saiu da conversa."); 
                    send(clienteSockfd, msg, strlen(msg), 0);
                    break;

                } else if(strcmp(msg, "/emoticons") == 0){
                    exibir_menu_emoticons();
                    scanf("%d", &escolha_emoticon);
                    getchar();
                    emoticon = obter_emoticon(escolha_emoticon);
                    printf("%s: %s\n",nome, emoticon);

                    send(clienteSockfd, emoticon, strlen(emoticon), 0);

                } else {
                    // Enviar mensagem normal
                    send(clienteSockfd, msg, strlen(msg), 0);
                }
            }
        } else if (op==2){
                limpar_tela();
                menu_comandos();
                printf("\nDigite 0 para sair\n");
                int a = scanf("%d", &a);

        } else{
            limpar_tela();
            printf("Comando inválido :(");
        }
    }
    // Encerra o socket
    closesocket(clienteSockfd);
    WSACleanup();
}

int main() {

    SOCKET clienteSockfd = clientconfig();
    Cliente(clienteSockfd);
    return 0;
}
