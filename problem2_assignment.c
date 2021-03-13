#include <stdio.h>
#include <stdlib.h>

const int SIZE =  100;

struct Process
{
    int processno;
    int AT;                     // Arrival Time
    int BT;                     // Burst Time
    int P;                      // Priority
    int WT;                     // Wait Time
    int CT;                     // Completion Time
};

struct Process ReadyQueue[SIZE];

int queueFilled = 0; //How much the Queue is FILLED

int Running; //Stores value of Running Proecess

 //Returns 1 if we should Run process p1, Returns 0 if we should run p2
 
int compareProcess(struct Process p1, struct Process p2, float W1, float W2)
{
    float ans = (W1 * (p1.BT - p2.BT)) + (W2 * (p1.P - p2.P));
    if (ans < 0 || (ans == 0 && p1.AT > p2.AT))
    {
        return 0;
    }
    else
        return 1;
}


void main()
{
   // Running = NULLPROCESS;
#ifndef ONLINE_JUDGE

    //freopen("input.txt", "r", stdin);

    //freopen("output.txt", "w", stdout);

#endif
    int n;
    float w1, w2;
    printf("Enter the number of processes: ");
    scanf("%d", &n);
    struct Process process[n];
    for (int i = 0; i < n; i++)
    {
        char temp;
        process[i].processno = i + '1';
        printf("\nEnter arrival time of P%d: ", i + 1);
        scanf("%d", &process[i].AT);
        printf("Enter CPU burst cycle of P%d: ", i + 1);
        scanf("%d", &process[i].BT);
        printf("Enter prioroity of P%d: ", i + 1);
        scanf("%d", &process[i].P);
        process[i].CT = -1;
    }
    printf("\nEnter value of w1: ");
    scanf("%f", &w1);
    printf("Enter value of w2: ");
    scanf("%f", &w2);

    int processCompleted = 0;
    int t = 0;
    char sequence[100];
    int seq = 0;
    char lastProc = 'n';

    //SORT PROCESS BY ARRIVAL TIME(O(n^2))
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n - 1; j++)
        {
            if (process[j].AT > process[j + 1].AT)
            {
                struct Process tempProcess;
                tempProcess = process[j];
                process[j] = process[j + 1];
                process[j + 1] = tempProcess;
            }
        }
    }

    int i = 0;
    while (processCompleted!=n)
    {
        while(process[i].AT==t){
            ReadyQueue[queueFilled++] = process[i++];
        }
        int nxtArrivalTime = process[i].AT;
        struct Process mxproc = ReadyQueue[0];

        for(int j=1;j<queueFilled;++j){
            if(!compareProcess(mxproc,process[j],w1,w2)){
                mxproc = process[j]; 
            }
        }

        Running = mxproc.processno;

        int exec_time = min(t+mxproc.BT,nxtArrivalTime);
        if(exec_time==t+mxproc.BT){
            processCompleted++;
            process[Running].CT = t+exec_time;
            i++;
        }
        mxproc.BT-=exec_time;
        t+=exec_time;
    }
    

    float sum;
    for (int i = 0; i < n; i++)
    {
        int wt = process[i].CT - process[i].AT - process[i].BT;
        printf("\nWaiting time of P%d: %d", i + 1, wt);
        sum += wt;
    }
    printf("\n");

    printf("\nAverage waiting time: %f", sum / (float)n);
    printf("\n");

    printf("\nProcess Execution Order: ");
    for (int i = 0; i < seq; i++)
    {
        printf("%c", sequence[i]);
    }
    printf("\n");
    for (int i = 0; i < n; i++)
    {
        printf("\nP%d finished at: %d\n", i + 1, process[i].CT);
    }
}