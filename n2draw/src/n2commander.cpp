#include "n2miniXml.h"

#include "n2commander.h"
#include "n2exception.h"
#include "n2imagemanager.h"
#include "n2utils.h"


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





nnCommander::nnCommander()
    :curItem(nullptr),
     bmpHeight(0),
     bmpWidth(0)
{
    STRING empty;
    images= new nnImageManager(empty);
}

nnCommander::~nnCommander()
{
    curItem=nullptr;
    if(images!=nullptr)
    {
        delete images;
        images=nullptr;
    }
    bmpHeight = 0;
    bmpWidth = 0;
    space.set(0, 0);
    for (auto i : items)
    {
        delete i;
    }
}


bool nnCommander::readConfiguration(IXmlNode *conf)
{
    bool res = false;
    TFUNCI();
    if(conf)
    {
        int numItem;
        maxExt.set(0, 0);
        IXmlNode *t=conf->find(X("SPACE_X"));
        if(t)
        {
            space.x=t->getLong();
        }
        else
        {
            xmlConfigurationNodeException *pe = new xmlConfigurationNodeException(X("SPACE_X"));
            throw (pe);
        }
        t = conf->find(X("SPACE_Y"));
        if (t)
        {
            space.y = t->getLong();
        }
        else
        {
            xmlConfigurationNodeException *pe = new xmlConfigurationNodeException(X("SPACE_Y"));
            throw (pe);
        }
        t = conf->find(X("ITEM_NUM"));
        if (t)
        {
            numItem = t->getLong();
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
                    commandItem *item = new commandItem();
                    IXmlNode *v = t->find(X("POS_X"));
                    if(v)
                    {
                        int u;
                        item->pos.x=u=v->getLong();
                        if (u < 0)
                            u = -u;
                        if (u > maxExt.x)
                            maxExt.x = u;
                    }
                    else
                    {
                        xmlConfigurationNodeException *pe = new xmlConfigurationNodeException(X("POS_X"));
                        throw (pe);
                    }
                    v = t->find(X("POS_Y"));
                    if(v)
                    {
                        int u;
                        item->pos.y=u=v->getLong();
                        if (u < 0)
                            u = -u;
                        if (u > maxExt.y)
                            maxExt.y = u;
                    }
                    else
                    {
                        xmlConfigurationNodeException *pe = new xmlConfigurationNodeException(X("POS_Y"));
                        throw (pe);
                    }
                    v = t->find(X("COMMAND"));
                    if(v)
                    {
                        item->command=v->getLong();
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
                            item->maskR=u->getLong()&0xff;
                        }
                        else
                        {
                            xmlConfigurationNodeException *pe = new xmlConfigurationNodeException(X("RED"));
                            throw (pe);
                        }
                        u = v->find(X("GREEN"));
                        if(u)
                        {
                            item->maskG=u->getLong()&0xff;
                        }
                        else
                        {
                            xmlConfigurationNodeException *pe = new xmlConfigurationNodeException(X("GREEN"));
                            throw (pe);
                        }
                        u = v->find(X("BLUE"));
                        if(u)
                        {
                            item->maskB=u->getLong()&0xff;
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
                        item->file=v->getValue();
                    }
                    else
                    {
                        xmlConfigurationNodeException *pe = new xmlConfigurationNodeException(X("FILE"));
                        throw (pe);
                    }
                    v = t->find(X("INFO"));
                    if(v)
                    {
                        item->info=v->getValue();
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
            maxExt += 1;
            res=true;
        }
    }
    TFUNCO();
    return res;
}

bool nnCommander::handlerMouseMove( nnPoint & phyPoint,IExtHandler *hook)
{
    bool res=false;
    TFUNCI();
    nnPoint pos = phyPoint;
    pos.y = bmpHeight - pos.y;
    if(curItem==nullptr)
    {
        listCommandItem::iterator it=items.begin();
        listCommandItem::iterator end=items.end();
        while(it!=end)
        {
            if((*it)->rect.into(pos))
            {
                curItem=(*it);
                if (hook)
                {
                    STRING info =curItem->info;
                    auto *t= new nnAbstractParam<STRING>(info);
                    hook->doHandler(action_update_statusbars_info, t);
                    hook->doHandler(action_redraw);
                }
                res=true;
                break;
            }
            it++;
        }
    }
    else
    {
        if(curItem->rect.into(pos)==false)
        {
            curItem=nullptr;
            if (hook)
            {
                std::string sm("...");
                auto *t= new nnAbstractParam<std::string>(sm);
                hook->doHandler(action_update_statusbars_info, t);
                hook->doHandler(action_redraw);
            }
            res=true;
        }
    }
    TFUNCO();
    return res;
}



bool nnCommander::checkRequestCommand( nnPoint & pos,int & command)
{
    bool res=false;
    TFUNCI();
    listCommandItem::iterator it=items.begin();
    listCommandItem::iterator end=items.end();
    log_verbose("current click mouse position : %d", pos);
    while(it!=end)
    {
        if((*it)->btRect.into(pos))
        {
            command=(*it)->command;
            log_verbose("ITEM : %d", (*it)->command);
            res=true;
            break;
        }
        it++;
    }
    TFUNCO();
    return res;
}




bool nnCommander::loadImages(STRING  &path)
{
    bool res=false;
    TFUNCI();
    if(images)
    {
        if(images->setPath(path))
        {
            listCommandItem *co=&items;
            res=images->loadImages(co);
        }
    }
    TFUNCO();
    return res;
}




bool nnCommander::draw(bmpImage & bkg, nnPoint & pos, IViewGlue * glue)
{
    bool res = false;
    TFUNCI();
    log_verbose("pos.x=%d pos.y=%d",pos.x,pos.y);
    listCommandItem::iterator it = items.begin();
    listCommandItem::iterator _end = items.end();
    bmpHeight  = bkg.getHeight();
    bmpWidth = bkg.getWidth();
    nnPoint quadrant;
    getQuadrant(pos, quadrant);
    while (it != _end)
    {
        commandItem *cur=*it;
        int spX = (images->getMaxWidth() + space.x);
        int spY = (images->getMaxHeight() + space.y);
        bmpImage & icon = *images->getImage(cur->command);
        unsigned int x = pos.x + cur->pos.x*spX +quadrant.x*spX-spX/2;
        unsigned int y1 = pos.y + cur->pos.y*spY+quadrant.y*spY+spY/2;
        unsigned int y = bmpHeight - y1;
        unsigned int icon_width = icon.getWidth();
        unsigned int icon_height = icon.getHeight();
        res = bkg.drawMaskSprite(icon, x, y, cur->maskR, cur->maskG, cur->maskB);
        cur->rect.set(x, y, x + icon_width, y + icon_height);
        cur->btRect.set(x, y1- icon_height, x + icon_width, y1 );
        if (!res)
            break;
#if 0
        bkg.frameRect(it->btRect.start.x, bkg.getHeight() - it->btRect.start.y,
                      it->btRect.stop.x, bkg.getHeight() - it->btRect.stop.y, 255, 0, 0, 0xffffffff);
#endif
        it++;
    }
    //nnLOG1(commandItem *, curItem);
    if (curItem != nullptr)
    {
        drawTips(bkg, pos, glue);
    }
    res = (it == _end);
    TFUNCO();
    return res;
}


bool nnCommander::drawTips(bmpImage & bkg, nnPoint &p, IViewGlue *view)
{

    bool res = false;
    TFUNCI();
    log_verbose("pos.x=%d pos.y=%d",p.x,p.y);
    if (curItem != nullptr && font != nullptr)
    {
        UtoA toA(curItem->info);
        std::string s = toA.utf8();
        res = nnUtils::drawBottomLeftTips(bkg, *font, s );
    }
    TFUNCO();
    return res;
}


void nnCommander::getQuadrant(nnPoint & pos, nnPoint & res)
{
    int divX = bmpWidth / 5;
    int divY = bmpHeight / 5;
    int pX = pos.x / divX;
    int pY = pos.y / divY;
    res.set(0, 0);
    TFUNCI();
    log_verbose("pos.x=%d pos.y=%d",pos.x,pos.y);
    if (pX == 0)
        res.x = maxExt.x;
    if (pX == 4)
        res.x = -maxExt.x;
    if (pY == 0)
        res.y = maxExt.y;
    if (pY == 4)
        res.y = -maxExt.y;
    log_verbose("res.x=%d res.y=%d",res.x,res.y);
    TFUNCO();
}
