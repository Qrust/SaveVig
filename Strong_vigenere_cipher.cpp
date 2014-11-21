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

#include "hash.h"

#define DEBUG

using namespace std;

int main(int argc, char const *argv[])
{
/*============================================================================
|	Пароль
*============================================================================*/
	string password ("k1z2u3");
	#ifdef DEBUG
	cout << "Пароль: " <<  password << endl
		 << "Длина пароля: " << password.size() << endl;
	#endif

/*============================================================================
|	Заполняем алфавит
*============================================================================*/
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

/*============================================================================
|	Считаем смещение каждой буквы пароля
*============================================================================*/
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

/*============================================================================
|	Моя таблица Виженера
*============================================================================*/
	#ifdef DEBUG
		#ifdef VIGENERE
		cout << "Моя таблица Виженера:" << endl;
		for (unsigned i = 0; i < alphabet.size(); i++)
		{
			for (unsigned k = i; k < alphabet.size(); k++)
				cout << alphabet[k] << " ";

			for (unsigned k = 0; k < i; k++)
				cout << alphabet[k] << " ";
			
			cout << endl << endl;
		}
		#endif
	#endif

/*============================================================================
|	Файлы для записи
*============================================================================*/
	ifstream plainText("./text.txt");
	//ofstream encryptedText("./encrypted_text.txt");

	if ( !plainText.is_open())
	{
        cout << "Файл не может быть открыт!\n";
        return 0;
	}

/*============================================================================
|	Пишем в буфер для  последующей обработки
*============================================================================*/
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
		 << "-------------------------------------" << endl << endl;
	#endif

/*============================================================================
|	Частотный анализ
*============================================================================*/
	cout << "Статистика по файлу" << endl;

	HashTable table(alphabet.size());
	table.PutKeys(alphabet);
	table.Analyse(size,buffer);
	table.PrintChainsIf();

/*============================================================================
|	Кодируем ключем с раундами равными длине ключа
*============================================================================*/
	#ifdef DEBUG
	cout << "=====================================" << endl
		 << buffer << endl
		 << "-------------------------------------" << endl << endl;
	#endif

	unsigned rounds = password.size();
	
	for ( unsigned k = 0; k < rounds; k++)
	{
		#ifdef DEBUG
		cout << "=================[" << k << "]=================" << endl
			 << "Кодируем паролем: " << password << ", длиной " << password.size() << " символов" << endl
			 << "-----------------[" << k << "]-----------------" << endl;
		#endif

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

		#ifdef DEBUG
		cout << "=====================================" << endl
			 << buffer << endl
			 << "-------------------------------------" << endl << endl;
		#endif
	}

	table.Flash();
	table.Analyse(size,buffer);
	table.PrintChainsIf();

/*============================================================================
|	Запись шифротекста в файл
*============================================================================*/

/*	plainText.seekg(0, plainText.beg);
	while( plainText.get(c) )
	{	
		cout << "[" << c << "]" << "_" << plainText.tellg() << " "  ;
		//encryptedText.put(c);
	}
	*/
	//encryptedText.close();

//===========================================================================//
	#ifdef DEBUG
	cout << endl << "Выход из программы" << endl;
	#endif

	plainText.close();
	delete[] buffer;
 	return 0;
}
