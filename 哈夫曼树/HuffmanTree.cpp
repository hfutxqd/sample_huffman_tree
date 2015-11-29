#include "HuffmanTree.h"

HuffmanTree::HuffmanTree(vector<char> data) :sourceStr(data.begin(), data.end())
{
	for (int i = 0; i < data.size(); i++)
	{
		CharMap[data[i]]++;
	}
	InitTreeList();
	Building();
	CreateCodeMap();
	CreateBinStr();
}

void HuffmanTree::InitTreeList()
{
	map<char, int>::iterator it = CharMap.begin();
	for (; it != CharMap.end(); it++)
	{
		BNode * tmp = new BNode;
		tmp->data = it->first;
		tmp->Weight = it->second;
		tmp->Left = NULL;
		tmp->Right = NULL;
		TreeList.push_back(tmp);
	}
}

void HuffmanTree::Building()
{
	while (getEffectSize() > 1)
	{
		int sum, pos1, pos2;
		sum = findMinTwoIndex(pos1, pos2);
		BNode * tmp = new BNode;
		tmp->Weight = sum;
		tmp->data = 0;
		tmp->Left = TreeList[pos1];
		tmp->Right = TreeList[pos2];
		TreeList.push_back(tmp);
		TreeList[pos1]->Weight = -1;
		TreeList[pos2]->Weight = -1;
	}
	root = getEffectOne();
}

BNode * HuffmanTree::getEffectOne()
{
	for (int i = 0; i < TreeList.size(); i++)
	{
		if (TreeList[i]->Weight > 0)
		{
			return TreeList[i];
		}
	}
}

int HuffmanTree::getEffectSize()
{
	int count = 0;
	for (int i = 0; i < TreeList.size(); i++)
	{
		if (TreeList[i]->Weight > 0)
		{
			count++;
		}
	}

	return count;
}

int HuffmanTree::findMinTwoIndex(int &Index1, int &Index2)
{
	int i = 0;
	while (TreeList[i]->Weight < 0)
	{
		i++;
	}
	int min = TreeList[i]->Weight;
	int pos = i;
	for (int i = 0; i < TreeList.size(); i++)
	{
		if (min > TreeList[i]->Weight && TreeList[i]->Weight > 0)
		{
			min = TreeList[i]->Weight;
			pos = i;
		}
	}
	TreeList[pos]->Weight = -1;
	i = 0;
	while (TreeList[i]->Weight < 0)
	{
		i++;
	}
	int min2 = TreeList[i]->Weight;
	int pos2 = i;
	for (int i = 0; i < TreeList.size(); i++)
	{
		if (min2 > TreeList[i]->Weight && TreeList[i]->Weight > 0)
		{
			min2 = TreeList[i]->Weight;
			pos2 = i;
		}
	}
	TreeList[pos]->Weight = min;
	Index1 = pos;
	Index2 = pos2;
	return min + min2;
}

void HuffmanTree::PreOrder(BNode * root)
{
	if (root != 0)
	{
		if (root->data > 0)
			cout << root->data << " " ;
		PreOrder(root->Left);
		PreOrder(root->Right);
	}
}

void HuffmanTree::PreOrder()
{
	PreOrder(root);
}

void HuffmanTree::findCode(BNode * root, char from, string code)
{
	if (from == 'L')
	{
		code.append("0");
	}
	else if (from == 'R')
	{
		code.append("1");
	}
	if (root != NULL)
	{
		findCode(root->Left,'L', code);
		findCode(root->Right, 'R', code);
		if (root->Left == NULL && root->Right == NULL)
		{
			if (from == 'B')
			{
				code.append("0");
			}
			CodeMap[root->data] = code;
			return;
		}
	}
}

void HuffmanTree::CreateCodeMap()
{
	findCode(root, 'B', code);
}

void HuffmanTree::CreateBinStr()
{
	for (int i = 0; i < sourceStr.size(); i++)
	{
		binStr.append(CodeMap[sourceStr[i]]);
	}
	/*cout << sourceStr << endl;
	cout << binStr << endl;*/
}

void HuffmanTree::WriteToFile(char * filename)
{
	ofstream out;
	out.open(filename, ios::binary);
	int count = CodeMap.size();

	out.write((char*)&count, sizeof(count));
	map<char, string>::iterator it = CodeMap.begin();
	for (; it != CodeMap.end(); it++)
	{
		/*char key;
		char value[16] = {0};
		key = it->first;
		strcpy_s(value, it->second.c_str());
		out.write((char*)&key, sizeof(key));
		out.write((char*)value, sizeof(char) * 16);*/
		char key;
		unsigned long value = 0;;
		char valueLenth = 0;
		key = it->first;
		bitset <32> bit(it->second);
		valueLenth = it->second.size();
		value = bit.to_ulong();
		out.write((char*)&key, sizeof(key));
		out.write((char*)&value, sizeof(value));
		out.write((char*)&valueLenth, sizeof(valueLenth));//用来表示哈夫曼编码的长度

	}

	int zeroCount = 32 - binStr.size() % 32;
	out.write((char*)&zeroCount, sizeof(zeroCount));
	for (int i = 0; i < binStr.size(); i += 32)
	{
		bitset<32> bit(binStr.substr(i,32));
		unsigned long tmp =  bit.to_ulong();
		out.write((char*)&tmp, sizeof(tmp));
	}
	
	out.close();
}

void Delete(BNode* root)
{
	if (root != NULL)
	{
		Delete(root->Left);
		Delete(root->Right);
		delete root;
	}
}

HuffmanTree::~HuffmanTree()
{
	Delete(root);
}

void HuffmanTree::Unzip(char * filename, char * outfile)
{
	ifstream file;
	ofstream out;

	file.open(filename, ios::binary);
	out.open(outfile, ios::binary);

	int size = 0;
	file.read((char*)&size, sizeof(int));
	map<char, string> CodeMap;
	for (int i = 0; i < size; i++)
	{
		char key;
		unsigned long value;
		char valueLenth;
		file.read((char*)&key, sizeof(char));
		file.read((char*)&value, sizeof(int));
		file.read((char*)&valueLenth, sizeof(char));
		bitset<32> bit(value);
		string tmp = bit.to_string();
		tmp = tmp.substr(tmp.size() - valueLenth, valueLenth);
		CodeMap[key] = tmp;
	}
	int zeroCount = 0;
	file.read((char*)&zeroCount, sizeof(int));
	map<string, char> CodeToChar;
	map<char, string>::iterator it = CodeMap.begin();
	for (; it != CodeMap.end(); it++)
	{
		CodeToChar[it->second] = it->first;
	}
	string codes = "";
	while (!file.eof())
	{
		unsigned long x;
		file.read((char*)&x, sizeof(unsigned long));
		bitset<32> bit(x);
		bit.to_string().size();
		codes.append(bit.to_string());
	}
	string sub_str = codes.substr(codes.size() - 32, 32);
	codes.resize(codes.size() - 32);
	codes.resize(codes.size() - 32);
	codes.append(sub_str.substr(zeroCount, 32 - zeroCount));
	string code;
	for (int i = 0; i < codes.size(); i++)
	{
		code.append(codes.substr(i, 1));
		if (CodeToChar.count(code))
		{
			char ch = CodeToChar[code];
			code = "";
			out.write((char*)&ch, sizeof ch);
		}
	}
	file.close();
	out.close();
}