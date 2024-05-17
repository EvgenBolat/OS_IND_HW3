#include <arpa/inet.h>  /* for sockaddr_in and inet_ntoa() */
#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for recv() and send() */
#include <unistd.h>     /* for close() */
#include <string.h>     /* for memset() */
#include <sys/socket.h> 

#define RCVBUFSIZE 1

void DieWithError(char *errorMessage);  /* Error handling function */
void makeServerLog(char *sending);


void HandleTCPClient(int clntSocket, pid_t pid, int lstnrPort)
{
    char echoBuffer[RCVBUFSIZE]; /* Buffer for echo string */
      makeServerLog("принял заказ");
      printf("Продавец %d: Сообщаю, что принял заказ\n", pid);
      echoBuffer[0] = '0';
    int a = send(clntSocket, echoBuffer, RCVBUFSIZE, 0);
      if (a != RCVBUFSIZE){
         printf("send() failed");
        return;
        }
      echoBuffer[0] = '1';
    makeServerLog("пробиваю товара");
  printf("Продавец %d: Сообщаю, что провожу пробивание товара на кассе\n", pid);
      usleep(2000000);
    makeServerLog("выполнил заказ");
      printf("Продавец %d: Сообщаю, что выполнил заказ\n", pid);
      if (send(clntSocket, echoBuffer, RCVBUFSIZE, 0) != RCVBUFSIZE)
        DieWithError("send() failed");
    makeServerLog("общение завершено");
      printf("Продавец %d: Завершаю общение с данным покупателем\n", pid);
      close(clntSocket); /* Close client socket */
}

