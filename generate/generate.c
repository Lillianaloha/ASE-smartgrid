#include <stdbool.h>
#include <semaphore.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>       // for sin/cos
#include <sys/mman.h>   // shared memory
#include <signal.h>     // signal()
#include <unistd.h>     // fork()
#include "socket.h"
#include "apue.h"

# define PI 3.14159265
# define Vin 110
# define SQRT2 1.14142
# define SQRT3 1.73205
# define THREADS 16

// Lily...Connect to this Port
unsigned short serverPort = 8001;

static pthread_t thread_pool[THREADS];
static int flag = 1;
struct data * d;

Sigfunc * signal_intr(int signo, Sigfunc *func)
{
    struct sigaction    act, oact;

    act.sa_handler = func;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
#ifdef SA_INTERRUPT
    act.sa_flags |= SA_INTERRUPT;
#endif
    if (sigaction(signo, &act, &oact) < 0)
        return(SIG_ERR);
    return(oact.sa_handler);
}

Sigfunc * signal(int signo, Sigfunc *func)
{
    struct sigaction act, oact;

    act.sa_handler = func;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    if (signo == SIGALRM)
    {
#ifdef  SA_INTERRUPT
        act.sa_flags |= SA_INTERRUPT;
#endif
    } 
    else
    {
        act.sa_flags |= SA_RESTART;
    }
    if (sigaction(signo, &act, &oact) < 0)
        return(SIG_ERR);
    return(oact.sa_handler);
}

static void breakLoop(int signo)
{
    flag = 0;
}

void * generateData()
{
    // For every second...
    // Generate new values...

    printf("Data Generator started...\n");
    int t = 1;
    int w = 1;
    int phi = 60;

    while (flag)
    {
        // pthread_mutex_lock(&(d -> mutex));
        pthread_rwlock_wrlock(&d -> rwlock);
        // va = \sqrt(2) V_in cos(wt + phi)
        // vb = \sqrt(2) V_in cos(wt + phi - 120)
        // vc = \sqrt(2) V_in cos(wt + phi + 120)
             
        // Independant Variables
        // Voltage is default 110, +/- 5%    
        // 104.5 - 114.5 Volts
        
        d -> Va = (int)(SQRT2 * (double) Vin * cos((double) ( w * t + phi)));
        d -> Vb = (int)(SQRT2 * (double) Vin * cos((double) ( w * t + phi - 120)));
        d -> Vc = (int)(SQRT2 * (double) Vin * cos((double) ( w * t + phi + 120)));

  
        // Current 0 - 50 A
        d -> Ia = rand() % 50;
        d -> Ib = rand() % 50;
        d -> Ic = rand() % 50;
    
        //Dependent Variables
        d -> Total_Power += 1;
        d -> Total_FundamentalPower += 1;
        d -> PhaseA_Power = d -> Va * d -> Ia; 
        d -> PhaseB_Power = d -> Vb * d -> Ib;
        d -> PhaseC_Power = d -> Vc * d -> Ic;
            

        //Reactive Power: Irms * Vrms * 
        d -> ReactivePower += 1;
        d -> PhaseA_ReactivePower += 1;
        d -> PhaseB_ReactivePower += 1;
        d -> PhaseC_ReactivePower += 1;
        d -> Consumed_Power += 1;
        d -> Sold_Power += 1;

        //pthread_mutex_unlock(&(d -> mutex));
        pthread_rwlock_unlock(&d -> rwlock);
    }
    printf("Generator shutting down\n");
    return NULL;
}

