#include "n2logger.h"
#include <chrono>
#include <thread>
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
    if (th != nullptr) {
        done = true;
        cond_var.notify_one();
        while (done) {
            if (th->joinable())
                th->join();
        }
        delete th;
        th = nullptr;
    }
    if (current_printer != nullptr) {
        delete current_printer;
        current_printer = nullptr;
    }
    while (!io.empty()) {
        delete io.front();
        io.pop();
    }
    if (instance == this)
        instance = nullptr;
}

void nnLogger::log(IDebug * param)
{
    //producer...
    if (!done) {
        if (!th) {
            if (current_printer != nullptr) {
                //only first time
                th = new std::thread(nnLogger::entry, this);
            }
        }
        std::unique_lock<std::mutex> lock(mtx);
        io.push(param);
        notify = true;
        cond_var.notify_one();
    }
}


void nnLogger::print(IDebug *p)
{
    if (current_printer ) {
        if (p) {
            std::string msg;
            std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
            std::stringstream ts;
            ts << "[" << (end - t_start).count() << "ns ]";
            t_start = end;
            msg = ts.str();
            p->utf8(msg);
            current_printer->out(msg);
            msg.clear();
        }
    }
}

IDebug * nnLogger::remove(void)
{
    IDebug *param = nullptr;
    std::unique_lock<std::mutex> lock(mtx);
    if (!notify) {
        //cond_var.wait_for(lock, std::chrono::milliseconds(100));
        cond_var.wait(lock);
    }
    if (!io.empty()) {
        param = io.front();
        io.pop();
    }
    notify = !io.empty();
    return param;
}


void nnLogger::enqueue(void)
{

    while(!done) {
        IDebug *param = remove();
        if(!done)
            print(param);
        if(param)
            delete param;
    }
    if (done) {
        std::unique_lock<std::mutex> lock(mtx);
        while (!io.empty()) {
            delete io.front();
            io.pop();
        }
        cond_var.notify_one();
    }
}

void nnLogger::entry(ILogger *current)
{
    if (current) {
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
    if (th != nullptr) {
        th->join();
        delete th;
        th = nullptr;
    }
    if (current_printer != nullptr) {
        delete current_printer;
        current_printer = nullptr;
    }
}


#endif
