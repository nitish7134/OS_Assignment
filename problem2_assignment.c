#include <stdio.h>
#include <stdlib.h>
struct Process
{
    char processno;
    int AT;
    int BT;
    int P;
    int WT;
    int TAT;
    int CT;
};

int compareProcess(struct Process p1, struct Process p2, float W1, float W2)
{
    return W1 * (p1.BT - p2.BT) + W2 * (p1.P - p2.P) < 0 ? 1 : 0;
}
void main()
{
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
    printf("INPUT DONE");
    int processCompleted = 0;
    int t = 0;
    char sequence[40];
    int seq = 0;
    char lastProc = '0';
    while (processCompleted != n)
    {
        int i = 0;
        while (i < n && process[i].AT > t)
                i++;
        if (i == n)
        {
            t++;
            continue;
        }
        int j = i;
        struct Process currentProcess = process[i];
        for (i = 0; i < n; i++)
        {
            if (j != i)
            {
                if (process[i].CT == -1)
                {
                    if (process[i].AT < t)
                    {
                        if (compareProcess(currentProcess, process[i], w1, w2) == 0)
                            currentProcess = process[i];
                    }
                }
            }
        }
        if (lastProc != currentProcess.processno)
        {
            if (seq != 0)
            {
                sequence[seq++] = '-';
                sequence[seq++] = '>';
            }
            sequence[seq++] = 'P';
            sequence[seq++] = currentProcess.processno;
            lastProc = currentProcess.processno;
        }
    
         currentProcess.BT--;

        if (currentProcess.BT == 0)
        {
            processCompleted++;
            currentProcess.CT = t;
        }
        t++;
    }

    for (int i = 0; i < n; i++)
    {
        printf("P%d finished at: %d", i + 1, process[i].CT);
    }
}