#include <iostream>
#include <iomanip>

using namespace std;

class TreeNode {
public:
    double data;
    TreeNode* left;
    TreeNode* right;

    TreeNode(double value) : data(value), left(nullptr), right(nullptr) {}
};

class BalancedBinaryTree {
public:
    BalancedBinaryTree(int n) {
        root = createBalancedTree(1, n);
    }

    ~BalancedBinaryTree() {
        deleteTree(root);
    }

    void printTree() {
        printTree(root, 0);
    }

    double averageLeftSubtree() {
        return calculateSubtreeAverage(root->left);
    }

    double averageRightSubtree() {
        return calculateSubtreeAverage(root->right);
    }

    void deleteTree() {
        deleteTree(root);
        root = nullptr;
    }

private:
    TreeNode* root;

    TreeNode* createBalancedTree(int start, int end) {
        if (start > end) {
            return nullptr;
        }

        int mid = (start + end) / 2;
        TreeNode* newNode = new TreeNode(mid);
        newNode->left = createBalancedTree(start, mid - 1);
        newNode->right = createBalancedTree(mid + 1, end);
        return newNode;
    }

    void deleteTree(TreeNode* node) {
        if (node) {
            deleteTree(node->left);
            deleteTree(node->right);
            delete node;
        }
    }

    void printTree(TreeNode* node, int indent) {
        if (node) {
            printTree(node->right, indent + 1);
            for (int i = 0; i < indent; i++) {
                cout << '\t';
            }
            cout << node->data << endl;
            printTree(node->left, indent + 1);
        }
    }

    double calculateSubtreeAverage(TreeNode* node) {
        if (!node) {
            return 0.0;
        }

        double sum = node->data;
        double count = 1.0;

        sum += calculateSubtreeAverage(node->left);
        count += countNodes(node->left);

        sum += calculateSubtreeAverage(node->right);
        count += countNodes(node->right);

        return sum / count;
    }

    int countNodes(TreeNode* node) {
        if (!node) {
            return 0;
        }

        return 1 + countNodes(node->left) + countNodes(node->right);
    }
};

int main() {

    setlocale(LC_ALL, "");

    int n, choice1, choice2;

a2:    cout << "--------МЕНЮ--------" << "\n";
    cout << "1. Создать дерево." << "\n";
    cout << "2. Выйти." << "\n";
    cout << "--------------------" << "\n";

    cout << ">>> ";
    cin >> choice1;

    if (choice1 == 1) {

        cout << "\n";
        cout << "Введите количество узлов для создания дерева: ";
        cin >> n;

        BalancedBinaryTree tree(n);
        cout << "\n";
        cout << "-------------------------------" << endl;
  a1:      tree.printTree();

        cout << "\n";
        cout << "Выберите действие:" << "\n";
        cout << "1. Найти среднее арифметическое чисел в левом поддереве." << "\n";
        cout << "2. Найти среднее арифметическое чисел в правом поддереве." << "\n";
        cout << "3. Удалить дерево." << "\n";
        cout << "4. Выйти." << "\n";

        cout << ">>> ";
        cin >> choice2;

        if (choice2 == 1) {
            cout << "\n";
            cout << "Среднее арифметическое чисел в левом поддереве: " << tree.averageLeftSubtree() << endl;
            goto a1;
        }

        else if (choice2 == 2) {
            cout << "\n";
            cout << "Среднее арифметическое чисел в правом поддереве: " << tree.averageRightSubtree() << endl;
            goto a1;
        }

        else if (choice2 == 3) {
            cout << "\n";
            tree.deleteTree();
            cout << "Дерево удалено." << endl;
            cout << "\n";
            goto a2;
        }

        else {
            return 0;
        }
    }

    else {
        return 0;
    }
}