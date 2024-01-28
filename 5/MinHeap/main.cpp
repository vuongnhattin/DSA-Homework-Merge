#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Structure representing an object with an ID, order, and priority
struct Object {
    string id;
    int order;
    int priority;
};

// Structure representing a priority queue implemented using a min-heap
struct PriorityQueueHeap {
    vector<Object> arr;
};

// Function to check if the priority queue is empty
bool isEmpty(const PriorityQueueHeap& pq) {
    return pq.arr.empty();
}

// Function to maintain the heap property while moving an element up the heap
void heapifyUp(vector<Object>& arr, int index) {
    while (index > 0) {
        int parentIndex = (index - 1) / 2;
        if (arr[index].priority < arr[parentIndex].priority) {
            swap(arr[index], arr[parentIndex]);
            index = parentIndex;
        } else {
            break;
        }
    }
}

// Function to maintain the heap property while moving an element down the heap
void heapifyDown(vector<Object>& arr, int index) {
    int size = arr.size();
    while (true) {
        int leftChild = 2 * index + 1;
        int rightChild = 2 * index + 2;
        int smallest = index;

        if (leftChild < size && arr[leftChild].priority < arr[smallest].priority) {
            smallest = leftChild;
        }

        if (rightChild < size && arr[rightChild].priority < arr[smallest].priority) {
            smallest = rightChild;
        }

        if (smallest != index) {
            swap(arr[index], arr[smallest]);
            index = smallest;
        } else {
            break;
        }
    }
}

// Function to insert an object into the priority queue
void insert(PriorityQueueHeap& pq, const Object& obj) {
    pq.arr.push_back(obj);
    heapifyUp(pq.arr, pq.arr.size() - 1);
}

// Function to extract the object with the highest priority from the priority queue
Object extract(PriorityQueueHeap& pq) {
    if (isEmpty(pq)) {
        cerr << "Error: Priority queue is empty." << endl;
        // Handle error accordingly, here we just return an Object with an empty string.
        return Object{"", 0, 0};
    }

    Object result = pq.arr[0];
    pq.arr[0] = pq.arr.back();
    pq.arr.pop_back();
    heapifyDown(pq.arr, 0);

    return result;
}

// Function to remove an object with a given ID from the priority queue
void remove(PriorityQueueHeap& pq, const string& objectId) {
    int index = -1;
    for (int i = 0; i < pq.arr.size(); ++i) {
        if (pq.arr[i].id == objectId) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        cerr << "Error: Object with id " << objectId << " not found." << endl;
        return;
    }

    pq.arr[index].priority = INT_MIN; // Set priority to negative infinity
    heapifyUp(pq.arr, index);
    (void)extract(pq);
}

// Function to change the priority of an object with a given ID
void changePriority(PriorityQueueHeap& pq, const string& objectId, int newPriority) {
    int index = -1;
    for (int i = 0; i < pq.arr.size(); ++i) {
        if (pq.arr[i].id == objectId) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        cerr << "Error: Object with id " << objectId << " not found." << endl;
        return;
    }

    int oldPriority = pq.arr[index].priority;
    pq.arr[index].priority = newPriority;

    if (newPriority < oldPriority) {
        heapifyUp(pq.arr, index);
    } else {
        heapifyDown(pq.arr, index);
    }
}

// Main function demonstrating the usage of the priority queue
int main() {
    // Example usage
    PriorityQueueHeap priorityQueue;

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
