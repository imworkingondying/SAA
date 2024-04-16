#include <iostream>
#include <fstream>
#include <string>

using namespace std;

struct PatientRecord {
    int cardNumber;
    int chronicDiseaseCode;
    string doctorLastName;
};

struct TreeNode {
    PatientRecord data;
    TreeNode* left;
    TreeNode* right;
};

struct AVLTreeNode {
    PatientRecord data;
    AVLTreeNode* left;
    AVLTreeNode* right;
    int height;

    AVLTreeNode(const PatientRecord& record) : data(record), left(nullptr), right(nullptr), height(1) {}
};

class FileManager {
public:
    FileManager(const string& filename) : filename_(filename) {}

    void createTextFile(const PatientRecord& record) {
        ofstream file(filename_, ios::app);
        file << record.cardNumber << " " << record.chronicDiseaseCode << " " << record.doctorLastName << "\n";
        file.close();
    }

    void createBinaryFile() {
        ifstream textFile(filename_);
        ofstream binaryFile("binary_" + filename_, ios::binary);

        PatientRecord record;
        while (textFile >> record.cardNumber >> record.chronicDiseaseCode) {
            textFile.ignore();
            getline(textFile, record.doctorLastName);

            binaryFile.write(reinterpret_cast<char*>(&record), sizeof(PatientRecord));
        }

        textFile.close();
        binaryFile.close();
    }

    bool searchAndPrintTextFile(int searchKey) {
        ifstream file(filename_);
        PatientRecord record;

        while (file >> record.cardNumber >> record.chronicDiseaseCode) {
            file.ignore();
            getline(file, record.doctorLastName);

            if (record.cardNumber == searchKey) {
                cout << "Найдена запись: " << record.cardNumber << " " << record.chronicDiseaseCode << " " << record.doctorLastName << "\n";
                file.close();
                return true;
            }
        }

        file.close();
        return false;
    }

    bool printBinaryFileRecord(int recordIndex) {
        ifstream binaryFile("binary_" + filename_, ios::binary);
        PatientRecord record;

        binaryFile.seekg(recordIndex * sizeof(PatientRecord));

        if (binaryFile.read(reinterpret_cast<char*>(&record), sizeof(PatientRecord))) {
            cout << "Запись по адресу " << recordIndex << ": " << record.cardNumber << " " << record.chronicDiseaseCode << " " << record.doctorLastName << "\n";
            system("pause");
            binaryFile.close();
            return true;
        }

        binaryFile.close();
        return false;
    }

    void addRecordToTextFile(const PatientRecord& record) {
        ofstream file(filename_, ios::app);
        file << record.cardNumber << " " << record.chronicDiseaseCode << " " << record.doctorLastName << "\n";
        file.close();
    }

private:
    string filename_;
};

class BinarySearchTree {
public:
    BinarySearchTree() {
        root = nullptr;
    }

    void buildTreeFromFile(const string& filename) {
        ifstream file(filename);
        PatientRecord record;

        while (file >> record.cardNumber >> record.chronicDiseaseCode) {
            file.ignore();
            getline(file, record.doctorLastName);
            insert(record);
        }

        file.close();
    }

    void insert(const PatientRecord& record) {
        root = insertNode(root, record);
    }

    bool search(int key) {
    TreeNode* result = searchNode(root, key);
    if (result) {
        cout << "Запись найдена: " << result->data.cardNumber << " " << result->data.doctorLastName << endl;
        return true;
    } else {
        cout << "Запись не найдена." << endl;
        return false;
    }
}

    void remove(int key) {
        root = deleteNode(root, key);
    }

    void printTree() {
        printTree(root, 0);
    }

private:
    TreeNode* root;

    TreeNode* insertNode(TreeNode* node, const PatientRecord& record) {
        if (node == nullptr) {
            TreeNode* newNode = new TreeNode;
            newNode->data = record;
            newNode->left = newNode->right = nullptr;
            return newNode;
        }

        if (record.cardNumber < node->data.cardNumber) {
            node->left = insertNode(node->left, record);
        }
        else if (record.cardNumber > node->data.cardNumber) {
            node->right = insertNode(node->right, record);
        }

        return node;
    }

    TreeNode* searchNode(TreeNode* node, int key) {
        if (node == nullptr || node->data.cardNumber == key) {
            return node;
        }

        if (key < node->data.cardNumber) {
            return searchNode(node->left, key);
        }

        return searchNode(node->right, key);
    }

    TreeNode* findMinNode(TreeNode* node) {
        while (node->left != nullptr) {
            node = node->left;
        }
        return node;
    }

    TreeNode* deleteNode(TreeNode* node, int key) {
        if (node == nullptr) {
            return node;
        }

        if (key < node->data.cardNumber) {
            node->left = deleteNode(node->left, key);
        }
        else if (key > node->data.cardNumber) {
            node->right = deleteNode(node->right, key);
        }
        else {
            if (node->left == nullptr) {
                TreeNode* temp = node->right;
                delete node;
                return temp;
            }
            else if (node->right == nullptr) {
                TreeNode* temp = node->left;
                delete node;
                return temp;
            }

            TreeNode* temp = findMinNode(node->right);
            node->data = temp->data;
            node->right = deleteNode(node->right, temp->data.cardNumber);
        }

        return node;
    }

