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
#include "n2miniXml.h"

#ifndef _MSC_VER
#include <strings.h>
#endif

class n2exception
    :public std::runtime_error
{
public:
    n2exception(const char *what):runtime_error(what) {}
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
    STRING  node;
public:
    explicit appManagerConfigureFileMissingNodeException(STRING  _node) throw()
        :n2exception("appManagerConfigureFileMissingNodeException"), node(_node)
    {

    }
    char const* msg()
    {
        std::stringstream ss;
        UtoA f(node);
        const char *b_msg=n2exception::msg();
        if(f.good())
            ss<<"exception:"<<b_msg<<std::endl<<" Cannot find XML Node :"<<f.utf8()<<std::endl;
        else
            ss<<"exception:"<<b_msg<<std::endl;
        delete b_msg;
        int size=ss.str().size();
        char *buff=new char[size+2];
        memcpy(buff,ss.str().c_str(),size);
        buff[size]='\0';
        return buff;
    }
};

class appManagerConfigureLoadImageException
    :public n2exception
{
public:
    explicit appManagerConfigureLoadImageException() throw()
        :n2exception("appManagerConfigureLoadImageException") {}
};


class appManagerConfigureFileUnknow
    :public n2exception
{
    STRING  file;
public:
    explicit appManagerConfigureFileUnknow(STRING  w) throw()
        :n2exception("appManagerConfigureFileUnknow"),file(w) {}
    char const* msg()
    {
        std::stringstream ss;
        UtoA f(file);
        const char *b_msg=n2exception::msg();
        if(f.good())
            ss<<"exception:"<<b_msg<<std::endl<<" Cannot find or open file :"<<f.utf8()<<std::endl;
        else
            ss<<"exception:"<<b_msg<<std::endl;
        delete b_msg;
        int size=ss.str().size();
        char *buff=new char[size+2];
        memcpy(buff,ss.str().c_str(),size);
        buff[size]='\0';
        return buff;
    }
};

class appManagerConfigureParseXmlFileException
    :public n2exception
{
    STRING  file;
public:
    explicit appManagerConfigureParseXmlFileException(STRING  w) throw()
        :n2exception("appManagerConfigureParseXmlFileException"),file(w) {}

    char const* msg()
    {
        std::stringstream ss;
        UtoA f(file);
        const char *b_msg=n2exception::msg();
        if(f.good())
            ss<<"exception:"<<b_msg<<std::endl<<" Cannot parse configuration file :"<<f.utf8()<<std::endl;
        else
            ss<<"exception:"<<b_msg<<std::endl;
        delete b_msg;
        int size=ss.str().size();
        char *buff=new char[size+2];
        memcpy(buff,ss.str().c_str(),size);
        buff[size]='\0';
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
    explicit  imagesConfigurationNoContactException(void) throw()
        :n2exception("imagesConfigurationNoContactException") {}
};

class imagesConfigurationNoCoilException
    :public n2exception
{
public:
    explicit  imagesConfigurationNoCoilException(void) throw()
        :n2exception("imagesConfigurationNoCoilException") {}
};


class imagesConfigurationListEmptyException
    :public n2exception
{
public:
    explicit  imagesConfigurationListEmptyException(void) throw()
        :n2exception("imagesConfigurationListEmptyException") {}
};


class imagesConfigurationBadSizeException
    :public n2exception
{
    int w, h;
    STRING filename;
public:
    explicit  imagesConfigurationBadSizeException(const XCHAR *_filename,int _w, int _h) throw()
        :n2exception("imagesConfigurationBadSizeException"),filename(_filename), w(_w), h(_h) {}

    char const* msg()
    {
        std::stringstream ss;
        const char *b_msg=n2exception::msg();
        UtoA  f(filename);
        ss<<"exception:"<<b_msg<<std::endl<<" Error bad image ("<<f.utf8()
         <<") size W :"<<w<< " H :"<<h<<std::endl;
        delete b_msg;
        int size=ss.str().size();
        char *buff=new char[size+2];
        memcpy(buff,ss.str().c_str(),size);
        buff[size]='\0';
        return buff;
    }
};


class imagesConfigurationBadFormatException
    :public n2exception
{
public:
    explicit  imagesConfigurationBadFormatException(void) throw()
        :n2exception("imagesConfigurationBadFormatException") {}
};


class imagesConfigurationAlreadyLoadException
    :public n2exception
{
    int offset;
public:
    explicit  imagesConfigurationAlreadyLoadException(int off) throw()
        :n2exception("imagesConfigurationAlreadyLoadException") ,offset(off) {}
};

class imagesConfigurationUnknowFileException
    :public n2exception
{
    STRING  filename;
public:
    explicit  imagesConfigurationUnknowFileException(STRING  _filename) throw()
        :n2exception("imagesConfigurationUnknowFileException"), filename(_filename) {}
    char const* msg()
    {
        std::stringstream ss;
        UtoA f(filename);
        const char *b_msg=n2exception::msg();
        if(f.good())
            ss<<"exception:"<<b_msg<<std::endl<<" Cannot parse configuration file :"<<f.utf8()<<std::endl;
        else
            ss<<"exception:"<<b_msg<<std::endl;
        delete b_msg;
        int size=ss.str().size();
        char *buff=new char[size+2];
        memcpy(buff,ss.str().c_str(),size);
        buff[size]='\0';
        return buff;
    }
};


// no conf phy view
class phyViewConfigurationException
    :public n2exception
{
    STRING  node;
public:
    explicit phyViewConfigurationException(const XCHAR *_node) throw()
        :n2exception("phyGlueConfigurationException"), node(_node) {}

};


// no conf phy view
class phyGlueConfigurationException
    :public n2exception
{
    STRING  node;
public:
    explicit phyGlueConfigurationException(const XCHAR *_node) throw()
        :n2exception("phyGlueConfigurationException"),node(_node) {}

};


// no conf phy view
class commanderConfigurationException
    :public n2exception
{
    STRING  node;
public:
    explicit commanderConfigurationException(const XCHAR *_node) throw()
        :n2exception("commanderConfigurationException"),node(_node) {}

};

#endif // N2EXCEPTION

