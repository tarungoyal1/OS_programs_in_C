
/* SRTF Preemptive program in C
   Made by Tarun Goyal,
   find me on github at https://github.com/tarungoyal1
 */

 /* This is Preemptive SJF or also known as SRTF and in this program, any process may contain same bt or at
 pid--> ProcessId
 bt --> Burst time
 at --> arrival time
 wt --> Waiting time
 tat --> Turn around time
 ct --> Completion time
 remt --> Remaining time
 */

#include <stdio.h>
#include <stdlib.h>

typedef struct P{
    int pid;
    int bt;
    int at;
    int wt;
    int tat;
    int ct;
    int remt;
}Process;

float calcAvgWaitingTime(Process *, int);
float calcAvgTurnAroundTime(Process *, int);

void swap(Process *a, Process *b){
    Process temp = *a;
    *a = *b;
    *b = temp;
}

int minimumAvailabeIndex(Process * pr, int available[], int size){
    //if available array has only one value just return the index
    if(size==1)return available[0];

    //return the process which has least BT as remt
    int m = available[0];
    int minBT = (pr+available[0])->remt;

    for(int i=1;i<size;i++){
        if(minBT>(pr+available[i])->remt){
            minBT = (pr+available[i])->remt;
            m = available[i];
        }
    }
    return m;
}

void SortProcessArray(Process * pr, int start, int end, int based){
    //Insertion sort
    //start indicates the index where you want to start sorting the array from, considered 0 here
    //end indicates the index where you want to sort the array to
    //based is flag, if 1 then sort based on AT else if 2 then sort based on PID here

    switch(based){
        case 1:
            for(int i=start+1;i<end;i++){
                Process key = *(pr+i);
                int j=i;
                for(;j>start&&(pr+j-1)->at>key.at;j--)
                    swap((pr+j), (pr+j-1));
                swap(pr+j, &key);
            }
        break;
        case 2:
            for(int i=start+1;i<end;i++){
                Process key = *(pr+i);
                int j=i;
                for(;j>start&&(pr+j-1)->pid>key.pid;j--)
                    swap(pr+j, pr+j-1);
                swap(pr+j, &key);
            }
        break;
        default:
            printf("please tell on which criteria you want to sort the process array (1 or 2)");
            return;
        break;
    }
}

int execute(Process * p, int gtc){
    p->ct = gtc;
    p->remt = p->remt-1;
    return p->remt;
}

void executeProcess(Process * pr, int n){

    //first declare an temporary array which holds all elements of main pr array
    //we will perform every operation on this array
    Process * tempr= (Process *)malloc(n*sizeof(Process));

    for(int i=0;i<n;i++)
        *(tempr+i) = *(pr+i);


    //NOTE: We are sorting the arrays just for the convenience and improve effeciency
    //we can move further without sorting as well

    //first sort the entire array according to arrival time
    SortProcessArray(tempr, 0, n, 1);


    //here we use counter to track all processes are fully executed,
    //counter will increment only when a process fully executes, means till its remaining time becomes 0
    int counter = 0;

    //gtc is another counter variable which is used similarly as gantt chart final value to track completion time of each process
    //initially set gtc to arrival time of first process in sorted array (as per arrival time)

    //since we have sorted the array so we assigned the gtc to the AT of first process
    //if we had not sorted the array then we should've selected the AT of the process having least BT

    int gtc=tempr->at;

    //run a loop till all proccesses gets executed
    while(counter<n){

        //now check which processes come under the gtc based on their arrival time
        //if yes then insert that process index only into available array

        int available[n];
        int availsize=0;
        int availableIndex=-1;
        for(int i=0;i<n;i++){
            //skip the process which is already complete
            if((tempr+i)->remt==0)
                continue;

            //insert the available process in availale array
            if((tempr+i)->at<=gtc){
                available[++availableIndex]=i;
                availsize++;
            }
        }

        //it is crucial to pre increment the gtc at this point
        //because it will cover all process that come after significant gap of completion of the last process available

        ++gtc;

        if(availsize>0){
            //now find the process which has minimum BT from available array
            // m represents the process index for tempr array which has least BT in available array
            //it is returned with the index value of minimum BT process that were stored in available array
            int m = minimumAvailabeIndex(tempr, available, availsize);

            //when the function returns 0 means the process BT value (remaining time) has become 0..means completed
            if(execute(tempr+m,gtc)==0){
                counter++;

                //also calculate WT and TAT of this completed process at this point:
                (tempr+m)->tat = (tempr+m)->ct-(tempr+m)->at;
                (tempr+m)->wt = (tempr+m)->tat-(tempr+m)->bt;

            }
        }
    }

        printf("\nPID\tBT\tAT\tWT\tTAT\tCT\n");
        for(int i=0;i<n;i++)
            printf(" %d\t%d\t%d\t%d\t%d\t%d\n", (tempr+i)->pid, (tempr+i)->bt, (tempr+i)->at, (tempr+i)->wt, (tempr+i)->tat, (tempr+i)->ct);


        printf("\nAverage Wait time = %.2f\n", calcAvgWaitingTime(tempr, n));
        printf("Average Turn Around time = %.2f\n", calcAvgTurnAroundTime(tempr, n));


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


int main()
{
    int n;
    printf("Enter the number of process:");
    scanf("%d", &n);

    if (n<=1){
        printf("Please enter atleast 2 processes!");
        return 0;
    }

        Process * pr = (Process *)malloc(n*sizeof(Process));

    //Enter process id, burst time and then arrival time for each process separated by space, line by line

    printf("Enter PID, BT, AT for each process separated by space, line by line:\n");
    for(int i=0;i<n;i++){

     scanf("%d%d%d", &(pr+i)->pid, &(pr+i)->bt, &(pr+i)->at);
     (pr+i)->remt = (pr+i)->bt;

    }

    executeProcess(pr,n);



//    printf("\nAverage Wait time = %.2f\n", calcAvgWaitingTime(pr, n));
//    printf("Average Turn Around time = %.2f\n", calcAvgTurnAroundTime(pr, n));

    return 0;
}
