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
#include <stdlib.h>
#include <algorithm>    // std::reverse

#include "hash.h"

#define DEBUG
//#define VIGENERE

using namespace std;

// Делает питоновский файл в исходной директории для построения графика
void MakePythonFile(HashTable *plane, HashTable *encr, string password, int alphabetLen);

// Простой виженер
void NativeVig(char *buffer, size_t size, vector<unsigned> passOffset, vector<char> alphabet);

// Шифрование с уменьшением длины пароля
void Revision_1(string password, vector<unsigned> passOffset, char *buffer, unsigned long long int size);

// Шифрование с уменьшением длины пароля, НО ШИФРУЮТСЯ ТОЛЬКО БУКВЫ (БЕЗ РЕГИСТРА)
void Revision_1_1(string password, vector<unsigned> passOffset, char *buffer, unsigned long long int size);

// Шифрование с уменьшением длины пароля и инвертированем
void Revision_2(string password, vector<char> alphabet, char *buffer, unsigned long long int size);

int main(int argc, char **argv)
{
/*============================================================================
|	Парсинг -пароль -метод_шифрования 
*============================================================================*/
	string password;
	int method = 0;
	
	int opt;
	while ((opt = getopt(argc, argv, "p:m:") ) != -1) 
	{
		switch (opt) 
		{
			case 'p':
				password = optarg;
				break;
			case 'm':
				method = atoi(optarg);
				break;				
			default:
				cout << "Использовать вот так: %s -p [password] -m [method]\n" << argv[0] << endl;
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
	int alphabet_len = (26+1);
	vector<char> alphabet(alphabet_len);

	switch (alphabet_len) 
	{
		case (26+1):
			for (unsigned i = 0; i < alphabet.size(); i++)
				alphabet[i] = (char)(i+97);// обычные буквы нижнего регистра
			break;		
		case (126-32+1):
			for (unsigned i = 0; i < alphabet.size(); i++)
				alphabet[i] = (char)(i+32);
			break;		
		default:
			cout << "Определите размер алфавита" << endl;
			return 0;
	}

	#ifdef DEBUG
		cout << "АЛФАВИТ:" << endl;

		for (unsigned i = 0; i < alphabet.size(); i++)
			//cout << "[" << alphabet[i] << "] ";
			cout << alphabet[i] << "";
			//cout <<"["<< i <<"]"<< "[" << alphabet[i] << "]" << endl;
		cout << endl;

		cout << endl
			 << "РАЗМЕР АЛФАВИТА: " << alphabet.size() << endl
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
				passOffset[i] = k;
				break;
			}
		}
	}

	#ifdef DEBUG
		cout << "СДВИГ У ПАРОЛЯ: "<< endl;
		for (unsigned i = 0; i < passOffset.size(); i++)
		{
			cout <<"["<< i <<"]"<< "[" << passOffset[i] << "]";
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

	/*#ifdef DEBUG
	cout << "/==========================================================" << endl
		 << buffer << endl
		 << "\\----------------------------------------------------------" << endl 
		 << endl
		 << endl;
	#endif*/
/*============================================================================
|	Частотный анализ planeText
*============================================================================*/
	HashTable planeTable(alphabet.size()); // [символ][количество]
	planeTable.Flash(); // очистка
	planeTable.PutKeys(alphabet); // заполнение ключами

	planeTable.Analyse(size,buffer);

	#ifdef DEBUG
		planeTable.PrintChainsIf();
	#endif
/*============================================================================
|	Кодируем ключем в зависимости от медота
*============================================================================*/
	/*
	#ifdef DEBUG
	cout << "/----------------------------------------------------------" << endl
		 << buffer << endl
		 << "\\----------------------------------------------------------" << endl 
		 << endl;
	#endif
	*/
	switch (method) 
	{
		case 0:
			Revision_0 (password, passOffset, buffer, size);
			break;		
		case 1:
			Revision_1 (password, passOffset, buffer, size);
			break;
		case 2:
			Revision_2 (password, alphabet, buffer, size);
			break;			
		default:
			Revision_0 (password, passOffset, buffer, size);
			break;
	}
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
	// ВНИМАНИЕ, ЗНАЧЕНИЕ ПОСЛЕДНЕЙ ЦИФРЫ СМОТРИ ВНУТРИ ФУНКЦИИ
	MakePythonFile (&planeTable, &encryptedTable, password, alphabet.size());
/*============================================================================
|	Запись шифротекста в файл
*============================================================================*/
	/*	
	plainText.seekg(0, plainText.beg);
	while( plainText.get(c) )
	{	
		cout << "[" << c << "]" << "_" << plainText.tellg() << " "  ;
		//encryptedText.put(c);
	}
	*/
	//encryptedText.close();
/*============================================================================
|	Очистка и выход
*============================================================================*/
	#ifdef DEBUG
	cout << endl << "Выход из программы" << endl;
	#endif

	plainText.close();
	delete[] buffer;
	return 0;
}

