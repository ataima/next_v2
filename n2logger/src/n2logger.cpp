#include "n2logger.h"
#include <chrono>
#include <thread>
#include <stdarg.h>
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

#if _LOGGER_

#include "n2logger.h"
#include <ctime>
#include <iomanip>



ILogger * ILogger::instance = nullptr;



nnLogger::nnLogger() :
    notify(false),
    done(false),
    th(nullptr),
    current_printer(nullptr)
{
    if (instance == nullptr)
        instance = this;
    t_start = std::chrono::steady_clock::now();
}

nnLogger::~nnLogger()
{
    if (th != nullptr)
    {
        done = true;
        cond_var.notify_one();
        while (done)
        {
            if (th->joinable())
                th->join();
        }
        delete th;
        th = nullptr;
    }
    if (current_printer != nullptr)
    {
        delete current_printer;
        current_printer = nullptr;
    }
    while (!io.empty())
    {
        delete io.front();
        io.pop();
    }
    if (instance == this)
        instance = nullptr;
}

void nnLogger::log(int level,const char *fmt, ... )
{
    char dest[0x3fff];
    va_list argptr;
    //producer...
    ILogParam *param= new ILogParam;
    param->time = std::chrono::steady_clock::now();
    if (!done)
    {
        if (!th)
        {
            if (current_printer != nullptr)
            {
                //only first time
                th = new std::thread(nnLogger::entry, this);
            }
        }
        std::unique_lock<std::mutex> lock(mtx);
        va_start(argptr, fmt);
        vsnprintf(dest,sizeof(dest), fmt, argptr);
        va_end(argptr);
        param->level=level;
        param->msg=dest;
        io.push(param);
        notify = true;
        cond_var.notify_one();
    }
}


void nnLogger::print(ILogParam *p)
{
    if (current_printer )
    {
        if (p)
        {
            std::string msg;
            std::stringstream ts;
            ts << "<"<<p->level<<"> "<<"[" << (p->time - t_start).count() << "ns ] "<<p->msg;
            t_start = p->time;
            msg = ts.str();
            current_printer->out(p->level,msg);
            msg.clear();
        }
    }
}

ILogParam * nnLogger::remove(void)
{
    ILogParam *param = nullptr;
    std::unique_lock<std::mutex> lock(mtx);
    if (!notify)
    {
        //cond_var.wait_for(lock, std::chrono::milliseconds(100));
        cond_var.wait(lock);
    }
    if (!io.empty())
    {
        param = io.front();
        io.pop();
    }
    notify = !io.empty();
    return param;
}


void nnLogger::enqueue(void)
{

    while(!done)
    {
        ILogParam *param = remove();
        if(param)
        {
            print(param);
            delete param;
        }
    }
    if (done)
    {
        std::unique_lock<std::mutex> lock(mtx);
        while (!io.empty())
        {
            delete io.front();
            io.pop();
        }
        cond_var.notify_one();
    }
}

void nnLogger::entry(ILogger *current)
{
    if (current)
    {
        nnLogger *log = dynamic_cast<nnLogger*>(current);
        if(log)
            log->enqueue();
    }
}



void nnLogger::reset()
{
    std::unique_lock<std::mutex> lock(mtx);
    done = true;
    cond_var.notify_one();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    cond_var.wait(lock);
    if (th != nullptr)
    {
        th->join();
        delete th;
        th = nullptr;
    }
    if (current_printer != nullptr)
    {
        delete current_printer;
        current_printer = nullptr;
    }
}


#define WHITE "\e[1;37m"
#define GRAY_LIGHT "\e[0;37m"
#define GRAY_DARK "\e[1;30m"
#define BLUE "\e[0;34m"
#define BLUE_LIGHT "\e[1;34m"
#define GREEN "\e[0;32m"
#define GREEN_LIGHT "\e[1;32m"
#define CYAN "\e[0;36m"
#define CYAN_LIGHT "\e[1;36m"
#define RED "\e[0;31m"
#define RED_LIGHT "\e[1;31m"
#define PURPLE "\e[0;35m"
#define PURPLE_LIGHT "\e[1;35m"
#define BROWN "\e[0;33m"
#define YELLOW "\e[1;33m"
#define BLACK "\e[0;30m"
#define REPLACE "\e[0m"

#include <iostream>

void nnDefaultPrinter::out( int level,const std::string & msg )
{

    const char * colors[]=
    {
        RED_LIGHT,BLUE_LIGHT,PURPLE_LIGHT,
        RED,GREEN,WHITE,GRAY_DARK,BROWN,nullptr
    };
    if(level<0)level=0;
    if(level>LOG_DEBUG)level=LOG_DEBUG+1;
    fprintf(stdout,"%s%s%s\n",colors[level],msg.c_str(),REPLACE);
}




#endif
