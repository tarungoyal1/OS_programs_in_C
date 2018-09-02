/* SJF - Non Preemptive program in C
   Made by Tarun Goyal,
   find me on github at https://github.com/tarungoyal1
 */

 /* This is Non-Preemptive SJF and in this program, any process may contain same bt or at

 prId --> ProceesId
 bt --> Burst time
 at --> arrival time
 wt --> Waiting time
 tat --> Turn around time

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
}Process;

void executeProcess(Process * pr, int n){
    //gtc represent the gantt chart trailing value
    int gtc=0;

    //first sort the entire array according to arrival time
    SortProcessArray(pr, 0, n, 1);

    //m contains arrival time of first process which is obviosly minimum
    int m = pr->at;

    //now count the occurrences of processes that have m .. it's done linear fashion starting from 0
    int c = countOccurs(pr, 0, n, m, 1);

//    printf("%d,%d", m, c);

    //now sort the process array from 0 to count based on 2 for BT
    SortProcessArray(pr, 0, c, 2);

    //execute the first process
    gtc=pr->at+pr->bt;
    execute(pr, gtc);

    SortProcessArray(pr, 1, n, 2);

    //now our process array is sorted after the first process means from 2nd element
    //find minimumAvailable not just minimum in terms of BT but also should be available in terms of AT
    for(int i=1;i<n;i++){



       int minIndex = minimumAvailabeIndex(pr, i, n,gtc);
//       printf("%d:%d-->gtc=\n", i,minIndex,gtc);

       //if arrival time of pr at minIndex>=gtc...means CPU remained idle..handle this here
       if((pr+minIndex)->at>gtc)gtc = ((pr+minIndex)->at-gtc)+(pr+minIndex)->bt;
       else gtc+=(pr+minIndex)->bt;

       execute(pr+minIndex, gtc);

       //this is crucial
       prependExecuted(pr, minIndex,i);
    }

}

void execute(Process *p, int gtc){
    p->ct = gtc;
    p->tat = p->ct - p->at;
    p->wt = p->tat - p->bt;
}

void prependExecuted(Process * pr, int minIndex, int start){
    Process temp = *(pr+minIndex);
    for(int i=minIndex;i>start;i--){
        swap(pr+i, pr+i-1);
    }
    *(pr+start) = temp;
}

int minimumAvailabeIndex(Process * pr, int start, int n, int gtc){
    //our array is already sorted as per BT
    for(int i=start;i<n;i++)
        if((pr+i)->at<=gtc)return i;

    //but what if none of the remaining process came under gtc
    //then return the process which has least AT in the remaining processes, that is next to come
    int m = start;
    int minAT = (pr+m)->at;
    for(int i=start+1;i<n;i++){
        if(minAT>(pr+i)->at){
            minAT = (pr+i)->at;
            m = i;
        }
    }
    return m;
}

int countOccurs(Process * pr, int start, int end, int key, int based){
    //count all occurrences in process array from start to end (given) based on 1 for at, 2 for bt
    int c=0;
    switch(based){
        case 1:
            for(int i=start;i<end;i++){
                    if((pr+i)->at==key)++c;
            }
            return c;
        break;
        case 2:
            for(int i=start;i<end;i++){
                    if((pr+i)->bt==key)++c;
            }
            return c;
        break;
        default:
            printf("please tell on which criteria you want to sort the process array (1 or 2)");
        break;
    }
    return c;
}

void SortProcessArray(Process * pr, int start, int end, int based){
    //Insertion sort
    //start indicates the index where you want to start sorting the array from, considered 0 here
    //end indicates the index where you want to sort the array to
    //based is flag, if 1 then sort based on AT else if 2 then sort based on BT

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
                for(;j>start&&(pr+j-1)->bt>key.bt;j--)
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

void swap(Process *a, Process *b){
    Process temp = *a;
    *a = *b;
    *b = temp;
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



int main(){
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
    for(int i=0;i<n;i++)
     scanf("%d%d%d", &(pr+i)->pid, &(pr+i)->bt, &(pr+i)->at);

    executeProcess(pr,n);


    printf("\nP#\tBT\tAT\tWT\tTAT\tCT\n");
    for(int i=0;i<n;i++)
        printf(" %d\t%d\t%d\t%d\t%d\t%d\n", (pr+i)->pid, (pr+i)->bt, (pr+i)->at, (pr+i)->wt, (pr+i)->tat, (pr+i)->ct);


    printf("\nAverage Wait time = %.2f\n", calcAvgWaitingTime(pr, n));
    printf("Average Turn Around time = %.2f\n", calcAvgTurnAroundTime(pr, n));

    return 0;
}
