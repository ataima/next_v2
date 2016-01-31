#include "n2miniXml.h"

#include "n2commander.h"
#include "n2exception.h"
#include "n2imagemanager.h"
#include "images.h"


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
{
    images= new nnImageManager();
    curItem=nullptr;
    bmpHeight = 0;
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
                    commandItem item = {0};
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
                    v = t->find(X("INFO"));
                    if(v)
                    {
                        item.info =v->getValue();
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

bool nnCommander::handlerMouseMove( nnPoint & phyPoint,IExtHandler *hook)
{
    bool res=false;
    nnPoint pos = phyPoint;
    pos.y = bmpHeight - pos.y;
    if(curItem==nullptr)
    {
        listCommandItem::iterator it=items.begin();
        listCommandItem::iterator end=items.end();
        while(it!=end)
        {
            if(it->rect.into(pos))
            {
                curItem=&(*it);
                if (hook)
                {
                    hook->doHandler(action_update_statusbars_info, (size_t)(curItem->info.c_str()));
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
                hook->doHandler(action_update_statusbars_info, (size_t)" ... ");
                hook->doHandler(action_redraw);
            }
            res=true;
        }
    }
    return res;
}



bool nnCommander::handlerRequestCommand( nnPoint & pos,int & command)
{
    bool res=false;
    listCommandItem::iterator it=items.begin();
    listCommandItem::iterator end=items.end();
    while(it!=end)
    {
        if(it->rect.into(pos))
        {
            command=it->command;
            res=true;
            break;
        }
        it++;
    }
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




bool nnCommander::draw(bmpImage & bkg, nnPoint & pos, IViewGlue * glue)
{    
    bool res = false;
    listCommandItem::iterator it = items.begin();
    listCommandItem::iterator _end = items.end();
    bmpHeight  = bkg.getHeight();
    while (it != _end)
    {
        bmpImage & icon = *images->getImage(it->command);
        unsigned int x = pos.x + it->pos.x;
        unsigned int y = bmpHeight - (pos.y + it->pos.y);
        res = bkg.drawMaskSprite(icon, x, y, it->maskR, it->maskG, it->maskB);
        it->rect.set(x, y, x + icon.getWidth(), y + icon.getHeight());
        if (!res)
            break;
        it++;
    }
    if (curItem != nullptr)
    {
        drawTips(bkg, pos, glue);
    }
    res = (it == _end);
    return res;
}


bool nnCommander::drawTips(bmpImage & bkg, nnPoint & pos, IViewGlue *glue)
{
    (glue);
    bool res = false;
    if (curItem != nullptr && font != nullptr)
    {
        int len = strlen(curItem->info.c_str());
        nnPoint sizeStr(font->getFontWidth()* len, font->getFontHeight());
        const int offsetX = 20;
        const int offsetY = 4;
        bmpImage rectbkg;
        res=rectbkg.create(sizeStr.x+2*offsetX, sizeStr.y+2*offsetY,32, 255);
        if (res)
        {
            bmpImage * strImage = font->getImage(curItem->info.c_str(), 0, 0, 255);
            res = rectbkg.drawMaskSprite(*strImage,offsetX,offsetY,0,0,0);
            delete strImage;
            if(res)
            {
                rectbkg.frameRect(0, 0, rectbkg.getWidth()-1, rectbkg.getHeight()-1, 0, 0, 0,0xffffffff);
                int posX = ( bkg.getWidth() - rectbkg.getWidth()-offsetX);
                res = bkg.drawSprite(rectbkg, posX, offsetY);
            }
        }
    }

    return res;
}
