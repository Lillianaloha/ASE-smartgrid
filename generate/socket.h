#ifndef SOCKET_H_
#define SOCKET_H_

#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>

#define MAXPENDING 5
int createClientSocket(char * ip, unsigned int port);
int createServerSocket(unsigned short port);
void die(char * message);

struct data
{
    int Va;
    int Vb;
    int Vc;
    int Ia;
    int Ib;
    int Ic;
    int Total_Power;
    int Total_FundamentalPower;
    int PhaseA_Power;
    int PhaseB_Power;
    int PhaseC_Power;
    int ReactivePower;
    int PhaseA_ReactivePower;
    int PhaseB_ReactivePower;
    int PhaseC_ReactivePower;
    int Consumed_Power;
    int Sold_Power;
    sem_t mutex;
    pthread_rwlock_t rwlock;
};
void data_init(struct data * d);
ssize_t Send(int sock, const char *buf);

#endif
