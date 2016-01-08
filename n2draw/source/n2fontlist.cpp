#include "images.h"
#include "n2fontlist.h"
#include "n2fontmanager.h"
#include "n2exception.h"

#if (_WIN32 || _WIN64)
#define getcwd  _getcwd
#endif

#ifndef _MSC_VER
#include <unistd.h>
#define _MAX_PATH  4096
#else
#include <direct.h>
#endif


nnFontList::nnFontList()
{

}

nnFontList::~nnFontList()
{
    clear();
}

void nnFontList::clear(void)
{
    for (auto i : fonts)
    {
        IFontManager *t = i.second;
        if (t)
        {
            delete t;
        }
    }
    fonts.clear();
}


bool nnFontList::readConfiguration(IXmlNode *node)
{
    bool res = false;
    STRING name;
    STRING path;
    IXmlNode *conf = node->find(X("FONTS"));
    if (conf)
    {
        IXmlNode *xpath = conf->find(X("PATH"));
        if (xpath)
        {
            char buff[_MAX_PATH];
            const XCHAR *v = xpath->getValue();
            if (getcwd(buff, _MAX_PATH))
            {
                std::string cpath(buff);
                AtoU toU(cpath);
                path = toU.utf16();
                path += X("/");
                path += v;
                path += X("/");
            }
            else
            {
                path = v;
            }
        }
        int numItems = 0;
        IXmlNode *items = conf->find(X("ITEMS"));
        if (items)
        {
            numItems = items->getLong();
        }
        else
        {
            xmlConfigurationNodeException *e = new xmlConfigurationNodeException(X("ITEMS"));
            throw(e);
        }
        if (numItems > 0)
        {

            int i = 0;
            do
            {
                IXmlNode *it = items->find(X("ITEM_"), i);
                if (it)
                {
                    IXmlNode *t = it->find(X("NAME"));
                    if (t)
                    {
                        name = t->getValue();
                        UtoA toA(name);
                        if (toA.good())
                        {
                            path += X("/");
                            path += toA.utf8();
                            path += X("/");
                            IFontManager *mn= new nnFontManager(path.c_str());
                            fonts[toA.utf8()] = mn;
                            if (mn)
                                res=mn->readConfiguration(it);
                        }
                    }
                    else
                    {
                        xmlConfigurationNodeException *e = new xmlConfigurationNodeException(X("NAME"));
                        throw(e);
                    }
                }
                i++;
            } while (i < numItems);
        }
        else
        {
            xmlConfigurationNodeValueException *e = new xmlConfigurationNodeValueException(numItems);
            throw(e);
        }
    }
    else
    {
        xmlConfigurationNodeException *e = new xmlConfigurationNodeException(X("FONTS"));
        throw(e);
    }
    return res;
}

bool nnFontList::loadImages(void)
{
    bool res = false;
    for (auto i : fonts)
    {
        res = i.second->loadImages();
        if (!res)break;
    }
    return res;
}

IFontManager* nnFontList::getManager(const char *name)
{
    IFontManager * res = nullptr;
    std::string _name(name);
    fontList::iterator it = fonts.find(name);
    if (it != fonts.end())
    {
        res = it->second;
    }
    return res;
}

bool  nnFontList::add(const char *name, IFontManager* manager)
{
    bool res = false;
    std::string _name(name);
    fontList::iterator it = fonts.find(_name);
    if (it != fonts.end())
    {
    fonts[_name] = manager;
    res=true;
    }
    return res;
}

bool nnFontList::remove(const char *name)
{
    bool res = false;
    std::string _name(name);
    fontList::iterator it = fonts.find(_name);
    if (it != fonts.end())
    {
        IFontManager *t = it->second;
        if (t)
        {
            delete t;
            fonts.erase(it);
            res = true;
        }
    }
    return res;
}

bool nnFontList::getFontNameList(fontNameList & list)
{
    bool res = false;
    for (auto i: fonts)
    {
        std::string name = i.first;
        list.push_back(name);
    }
    res = (list.size() > 0);
    return res;
}
