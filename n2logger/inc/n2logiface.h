#ifndef N2_LOG_INTERFACES
#define N2_LOG_INTERFACES

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
#include <sstream>
#include <vector>
#include <list>
#include <chrono>
#include <syslog.h>


#define _LOGGER_ 1


#if _LOGGER_









class IPrinter
{
public :
    virtual void out( int level, std::string & msg) = 0;
    virtual ~IPrinter() {}
};


class ILogger
{
protected:
    static ILogger *instance;
public:
    virtual void log(int level,const char *__fmt, ... ) = 0;
    static  ILogger * getInstance(void)
    {
        return instance;
    }
    virtual void setOutput(IPrinter *printer) = 0;
    virtual IPrinter *output(void) = 0;
    virtual void reset()=0;
    virtual ~ILogger() {}
};



#define Log0(...)   ILogger::getInstance()->log(0,__VA_ARGS__)
#define Log1(...)   ILogger::getInstance()->log(1,__VA_ARGS__)
#define Log2(...)   ILogger::getInstance()->log(2,__VA_ARGS__)
#define Log3(...)   ILogger::getInstance()->log(3,__VA_ARGS__)
#define Log4(...)   ILogger::getInstance()->log(4,__VA_ARGS__)
#define Log5(...)   ILogger::getInstance()->log(5,__VA_ARGS__)
#define Log6(...)   ILogger::getInstance()->log(6,__VA_ARGS__)


#else

#define Log0(...)
#define Log1(...)
#define Log2(...)
#define Log3(...)
#define Log4(...)
#define Log5(...)
#define Log6(...)

#endif



#endif // N2_LOG_INTERFACES

