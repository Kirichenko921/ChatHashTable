#include "chat.h"


HashTable::HashTable() {
    _countLogin = 0;
    _memSize = 8;        // начальный размер массива 
    _arrayData = new Pair[_memSize];
 }
HashTable::~HashTable() {
    delete[] _arrayData;
}
void HashTable::registration(char login[], char password[], int passwordLenght)
{
    uint* digest = sha1(password, passwordLenght);  // вычисляем хэш пароля через  алгоритм SHA1
    registrationInner(login, digest);              // чтобы второй раз не вычислять хэш, если массив будет занят
}

void  HashTable::registrationInner(char login[LOGINLENGTH], uint* digest)//регистрация
{
    int index = -1, i = 0; 

    for (; i < _memSize; i++)  // ищем пустую ячейку
    {
        index = hashFunction(login, i * i);  // определяем индекс массива для логина через хэш-функцию 
        if (_arrayData[index]._status == enPairStatus::free) // найдена пустая ячейка, занимаем ее 
        {
                       break;
        }
    }

    if (i >= _memSize)  // если прошли по всему массиву и не нашли свободной ячейки
    {
        resize();  // изменяем размер массива 
        registrationInner(login, digest); // и снова ищем место для логина
    }

    else  // записывам данные 
    {
        _arrayData[index] = Pair(login, digest);  
        _arrayData[index]._status = engaged;
        _countLogin++;
    }

}

void HashTable::deletingRegistered(char login[LOGINLENGTH])//удаление логина
{
    int index, i = 0;
    for (; i < _memSize; i++) // поиск индекса в массиве
    {
        index = hashFunction(login, i * i); 
        if (_arrayData[index]._status == enPairStatus::deleted ||_arrayData[index]._status == enPairStatus::free) // если ячейка пустая или удалённая
            return;
        else if (_arrayData[index]._status == enPairStatus::engaged && !memcmp(login, _arrayData[index]._login, LOGINLENGTH)) // если совпал удаляем
        {
            _arrayData[index]._status = enPairStatus::deleted;
            return;
        }
    }
}
   

   
bool HashTable::comparisonLogin(char login[LOGINLENGTH], char password[], int passwordLenght)// сличение введённой пары логин-пароль с сущесвующей в массиве
{

    uint* digest = sha1(password, passwordLenght);//генериуем хэш пароля
    int index = -1, i = 0;

    for (; i < _memSize; i++) // ищем логин
    {
        index = hashFunction(login, i * i);
        if (!strcmp(login, _arrayData[index]._login)) // если нашли
        {
            if (!memcmp(digest, _arrayData[index]._pass_sha1_hash, SHA1HASHLENGTHBYTES) && _arrayData[index]._status == engaged) // сличаем хэши паролей и проверяем статус
            return true;
            else
                return false;
        }
        
    }
    // если логин не нашли
    return false;
}
void HashTable::showLogin()  //печать существующих логинов
{
    for (int i = 0; i < _memSize; i++)
    {
        if (_arrayData[i]._status == engaged)
            std::cout << _arrayData[i]._login << std::endl;
    }
}
int HashTable::hashFunction(char login[LOGINLENGTH], int step)// хэш-функция для индекса массива пар логин -пароль
{
    // вычисляем индекс
    int sum = 0, i = 0;
    for (; i < LOGINLENGTH; i++)
    {
        if (login[i] == '\0')
            break;
        sum += login[i];
    }

    return (sum % _memSize + step) % _memSize;
}
void HashTable::resize() // изменение размера массива пар логин-пароль 
{
    Pair* arraySave = new Pair[_memSize]; // запоминаем старый массив
    int oldSize = _memSize;// и старый размер массива
    for (int i = 0; i < _memSize; ++i)
        arraySave[i] = _arrayData[i];
    _memSize *= 2;  // увеличиваем размер в два раза  
    _countLogin = 0; // обнуляем количество элементов
    _arrayData = new Pair[_memSize]; // выделяем новую память

    for (int i = 0; i < oldSize; i++) //заполнем новый массив 
    {
        Pair& oldData = arraySave[i];
        if (oldData._status == enPairStatus::engaged)
        {
            uint* shaHashRewritable = new uint[SHA1HASHLENGTHUINTS];
            memcpy(shaHashRewritable, oldData._pass_sha1_hash, SHA1HASHLENGTHBYTES);

            registrationInner(oldData._login, shaHashRewritable);
            delete shaHashRewritable;
        }
        // чистим за собой
        delete[] arraySave;
        
    }
}
