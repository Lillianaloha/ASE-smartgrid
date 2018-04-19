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
#include <string.h>

# define PI 3.14159265
# define Vin 110
# define SQRT2 1.14142
# define SQRT3 1.73205
# define THREADS 16
# define ON 1
# define OFF 0

//Just in case I want someone to set frequency and phase shift
static int omega = 1;
static int phi = 60;

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

// Set Program off... Kill processes and threads now!
static void breakLoop(int signo)
{
    status = OFF;
}

void * generateData()
{
    // For every second...
    // Generate new values...
    // Omega = 1, Phase = 60 by default
    // Omega/Phase can be changed through command line arguments  

    int t = 1;
    time_t current;
    time_t previous;
    printf("Data Generator started...\n");

    time(&current);
    current -= epoch;

    while (status)
    {
	// http://www.ece.k-state.edu//~starret/581/3phase.html
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
        
        d -> Va = (int)(SQRT2 * (double) Vin * cos((double) ( omega * t + phi)));
        d -> Vb = (int)(SQRT2 * (double) Vin * cos((double) ( omega * t + phi - 120)));
        d -> Vc = (int)(SQRT2 * (double) Vin * cos((double) ( omega * t + phi + 120)));

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
            

        //Reactive Power: Irms * Vrms
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
    char port_input [4];
    unsigned remote_port = 0;
    int remoteSock = 0;
    */

    // Data to Return
    char printData [255] = "";

    // Get time to read
    // Get sampling rate: 1 reading per second, 2 second, etc.
    char time_input [4];
    char sampling_input[4];
    char * temp;        //Shift to remove padding on 0s
    
    int time_out = 0;   //(in seconds)
    int sampling = 0;   //(in seconds)
    
    // Keep track of time
time_t previous;
    time_t current;
	
    time(&current);
    current -= epoch;

//-------------------Get Time------------------------
    printf("Waiting for reading time\n");
    if(read(clntSock, time_input, 4) < 0)
    {
        printf("Error at reading time.\n");
	pthread_exit(NULL);
    }
    printf("Thread received time rate: %s\n", time_input);
    
    //Remove leading 0s...
    temp = time_input;
    for(int i = 0; i < 4; i++)
    {
        if(time_input[i] == '0')
        {
            temp++;
        }
        else
        {
            break;
        }
    }
    printf("value of time_input is: %s\n", temp);
    time_out = atoi(temp);

//----------------Get Sampling Rate-----------------
    printf("Waiting for reading sampling rate\n");
    if(read(clntSock, sampling_input, 4) < 0)
    {
        printf("Error at reading sampling rate.\n");
	pthread_exit(NULL);
    }
    printf("Thread received sampling rate: %s\n", sampling_input);
    
    //Remove leading 0s...
    temp = sampling_input;
    for(int i = 0; i < 4; i++)
    {
        if(sampling_input[i] == '0')
        {
            temp++;
        }
        else
        {
            break;
        }
    }
    printf("value of time_input is: %s\n", temp);
    sampling = atoi(temp);

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

    while(true)  
    {
	// Prev is holding last iteration of current
	previous = current;
	
	// Update and check...
	time(&current);
        current -= epoch;
	
	// A second did not pass...
	if(current == previous)
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
            printf("Begin Reading data\n");
            pthread_rwlock_rdlock(&d -> rwlock);
            sprintf(printData, "{%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d}",
    	        d->Va, d->Vb, d->Vc, d->Ia, d->Ib, d->Ic, d->Total_Power, d->Total_FundamentalPower, 
		d->PhaseA_Power, d->PhaseB_Power, d->PhaseC_Power, d->ReactivePower, 
		d->PhaseA_ReactivePower, d->PhaseB_ReactivePower, d->PhaseC_ReactivePower, 
		d->Consumed_Power, d->Sold_Power);
            pthread_rwlock_unlock(&d -> rwlock);
            
            //fprintf(stdout, "%s\n", printData);
            Send(clntSock, printData);
            //Send(remoteSock, printData);
        }
    }
    printf("Listening Thread finished...closing...\n");
    pthread_exit(NULL);
}

int main(int argc, char **argv)
{
    int servSock;
    pthread_t generatorThread;
    struct sockaddr_in clntAddr;

    if(signal_intr(SIGINT, &breakLoop) == SIG_ERR)
    {
	die("CTRL + C failed\n");
    }

    if(signal(SIGUSR1, &breakLoop) == SIG_ERR)
    {
	die("Killed by SIGUSR1\n");
    }

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
