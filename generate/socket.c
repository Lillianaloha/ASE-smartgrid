#include "socket.h"

void die(char * message)
{
    perror(message);
    exit(0);
}

// Create a listening socket bound to given port.
int createServerSocket(unsigned short port)
{
    int servSock;
    struct sockaddr_in servAddr;

    /* Create socket for incoming connections */
    if ((servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    {
        die("socket() failed");
    }

    /* Construct local address structure */
    memset(&servAddr, 0, sizeof(servAddr));       /* Zero out structure */
    servAddr.sin_family = AF_INET;                /* Internet address family */
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
    servAddr.sin_port = htons(port);              /* Local port */

    /* Bind to the local address */
    if (bind(servSock, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0)
    {
        die("bind() failed");
    }

    /* Mark the socket so it will listen for incoming connections */
    if (listen(servSock, MAXPENDING) < 0)
    {
        die("listen() failed");
    }
    return servSock;
}

int createClientSocket(char * ip, unsigned int port)
{
    int sock;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        die("socket failed");
    }

    // Construct a server address structure
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr)); // must zero out the structure
    servaddr.sin_family      = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(ip); //Convert String to IP Address
    servaddr.sin_port        = htons(port); // must be in network byte order

    // Establish a TCP connection to the server
    // C version of inheritance
    if (connect(sock, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
    {
        die("connect failed");
    }
    return sock;
}

/*
 * A wrapper around send() that does error checking and logging.
 * Returns -1 on failure.
 * 
 * This function assumes that buf is a null-terminated string, so
 * don't use this function to send binary data.
 */
ssize_t Send(int sock, const char *buf)
{
    size_t len = strlen(buf);
    ssize_t res = send(sock, buf, len, 0);
    if (res != len) 
    {
        perror("send() failed");
        return -1;
    }
    else
    { 
        return res;
    }
}
