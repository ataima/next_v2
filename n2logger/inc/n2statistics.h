#ifndef N2_STATISTICS_HEADER
#define N2_STATISTICS_HEADER


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

#include "n2logiface.h"

#if _STATISTICS_

#include <set>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>



typedef struct tag_stat_param
{
    size_t count;
    std::string name;
    std::chrono::steady_clock::duration time;
    std::chrono::steady_clock::time_point last_time;
} IStatParam;

struct IStatCompare
{
    bool operator() (IStatParam *a, IStatParam *b) const
    {
        return a->name<b->name;
    }
};

typedef std::set<IStatParam *,  IStatCompare > db_func;

class nnStatistics
    : public IStatistics
{
    static db_func db;

public:
    nnStatistics();
    ~nnStatistics();

    void addCallIn(const char *_class_,const char *func  );
    void addCallOut(const char *_class_,const char *func  );

    void dumpStatistics(void) final;

};





#endif
#endif
