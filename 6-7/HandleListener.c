#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for recv() and send() */
#include <unistd.h>     /* for close() */

#define LstnBufSize 80   /* Size of receive buffer */

void DieWithError(char *errorMessage);  /* Error handling function */

void HandleListener(int sock)
{
  int bytesRcvd;
  char lstnBuffer[LstnBufSize]; /* Buffer for echo string */
    if ((bytesRcvd = recv(sock, lstnBuffer, LstnBufSize, MSG_WAITALL)) < 0){
      printf("error");
      return;
    }
    printf("%s", lstnBuffer);
    close(sock); /* Close client socket */
}

