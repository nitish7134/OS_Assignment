
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>

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

void *runner(void *param);
int main(void)
{

    // freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);
    pid_t pid;
    int pipe1[2], pipe2[2], pipe3[2], pipe4[2], pipe5[2];
    int n1, n2, n3, n4;
    if (pipe(pipe1) == -1)
    {
        fprintf(stderr, "Pipe1 failed");
        return 1;
    }
    printf("Enter to parent the no. of terms of Fibonacci series:");
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
        wait(NULL);
    }
    else
    {
        //CHILD 1
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

    int arr[n3];
    printf("\nEnter to parent the array elements for Child2: ");
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
        write(pipe4[WRITE_END], arr, sizeof(arr));

        close(pipe2[WRITE_END]);
        close(pipe3[WRITE_END]);
        close(pipe4[WRITE_END]);
        wait(NULL);
    }
    else
    {
        close(pipe2[WRITE_END]);
        close(pipe3[WRITE_END]);
        close(pipe4[WRITE_END]);
        int n2Read;
        read(pipe2[READ_END], &n2Read, sizeof(int));
        close(pipe2[READ_END]);
        printf("\nChild2 received %d as no. of threads", n2Read);

        int n3Read;
        read(pipe3[READ_END], &n3Read, sizeof(int));
        printf("\nChild2 received %d as number of array elements", n3Read);
        close(pipe3[READ_END]);

        int arrRead[n3Read];
        read(pipe4[READ_END], arrRead, sizeof(arrRead));
        printf("\nChild2 Received array as:");
        for (int i = 0; i < n3Read; i++)
            printf(" %d ", arrRead[i]);
        close(pipe4[READ_END]);

        pthread_t workers[n2];
        pthread_attr_t attr[n3Read];
        for (int i = 0; i < n3Read; i++)
            pthread_attr_init(&attr[i]);
        int countPerThread = n3Read / n2Read;

        for (int i = 0; i < n2; i++)
        {
            struct ThreadData *threadData = malloc(sizeof(struct ThreadData));
            int l = i * (n3Read / n2Read);
            threadData->lower_bound = l;
            threadData->upper_bound = l + countPerThread - 1;
            threadData->threadID = i + 1;
            threadData->size = countPerThread;
            threadData->arr = (int *)malloc(sizeof(int) * countPerThread);
            for (int j = 0; j < countPerThread; j++)
                threadData->arr[j] = arrRead[j + l];
            pthread_create(&workers[i], &attr[i], runner, (void *)threadData);
        }

        int maxNum = arrRead[0];
        int minNum = arrRead[0];
        for (int i = 0; i < n2; i++)
        {
            void *voidResult;
            pthread_join(workers[i], &voidResult);
            struct ThreadReturn *Result = voidResult;
            if (Result->minNum < minNum)
                minNum = Result->minNum;
            if (Result->maxNum > maxNum)
                maxNum = Result->maxNum;
        }
        printf("\nCalculated by Child2: max = %d, min = %d", maxNum, minNum);
        exit(0);
    }

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
        close(pipe5[WRITE_END]);
        int n4Read;
        read(pipe5[READ_END], &n4Read, sizeof(n4Read));
        close(pipe5[READ_END]);
        printf("\nChild3 received %d for factorial computation", n4Read);
        printf("\nFactorial computed by Child3 is :");
        int i, fact = 1;
        for (i = 1; i <= n4Read; i++)
        {
            fact = fact * i;
        }
        printf("%d", fact);
        exit(0);
    }

    wait(NULL);
    printf("\nGOODBYE\n");
    return 0;
}
void *runner(void *param)
{
    struct ThreadData *threadData = (struct ThreadData *)param;

    printf("\nLower bound for Thread%d is: %d", threadData->threadID, threadData->lower_bound);
    printf("\nUpper bound for Thread%d is: %d", threadData->threadID, threadData->upper_bound);
    printf("\nsize for Thread%d is: %d", threadData->threadID, threadData->size);
    int maxNum = INT_MIN;
    int minNum = INT_MAX;
    for (int i = 0; i < threadData->size; i++)
    {
        int temp = threadData->arr[i];
        if (temp > maxNum)
            maxNum = temp;

        if (temp < minNum)
            minNum = temp;
    }
    printf("\nComputed by Thread%d: max = %d, min = %d", threadData->threadID, maxNum, minNum);
    struct ThreadReturn *threadReturn = malloc(sizeof(struct ThreadReturn));
    threadReturn->minNum = minNum;
    threadReturn->maxNum = maxNum;
    pthread_exit(threadReturn);
}