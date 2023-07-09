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
	int reuse = 1;
	if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) == -1) {
		perror("setsockopt");
		exit(1);
	}
	struct linger linger_opt;
	linger_opt.l_onoff = 1;  // Enable linger
	linger_opt.l_linger = 0;  // Set the timeout to 0

	if (setsockopt(serverSocket, SOL_SOCKET, SO_LINGER, &linger_opt, sizeof(linger_opt)) == -1) {
		perror("setsockopt");
		exit(1);
	}


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

void childSetup() {
	// Accept a client connection
	clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &addrLen);
	if (clientSocket == -1) {
		perror("accept");
		exit(1);
	}
	printf("child socket: %d\r\n", clientSocket);
}

void communicate() {
	char buffer[64];
	uint8_t tmp;
	uint_fast8_t i = 0;;
	// bool connected = false;

	puts("waiting for data...");
	while (1) {
		if (read(clientSocket, &tmp, 1) <= 0) {
			fputs("dying now\r\n", stderr);
			break;
		}
		buffer[i] = tmp;

		if (buffer[i] == '\0') {
			printf("Received from parent: %s\n", buffer);

			if (strcmp(buffer, "exit") == 0) {
				fputs("exiting...\r\n", stderr);
				break;
			}
			else if (strcmp(buffer, "hello ipc") == 0) {
				// the app has connected
				puts("Got IPC Connection!");
				write(clientSocket, "welcome", 7);
				// connected = true;
			}
			else if (strcmp(buffer, "mcastCopy") == 0) {
				puts("copy");
			}
			else {
				fputs("bad cmd\r\n", stderr);
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