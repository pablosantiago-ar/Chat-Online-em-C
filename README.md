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
- `EnviarHistorico`: Função para enviar o histórico de mensagens para um novo cliente.
- `ApagarHistorico`: Função para apagar o arquivo do histórico.

Client.c
- `Função receive_messages`: Recebe e exibe mensagens do servidor em um loop.
- `Seleção de Emoticons`: Permite que o usuário escolha um emoticon e o anexe às suas mensagens. Versão atualizada na qual estão disponíveis emoticons Unicode 😊.
- `Prompt de Mensagens`: Exibe um prompt para que o usuário digite suas mensagens.
- `Zerar Strings`: Função para zerar uma string, substituindo caracteres por espaços.
- `Menu`: Função que exibe um menu interativo, no qual permiete acessar o chat ou ver os comando disponíveis no código.

## Comandos Especiais
- `/APAGAR_HISTORICO`: Envia comando para apagar o histórico do Chat.
- `/quit`: Envia comando para o usuário sair da conversa.
- `/emoticons`: Envia comando o usuário selecionar um emoticon para adicionar ao texto.

## Autores

- [Maria Fernanda](https://www.github.com/mariafernandabq)

- [Pablo Santiago](https://www.github.com/pablosantiago-ar)

- [Diego Amorim](https://www.github.com/diego-amvi)

- [João Pedro Queiroz](https://www.github.com/JoaopedroQR)

- [Gustavo Monteiro](https://www.github.com/tavinml)

- [Pedro Paulo Teixeira](https://github.com/pedrolvrx86)

- Rafael Soares

Projeto de um Chat Online em C para a disciplina de Introdução a programação, do S1 de Engenharia de Computação
