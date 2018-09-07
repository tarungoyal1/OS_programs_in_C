/* Priority Scheduling Algorithm -Preemptive program in C
   Made by Tarun Goyal,
   find me on github at https://github.com/tarungoyal1
 */

 /* This is Preemptive version of Priority Scheduling Algorithm and in this program, any process may contain same bt or at
 You will have to decide which process has to executed which has highest priority and also available in system as per their arrival time

 prId --> ProceesId
 bt --> Burst time
 at --> arrival time
 wt --> Waiting time
 tat --> Turn around time
 ct ---> Completion time
 remt --> Remaining time
 */


#include <stdio.h>
#include <stdlib.h>

typedef struct P{
    int pid;
    int bt;
    int at;
    int prity;
    int wt;
    int tat;
    int ct;
    int remt;
}Process;


void swap(Process *A, Process *B){
    Process temp = *A;
    *A = *B;
    *B = temp;
}

void Insertion_Sort(Process * pr, int n){
    for(int i=1;i<n;i++){
        int j=i;
        Process key  = *(pr+i);
        while(j>0&&(pr+j-1)->at>key.at){
            swap(pr+j,pr+j-1);
            j--;
        }
        swap(pr+j, &key);
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

int highestPrityAvailableIndex(Process * pr, int available[], int size, int prdecide){

    //if available array has only one value just return the index
    if(size==1)return available[0];

    //return the process index which has highest priority
    int m = available[0];
    int maxPrity = (pr+available[0])->prity;

    //if prdecide is 1 means least number indicates least priority
    // if 2 means least number indicates higher priority

    if(prdecide==1){
        //maxPrity will be the largest number given as priority
        for(int i=1;i<size;i++){
            if(maxPrity<(pr+available[i])->prity){
                maxPrity = (pr+available[i])->prity;
                m = available[i];
            }
        }
         return m;
    }else if(prdecide==2){
        //maxPrity will be the smallest number given as priority
        for(int i=1;i<size;i++){
            if(maxPrity>(pr+available[i])->prity){
                maxPrity = (pr+available[i])->prity;
                m = available[i];
            }
        }
    }
    return m;
}

int execute(Process * p, int gtc){
    p->ct = gtc;
    p->remt = p->remt-1;
    return p->remt;
}

void executeProcess(Process * pr, int n, int prdecide){

    Process * tempr = (Process *)malloc(n*sizeof(Process));

    for(int i=0;i<n;i++)
        *(tempr+i) = *(pr+i);

    //first sort the process as per their arrival time
    Insertion_Sort(tempr, n);

    //now take the first process which has obviously the least arrival time
    //but don't execute it select the available process and execute based on burst time


    int gtc = tempr->at;


    int counter=0;

    while(counter<n){

        //first select the available processes and add them their indexes in available array

        int available[n];
        int availSize=0;
        int availIndex = -1;

        for(int i=0;i<n;i++){
            //skip the process that is fully executed
            if((tempr+i)->remt==0)continue;

            if((tempr+i)->at<=gtc){
                available[++availIndex] = i;
                availSize++;
            }

        }

        //it is highly crucial to increment gtc at this point otherwise any process which comes after significant gap won't become available ever
        ++gtc;

        //now available array has indexes of all available process


        if(availSize>0){

            //now choose the process which has highest priority

            int m = highestPrityAvailableIndex(tempr, available, availSize, prdecide);


            if(execute(tempr+m,gtc)==0){
                counter++;

                //also calculate WT and TAT of this completed process at this point:
                (tempr+m)->tat = (tempr+m)->ct-(tempr+m)->at;
                (tempr+m)->wt = (tempr+m)->tat-(tempr+m)->bt;

            }
        }
    }

    printf("PID\tBT\tAT\tPRITY\tWT\tTAT\tCT\n");
    for(int i=0;i<n;i++)
        printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\n", (tempr+i)->pid, (tempr+i)->bt, (tempr+i)->at, (tempr+i)->prity, (tempr+i)->wt, (tempr+i)->tat, (tempr+i)->ct);


    printf("\nAverage Wait time = %.2f\n", calcAvgWaitingTime(tempr, n));
    printf("Average Turn Around time = %.2f\n", calcAvgTurnAroundTime(tempr, n));
}



int main()
{
    int n, prdecide;
    printf("Enter the number of process:");
    scanf("%d", &n);

    Process * pr = (Process *)malloc(n*sizeof(Process));

    printf("Enter PID, BT, AT, PRITY: seperated by space, line by line:\n");

    for(int i=0;i<n;i++){
         scanf("%d%d%d%d", &(pr+i)->pid, &(pr+i)->bt, &(pr+i)->at, &(pr+i)->prity);
         (pr+i)->remt = (pr+i)->bt;
    }

    printf("Please enter:\n 1 if least number indicates least priority or \n 2 if least number indicates highest priority:\n");

    scanf("%d", &prdecide);

    executeProcess(pr, n, prdecide);

    return 0;
}
