#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <sstream>

using namespace std;

// Function to calculate the greatest common divisor (GCD) of two numbers
int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Class for Sparse Table data structure
class SparseTable {
public:
    // Constructor for initializing the SparseTable with an array and operation type
    SparseTable(const vector<int>& arr, const string& operation)
        : operation(operation) {
        buildSparseTable(arr);
    }

    // Query function to perform range queries based on the specified operation type
    int query(int left, int right) {
        if (operation == "MIN") {
            return queryMin(left, right);
        } else if (operation == "MAX") {
            return queryMax(left, right);
        } else if (operation == "GCD") {
            return queryGCD(left, right);
        } else {
            cerr << "Unknown operation: " << operation << endl;
            return 0;
        }
    }

    // Save the SparseTable to a file
    void saveToFile(const string& fileName) {
        ofstream file(fileName);
        file << operation << '\n';
        for (const auto& row : sparseTable) {
            for (int val : row) {
                file << val << ' ';
            }
            file << '\n';
        }
        file.close();
    }

    // Load the SparseTable from a file
    void loadFromFile(const string& fileName) {
        ifstream file(fileName);
        sparseTable.clear();

        // Read the operation and consume the newline character
        file >> operation;
        file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        string line;
        while (getline(file, line)) {
            istringstream iss(line);
            vector<int> row;
            int value;
            while (iss >> value) {
                row.push_back(value);
            }
            sparseTable.push_back(row);
        }

        file.close();
    }

private:
    vector<vector<int>> sparseTable;  // 2D vector to store the SparseTable
    string operation;  // Operation type (MIN, MAX, GCD)

    // Build the SparseTable based on the given array and operation type
    void buildSparseTable(const vector<int>& arr) {
        sparseTable.push_back(arr);

        int n = arr.size();
        for (int j = 1; (1 << j) <= n; j++) {
            for (int i = 0; i + (1 << j) - 1 < n; i++) {
                sparseTable.push_back(vector<int>());
                if (operation == "MIN") {
                    sparseTable[j].push_back(min(sparseTable[j - 1][i], sparseTable[j - 1][i + (1 << (j - 1))]));
                } else if (operation == "MAX") {
                    sparseTable[j].push_back(max(sparseTable[j - 1][i], sparseTable[j - 1][i + (1 << (j - 1))]));
                } else if (operation == "GCD") {
                    sparseTable[j].push_back(gcd(sparseTable[j - 1][i], sparseTable[j - 1][i + (1 << (j - 1))]));
                } else {
                    cerr << "Unknown operation: " << operation << endl;
                }
            }
        }
    }

    // Query function for finding the minimum value in the specified range
    int queryMin(int left, int right) {
        int j = log2(right - left + 1);
        return min(sparseTable[j][left], sparseTable[j][right - (1 << j) + 1]);
    }

    // Query function for finding the maximum value in the specified range
    int queryMax(int left, int right) {
        int j = log2(right - left + 1);
        return max(sparseTable[j][left], sparseTable[j][right - (1 << j) + 1]);
    }

    // Query function for finding the greatest common divisor in the specified range
    int queryGCD(int left, int right) {
        int j = log2(right - left + 1);
        return gcd(sparseTable[j][left], sparseTable[j][right - (1 << j) + 1]);
    }
};

// Main function to execute commands
int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " command [args...]" << endl;
        return 1;
    }

    string command = argv[1];

    if (command == "make") {
        if (argc < 4) {
            cerr << "Usage: " << argv[0] << " make table_name operation values..." << endl;
            return 1;
        }

        string tableName = argv[2];
        string operation = argv[3];

        // Read the current table count
        int tableCount;
        ifstream countFile("count.txt");
        countFile >> tableCount;
        countFile.close();

        if (tableCount >= 5) {
            ifstream checkfile(tableName + ".txt");
            if (!checkfile.good()) {
                cerr << "Maximum table count reached. Cannot create more tables." << endl;
                return 1;
            }
        }

        // Check if the table already exists
        ifstream checkFile(tableName + ".txt");
        if (checkFile.good()) {
            checkFile.close();
            cout << "Table already exists. Replacing..." << endl;
        } else {
            checkFile.close();
            // Increment the table count
            tableCount++;
            ofstream countFileOut("count.txt");
            countFileOut << tableCount;
            countFileOut.close();
        }

        vector<int> values;
        for (int i = 4; i < argc; ++i) {
            values.push_back(stoi(argv[i]));
            cout << stoi(argv[i]) << " ";
        }
        cout << endl;

        // Create an instance of the SparseTable and save it to a file
        SparseTable sparseTable(values, operation);
        sparseTable.saveToFile(tableName + ".txt");
    } else if (command == "query") {
        if (argc != 5) {
            cerr << "Usage: " << argv[0] << " query table_name left right" << endl;
            return 1;
        }

        string tableName = argv[2];
        int left = stoi(argv[3]);
        int right = stoi(argv[4]);

        // Create an empty instance of the SparseTable, load from file, and perform the query
        SparseTable sparseTable({}, "");
        sparseTable.loadFromFile(tableName + ".txt");
        int result = sparseTable.query(left, right);
        cout << "Query result: " << result << endl;
    } else {
        cerr << "Unknown command: " << command << endl;
        return 1;
    }

    return 0;
}
