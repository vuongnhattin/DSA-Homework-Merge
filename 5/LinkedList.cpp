#include <iostream>
#include <string>

using namespace std;

// Structure representing an object with an ID, order, and priority
struct Object {
    string id;
    int order;
    int priority;
};

// Structure representing a node in a singly linked list
struct Node {
    Object key;
    Node* next;
};

// Structure representing a priority queue implemented using a linked list
struct PriorityQueueList {
    Node* front;  // Pointer to the front (head) of the linked list

    // Constructor to initialize an empty priority queue
    PriorityQueueList() : front(nullptr) {}
};

// Function to check if the priority queue is empty
bool isEmpty(const PriorityQueueList& pq) {
    return pq.front == nullptr;
}

// Function to insert an object into the priority queue based on its priority
void insert(PriorityQueueList& pq, const Object& obj) {
    Node* newNode = new Node{obj, nullptr};

    if (isEmpty(pq)) {
        // If the priority queue is empty, insert the new node at the front
        pq.front = newNode;
        return;
    }

    // Find the correct position to insert based on priority
    Node* current = pq.front;
    Node* prev = nullptr;
    while (current != nullptr && current->key.priority < obj.priority) {
        prev = current;
        current = current->next;
    }

    // Insert the new node
    if (prev == nullptr) {
        // Insert at the beginning
        newNode->next = pq.front;
        pq.front = newNode;
    } 
    else {
        newNode->next = current;
        prev->next = newNode;
    }
}

// Function to extract the object with the highest priority from the priority queue
Object extract(PriorityQueueList& pq) {
    if (isEmpty(pq)) {
        cerr << "Error: Priority queue is empty." << endl;
        // Handle error accordingly, here we just return an Object with an empty string.
        return Object{"", 0, 0};
    }

    // Extract the object from the front of the linked list
    Object result = pq.front->key;
    Node* temp = pq.front;
    pq.front = pq.front->next;

    // Deallocate memory for the extracted node
    delete temp;
    return result;
}

// Function to remove an object with a given ID from the priority queue
void remove(PriorityQueueList& pq, const string& objectId) {
    if (isEmpty(pq)) {
        cerr << "Error: Priority queue is empty." << endl;
        return;
    }

    Node* current = pq.front;
    Node* prev = nullptr;

    // Search for the node with the specified objectId
    while (current != nullptr && current->key.id != objectId) {
        prev = current;
        current = current->next;
    }

    if (current == nullptr) {
        cerr << "Error: Object with id " << objectId << " not found." << endl;
        return;
    }

    // Remove the found node
    if (prev == nullptr) {
        // Removing from the front
        pq.front = current->next;
    } 
    else {
        prev->next = current->next;
    }

    delete current;
}

// Function to change the priority of an object with a given ID
void changePriority(PriorityQueueList& pq, const string& objectId, int newPriority) {
    if (isEmpty(pq)) {
        cerr << "Error: Priority queue is empty." << endl;
        return;
    }

    Node* current = pq.front;

    // Search for the node with the specified objectId
    while (current != nullptr && current->key.id != objectId) {
        current = current->next;
    }

    if (current == nullptr) {
        cerr << "Error: Object with id " << objectId << " not found." << endl;
        return;
    }

    // Update the priority of the found node
    Object newObj = current->key;
    newObj.priority = newPriority;

    // Reorganize the queue based on the updated priority
    remove(pq, objectId);
    insert(pq, newObj);
}

// Main function demonstrating the usage of the priority queue
int main() {
    // Example usage
    PriorityQueueList priorityQueue;

    insert(priorityQueue, {"A", 1, 10});
    insert(priorityQueue, {"B", 2, 5});
    insert(priorityQueue, {"C", 3, 8});
    insert(priorityQueue, {"D", 4, 8});

    changePriority(priorityQueue, "B", 1);
    remove(priorityQueue, "A");

    cout << "Priority queue content:" << endl;
    while (!isEmpty(priorityQueue)) {
        Object obj = extract(priorityQueue);
        cout << "Object ID: " << obj.id << ", Priority: " << obj.priority << endl;
    }

    return 0;
}
