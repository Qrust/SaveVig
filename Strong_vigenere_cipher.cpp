//=============================================================================
// Name 		: Strong_vigenere_cipher.cpp
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

//#define DEBUG
//#define VIGENERE

using namespace std;

void MakePythonFile (HashTable *plane, HashTable *encr, string password);
void Revision_1 (string password, vector<unsigned> passOffset, char *buffer, unsigned long long int size);

int main(int argc, char **argv)
{

/*============================================================================
|	Парсинг пароля
*============================================================================*/
	string password;
	
	int opt;
	while ((opt = getopt(argc, argv, "p:") ) != -1) 
	{
		switch (opt) 
		{
			case 'p':
				password = optarg;
				break;
			default:
				cout << "Использовать вот так: %s -p [password]\n" << argv[0] << endl;
				return 0;
		}
	}
	
	#ifdef DEBUG
	cout << "ПАРОЛЬ: " <<  password << endl
		 << "Длина пароля: " << password.size() << endl
		 << endl;
	#endif

/*============================================================================
|	Заполняем алфавит
*============================================================================*/
	vector<char> alphabet(126 - 32 + 1);
	
	for (unsigned i = 0; i < alphabet.size(); i++)
		alphabet[i] = (char)(i+32);

	#ifdef DEBUG
	cout << "АЛФАВИТ:" << endl;

	for (unsigned i = 0; i < alphabet.size(); i++)
		//cout << "[" << alphabet[i] << "] ";
		cout << alphabet[i] << "";
		//cout <<"["<< i <<"]"<< "[" << alphabet[i] << "]" << endl;
	cout << endl;

	cout << endl
		 << "Размер алфавита: " << alphabet.size() << endl
		 << endl;
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
		cout << endl;
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
				cout << alphabet[k] << "";

			for (unsigned k = 0; k < i; k++)
				cout << alphabet[k] << "";
			
			cout << endl;
			//cout << endl << endl;
		}
			cout << endl;
		#endif
	#endif

/*============================================================================
|	Файлы для чтения записи
*============================================================================*/
	ifstream plainText("./text.txt");
	//ofstream encryptedText("./encrypted_text.txt");

	if ( !plainText.is_open())
	{
		cout << "Файл text.txt не может быть открыт!\n";
		return 0;
	}

/*============================================================================
|	Пишем в буфер для последующей обработки
*============================================================================*/
	plainText.seekg(0, plainText.end);
	unsigned long long int size = plainText.tellg();
	plainText.seekg (0, plainText.beg);
	
	#ifdef DEBUG
	cout << "Размер файла: " << size << " байт" <<endl;
	#endif
	
	char *buffer = new char [size];
	plainText.read(buffer, size);

/*	#ifdef DEBUG
	cout << "/==========================================================" << endl
		 << buffer << endl
		 << "\\----------------------------------------------------------" << endl 
		 << endl
		 << endl;
	#endif*/

/*============================================================================
|	Частотный анализ planeText
*============================================================================*/
	HashTable planeTable(alphabet.size());

	planeTable.Flash();
	planeTable.PutKeys(alphabet);
	planeTable.Analyse(size,buffer);

	#ifdef DEBUG
		planeTable.PrintChainsIf();
	#endif

/*============================================================================
|	Кодируем ключем с раундами равными длине ключа
*============================================================================*/
/*	#ifdef DEBUG
	cout << "/----------------------------------------------------------" << endl
		 << buffer << endl
		 << "\\----------------------------------------------------------" << endl 
		 << endl;
	#endif
*/

	Revision_1 (password, passOffset, buffer, size);

/*============================================================================
|	Частотный анализ encryptedText
*============================================================================*/
	HashTable encryptedTable(alphabet.size());
	
	encryptedTable.Flash();
	encryptedTable.PutKeys(alphabet);
	encryptedTable.Analyse(size,buffer);

	#ifdef DEBUG
		encryptedTable.PrintChainsIf();
	#endif

/*============================================================================
|	Пишем питонский файлик для графопостроителя
*============================================================================*/
	MakePythonFile (&planeTable, &encryptedTable, password);

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

void MakePythonFile (HashTable *plane, HashTable *encr, string password)
{
	string nameFile; 
	nameFile = "./py_tests/plotdata_" + password;
	nameFile += ".py";
	ofstream pyCode(nameFile.c_str());
	
	if ( !pyCode.is_open())
	{
		cout << nameFile << " не может быть открыт!\n";
	}

	pyCode <<
	"#!/usr/bin/env python" << endl <<
	"" << endl <<
	"import numpy as np" << endl <<
	"import matplotlib.pyplot as plt" << endl <<
	"" << endl <<
	"N = 95" << endl <<
	"" << endl <<
	"PlainText = (";
	
	streambuf *coutbuf = cout.rdbuf();
	cout.rdbuf(pyCode.rdbuf());
	plane->PrintChainsForPython();
	cout.rdbuf(coutbuf);

	pyCode <<
	")" << endl <<
	"" << endl <<
	"CryptedText = (";

	coutbuf = cout.rdbuf();
	cout.rdbuf(pyCode.rdbuf());
	encr->PrintChainsForPython();
	cout.rdbuf(coutbuf);

	pyCode <<
	")" << endl <<
	"" << endl <<
	"ind = np.arange(N)" << endl <<
	"width = 0.35" << endl <<
	"fig, ax = plt.subplots()" << endl <<
	"rects1 = ax.bar(ind, PlainText, width, color='green')" << endl <<
	"rects2 = ax.bar(ind+width, CryptedText, width, color='red')" << endl <<
	"ax.set_title('Частотный анализ')" << endl <<
	"ax.set_ylabel('Количество символов')" << endl <<
	"ax.set_xlabel('Алфавит')" << endl <<
	"ax.set_xticks(ind+width)" << endl <<
	"ax.set_xticklabels( (' ', '!', '\"', '#', '$', '%', '&', '\\\'', '(', ')', '*', '+', ',', '-', '.', '/', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ':', ';', '<', '=', '>', '?', '@', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '[', '\\\\', ']', '^', '_', '`', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '{', '|', '}', '~') )" << endl <<
	"ax.legend( (rects1[0], rects2[0]), ('PlainText', 'CryptedText') )" << endl <<
	"ax.set_xlim(0,N-width/2)" << endl <<
	"fig.set_size_inches(24,15)" << endl <<
	"plt.savefig('Graph_" <<
	password <<
	".png', format = 'png', bbox_inches='tight')" << endl;
	//".png', format = 'png', bbox_inches='tight', transparent=True)" << endl;
	pyCode.close();
}

void Revision_1 (string password, vector<unsigned> passOffset, char *buffer, unsigned long long int size)
{
	unsigned rounds = password.size();

	for ( unsigned k = 0; k < rounds; k++)
	{
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

/*	#ifdef DEBUG
	cout << "/----------------------------------------------------------" << endl
		 << buffer << endl
		 << "\\----------------------------------------------------------" << endl 
		 << endl
		 << endl;
	#endif
*/	}
}