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
    time_t start, now;
    time(&start);
    printf("Epoch time in seconds is: %ld\n", start);
    int complete = 10;
    int sampling = 2;
    
    //int counter = 9;
    while (true)
    {
        //Get current time
        now = time(NULL) - start;
        
        // 10 seconds passed close...
        if (now >= complete)
        {
            printf("10 seconds are up!\n");
            break;
        }
        else
        {
            //printf("current value in seconds: %ld\n", now);
            if(now % sampling == 0)
            {
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
