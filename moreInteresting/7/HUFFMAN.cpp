#include <iostream>
#include <string>
#include <queue>
#include <unordered_map>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <bitset>
#include <vector>
#include <algorithm>
#include <map>
#include "ShannonFano.h"
using namespace std;


struct Node
{
	char ch;
	int freq;
	Node* left;
	Node* right;

	Node(char ch, int freq, Node* left, Node* right)
		: ch(ch), freq(freq), left(left), right(right)
	{
	}
};

struct comp {
	bool operator()(Node* l, Node* r) { return l->freq > r->freq; }
};

void postorder(Node* p, int indent, unordered_map<char, string> huffmanCode)
{
	if (p == NULL) return;
	if (p->right) postorder(p->right, indent + 4, huffmanCode);
	if (indent) cout << setw(indent) << ' ';
	if (p->right) cout << "/\n" << setw(indent) << ' ';
	if (p->ch != 0)
		cout << p->ch << "(" << huffmanCode[p->ch] << ")\n ";
	else
		cout << "1\n" << setw(indent) << ' ' << "0\n";
	if (p->left) {
		cout << setw(indent) << ' ' << " \\\n";
		postorder(p->left, indent + 4, huffmanCode);
	}
}

// Функция заполняет словарь по дереву
void encode(Node* root, string str, unordered_map<char, string>& huffmanCode)
{
	if (root == nullptr) return;

	if (!root->left && !root->right) huffmanCode[root->ch] = str;

	encode(root->left, str + "0", huffmanCode);
	encode(root->right, str + "1", huffmanCode);
}

char decode(Node* root, int& index, int& charsRead, string str)
{
	if (root == nullptr) return 0;

	if (!root->left && !root->right) {
		charsRead++;
		return root->ch;
	}

	index++;

	if (str[index] == '0')
		return decode(root->left, index, charsRead, str);
	else
		return decode(root->right, index, charsRead, str);
}

int main()
{
	setlocale(LC_ALL, "");
	std::cout << "Выберите опцию:\n";
	std::cout << "1. Кодирование/декодирование методом Хаффмана\n";
	std::cout << "2. Кодирование/декодирование методом Шеннона-Фано\n";
	std::cout << "0. Выход\n";
	std::cout << "Введите ваш выбор: ";

	int choice;
	cin >> choice;

	if (choice == 1) {
		ifstream fs("dc.txt");
		stringstream buffer;
		buffer << fs.rdbuf();
		string text = buffer.str();

		unordered_map<char, int> freq;
		for (char ch : text) freq[ch]++;


		priority_queue<Node*, vector<Node*>, comp> pq;
		for (auto pair : freq) pq.push(new Node(pair.first, pair.second, nullptr, nullptr));


		while (pq.size() != 1)
		{

			Node* left = pq.top(); pq.pop();
			Node* right = pq.top();	pq.pop();


			pq.push(new Node('\0', left->freq + right->freq, left, right));
		}


		Node* root = pq.top();


		unordered_map<char, string> huffmanCode;
		encode(root, "", huffmanCode);

		cout << "Дерево Хаффмана:\n";
		postorder(root, 0, huffmanCode);

		cout << "Коды Хаффмана:\n";
		for (auto pair : huffmanCode) cout << pair.first << " " << pair.second << '\n';


		string encoded = "";
		string encodedbin = "";
		bitset<8> byte;
		int i = 0;
		int chars = 0;
		for (char ch : text) {
			string code = huffmanCode[ch];
			encoded += code;
			chars++;
			for (char ch : code) {
				bool bit = ch - 48;
				byte[i] = bit;
				i++;
				if (i == 8) {
					encodedbin += (char)byte.to_ulong();
					i = 0;
					byte.reset();
				}
			}
		}
		encodedbin += (char)byte.to_ulong();
		int filelen = encodedbin.length();
		ofstream fsb("encoded.txt", ios::binary | ios::out);
		fsb.write((char*)&filelen, sizeof(int));
		fsb.write((char*)&chars, sizeof(int));
		fsb << encodedbin;
		fsb.close();


		ifstream fse("encoded.txt", ios::binary);
		int fileLength = 0;
		int charsInFile = 0;
		fse.read((char*)&fileLength, sizeof(int));
		fse.read((char*)&charsInFile, sizeof(int));
		char* encodedFromFile = new char[fileLength];
		fse.read(encodedFromFile, fileLength);
		printf("\nТекст сжат в: %.6f раз\nКоэффициент сжатия=%.6f\n", (float)charsInFile / fileLength, (float)fileLength / charsInFile);

		int index = -1;
		int charsRead = 0;
		ofstream fsd("decoded.txt");
		cout << "\nДекодирование файла...\n";
		int progressWidth = 50;
		int nextUpdate = -1;
		while ((index < fileLength * 8) && (charsRead < charsInFile)) {
			char x = decode(root, index, charsRead, encoded);
			fsd << x;

			if ((100 * charsRead / charsInFile) > nextUpdate) {
				cout << "[";
				nextUpdate++;

				int pos = progressWidth * nextUpdate / 100;
				for (int i = 0; i < progressWidth; ++i) {
					if (i < pos) cout << "=";
					else if (i == pos) cout << ">";
					else cout << " ";
				}
				cout << "] " << int(((float)charsRead / charsInFile) * 100.0) << " %\r";
				cout.flush();
			}
		}
		fsd.close();
		cout << endl;
	}
	else if (choice == 2) {
		{
		string normal =
			"Прибавь к ослиной голове \n"
			"Еще одну, получишь две.\n"
			"Но сколько б ни было ослов, \n"
			"Они и двух не свяжут слов.\n";

		cout << "Original: " << normal << endl;

		cout << "-----------------------------------" << endl;

		ShannonFano::printEncoded(normal);

		cout << "-----------------------------------" << endl;

		string encoded = ShannonFano::encode(normal);
		vector<ShannonFano::Node*> nodes;
		ShannonFano::Node* root = ShannonFano::buildTree(nodes);
		ShannonFano::printDecoded(root, encoded);

		return 0;
		}
	}

	else {
		return 0;
	}
}
