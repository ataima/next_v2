#ifndef N2_LOGGER_HEADER
#define N2_LOGGER_HEADER


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

#include "n2interfaces.h"

#if _LOGGER_
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>


class nnLogger
    : public ILogger
{

     std::chrono::steady_clock::time_point t_start;
     std::mutex mtx;
     std::condition_variable cond_var;
     std::queue<IDebug *> io;
     bool notify;
     bool done;
     std::thread *th;
     IPrinter * current_printer;
private:
    void print(IDebug *p);
    IDebug * remove(void);
public:
    nnLogger();
    ~nnLogger();
    void log(IDebug *param);
    static void entry(ILogger *current);
    void enqueue(void);
    inline  void setOutput(IPrinter *printer) { current_printer = printer; }
    inline IPrinter *output(void) { return current_printer; }
    void reset();

};



#endif
#endif