    void printTree(TreeNode* node, int level) {
        if (node != nullptr) {
            printTree(node->right, level + 1);
            for (int i = 0; i < level; i++) {
                cout << "    ";
            }
            cout << node->data.cardNumber << " " << node->data.doctorLastName << "\n";
            printTree(node->left, level + 1);
        }
    }
};

class AVLTree {
public:
    AVLTree() {
        root = nullptr;
    }

    void buildTreeFromFile(const string& filename) {
        ifstream file(filename);
        PatientRecord record;

        while (file >> record.cardNumber >> record.chronicDiseaseCode) {
            file.ignore();
            getline(file, record.doctorLastName);
            root = insert(root, record);
        }

        file.close();
    }

    void insert(const PatientRecord& record) {
        root = insert(root, record);
    }

    bool search(int key) {
        AVLTreeNode* result = searchNode(root, key);
        if (result) {
            cout << "Запись найдена: " << result->data.cardNumber << " " << result->data.doctorLastName << endl;
            return true;
        }
        else {
            cout << "Запись не найдена." << endl;
            return false;
        }
    }


    void remove(int key) {
        root = deleteNode(root, key);
    }

    void printTree() {
        printTree(root, 0);
    }

private:
    AVLTreeNode* root;

    int height(AVLTreeNode* node) {
        if (node == nullptr) {
            return 0;
        }
        return node->height;
    }

    int balanceFactor(AVLTreeNode* node) {
        if (node == nullptr) {
            return 0;
        }
        return height(node->left) - height(node->right);
    }

    void updateHeight(AVLTreeNode* node) {
        if (node != nullptr) {
            node->height = 1 + max(height(node->left), height(node->right));
        }
    }

    AVLTreeNode* rotateRight(AVLTreeNode* y) {
        AVLTreeNode* x = y->left;
        AVLTreeNode* T2 = x->right;

        x->right = y;
        y->left = T2;

        updateHeight(y);
        updateHeight(x);

        return x;
    }

    AVLTreeNode* rotateLeft(AVLTreeNode* x) {
        AVLTreeNode* y = x->right;
        AVLTreeNode* T2 = y->left;

        y->left = x;
        x->right = T2;

        updateHeight(x);
        updateHeight(y);

        return y;
    }

    AVLTreeNode* insert(AVLTreeNode* node, const PatientRecord& record) {
        if (node == nullptr) {
            return new AVLTreeNode(record);
        }

        if (record.cardNumber < node->data.cardNumber) {
            node->left = insert(node->left, record);
        }
        else if (record.cardNumber > node->data.cardNumber) {
            node->right = insert(node->right, record);
        }
        else {
            return node;
        }

        updateHeight(node);

        int balance = balanceFactor(node);

        if (balance > 1) {
            if (record.cardNumber < node->left->data.cardNumber) {
                return rotateRight(node);
            }
            else {
                node->left = rotateLeft(node->left);
                return rotateRight(node);
            }
        }

        if (balance < -1) {
            if (record.cardNumber > node->right->data.cardNumber) {
                return rotateLeft(node);
            }
            else {
                node->right = rotateRight(node->right);
                return rotateLeft(node);
            }
        }

        return node;
    }

    AVLTreeNode* searchNode(AVLTreeNode* node, int key) {
        if (node == nullptr || node->data.cardNumber == key) {
            return node;
        }

        if (key < node->data.cardNumber) {
            return searchNode(node->left, key);
        }

        return searchNode(node->right, key);
    }

    AVLTreeNode* findMinNode(AVLTreeNode* node) {
        while (node->left != nullptr) {
            node = node->left;
        }
        return node;
    }

    AVLTreeNode* deleteNode(AVLTreeNode* node, int key) {
        if (node == nullptr) {
            return node;
        }

        if (key < node->data.cardNumber) {
            node->left = deleteNode(node->left, key);
        }
        else if (key > node->data.cardNumber) {
            node->right = deleteNode(node->right, key);
        }
        else {
            if (node->left == nullptr || node->right == nullptr) {
                AVLTreeNode* temp = (node->left != nullptr) ? node->left : node->right;
                if (temp == nullptr) {
                    temp = node;
                    node = nullptr;
                }
                else {
                    *node = *temp;
                }
                delete temp;
            }
            else {
                AVLTreeNode* temp = findMinNode(node->right);
                node->data = temp->data;
                node->right = deleteNode(node->right, temp->data.cardNumber);
            }
        }

        if (node == nullptr) {
            return node;
        }

        updateHeight(node);

        int balance = balanceFactor(node);

        if (balance > 1) {
            if (balanceFactor(node->left) >= 0) {
                return rotateRight(node);
            }
            else {
                node->left = rotateLeft(node->left);
                return rotateRight(node);
            }
        }

        if (balance < -1) {
            if (balanceFactor(node->right) <= 0) {
                return rotateLeft(node);
            }
            else {
                node->right = rotateRight(node->right);
                return rotateLeft(node);
            }
        }

        return node;
    }

