#include <stdbool.h>
#include <semaphore.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>       // for sin/cos
#include <sys/mman.h>   // shared memory
#include <signal.h>     // signal()
#include <unistd.h>     // fork()
#include <errno.h>

#include "generate.h"
#include "apue.h"

# define PI 3.14159265
# define Vin 110
# define SQRT2 1.14142
# define SQRT3 1.73205
# define THREADS 16
# define ON 1
# define OFF 0

// Epoch time
time_t epoch;

// Modify AC Power
static int omega = 1;
static int phi = 60;

// Lily...Connect to this Port for generator
unsigned short serverPort = 8001;

// Contains both a flag that turns program off...
static int status = ON;

// Contains all smart meter data
struct data * d;

// Signal handler for interrupt. 
// Allows me to break from accept() blocking call
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

// To handle other signals like SIGUSR1
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

/*
 * When someone presses CTRL + C
 * on the command line terminal,
 * the flag is set to off, which
 * allows the program to leave the
 * while loop
 */
static void breakLoop(int signo)
{
    status = OFF;
}

// This is the function run by the thread
// to generate smart meter readings
void * generateData()
{
    int time_tick = 0;
    double cosAngle = 0; 
    double sinAngle = 0; 
    time_t current;
    time_t previous;
    printf("Data Generator started...\n");

    time(&current);
    current -= epoch;
    while (status)
    {
        // Resources used:
	// http://www.ece.k-state.edu//~starret/581/3phase.html
	// https://www.electronics-tutorials.ws/accircuits/reactive-power.html
	
        // Get previous time tick
        previous = current;

	// Update Current...
	time(&current);
    	current -= epoch;
	
	// If a second hasn't elapsed 
        // do NOT update	
	if(current == previous)
	{
            continue;		
	}

	++time_tick;

        // Compute angle for different phases
	cosAngle = cos((double) (omega * time_tick + phi));
	sinAngle = sin((double) (omega * time_tick + phi));
        
        // Lock the data, only one thread should write
        // All other threads should NOT read
        pthread_rwlock_wrlock(&d -> rwlock);
             
        // Independant Variables
        // Voltage is default 110, +/- 5%    
        // 104.5 - 114.5 Volts
        // Since I am using int (105 - 115 Volts)
        
        // See the helper.c file
        // In short all you need to know it to expect
        // a number between 105 - 115 returned
        d -> Va = volts();
        d -> Vb = volts();
        d -> Vc = volts();
  
        // Current 0 - 50 A
        // Subject to most change over the course of a day...
        // We will use our specified random number generator
        d -> Ia = rand() % 50;
        d -> Ib = rand() % 50;
        d -> Ic = rand() % 50;
    
        // Dependent Variables (Active Power)
        // Note Active Power can't be negative...
        // Essentially this is power that can used
        d -> PhaseA_Power = abs(d -> Va * d -> Ia * cosAngle); 
        d -> PhaseB_Power = abs(d -> Vb * d -> Ib * cosAngle);
        d -> PhaseC_Power = abs(d -> Vc * d -> Ic * cosAngle);
	d -> Total_Power = abs(d -> PhaseA_Power + d -> PhaseB_Power + d -> PhaseC_Power);  
	
	// Dependent Variables (Reactive Power)
        // Can be Positive or Negative
        // Usually you want to minimize this
        // This tends to be considered "wasted" power.
	d -> PhaseA_ReactivePower = d -> Va * d -> Ia * sinAngle;
        d -> PhaseB_ReactivePower = d -> Vb * d -> Ib * sinAngle;
        d -> PhaseC_ReactivePower = d -> Vc * d -> Ic * sinAngle;
	d -> ReactivePower = d -> PhaseA_ReactivePower + d -> PhaseB_ReactivePower + d -> PhaseC_ReactivePower;   
        
	// Using Power Triangle (See references)
        // Get the total power from both Active and Reactive Power
        d -> Total_FundamentalPower = (int) sqrt((double) d->ReactivePower * d->ReactivePower + d->Total_Power * d->Total_Power);

	// Assume home has some solar panel.
	// https://solarpowerrocks.com/solar-basics/how-much-electricity-does-a-solar-panel-produce/
	// 265 Watts of power, will give it a range 200 - 265 based on 2012-2015 panels
        // The random number generator is in helper.c
        // Expect an int returned between 200 - 265
        d -> Sold_Power = solarPanel();

        // Assuming that this homeowner decides to use all their power they 
        // generated instead of selling it to the grid, subtract it.
	d -> Consumed_Power = d -> Total_FundamentalPower - d -> Sold_Power;

        // Unlock, all readers can access now
        pthread_rwlock_unlock(&d -> rwlock);
    }
    printf("Generator shutting down\n");
    pthread_exit(NULL);
}

