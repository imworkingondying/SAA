#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

struct Record {
    int cardNumber;
    int diseaseCode;
    char doctorName[50];
};

bool fileExists(const string& filename) {
    ifstream file(filename.c_str());
    return file.good();
}

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

int main() {

    setlocale(LC_ALL, "");

    string filename;
    cout << "Введите имя файла: ";
    cin >> filename;

    if (!fileExists(filename)) {
        cout << "Файл не существует. Создаем новый файл." << endl;
    }

    while (true) {
        cout << "1. Добавить запись" << endl;
        cout << "2. Найти запись по номеру карточки" << endl;
        cout << "3. Вывести все записи" << endl;
        cout << "4. Преобразовать текстовый файл в двоичный" << endl;
        cout << "5. Преобразовать двоичный файл в текстовый" << endl;
        cout << "6. Удалить запись по коду заболевания" << endl;
        cout << "7. Создать файл по коду заболевания" << endl;
        cout << "8. Заменить фамилию, имя, отчество врача" << endl;
        cout << "9. Выход" << endl;
        int choice;
        cin >> choice;

        switch (choice) {
        case 1: {
            Record newRecord;
            cout << "Введите номер карточки: ";
            cin >> newRecord.cardNumber;
            cout << "Введите код заболевания: ";
            cin >> newRecord.diseaseCode;
            cout << "Введите ФИО лечащего врача: ";
            cin.ignore(); // Очищаем буфер для символов новой строки
            cin.getline(newRecord.doctorName, sizeof(newRecord.doctorName));
            addRecord(filename, newRecord);
            cout << "Запись добавлена." << endl;
            break;
        }
        case 2: {
            int searchNumber;
            cout << "Введите номер карточки для поиска: ";
            cin >> searchNumber;
            Record foundRecord;
            if (findRecord(filename, searchNumber, foundRecord)) {
                cout << "Найдена запись:" << endl;
                cout << "Номер карточки: " << foundRecord.cardNumber << endl;
                cout << "Код заболевания: " << foundRecord.diseaseCode << endl;
                cout << "ФИО лечащего врача: " << foundRecord.doctorName << endl;
            }
            else {
                cout << "Запись с таким номером карточки не найдена." << endl;
            }
            break;
        }
        case 3:
            displayAllRecords(filename);
            break;
        case 4: {
            string inputTextFile, outputBinaryFile;
            cout << "Введите имя текстового файла: ";
            cin >> inputTextFile;
            cout << "Введите имя двоичного файла: ";
            cin >> outputBinaryFile;
            convertTextToBinary(inputTextFile, outputBinaryFile);
            cout << "Преобразование завершено." << endl;
            break;
        }
        case 5: {
            string inputBinaryFile, outputTextFile;
            cout << "Введите имя двоичного файла: ";
            cin >> inputBinaryFile;
            cout << "Введите имя текстового файла: ";
            cin >> outputTextFile;
            convertBinaryToText(inputBinaryFile, outputTextFile);
            cout << "Преобразование завершено." << endl;
            break;
        }
        case 6: {
            int deleteDiseaseCode;
            cout << "Введите код заболевания для удаления: ";
            cin >> deleteDiseaseCode;
            deleteRecord(filename, deleteDiseaseCode);
            cout << "Записи с кодом заболевания " << deleteDiseaseCode << " удалены." << endl;
            break;
        }
        case 7: {
            int targetDiseaseCode;
            cout << "Введите код заболевания для создания файла: ";
            cin >> targetDiseaseCode;
            string outputBinaryFile;
            cout << "Введите имя двоичного файла для сохранения: ";
            cin >> outputBinaryFile;
            createBinaryFileByDiseaseCode(filename, outputBinaryFile, targetDiseaseCode);
            cout << "Файл создан." << endl;
            break;
        }
        case 8: {
            int numPatients;
            cout << "Введите количество пациентов для изменения данных: ";
            cin >> numPatients;
            vector<int> patientCardNumbers(numPatients);
            cout << "Введите номера карточек пациентов:" << endl;
            for (int i = 0; i < numPatients; ++i) {
                cin >> patientCardNumbers[i];
            }
            string newDoctorName;
            cout << "Введите новое ФИО лечащего врача: ";
            cin.ignore(); // Очищаем буфер для символов новой строки
            getline(cin, newDoctorName);
            replaceDoctorNames(filename, patientCardNumbers, newDoctorName);
            cout << "Данные изменены." << endl;
            break;
        }
        case 9:
            return 0;
        default:
            cout << "Неверный выбор. Попробуйте снова." << endl;
        }
    }

    return 0;
}