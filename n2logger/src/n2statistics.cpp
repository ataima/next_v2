#include "n2logiface.h"
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

#if _STATISTICS_

#include <chrono>
#include <thread>
#include <stdarg.h>

#include "n2statistics.h"
#include <ctime>
#include <iomanip>

IStatistics *IStatistics::instance = nullptr;

db_func nnStatistics::db;

nnStatistics::nnStatistics()
{
    if (instance == nullptr)
        instance = this;
}

nnStatistics::~nnStatistics()
{
    if (instance == this)
        instance = nullptr;
    db.clear();
}

void nnStatistics::addCallIn(const char *c, const char *func)
{
    std::chrono::steady_clock::time_point t_start;
    std::string called(c);
    called += "::";
    called += func;
    Log6("%s", called.c_str());
    IStatParam temp;
    temp.name = called;
    db_func::iterator it = db.find(&temp);
    t_start = std::chrono::steady_clock::now();
    if (it == db.end())
    {
        IStatParam *param = new IStatParam;
        if (param)
        {
            param->count = 1;
            param->last_time = t_start;
            param->name = called;
            db.insert(param);
        }
    }
    else
    {
        IStatParam *param = *it;
        param->last_time = t_start;
        param->count += 1;
    }
}


void nnStatistics::addCallOut(const char *c, const char *func)
{
    std::chrono::steady_clock::time_point t_start;
    std::string called(c);
    called += "::";
    called += func;
    IStatParam temp;
    temp.name = called;
    db_func::iterator it = db.find(&temp);
    t_start = std::chrono::steady_clock::now();
    if (it == db.end())
    {
        std::string msg("ERROR addCallOut without class::function name!!");
        ILogger::getInstance()->output()->out(8,msg);
    }
    else
    {
        IStatParam *param = *it;
        std::chrono::steady_clock::duration call_time=t_start-param->last_time;
        param->time = (param->time+call_time)/2;
    }
}



void nnStatistics::dumpStatistics(void)
{
    std::stringstream ss;
    ss<<std::setw(40)<<"FUNC. NAME |"<<std::setw(18)<<"NUM. CALL |"<<std::setw(24)<<"AVERAGE TIME |";
    ILogger::getInstance()->output()->out(0,ss.str());
    for (auto i : db)
    {
        IStatParam *param = i;
        ss.str("");
        ss<<std::setw(40)<<param->name<<" |"<<std::setw(18)<<param->count<<" |"<<std::setw(24)<<param->time.count()<<" |";
        ILogger::getInstance()->output()->out(7,ss.str());
        delete param;
    }
}

#endif
