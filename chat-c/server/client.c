#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MAX_LENGTH 1024

void exibir_menu_emoticons() {
    printf("\nSelecione um emoticon:\n");
    printf("1. :-) (Sorriso)\n");
    printf("2. :-( (Triste)\n");
    printf("3. ;-) (Piscada)\n");
    printf("4. :-D (Riso)\n");
    printf("5. :-O (Surpreso)\n");
    printf("6. :-P (Língua)\n");
    printf("7. Nenhum\n");
    printf("Escolha uma opção (1-7): ");
}

char* obter_emoticon(int escolha) {
    switch (escolha) {
        case 1: return ":-)";
        case 2: return ":-(";
        case 3: return ";-)";
        case 4: return ":-D";
        case 5: return ":-O";
        case 6: return ":-P";
        default: return "";
    }
}

void* receber_mensagens(void* sock_ptr) {
    int sock = *(int*)sock_ptr;
    char buffer[MAX_LENGTH];
    int n;

    while (1) {
        bzero(buffer, MAX_LENGTH);
        n = recv(sock, buffer, sizeof(buffer) - 1, 0);
        if (n <= 0) {
            printf("Servidor desconectado.\n");
            close(sock);
            exit(1);
        }

        buffer[n] = '\0';
        printf("\nServidor: %s\n", buffer);
        printf("Digite sua mensagem: "); // Reimprime o prompt após receber uma mensagem
        fflush(stdout);
    }
}

int main() {
    char *ip = "127.0.0.1";
    int porta = 5557;
    char nome[50];
    char buffer[MAX_LENGTH];
    int sock;
    struct sockaddr_in addr;
    int escolha_emoticon = 7; // Padrão para nenhum
    char *emoticon;
    pthread_t thread_receber;

    printf("Digite seu nome: ");
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = 0; // Remove o caractere de nova linha

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("[-]Erro ao criar socket");
        exit(1);
    }
    printf("[+]Socket TCP do cliente criado.\n");

    memset(&addr, '\0', sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(porta);
    addr.sin_addr.s_addr = inet_addr(ip);

    if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("[-]Erro ao conectar");
        close(sock);
        exit(1);
    }
    printf("Conectado ao servidor.\n");

    pthread_create(&thread_receber, NULL, receber_mensagens, (void*)&sock);

    while (1) {
        printf("Digite '/emoticons' para selecionar um emoticon, ou digite sua mensagem: ");
        fflush(stdout);

        fgets(buffer, MAX_LENGTH, stdin);
        buffer[strcspn(buffer, "\n")] = 0; // Remove o caractere de nova linha do fgets

        if (strcmp(buffer, "/emoticons") == 0) {
            exibir_menu_emoticons();
            scanf("%d", &escolha_emoticon);
            while (getchar() != '\n'); // Limpa o caractere de nova linha deixado pelo scanf

            emoticon = obter_emoticon(escolha_emoticon);
            printf("Emoticon selecionado: %s\n", emoticon);
        } else {
            char mensagem[MAX_LENGTH + 50];
            snprintf(mensagem, sizeof(mensagem), "%s: %s %s", nome, buffer, emoticon);

            printf("Você: %s\n", mensagem);
            send(sock, mensagem, strlen(mensagem), 0);

            bzero(buffer, MAX_LENGTH);
        }
    }

    close(sock);
    return 0;
}
