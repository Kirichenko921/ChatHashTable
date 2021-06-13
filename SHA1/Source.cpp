#include "chat.h"
#include  <vector>
#include<iostream>
#include <string>

int main()
{

	HashTable a; // данные логинов чата
	std::string login;  // ввод логина 
	std::string password; // ввод пароля 
	
	char clogin[LOGINLENGTH]; // для передачи логина в методы класса
	char cpassword[50];   // для передачи пароля в методы класса
	int choice;           // выбор - регистрация или вход 

	while (std::cin)  // программа будет работать пока пользователь не закроет терминал
	{
		std::cout << "\n Registration - 1, input - 2 \n"; // предлагаем регистрация илли вход
		std::cin >> choice;
		std::cin.ignore(32767, '\n');
		if (choice == 1)// регистрация
		{
			//(std::cin >> inputWord).get();// если не будет работать cin раскоментить эту строку
			std::cout << "\n Registration login\n";
			getline(std::cin, login);
			//std::cin.ignore(32767, '\n');
			std::cout << "\n Registration password\n";
			getline(std::cin, password);
		}
		else  // вход
		{
			std::cout << "\n Enter your login\n";
			getline(std::cin, login);
			//std::cin.ignore(32767, '\n');
			std::cout << "\n Enter your password\n";
			getline(std::cin, password);
		}
		// копируем логин и пароль в массивы char
		login.copy(clogin, LOGINLENGTH);
		clogin[login.length()] = '\0';
		password.copy(cpassword, 50);
		cpassword[password.length()] = '\0';
			if (choice == 1) // если регистрируемся
			{
				a.registration(clogin, cpassword, password.length()); // метод регистрации
				a.showLogin();                                        // вывод лнового списка логинов
			}
		
		
			else   // если входим проверяем пару логин - пароль
			{
				if (a.comparisonLogin(clogin, cpassword, password.length()))
					std::cout << "\n !!!OK!!! \n";
				else
					std::cout << "\n invalid username or password \n";
			}
			//очищаем данные в логинах и паролях
			login.clear();
			password.clear();
			login.copy(clogin, LOGINLENGTH);
			clogin[login.length()] = '\0';
			password.copy(cpassword, 50);
			cpassword[password.length()] = '\0';

		
	}
	return 0;
}