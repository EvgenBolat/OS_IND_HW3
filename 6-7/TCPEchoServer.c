#include <arpa/inet.h>  /* for sockaddr_in and inet_ntoa() */
#include <stdio.h>      /* for printf() and fprintf() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <sys/socket.h> /* for socket(), bind(), and connect() */
#include <unistd.h>     /* for close() */

#define MAXPENDING 10 /* Maximum outstanding connection requests */

void DieWithError(char *errorMessage); /* Error handling function */
void HandleTCPClient(int clntSocket, pid_t pid,
                     int lstnrPort); /* TCP client handling function */

#define RCVBUFLSTNSIZE 80
struct sockaddr_in echoListnerServAddr;
char lstnerBuffer[RCVBUFLSTNSIZE];
int listinerServPort;

int lstnsock;

void makeServerLog(char *sending) {
  if ((lstnsock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
    printf("socket() failed");
    exit(0);
  }
  int curr = 0;
  char my_pid[6];
  sprintf(my_pid, "%d", getpid());
  char *seller_word = "Продавец";
  for (; curr < strlen(seller_word); curr++) {
    lstnerBuffer[curr] = seller_word[curr];
  }
  lstnerBuffer[curr++] = ' ';
  for (int j = 0;  j < strlen(my_pid); j++) {
    lstnerBuffer[curr++] = my_pid[j];
  }
  lstnerBuffer[curr++] = ':';
  lstnerBuffer[curr++] = ' ';
  char buffer[RCVBUFLSTNSIZE];
  sprintf(buffer, "%s", sending);
  for (int j = 0; j < strlen(buffer); j++) {
    lstnerBuffer[curr++] = buffer[j];
  }
  lstnerBuffer[curr] = '\0';
  if (connect(lstnsock, (struct sockaddr *)&echoListnerServAddr,
              sizeof(echoListnerServAddr)) < 0) {
    printf("connect() failed c listner");
  }
  if (send(lstnsock, lstnerBuffer, RCVBUFLSTNSIZE, 0) != RCVBUFLSTNSIZE)
  {
    printf("something goes wrong");
  }
  //   DieWithError("send() failed");
  close(lstnsock);
}

int main(int argc, char *argv[]) {
  int servSock;                    /* Socket descriptor for server */
  int clntSock;                    /* Socket descriptor for client */
  struct sockaddr_in echoServAddr; /* Local address */
  struct sockaddr_in echoClntAddr; /* Client address */
  unsigned short echoServPort;     /* Server port */
  unsigned int clntLen;            /* Length of client address data structure */

  if (argc != 5) /* Test for correct number of arguments */
  {
    fprintf(stderr, "Usage:  %s <Server Port>\n", argv[0]);
    exit(1);
  }
  /* Construct the server address structure */
  memset(&echoListnerServAddr, 0,
         sizeof(echoListnerServAddr));      /* Zero out structure */
  echoListnerServAddr.sin_family = AF_INET; /* Internet address family */
  echoListnerServAddr.sin_addr.s_addr =
      inet_addr(argv[3]); /* Server IP address */
  echoListnerServAddr.sin_port = htons(atoi(argv[4]));
  int pid = fork();
  if (pid == -1) {
    printf("Что-то случилось");
    return 0;
  } else if (pid == 0) {
    usleep(1000);
    echoServPort =
        atoi(argv[1]); /* First arg:  local port for the first department */
  } else {
    echoServPort =
        atoi(argv[2]); /* Second arg:  local port for the second department */
  }
  pid = getpid();

  /* Create socket for incoming connections */
  if ((servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    DieWithError("socket() failed");

  /* Construct local address structure */
  memset(&echoServAddr, 0, sizeof(echoServAddr)); /* Zero out structure */
  echoServAddr.sin_family = AF_INET;              /* Internet address family */
  echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
  echoServAddr.sin_port = htons(echoServPort);      /* Local port */



  /* Bind to the local address */
  if (bind(servSock, (struct sockaddr *)&echoServAddr, sizeof(echoServAddr)) <
      0)
    DieWithError("bind() failed");

  printf("Server IP address = %s. Wait...\n", inet_ntoa(echoClntAddr.sin_addr));

  /* Mark the socket so it will listen for incoming connections */
  if (listen(servSock, MAXPENDING) < 0)
    DieWithError("listen() failed");
  for (;;) /* Run forever */
  {
    makeServerLog("Ожидаю покупателя");
    printf("Продавец %d: Ожидаю нового покупателя\n", pid);
    if ((clntSock =
             accept(servSock, (struct sockaddr *)&echoClntAddr, &clntLen)) < 0)
      DieWithError("accept() failed");
    makeServerLog("Покупатель появился");
    printf("Продавец %d: Покупатель появился\n", pid);
    HandleTCPClient(clntSock, pid, listinerServPort);
    makeServerLog("Покупатель ушёл");
    printf("Продавец %d: Покупатель ушел\n", pid);
    putchar('\n');
  }
  /* NOT REACHED */
}
