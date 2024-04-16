#include <iostream>
#include <string>
#include <vector>
using namespace std;

string runLengthEncode(const string& input) {
    string result = "";
    int count = 1;

    for (int i = 1; i <= input.length(); ++i) {
        if (i == input.length() || input[i] != input[i - 1]) {
            result += input[i - 1] + to_string(count);
            count = 1;
        }
        else {
            ++count;
        }
    }

    return result;
}


double calculateCompressionRatio(const string& original, const string& compressed) {
    return static_cast<double>(original.length()) / compressed.length();
}

// Реализация алгоритма LZ77
vector<pair<int, char>> compressLZ77(const string& input) {
    vector<pair<int, char>> result;
    int len = input.length();
    int pos = 0;

    while (pos < len) {
        int start = pos;
        int matchLen = 0;
        char nextChar = input[pos];

        // Ищем самое длинное совпадение
        for (int i = 0; i < pos; ++i) {
            int j = 0;
            while (pos + j < len && input[i + j] == input[pos + j]) {
                ++j;
            }
            if (j > matchLen) {
                matchLen = j;
                start = i;
                nextChar = input[pos + j];
            }
        }

        result.push_back({ pos - start, nextChar });
        pos += (matchLen + 1);
    }

    return result;
}

// Реализация алгоритма LZ78
vector<pair<int, char>> compressLZ78(const string& input) {
    vector<pair<int, char>> result;
    vector<string> dictionary;
    int len = input.length();
    int pos = 0;

    while (pos < len) {
        int start = pos;
        int matchLen = 0;
        char nextChar = input[pos];

        // Ищем самое длинное совпадение с текущим словарем
        for (int i = 0; i < dictionary.size(); ++i) {
            int j = 0;
            while (pos + j < len && dictionary[i][j] == input[pos + j]) {
                ++j;
            }
            if (j > matchLen) {
                matchLen = j;
                start = i;
                nextChar = (pos + j < len) ? input[pos + j] : '\0';
            }
        }

        result.push_back({ start, nextChar });
        if (pos + matchLen < len) {
            dictionary.push_back(input.substr(pos, matchLen + 1));
        }
        pos += (matchLen + 1);
    }

    return result;
}

// Функция для вывода результата сжатия
void printCompressionResult(const vector<pair<int, char>>& compressed) {
    cout << "Сжатое значение: ";
    for (const auto& entry : compressed) {
        cout << "<" << entry.first << ", " << entry.second << "> ";
    }
    cout << endl;
}


int main() {
    setlocale(LC_ALL, "");

    cout << "Выберите тест для выполнения:" << endl;
    cout << "1. Тестирование повторяющихся символов" << endl;
    cout << "2. Тестирование уникальных символов" << endl;
    cout << "3. Тестирование LZ77" << endl;
    cout << "4. Тестирование LZ78" << endl;

    int choice;
    cin >> choice;

    switch (choice) {
    case 1: {
        // Тестирование на длинной серии повторяющихся символов
        string repeatedInput(10000, 'A');
        string compressedRepeated = runLengthEncode(repeatedInput);
        double ratioRepeated = calculateCompressionRatio(repeatedInput, compressedRepeated);

        cout << "1. Тестирование повторяющихся символов:" << endl;
        cout << "   Исходное значение: " << repeatedInput << endl;
        cout << "   Сжатое значение: " << compressedRepeated << endl;
        cout << "   Коэффициент сжатия: " << ratioRepeated << endl;
        break;
    }
    case 2: {
        // Тестирование на длинной серии неповторяющихся символов
        string uniqueInput = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        string compressedUnique = runLengthEncode(uniqueInput);
        double ratioUnique = calculateCompressionRatio(uniqueInput, compressedUnique);

        cout << "2. Тестирование уникальных символов:" << endl;
        cout << "   Исходное значение: " << uniqueInput << endl;
        cout << "   Сжатое значение: " << compressedUnique << endl;
        cout << "   Коэффициент сжатия: " << ratioUnique << endl;
        break;
    }
    case 3: {
        // Тестирование LZ77
        string inputLZ77 = "101000100101010001011";
        vector<pair<int, char>> compressedLZ77 = compressLZ77(inputLZ77);
        cout << "3. Тестирование LZ77:" << endl;
        printCompressionResult(compressedLZ77);
        break;
    }
    case 4: {
        // Тестирование LZ78
        string inputLZ78 = "какатанекатанекатата";
        vector<pair<int, char>> compressedLZ78 = compressLZ78(inputLZ78);
        cout << "4. Тестирование LZ78:" << endl;
        printCompressionResult(compressedLZ78);
        break;
    }
    default:
        cout << "Некорректный выбор." << endl;
    }

    return 0;
}