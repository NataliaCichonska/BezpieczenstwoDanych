#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <vector>
#include <climits>
#include <chrono>

// DJB2 hash function
unsigned long djb2(const std::string& str) {
    unsigned long hash = 5381;
    for (char c : str) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

// TreeNode structure
struct TreeNode {
    unsigned long hash;
    std::vector<std::string> data;
    TreeNode* left;
    TreeNode* right;

    TreeNode(unsigned long h, const std::string& d)
        : hash(h), data({d}), left(nullptr), right(nullptr) {}
};

// Insert function 
TreeNode* insert(TreeNode* root, unsigned long hash, const std::string& data) {
    if (!root) {
        return new TreeNode(hash, data);
    }
    
    if (hash < root->hash) {
        root->left = insert(root->left, hash, data);
    }
    else if (hash > root->hash) {
        root->right = insert(root->right, hash, data);
    }
    else {
        root->data.push_back(data);
    }
    return root;
}

// Add cleanup function at the top
void cleanupTree(TreeNode* root) {
    if (root) {
        cleanupTree(root->left);
        cleanupTree(root->right);
        delete root;
    }
}

int main(int argc, char* argv[]) {
    TreeNode* root = nullptr;
    
    try {
        if (argc < 2 || argc > 3) {
            std::cout << "Usage: " << argv[0] << " <input_file> [save_to_file]" << std::endl;
            return 1;
        }

        std::string inputFileName = argv[1];
        bool shouldSave = true;  // default value
        
        if (argc == 3) {
            shouldSave = (std::string(argv[2]) == "true");
        }

        std::ifstream inFile(inputFileName);
        if (!inFile) {
            std::cout << "Error opening input file!" << std::endl;
            return 1;
        }

        std::ofstream outFile;
        if (shouldSave) {
            size_t dotPos = inputFileName.find_last_of('.');
            std::string baseName = (dotPos != std::string::npos) ? 
                inputFileName.substr(0, dotPos) : inputFileName;
            std::string outFileName = baseName + "_hashed.txt";
            outFile.open(outFileName);
            
            if (!outFile) {
                std::cout << "Error creating output file!" << std::endl;
                cleanupTree(root);
                return 1;
            }
        }

        std::string word;
        while (std::getline(inFile, word)) {
            if (!word.empty()) {
                unsigned long hash = djb2(word);
                root = insert(root, hash, word);
                if (shouldSave) {
                    outFile << hash << std::endl;
                }
            }
        }
    } catch (...) {
        cleanupTree(root);
        throw;
    }

    cleanupTree(root);
    return 0;
}
