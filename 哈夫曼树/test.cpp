#include <iostream>
#include <string>
#include <fstream>
#include <bitset>
#include "HuffmanTree.h"
using namespace std;

int main()
{
	ifstream file;
	file.open("1.txt");
	istreambuf_iterator <char> beg(file), end;
	vector<char> data(beg, end);
	/*char ch;
	while (!file.eof())
	{
		file.read((char*)&ch, sizeof(char));
		char tmp[2] = { ch, 0 };
		str.append(tmp);
	}*/
	file.close();

	HuffmanTree t(data);
	t.WriteToFile("1.txt.myz");

	HuffmanTree::Unzip("1.txt.myz", "1.txt.unzip");
	system("pause");
	return 0;
}