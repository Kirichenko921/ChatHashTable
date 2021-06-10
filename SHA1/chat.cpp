#include "chat.h"
#include "iostream"
#include "string.h"

Chat::Chat() {
    data_count = 0;
}
void Chat::reg(char _login[LOGINLENGTH], char _pass[], int pass_length) {
    // Ваш код
    uint* a = sha1(_pass, sizeof(char) * pass_length);
    data[data_count++] = AuthData(_login, a);
}
bool Chat::login(char _login[LOGINLENGTH], char _pass[], int pass_length) 
{
    // Ваш код
    // Нужно вернуть true в случае успешного логина
    for (int i = 0; i < SIZE; i++) 
    {
        if (!strcmp(data[i].login, _login)) 
        {
            uint* a = sha1(_pass, sizeof(char) * pass_length);
            if (! memcmp( data[i].pass_sha1_hash,a, SHA1HASHLENGTHBYTES))
                return true;
            else
                return false;
            
        }
    }
    return false;
                 
}