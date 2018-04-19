#include "socket.h"

double validMargin = 1.05;

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

bool isAnomaly(int x)
{
    double testCase = (double) x;    
    double comparison = (testCase * validMargin);

    if(testCase > comparison)
    {
        return true;
    }
    else if(testCase < comparison)
    {
        return true;
    }
    return false;
}
