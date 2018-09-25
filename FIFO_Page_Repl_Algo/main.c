/*
	Page replacement algo - FIFO mode
	Implementation in C by Tarun Goyal (15scse104026) 2017
	find me at github: https://github.com/tarungoyal1

*/



#include <stdio.h>
#include <stdlib.h>


int main()
{
    int framesize, page=0, pagehits=0, pagefaults=0, totalpages=0, flag=0;
    float phratio=0, pfratio=0;
    printf("Enter the frame size in RAM:");
    scanf("%d", &framesize);

    int * frame = (int *)malloc(framesize*sizeof(int));

    for(int i=0;i<framesize;i++)
        *(frame+i) = -1;

    // -1 means no page is inserted initially

    //this is the index/counter which will be used to track where the next page will be inserted as per FIFO rule
    int frameindex = 0;

    printf("!--Enter page=-1 to exit the loop--!\n");

    while(1){
        printf("Enter the new page:");
        scanf("%d", &page);

        if(page==-1)break;

        //First check if page is already present or not in frame
        // if it's then it's a PAGE HIT
        //else it's a PAGE FAULT
        for(int i=0;i<framesize;i++){
            if(*(frame+i)==page){
                ++pagehits;
                printf("PAGE HIT for page=%d\n", page);
                flag=1;
                break;
            }
        }

        if(!flag){
            //means page fault occurred, insert the page in frame
            ++pagefaults;
            *(frame+frameindex++) = page;
            if(frameindex==framesize)frameindex=0;

             //this is optional,only do this if you want to print the frame at every page operation
            printf("---frame current snapshot---\n");
            for(int i=0;i<framesize;i++)
                printf("%d\n", *(frame+i));

        }else
            //means it was page hit, reset flag=0
            flag=0;

        //increase the total pages operation
        ++totalpages;

    }

    printf("\nTotal pagehits = %d\n", pagehits);
    printf("Total pagefaults = %d\n", pagefaults
    );

    phratio = (float) pagehits/totalpages;
    pfratio = (float) pagefaults/totalpages;

    printf("Page Hit Ratio = %f\n", phratio);
    printf("Page Fault Ratio= %f\n", pfratio);



    return 0;
}
