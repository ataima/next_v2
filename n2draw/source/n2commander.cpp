#include "n2miniXml.h"

#include "n2commander.h"


nnCommander::nnCommander()
{

}

nnCommander::~nnCommander()
{

}


bool nnCommander::readConfiguration(miniXmlNode &node)
{
    int numItem;
    bool res = false;
    miniXmlNode *conf = node.find(X("COMMANDER"));
    if (conf)
    {
        // size of single buttons
        miniXmlNode *t = conf->find(X("DIM"));
        if (t)
        {
            miniXmlNode *v = t->find(X("WIDTH"));
            if(v)
            {
                const_Size.x=v->getLong();
            }
            else
            {
                commanderConfigurationException *pe = new commanderConfigurationException(X("WIDTH"));
                throw (pe);
            }
            v = t->find(X("HEIGHT"));
            if(v)
            {
                const_Size.y=v->getLong();
            }
            else
            {
                commanderConfigurationException *pe = new commanderConfigurationException(X("HEIGHT"));
                throw (pe);
            }
        }
        else
        {
            commanderConfigurationException *pe = new commanderConfigurationException(X("DIM"));
            throw (pe);
        }
        t=conf->find(X("ITEM_NUM"));
        if(t)
        {
            numItem=t->getLong();
        }
        else
        {
            commanderConfigurationException *pe = new commanderConfigurationException(X("ITEM_NUM"));
            throw (pe);
        }
        if(numItem>0)
        {
            int i=0;
            while(numItem>0)
            {
                SSTRING name;
                name<<"ITEM_"<<i;
                t=conf->find(name.str().c_str());
                if(t)
                {
                    commandItem item;
                    miniXmlNode *v = t->find(X("POS_X"));
                    if(v)
                    {
                        item.pos.x=v->getLong();
                    }
                    else
                    {
                        commanderConfigurationException *pe = new commanderConfigurationException(X("POS_X"));
                        throw (pe);
                    }
                    v = t->find(X("POS_Y"));
                    if(v)
                    {
                        item.pos.y=v->getLong();
                    }
                    else
                    {
                        commanderConfigurationException *pe = new commanderConfigurationException(X("POS_Y"));
                        throw (pe);
                    }
                    v = t->find(X("COMMAND"));
                    if(v)
                    {
                        item.command=v->getLong();
                    }
                    else
                    {
                        commanderConfigurationException *pe = new commanderConfigurationException(X("COMMAND"));
                        throw (pe);
                    }
                    items.push_back(item);
                }
                else
                {
                    commanderConfigurationException *pe = new commanderConfigurationException(name.str().c_str());
                    throw (pe);
                }
                i++;
                numItem--;
            }
            res=true;
        }
    }
    return res;
}

bool nnCommander::handlerRequestCommand( nnPoint & pos,int & command)
{
    bool res=false;
    // pos is zero referenced...
    listCommandItem::iterator it=items.begin();
    listCommandItem::iterator end=items.end();
    while(it!=end)
    {
        nnRect rect=rectFromPos(pos);
        if(rect.into(it->pos))
            {
                command=it->command;
                res=true;
                break;
            }
        }
    return res;
}


nnRect nnCommander::rectFromPos(nnPoint & pos)
{
    nnPoint start =(pos/const_Size)*const_Size;
    nnPoint stop = start+const_Size;
    nnRect res(start,stop);
    return res;
}
