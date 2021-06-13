#pragma once
#include "sha1.h"
#include <string.h>
#include<iostream>
#define LOGINLENGTH  10  // Зафиксируем длину имени
//#define SIZE 10


class HashTable { // хэш таблица
public:

	HashTable();
	~HashTable();
	void registration(char login[LOGINLENGTH], char password[], int passwordLenght);// регистрация пользователя
	void registrationInner(char login[LOGINLENGTH], uint* digest);// внутрення функция регистрации пользователя
	void deletingRegistered(char login[LOGINLENGTH]);// удаление пользователя
	bool comparisonLogin(char login[LOGINLENGTH], char password[], int passwordLenght);// проверка логина и пароля
	void showLogin();// выод логинов на экран

private:

	enum  enPairStatus {   // статусы логинов
		free,
		engaged,
		deleted
	};

	struct Pair { // пара логин-пароль

		Pair() : _login(""), _pass_sha1_hash(0), _status(enPairStatus::free)  // изначально всё пусто
		{}
		~Pair()
		{
			if (_pass_sha1_hash != 0)
				delete[] _pass_sha1_hash;
		}
		Pair(char login[LOGINLENGTH], uint* sha1)
		{
			memcpy(_login, login, LOGINLENGTH);
			_pass_sha1_hash = sha1;
			_status = enPairStatus::engaged;
		}
		Pair& operator = (const Pair& other)
		{
			memcpy(_login, other._login, LOGINLENGTH);

			if (_pass_sha1_hash != 0)
				delete[] _pass_sha1_hash;
			_pass_sha1_hash = new uint[SHA1HASHLENGTHUINTS];

			memcpy(_pass_sha1_hash, other._pass_sha1_hash, SHA1HASHLENGTHBYTES);

			_status = other._status;

			return *this;
		}

		bool operator == (const Pair& other) {
			return  _status == other._status &&
				(_status != enPairStatus::engaged || (!memcmp(_pass_sha1_hash, other._pass_sha1_hash, SHA1HASHLENGTHBYTES) && !strcmp(_login, other._login)));
		}

		char _login[LOGINLENGTH];  // хранение логина
		uint* _pass_sha1_hash;     // хэш пароля

		enPairStatus _status;       // статус ячейки массива логинов-пароля
	};

	
	int hashFunction(char login[LOGINLENGTH], int step); // поиск индекса для логина
	void resize();                                        // изменение размера массива пар логин-пароль 

	Pair* _arrayData;  // массив пар логин-пароль
	int _memSize;       // размер массива пар логин-пароль
	int _countLogin;    // количество пользователей 
};