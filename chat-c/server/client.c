#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX_LENGTH 1024

char mensagem_client(char source[], char buffer[], char p);

int main(){

  char *ip = "127.0.0.1";
  int port = 5566;

  int sock;
  struct sockaddr_in addr;
  socklen_t addr_size;
  char buffer[MAX_LENGTH], source[MAX_LENGTH];
  int n;


  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0){
    perror("[-]Socket error");
    exit(1);
  }
  printf("[+]TCP server socket created.\n");

  memset(&addr, '\0', sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = port;
  addr.sin_addr.s_addr = inet_addr(ip);

  connect(sock, (struct sockaddr*)&addr, sizeof(addr));
  printf("Connected to the server.\n");

  // bzero(buffer, 1024);
  // printf("Digite sua mensagem: ");
  // fgets(source, MAX_LENGTH, stdin);
  // strcpy(buffer, source);
  // printf("Client: %s\n", buffer);
  // send(sock, buffer, strlen(buffer), 0);

    mensagem_client(source)
  // bzero(buffer, MAX_LENGTH);
  // recv(sock, buffer, sizeof(buffer), 0);
  // printf("Server: %s\n", buffer);

  //close(sock);
  //printf("Disconnected from the server.\n");

  return 0;

}

char mensagem_client(char source[], char buffer[]){

    malloc
    bzero(buffer, MAX_LENGTH);
    bzero(source, MAX_LENGTH);
    printf("Digite sua mensagem: ");
    fgets(source, MAX_LENGTH, stdin);
    strcpy(buffer, source);
    printf("Client: %s\n", buffer);
    send(sock, buffer, strlen(buffer), 0);

    bzero(buffer, MAX_LENGTH);
    recv(sock, buffer, sizeof(buffer), 0);
    // printf("Server: %s\n", buffer);
    return buffer[];
}