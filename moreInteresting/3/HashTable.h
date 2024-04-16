#ifndef HASH_H
#define HASH_H

#include <iomanip>
#include <iostream>
using namespace std;

class Hash {
public:

	struct cell {

		int cardid;
		int fileindex;
		cell* next;

		cell(int cardid, int fileindex) {
			this->cardid = cardid;
			this->fileindex = fileindex;
			next = nullptr;
		}

		cell() {
			cardid = -1;
			fileindex = -1;
			next = nullptr;
		}
	};

	struct patient {
		int cardNumber;
		int diseaseCode;
		char doctorName[50];
	};

	int size = 10;
	int elementammount = 0;
	cell* arr = new cell[size];

	Hash() {

		for (int i = 0; i < size; i++) {
			arr[i] = cell();
		}
	};

	int hashfuncPoli(int cardid) {

		int hash = 0;
		int num = 13;

		for (int i = 0; i < sizeof(cardid); i++) {
			hash = (hash * num) + ((cardid >> (i * 8)) & 0xFF);
		}

		return hash % size;
	}

	struct cell search(int cardid) {

		int hashindex = hashfuncPoli(cardid);

		if (arr[hashindex].fileindex == -1) {
			return cell();
		}

		cell hashcell = arr[hashindex];

		if (hashcell.cardid == cardid) {
			return hashcell;
		}

		do {
			if (hashcell.next != nullptr) {
				hashcell = *hashcell.next;
			}

			if (hashcell.cardid == cardid) {
				return hashcell;
			}

		} while (hashcell.next != nullptr);

		return cell();
	};

	void insertcell(int cardid, int fileindex) {

		cell* hashcell = new cell;
		hashcell->cardid = cardid;
		hashcell->fileindex = fileindex;
		hashcell->next = nullptr;

		int hashindex = hashfuncPoli(cardid);

		if (arr[hashindex].fileindex == -1) {
			arr[hashindex] = *hashcell;
			this->elementammount++;
			cout << "Вставлено по индексу " << hashindex << ", на слое: 0" << endl;
			rehash();
			return;
		}

		else {
			int layer = 1;
			cell* readyhashcell = &arr[hashindex];

			while (readyhashcell->next != nullptr) {
				readyhashcell = readyhashcell->next;
				layer++;
			}

			readyhashcell->next = hashcell;
			this->elementammount++;
			cout << "Вставлено по индексу " << hashindex << ", на слое: " << layer << endl;
			rehash();

			return;
		}

		return;
	};

	struct cell deletecell(int cardid) {

		int hashindex = hashfuncPoli(cardid);

		cell* temp = &arr[hashindex];
		cell* temp2 = new cell();

		if (temp->next != nullptr) {
			if (temp->cardid == cardid) {
				arr[hashindex] = *arr[hashindex].next;
				this->elementammount--;
				return *temp;
			}

			do {
				temp2 = temp;
				temp = temp->next;

				if (temp->cardid == cardid) {
					temp2->next = temp->next;
					this->elementammount--;
					cout << "Ячейка удалена (1)" << endl;
					return *temp;
				}

			} while (temp->next != nullptr);
			cout << "Нет ячейки для удаления (1)" << endl;
			return cell();
		}

		else
		{
			if (temp->cardid == cardid) {
				arr[hashindex] = cell();
				this->elementammount--;
				return *temp;
				cout << "Ячейка удалена (2)" << endl;
			}

			else {
				return cell();
				cout << "Нет ячейки для удаления (2)" << endl;
			}

		}

		return cell();
	};

	void rehash() {

		double c = (double)elementammount / (double)size;

		if (c < 0.75) {
			cout << "Рехеширование не нужно." << endl;
			return;
		}

		this->elementammount = 0;

		this->size *= 10;
		cell* old_arr = arr;
		this->arr = new cell[size];

		for (int i = 0; i < size / 10; i++) {

			if (old_arr[i].fileindex == -1) {
				continue;
			}

			cell temp = old_arr[i];
			cout << "Рехеш: ";
			insertcell(temp.cardid, temp.fileindex);

			do {
				if (temp.next != nullptr) {
					temp = *temp.next;
					cout << "Рехеш: ";
					insertcell(temp.cardid, temp.fileindex);
				}
			} while (temp.next != nullptr);
		}
		cout << "Рехешировано до размера: " << size << endl;

		return;
	};

	void print() {

		cout << "\n";
		cout << "ВЫВОД\n";
		cout << "\n";

		for (int i = 0; i < this->size; i++) {
			if (arr[i].cardid == -1) {
				continue;
			}

			cout << "Ключ: " << setw(2) << arr[i].cardid << " | Индекс элемента: " << setw(2) << arr[i].fileindex;

			if (arr[i].next != nullptr) {
				cell* buff = arr[i].next;

				while (true) {
					cout << " >> ";
					cout << "Ключ: " << setw(2) << buff->cardid << " | Индекс элемента: " << setw(2) << buff->fileindex;

					if (buff->next != nullptr) {
						buff = buff->next;
					}

					else { 
						break; 
					}
				}
			}

			cout << "\n";
		}
	}
};

#endif