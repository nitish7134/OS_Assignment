#include <stdio.h>
#include <stdlib.h>

const int INF = 1e6 + 5;

struct Process
{
    int id;
    int AT;
    int BT;
    int PRT;
    int CT;
};

int n;
float w1, w2;
struct Process processData[1000];
int ReadyQueue[1000];
int sequence[1000];
int burst[1000];

int min(int i1, int i2)
{
    if (i1 <= i2)
    {
        return i1;
    }
    else
        return i2;
}

double max(double d1, double d2)
{
    if (d1 >= d2)
    {
        return d1;
    }
    else
        return d2;
}

int fillReadyQueue(int T)
{
    //printf("Entered readyqueue\n");
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

double paramValue(int processID)
{
    return w1 * processData[processID - 1].BT + w2 * processData[processID - 1].PRT;
}

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

int nxtArrival(int T)
{
    // printf("You passed a time of : %d\n", T);
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

    int idx = 0, completedProcesses = 0, curTime = 0;

    while (completedProcesses != 3)
    {
        int readyQueueFillSize = fillReadyQueue(curTime);

        if (readyQueueFillSize == 0)
            break;

        // for (int i = 0; i < readyQueueFillSize; ++i)
        // {
        //     printf("\nCurrently in Ready Queue : P%d\n", ReadyQueue[i]);
        // }

        // printf("Ready Queue Size : %d\n", readyQueueFillSize);

        if (readyQueueFillSize == 0)
        {
            sequence[idx++] = -1;
            curTime++;
            continue;
        }
        int RunningID = findBestProcess(readyQueueFillSize);

        // printf("Currently Best Process : P%d\n", RunningID);

        sequence[idx++] = RunningID;

        int nxtTime = nxtArrival(curTime);

        // printf("Next Nearest Process would arrive at : %d\n", nxtTime);

        int execTime;

        if (nxtTime == INF)
        {
            execTime = processData[RunningID - 1].BT;
        }
        else
            execTime = min(nxtTime - curTime, processData[RunningID - 1].BT);

        // printf("This process executed for : %d\n", execTime);

        // printf("This process has left burst of : %d\n", processData[RunningID - 1].BT);

        processData[RunningID - 1].BT -= execTime;

        // printf("This process has left burst of : %d\n", processData[RunningID - 1].BT);

        curTime += execTime;

        //printf("The time now is : %d\n",curTime);

        if (processData[RunningID - 1].BT == 0)
        {
            completedProcesses++;
            processData[RunningID - 1].CT = curTime;
        }
    }

    waitTime();

    execOrder();

    completeTime();
}