// Требует правки при изменении алфавита
void MakePythonFile (HashTable *plane, HashTable *encr, string password, int alphabetLen)
{
	string nameFile; 
	nameFile = "./plotdata_" + password;
	nameFile += ".py";
	ofstream pyCode(nameFile.c_str());
	
	if ( !pyCode.is_open())
	{
		cout << nameFile << " Питоновский файл не может быть открыт!\n";
	}

	pyCode <<
	"#!/usr/bin/env python" << endl <<
	"" << endl <<
	"import numpy as np" << endl <<
	"import matplotlib.pyplot as plt" << endl <<
	"" << endl;
	
	switch (alphabetLen) 
	{
		case 94:
			pyCode << "N = 94" << endl;
			break;
		case 95:
			pyCode << "N = 95" << endl;
			break;
		case 26:
			pyCode << "N = 26" << endl;// Смотрим только буквы
			break;
		default:
			pyCode << "N = 95" << endl;
			break;
	}

	pyCode << "" << endl <<
	"PlainText = (";
	
	streambuf *coutbuf = cout.rdbuf();
	cout.rdbuf(pyCode.rdbuf());
	switch (alphabetLen) 
	{
		case 94:
			plane->PrintChainsForPythonNoSpace(); //надо поправить
			break;
		case 95:
			plane->PrintChainsForPython();
			break;
		case 26:
			plane->PrintChainsForPython();
			break;
		default:
			plane->PrintChainsForPython();
			break;
	}
	cout.rdbuf(coutbuf);

	pyCode <<
	")" << endl <<
	"" << endl <<
	"CryptedText = (";

	coutbuf = cout.rdbuf();
	cout.rdbuf(pyCode.rdbuf());
	switch (alphabetLen) 
	{
		case 94:
			encr->PrintChainsForPythonNoSpace();
			break;
		case 95:
			encr->PrintChainsForPython();
			break;
		case 26:
			encr->PrintChainsForPython();
			break;			
		default:
			encr->PrintChainsForPython();
			break;
	}
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
	"ax.set_xticks(ind+width)" << endl;

	switch (alphabetLen) 
	{
		case 94:
			pyCode << "ax.set_xticklabels( ('!', '\"', '#', '$', '%', '&', '\\\'', '(', ')', '*', '+', ',', '-', '.', '/', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ':', ';', '<', '=', '>', '?', '@', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '[', '\\\\', ']', '^', '_', '`', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '{', '|', '}', '~') )" << endl;
			break;
		case 95:
			pyCode << "ax.set_xticklabels( (' ', '!', '\"', '#', '$', '%', '&', '\\\'', '(', ')', '*', '+', ',', '-', '.', '/', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ':', ';', '<', '=', '>', '?', '@', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '[', '\\\\', ']', '^', '_', '`', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '{', '|', '}', '~') )" << endl;
			break;
		case 26:
			pyCode << "ax.set_xticklabels( ('a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z') )" << endl;
			break;
		default:
			pyCode << "ax.set_xticklabels( (' ', '!', '\"', '#', '$', '%', '&', '\\\'', '(', ')', '*', '+', ',', '-', '.', '/', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ':', ';', '<', '=', '>', '?', '@', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '[', '\\\\', ']', '^', '_', '`', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '{', '|', '}', '~') )" << endl;
			break;
	}
	
	pyCode << 
	"ax.legend( (rects1[0], rects2[0]), ('PlainText', 'CryptedText') )" << endl <<
	"ax.set_xlim(0,N-width/2)" << endl <<
	"fig.set_size_inches(24,15)" << endl <<
	"plt.savefig('Graph_" <<
	password <<
	".png', format = 'png', bbox_inches='tight')" << endl;
	//".png', format = 'png', bbox_inches='tight', transparent=True)" << endl;
	pyCode.close();
}

void NativeVig(char *buffer, size_t size, vector<unsigned> passOffset, vector<char> alphabet)
{
	size_t textPosition = 0;

	while (textPosition < size)
	{
		for (unsigned i = 0; i < passOffset.size(); i++)
		{	
			if (textPosition >= size)
				break;
			// 'for' c 'if' смотрият, что за символ у нас из алфавита
			for (unsigned k = 0; k < alphabet.size(); k++)//я тупой
			{
				if ( buffer[textPosition] == alphabet[k])
				{

					buffer[textPosition] = alphabet[(k+passOffset[i] )%alphabet.size()];
					break;
				}
			}
			
			textPosition++;
		}
	}

	/*	
	#ifdef DEBUG
	cout << "/----------------------------------------------------------" << endl
		 << buffer << endl
		 << "\\----------------------------------------------------------" << endl 
		 << endl
		 << endl;
	#endif
	*/	
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

		/*	
		#ifdef DEBUG
		cout << "/----------------------------------------------------------" << endl
			 << buffer << endl
			 << "\\----------------------------------------------------------" << endl 
			 << endl
			 << endl;
		#endif
		*/	
	}
}

void Revision_2 (string password, vector<char> alphabet, char *buffer, unsigned long long int size)
{
	unsigned rounds = password.size();

	vector<unsigned> passOffset(password.size());

	for ( unsigned k = 0; k < rounds; k++)
	{
		unsigned long long int run  = 0;

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
		reverse(password.begin(),password.end()); //реверс
		//cout << password << endl;		

		/*	
		#ifdef DEBUG
		cout << "/----------------------------------------------------------" << endl
			 << buffer << endl
			 << "\\----------------------------------------------------------" << endl 
			 << endl
			 << endl;
		#endif
		*/	
	}
}