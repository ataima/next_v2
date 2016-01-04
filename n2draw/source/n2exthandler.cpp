#include "n2exthandler.h"
#include "n2exception.h"

nnExtHandler::nnExtHandler(handler_exec _type,extHandler & _hook, void *unkObj)
    :type(_type),hook(_hook),unknow(unkObj)
{}


void nnExtHandler::doHandler(size_t param )
{
    if(hook && unknow)
    {
        try {
            hook(unknow,param);
        }
        catch(...)
        {
            extHandlerException *e = new extHandlerException(type);
            throw(e);
        }
    }
}



////////////////////////////////////////////////////////

bool nnExtHandlerList::add(unsigned int type,IExtHandler *handler)
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
