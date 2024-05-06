#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_addr() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */
#include <time.h> // чтобы использовать функцию time()

#define RCVBUFSIZE 1   /* Size of receive buffer */

void DieWithError(char *errorMessage);  /* Error handling function */
int Nproc;
pid_t t[100];


int sock;                        /* Socket descriptor */
struct sockaddr_in echoServAddr; /* Echo server address */
unsigned short echoServPort, echoServPort1, echoServPort2;     /* Echo server port */
char *servIP;                    /* Server IP address (dotted quad) */
char *echoString;                /* String to send to echo server */
char echoBuffer[RCVBUFSIZE];     /* Buffer for echo string */
unsigned int echoStringLen;      /* Length of string to echo */
int bytesRcvd, totalBytesRcvd;   /* Bytes read in single recv() 
                                    and total bytes read */

void Customer(){
  srand(time(NULL) * getpid());
  pid_t my_number = getpid();
  int k = rand() % 5 + 1;
  for (int i = 0; i < k; i++) {
    
    if (rand() % 2 == 0) {
      printf("\n%d: Товар находится в первом отделе.", my_number);
      echoServPort = echoServPort1;
    } else {
      printf("\n%d: Товар находится во втором отделе.", my_number);
      echoServPort = echoServPort2;
    }
    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
     printf("socket() failed");
    exit(0);
    }

    /* Construct the server address structure */
    memset(&echoServAddr, 0, sizeof(echoServAddr)); /* Zero out structure */
    echoServAddr.sin_family = AF_INET; /* Internet address family */
    echoServAddr.sin_addr.s_addr = inet_addr(servIP); /* Server IP address */
    echoServAddr.sin_port = htons(echoServPort);      /* Server port */
    printf("\n%d: Иду в нужный отдел", my_number);
    usleep(2000000);
    printf("\n%d: Пришёл, жду своей очереди", my_number);
    /* Establish the connection to the echo server */
    if (connect(sock, (struct sockaddr *)&echoServAddr, sizeof(echoServAddr)) <
        0){
      printf("connect() failed");
      exit(0);
    }
    printf("\n%d: коннект произошёл", my_number);
    printf("\n%d: Настала моя очередь. Приобретаю товар", my_number);
    if ((bytesRcvd = recv(sock, echoBuffer, RCVBUFSIZE, MSG_WAITALL)) < 0){
      printf("error");
      exit(0);
    }
    while (echoBuffer[0] != '0') {
      if ((bytesRcvd = recv(sock, echoBuffer, RCVBUFSIZE, MSG_WAITALL)) < 0){
        printf("recv() failed 1");
        exit(0);
      }
      usleep(10000);
    }
    printf("\n%d: Продавец проводит пробивание товара.", my_number);
    if ((bytesRcvd = recv(sock, echoBuffer, RCVBUFSIZE, MSG_WAITALL)) < 0){
      printf("error");
      exit(0);
    }
    while (echoBuffer[0] != '1') {
      if ((bytesRcvd = recv(sock, echoBuffer, RCVBUFSIZE, MSG_WAITALL)) < 0){
          printf("recv() failed 2");
          exit(0);
        }
      usleep(20000);
    }
    printf("\n%d: Оплатил и получил товар", my_number);
    close(sock);
    putchar('\n');
  }
}

int main(int argc, char *argv[])
{
    if (argc != 5)    /* Test for correct number of arguments */
    {
       fprintf(stderr, "Usage: %s <Server IP>  [<Echo Port_1>] [<Echo Port_1>]\n",
               argv[0]);
       exit(1);
    }

    servIP = argv[1];             /* First arg: server IP address (dotted quad) */
    echoServPort1 = atoi(argv[2]);
    echoServPort2 = atoi(argv[3]);
    Nproc = atoi(argv[4]);
   
   for(int i = 0 ;i < Nproc ;i++)
   {
       if(fork() == 0)
       {
           Customer();
           exit(0);
         // servIP[strlen(servIP) - 1] += 1;
       }
   }
  exit(0);
}
