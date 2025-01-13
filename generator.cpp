#include <iostream>
#include <fstream>
#include <random>
#include <vector>
#include <string>

void generate_uniform_ascii(std::string filename, size_t L, size_t n, int start, int end) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(start, end);

    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < L; ++j) {
            file << static_cast<char>(distrib(gen));
        }
        file << '\n';
    }

    file.close();
}

void generate_normal_ascii(std::string filename, size_t L, size_t n, double mean, double stddev, int start, int end) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<> distrib(mean, stddev);

    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < L; ++j) {
            int value = static_cast<int>(distrib(gen));
            if (value < start) value = start;
            if (value > end) value = end;
            file << static_cast<char>(value);
        }
        file << '\n';
    }

    file.close();
}

int main() {
    size_t L = 5; // Długość każdego stringa
    int ascii_start = 32; // Początek zakresu ASCII (drukowalne znaki)
    int ascii_end = 126;  // Koniec zakresu ASCII (drukowalne znaki)

    // Liczba stringów w każdym zbiorze
    size_t list_size1 = 100000;
    size_t list_size2 = 500000;
    size_t list_size3 = 1000000;
    size_t list_size4 = 5000000;

    // Średnia i odchylenie standardowe dla rozkładu normalnego
    double mean = (ascii_start + ascii_end) / 2.0; // Środek zakresu
    double stddev = 10.0;                          // Odchylenie standardowe

    // Generowanie zbiorów z rozkładem równomiernym
    generate_uniform_ascii("uniform_1.txt", L, list_size1, ascii_start, ascii_end);
    generate_uniform_ascii("uniform_2.txt", L, list_size2, ascii_start, ascii_end);
    generate_uniform_ascii("uniform_3.txt", L, list_size3, ascii_start, ascii_end);
    generate_uniform_ascii("uniform_4.txt", L, list_size4, ascii_start, ascii_end);

    // Generowanie zbiorów z rozkładem normalnym
    generate_normal_ascii("normal_1.txt", L, list_size1, mean, stddev, ascii_start, ascii_end);
    generate_normal_ascii("normal_2.txt", L, list_size2, mean, stddev, ascii_start, ascii_end);
    generate_normal_ascii("normal_3.txt", L, list_size3, mean, stddev, ascii_start, ascii_end);
    generate_normal_ascii("normal_4.txt", L, list_size4, mean, stddev, ascii_start, ascii_end);

    std::cout << "ASCII datasets generated and saved to files." << std::endl;
    return 0;
}
