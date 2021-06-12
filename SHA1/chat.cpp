#include "chat.h"


HashTable::HashTable() {
    _countLogin = 0;
    _memSize = 8;
    _arrayData = new Pair[_memSize];
 }
HashTable::~HashTable() {
    delete[] _arrayData;
}
void HashTable::registration(char login[], char password[], int passwordLenght)
{
    uint* digest = sha1(password, passwordLenght);
    registrationInner(login, digest);
}

void  HashTable::registrationInner(char login[LOGINLENGTH], uint* digest)
{
    int index = -1, i = 0;

    for (; i < _memSize; i++)
    {
        index = hashFunction(login, i * i);
        if (_arrayData[index]._status == enPairStatus::free)
        {
            // найдена пустая ячейка, занимаем ее
            break;
        }
    }

    if (i >= _memSize)
    {
        resize();
        registrationInner(login, digest);
    }

    else
    {
        _arrayData[index] = Pair(login, digest);
        _countLogin++;
    }

}

void HashTable::deletingRegistered(char login[LOGINLENGTH])
{
    int index, i = 0;
    for (; i < _memSize; i++) {
        index = hashFunction(login, i * i);
        if (_arrayData[index]._status == enPairStatus::deleted ||_arrayData[index]._status == enPairStatus::free)
            return;
        else if (_arrayData[index]._status == enPairStatus::engaged && !memcmp(login,_arrayData[index]._login, LOGINLENGTH))
                       break;
    }
    if (i >= _memSize) 
        return;

    _arrayData[index]._status = enPairStatus::deleted;
}
int HashTable::hashFunction(char login[LOGINLENGTH], int step)
{
    // вычисляем индекс
    int sum = 0, i = 0;
    for (; i < LOGINLENGTH; i++)
    {
        sum += login[i];
    }
   
    return (sum % _memSize + step) % _memSize;
}
void HashTable::resize() {

    Pair* arraySave = new Pair[_memSize]; // запоминаем старый массив
    int oldSize = _memSize;
       for (int i = 0; i < _memSize; ++i)
        arraySave[i] = _arrayData[i];
    _memSize *= 2;  // увеличиваем размер в два раза  
    _countLogin = 0; // обнуляем количество элементов
    _arrayData = new Pair[_memSize]; // выделяем новую память
    
    for (int i = 0; i < oldSize; i++) 
    {
        Pair& oldData = arraySave[i];
        if (oldData._status == enPairStatus::engaged)
        {     uint* shaHashRewritable = new uint[SHA1HASHLENGTHUINTS];
        memcpy(shaHashRewritable, oldData._pass_sha1_hash, SHA1HASHLENGTHBYTES);

        registrationInner(oldData._login, shaHashRewritable);
         }
    // чистим за собой
    delete[] arraySave;
}
   
   
  

  

   