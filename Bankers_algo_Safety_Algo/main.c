/*-
    Banker's Algo - Safety Algorithm in C
    Implementation by Tarun Goyal (15scse104026) in (2017)
    find me at github: https://github.com/tarungoyal1

*/

/*
    m-> is the number of resource type
    n-> is the number of processes

    available -> is the vector(1d array) of length m indicates number of available resources of each type
    allocation -> is the vector(1d arrray) for each process which indicates number of resources already allocated to that process
    max -> is a vector defines the maximum demand that each process can make of the given resources
    need --> is also a vector(1d array) for each process indicates number of resources required by process to complete execution
    finish -> is just a flag variable that indicates process is finished executing
*/

#include <stdio.h>
#include <stdlib.h>

typedef struct P{
    int pid;
    int * allocation;
    int * max;
    int * need;
    int finish;
}Process;

int n,m;

void printMatrixTable(Process *pr){
     printf("\nPID\tAlloc\tMAX\tNeed\n");
    for(int i=0;i<n;i++){
        printf("P%d\t",i);
        for(int j=0;j<m;j++)
            printf("%d ",*((pr+i)->allocation+j));

        // i could have used just 1 inner loop but i needed to print a tabs after printing entries of each type of matrix
        printf("\t");
         for(int j=0;j<m;j++)
            printf("%d ",*((pr+i)->max+j));

        printf("\t");
         for(int j=0;j<m;j++)
            printf("%d ",*((pr+i)->need+j));
        printf("\n");
    }
}


int main()
{

    //first ask the number of resources
    printf("Enter the number of Resource types:");
    scanf("%d", &m);

    //now ask for the number of the processes

    printf("Enter the number of processes:");
    scanf("%d", &n);


    int * available = (int *)malloc(m*sizeof(int));
    int *work = (int *)malloc(m*sizeof(int));
    printf("Enter the available vector:");


    for(int i=0;i<m;i++){
        scanf("%d", available+i);
        *(work+i) = *(available+i);
    }

    //Now make an array process of size n
    Process * pr = (Process*)malloc(n*sizeof(Process));


    printf("Enter the allocation and max matrix, for each process:\n");
    for(int i=0;i<n;i++){
        (pr+i)->allocation = (int *)malloc(m*sizeof(int));
        (pr+i)->max = (int *)malloc(m*sizeof(int));
        (pr+i)->need = (int *)malloc(m*sizeof(int));

        printf("Enter for P%d:",i);
        for(int j=0;j<m;j++)
            scanf("%d", (((pr+i)->allocation)+j));

        for(int j=0;j<m;j++){
            scanf("%d", (((pr+i)->max)+j));

             //compute the need matrix for ith process here to save another loops
            *((pr+i)->need+j) = *((pr+i)->max+j) - *((pr+i)->allocation+j);
        }
    }

    printMatrixTable(pr);

    // we will insert the finished process in seq[]
    int seq[n];

    //sc is sequence counter that tracks how many processes are finished executed
    int sc = 0;

    //flag indicates whether need is bigger (flag=0) or lesser (flag remains 1)
    //this flag is assigned to finish variable of Process
    int flag=1;


    //Now the main algorithm starts from here

    //i is just counter variable that is used for process index/id
    int i=0;

    //run the loop till sc becomes n means all processes comes in seq[]
    while(sc<n){

        //if the process is already finished executed then continue and check for next process
        if(((pr+i)->finish)==1){
            //if i is not incremented before continue than loop will go infinite
            ++i;
            continue;
        }


        //check the need for ith process
        for(int j=0;j<m;j++){
            if(*((pr+i)->need+j)>*(work+j)){
                //means set value of flag=0 and then this flag is indeed assigned to finish var of this process
                flag=0;
                break;
            }
        }


        (pr+i)->finish=flag;

        //means if process's need vector is lesser than work/available
        if(((pr+i)->finish)==1){

            //now insert this process in sequence array
            seq[sc] = i;
            sc++;

            //update the work = work + allocation(ith)
            for(int j=0;j<m;j++)
              *(work+j) = *(work+j) + *((pr+i)->allocation+j);

        }
//         printf("%d,%d,%d\n", i, (pr+i)->finish, sc);

        // must set the flag to 1 for next process means if need checking loop on line 124 is fully executed
        // then finish will be set to flag=1 for the next ith process.
        flag=1;

        //increment the i counter to check for next process
        ++i;

        //this is very crucial to check otherwise loop will go infinite
        //because, when the last process is checked and there are few processes that are left executing
        //means their need was greater than work at the time of checking,
        //now it will go back to 1st process and repeat the procedure
        if(i==n)i=0;

    }

    //now you can print it in any fashion it's all upto you
    printf("\nSafe Sequence:< P%d", seq[0]);
    for(int i=1;i<n;i++)
        printf(", P%d", seq[i]);
    printf(" >\n");




    return 0;
}
