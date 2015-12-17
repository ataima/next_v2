
#include "n2draw.h"
#include "n2drawmanager.h"
#include "n2imagemanager.h"
#include "n2view.h"
#include "n2viewglue.h"

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

bool nnTextView::draw(IManager * manager, void * context)
{
    if (manager != nullptr)
    {
        size_t h, w;
        nnPoint start = manager->getStartPoint();
        nnPoint stop = manager->getStopPoint();
        for (h = start.y; h <= stop.y; h++)
        {
            for (w = start.x; w <= stop.x; w++)
            {
                InnObj *obj = manager->getObj(w, h);
                if (obj != nullptr)
                {
                    if (obj->isComponent())
                    {
                        if (obj->getContext() == ObjContext::objCoil)
                        {
                            std::cout << "O";
                        }
                        else
                            if (obj->getContext() == ObjContext::objContact)
                            {
                                std::cout << "=";
                            }
                    }
                    else
                    {
                        nnObjWire *wire = dynamic_cast<nnObjWire*>(obj);
                        switch (wire->getWire())
                        {
                        case noWire:
                            std::cout << ".";
                            break;
                        case wireHorizzontal:
                            std::cout << "-";
                            break;
                        case wireVertical:
                            std::cout << "|";
                            break;
                        case wireAngleUpRight:
                            std::cout << "<";
                            break;
                        case wireAngleUpLeft:
                            std::cout << ">";
                            break;
                        case wireAngleDownRight:
                            std::cout << "[";
                            break;
                        case wireAngleDownLeft:
                            std::cout << "]";
                            break;
                        case wireTHorizDown:
                            std::cout << "T";
                            break;
                        case wireTHorizUp:
                            std::cout << "L";
                            break;
                        case wireTVertRight:
                            std::cout << "{";
                            break;
                        case wireTVertLeft:
                            std::cout << "}";
                            break;
                        case wireCross:
                            std::cout << "+";
                            break;
                        }
                    }
                }
                else
                {
                    std::cout << " ";
                }
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

    return true;
}

nnView::nnView(IImageManager *_images) :
    images(_images)
{
}

nnView::~nnView()
{
}

bool nnView::draw(IManager * manager, void * context)
{
    bool res = false;
    size_t x, y;
    size_t ix, iy;
    nnViewGlue *glue = (nnViewGlue *)(context);
    x = y = -1;
    if (glue != nullptr)
    {
        nnObjManager & mn = *dynamic_cast<nnObjManager*>(manager);
        nnPoint off = glue->getOffsetView();
        nnPoint map = glue->getMap();
        hashObjTable::iterator it = mn.begin();
        hashObjTable::iterator end = mn.end();
        if (it != end)
        {
            do
            {
                mn.revIndexes((hashkey)(it->first), ix, iy);               
            } while (ix < off.x && iy < off.y && it != end);
        }
        InnObj *obj;
        res = true;
        bool t = false;
        for (y = 0; y < map.y; y++)
        {
            for (x =0 ; x <map.x; x++)
            {
                if (ix == x && iy == y)
                {
                    obj = it->second;
                    res &= drawObj(obj, x, y, glue);
                    it++;
                    if(it!=end)
                        mn.revIndexes((hashkey)(it->first), ix, iy);
                }
                else
                {
                    res &= drawBkg(x, y, glue);
                }
            }            
        }
    }
    return res;
}

bool nnView::readConfiguration(miniXmlNode * node)
{
    bool res = false;
    miniXmlNode *t = node->find(X("PARALLELISM"));
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
    }
    else
    {
        phyViewConfigurationException *pe = new phyViewConfigurationException(X("PARALLELISM"));
        throw (pe);
    }
    return res;
}

bool nnView::createMainBitmap(size_t w, size_t h)
{
    bool res = false;
    res = page.create(w, h, 0);
    return res;
}

bool nnView::drawObj(InnObj * obj, size_t & x, size_t & y, IViewGlue * glue)
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
    if (images != nullptr)
    {
        nnPoint pos = glue->getMirrorCoordPhy(x, y);
        const listImage *mapImage = images->getImageList();
        if (mapImage)
        {
            listImage::const_iterator it = mapImage->find(nImage);
            if (it != mapImage->end())
            {
                res = page.drawSprite(*it->second, pos.x, pos.y);
            }
        }

    }
    return res;
}

bool nnView::drawBkg(size_t & x, size_t & y, IViewGlue * glue)
{
    bool res = false;
    if (images != nullptr)
    {

        nnPoint pos = glue->getMirrorCoordPhy(x, y);
        const listImage *mapImage = images->getImageList();
        if (mapImage)
        {
            listImage::const_iterator it = mapImage->find(0);
            if (it != mapImage->end())
            {
                res = page.drawSprite(*it->second, pos.x, pos.y);
            }
        }

    }
    return res;
}


