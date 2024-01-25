#include <iostream>
#include "topo-sort.h"

int main()
{   
    LLIST list = {nullptr, nullptr};

    std::ifstream f = openFile("input.txt");
    std::string str; 
    f >> str; //lấy xâu ký tự

    //create leader and trailer system from the data file and return the numbmer of leaders
    int listSize = getListSystem(list, str); 
    if (listSize == -1) //error occurs
    {
        delListSystem(list);
        return -1;
    }

    //topo sorts
    LLIST sortList = {nullptr, nullptr}; //leader linked list which are topo sorted
    int sortListSize = 0; //number of elements that are put into topo sorted list
    topoSort(list, sortList, listSize, sortListSize);
    printLeader(sortList);

    delListSystem(list);
    f.close();
    return 0;
}

TRAILER* createTrailer()
{
    TRAILER* trailer = new TRAILER;
    if (!trailer)
    {
        std::cerr << "Cannot allocate memory!";
        return nullptr;
    }
    trailer->id = nullptr;
    trailer->next = nullptr;
    return trailer;
}

LEADER* createLeader(int key)
{
    LEADER* leader = new LEADER;
    if (!leader)
        return nullptr;
    leader->key = key;
    leader->degIn = 0;
    leader->next = nullptr;
    leader->thead = nullptr;
    return leader;
}

LEADER* addLeader(LLIST& list, int key, int& listSize)
{
    list.tail->key = key; //tail always pointing to the sentinel 
    LEADER* leader = list.head;
    while (leader->key != key) //find the leader with the given key
        leader = leader->next;
     
    //if found leader is sentinel -> create a new one
    if (leader == list.tail)
    {
        LEADER* newLeader = createLeader(-1); //create a new sentinel
        leader->next = newLeader;
        leader->degIn = 0;
        list.tail = newLeader; //update tail
        listSize++;
    }
    return leader;
}

void delTrailer(LEADER*& leader)
{
    if (!leader->thead)
        return;
    TRAILER* trailer = leader->thead;
    do
    {
        TRAILER* delTrailer = trailer;
        trailer = trailer->next;
        delete trailer;
    } while (trailer);
}

void delListSystem(LLIST& list) //delete trailer and leader system
{
    if (!list.head)
        return; 

    LEADER* leader = list.head;
    do
    {
        LEADER* delLeader = leader;
        leader = leader->next;
        delete delLeader;
    } while (leader);
}

int getListSystem(LLIST& list, std::string str)
{
    int x, y;
    int listSize = 0;
    list.head = list.tail = createLeader(-1); //create a sentinel
    if (!list.head)
        return -1;
    while (getPair(str, x, y))
    {
        //add x and y into leader sentinel (if any of them havealready existed, simply return but not add new ones into the system)
        //and return the pointer to corresponding leaders
        LEADER* xLeader = addLeader(list, x, listSize); 
        LEADER* yLeader = addLeader(list, y, listSize);
        if (!xLeader || !yLeader)
            return -1;
        
        TRAILER* trailer = createTrailer();
        if (!trailer)
            return -1;
        trailer->id = yLeader; //connect x trailer into y leader 
        //addHead leader
        trailer->next = xLeader->thead; 
        xLeader->thead = trailer;
        //tăng bậc vào của leader y
        yLeader->degIn++;
    }
    return listSize;
}

void printLeader(LLIST list)
{
    for (LEADER* leader = list.head; leader; leader = leader->next)
        std::cout << leader->key << " ";
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

bool getPair(std::string& str, int& x, int& y) //extract a pair of x y -> if success then return true, else false
{
    if (str.empty())
        return false;
    str.erase(str.begin()); //delete "("
    int cmPos = str.find(",", 0); //position of the first "," 
    std::string xStr = str.substr(0, cmPos); //take the x 
    str.erase(0, cmPos + 1); //delete x and ","

    int clBrPos = str.find(")", 0); //postition of the first ")" 
    std::string yStr = str.substr(0, clBrPos); //take the y
    str.erase(0, clBrPos + 1); //delete y and ")"

    x = std::stoi(xStr);
    y = std::stoi(yStr);
    return true;
}


bool topoSort(LLIST& list, LLIST& sortList, int listSize, int& sortListSize) //if error occurs, return false
{
    if (!list.head) 
        return true;

    //find the leader that in degree equals to 0
    LEADER* leader = list.head;
    do
    {
        LEADER* checkLeader = leader;
        leader = leader->next;

        if (!checkLeader->degIn) //check if the in degree equals to 0
        {            
            if (!sortList.head) //if it does not contain any element
                sortList.head = sortList.tail = checkLeader; 
            //addTail the list
            sortList.tail->next = checkLeader;
            checkLeader->next = nullptr;
            sortList.tail = checkLeader;
            sortListSize++;
        }
    }
    while (leader != list.tail);

    //if no node in the graph has 0 in degree -> the given graph is not DAG
    if (!sortList.head)
        return false;
    
    //start topo sorting
    leader = sortList.head;
    while (leader)
    {
        for (TRAILER* trailer = leader->thead; trailer; trailer = trailer->next) //traversal all the trailers
        {
            //delete edge [leader --> (leader->id)]
            LEADER* delEdgeLeader = trailer->id; //delete an edge 
            delEdgeLeader->degIn--; //decrese in degree
            if (delEdgeLeader->degIn == 0) //after deleting, if (leader->id) equals to 0 then put it in topo sorted list by using addTail
            {
                sortList.tail->next = delEdgeLeader;
                delEdgeLeader->next = nullptr;
                sortList.tail = delEdgeLeader;
                sortListSize++;
            } 
        }
        leader = leader->next;
    }
    if (sortListSize < listSize) //the given graph is not DAG
        return false;
    return true;
}

