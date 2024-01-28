#include <iostream>
#include <vector>
#include <queue>

struct TrieNode {
	int ID;
	TrieNode* next[26];

	TrieNode() : ID(-1) {
		for (int i = 0; i < 26; i++) {
			next[i] = nullptr;
		}
	}
};

void Insert(TrieNode*& root, const std::string& word, int id) {
    TrieNode* current = root;

    for (char ch : word) {
        int index = ch - 'a';

        if (!current->next[index]) {
            current->next[index] = new TrieNode();
        }

        current = current->next[index];
    }

    // Assign the ID to the last node representing the complete key
    current->ID = id;
}

void createTree(TrieNode*& root, const std::string& dicFile) {
	FILE* file = fopen(dicFile.c_str(), "r");
	if (!file) {
		std::cout << "Cannot open file!";
		return;
	}

	char word[100]; int id;
	while (fscanf(file, "%d. %s\n", &id, word) != EOF) {
		Insert(root, word, id);
	}
}

int lookUp(TrieNode* root, const std::string& word) {
    TrieNode* current = root;

    for (char ch : word) {
        int index = ch - 'a';

        if (current->next[index] == nullptr) {
            // The word is not in the Trie
            return -1;
        }

        current = current->next[index];
    }

    // Return the ID of the last node representing the complete word
    return current->ID;
}

void getAllWords(TrieNode* node, const std::string& currentPrefix, std::vector<std::string>& result) {
    if (!node) {
        return;
    }

    if (node->ID != -1) {
        // If the current node represents a complete word, add it to the result
        result.push_back(currentPrefix);
    }

    // Recursively traverse the Trie
    for (int i = 0; i < 26; ++i) {
        if (node->next[i]) {
            char ch = 'a' + i;
            getAllWords(node->next[i], currentPrefix + ch, result);
        }
    }
}

std::vector<std::string> lookUpPrefix(TrieNode* root, const std::string& prefix) {
    std::vector<std::string> result;

    TrieNode* current = root;

    // Traverse the Trie until the end of the prefix
    for (char ch : prefix) {
        int index = ch - 'a';

        if (current->next[index] == nullptr) {
            // The prefix is not in the Trie
            return result;
        }

        current = current->next[index];
    }

    getAllWords(current, prefix, result);

    return result;
}

bool hasNoChildren(TrieNode* node) {
    for (int i = 0; i < 26; ++i) {
        if (node->next[i] != nullptr) {
            return false;
        }
    }
    return true;
}

// Helper function to remove a word from the Trie
TrieNode* removeHelper(TrieNode* node, const std::string& word, int depth) {
    if (node == nullptr) {
        return nullptr;
    }

    // If end of the word is reached
    if (depth == word.size()) {
        // If the current node represents the end of a word, remove it
        if (node->ID != -1) {
            node->ID = -1;
        }

        // If the current node has no children, it can be safely removed
        if (hasNoChildren(node)) {
            delete node;
            return nullptr;
        }

        return node;
    }

    // Recursively remove the rest of the word
    int index = word[depth] - 'a';
    node->next[index] = removeHelper(node->next[index], word, depth + 1);

    // If the current node has no children and does not represent the end of a word, it can be safely removed
    if (hasNoChildren(node) && node->ID == -1) {
        delete node;
        return nullptr;
    }

    return node;
}

void Remove(TrieNode* root, const std::string& word) {
    removeHelper(root, word, 0);
}

int main() {
	TrieNode* root = new TrieNode();
	createTree(root, "HW6-dic.txt");
	//std::cout << lookUp(root, "zumatic");
	/*std::vector<std::string> result = lookUpPrefix(root, "zu");
	for (auto s : result) std::cout << s << "\n";*/
    Remove(root, "aaa");
    std::cout << lookUp(root, "aaa");
}

