#include "socket.h"

double validMargin = 1.05;

// Initialize database
void data_init(struct data * d)
{
    if((sem_init(&d->mutex, 1, 1) != 0))
    {
        die("failed to initialize semaphore");
    }
    if(pthread_rwlock_init(&d -> rwlock, NULL) != 0)
    {
        die("Failed to initialize read/write lock");
    }

    d -> Va = 0;
    d -> Vb = 0;
    d -> Vc = 0;
    d -> Ia = 0;
    d -> Ib = 0;
    d -> Ic = 0;
    d -> Total_Power = 0;
    d -> Total_FundamentalPower = 0;
    d -> PhaseA_Power = 0; 
    d -> PhaseB_Power = 0;
    d -> PhaseC_Power = 0;
    d -> ReactivePower = 0;
    d -> PhaseA_ReactivePower = 0;
    d -> PhaseB_ReactivePower = 0;
    d -> PhaseC_ReactivePower = 0;
    d -> Consumed_Power = 0;
    d -> Sold_Power = 0;
}

// Volts are 105 - 115 Volts
// rand() % range + min
int volts()
{
    return (int) rand() % 115 + 105;	
}

// Solar Panels recently can generated
// between 200 - 265 Watts. 

int solarPanel()
{
    return (int) rand() % 265 + 200;	
}


// Current Random Generator
// By: afternoon increase the input 
// morning decrease the input.

// Can easily be modified to increase/decrease
// on month/day/year, etc.

int randomize(int input, double flexibility)
{
    time_t currentTime;
    struct tm * startTime;
    
    //Get the current time
    currentTime = time(NULL);
    
    //Convert it to Local Time representation
    startTime = localtime(&currentTime);
    int startHour = startTime -> tm_hour;
 
    //expect high electricity usage from 5pm till 1am. 
    if(startHour > 17|| stargeHour < 1)
    {
	    input + 20;
    } 
    int diff = input * flexibility;
    int r = rand();
    return (r % (2 * diff + 1)) + (input - diff);
}
