#ifndef SOCKET_H_
#define SOCKET_H_

#include <sys/socket.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>

// Have all method calls from helper.c and socket.c

#define MAXPENDING 5
int createClientSocket(char * ip, unsigned int port);
int createServerSocket(unsigned short port);
ssize_t Send(int sock, const char *buf);
void die(char * message);
int randomize(int input, double flexibility);
int volts();
int solarPanel();

// Data structure containing generated data...
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
#endif
