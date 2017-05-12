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
#include "string.h"
#include "n2interfaces.h"
#include "n2utoatou.h"



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


class xmlConfigurationNodeException
    :public n2exception
{
    STRING  node;
public:
    explicit xmlConfigurationNodeException(STRING  _node) throw()
        :n2exception("xmlConfigurationNodeException"), node(_node)
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


class xmlConfigurationNodeValueException
    :public n2exception
{
    int  value;
public:
    explicit xmlConfigurationNodeValueException(int  _value) throw()
        :n2exception("xmlConfigurationNodeValueException"), value(_value)
    {

    }
    char const* msg()
    {
        std::stringstream ss;
        const char *b_msg = n2exception::msg();
        ss << "exception:" << b_msg << std::endl << " Inavlid XML Node Value:" << value << std::endl;
        delete b_msg;
        int size = ss.str().size();
        char *buff = new char[size + 2];
        memcpy(buff, ss.str().c_str(), size);
        buff[size] = '\0';
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


class wireConnetComponentException
    :public n2exception
{
public:
    explicit wireConnetComponentException() throw()
        :n2exception("wireConnetComponentException"){}

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




class imagesConfigurationListEmptyException
    :public n2exception
{
public:
    explicit  imagesConfigurationListEmptyException(void) throw()
        :n2exception("imagesConfigurationListEmptyException") {}
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
    char const* msg()
    {
        std::stringstream ss;
        const char *b_msg=n2exception::msg();
        ss<<"exception:"<<b_msg<<std::endl<<" image at offset : "<<offset<<"already loaded"<< std::endl;
        delete b_msg;
        int size=ss.str().size();
        char *buff=new char[size+2];
        memcpy(buff,ss.str().c_str(),size);
        buff[size]='\0';
        return buff;
    }
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
            ss<<"exception:"<<b_msg<<std::endl<<" Cannot open image file :"<<f.utf8()<<std::endl;
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





class xmlBadFormatException
        :public n2exception
{
public:
    explicit xmlBadFormatException(void) throw()
        :n2exception("xmlBadFormatException") {}
    char const* msg()
    {
        const char *info="exception:xmlBadFormatException\n On unicode the xml must be in LITTLE ENDIAN mode";
        int len=strlen(info);
        char *buff=new char [len+2];
        memcpy(buff,info,len);
        buff[len]='\0';
        return info;
    }
};



class extHandlerException
    :public n2exception
{
public:
    explicit  extHandlerException() throw()
        :n2exception("extHandlerException") {}
    char const* msg()
    {
        std::stringstream ss;
        const char *b_msg=n2exception::msg();
        ss<<"exception:"<<b_msg<<std::endl<<" An exception is occurred on requested handler";
        ss<<std::endl;
        delete b_msg;
        int size=ss.str().size();
        char *buff=new char[size+2];
        memcpy(buff,ss.str().c_str(),size);
        buff[size]='\0';
        return buff;
    }
};


#endif // N2EXCEPTION

