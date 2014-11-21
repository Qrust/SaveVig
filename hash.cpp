#include "hash.h"

#include <iostream>

using namespace std;

/*============================================================================
|	KeyNum
*============================================================================*/
KeyNum::KeyNum():key('\0'),num(0) {};
KeyNum::~KeyNum()
{

};
KeyNum::KeyNum(char x, unsigned long long int y)
{
	key = x;
	num = y;
};
KeyNum::KeyNum(char x)
{
	key = x;
};
void KeyNum::PutKey(char keyChar)
{
	key = keyChar;
};
void KeyNum::IncrNum()
{
	num++;
};
void KeyNum::FlashNum()
{
	num = 0;
};
void KeyNum::PrintChain()
{
	cout <<"["<< key <<"]"<<"["<< num <<"]"<< endl;
};
void KeyNum::PrintChainIf()
{
	if (num != 0)
		cout <<"["<< key <<"]"<<"["<< num <<"]"<< endl;
};

/*============================================================================
|	HashTable
*============================================================================*/
HashTable::HashTable(unsigned size)
{
	table.reserve(size);
	#ifdef DEBUG
	cout << "size: " << size <<" capacity: " << table.capacity() << endl;
	#endif
};
HashTable::~HashTable()
{

};
void HashTable::PutKeys(vector<char> array)
{
	for (unsigned i = 0; i < table.capacity(); i++)
	{
		table[i].PutKey(array[i]);
	}
}
void HashTable::PrintChains()
{
	cout << "========СТАТИСТИКА=ПО=ФАЙЛУ==========" << endl;
	for (unsigned i = 0; i < table.capacity(); i++)
		table[i].PrintChain();
	cout << "-------------------------------------" << endl;
}
void HashTable::PrintChainsIf()
{
	cout << "========СТАТИСТИКА=ПО=ФАЙЛУ==========" << endl;
	for (unsigned i = 0; i < table.capacity(); i++)
		table[i].PrintChainIf();
	cout << "-------------------------------------" << endl;
}
void HashTable::Flash()
{
	for (unsigned i = 0; i < table.capacity(); i++)
		table[i].FlashNum();
}
void HashTable::Analyse(unsigned long long int size, char* buffer)
{
	unsigned long long int textPosition  = 0;
	while (textPosition < size)
	{
		if ( buffer[textPosition] != '\n')
		{
			table[(int)(buffer[textPosition]) - 32].IncrNum();
		}
		// дописать, отдельная обработка для '\n'
		textPosition++;
	}
}