void * run(void * serv)
{
    printf("Thread Running!\n");	
    
    //This socket connects generator and master computer...
    int clntSock = (long) serv;
    
    //This socket connects generator and master computer...
    /*
    int servSock = (long) serv;
    struct sockaddr_in clntAddr;
    unsigned int clntLen = sizeof(clntAddr);
    int clntSock = accept(servSock, (struct sockaddr *)&clntAddr, &clntLen);
    if (clntSock < 0)
    {
        die("accept() failed");
    }
    */


    char printData [1000] = "";

    time_t current;
    time_t previous;
    time_t thread_epoch;

    // Get time to read
    // Get sampling rate: 1 reading per second, 2 second, etc.
    char sampling_input [4];
    char time_input [4];
    char * temp;
    int time_out = 0;       //(in seconds)
    int sampling = 0;       //(in seconds)
    
//-------------------Get Time------------------------
    printf("Waiting for reading time\n");
    /* We learned that PHP sockets don't seem to 
     * understand primitive type ints.
     * For this to work we discovered I need to 
     * read in as a string.
     * Then I can parse it using atoi()
     */
    if(read(clntSock, time_input, 4) < 0)
    {
        printf("Error at reading time.\n");
	pthread_exit(NULL);
    }
    printf("Thread received time rate: %s\n", time_input);
    
    /*
     * For a while, we suspected the issue was 
     * PHP not reading in 4 bytes...
     * So we padded with 0's. But it looks like that
     * wasn't the issue.
     *
     * Either way, we decided to keep this code here
     * since it does work and I guess it is nice to
     * have in case someone did place leading 0's
     */
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
// This is essentially the same as get get time...

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
        
        // No time to sample yet...
        if(current % sampling != 0)
        {
            continue;
        }

        //LOCK
    	pthread_rwlock_rdlock(&d -> rwlock);
   
    	sprintf(printData, "{%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d}",
    	        d->Va, d->Vb, d->Vc, d->Ia, d->Ib, d->Ic, d->Total_Power, d->Total_FundamentalPower, d->PhaseA_Power,
    	        d->PhaseB_Power, d->PhaseC_Power, d->ReactivePower, d->PhaseA_ReactivePower, d->PhaseB_ReactivePower, 
    	        d->PhaseC_ReactivePower, d->Consumed_Power, d->Sold_Power);

        //UNLOCK
        pthread_rwlock_unlock(& d->rwlock);

    	fprintf(stdout, "%s\n", printData);
    	Send(clntSock, printData);
        if(current == time_out)
        {
            printf("Reader Thread time expired...\n");
            close(clntSock);
            break;
        }
    }
    printf("Thread is exiting...\n");
    //close(remoteSock);
    pthread_exit(NULL);
}

int main(int argc, char **argv)
{

    // SIGPIPE can be ignored, we close the socket
    if(signal(SIGPIPE, SIG_IGN) == SIG_ERR)
    {
        die("Failed to ignore SIGPIPE\n");
    }

    // For nice closure of program...
    if(signal_intr(SIGINT, &breakLoop) == SIG_ERR)
    {
	die("CTRL + C failed\n");
    }

    // Ignore signal
    if(signal(SIGUSR1, SIG_IGN) == SIG_ERR)
    {
        die("Failed to ignore SIGUSER1\n");
    }
    
    // Command Line Arguments...
    if (argc == 1)
    {
        // No arguments
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
    
    // Listen for incoming requests
    int servSock = createServerSocket(serverPort);

    /*
     * Initialize data struct
     * I know that I don't need shared memory for 2 threads
     * long story short, I was originally going to fork()
     * but I reverted to threading. 
     * But should I ever need to go to multi=process, this wil work
     */
    d = (struct data *) mmap(0, sizeof(struct data), 
            PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, -1, 0);
    if(d == MAP_FAILED)
    {
        die("Failed to Map");
    }

    // Initialize data, place initial input current...
    data_init(d);

    printf("Smart Meter Program Initialized! Clock initialized\n");
    
    // Set random seed ONCE, might as well set epoch time now as well
    srand(time(&epoch));

    // Run the Generator Thread
    pthread_t generatorThread;
    pthread_create(&generatorThread, NULL, &generateData, NULL);
   
    // Status = ON (1)
    // When you hit CTRL+C will turn to OFF (0)
    // and break out and close program
    while(status)
    {
        // wait for a client to connect
        struct sockaddr_in clntAddr;
        unsigned int clntLen = sizeof(clntAddr);
        int clntSock = accept(servSock, (struct sockaddr *)&clntAddr, &clntLen);
        if (clntSock < 0)
        {
            if(errno == EINTR)
            {
                continue;
            }
            status = OFF;
            break;
        }
        
        // Spawn a new thread (from web server)
        // to read data from generator
        pthread_t reader;
	pthread_create(&reader, NULL, run, (void *) (long) clntSock);
        pthread_detach(reader);
    }
    
    // Close/Clean up Program
    pthread_join(generatorThread, NULL);
    pthread_rwlock_destroy(&d -> rwlock);
    sem_destroy(&d -> mutex);
    munmap(d, sizeof(struct data));
    return 0;
}
