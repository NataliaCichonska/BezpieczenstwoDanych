#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <vector>
#include <climits>

// Funkcja haszuj�ca DJB2
unsigned long djb2(const std::string& str) {
    unsigned long hash = 5381;
    for (char c : str) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    return hash;
}

// Struktura w�z�a drzewa binarnego
struct TreeNode {
    unsigned long hash;
    std::vector<std::string> data; // Lista danych z tym samym haszem
    TreeNode* left;
    TreeNode* right;

    TreeNode(unsigned long h, const std::string& d)
        : hash(h), data({d}), left(nullptr), right(nullptr) {}
};

// Funkcja wstawiaj�ca element do drzewa binarnego
TreeNode* insert(TreeNode* root, unsigned long hash, const std::string& data) {
    if (!root) {
        return new TreeNode(hash, data);
    }
    if (hash < root->hash) {
        root->left = insert(root->left, hash, data);
    } else if (hash > root->hash) {
        root->right = insert(root->right, hash, data);
    } else {
        // Konflikt - dodaj dane do listy w bie��cym w�le
        root->data.push_back(data);
    }
    return root;
}

// Funkcja wyszukuj�ca dane w drzewie na podstawie hasza
TreeNode* findInTree(TreeNode* root, unsigned long hash) {
    if (!root) {
        return nullptr; // Je�li dotarli�my do pustego w�z�a, hash nie istnieje
    }
    if (hash == root->hash) {
        return root; // Znaleziono w�ze� o pasuj�cym hashu
    } else if (hash < root->hash) {
        return findInTree(root->left, hash); // Szukaj w lewym poddrzewie
    } else {
        return findInTree(root->right, hash); // Szukaj w prawym poddrzewie
    }
}


// Funkcja rekurencyjna do wy�wietlania drzewa w spos�b czytelny od prawej do lewej
void printTree(TreeNode* root, int level = 0, int indent = 5) {
    if (root == nullptr) return;

    // Wy�wietl prawe poddrzewo
    printTree(root->right, level + 1, indent);

    // Wy�wietl bie��cy w�ze�
    std::cout << std::string(level * indent, ' '); // Dodaj odpowiednie wci�cie
    std::cout << root->hash << " (";
    for (size_t i = 0; i < root->data.size(); ++i) {
        std::cout << root->data[i];
        if (i < root->data.size() - 1) std::cout << ", ";
    }
    std::cout << ")" << std::endl;

    // Wy�wietl lewe poddrzewo
    printTree(root->left, level + 1, indent);
}


// Zwolnienie pami�ci drzewa
void freeTree(TreeNode* root) {
    if (root) {
        freeTree(root->left);
        freeTree(root->right);
        delete root;
    }
}

// Funkcja wczytuj�ca dane z pliku
void loadFromFile(const std::string& filename, TreeNode*& root) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Nie mo�na otworzy� pliku: " << filename << std::endl;
        return;
    }

    std::string word;
    while (file >> word) { // Czytaj s�owa jedno po drugim
        unsigned long hash = djb2(word);
        root = insert(root, hash, word);
        std::cout << "Dodano: \"" << word << "\" z haszem: " << hash << std::endl;
    }

    file.close();
}

// G��wna funkcja
int main() {
    TreeNode* root = nullptr;
    int choice;

    while(1==1) {
        std::cin.clear();
        std::cout << "Wybierz tryb dzia�ania:\n"
                  << "1. R�czne wprowadzanie danych\n"
                  << "2. Wczytanie danych z pliku tekstowego\n"
                  << "3. Podaj hash aby odczyta� dane z wcze�niej stowrzonego drzewa\n"
                  << "4. EXIT\n"
                  << "Tw�j wyb�r: ";

        std::cin >> choice;
        std::cin.ignore(); // Usuwa znak nowej linii po wprowadzeniu liczby

        if (choice == 1) {
            std::cout << "Podaj dane do zapisania w drzewie (wpisz '\\STOP', aby zako�czy�):" << std::endl;
            std::string input;
            while (true) {
                std::cout << "Dane: ";
                std::getline(std::cin, input);
                if (input == "\\STOP") {
                        std::cout << "\nOstateczne drzewo binarne:\n";
                        printTree(root);
                        break;
                }
                unsigned long hash = djb2(input);
                root = insert(root, hash, input);

                std::cout << "Hasz dla \"" << input << "\": " << hash << std::endl;
                std::cout << "Aktualne drzewo:" << std::endl;
                printTree(root);
            }
        } else if (choice == 2) {
            std::cout << "Podaj nazw� pliku tekstowego: ";
            std::string filename;
            std::cin >> filename;
            loadFromFile(filename, root);
        } else if (choice == 3) {
            std::cout << "Podaj hash do wyszukania: ";
            unsigned long hashToFind;
            std::cin >> hashToFind;
            if (std::cin.fail()) {
                std::cin.clear();                // Czy�� flag� b��du
                std::cin.ignore(INT_MAX, '\n');  // Usu� b��dne dane z bufora
                std::cerr << "Nieprawid�owe wej�cie! Wprowad� liczb� ca�kowit�.\n";
                continue; // Powr�t do g��wnego menu
            }

            TreeNode* result = findInTree(root, hashToFind);

            if (result != nullptr) {
                std::cout << "Znaleziono hash: " << hashToFind << " z danymi: ";
                for (const auto& word : result->data) {
                    std::cout << word << " ";
                }
                std::cout << std::endl;
            } else {
                std::cout << "Hash " << hashToFind << " nie istnieje w drzewie." << std::endl;
            }
        } else if (choice == 4) {
            break;
        } else {
            std::cerr << "Nieprawid�owy wyb�r!" << std::endl;
            return 1;
        }
    }
    freeTree(root);
    return 0;
}
