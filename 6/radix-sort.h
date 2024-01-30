#pragma once
#include <fstream>
#pragma once
struct TRAILER //a node contains a real number
{
    int number;
    TRAILER* next;
};

struct TLIST //trailer linked list
{
    TRAILER* head;
};

struct LEADER //a node contains digits in order to compare (and they are sorted in ascending order)
{
    int digit;
    LEADER* next;
    TRAILER* trailerHead;
    TRAILER* trailerTail;
};

struct LLIST //leader linked list
{
    LEADER* head;
};

TRAILER* createTrailer(int number);
LEADER* createLeader(int digit);
void addHeadTrailer(TLIST& tlist, int number);
void delTrailer(TLIST& tlist);
void delLeader(LLIST& llist);
LEADER* getLeader(LLIST& llist, int digit);

std::ifstream openFile(const char path[]);
int getDigit(int number, int k, int i);
void radixSort(TLIST& tlist, int k);