void * run(void * serv)
{
    printf("Thread Running!\n");	
    /*
    //This socket connects generator and master computer...
    int clntSock = (long) serv;
    */

    //This socket connects generator and master computer...
    int servSock = (long) serv;
    char printData [255] = "";
    int ctr = 0;

    struct sockaddr_in clntAddr;
    unsigned int clntLen = sizeof(clntAddr);
    int clntSock = accept(servSock, (struct sockaddr *)&clntAddr, &clntLen);

    if (clntSock < 0)
    {
        die("accept() failed");
    }

    // Get time to read
    // Get sampling rate: 1 reading per second, 2 second, etc.
    int time_out = 0;       //(in seconds)
    int sampling = 0;   //(in seconds)
    
    printf("Waiting for reading time\n");
    if(read(clntSock, &time, sizeof(int)) < 0)
    {
        die("Error at reading time.");
    }
    printf("Thread received time rate: %d\n", time_out);

    printf("Waiting for reading sampling rate\n");
    if(read(clntSock, &sampling, sizeof(int)) < 0)
    {
        die("Error at reading sampling rate.");
    }
    //sampling = ntohl(sampling);
    printf("Thread received sampling rate: %d\n", sampling);

//---------------Get Extra Data for direct outward communication----------------
/*
    // If a user wants to send data else where as well...
    // Well, I mean they can download a CSV, but I can't judge right?
    // The front end will test the validity of input...
    
  
    if(read(clntSock, port_input, 4) < 0)
    {
        printf("Error at reading external port number.");
	pthread_exit(NULL);
    }
    printf("Thread received sampling rate: %s\n", port_input);
    
    //Remove leading 0s...
    temp = port_input;
    for(int i = 0; i < 4; i++)
    {
        if(port_input[i] == '0')
        {
            temp++;
        }
    }
    printf("value of time_input is: %s\n", temp);
    remote_portNum = atoi(temp);
    
    if(read(clntSock, remote_ip, sizeof(remote_ip)) < 0)
    {
        printf("Error at reading external IP address.");
	pthread_exit(NULL);
    }
    remoteSock = createClientSocket(remote_ip, remote_portNum);      
*/   

    int Va, Vb, Vc;
    int Ia, Ib, Ic;
    int Total_Power, Total_FundamentalPower;
    int PhaseA_Power, PhaseB_Power, PhaseC_Power;
    int ReactivePower;
    int PhaseA_ReactivePower, PhaseB_ReactivePower, PhaseC_ReactivePower;
    int Consumed_Power, Sold_Power;

    while(true)  
    {  
    	pthread_rwlock_rdlock(&d -> rwlock);
        Va = d -> Va;
    	Vb = d -> Vb;
    	Vc = d -> Vc;
    	Ia = d -> Ia;
    	Ib = d -> Ib;
    	Ic = d -> Ic;
    	Total_Power = d -> Total_Power;
    	Total_FundamentalPower = d -> Total_FundamentalPower;
    	PhaseA_Power = d -> PhaseA_Power; 
    	PhaseB_Power = d -> PhaseB_Power;
    	PhaseC_Power = d -> PhaseC_Power;
    	ReactivePower = d -> ReactivePower;
    	PhaseA_ReactivePower = d -> PhaseA_ReactivePower;
    	PhaseB_ReactivePower = d -> PhaseB_ReactivePower;
    	PhaseC_ReactivePower = d -> PhaseC_ReactivePower;
    	Consumed_Power = d -> Consumed_Power;
    	Sold_Power = d -> Sold_Power;
        pthread_rwlock_unlock(&d -> rwlock);

    	sprintf(printData, "{%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d}",
    	        Va, Vb, Vc, Ia, Ib, Ic, Total_Power, Total_FundamentalPower, PhaseA_Power,
    	        PhaseB_Power, PhaseC_Power, ReactivePower, PhaseA_ReactivePower, PhaseB_ReactivePower, 
    	        PhaseC_ReactivePower, Consumed_Power, Sold_Power);
    	fprintf(stdout, "%s\n", printData);
    	Send(clntSock, printData);
        if(ctr == time_out)
        {
            break;
        }
        ++ctr;   
    }
    return NULL;
}

int main(int argc, char **argv)
{
    if(signal(SIGINT, &breakLoop) == SIG_ERR)
    {
	die("CTRL + C failed");
    }

    //Ignore signal...
    if(signal(SIGUSR1, NULL) == SIG_ERR)
    {

    }
    
    /*
    // Command Line Arguments...
    if (argc == 1)
    {
        // Need arguments?
    }
    // Pass in Server Port
    else if (argc == 2)
    {
        serverPort = atoi(argv[1]);
    }
    // Pass in Server Port, omega (frequency)
    else if (argc == 3)
    {
        serverPort = atoi(argv[1]);
        omega = atoi(argv[2]);
    }
    // Pass in Server Port, omega (frequency), phi
    else
    {
        serverPort = atoi(argv[1]);
        omega = atoi(argv[2]);
        phi = atoi(argv[3]);
    }
    */
    srand(time(NULL));

    //Listen for incoming requests
    int servSock = createServerSocket(serverPort);

    //Initialize data struct
    d = (struct data *) mmap(0, sizeof(struct data), 
            PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, -1, 0);
    if(d == MAP_FAILED)
    {
        die("Failed to Map");
    }
    data_init(d);

    printf("Smart Meter Program Initialized!\n");
    pthread_t generatorThread;
    pthread_create(&generatorThread, NULL, &generateData, NULL);
   
    for (int i = 0; i < THREADS; i++)
    {
        pthread_create(&thread_pool[i], NULL, run, (void *)(long) servSock);
    }

    /*
    while(flag)
    {
        // wait for a client to connect
        struct sockaddr_in clntAddr;
        unsigned int clntLen = sizeof(clntAddr);
        int clntSock = accept(servSock, (struct sockaddr *)&clntAddr, &clntLen);
        if (clntSock < 0)
        {
            die("accept() failed");
        }
        
        //Spawn a new thread
        pthread_t t1;
	long client = (long) clntSock;
	pthread_create(&t1, NULL, run, (void *) client);
        pthread_join(t1, NULL);
    }
    // for (;;)
    */    
    
    //Clean up Program...
    pthread_join(generatorThread, NULL);
    for (int i = 0; i < THREADS; i++)
    {
        pthread_join(thread_pool[i], NULL);
    }

    pthread_rwlock_destroy(&d -> rwlock);
    sem_destroy(&d -> mutex);
    munmap(d, sizeof(struct data));
    return 0;
}
