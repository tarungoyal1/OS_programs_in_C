#include <stdio.h>
#include <stdlib.h>


int main()
{
    int framesize, page=0, pagehits=0, pagefaults=0, totalpages=0, flag=0;
    float phratio=0, pfratio=0;
    printf("Enter the number of frames in RAM:");
    scanf("%d", &framesize);

    int * frame = (int *)malloc(framesize*sizeof(int));
    int frameindex = 0;

    printf("!--Enter page=0 to exit the loop--!\n");

    while(1){
        printf("Enter the new page:");
        scanf("%d", &page);

        if(page==0)break;

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
        ++totalpages;
        if(!flag){
            //means page fault occurred, insert the page in frame
            ++pagefaults;
            *(frame+frameindex++) = page;
            if(frameindex==framesize)frameindex=0;

        }else{
            //means it was page hit, reset flag=0 and continue
            flag=0;
            continue;
        }

    }

    printf("\nTotal pagehits = %d\n", pagehits);
    printf("Total pagefaults = %d\n", pagefaults);

    phratio = (float) pagehits/totalpages;
    pfratio = (float) pagefaults/totalpages;

    printf("Page Hit Ratio = %f\n", phratio);
    printf("Page Fault Ratio= %f\n", pfratio);



    return 0;
}
