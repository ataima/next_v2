#include "n2exthandler.h"
#include "n2exception.h"
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

nnExtHandler::nnExtHandler(handler_exec _type,extHandler & _hook, void *unkObj)
    :type(_type),hook(_hook),unknow(unkObj)
{}


void nnExtHandler::doHandler(handlerAction Tparam , size_t Uparam)
{
    if(hook && unknow)
    {
        try {
            hook(unknow,Tparam,Uparam);
        }
        catch(...)
        {
            extHandlerException *e = new extHandlerException(type);
            throw(e);
        }
    }
}



////////////////////////////////////////////////////////

bool nnExtHandlerList::add( handler_exec type,IExtHandler *handler)
{
    bool res=false;
    iterator it= find(type);
    if(it==end())
    {
        std::pair<unsigned int,IExtHandler *> key(type,handler);
        insert(key);
        res=true;
    }
    else
    {
        IExtHandler * old=it->second;
        if(old)
        {
            delete old;
        }
        it->second=handler;
        res=true;
    }
    return res;
}

bool nnExtHandlerList::remove(unsigned int type)
{
    bool res=false;
    iterator it= find(type);
    if(it!=end())
    {
        IExtHandler *old;
        old=it->second;
        if(old)
        {
            delete old;
        }
        erase(it);
    }
    return res;
}

bool nnExtHandlerList::clear(void)
{
    bool res=false;
    iterator it=begin();
    iterator _end=end();
    while(it!=_end)
    {
        IExtHandler *old;
        old=it->second;
        if(old)
        {
            delete old;
        }
        it++;
    }
    std::map<unsigned int , IExtHandler *>::clear();
    res=empty();
    return res;
}

IExtHandler *nnExtHandlerList::get(unsigned int type)
{
    IExtHandler *res=nullptr;
    iterator it= find(type);
    if(it!=end())
    {
        res=it->second;
    }
    return res;
}