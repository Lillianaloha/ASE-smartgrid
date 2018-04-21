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
# define ON 1
# define OFF 0
# define THREADS 16

// Power Generator
static int omega = 1;
static int phi = 60;

// Lily...Connect to this Port
unsigned short serverPort = 8000;

//Current Epoch
time_t epoch;

static pthread_t thread_pool[THREADS];
static int status = ON;
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
    status = OFF;
}

void * generateData()
{
    // For every second...
    // Generate new values...

    int t = 0;
    time_t current;
    time_t previous;
    printf("Data Generator started...\n");

    time(&current);
    current -= epoch;

    while (status)
    {
	// http://www.ece.k-state.edu//~starret/581/3phase.html
	// https://www.electronics-tutorials.ws/accircuits/reactive-power.html
	previous = current;
	// Update Current...
	time(&current);
    	current -= epoch;

	//If a second hasn't elapsed wait a bit more...
	if(current == previous)
	{
            continue;		
	}
	printf("Data being generated\n");
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
    printf("Generator shutting down\n");
    return NULL;
}

void * run(void * connection)
{
    printf("Thread Running!\n");	
    /*
    // Get Client Socket
    long temp = (long) client;
    //This socket connects generator and master computer...
    int clntSock = (int) temp;
    */

    // Get time to read
    // Get sampling rate: 1 reading per second, 2 second, etc.
    char time_input [4];
    char sampling_input[4];
    char * temp;        //Shift to remove padding on 0s
    char printData [255] = "";

    time_t current;
    time_t thread_epoch;
    time_t previous;

    int servSock = (long) connection;

    struct sockaddr_in clntAddr;
    unsigned int clntLen = sizeof(clntAddr);
    int clntSock = accept(servSock, (struct sockaddr *)&clntAddr, &clntLen);

    if (clntSock < 0)
    {
        printf("accept() failed\n");
	pthread_exit(NULL);
    }

    // Get time to read
    // Get sampling rate: 1 reading per second, 2 second, etc.
    unsigned int time_out = 0;           //(in seconds)
    unsigned int sampling = 0;           //(in seconds)
    
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
 
    // Start While
    time(&current);
    time(&thread_epoch);
    current-= thread_epoch;

    while(true)  
    {
	// Prev is holding last iteration of current
	previous = current;
	
	// Update and check...
	time(&current);
        current -= thread_epoch;
	
	// A second did not pass...
	if(current == previous)
	{
            continue;
	}
        //printf("Current is: %ld and Previous is: %ld\n", current, previous);
	//printf("Start to read, time out is: %d and sampling is: %d\n", time_out, sampling);
        
   	// Sample data now!
	if(current % sampling == 0)
	{
            printf("Begin Reading data\n");
            //pthread_rwlock_rdlock(&d -> rwlock);
            sprintf(printData, "{%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d}",
    	        d->Va, d->Vb, d->Vc, d->Ia, d->Ib, d->Ic, d->Total_Power, d->Total_FundamentalPower, 
		d->PhaseA_Power, d->PhaseB_Power, d->PhaseC_Power, d->ReactivePower, 
		d->PhaseA_ReactivePower, d->PhaseB_ReactivePower, d->PhaseC_ReactivePower, 
		d->Consumed_Power, d->Sold_Power);
            //pthread_rwlock_unlock(&d -> rwlock);
            
            fprintf(stdout, "%s\n", printData);
            
            /*
            if(Send(clntSock, printData) < 0)
            {
                printf("Error sending!");
            }
            */
            //send(clntSock, printData, sizeof(printData), 0);
        }

	// Time is up!
	if(current >= time_out)
        {
            printf("Time is up!\n");
            break;
	}
    }
    printf("Listening Thread complete!\n");
    return NULL;
}

int main(int argc, char **argv)
{
    if(signal_intr(SIGINT, &breakLoop) == SIG_ERR)
    {
	die("CTRL + C failed\n");
    }

    //Ignore signal...
    if(signal(SIGUSR1, NULL) == SIG_ERR)
    {

    }

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

    printf("Smart Meter Program Initialized! Clock initialized!\n");
    time(&epoch);

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


    //Wait...CTRL+C
    while(status);
    
    //Clean up Program...
    pthread_join(generatorThread, NULL);
    for (int i = 0; i < THREADS; i++)
    {
        pthread_join(thread_pool[i], NULL);
    }

    pthread_rwlock_destroy(&d -> rwlock);
    sem_destroy(&d -> mutex);
    free(d);
    munmap(d, sizeof(struct data));
    return 0;
}
