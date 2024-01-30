#include "n2draw.h"
#include "n2miniXml.h"
#include "n2drawmanager.h"
#include "n2imagemanager.h"
#include "n2view.h"
#include "n2viewglue.h"
#include "n2exception.h"
#include "n2fontmanager.h"
#include "n2fontlist.h"
#include "n2exthandler.h"
#include "n2childapp.h"
#include "n2appmanager.h"
#include "n2logger.h"
#include "n2resource.h"


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

IAppManager *IAppManager::getInstance(int v)
{
    IAppManager *res=nullptr;
    if(v==0)   //default...
    {
        res= n2AppManager::getInstance();
    }
    return res;
}




int n2AppManager::UID = 1;
IAppManager *n2AppManager::instance=nullptr;

n2AppManager::n2AppManager():selected(-1)
{
    configuration = new xmlConfig();
    nnResource::Init();
    if(instance==nullptr)
        instance=this;
}

n2AppManager::~n2AppManager()
{
    closeAll();
    delete configuration;
    configuration=nullptr;
    instance=nullptr;
}


IAppManager *n2AppManager::getInstance(void)
{
    return instance;
}

IChild * n2AppManager::create(STRING & conf_file_name)
{
    STRING res="resource/";
    return createObjects(conf_file_name,res);
}


IChild * n2AppManager::createObjects(STRING & conf_file_name,STRING & path_name)
{
    bool res = false;
    IChild * child = new nnChildApp(UID);
    MEMCHK(IChild, child);
    try
    {
        res = child->createObjects(configuration,conf_file_name,path_name);
    }
    catch (n2exception *e)
    {
        child->clean();
        delete child;
        child = nullptr;
        throw(e);
    }
    if(res==false)
    {
        child->clean();
        delete child;
        child = nullptr;
    }
    if (child != nullptr)
    {
        selected = static_cast<int>(UID);
        childs[UID] = child;
        UID++;
    }
    // OK is Valid;
    return child;
}


#if _LOGGER_
void n2AppManager::setPrinter(IPrinter * printer)
{
    ILogger *current_logger = ILogger::getInstance();
    if (current_logger == nullptr)
    {
        current_logger = new nnLogger();
    }
    if (current_logger)
    {
        current_logger->setOutput(printer);
    }
}
#endif


bool n2AppManager::closeAll(void)
{
    if (childs.size() > 0)
    {
#if _LOGGER_
        if (nnLogger::getInstance())
        {
            nnLogger::getInstance()->reset();
            delete nnLogger::getInstance();
        }
#endif
        clean();
        selected = -1;
    }
    return true;
}


IChild *n2AppManager::activate(int v)
{
    IChild * res = nullptr;
    if (!childs.empty())
    {
        listChild::iterator it = childs.find(v);
        if (it != childs.end())
        {
            res = it->second;
            selected = v;
            res->getView()->updateDraw();
        }
    }
    return res;
}


IChild *n2AppManager::active(void)
{
    IChild * res = nullptr;
    if (!childs.empty() && selected>0)
    {
        listChild::iterator it = childs.find(selected);
        if (it != childs.end())
        {
            res = it->second;
        }
    }
    return res;
}








bool n2AppManager::clean(void)
{
    for (auto i : childs)
    {
        i.second->clean();
        delete i.second;
    }
    childs.clear();
    return childs.empty();
}



