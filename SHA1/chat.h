#pragma once
#include "sha1.h"

#include <string.h>

#define LOGINLENGTH  10  // Зафикируем длину имени
#define SIZE 10


class HashTable { // хэш таблица
public:

	HashTable();
	~HashTable();
	void registration(char login[LOGINLENGTH], char password[], int passwordLenght);
	void registrationInner(char login[LOGINLENGTH], uint* digest);
	void deletingRegistered(char login[LOGINLENGTH]);
private:

	enum enPairStatus {
		free,
		engaged,
		deleted
	};

	struct Pair { // пара логин-пароль

		Pair() : _login(""), _pass_sha1_hash(0), _status(enPairStatus::free)
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

		char _login[LOGINLENGTH];
		uint* _pass_sha1_hash;

		enPairStatus _status;
	};

	void resize();
	int hashFunction(char login[LOGINLENGTH], int step);


	Pair* _arrayData;
	int _memSize;
	int _countLogin;
};