/* FCFS program in C
   Made by Tarun Goyal,
   find me on github at https://github.com/tarungoyal1
 */

 /* In this program, I have assumed the arrival time of all processes is same i.e. 0
 means all processes are available to CPU.

 prId --> ProceesId
 bt --> Burst time
 at --> arrival time
 wt --> Waiting time
 tat --> Turn around time

 */

#include <stdio.h>
#include <stdlib.h>

 struct Process{
    int prId;
    int bt;
    int at;
    int wt;
    int tat;
};

 void executeProcesses(struct Process * pr, int n){
    (pr+0)->wt = 0;
    (pr+0)->tat = (pr+0)->bt + (pr+0)->wt;
    for(int i=1;i<n;i++){
        (pr+i)->wt = (pr+i-1)->bt + (pr+i-1)->wt;
        (pr+i)->tat = (pr+i)->bt + (pr+i)->wt;
    }
 }

 float calculateAverageTime(struct Process * pr, int n){
    int awt = 0;
    for(int i=0;i<n;i++)
        awt+=(pr+i)->wt;
    return (float)awt/n;
 }

 float calculateTurnAroundTime(struct Process * pr, int n){
    int atat = 0;
    for(int i=0;i<n;i++)
        atat+=(pr+i)->tat;
    return (float)atat/n;
 }

int main()
{

    int n;
    printf("Enter the number of processes:");
    scanf("%d", &n);
    if (n<=2){
        printf("Please enter atleast 2 processes!");
        return 0;
    }

    struct Process * pr = (struct Process *)malloc(n*sizeof(struct Process));

    //Enter process id and burst time separated by space, line by line

    printf("Enter process id and burst time of process:\n");
    for(int i=0;i<n;i++){
     scanf("%d%d", &(pr+i)->prId, &(pr+i)->bt);
     (pr+i)->at = 0;
    }


    executeProcesses(pr, n);


    printf("\nP#\tBT\tWT\tTAT\n");
    for(int i=0;i<n;i++)
        printf(" %d\t%d\t%d\t%d\n", (pr+i)->prId, (pr+i)->bt, (pr+i)->wt, (pr+i)->tat);


    printf("\nAverage Wait time = %.2f\n", calculateAverageTime(pr, n));
    printf("Average Turn Around time = %.2f\n", calculateTurnAroundTime(pr, n));


    return 0;
}
