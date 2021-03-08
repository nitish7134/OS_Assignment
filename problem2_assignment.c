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
    if (ans < 0 || (ans == 0 && p1.AT > p2.AT )){
          //  printf("\n%f ##",ans);
        return 0;
    }
    else
    return 1;
}
void main()
{
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
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
        int currentProcess = i++;

        for (; i < n; i++)
        {
            if (process[i].CT == -1)
            {
                if (process[i].AT <= t)
                {
                    if (compareProcess(process[currentProcess], process[i], w1, w2) == 0)
                    {
                        //printf("\n%d is better than %d so switching to %d : Time %ds", i, currentProcess, i, t);
                       // printf("\t change cuz currentProcess has BT and P as %d and %d while change to has %d and %d", process[currentProcess].BT, process[currentProcess].P, process[i].BT, process[i].P);
                        currentProcess = i;
                    }
                }
            }
        }
        if (lastProc != process[currentProcess].processno)
        {
           // printf("\nCHANGED %d to %d at T %ds", lastProc - '1', process[currentProcess].processno - '1', t);
            if (seq != 0)
            {
                sequence[seq++] = '-';
                sequence[seq++] = '>';
            }
            else
            {
                //printf("\nFIRST ONE : %d \n", currentProcess);
            }
            sequence[seq++] = 'P';
            sequence[seq++] = process[currentProcess].processno;
            lastProc = process[currentProcess].processno;
        }

        process[currentProcess].BT--;

        if (process[currentProcess].BT == 0)
        {
            processCompleted++;
            lastProc = '0';
            process[currentProcess].CT = t;
        }
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
        printf("\nP%d finished at: %d", i + 1, process[i].CT);
    }
}
/*
Received Output:
    Waiting time of P1: 23
    Waiting time of P2: 24
    Waiting time of P3: 22

    Average waiting time: 23.000000

    Process Execution Order: P1->P3->P1->P3->P1->P2->P3->P1->P2->P3->P1->P2->P3->P1->P2->P3->P1->P2->P3

    P1 finished at: 22
    P2 finished at: 26
    P3 finished at: 27

Expected Output:
    Waiting time of P1: 12
    Waiting time of P2: 20
    Waiting time of P3: 0
    Average waiting time: 10.66
    Process Execution Order: P1->P3->P1->P2
    P1 finished at: 22
    P2 finished at: 27
    P3 finished at: 17
*/