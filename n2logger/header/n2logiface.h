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
    virtual ~IPrinter(){}
};


class ILogger
{
protected:
    static ILogger *instance;
public:
    virtual void log(int level,const char *__fmt, ... ) = 0;
    static  ILogger * getInstance(void) { return instance; }
    virtual void setOutput(IPrinter *printer) = 0;
    virtual IPrinter *output(void) = 0;
    virtual void reset()=0;
    virtual ~ILogger(){}
};


#define	LOG_EMERG	0
#define	LOG_ALERT	1
#define	LOG_CRIT	2
#define	LOG_ERR		3
#define	LOG_WARNING	4
#define	LOG_NOTICE	5
#define	LOG_INFO	6
#define	LOG_DEBUG	7

#define LogE(...)   ILogger::getInstance()->log(LOG_EMERG,__VA_ARGS__)
#define LogA(...)   ILogger::getInstance()->log(LOG_ALERT,__VA_ARGS__)
#define LogC(...)   ILogger::getInstance()->log(LOG_CRIT,__VA_ARGS__)
#define LogW(...)   ILogger::getInstance()->log(LOG_ERR,__VA_ARGS__)
#define LogN(...)   ILogger::getInstance()->log(LOG_NOTICE,__VA_ARGS__)
#define LogI(...)   ILogger::getInstance()->log(LOG_INFO,__VA_ARGS__)
#define LogD(...)   ILogger::getInstance()->log(LOG_DEBUG,__VA_ARGS__)

#else

#define LogE(...)
#define LogA(...)
#define LogC(...)
#define LogW(...)
#define LogN(...)
#define LogI(...)
#define LogD(...)

#endif



#endif // N2_LOG_INTERFACES

