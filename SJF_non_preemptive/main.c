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
    gtc+=pr->bt;
    execute(pr, gtc);

    //now starting from 1 to n, fill all process that are <= gtc and execute the one that has minimum BT
    //do this for each iteration

    for(int i=1;i<n;i++){
//        Process **available = (Process * )malloc((n-i)*sizeof(Process));
        int available[n-1];
        int avCount = 0;

        //error-prone --->
        for(int j=0;j<n;j++){
            if((pr+i+j)->at<=gtc){
                available[j] = (i+j);
                avCount++;
            }
        }
        //find shortest of available processes
        int min = minimum(pr,available, avCount);
        gtc+= (pr+min)->bt;
        execute((pr+min), gtc);
        swap((pr+min), pr+i);
        //--->
    }
}
void execute(Process *p, int gtc){
    p->ct = gtc;
    p->tat = p->ct - p->at;
    p->wt = p->tat - p->bt;
}

int minimum(Process * pr,int a[], int n){
    int min = (pr+a[0])->bt;
    int j=a[0];
    for(int i=1;i<n;i++){
        if(min>(pr+a[i])->bt){
            min = (pr+a[i])->bt;
            j=a[i];
        }
    }
    return j;
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
                for(;j>0&&(pr+j-1)->at>key.at;j--)
                    swap((pr+j), (pr+j-1));
                swap(pr+j, &key);
            }
        break;
        case 2:
            for(int i=start+1;i<end;i++){
                Process key = *(pr+i);
                int j=i;
                for(;j>0&&(pr+j-1)->bt>key.bt;j--)
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


    printf("\nP#\tBT\tAT\tWT\tTAT\n");
    for(int i=0;i<n;i++)
        printf(" %d\t%d\t%d\t%d\t%d\n", (pr+i)->pid, (pr+i)->bt, (pr+i)->at, (pr+i)->wt, (pr+i)->tat);


//    printf("\nAverage Wait time = %.2f\n", calcAvgWaitingTime(pr, n));
//    printf("Average Turn Around time = %.2f\n", calcAvgTurnAroundTime(pr, n));

    return 0;
}
