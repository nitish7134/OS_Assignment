#include <stdio.h>
#include <stdlib.h>

const int INF = 1e6 + 5;

/* Struct for a process*/
struct Process
{
    int id;
    int AT;
    int BT;
    int PRT;
    int CT;
};

int n;  // Total No. of Processses
float w1, w2;   // Parameter constants w1,w2
struct Process processData[1000];   // Array containing all the processes
int ReadyQueue[1000]; // Array for storing id of processes in Ready Queue
int sequence[1000]; // Array for storing the sequence of execution 
int burst[1000]; // Array for storing original burst times prior to execution


// Min functiion utility for finding the minimum arrival time
int min(int i1, int i2)
{
    if (i1 <= i2)
    {
        return i1;
    }
    else
        return i2;
}


// max function utility for finding the maximum parameter value out of all given processes
double max(double d1, double d2)
{
    if (d1 >= d2)
    {
        return d1;
    }
    else
        return d2;
}

// function to fill the ready with all uncomplete processes that have arrived 
int fillReadyQueue(int T)
{
    int fillCount = 0;
    for (int i = 0; i < n; ++i)
    {
        if (processData[i].AT <= T && processData[i].CT == INF)
        {
            ReadyQueue[fillCount++] = processData[i].id;
        }
    }
    return fillCount;
}

// function to find the parameter value w1*T+w2*PR
double paramValue(int processID)
{
    return w1 * processData[processID - 1].BT + w2 * processData[processID - 1].PRT;
}

// function to find the process that has maximum value of parameter and has arrived the earliest
int findBestProcess(int RQSize)
{
    double mxParam = 0.0;
    int minArrival = INF;
    for (int i = 0; i < RQSize; ++i)
    {
        mxParam = max(mxParam, paramValue(ReadyQueue[i]));
    }
    for (int i = 0; i < RQSize; ++i)
    {
        if (paramValue(ReadyQueue[i]) == mxParam)
        {
            minArrival = min(minArrival, processData[ReadyQueue[i] - 1].AT);
        }
    }
    for (int i = 0; i < RQSize; ++i)
    {
        if (paramValue(ReadyQueue[i]) == mxParam && processData[ReadyQueue[i] - 1].AT == minArrival)
        {
            return ReadyQueue[i];
        }
    }
}

// function to find the arrival time for the next process, INF if all process have already arrived
int nxtArrival(int T)
{
    int nxat = INF;
    for (int i = 0; i < n; ++i)
    {
        if (processData[i].AT > T)
        {
            nxat = min(nxat, processData[i].AT);
        }
    }
    return nxat;
}

// function to find the waiting time of all processes as WT = CT-AT-BT and avg wt time = sum(wt time)/n
void waitTime()
{
    float sum = 0.0;
    for (int i = 0; i < n; i++)
    {
        int wt = processData[i].CT - processData[i].AT - burst[i];
        printf("\nWaiting time of P%d: %d", i + 1, wt);
        sum += wt;
    }
    printf("\n");

    printf("\nAverage waiting time: %.4f", sum / (float)n);
    printf("\n");
}

// function to print the execution order of process, print only when process changes
void execOrder()
{
    int i = 0;
    while (sequence[i] != 0)
    {
        if (sequence[i] != sequence[i + 1])
        {
            if (sequence[i + 1] == 0)
                printf("P%d", sequence[i]);
            else
            {
                if (sequence[i] == -1)
                    printf("IDLE->");
                else printf("P%d->",sequence[i]);
            }
        }
        i++;
    }
    printf("\n");
}

// function to print the completion time of all the processes
void completeTime()
{
    printf("\n");
    for (int i = 0; i < n; ++i)
    {
        printf("P%d finished at: %d\n", i + 1, processData[i].CT);
    }
}

int main()
{

    /*Input Starts Here*/
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    for (int i = 0; i < n; i++)
    {
        processData[i].id = i + 1;
        printf("\nEnter arrival time of P%d: ", i + 1);
        scanf("%d", &processData[i].AT);
        printf("Enter CPU burst cycle of P%d: ", i + 1);
        scanf("%d", &processData[i].BT);
        printf("Enter prioroity of P%d: ", i + 1);
        scanf("%d", &processData[i].PRT);
        processData[i].CT = INF;
        burst[i] = processData[i].BT;
    }

    printf("\nEnter value of w1: ");
    scanf("%f", &w1);
    printf("Enter value of w2: ");
    scanf("%f", &w2);

    /*Input Ends*/

    int idx = 0, completedProcesses = 0, curTime = 0;

    /*Loop until all processes are complete*/
    while (completedProcesses != n)
    {
        int readyQueueFillSize = fillReadyQueue(curTime); // fill the ready queue until

        int nxtTime = nxtArrival(curTime); // arrival time for the next process

        if (readyQueueFillSize == 0)    // If the readyQueue is empty, CPU is IDLE 
        {
            sequence[idx++] = -1;
            curTime+=nxtTime-curTime;
            continue;
        }
        // else readyQueue is not empty
        int RunningID = findBestProcess(readyQueueFillSize); // find the ID of the best process

        sequence[idx++] = RunningID;// set sequence as ID of the choosen process

        int execTime;// execution time of the current process

        if (nxtTime == INF) // All processes have already arrived
        {
            execTime = processData[RunningID - 1].BT;
        }
        else // We run the choosen process either till its left burst time or until a new process arrives for selection
            execTime = min(nxtTime - curTime, processData[RunningID - 1].BT);

        processData[RunningID - 1].BT -= execTime;// Burst time of the current process decreases by execution time


        curTime += execTime;// A time lapse forward till execution time of this process

        if (processData[RunningID - 1].BT == 0) // No burst left
        {
            completedProcesses++;// Increment completed processes
            processData[RunningID - 1].CT = curTime; // set the completion time as the current time
        }
    }

    /*Output Sarts Here*/
    
    waitTime();

    execOrder();

    completeTime();

    /*Output Ends*/

    return 0;
}