#include <iostream>
#include <fstream>
#include <math.h>
#include "radix-sort.h"

int main()
{
    TLIST tlist = {nullptr};
    std::ifstream f = openFile("input.txt");
    int k, n;
    f >> k >> n;
    for (int i = 0; i < n; i++) //create trailer linked list
    {
        int number; 
        f >> number;
        addHeadTrailer(tlist, number);
    }
    radixSort(tlist, k);


    for (TRAILER* trailer = tlist.head; trailer; trailer=trailer->next)
        std::cout << trailer->number << " ";
    delTrailer(tlist);
    return 0;
}

TRAILER* createTrailer(int number) //create trailer contains real number
{
    TRAILER* trailer = new TRAILER;
    if (!trailer)
    {
        std::cerr << "Cannot allocate memory\n";
        return nullptr;
    }
    trailer->next = nullptr;
    trailer->number = number;
    return trailer;
}

LEADER* createLeader(int digit) //create leader contains real number
{
    LEADER* leader = new LEADER;
    if (!leader)
    {
        std::cerr << "Cannot allocate memory\n";
        return nullptr;
    }
    leader->digit = digit;
    leader->next = nullptr;
    leader->trailerHead = nullptr;
    leader->trailerTail = nullptr;
    return leader;
}

void addHeadTrailer(TLIST& tlist, int number)
{
    TRAILER* newTrailer = createTrailer(number); 
    if (!newTrailer)
        return;

    if (tlist.head)
        newTrailer->next = tlist.head;
    tlist.head = newTrailer;
}

void delTrailer(TLIST& tlist) //delete trailer linked list
{
    TRAILER* delTrailer;
    while (tlist.head)
    {
        delTrailer = tlist.head;
        tlist.head = tlist.head->next;
        delete delTrailer;
    }
}

void delLeader(LLIST& llist) //delete leader linked list
{
    LEADER* delLeader;
    while (llist.head)
    {
        delLeader = llist.head;
        llist.head = llist.head->next;
        delete delLeader;
    }
}

LEADER* getLeader(LLIST& llist, int digit)
{
    if (!llist.head) //no element in leader linked list -> create the first one
    {
        LEADER* newLeader = createLeader(digit);
        if (!newLeader)
            return nullptr;
        llist.head = newLeader;
        return llist.head;
    }

    //find the leader with the given digit
    LEADER* fakeHead = createLeader(-1); //create a fake head pointing to real head
    fakeHead->next = llist.head;
    LEADER* leader = fakeHead;
    while (leader->next && (leader->next)->digit < digit) //find the last element
                                                          //or the element with digits that is greater or is equals to the given digits
            leader = leader->next;
    //if the next element is nullptr or digits which is greater than the given ones 
    //-> the leader with the given digits does not exist
    //-> create new leader
    if (!leader->next || (leader->next)->digit > digit)
    {
        LEADER* newLeader = createLeader(digit);
        newLeader->next = leader->next;
        if (leader == fakeHead)
            llist.head = newLeader;
        else
            leader->next = newLeader;
        return newLeader;
    }

    //last case: the next element is the digit which is equals to the given ones 
    //-> the leader with the given digits does exist
    //-> return the next leader 
    return leader->next;
}

std::ifstream openFile(const char path[])
{   
    std::ifstream f;
    f.open(path);
    if (!f) 
    {
        std::cerr << "Cannot open file!\n";
        exit(-1);
    }
    return f;
}

int getDigit(int number, int k, int i) //divide the number into parts: each one has k digits
                                       //take the i part from right to left
{
    return (number / (int)pow(10, k * i)) % (int)pow(10, k);
}

void radixSort(TLIST& tlist, int k)
{
    if (!tlist.head)
        return;

    int max = tlist.head->number;
    for (TRAILER* trailer = tlist.head; trailer; trailer = trailer->next) 
        if (max < trailer->number)
            max = trailer->number; //find the largest numbet in trailer linked list
    int times = ceil(ceil(log10(max)) / k); //calculate the number of loops 
                                            //use the largest number to calculate
                                            //formula: (number of digits of the largest number) / (number of digits of each part)    
    for (int i = 0; i < times; i++)
    {
        LLIST llist = {nullptr};
        TRAILER* trailer = tlist.head;
        do
        {
            int digit = getDigit(trailer->number, k, i);
            LEADER* leader = getLeader(llist, digit); //return the leader holding the digit 
                                                      //if it does not exist, create a new one and return
            
            //addTail trailer to keep stable ability of radix sort
            TRAILER* curTrailer = trailer; //save the current traielr
            trailer = trailer->next;

            ////if current leader does not have any trailer -> add and update trailerHead and trailerTail
            if (!leader->trailerHead)
            {
                leader->trailerHead = leader->trailerTail = curTrailer;
                curTrailer->next = nullptr;
                continue;
            }
            ////if it does
            curTrailer->next = nullptr;
            leader->trailerTail = (leader->trailerTail)->next = curTrailer;
        } while (trailer);

        //after putting trailers into corresponding leaders (and the leaders are sorted in ascending order)
        //-> connect trailers into one linked list
        tlist.head = (llist.head)->trailerHead;
        for (LEADER* leader = llist.head; leader->next; leader = leader->next)
            (leader->trailerTail)->next = (leader->next)->trailerHead;
            
        delLeader(llist); //after connecting phase -> delete leader system
    }
}
