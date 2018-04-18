#include <stdio.h>
#include <time.h>
#include <stdbool.h>

/*
 * Goals: 
 * 1- Get something to print every second
 * 2- Get anomaly detection online (say over 5% change)
 * 3- Get your initial values to change by time of day!
 */

void message();

int main(int argc, char ** argv)
{
    time_t epoch;
    time_t prev;
    time_t now;

    time(&epoch);
    unsigned int complete = 10;
    unsigned int sampling = 2;

    time(&now);
    now -= epoch;

    while (true)
    {
        //Prev is holding last iteration of now...
        prev = now;

        //Get current time
        time(&now);
        now -= epoch;
        
        //Has a second passed? If not continue!
        if(now == prev)
        {
            continue;
        }
        printf("A second has passed...");
        
        // 10 seconds passed close...
        if (now >= complete)
        {
            printf("10 seconds are up!\n");
            break;
        }
        else
        {
            printf("current value in seconds: %ld\n", now);
            if(now % sampling == 0)
            {
                //printf("Current Elapsed time is %ld\n", now);
                message();
            }
        }
    }
    return 0;
}

void message()
{
    fprintf(stdout, "Advanced Software Engineering\n");
}
