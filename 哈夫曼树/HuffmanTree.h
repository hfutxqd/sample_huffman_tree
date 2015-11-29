#pragma once
#include <vector>
#include <list>
#include <map>
#include <bitset>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

struct BNode
{
	BNode * Left;
	int Weight;
	char data;
	BNode * Right;
};

class HuffmanTree
{
public:
	HuffmanTree(vector<char> data);
	void PreOrder();
	void WriteToFile(char * filename);
	static void Unzip(char * filename, char * outfile);
	~HuffmanTree();
private:
	vector<BNode*> TreeList;
	map<char, int> CharMap;
	map<char, string> CodeMap;
	BNode * root;
	string code;
	vector<char> sourceStr;
	string binStr;

	void InitTreeList();
	void Building();
	int findMinTwoIndex(int &Index1, int &Index2);
	int getEffectSize();
	BNode * getEffectOne();
	void PreOrder(BNode * root);
	void CreateCodeMap();
	void findCode(BNode * root, char from, string code);
	void CreateBinStr();
};

