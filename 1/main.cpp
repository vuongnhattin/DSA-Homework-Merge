#include <iostream>
using namespace std;

// Global variable to keep track of the number of iterations in the interpolation search
int cnt = 0;

// Function to perform interpolation search on a sorted array
// Returns the index of the element if found, otherwise -1
int interpolationSearch(int arr[], int n, int x) {
    int low = 0, high = (n - 1);

    // Perform interpolation search as long as the search space is valid
    while (low <= high && x >= arr[low] && x <= arr[high]) {
        cnt++; // Increment the iteration count

        if (low == high) {
            // If there's only one element left in the search space
            if (arr[low] == x) return low; // Element found at the single remaining position
            return -1; // Element not found
        }

        // Calculate the position using interpolation formula
        int pos = low + (((double)(high - low) / (arr[high] - arr[low])) * (x - arr[low]));

        if (arr[pos] == x)
            return pos; // Element found at the calculated position

        if (arr[pos] < x)
            low = pos + 1; // Adjust the search space to the right
        else
            high = pos - 1; // Adjust the search space to the left
    }

    return -1; // Element not found
}

// Main function
int main(int argc, char** argv) {
    // Retrieve the target value from the command line argument
    int x = atoi(argv[1]);

    int a[100], idx = 0;

    // Populate the array with values from command line arguments
    for (int i = 2; i < argc; i++) {
        a[idx++] = atoi(argv[i]);
    }

    // Perform interpolation search and print the result along with the iteration count
    cout << interpolationSearch(a, idx, x) << " - " << cnt << "\n";

    return 0;
}
