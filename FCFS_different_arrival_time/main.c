/* FCFS program in C
   Made by Tarun Goyal,
   find me on github at https://github.com/tarungoyal1
 */

 /* This is Non-Preemptive FCFS and in this program, each process is likey have different arrival time
 i.e., one process comes after the other.

 prId --> ProcessId
 bt --> Burst time
 at --> arrival time
 wt --> Waiting time
 tat --> Turn around time

 */

#include <stdio.h>
#include <stdlib.h>

typedef struct P{
    int prId;
    int bt;
    int at;
    int wt;
    int tat;
}Process;

void executeProcesses(Process * pr, int n){
    /*
    Although this is Non pre-emptive FCFS and we don't require sorting based on arrival time as per original
    scheduling logic but we've sorted the process array bcoz user may enter arrival time in unsorted order which will break
    the logic we are using to achieve the result.
    */

    SortArrivalTime(pr, n);

    //Here we are calculating WT[i] = sum of BT of all previous process - arrival time of ith (current) process
    (pr+0)->wt = 0;
    (pr+0)->tat = (pr+0)->bt + (pr+0)->wt;
    for(int i=1;i<n;i++){
        int sum = 0;
        for(int j=0;j<i;j++)sum+=(pr+j)->bt;
        (pr+i)->wt = sum - (pr+i)->at;
        (pr+i)->tat = (pr+i)->bt + (pr+i)->wt;
    }
 }

float calcAvgWaitingTime(Process * pr, int n){
    int awt = 0;
    for(int i=0;i<n;i++)
        awt+=(pr+i)->wt;
    return (float)awt/n;
 }

 float calcAvgTurnAroundTime(Process * pr, int n){
    int atat = 0;
    for(int i=0;i<n;i++)
        atat+=(pr+i)->tat;
    return (float)atat/n;
 }



 void SortArrivalTime(Process * pr, int n){
 // sorting using Insertion Sort
    for(int i=1;i<n;i++){
        Process key = *(pr+i);
        int j=i;
        while(j>0&&(pr+j-1)->at>key.at){
            //swap the entire Process object
            swap(pr+j, pr+j-1);
            j--;
        }
        swap(pr+j, &key);
    }

 }

 void swap(Process *A, Process *B){
    Process temp = *A;
    *A = *B;
    *B = temp;
}


int main()
{
    int n;
    printf("Enter the number of processes:");
    scanf("%d", &n);
    if (n<=1){
        printf("Please enter atleast 2 processes!");
        return 0;
    }

    Process * pr = (Process * )malloc(n*sizeof(Process));


    //Enter process id, burst time and then arrival time for each process separated by space, line by line

    printf("Enter PID, BT, AT for each process separated by space, line by line:\n");
    for(int i=0;i<n;i++)
     scanf("%d%d%d", &(pr+i)->prId, &(pr+i)->bt, &(pr+i)->at);


    executeProcesses(pr, n);

    printf("\nP#\tBT\tAT\tWT\tTAT\n");
    for(int i=0;i<n;i++)
        printf(" %d\t%d\t%d\t%d\t%d\n", (pr+i)->prId, (pr+i)->bt, (pr+i)->at, (pr+i)->wt, (pr+i)->tat);


    printf("\nAverage Wait time = %.2f\n", calcAvgWaitingTime(pr, n));
    printf("Average Turn Around time = %.2f\n", calcAvgTurnAroundTime(pr, n));

    return 0;
}
