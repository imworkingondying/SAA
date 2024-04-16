#include <iostream>
#include <vector>

using namespace std;

int countPaths(int n, int m) {

    vector<vector<int>> dp(n, vector<int>(m, 0));

    for (int i = 0; i < n; ++i)
        dp[i][0] = 1;

    for (int j = 0; j < m; ++j)
        dp[0][j] = 1;

    for (int i = 1; i < n; ++i) {
        for (int j = 1; j < m; ++j) {
            dp[i][j] = dp[i - 1][j] + dp[i][j - 1];
        }
    }

    return dp[n - 1][m - 1];
}

int main() {
    
    setlocale(LC_ALL, "");

    int n, m, choice;

    cout << "--------МЕНЮ--------\n";
    cout << "Выберите действие:\n";
    cout << "1. Создать таблицу\n";
    cout << "2. Выйти\n";
    cout << ">>> ";
    cin >> choice;

    a1:   if (choice == 1) {
        cout << "\nВведите размеры поля (n и m):\n";
        cout << ">>> ";
        cin >> n >> m;

        int result = countPaths(n, m);

        cout << "\nКоличество путей: " << result << "\n";

        cout << "\nВыберите действие:\n";
        cout << "1. Создать другую таблицу\n";
        cout << "2. Выйти\n";
        cout << ">>> ";
        cin >> choice;

        if (choice == 1) {
            goto a1;
        }
        else {
            return 0;
        }
    }

    else {
        return 0;
    }

}
