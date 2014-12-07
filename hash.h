#ifndef HASH_H
#define HASH_H

#include <vector>

using namespace std;

class KeyNum
{   
	private:
		char key;
		unsigned long long int num;

	public:
		KeyNum();
		~KeyNum();
		KeyNum(char x, unsigned long long int y);
		KeyNum(char x);
		void PutKey(char keyChar);
		void IncrNum();
		void FlashNum();
		void PrintChain();
		void PrintChainIf();
		void PrintChainForPython();
		void PrintChainForPythonNoSpace();
};

class HashTable
{	
	private:
		vector<class KeyNum> table;
	public:
		HashTable(unsigned size);
		~HashTable();
		void PutKeys(vector<char> array);
		void PrintChains();
		void PrintChainsIf();
		void PrintChainsForPython();
		void PrintChainsForPythonNoSpace();
		void Flash();
		void Analyse(size_t size, char* buffer, vector<char> alphabet);
};

#endif