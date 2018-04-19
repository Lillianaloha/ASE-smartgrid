#include <stdbool.h>
#include <semaphore.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>       // for sin/cos
#include <sys/mman.h>   // shared memory
#include <signal.h>     // signal()
#include <unistd.h>     // fork()
#include <time.h>	// for time()...I can't use SIGALRM because of synchronization =(
#include "socket.h"
#include "apue.h"
#include <errno.h>	// for error number

# define PI 3.14159265
# define Vin 110
# define SQRT2 1.14142
# define SQRT3 1.73205
# define THREADS 16
# define ON 1
# define OFF 0

//Current Epoch
time_t epoch;

// Lily...Connect to this Port
unsigned short serverPort = 8001;

// Contain 16 Threads to listen to incoming requests...
// static pthread_t thread_pool[THREADS];

// flag to nicely close program
static int status = ON;

// Contains generator data
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
    {
        return(SIG_ERR);
    }
    return(oact.sa_handler);
}

// Set Program off... Kill processes and threads now!
static void breakLoop(int signo)
{
    status = OFF;
}

void * generateData()
{
    // For every second...
    // Generate new values...
    int t = 1;
    int w = 1;
    int phi = 60;
    time_t current;
    time_t previous;
    printf("Data Generator started...\n");

    time(&current);
    current -= epoch;

    while (status)
    {
	previous = current;
	// Update Current...
	time(&current);
    	current -= epoch;

	//If a second hasn't elapsed wait a bit more...
	if(current == previous)
	{
            continue;		
	}
	//Update Time
	++t;
	
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

        pthread_rwlock_unlock(&d -> rwlock);
    }
    printf("Generator Thread shutting down\n");
    pthread_exit(NULL);
}

void * run(void * connection)
{
    printf("Listening Thread Running!\n");	
    
    //This socket connects generator and computer wanting data...
    int clntSock = (long) connection;
    
    /*
    // For later: This is in case a user wants to get the data in real time
    // instead of downloading from CSV from website...
    char remote_ip [15] = "";
    unsigned remote_port = 0;
    int remoteSock = 0;
    */

    // Data to Return
    int Va, Vb, Vc;
    int Ia, Ib, Ic;
    int Total_Power, Total_FundamentalPower;
    int PhaseA_Power, PhaseB_Power, PhaseC_Power;
    int ReactivePower;
    int PhaseA_ReactivePower, PhaseB_ReactivePower, PhaseC_ReactivePower;
    int Consumed_Power, Sold_Power;
    char printData [255] = "";

    // Get time to read
    // Get sampling rate: 1 reading per second, 2 second, etc.
    char time_input [4];
    char sampling_input[4];
    
    int time_out = 0;   //(in seconds)
    int sampling = 0;   //(in seconds)
    
    // Keep track of time
    time_t current;
	
    time(&current);
    current -= epoch;

    printf("Waiting for reading time\n");
    if(read(clntSock, time_input, 4) < 0)
    {
        die("Error at reading time.");
    }
    //time = ntohl(time_out);
    printf("Thread received time rate: %s\n", time_input);

    printf("Waiting for reading sampling rate\n");
    if(read(clntSock, sampling_input, 4) < 0)
    {
        die("Error at reading sampling rate.");
    }
    //sampling = ntohl(sampling);
    printf("Thread received sampling rate: %s\n", sampling_input);

    // If a user wants to send data else where as well...
    // Well, I mean they can download a CSV, but I can't judge right?
    // The front end will test the validity of input...
    /*
    if(read(clntSock, &portNum, sizeof(int)) < 0)
    {
        // No Port Number read...
    }
    if(read(clntSock, ip, sizeof(ip)) < 0)
    {
        // No IP was read
    }
    remoteSock = createClientSocket(remote_ip, remote_portNum);      
    */

    while(true)  
    {
	// Prev is holding last iteration of current
	time_t previous;
	previous = current;
	
	// Update and check...
	time(&current);
        current -= epoch;
	
	// A second did not pass...
	if(current != previous)
	{	
            continue;
	}

	// Time is up!
	if(current >= time_out)
	{
            printf("Time is up!\n");
            break;
	}

	// Sample data now!
	if(current % sampling == 0)
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
    	
            //fprintf(stdout, "%s\n", printData);
    	
            Send(clntSock, printData);
        }
    }
    pthread_exit(NULL);
}

int main(int argc, char **argv)
{
    int servSock;
    pthread_t generatorThread;
    struct sockaddr_in clntAddr;

    if(signal(SIGINT, &breakLoop) == SIG_ERR)
    {
	die("CTRL + C failed");
    }

    if(signal(SIGUSR1, &breakLoop) == SIG_ERR)
    {
	die("Killed by SIGUSR1");
    }

    // Get Epoch time...to compute elapsed time/time out
    time(&epoch);
    srand(time(NULL));

    //Listen for incoming requests
    servSock = createServerSocket(serverPort);

    //Initialize data struct
    d = (struct data *) mmap(0, sizeof(struct data), 
            PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, -1, 0);
    if(d == MAP_FAILED)
    {
        die("Failed to Map");
    }
    data_init(d);
    printf("Smart Meter Program Initialized!\n");
    pthread_create(&generatorThread, NULL, &generateData, NULL);
    //I will get a print statement saying generator thread is running...
    
    while (status)
    {
        // wait for a client to connect
        // initialize the in-out parameter
        unsigned int clntLen = sizeof(clntAddr);
        int clntSock = accept(servSock, (struct sockaddr *)&clntAddr, &clntLen);
        if (clntSock == -1)
 	{
            if (errno == EINTR)
	    {
                // Signal interrupted accept. Break now.
                break;
            }
            die("accept() failed");
        }

        pthread_t tid;
        pthread_create(&tid, NULL, run, (void *)(long)clntSock);
	pthread_detach(tid);
    }

    pthread_rwlock_destroy(&d -> rwlock);
    sem_destroy(&d -> mutex);
    free(d);
    munmap(d, sizeof(struct data));
    return 0;
}
