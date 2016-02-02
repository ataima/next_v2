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
    if(v==0)//default...
    {
        res= nnAppManager::getInstance();
    }
    return res;
}




int nnAppManager::UID = 1;
IAppManager *nnAppManager::instance=nullptr;

nnAppManager::nnAppManager():selected(-1)
{
    configuration = new xmlConfig();
    if(instance==nullptr)
        instance=this;
}

nnAppManager::~nnAppManager()
{
    clean();
    delete configuration;
    configuration=nullptr;
    instance=nullptr;
}


IAppManager *nnAppManager::getInstance(void)
{
    if(instance==nullptr)
    {
        instance = new nnAppManager();
    }
    return instance;
}

IChild * nnAppManager::createObjects(STRING & conf_file_name)
{
    bool res = false;
    IChild * child = new nnChildApp(UID);
    MEMCHK(IChild, child);
    try
    {
        res = child->createObjects(configuration,conf_file_name);
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





bool nnAppManager::closeAll(void)
{
    clean();
    selected = -1;
    return true;
}


IChild *nnAppManager::activate(int v)
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


IChild *nnAppManager::active(void)
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








bool nnAppManager::clean(void)
{
    for (auto i : childs)
    {
        i.second->clean();
    }
    return childs.empty();
}



