/*
	Page replacement algo - LRU mode (Least recently used)
	Implementation in C by Tarun Goyal (15scse104026) 2017
	find me at github: https://github.com/tarungoyal1

*/

#include <stdio.h>
#include <stdlib.h>

int getLRUframeindex(int *frame, int allpages[], int framesize, int totalpages){

    /*
       this will contain the value of proximity of page in allpages at the index same as page in framesize
       means if framesize is 3 then

       [
       proximity of page in allpages array at index=0 in frame,
       proximity of page  "  "          "   at index=1 "   ",
       proximity of page  "  "          "   at index=2 "   ",
       ]
       here, least proximity means farthest the page is in allpages
       and this is our page which is to be replaced, return its index in frame array
    */

    int LRUindex[framesize];


    //   we compare each page in current frame with each page in allpages array

    for(int i=0;i<framesize;i++){
        /*
            we start from reverse (totalpages inserted so far) upto the front (0)
            but we will break the inner loop on the very first match
        */
            if(*(frame+i)==-1)return i;

            for(int j=totalpages;j>-1;j--){
                if(*(frame+i)==allpages[j]){
                    LRUindex[i] = j;
                    break;
                }
        }
    }

    //return the index on which minimum proximity is present
    int minIndex = 0;
    int minVal = LRUindex[0];
    for(int i=1;i<framesize;i++){
        if(minVal>LRUindex[i]){
            minVal=LRUindex[i];
            minIndex = i;
        }
    }
    return minIndex;
}

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

    /*we will insert each (page using totalpages counter) in this array for both cases (page hit + page fault) to
    make it act as page string and find the farthest used page in the array
    */
    int allpages[100];

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

            /*here the tricky part is how to find the frameindex which corresponds to the page
            which is LRU we have enclosed the logic of the LRU frameindex in getLRUframeindex
            */
            frameindex = getLRUframeindex(frame, allpages, framesize, totalpages);
            *(frame+frameindex) = page;

             //this is optional,only do this if you want to print the frame at every page operation
            printf("---frame current snapshot---\n");
            for(int i=0;i<framesize;i++)
                printf("%d\n", *(frame+i));

        }else
            //means it was page hit, reset flag=0
            flag=0;


        //increase the total pages operation and insert the page in allpages array no matter what
        allpages[totalpages++] = page;
    }

    printf("\nTotal pagehits = %d\n", pagehits);
    printf("Total pagefaults = %d\n", pagefaults);

    phratio = (float) pagehits/totalpages;
    pfratio = (float) pagefaults/totalpages;

    printf("Page Hit Ratio = %f\n", phratio);
    printf("Page Fault Ratio= %f\n", pfratio);

    return 0;
}
