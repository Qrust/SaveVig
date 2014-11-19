//=============================================================================
// Name 			: Strong_vigenere_cipher.cpp
// Author 		: Kurbanov
// Version 		: 0.1
// Copyright 	: ASIS
// Description	: Убираем периодичность шифра Виженера
// IN 			: Текст латиница
// OUT			: Шифротекст латиница
//=============================================================================

#include <iostream>
#include <fstream>
#include <vector>

#include <unistd.h>

#define DEBUG

using namespace std;

class KeyNum
{   
	private:
		char key;
		unsigned long long int num;

	public:
		KeyNum()
		{
			key = '\0';
			num = 0;
		}
		~KeyNum()
		{

		}
		void PutKey(char keyChar)
		{
			key = keyChar;
		}       
		void IncrNum()
		{
			num++;
		}
		void FlashNum()
		{
			num = 0;
		}
		void PrintChain()
		{
			cout <<"["<< key <<"]"<<"["<< num <<"]"<< endl;
		}
		void PrintChainIf()
		{
			if (num != 0)
				cout <<"["<< key <<"]"<<"["<< num <<"]"<< endl;
		}
};

class HashTable
{	
	private:
		vector<KeyNum> table;
	public:
		HashTable(unsigned size)
		{
			table.reserve(size);
			#ifdef DEBUG
			cout << "size: " << size <<" capacity: " << table.capacity() << endl;
			#endif
		}
		~HashTable()
		{

		}
		void PutKeys(vector<char> array)
		{
			for (unsigned i = 0; i < table.capacity(); i++)
			{
				table[i].PutKey(array[i]);
			}
		}
		void PrintChains()
		{
			cout << "=====================================" << endl;
			for (unsigned i = 0; i < table.capacity(); i++)
				table[i].PrintChain();
			cout << "=====================================" << endl;
		}
		void PrintChainsIf()
		{
			cout << "=====================================" << endl;
			for (unsigned i = 0; i < table.capacity(); i++)
				table[i].PrintChainIf();
			cout << "=====================================" << endl;
		}
		void Analyse(unsigned long long int size, char* buffer)
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
		void Flash()
		{
			for (unsigned i = 0; i < table.capacity(); i++)
				table[i].FlashNum();
		}
};



int main(int argc, char const *argv[])
{
//===========================================================================//	
	string password ("kzk");
	#ifdef DEBUG
	cout << "Пароль: " <<  password <<endl;
	cout << "Длина пароля: " << password.size() << endl;
	#endif
//===========================================================================//
	vector<char> alphabet(126 - 32 + 1);

	#ifdef DEBUG
	cout << "Размер алфавита: " << alphabet.size() << endl;
	#endif
	
	for (unsigned i = 0; i < alphabet.size(); i++)
		alphabet[i] = (char)(i+32);

	#ifdef DEBUG
	cout << "Алфавит:" << endl;
	for (unsigned i = 0; i < alphabet.size(); i++)
		//cout << "[" << alphabet[i] << "] ";
		cout << alphabet[i] << "";
		//cout <<"["<< i <<"]"<< "[" << alphabet[i] << "]" << endl;
	cout << endl << endl;
	#endif
//===========================================================================//
	vector<unsigned> passOffset(password.size());
	for (unsigned i = 0; i < password.size(); i++)
	{
		for (unsigned k = 0; k < alphabet.size(); k++)
		{
			if (password[i] == alphabet[k])
			{
				passOffset[i] = (unsigned)alphabet[k] - 32;
				break;
			}
		}
	}

	#ifdef DEBUG
	cout << "Сдвиг у пароля: "<< endl;
	for (unsigned i = 0; i < passOffset.size(); i++)
	{
		cout <<"["<< i <<"]"<< "[" << passOffset[i] << "]" << endl;
	}
	#endif

//===========================================================================//
/*	#ifdef DEBUG
	cout << "Моя таблица Виженера:" << endl;
	for (unsigned i = 0; i <= alphabet.size(); i++)
	{
		for (unsigned k = i; k <= alphabet.size(); k++)
			cout << alphabet[k] << " ";

		for (unsigned k = 0; k < i; k++)
			cout << alphabet[k] << " ";
		
		cout << endl << endl;
	}
	#endif*/
//===========================================================================//
	ifstream plainText("./text.txt");
	//ofstream encryptedText("./encrypted_text.txt");

	if ( !plainText.is_open())
	{
        cout << "Файл не может быть открыт!\n";
        return 0;
	}
//===========================================================================//
	plainText.seekg(0, plainText.end);
	unsigned long long int size = plainText.tellg();
	plainText.seekg (0, plainText.beg);
	
	#ifdef DEBUG
	cout << "Размер файла: " << size << " байт" <<endl;
	#endif
	
	char* buffer = new char [size];
	plainText.read(buffer, size);

	#ifdef DEBUG
	cout << "=====================================" << endl
		 << buffer << endl
		 << "=====================================" << endl << endl;
	#endif
//===========================================================================//
	cout << "Статистика по файлу" << endl;

	HashTable table(alphabet.size());
	table.PutKeys(alphabet);
	table.Analyse(size,buffer);
	table.PrintChainsIf();

/*	vector<KeyNum> KeyNum;
	KeyNum.reserve(alphabet.size() + 1);
	
	for (unsigned i = 0; i < (alphabet.size() + 1); i++)
		KeyNum[i].PutKey(alphabet[i]);

	for (unsigned i = 0; i < (alphabet.size() + 1); i++)
		KeyNum[i].PrintChain();

	unsigned long long int textPosition  = 0;
	while (textPosition < size)
	{
		if ( buffer[textPosition] != '\n')
		{
			KeyNum[(int)(buffer[textPosition]) - 32].IncrNum();
		}
		// дописать, отдельная обработка для '\n'
		textPosition++;
	}

	for (unsigned i = 0; i < (alphabet.size() + 1); i++)
		KeyNum[i].PrintChainIf();
	
	cout << "=====================================" << endl;*/
//===========================================================================//
/*	#ifdef DEBUG
	cout << buffer << endl;
	#endif*/
	unsigned rounds = password.size();
	for ( unsigned k = 0; k < rounds; k++)
	{
/*		#ifdef DEBUG
		cout << "=================[" << k << "]=================" << endl
			 << "Кодируем паролем: " << password 
			 << " длиной " << password.size() << " символов" 
			 << endl;
		#endif*/

		unsigned long long int run  = 0;
		while (run < size)
		{
			for (unsigned i = 0; i < password.size(); i++)
			{
				if (run >= size)
					break;

				if ( buffer[run] != '\n')
				{
					buffer[run] = (char)(((int )buffer[run]- 32 + passOffset[i] )%95 + 32 );
				}
				// дописать, отдельная обработка для '\n'
				run++;
			}
		}

		password.erase(password.size()-1); // сокращаем длину пароля на 1

/*		#ifdef DEBUG
		cout << "=====================================" << endl
			 << buffer << endl
			 << "=====================================" << endl << endl;
		#endif*/

/*		#ifdef DEBUG
		cout << buffer << endl;
		#endif*/
	}

	table.Flash();
	table.Analyse(size,buffer);
	table.PrintChainsIf();
//===========================================================================//
/*	plainText.seekg(0, plainText.beg);
	while( plainText.get(c) )
	{	
		cout << "[" << c << "]" << "_" << plainText.tellg() << " "  ;
		//encryptedText.put(c);
	}
	*/
//===========================================================================//
	#ifdef DEBUG
	cout << endl << "Выход из программы" << endl;
	#endif

	plainText.close();
	//encryptedText.close();
	delete[] buffer;
 	return 0;
}
