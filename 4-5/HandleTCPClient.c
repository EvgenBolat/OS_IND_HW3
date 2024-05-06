#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for recv() and send() */
#include <unistd.h>     /* for close() */

#define RCVBUFSIZE 1   /* Size of receive buffer */

void DieWithError(char *errorMessage);  /* Error handling function */

void HandleTCPClient(int clntSocket, pid_t pid)
{
    char echoBuffer[RCVBUFSIZE]; /* Buffer for echo string */
      
      printf("Продавец %d: Сообщаю, что принял заказ\n", pid);
      echoBuffer[0] = '0';
    int a = send(clntSocket, echoBuffer, RCVBUFSIZE, 0);
      if (a != RCVBUFSIZE){
         printf("send() failed");
        return;
        }
      echoBuffer[0] = '1';
  printf("Продавец %d: Сообщаю, что провожу пробивание товара на кассе\n", pid);
      usleep(2000000);
      printf("Продавец %d: Сообщаю, что выполнил заказ\n", pid);
      if (send(clntSocket, echoBuffer, RCVBUFSIZE, 0) != RCVBUFSIZE)
        DieWithError("send() failed");
      printf("Продавец %d: Завершаю общение с данным покупателем\n", pid);
      close(clntSocket); /* Close client socket */
}

