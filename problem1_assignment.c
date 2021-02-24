
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 25
#define READ_END 0
#define WRITE_END 1

struct ThreadData
{
    int *arr, lower_bound, upper_bound, threadID, size;
};
struct ThreadReturn
{
    int minNum, maxNum;
};

void printarr(int *arr, int n)
{
    printf("\n %d \n", getpid());

    for (int i = 0; i < n; i++)
        printf(" %d ", arr[i]);
    printf("\n");
}
void *runner(void *param);
int main(void)
{
    pid_t pid;
    int pipe1[2], pipe2[2], pipe3[2], pipe4[2], pipe5[2];
    int n1, n2, n3, n4;
    /*  if (pipe(pipe1) == -1)
    {
        fprintf(stderr, "Pipe1 failed");
        return 1;
    }
    printf("\nI AM %d son of %d", getpid(), getppid());
    printf("\nEnter to parent the no. of terms of Fibonacci series:");
    scanf(" %d", &n1);

    pid = fork();
    if (pid < 0)
    {
        fprintf(stderr, "Fork failed");
    }
    else if (pid > 0)
    {
        close(pipe1[READ_END]);
        write(pipe1[WRITE_END], &n1, sizeof(n1));
        close(pipe1[WRITE_END]);
    }
    else
    {
        //CHILD 1
        printf("\nI AM %d son of %d", getpid(), getppid());
        close(pipe1[WRITE_END]);
        int n1Read;
        read(pipe1[READ_END], &n1Read, sizeof(n1Read));
        close(pipe1[READ_END]);
        printf("\nChild1 received %d", n1Read);
        printf("\nThe Fibonacci Series printed by Child1 is:");
        int firstTerm = 1, secondTerm = 1, nextTerm;
        for (int i = 1; i <= n1Read; ++i)
        {
            printf(" %d", firstTerm);
            nextTerm = firstTerm + secondTerm;
            firstTerm = secondTerm;
            secondTerm = nextTerm;
        }
        exit(0);
    }
 */
    wait(NULL);
    if (pipe(pipe2) == -1)
    {
        fprintf(stderr, "Pipe2 failed");
        return 1;
    }
    if (pipe(pipe3) == -1)
    {
        fprintf(stderr, "Pipe3 failed");
        return 1;
    }
    if (pipe(pipe4) == -1)
    {
        fprintf(stderr, "Pipe4 failed");
        return 1;
    }
    printf("\nEnter to parent the no. of child threads to be created:");
    scanf("%d", &n2);
    printf("\nEnter to parent number of array elements for Child2:");
    scanf("%d", &n3);

    int *arr = (int *)malloc(sizeof(int) * n3);

    printf("Enter to parent the array elements for Child2: ");
    char temp;
    for (int i = 0; i < n3; i++)
        scanf("%d%c", &arr[i], &temp);
    fflush(stdin);
    pid = fork();
    if (pid < 0)
    {
        fprintf(stderr, "Fork failed");
    }
    else if (pid > 0)
    {
        close(pipe2[READ_END]);
        close(pipe3[READ_END]);
        close(pipe4[READ_END]);
        write(pipe2[WRITE_END], &n2, sizeof(int));
        write(pipe3[WRITE_END], &n3, sizeof(int));
        write(pipe4[WRITE_END], arr, sizeof(int) * n3);

        close(pipe2[WRITE_END]);
        close(pipe3[WRITE_END]);
        close(pipe4[WRITE_END]);
    }
    else
    {
        close(pipe2[WRITE_END]);
        close(pipe3[WRITE_END]);
        close(pipe4[WRITE_END]);
        int n2Read, n3Read;
        read(pipe2[READ_END], &n2Read, sizeof(int));
        close(pipe2[READ_END]);

        printf("\nChild2 received %d as no. of threads", n2Read);
        read(pipe3[READ_END], &n3Read, sizeof(int));
        close(pipe3[READ_END]);

        printf("\nChild2 received %d as number of array elements", n3Read);
        int *arrRead;
        arrRead = (int *)malloc(sizeof(int) * n3Read);
        read(pipe4[READ_END], arrRead, sizeof(int) * n3Read);
        close(pipe4[READ_END]);

        printf("\nChild2 Received array as: ");
        for (int i = 0; i < n3Read; i++)
            printf(" %d ", arrRead[i]);

        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_t workers[n2];
        for (int i = 0; i < n2; i++)
        {
            struct ThreadData *threadData = malloc(sizeof(struct ThreadData));
            threadData->lower_bound = i * (n3Read / n2Read);
            threadData->upper_bound = (i + 1) * (n3Read / n2Read) - 1;
            threadData->threadID = i + 1;
            threadData->size = n3Read / n2Read;
            threadData->arr = (int *)malloc(sizeof(int) * n3Read / n2Read);
            for (int j = 0; j < n3 / n2; j++)
                threadData->arr[j] = arrRead[i + threadData->lower_bound];
            pthread_create(&workers[i], &attr, runner, (void *)threadData);
        }
        int maxNum = arrRead[0];
        int minNum = arrRead[0];
        for (int i = 0; i < n2; i++)
        {
            struct ThreadReturn *Result;
            pthread_join(workers[i], (void **)&Result);
            if (Result->minNum < minNum)
                minNum = Result->minNum;
            if (Result->maxNum > maxNum)
                maxNum = Result->maxNum;
        }
        printf("\nCalculated by Child2: max = %d, min = %d", maxNum, minNum);
        exit(0);
    }
    wait(NULL);
    /* 
    if (pipe(pipe5) == -1)
    {
        fprintf(stderr, "Pipe5 failed");
        return 1;
    }
    printf("\nEnter the no. for factorial computation in parent: ");
    scanf(" %d", &n4);

    pid = fork();
    if (pid < 0)
    {
        fprintf(stderr, "Fork failed");
    }
    else if (pid > 0)
    {
        close(pipe5[READ_END]);
        write(pipe5[WRITE_END], &n4, sizeof(int));
        close(pipe5[WRITE_END]);
        wait(NULL);
    }
    else
    {
        //CHILD 3
        printf("\nI AM %d son of %d", getpid(), getppid());
        close(pipe5[WRITE_END]);
        int n4Read;
        read(pipe5[READ_END], &n4Read, sizeof(n4Read));
        close(pipe5[READ_END]);
        printf("\Child3 received %d for factorial computation", n4Read);
        printf("\nFactorial computed by Child3 is :", n4Read);
        int i, fact = 1;
        for (i = 1; i <= n4Read; i++)
        {
            fact = fact * i;
        }
        printf("%d",fact);
        exit(0);
    } */
    wait(NULL);
    printf("\nGOODBYE");
}
void *runner(void *param)
{
    struct ThreadData *threadData = (struct ThreadData *)param;
    printf("Lower bound for Thread%d is: %d", threadData->threadID, threadData->lower_bound);
    printf("Upper bound for Thread%d is: %d", threadData->threadID, threadData->upper_bound);
    int minNum = threadData->arr[0], maxNum = threadData->arr[0];
    for (int i = 1; i < threadData->size; i++)
    {
        if (threadData->arr[i] > maxNum)
            maxNum = threadData->arr[i];
        if (threadData->arr[i] < minNum)
            minNum = threadData->arr[i];
    }
    printf("Computed by Thread%d: max = %d, min = %d", threadData->threadID, maxNum, minNum);
    struct ThreadReturn *threadReturn;
    threadReturn->minNum = minNum;
    threadReturn->maxNum = maxNum;
    pthread_exit((void *)(threadReturn));
}