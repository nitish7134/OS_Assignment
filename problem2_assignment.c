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
    float ans = (W1 * (p1.BT - p2.BT)) + (W2 * (p1.P - p2.P));
    if (ans < 0 || (ans == 0 && p1.AT > p2.AT))
    {
        //  printf("\n%f ##",ans);
        return 0;
    }
    else
        return 1;
}
void main()
{

#ifndef ONLINE_JUDGE

    freopen("input.txt", "r", stdin);

    freopen("output.txt", "w", stdout);

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
    while (processCompleted != n)
    {
        int i = 0;
        while (i < n && (process[i].AT > t || process[i].CT > 0))
            i++;
        if (i == n)
        {
            //printf("IDLE : %ds",t);
            t++;
            continue;
        }
        int currentProcess = i++;
        for (; i < n; i++)
        {
            if (process[i].CT == -1)
            {
                if (process[i].AT <= t)
                {
                    if (compareProcess(process[currentProcess], process[i], w1, w2) == 0)
                    {
                        currentProcess = i;
                    }
                }
            }
        }
        if (lastProc != process[currentProcess].processno)
        {
            if (seq != 0)
            {
                sequence[seq++] = '-';
                sequence[seq++] = '>';
            }
            sequence[seq++] = 'P';
            sequence[seq++] = process[currentProcess].processno;
            lastProc = process[currentProcess].processno;
        }

        process[currentProcess].BT--;

        if (process[currentProcess].BT == 0)
        {
            processCompleted++;
            lastProc = 'n';
            process[currentProcess].CT = t;
        }
       //printf("\nAt t= %ds : ", t);
        printf("\nP%d",currentProcess+1);
       //printf("P%d(%d,%d)", currentProcess + 1, process[currentProcess].BT, process[currentProcess].P);
        if (lastProc == 'n')
            printf(" THIS IS DONE ");
        t++;
    }
    float sum = 0;
    printf("\n");

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