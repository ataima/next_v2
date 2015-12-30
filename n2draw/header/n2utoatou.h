#ifndef UTOATOU
#define UTOATOU


#include "n2miniXml.h"
#include <codecvt>
#include <locale>


#if  ( _WIN32 || _WIN64 )
#define wctombs wcstombs
#endif


class UtoA
{
public:
    UtoA(const std::u16string & v):_good(false)
    {
        std::wstring_convert<std::codecvt_utf8_utf16<char16_t>,char16_t> converter;
        if(v.size())
        {
            u8 = converter.to_bytes(v);
            if(u8.size())
            {
            _good=true;
            }
        }
        else
        {
           _good=true;
           u8.clear();
        }
    }
    inline const char *utf8(){return u8.c_str();}
    inline bool good(){return _good;}
private:
    std::string u8;
    bool _good;
};

class AtoU
{
public:
    AtoU(const std::string & v):_good(false)
    {
        std::wstring_convert<std::codecvt_utf8_utf16<char16_t>,char16_t> converter;
        if(v.size())
        {
            u16 = converter.from_bytes(v);
            if(u16.size())
            {
            _good=true;
            }
        }
        else
        {
            _good=true;
            u16.clear();
        }
    }
    inline const char16_t *utf16(){return u16.c_str();}
    inline bool good(){return _good;}
private:
    std::u16string u16;
    bool _good;
};

#endif // UTOATOU

