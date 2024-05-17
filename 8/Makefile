all: TCPEchoClient TCPEchoServer ListenerServer
TCPEchoClient: TCPEchoClient.c DieWithError.c
	gcc TCPEchoClient.c DieWithError.c -o Client
TCPEchoServer: TCPEchoServer.c DieWithError.c
	gcc TCPEchoServer.c DieWithError.c HandleTCPClient.c -o Server
ListenerServer: ListenerServer.c DieWithError.c
	gcc ListenerServer.c DieWithError.c HandleListener.c -o ListenerServer