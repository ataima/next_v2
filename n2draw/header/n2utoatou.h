#ifndef UTOATOU
#define UTOATOU


#include <codecvt>
#include <locale>


#if  ( _WIN32 || _WIN64 )
#define wctombs wcstombs
#endif

#ifdef _UNICODE

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
#else
class UtoA
{
public:
    inline UtoA(const std::string & v)
    {
        u8=v.c_str();
    }
    inline const char *utf8(){return u8;}
    inline bool good(){return true;}
private:
    const char *u8;
};
#endif

#ifdef _UNICODE
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

#else
class AtoU
{
public:
    AtoU(const std::string & v)
    {
        u16=v.c_str();
    }
    inline const char *utf16(){return u16;}
    inline bool good(){return true;}
private:
    const char *u16;
};

#endif
#endif // UTOATOU