    void printTree(AVLTreeNode* node, int level) {
        if (node != nullptr) {
            printTree(node->right, level + 1);
            for (int i = 0; i < level; i++) {
                cout << "    ";
            }
            cout << node->data.cardNumber << " " << node->data.doctorLastName << "\n";
            printTree(node->left, level + 1);
        }
    }
};

int main() {

    setlocale(LC_ALL, "");

    FileManager fileManager("pats.txt");

    BinarySearchTree treeBIN;
    treeBIN.buildTreeFromFile("pats.txt");

    AVLTree treeAVL;
    treeAVL.buildTreeFromFile("pats.txt");

    while (true) {
        cout << "Меню:\n";
        cout << "1. Создать текстовый файл записей\n";
        cout << "2. Создать двоичный файл записей из текстового файла\n";
        cout << "3. Поиск записи по номеру карточки\n";
        cout << "4. Вывод записи по адресу в двоичном файле\n";
        cout << "5. Добавить запись в текстовый файл\n";
        cout << "6. Вставить запись в бинарное дерево\n";
        cout << "7. Поиск записи по номеру карточки(бинарное дерево)\n";
        cout << "8. Удалить запись из бинарного дерева\n";
        cout << "9. Вывести бинарное дерево\n";
        cout << "10. Вставить запись в AVL дерево\n";
        cout << "11. Поиск записи по номеру карточки(AVL дерево)\n";
        cout << "12. Удалить запись из AVL дерева\n";
        cout << "13. Вывести AVL дерево\n";
        cout << "14. Выход\n";

        int choice;
        cout << "Выберите операцию: ";
        cin >> choice;

        switch (choice) {
        case 1: {
            PatientRecord record;
            cout << "Введите номер карточки: ";
            cin >> record.cardNumber;
            cout << "Введите код хронического заболевания: ";
            cin >> record.chronicDiseaseCode;
            cout << "Введите фамилию лечащего врача: ";
            cin.ignore();
            getline(cin, record.doctorLastName);
            fileManager.createTextFile(record);
            break;
        }

        case 2:
            fileManager.createBinaryFile();
            cout << "Двоичный файл создан из текстового файла.\n";
            break;

        case 3: {
            int searchKey;
            cout << "Введите номер карточки для поиска: ";
            cin >> searchKey;
            if (!fileManager.searchAndPrintTextFile(searchKey)) {
                cout << "Запись не найдена.\n";
            }
            break;
        }

        case 4: {
            int recordIndex;
            cout << "Введите адрес записи в двоичном файле: ";
            cin >> recordIndex;
            if (!fileManager.printBinaryFileRecord(recordIndex)) {
                cout << "Запись не найдена.\n";
            }
            break;
        }

        case 5: {
            PatientRecord newRecord;
            cout << "Введите номер карточки: ";
            cin >> newRecord.cardNumber;
            cout << "Введите код хронического заболевания: ";
            cin >> newRecord.chronicDiseaseCode;
            cout << "Введите фамилию лечащего врача: ";
            cin.ignore();
            getline(cin, newRecord.doctorLastName);
            fileManager.addRecordToTextFile(newRecord);
            cout << "Запись добавлена в текстовый файл.\n";
            break;
        }

        case 6: {
            PatientRecord record;
            cout << "Введите номер карточки: ";
            cin >> record.cardNumber;
            cout << "Введите код хронического заболевания: ";
            cin >> record.chronicDiseaseCode;
            cout << "Введите фамилию лечащего врача: ";
            cin.ignore();
            getline(cin, record.doctorLastName);
            treeBIN.insert(record);
            break;
        }

        case 7: {
            int searchKey;
            cout << "Введите номер карточки для поиска: ";
            cin >> searchKey;
            if (treeBIN.search(searchKey)) {}
            else {}
            break;
        }

        case 8: {
            int deleteKey;
            cout << "Введите номер карточки для удаления: ";
            cin >> deleteKey;
            treeBIN.remove(deleteKey);
            break;
        }

        case 9:
            treeBIN.printTree();
            break;

        case 10: {
            PatientRecord record;
            cout << "Введите номер карточки: ";
            cin >> record.cardNumber;
            cout << "Введите код хронического заболевания: ";
            cin >> record.chronicDiseaseCode;
            cout << "Введите фамилию лечащего врача: ";
            cin.ignore();
            getline(cin, record.doctorLastName);
            treeAVL.insert(record);
            break;
        }

        case 11: {
            int searchKey;
            cout << "Введите номер карточки для поиска: ";
            cin >> searchKey;
            if (treeAVL.search(searchKey)) {
                cout << "Запись найдена.\n";
            }
            else {}
            break;
        }

        case 12: {
            int deleteKey;
            cout << "Введите номер карточки для удаления: ";
            cin >> deleteKey;
            treeAVL.remove(deleteKey);
            break;
        }

        case 13:
            treeAVL.printTree();
            break;

        case 14:
            return 0;

        default:
            cout << "Неправильный выбор. Попробуйте еще раз.\n";
        }
    }

    return 0;
}