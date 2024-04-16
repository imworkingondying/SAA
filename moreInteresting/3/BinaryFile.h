#ifndef BIN_H
#define BIN_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class bin {
public:

    struct Record { 

        int cardNumber;
        int diseaseCode;
        char doctorName[50];

        Record(int cardNumber, int diseaseCode, char doctorName[50]) {

            this->cardNumber = cardNumber;
            this->diseaseCode = diseaseCode;

            for (int i = 0; i < 50; i++) {
                this->doctorName[i] = doctorName[i];
            }
        }

        Record() {

            this->cardNumber = -1;
            this->diseaseCode = -1;

            for (int i = 0; i < 50; i++) {
                this->doctorName[i] = NULL;
            }
        }
    };

    bool fileExists(const string& filename) {

        ifstream file(filename.c_str());

        return file.good();
    }

    void createFileBin(const string& filename) {

        fstream f;
        f.open(filename);
        fstream fs(filename, ios::out | ios::binary);
        srand((unsigned)time(NULL));

        string nameArr[7] = { "Plato", "Alexey", "Daniel", "Dmitri", "Roman", "Patrick", "Robert" };
        string lastNameArr[7] = { "Ionov", "Rudenko", "Rekshinskiy", "Zabrodin", "Dolgov", "Baitman", "Pattinson" };
        string fatherNameArr[7] = { "Borisovich", "Dmitrievich", "Valentinovich", "Alexandrovich", "Sergeevich", "Sigmavich", "Platonovich" };


        for (int i = 0; i < (50 + (rand() % 51)); i++) {

            int cardnumber = 10000000 + rand()%90000001;
            int diseasecode = rand() % 251;
            char doctorname[50];
            string docnamestr = lastNameArr[rand() % 7] + " " + nameArr[rand() % 7] + " " + fatherNameArr[rand() % 7];
            strcpy_s(doctorname, docnamestr.c_str());
            
            Record* record = new Record(cardnumber,diseasecode,doctorname);
            cout << "Номер карты:" << record->cardNumber << "    Код заболевания:"
                << record->diseaseCode << "    ФИО врача:" << record->doctorName << endl;
            fs.write(reinterpret_cast<char*>(&(*record)), sizeof(*record));
        }

        fs.close();

        return;
    };

    void addRecord(const string& filename, const Record& record) {

        ofstream file(filename, ios::binary | ios::app);
        file.write(reinterpret_cast<const char*>(&record), sizeof(Record));
        file.close();
    }

    bool findRecord(const string& filename, int cardNumber, Record& foundRecord) {

        ifstream file(filename, ios::binary);

        if (!file) {
            cerr << "Ошибка открытия файла!" << endl;
            return false;
        }

        while (file.read(reinterpret_cast<char*>(&foundRecord), sizeof(Record))) {
            if (foundRecord.cardNumber == cardNumber) {
                file.close();
                return true;
            }
        }

        file.close();
        return false;
    }

    void readRecord(const string& binaryTextFile, int index) {

        ifstream inputFile(binaryTextFile, ios::binary);

        if (!inputFile.is_open()) {
            return;
        }

        Record record;
        
        inputFile.seekg((index) * sizeof(record), ios::beg);
        inputFile.read(reinterpret_cast<char*>(&record), sizeof(record));

        if (inputFile) {
            cout << "Номер карты:" << record.cardNumber << "    Код заболевания:"
                << record.diseaseCode << "    ФИО врача:" << record.doctorName << endl;
        }

        else {
            return;
        }

        inputFile.close();

        return;
    }

    void convertTextToBinary(const string& inputTextFile, const string& outputBinaryFile) {

        ifstream inputFile(inputTextFile);
        ofstream outputFile(outputBinaryFile, ios::binary);

        Record record;
        while (inputFile >> record.cardNumber >> record.diseaseCode) {
            inputFile.ignore();
            inputFile.getline(record.doctorName, sizeof(record.doctorName));
            outputFile.write(reinterpret_cast<const char*>(&record), sizeof(Record));
        }

        inputFile.close();
        outputFile.close();
    }

    void convertBinaryToText(const string& inputBinaryFile, const string& outputTextFile) {

        ifstream inputFile(inputBinaryFile, ios::binary);
        ofstream outputFile(outputTextFile);

        Record record;
        while (inputFile.read(reinterpret_cast<char*>(&record), sizeof(Record))) {
            outputFile << record.cardNumber << " " << record.diseaseCode << " " << record.doctorName << endl;
        }

        inputFile.close();
        outputFile.close();
    }

    void displayAllRecords(const string& filename) {

        ifstream file(filename, ios::binary);

        Record record;
        while (file.read(reinterpret_cast<char*>(&record), sizeof(Record))) {
            cout << "Номер карточки: " << record.cardNumber << endl;
            cout << "Код заболевания: " << record.diseaseCode << endl;
            cout << "ФИО лечащего врача: " << record.doctorName << endl;
            cout << "---------------------------" << endl;
        }

        file.close();
    }

    void deleteRecord(const string& filename, int diseaseCode) {

        ifstream inputFile(filename, ios::binary);
        ofstream tempFile("temp.dat", ios::binary);

        Record record;
        while (inputFile.read(reinterpret_cast<char*>(&record), sizeof(Record))) {
            if (record.diseaseCode != diseaseCode) {
                tempFile.write(reinterpret_cast<const char*>(&record), sizeof(Record));
            }
        }

        inputFile.close();
        tempFile.close();

        remove(filename.c_str());
        if (rename("temp.dat", filename.c_str()) != 0) {
            cerr << "Ошибка при переименовании файла." << endl;
        }
    }



    void createBinaryFileByDiseaseCode(const string& inputBinaryFile, const string& outputBinaryFile, int targetDiseaseCode) {

        ifstream inputFile(inputBinaryFile, ios::binary);
        ofstream outputFile(outputBinaryFile, ios::binary);

        Record record;
        while (inputFile.read(reinterpret_cast<char*>(&record), sizeof(Record))) {
            if (record.diseaseCode == targetDiseaseCode) {
                outputFile.write(reinterpret_cast<const char*>(&record), sizeof(Record));
            }
        }

        inputFile.close();
        outputFile.close();
    }


    void replaceDoctorNames(const string& filename, const vector<int>& patientCardNumbers, const string& newDoctorName) {

        fstream file(filename, ios::binary | ios::in | ios::out);

        Record record;
        for (int cardNumber : patientCardNumbers) {
            while (file.read(reinterpret_cast<char*>(&record), sizeof(Record))) {
                if (record.cardNumber == cardNumber) {
                    newDoctorName.copy(record.doctorName, sizeof(record.doctorName) - 1);
                    // Добавляем завершающий нуль-символ
                    record.doctorName[newDoctorName.size()] = '\0';
                    file.seekp(-static_cast<streamoff>(sizeof(Record)), ios::cur);
                    file.write(reinterpret_cast<const char*>(&record), sizeof(Record));
                    file.seekp(0, ios::end);
                    break;
                }
            }

            file.clear();
            file.seekg(0);
        }

        file.close();
    }

};

#endif