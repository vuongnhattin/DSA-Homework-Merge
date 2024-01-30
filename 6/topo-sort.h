#pragma once
#include <string>
#include <fstream>

#pragma once
struct LEADER;
struct TRAILER
{
    LEADER* id; //work can be done after the leader's work is finished
    TRAILER* next;
};

struct LEADER 
{
    int key; //current work
    int degIn; //in degree - number of works that is before the current one
    LEADER* next;      
    TRAILER* thead; //list of works that is behind the current one
};

struct LLIST //leader list
{
    LEADER* head;
    LEADER* tail;
};

TRAILER* createTrailer();
LEADER* createLeader(int key);
LEADER* addLeader(LLIST& list, int key, int& listSize);
void delTrailer(LEADER*& leader);
void delListSystem(LLIST& list);
int getListSystem(LLIST& list, std::string str);
void printLeader(LLIST list);

std::ifstream openFile(const char path[]);
bool getPair(std::string& str, int& x, int& y);
bool topoSort(LLIST& list, LLIST& sortList, int listSize, int& sortListSize);