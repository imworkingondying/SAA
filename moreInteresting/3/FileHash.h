#ifndef BINHASH_H
#define BINHASH_H
#include "BinaryFile.h"
#include "HashTable.h"

using namespace std;

class binhash {

    bin bin;
    Hash hash;

public:
    void start() {

        bin.createFileBin("binaryTestFile.txt");
        fstream fs("binaryTestFile.txt", ios::in | ios::binary);
        if (!fs.is_open()) {
            return;
        }

        bin::Record record;
        int fileindex = 0;

        while (fs.read((char*)&record, sizeof(struct bin::Record))) {
            cout << "Номер карты:" << record.cardNumber << "    Код заболевания:"
                << record.diseaseCode << "    ФИО врача:" << record.doctorName << endl;
            hash.insertcell(record.cardNumber, fileindex);
            fileindex++;
        }

        fs.close();
        hash.print();
    };

    void read(unsigned long long polis) {

        if (hash.search(polis).fileindex == -1) {
            cout << "Нет такой записи!" << endl;
            return;
        }

        bin.readRecord("binaryTestFile.txt", hash.search(polis).fileindex);
    };

    void del(unsigned long long polis) {

        if (hash.deletecell(polis).fileindex == -1) {
            return;
        }

        bin.deleteRecord("binaryTestFile.txt", polis);
    };

    void printHashTable() {
        hash.print();
    }
};
#endif