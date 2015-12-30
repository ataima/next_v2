#ifndef N2EXCEPTION
#define N2EXCEPTION

/**************************************************************
Copyright(c) 2015 Angelo Coppi

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files(the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and / or sell
copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions :

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
********************************************************************/

#include <string>
#include <stdexcept>
#include <sstream>
#include <vector>
#include <string.h>
#include <algorithm>
#include "n2utoatou.h"


class n2exception
        :public std::runtime_error
{
public:
    n2exception(const char *what):runtime_error(what){}
    virtual const char *msg()
    {
        char *buff=nullptr;
        const char *p=std::runtime_error::what();
        if(p)
        {
            size_t len=strlen(p);
            buff= new char[len+2];
            memcpy(buff,p,len);
            buff[len]='\0';
        }
        return buff;
    }
};


class appManagerConfigureFileMissingNodeException
    :public n2exception
{
    std::wstring node;
public:
    explicit class appManagerConfigureFileMissingNodeException(std::wstring _node) throw()
        :n2exception("appManagerConfigureFileMissingNodeException"), node(_node) {}
};

class appManagerConfigureLoadImageException
    :public n2exception
{
public:
    explicit class appManagerConfigureLoadImageException() throw()
        :n2exception("appManagerConfigureLoadImageException"){}
};


class appManagerConfigureFileUnknow
        :public n2exception
    {
        std::wstring file;
    public:
        explicit class appManagerConfigureFileUnknow(std::wstring w) throw()
            :n2exception("appManagerConfigureFileUnknow"),file(w){}
        char const* what() const
        {
            std::stringstream ss;
            UtoA<_MAX_PATH> f(file);
            if(f.good())
            ss<<"exception:"<<runtime_error::what()<<std::endl<<" Cannot find or open file :"<<f.utf8()<<std::endl;
            else
                ss<<"exception:"<<runtime_error::what()<<std::endl;
            char * buff = new char [ss.str().size()+2];
            memcpy(buff,ss.str().c_str(),ss.str().size());
            buff[ss.str().size()]=0;
            return buff;
        }
    };


class wireConnectionException
    :public n2exception
{
public:
    std::vector<int > up;
    std::vector<int > down;
    explicit wireConnectionException(std::vector<int > _up, std::vector<int > _down) throw()
        :n2exception("wireConnectionException"), up(_up), down(_down) {}

};

class positionBusyException
    :public n2exception
{
public:
    int x, y;
    explicit positionBusyException(int _x, int _y) throw()
        :n2exception("positionBusyException"), x(_x), y(_y) {}
};

class failMemoryException
    :public n2exception
{
public:
    char fun[256];
    int line;
    explicit failMemoryException(const char *funct, int _line) throw()
        :n2exception("failMemoryException"), line(_line) {
        size_t l = strlen(funct);
        size_t t = l < 255 ? l : 255;
        memcpy(fun, funct, t);
        fun[t] = 0;
    }
};


class wireNoAvaiableConnection
    :public n2exception
{
public:
    explicit wireNoAvaiableConnection(void) throw()
        :n2exception("wireNoAvaiableConnection") {}
};


class imagesConfigurationException
    :public n2exception
{
public:
    explicit imagesConfigurationException(void) throw()
        :n2exception("imagesConfigurationException") {}

};


class imagesConfigurationNoWireException
    :public n2exception
{
public:
    explicit imagesConfigurationNoWireException(void) throw()
        :n2exception("imagesConfigurationNoWireException") {}
};


class imagesConfigurationNoContactException
    :public n2exception
{
public:
    explicit class imagesConfigurationNoContactException(void) throw()
        :n2exception("imagesConfigurationNoContactException") {}
};

class imagesConfigurationNoCoilException
    :public n2exception
{
public:
    explicit class imagesConfigurationNoCoilException(void) throw()
        :n2exception("imagesConfigurationNoCoilException") {}
};


class imagesConfigurationListEmptyException
    :public n2exception
{
public:
    explicit class imagesConfigurationListEmptyException(void) throw()
        :n2exception("imagesConfigurationListEmptyException") {}
};


class imagesConfigurationBadSizeException
    :public n2exception
{
    int w, h;
public:
    explicit class imagesConfigurationBadSizeException(int _w, int _h) throw()
        :n2exception("imagesConfigurationBadSizeException"), w(_w), h(_h) {}
};


class imagesConfigurationBadFormatException
    :public n2exception
{
public:
    explicit class imagesConfigurationBadFormatException(void) throw()
        :n2exception("imagesConfigurationBadFormatException") {}
};


class imagesConfigurationAlreadyLoadException
    :public n2exception
{
    int offset;
public:
    explicit class imagesConfigurationAlreadyLoadException(int off) throw()
        :n2exception("imagesConfigurationAlreadyLoadException") ,offset(off) {}
};

class imagesConfigurationUnknowFileException
    :public n2exception
{
    std::wstring filename;
public:
    explicit class imagesConfigurationUnknowFileException(std::wstring _filename) throw()
        :n2exception("imagesConfigurationUnknowFileException"), filename(_filename) {}
};


// no conf phy view
class phyViewConfigurationException
    :public n2exception
{
    std::wstring node;
public:
    explicit phyViewConfigurationException(const wchar_t *_node) throw()
        :n2exception("phyGlueConfigurationException"), node(_node) {}

};


// no conf phy view
class phyGlueConfigurationException
    :public n2exception
{
    std::wstring node;
public:
    explicit phyGlueConfigurationException(const wchar_t *_node) throw()
        :n2exception("phyGlueConfigurationException"),node(_node) {}

};




#endif // N2EXCEPTION

