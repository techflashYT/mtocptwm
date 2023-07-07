#include <mtocptwm.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#if __SWITCH__
#include <nswitch.h>
#endif
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

extern int GUI_Main(char *envp[]);
extern bool mode;


static void communicate(int socket) {
    char buffer[64];
    ssize_t bytesRead;

    while ((bytesRead = read(socket, buffer, 64)) > 0) {
        if (buffer[bytesRead - 1] == '\0') {
            printf("Received: %s\n", buffer);
            break;
        }
    }

    // Close the socket
    // close(socket);
}

void ARG_Init(int argc, char *argv[], char *envp[]) {
	bool badArgs = false;
	if (argc > 2)  {badArgs = true;}
	if (argc == 2) {
		if (argv[1][0] == 'r') {mode = true;}
		else if (argv[1][0] == 't') {mode = false;}
		else {badArgs = true;}
	}
	if (badArgs) {
		puts("Please give argument `r` or `t` for recieve or transmit, or none to start GUI.");
		PLAT_Exit(true);
	}
	// no args, handle GUI stuff
	if (argc == 1) {
		int serverSocket, clientSocket;
		struct sockaddr_in serverAddr, clientAddr;
		socklen_t addrLen = sizeof(struct sockaddr_in);

		// Create socket
		serverSocket = socket(AF_INET, SOCK_STREAM, 0);
		if (serverSocket == -1) {
			perror("socket");
			exit(1);
		}

		// Prepare the server address
		memset(&serverAddr, 0, sizeof(struct sockaddr_in));
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
		serverAddr.sin_port = htons(40435);

		// Bind the socket to localhost
		if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(struct sockaddr_in)) == -1) {
			perror("bind");
			exit(1);
		}

		// Listen for connections
		if (listen(serverSocket, 1) == -1) {
			perror("listen");
			exit(1);
		}

		pid_t pid = fork();
		if (pid == -1) {
			// error
			perror("fork");
			exit(1);
		}
		if (pid == 0) {
			clientSocket = socket(AF_INET, SOCK_STREAM, 0);
			if (clientSocket == -1) {
				perror("socket");
				exit(EXIT_FAILURE);
			}

			// Connect to the server
			if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(struct sockaddr_in)) == -1) {
				perror("connect");
				exit(EXIT_FAILURE);
			}

			communicate(clientSocket);
		}
		if (pid >= 1) {
			// parent
			// Accept a client connection
			clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &addrLen);
			if (clientSocket == -1) {
				perror("accept");
				exit(EXIT_FAILURE);
			}

			// Replace stdin and stdout with the client socket
			if (dup2(clientSocket, STDIN_FILENO) == -1 || dup2(clientSocket, STDOUT_FILENO) == -1) {
				perror("dup2");
				exit(EXIT_FAILURE);
			}

			GUI_Main(envp);
			exit(0);
		}
	}
}
