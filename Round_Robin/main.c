/* Round_Robin Scheduling Algorithm program in C
   Made by Tarun Goyal,
   find me on github at https://github.com/tarungoyal1
 */

 /* This is Round_Robin Scheduling Algorithm and in this program, you will have to select the process that are available in request queue and
 execute each of them one by one <= time quantum given by the user
 pid --> ProcessId
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

int checkAlreadyAvailabe(int av[], int n, int key){
    //using recursive linear search
    if(n<0)return 0;
    if(av[n]==key)return 1;
    return checkAlreadyAvailabe(av, n-1,key);

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

int execute(Process * p, int gtc, int tq){
    p->ct = gtc;
    return p->remt-=tq;
}

void executeProcess(Process * pr, int n, int quantum){

    Process * tempr = (Process *)malloc(n*sizeof(Process));

    for(int i=0;i<n;i++)
        *(tempr+i) = *(pr+i);

    //first sort the process as per their arrival time
    Insertion_Sort(tempr, n);

    //now take the first process which has obviously the least arrival time
    //but don't execute it select the available process and execute based on burst time


    int gtc = tempr->at;

    //we will execute each process only <= time quantum given


    int counter=0;


    //here the request array will works similar to request Queue which will contain all the process that next to come and remaining one to be executed as well
    //first select the available processes and add them their indexes in available array

        int request[20];

        int requestIndex = 0;

         //requestCounter will track which process is to be executed next in request queue
        int requestCounter = 0;

    //  this will contain the index of the process which is last executed
    int last =-1;

    while(counter<n){


        for(int i=0;i<n;i++){
            //skip the process that is fully executed
            if((tempr+i)->remt==0)continue;
            if(last==i)continue;

            if((tempr+i)->at<=gtc){
                //but also check that available is already in request queue or not
                //if not then don't insert it in request queue
                if(checkAlreadyAvailabe(request, requestIndex, i))
                    continue;

                request[requestIndex++] = i;
            }
        }

        //after inserting all available processes, also insert the last process which has remaining time left to be executed
        // this last process is made up below, after multiline comment
        if(last>-1){
            if((tempr+last)->remt!=0){
                request[requestIndex++] = last;
                }
        }

        //now available array has indexes of all available process


        if(requestIndex>0){

            //but in Round Robin we will always have to execute the next process that is in request queue based on requestCounter

            int c =  request[requestCounter];
            int tq = quantum;

            printf("%d\n", c+1);

            if((tempr+c)->remt<quantum){
                gtc+=(tempr+c)->remt;
                tq = (tempr+c)->remt;
            }else gtc+=quantum;



            if(execute(tempr+c,gtc,tq)==0){
                counter++;
                last = -1;

                //also calculate WT and TAT of this completed process at this point:
                (tempr+c)->tat = (tempr+c)->ct-(tempr+c)->at;
                (tempr+c)->wt = (tempr+c)->tat-(tempr+c)->bt;

            }else{
                /*this is very important to tell the current process index which has not fully executed to last variable
                so that it could be inserted at the last of request queue
                */

                last = c;


            }

            ++requestCounter;

        //it is crucial to pre increment the gtc in the else block
        //because it will cover all process that come after significant gap of completion of the last process available
        }else ++gtc;
    }

    printf("PID\tBT\tAT\tWT\tTAT\tCT\n");
    for(int i=0;i<n;i++)
        printf("%d\t%d\t%d\t%d\t%d\t%d\n", (tempr+i)->pid, (tempr+i)->bt, (tempr+i)->at, (tempr+i)->wt, (tempr+i)->tat, (tempr+i)->ct);


    printf("\nAverage Wait time = %.2f\n", calcAvgWaitingTime(tempr, n));
    printf("Average Turn Around time = %.2f\n", calcAvgTurnAroundTime(tempr, n));
}



int main()
{
    int n, quantum;
    printf("Enter the number of process:");
    scanf("%d", &n);

    Process * pr = (Process *)malloc(n*sizeof(Process));

    printf("Enter PID, BT, AT: seperated by space, line by line:\n");

    for(int i=0;i<n;i++){
         scanf("%d%d%d", &(pr+i)->pid, &(pr+i)->bt, &(pr+i)->at);
         (pr+i)->remt = (pr+i)->bt;
    }

    printf("Please enter the time quantum:");
    scanf("%d", &quantum);

    executeProcess(pr, n, quantum);

    return 0;
}
