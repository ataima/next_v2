
#include "n2draw.h"
#include "n2drawmanager.h"
#include "n2imagemanager.h"
#include "n2view.h"
#include "n2viewglue.h"

#include "n2exception.h"
#include <iostream>


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

nnView::nnView(IChild *_parent) :
    n_thread(1), parent(_parent), font(nullptr),
    Width(-1),Height(-1)
{
}

nnView::~nnView()
{    
}

bmpImage & nnView::getMainBitmap(void)
{
    copy.copyFrombmpImage(page);
    return copy;
}


bool nnView::draw(void)
{
    bool res = false;
    bool empty = false;
    int x, y;
    int ix, iy;
    ix = iy = 0;
    nnPoint map = {0};
    if (parent)
    {
        IManager *manager = parent->getManager();
        IViewGlue * glue = parent->getView();
        if (glue != nullptr)
        {
            nnObjManager & mn = *dynamic_cast<nnObjManager*>(manager);
            nnPoint off = glue->getOffsetView();
            nnPoint map = glue->getMap();
            empty = mn.empty();
            hashObjTable::iterator it = mn.begin();
            hashObjTable::iterator end = mn.end();
            if (!empty)
            {
                if (it != end)
                {
                    do
                    {
                        mn.revIndexes(const_cast<hashkey&>(it->first), ix, iy);
                    } while (ix < off.x && iy < off.y && it != end);
                }
            }
            InnObj *obj;
            res = true;
            map += off;
            for (y = off.y; y < map.y; y++)
            {
                for (x = off.x; x < map.x; x++)
                {
                    if (empty == false && ix == x && iy == y)
                    {
                        obj = it->second;
                        res &= drawObj(obj, x, y, glue);
                        it++;
                        if (it != end)
                            mn.revIndexes(const_cast<hashkey&>(it->first), ix, iy);
                    }
                    else
                    {
                        res &= drawBkg(x, y, glue);
                    }
                }
            }
        }
        if (res)
        {
            drawPower(glue);
        }
    }
    return res;
}

bool nnView::readConfiguration(IXmlNode *node)
{
    bool res = false;
    IXmlNode *t = node->find(X("THREADS"));
    if (t)
    {
        n_thread = t->getLong();
        if (n_thread > 8)
        {
            n_thread = 8;
        }
        if (n_thread < 1)
        {
            n_thread = 1;
        }
        res=true;
    }
    else
    {
        xmlConfigurationNodeException *pe = new xmlConfigurationNodeException(X("PARALLELISM"));
        throw (pe);
    }
    return res;
}

bool nnView::createMainBitmap(nnPoint & size)
{
    bool res = false;
    res = page.create(size.x, size.y,24, 192);
    Width = page.getWidth();
    Height = page.getHeight();
    return res;
}


bool nnView::remapMainBitmap(nnPoint & size)
{
    page.clear();
    return createMainBitmap(size);
}

bool nnView::drawObj(InnObj * obj, int & x, int & y, IViewGlue * glue)
{
    bool res = false;
    unsigned int nImage;
    ObjContext context = obj->getContext();
    if (context == objWire)
    {
        InnWire *wire = dynamic_cast<InnWire *>(obj);
        nImage = (unsigned int)wire->getWire();
    }
    else
    {
        nnObjComponent * comp = dynamic_cast<nnObjComponent *>(obj);

        nImage = (unsigned int)comp->getCustomization();
    }
    if (parent)
    {
        IImageManager * images = parent->getImage();
        if (images)
        {
            nnPoint pos = glue->getMirrorCoordPhy(page.getHeight(), x, y);
            bmpImage *sprite = images->getImage(nImage);
            if (sprite)
            {
                res = page.drawSprite(*sprite, (int)pos.x, (int)pos.y);
            }

        }
    }
    return res;
}

bool nnView::drawBkg(int & x, int & y, IViewGlue * glue)
{
    bool res = false;
    if (parent)
    {
        IImageManager * images = parent->getImage();
        if (images != nullptr)
        {
            nnPoint pos = glue->getMirrorCoordPhy(page.getHeight(),x, y);
            bmpImage *sprite=images->getImage(0);
            if(sprite)
            {
                    res = page.drawSprite(*sprite, (int)pos.x, (int)pos.y);
            }
#if 0
        char buff[128];
        sprintf(buff, "%d:%d", x, y);
        bmpImage * strImage = font->getImage(buff, 32, 32, 32);
        res = page.drawMaskSprite(*strImage, pos.x+5, pos.y +20, 0, 0, 0);
        delete strImage;
#endif
        }
    }
    return res;
}


void nnView::drawPower(IViewGlue * glue)
{
    if(glue)
    {
        nnPoint const_p = glue->getConstPhy();
        nnPoint map = glue->getMap();
        nnPoint offset = glue->getOffsetView();
        int W = map.x*const_p.x;
        int H = map.y * const_p.y;
        if (H >= Height)
        {
            page.frameRect(1, Height - 2, W - 1, Height - 1, 255, 0, 0, 0xffffffff);
            page.frameRect(1, Height - H, W - 1, Height - H - 1, 0, 0, 255, 0xffffffff);
        }
        else
        {
            if(offset.y==0)
                page.frameRect(1, Height - 2, W - 1, Height - 1, 255, 0, 0, 0xffffffff);
            if (map.y - offset.y <= Height / const_p.y)
                page.frameRect(1, Height - H, W - 1, Height - H - 1, 0, 0, 255, 0xffffffff);
        }
    }
}