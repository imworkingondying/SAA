#include "BinaryFile.h"
#include "FileHash.h"
#include "HashTable.h"
#include <iostream>
#include <cstdlib>

using namespace std;

int main() {
    setlocale(LC_ALL, "");

    bin BIN;
    BIN.createFileBin("testFile.txt");
    BIN.readRecord("testFile.txt", 0);
   

    binhash BINHASH;
    int choice;

    do {
        cout << "\n===== МЕНЮ =====" << endl;
        cout << "1. Ввод новой записи" << endl;
        cout << "2. Поиск записи по номеру карты" << endl;
        cout << "3. Удаление записи по номеру карты" << endl;
        cout << "4. Вывод хэш-таблицы" << endl;
        cout << "5. Выход" << endl;

        cout << "Введите ваш выбор: ";
        cin >> choice;

        switch (choice) {
        case 1:
            int cardNumber, diseaseCode;
            char doctorName[50];

            cout << "Номер карты: ";
            cin >> cardNumber;

            cout << "Код заболевания: ";
            cin >> diseaseCode;

            cout << "ФИО врача: ";
            cin.ignore();
            cin.getline(doctorName, sizeof(doctorName));

            bin::Record newRecord(cardNumber, diseaseCode, doctorName);
            BIN.addRecord("binaryTestFile.txt", newRecord);
            break;
        case 2:
        {
            unsigned long long polis;
            cout << "Введите номер карты для поиска: ";
            cin >> polis;
            BINHASH.read(polis);
        }
        break;
        case 3:
        {
            unsigned long long polis;
            cout << "Введите номер карты для удаления: ";
            cin >> polis;
            BINHASH.del(polis);
        }
        break;
        case 4:
            BINHASH.printHashTable();
            break;
        case 5:
            cout << "Завершение программы. До свидания!" << endl;
            break;
        default:
            cout << "Неверный выбор. Пожалуйста, попробуйте снова." << endl;
        }
    } while (choice != 5);


    return 0;
}
