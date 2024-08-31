# Chat Server e Cliente em C

Este projeto inclui um servidor e um cliente de chat em C que permitem comunicação em tempo real entre múltiplos clientes. O servidor suporta vários clientes e pode retransmitir mensagens de um cliente para todos os outros conectados. O cliente pode enviar mensagens e utilizar emoticons.

## Requisitos

- Compilador C (como `gcc`)
- Biblioteca pthread (para threads)

## Estrutura do Projeto

- `server.c`: Código-fonte do servidor.
- `client.c`: Código-fonte do cliente.

## Compilação

Para compilar o servidor e o cliente, use os seguintes comandos:

```bash
gcc -o server server.c -lpthread
gcc -o client client.c -lpthread
```


# Chat Server e Cliente em C

Este projeto inclui um servidor e um cliente de chat em C que permitem comunicação em tempo real entre múltiplos clientes. O servidor suporta vários clientes e pode retransmitir mensagens de um cliente para todos os outros conectados. O cliente pode enviar mensagens e utilizar emoticons.

## Requisitos

- Compilador C (como `gcc`)
- Biblioteca pthread (para threads)

## Estrutura do Projeto

- `server.c`: Código-fonte do servidor.
- `client.c`: Código-fonte do cliente.

## Compilação

Para compilar o servidor e o cliente, use os seguintes comandos:

```bash
gcc -o server server.c -lpthread
gcc -o client client.c -lpthread
```

## Detalhes do Código
- `server.c`:
Função broadcast_message: Envia uma mensagem para todos os clientes conectados, exceto o remetente.
Função handle_client: Trata as mensagens enviadas por um cliente, repassando-as para todos os outros clientes conectados.
Multithreading: Utiliza threads para lidar com múltiplos clientes simultaneamente.
client.c
Função receive_messages: Recebe e exibe mensagens do servidor em um loop.
Seleção de Emoticons: Permite que o usuário escolha um emoticon e o anexe às suas mensagens.
Prompt de Mensagens: Exibe um prompt para que o usuário digite suas mensagens.
Problemas Conhecidos
O cliente deve digitar /emoticons para selecionar um emoticon, ou você pode incluir o emoticon diretamente na mensagem. Melhorias futuras podem incluir a seleção de emoticons diretamente na interface do cliente.
# Chat Server e Cliente em C

Este projeto inclui um servidor e um cliente de chat em C que permitem comunicação em tempo real entre múltiplos clientes. O servidor suporta vários clientes e pode retransmitir mensagens de um cliente para todos os outros conectados. O cliente pode enviar mensagens e utilizar emoticons.

## Requisitos

- Compilador C (como `gcc`)
- Biblioteca pthread (para threads)

## Estrutura do Projeto

- `server.c`: Código-fonte do servidor.
- `client.c`: Código-fonte do cliente.

## Compilação

Para compilar o servidor e o cliente, use os seguintes comandos:

```bash
gcc -o server server.c -lpthread
gcc -o client client.c -lpthread
```
## Detalhes do Código
Server.c
- `Função broadcast_message`: Envia uma mensagem para todos os clientes conectados, exceto o remetente.
- `Função handle_client`: Trata as mensagens enviadas por um cliente, repassando-as para todos os outros clientes conectados.
- `Multithreading`: Utiliza threads para lidar com múltiplos clientes simultaneamente.
Client.c
- `Função receive_messages`: Recebe e exibe mensagens do servidor em um loop.
- `Seleção de Emoticons`: Permite que o usuário escolha um emoticon e o anexe às suas mensagens.
- `Prompt de Mensagens`: Exibe um prompt para que o usuário digite suas mensagens.

## Autores

- [@mariafernandabq](https://www.github.com/mariafernandabq)

- [@pablosantiago-ar](https://www.github.com/pablosantiago-ar)

- [@diego-amvi](https://www.github.com/diego-amvi)

- [@JoaopedroQR](https://www.github.com/JoaopedroQR)

- [@tavinml](https://www.github.com/tavinml)

Projeto de um Chat Online em C para a disciplina de Introdução a programação, do S1 de Engenharia de Computação
