#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MAX_LENGTH 1024
#define MAX_CLIENTS 100

int soquetes_clientes[MAX_CLIENTS];
int quantidade_clientes = 0;
pthread_mutex_t mutex_soquetes_clientes = PTHREAD_MUTEX_INITIALIZER;

void enviar_mensagem_para_todos(const char *mensagem, int soquete_remetente) {
    pthread_mutex_lock(&mutex_soquetes_clientes);
    for (int i = 0; i < quantidade_clientes; i++) {
        if (soquetes_clientes[i] != soquete_remetente) {
            send(soquetes_clientes[i], mensagem, strlen(mensagem), 0);
        }
    }
    pthread_mutex_unlock(&mutex_soquetes_clientes);
}

void* tratar_cliente(void* soquete_cliente_ptr) {
    int soquete_cliente = *(int*)soquete_cliente_ptr;
    char buffer[MAX_LENGTH];
    int len;

    while (1) {
        bzero(buffer, MAX_LENGTH);
        len = recv(soquete_cliente, buffer, sizeof(buffer) - 1, 0);
        if (len <= 0) {
            if (len == 0) {
                printf("[+]Cliente desconectado.\n");
            } else {
                perror("[-]Erro ao receber");
            }
            break;
        }

        buffer[len] = '\0';
        printf("Mensagem recebida do cliente %d: %s\n", soquete_cliente, buffer);

        char mensagem[MAX_LENGTH + 50];
        snprintf(mensagem, sizeof(mensagem), "Cliente %d: %s", soquete_cliente, buffer);
        enviar_mensagem_para_todos(mensagem, soquete_cliente);
    }

    pthread_mutex_lock(&mutex_soquetes_clientes);
    for (int i = 0; i < quantidade_clientes; i++) {
        if (soquetes_clientes[i] == soquete_cliente) {
            soquetes_clientes[i] = soquetes_clientes[quantidade_clientes - 1];
            quantidade_clientes--;
            break;
        }
    }
    pthread_mutex_unlock(&mutex_soquetes_clientes);

    close(soquete_cliente);
    return NULL;
}

int main() {
    char *ip = "127.0.0.1";
    int porta = 5557;

    int soquete_servidor, soquete_cliente;
    struct sockaddr_in endereco_servidor, endereco_cliente;
    socklen_t tamanho_endereco;
    pthread_t id_thread;

    soquete_servidor = socket(AF_INET, SOCK_STREAM, 0);
    if (soquete_servidor < 0) {
        perror("[-]Erro ao criar socket");
        exit(1);
    }
    printf("[+]Socket TCP do servidor criado.\n");

    memset(&endereco_servidor, 0, sizeof(endereco_servidor));
    endereco_servidor.sin_family = AF_INET;
    endereco_servidor.sin_port = htons(porta);
    endereco_servidor.sin_addr.s_addr = inet_addr(ip);

    if (bind(soquete_servidor, (struct sockaddr*)&endereco_servidor, sizeof(endereco_servidor)) < 0) {
        perror("[-]Erro ao vincular");
        close(soquete_servidor);
        exit(1);
    }
    printf("[+]Vinculado ao número da porta: %d\n", porta);

    if (listen(soquete_servidor, 5) < 0) {
        perror("[-]Erro ao escutar");
        close(soquete_servidor);
        exit(1);
    }
    printf("Ouvindo...\n");

    while (1) {
        tamanho_endereco = sizeof(endereco_cliente);
        soquete_cliente = accept(soquete_servidor, (struct sockaddr*)&endereco_cliente, &tamanho_endereco);
        if (soquete_cliente < 0) {
            perror("[-]Erro ao aceitar");
            continue;
        }
        printf("[+]Cliente conectado.\n");

        pthread_mutex_lock(&mutex_soquetes_clientes);
        if (quantidade_clientes < MAX_CLIENTS) {
            soquetes_clientes[quantidade_clientes++] = soquete_cliente;
        } else {
            printf("[-]Número máximo de clientes atingido. Rejeitando novo cliente.\n");
            close(soquete_cliente);
        }
        pthread_mutex_unlock(&mutex_soquetes_clientes);

        if (pthread_create(&id_thread, NULL, tratar_cliente, (void*)&soquete_cliente) != 0) {
            perror("[-]Erro ao criar thread");
            close(soquete_cliente);
        }

        pthread_detach(id_thread);
    }

    close(soquete_servidor);
    return 0;
}
