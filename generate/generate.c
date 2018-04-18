#include <stdbool.h>
#include <semaphore.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>    // Threading
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

// Connection Variables
unsigned short port = 3500;
unsigned short serverPort = 8001;

char ipAddress [15] = "127.0.0.1";
static int flag = 1;
struct data * d;

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

void * run(void * client)
{
    printf("Thread Running!\n");

    // Get Client Socket
    long temp = (long) client;
    
    //This socket connects generator and master computer...
    int clntSock = (int) temp;

    // Get time to read
    // Get sampling rate: 1 reading per second, 2 second, etc.
    int time = 0; //(in seconds)
    int sampling = 0;//(in seconds)
    
    printf("Waiting for reading time\n");
    if(read(clntSock, &time, sizeof(int)) < 0)
    {
        die("Error at reading time.");
    }
    //time = ntohl(time);
    printf("Thread received time rate: %d\n", time);

    printf("Waiting for reading sampling rate\n");
    if(read(clntSock, &sampling, sizeof(int)) < 0)
    {
        die("Error at reading sampling rate.");
    }
    //sampling = ntohl(sampling);
    printf("Thread received sampling rate: %d\n", sampling);

    //If a user wants to send data offline...read it now
    /*
    char ip [15] = "";
    unsigned short portNum;
    if(read(clntSock, &portNum, sizeof(unsigned short)) < 0)
    {
        // No Port Number read...
    }
    if(read(clntSock, &ip, sizeof(ip)) < 0)
    {
        // No IP was read
    }
    // I will assume that the web app checked IP/Port for me    
    */

    int Va, Vb, Vc;
    int Ia, Ib, Ic;
    int Total_Power, Total_FundamentalPower;
    int PhaseA_Power, PhaseB_Power, PhaseC_Power;
    int ReactivePower;
    int PhaseA_ReactivePower, PhaseB_ReactivePower, PhaseC_ReactivePower;
    int Consumed_Power, Sold_Power;

    // Connect to Gateway Computer (ONLINE)
    // Connet to other Computer (OFFLINE)
    // int gatewaySock = createClientSocket(ipAddress, port);

    // Read the Data, get local size
    // Repeat until time is up!

    // Print the data
    // {00000000110,00000000011,...,...,}
    char printData [255] = "";
    int ctr = 0;
    while(true)  
    {  
	//pthread_mutex_lock(&(d -> mutex));
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
    	//pthread_mutex_unlock(&(d -> mutex));
    
    	sprintf(printData, "{%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d}",
    	        Va, Vb, Vc, Ia, Ib, Ic, Total_Power, Total_FundamentalPower, PhaseA_Power,
    	        PhaseB_Power, PhaseC_Power, ReactivePower, PhaseA_ReactivePower, PhaseB_ReactivePower, 
    	        PhaseC_ReactivePower, Consumed_Power, Sold_Power);
    	fprintf(stdout, "%s\n", printData);
    	Send(clntSock, printData);
        if(ctr == time)
        {
            break;
        }
        ++ctr;   
    }
    return NULL;
}

int main(int argc, char **argv)
{
    if (signal(SIGINT, &breakLoop) == SIG_ERR)
    {
	die("CTRL + C failed");
    }

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

    printf("Smart Meter data generator initialized\n");
    pid_t pid = fork();

 
    if(pid < 0)
    {
        die("fork() failed");
    }
    
    // Parent Process will be generating Data
    else if (pid != 0)
    {
        // For every second...
        // Generate new values...
        printf("Data Generator started...\n");
        int t = 1;
        int w = 1;
        int phi = 60;
        while (flag)
        {
            //pthread_mutex_lock(&(d -> mutex));
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
        }
	printf("Generator shutting down\n");
        exit(0);
    }
    
    //Child Processes will be all the incoming requests
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
        printf("New client thread created\n");
	pthread_create(&t1, NULL, run, (void *) client);
        pthread_join(t1, NULL);
    } 
    // for (;;)
    
    //Free everything
    free(d);
    munmap(d, sizeof(struct data));
    return 0;
}
