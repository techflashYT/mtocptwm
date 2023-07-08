#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <mtocptwm.h>

static int serverSocket, clientSocket;
static struct sockaddr_in serverAddr, clientAddr;
static socklen_t addrLen = sizeof(struct sockaddr_in);
void NET_SetupIPCLocalhost() {
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
}

void parentSetup() {
	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket == -1) {
		perror("socket");
		exit(1);
	}

	// Connect to the server
	if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(struct sockaddr_in)) == -1) {
		perror("connect");
		exit(1);
	}
}

void childSetup() {
	// Accept a client connection
	clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &addrLen);
	if (clientSocket == -1) {
		perror("accept");
		exit(1);
	}

	// Replace stdin and stdout with the client socket
	if (dup2(clientSocket, STDIN_FILENO) == -1 || dup2(clientSocket, STDOUT_FILENO) == -1) {
		perror("dup2");
		exit(1);
	}
}

void communicate() {
	uint8_t buffer[64];
	uint8_t tmp;
	uint_fast8_t i = 0;;

	while (1) {
		fputs("reading from sock\r\n", stderr);
		if (read(clientSocket, &tmp, 1) <= 0) {
			fputs("dying now\r\n", stderr);
			break;
		}
		buffer[i] = tmp;

		if (buffer[i] == '\0') {
			fprintf(stderr, "Received from parent: %s\n", buffer);
			if (strcmp(buffer, "exit") == 0) {
				fputs("exiting...", stderr);
				break;
			}
			if (strcmp(buffer, "hello ipc") == 0) {
				// the app has connect
				puts("ipc connection detected");
				write(clientSocket, "welcome", 8);
			}
			
			i = 0;
			continue;
		}

		i++;
	}


	// Close the socket
	close(clientSocket);
	PLAT_Exit(false);
}