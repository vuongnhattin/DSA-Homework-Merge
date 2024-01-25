#include <iostream>
#include "circularLL.h"

void removeAll(List* &L) //delete all the elements in circular linked list
{
    if (!L) //circular linked list does not exist
        return;

    if (!L->p_head) //circular linked list contains no element
        return;

    NODE* node = L->p_head; //need-to-be-deleted node
    do
    {
        NODE* next = node->p_next;
        delete node;
        node = next;
    } while (node != L->p_head);
}

NODE* createNode(int data)
{
    NODE* node = new NODE;
    if (!node) //lỗi
        return nullptr;
    node->key = data;
    node->p_next = nullptr;
    return node;
}

NODE* getLastNode(List* &L)
{
    NODE* lastNode = L->p_head;
    for (; lastNode->p_next != L->p_head; lastNode = lastNode->p_next); //find the last node in circular linked list
    return lastNode;
}

bool addHead(List* &L, int data)
{
    NODE* newNode = createNode(data); //create new node
    if (!newNode) //error
        return false;

    if (!L) //circular linked list does not exist
        return false; 

    if (!L->p_head) //circular linked list contains no element
    {
        L->p_head = newNode;
        newNode->p_next = newNode;
        return true; 
    }

    NODE* lastNode = getLastNode(L);
    lastNode->p_next = newNode; //last node points to the new one
    newNode->p_next = L->p_head; //new node points to current head
    L->p_head = newNode; //update linked list head
    return true;
}

bool addTail(List* &L, int data)
{
    NODE* newNode = createNode(data); //create new node
    if (!newNode) //lỗi
        return false;

    if (!L) //circular linked list does not exist
        return false; 

    if (!L->p_head) //circular linked list contains no element
    {
        L->p_head = newNode;
        newNode->p_next = newNode;
        return true; 
    }

    NODE* lastNode = getLastNode(L);
    lastNode->p_next = newNode; //last node points to the new one
    newNode->p_next = L->p_head; //new node points to current head
    return true;
}

void printList(List* L)
{
    if (!L)
    {
        std::cout << "Circular link list is not available!\n";
        return;
    }

    if (!L->p_head)
    {
        std::cout << "There is no element in circular link list.\n";
        return;
    }

    std::cout << "Elements in circular link list: ";
    NODE* node = L->p_head;
    for (; node->p_next != L->p_head; node = node->p_next)
        std::cout << node->key << " ";
    std::cout << node->key << "\n"; //print out the last element in circular linked list
}

void removeHead(List* &L)
{
    if (!L || !L->p_head) 
        return;

    bool isOneNode = false; //variable used to check if list has more than one element
    if (L->p_head == L->p_head->p_next)
        isOneNode = true;

    NODE* lastNode = getLastNode(L);
    NODE* delNode = L->p_head; //need-to-be-deleted node
    lastNode->p_next = L->p_head->p_next; 
    L->p_head = L->p_head->p_next; //update linked list head
    
    delete delNode;
    if (isOneNode)
        L->p_head = nullptr; //update linked list head
}

void removeTail(List* &L)
{
    if (!L || !L->p_head) 
        return;

    bool isOneNode = false; //variable used to check if list has more than one element
    if (L->p_head == L->p_head->p_next)
        isOneNode = true;

    //đảm bảo đúng với danh sách liên kết có một node
    NODE* prevLastNode = L->p_head; //node points to the last node
    for (; prevLastNode->p_next->p_next != L->p_head; prevLastNode = prevLastNode->p_next); //find the last node in the linked list
    NODE* delNode = prevLastNode->p_next; //need-to-be-deleted node
    prevLastNode->p_next = L->p_head;

    delete delNode;
    if (isOneNode)
        L->p_head = nullptr; //update linked list head
}

NODE* findNodeVal(List* &L, int val)
{
    if (!L || !L->p_head)
        return nullptr;

    NODE* node = L->p_head;
    do 
    {
        node = node->p_next;
        if (node->key == val)
            return node;
    } while (node != L->p_head);
    return nullptr;
}

bool addAfter(List* &L, int data, int val) //add a node with "data" value after the first node with "val" value
{
    if (!L || !L->p_head)
        return false; //adding unsuccessfully
    
    NODE* nodeVal = findNodeVal(L, val);
    if (!nodeVal) //there is no node with "val" value
        return false;
    
    //the node with "val" value has been found
    NODE* newNode = createNode(data);
    newNode->p_next = nodeVal->p_next;
    nodeVal->p_next = newNode;
    return true;
}

void removeAfter(List* &L, int val)
{
    if (!L || !L->p_head)
        return;
    
    NODE* nodeVal = findNodeVal(L, val);
    if (!nodeVal) //there is no node with "val" value
        return;

    //the node with "val" value has been found
    //3 cases:
    // - Ordinary.
    // - Linked list with one node -> assign head to nullptr
    // - Deleted node is head -> update head as head is the next one
    bool isOneNode = false; //variable used to check if list has more than one element
    if (L->p_head == L->p_head->p_next)
        isOneNode = true;

    NODE* delNode = nodeVal->p_next;
    nodeVal->p_next = nodeVal->p_next->p_next;

    if (L->p_head == delNode) //deleted node is head
        L->p_head = L->p_head->p_next;

    if (isOneNode) //linked list with one node
        L->p_head = nullptr;
    delete delNode;
}