#ifndef UTOATOU
#define UTOATOU


#include "n2miniXml.h"

#if  ( _WIN32 || _WIN64 )
#define wctombs wcstombs
#endif


template < int size > class UtoA
{
public:
    UtoA(const std::wstring & v):m_ptr(buff),_good(false)
    {
        if(v.size())
        {
            _good=(0!=wctombs(buff,v.c_str(),size-1));
        }
        else
        {
            _good=true;
            buff[0]='\0';
        }
    }
    inline const char *utf8(){return m_ptr;}
    inline bool good(){return _good;}
private:
    const char *m_ptr;
    char buff[size];
    bool _good;
};

template < int size > class AtoU
{
public:
    AtoU(const std::string & v):m_ptr(buff),_good(false)
    {
        if(v.size())
        {
            _good=(0!= mbstowcs (buff,v.c_str(),size-1));
        }
        else
        {
            _good=true;
            buff[0]='\0';
        }
    }
    inline const char *utf16(){return m_ptr;}
    inline bool good(){return _good;}
private:
    const wchar_t *m_ptr;
    wchar_t buff[size];
    bool _good;
};

#endif // UTOATOU

