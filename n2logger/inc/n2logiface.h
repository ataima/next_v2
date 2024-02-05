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
#define _STATISTICS_ 1

#if _LOGGER_









class IPrinter
{
public :
    virtual void out( int level, const std::string & msg  ) = 0;
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

class IStatistics
{
protected:
    static IStatistics *instance;
public:
    virtual void addCallIn(const char *_class_,const char *func  ) = 0;
    virtual void addCallOut(const char *_class_,const char *func  ) = 0;
    static  IStatistics * getInstance(void)
    {
        return instance;
    }
    virtual void dumpStatistics(void) = 0;
    virtual ~IStatistics() {}

};


#define Log0(FORMAT,...)   ILogger::getInstance()->log(0,FORMAT,__VA_ARGS__) //PANIC
#define Log1(FORMAT,...)   ILogger::getInstance()->log(1,FORMAT,__VA_ARGS__) //EMERG
#define Log2(FORMAT,...)   ILogger::getInstance()->log(2,FORMAT,__VA_ARGS__) //ERROR
#define Log3(FORMAT,...)   ILogger::getInstance()->log(3,FORMAT,__VA_ARGS__) //WARNING
#define Log4(FORMAT,...)   ILogger::getInstance()->log(4,FORMAT,__VA_ARGS__) //INFO
#define Log5(FORMAT,...)   ILogger::getInstance()->log(5,FORMAT,__VA_ARGS__) //VERBOSE
#define Log6(FORMAT,...)   ILogger::getInstance()->log(6,FORMAT,__VA_ARGS__) //DEBUG


#define log_panic(FORMAT,...)      Log0("PANIC: %s::%s : "#FORMAT,toString(),__func__,__VA_ARGS__)
#define log_emerg(FORMAT,...)      Log1("EMERG: %s::%s : "#FORMAT,toString(),__func__,__VA_ARGS__)
#define log_error(FORMAT,...)      Log2("ERROR: %s::%s : "#FORMAT,toString(),__func__,__VA_ARGS__)
#define log_warning(FORMAT,...)    Log3("WARNING: %s::%s : "#FORMAT,toString(),__func__,__VA_ARGS__)
#define log_info(FORMAT,...)       Log4("INFO: %s::%s : "#FORMAT,toString(),__func__,__VA_ARGS__)
#define log_verbose(FORMAT,...)    Log5("VERBOSE: %s::%s : "#FORMAT,toString(),__func__,__VA_ARGS__)

#else

#define Log0(...)
#define Log1(...)
#define Log2(...)
#define Log3(...)
#define Log4(...)
#define Log5(...)

#define TFUNC()


#define log_panic(...)
#define log_emerg(...)
#define log_error(...)
#define log_warning(...)
#define log_info(...)
#define log_verbose(...)

#endif



#if _STATISTICS_
#define TFUNCI()  IStatistics::getInstance()->addCallIn(toString(),__func__)
#define TFUNCO()  IStatistics::getInstance()->addCallOut(toString(),__func__)
#else
#define TFUNCI()
#define TFUNCO()
#endif


#endif // N2_LOG_INTERFACES

