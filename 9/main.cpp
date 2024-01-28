#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <cstring>

// Function to convert an integer to a binary string representation with a specified bit count
std::string intToBin(int input, int bitCount) {
    std::string result;

    for (int i = 0; i < bitCount; i++) {
        result += (input % 2 == 0) ? '0' : '1';
        input /= 2;
    }

    reverse(begin(result), end(result));
    return result;
}

// Function to get the maximum number of bits needed to represent the largest element in a vector
int getMaxBit(const std::vector<int>& a) {
    return log2(*max_element(begin(a), end(a))) + 1;
}

// Function to calculate the compression ratio given the original string and compressed vector
double calculateCompressionRatio(const std::string& original, const std::vector<int>& compressed) {
    double N = original.size() * 8.0; // Assuming 8 bits per character
    double n = compressed.size() * getMaxBit(compressed);
    return (N * 1.0 - n) / N * 100;
}

// Function to compress a string using the LZW algorithm
void compressLZW(const std::string& input) {
    std::unordered_map<std::string, int> dictionary;
    std::vector<int> compressed;
    std::string current;

    // Initialize the dictionary with single-character strings
    for (int i = 0; i < 256; ++i) {
        std::string ch = std::string(1, static_cast<char>(i));
        dictionary[ch] = i;
    }

    int nextCode = 256; // Next available code in the dictionary

    for (char c : input) {
        std::string temp = current + c;
        if (dictionary.find(temp) != dictionary.end()) {
            // If the current + c is in the dictionary, continue building the current sequence
            current = temp;
        } else {
            // Output the code for the current sequence and add current + c to the dictionary
            compressed.push_back(dictionary[current]);
            dictionary[temp] = nextCode++;
            current = std::string(1, c);
        }
    }

    // Output the last code in the dictionary
    if (!current.empty()) {
        compressed.push_back(dictionary[current]);
    }

    // Output the compressed sequence in decimal
    for (int code : compressed) {
        std::cout << code << " ";
    }
    std::cout << std::endl;

    int maxBit = getMaxBit(compressed);

    // Output the compressed sequence in binary
    for (int code : compressed) {
        std::cout << intToBin(code, maxBit) << " "; // Assuming 16-bit codes
    }
    std::cout << std::endl;

    // Calculate and print the compression ratio
    double ratio = calculateCompressionRatio(input, compressed);

    std::cout << "Compression Ratio: " << ratio << std::endl;
}

// Function to decompress a vector of integers using the LZW algorithm
void decompressLZW(const std::vector<int>& compressed) {
    std::unordered_map<int, std::string> dictionary;
    std::string result;

    // Initialize the dictionary with single-character strings
    for (int i = 0; i < 256; ++i) {
        dictionary[i] = std::string(1, static_cast<char>(i));
    }

    int currentCode = compressed[0];
    std::string currentString = dictionary[currentCode];
    result += currentString;

    for (size_t i = 1; i < compressed.size(); ++i) {
        int code = compressed[i];

        // Check if the code is in the dictionary
        if (dictionary.find(code) != dictionary.end()) {
            currentString = dictionary[code];
        } else {
            // Special case for handling the edge of the dictionary
            currentString = dictionary[currentCode] + currentString[0];
        }

        // Append the current string to the result
        result += currentString;

        // Add a new entry to the dictionary
        dictionary[static_cast<int>(dictionary.size())] = dictionary[currentCode] + currentString[0];

        // Update the current code for the next iteration
        currentCode = code;
    }

    std::cout << result << std::endl;
    std::cout << calculateCompressionRatio(result, compressed) << std::endl;
}

// Main function for LZW compression and decompression
int main(int argc, char** argv) {
    if (argc <= 1) return -1;

    // Check if the user wants to compress
    if (strcmp(argv[1], "-c") == 0) { 
        compressLZW(argv[2]);
    }

    // Check if the user wants to decompress
    if (strcmp(argv[1], "-e") == 0) { 
        std::vector<int> input;
        for (int i = 2; i < argc; i++) {
            input.push_back(atoi(argv[i]));
        }

        decompressLZW(input);
    }

    return 0;
}
