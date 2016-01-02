#include "n2miniXml.h"

#include "n2commander.h"
#include "n2exception.h"


nnCommander::nnCommander()
{

}

nnCommander::~nnCommander()
{

}


bool nnCommander::readConfiguration(IXmlNode *conf)
{
    bool res = false;
    if(conf)
    {
        int numItem;
        // size of single buttons
        IXmlNode *t = conf->find(X("DIM"));
        if (t)
        {
            IXmlNode *v = t->find(X("WIDTH"));
            if(v)
            {
                const_Size.x=v->getLong();
            }
            else
            {
                xmlConfigurationNodeException *pe = new xmlConfigurationNodeException(X("WIDTH"));
                throw (pe);
            }
            v = t->find(X("HEIGHT"));
            if(v)
            {
                const_Size.y=v->getLong();
            }
            else
            {
                xmlConfigurationNodeException *pe = new xmlConfigurationNodeException(X("HEIGHT"));
                throw (pe);
            }
        }
        else
        {
            xmlConfigurationNodeException *pe = new xmlConfigurationNodeException(X("DIM"));
            throw (pe);
        }
        t=conf->find(X("ITEM_NUM"));
        if(t)
        {
            numItem=t->getLong();
        }
        else
        {
            xmlConfigurationNodeException *pe = new xmlConfigurationNodeException(X("ITEM_NUM"));
            throw (pe);
        }
        if(numItem>0)
        {
            int i=0;
            while(numItem>0)
            {
                t=conf->find(X("ITEM_"),i);
                if(t)
                {
                    commandItem item;
                    IXmlNode *v = t->find(X("POS_X"));
                    if(v)
                    {
                        item.pos.x=v->getLong();
                    }
                    else
                    {
                        xmlConfigurationNodeException *pe = new xmlConfigurationNodeException(X("POS_X"));
                        throw (pe);
                    }
                    v = t->find(X("POS_Y"));
                    if(v)
                    {
                        item.pos.y=v->getLong();
                    }
                    else
                    {
                        xmlConfigurationNodeException *pe = new xmlConfigurationNodeException(X("POS_Y"));
                        throw (pe);
                    }
                    v = t->find(X("COMMAND"));
                    if(v)
                    {
                        item.command=v->getLong();
                    }
                    else
                    {
                        xmlConfigurationNodeException *pe = new xmlConfigurationNodeException(X("COMMAND"));
                        throw (pe);
                    }
                    items.push_back(item);
                }
                else
                {
                    SSTREAM ss;
                    ss<<X("ITEM_")<<i;
                    xmlConfigurationNodeException *pe = new xmlConfigurationNodeException(ss.str().c_str());
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
