#include "n2miniXml.h"

#include "n2commander.h"
#include "n2exception.h"
#include "n2imagemanager.h"
#include "images.h"

nnCommander::nnCommander()
{
    images= new nnImageManager();
}

nnCommander::~nnCommander()
{
    if(images!=nullptr)
    {
        delete images;
        images=nullptr;
    }
}


bool nnCommander::readConfiguration(IXmlNode *conf)
{
    bool res = false;
    if(conf)
    {
        int numItem;
        IXmlNode *t=conf->find(X("ITEM_NUM"));
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
                    v = t->find(X("MASK"));
                    if(v)
                    {
                        IXmlNode *u = v->find(X("RED"));
                        if(u)
                        {
                            item.maskR=u->getLong()&0xff;
                        }
                        else
                        {
                            xmlConfigurationNodeException *pe = new xmlConfigurationNodeException(X("RED"));
                            throw (pe);
                        }
                        u = v->find(X("GREEN"));
                        if(u)
                        {
                            item.maskG=u->getLong()&0xff;
                        }
                        else
                        {
                            xmlConfigurationNodeException *pe = new xmlConfigurationNodeException(X("GREEN"));
                            throw (pe);
                        }
                        u = v->find(X("BLUE"));
                        if(u)
                        {
                            item.maskB=u->getLong()&0xff;
                        }
                        else
                        {
                            xmlConfigurationNodeException *pe = new xmlConfigurationNodeException(X("BLUE"));
                            throw (pe);
                        }
                    }
                    else
                    {
                        xmlConfigurationNodeException *pe = new xmlConfigurationNodeException(X("MASK"));
                        throw (pe);
                    }
                    v = t->find(X("FILE"));
                    if(v)
                    {
                        item.file =v->getValue();
                    }
                    else
                    {
                        xmlConfigurationNodeException *pe = new xmlConfigurationNodeException(X("FILE"));
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
        nnPoint start=pos;
        start+=it->pos;
        nnRect rect=rectFromPos(start,it->command);
        if(rect.into(it->pos))
        {
            command=it->command;
            res=true;
            break;
        }
        it++;
    }
    return res;
}


nnRect nnCommander::rectFromPos(nnPoint & pos,int command)
{
    nnPoint dim;
    nnPoint stop = pos;
    if(images)
    {
        bmpImage *image=images->getImage(command);
        if(image)
        {
            dim.x=image->getWidth();
            dim.y=image->getHeight();
        }
        stop+=dim;
    }
    nnRect res(pos,stop);
    return res;
}

bool nnCommander::loadImages(const XCHAR *path)
{
    bool res=false;
    if(images)
    {
        if(images->setPath(path))
        {
            objImageList toLoad;
            for(auto i: items)
            {
                toLoad[i.command]=i.file;
            }
            res=images->loadImages(&toLoad);
        }
    }
    return res;
}

bmpImage *nnCommander::getImage(int command)
{
    bmpImage * res=nullptr;
    if(images)
    {
        res=images->getImage(command);
    }
    return res;
}
