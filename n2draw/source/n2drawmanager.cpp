#include "n2drawmanager.h"
#include <algorithm>
#include <iostream>
#include <vector>

nnObjManager::nnObjManager(size_t x, size_t y) 
    : v_width(x), v_height(y),
      mask_width(0), mask_height(0)
{
    nnObjWire::resetUI();
    size_t i = v_width&0x0fffffff;
    mask_width = 1;
    while (i > 0)
    {
        i >>= 1;
        mask_width <<= 1;
        mask_width |= 1;
    }
    i = v_height & 0x0fffffff;
    mask_height = 1;
    while (i > 0)
    {
        i >>= 1;
        mask_height <<= 1;
        mask_height |= 1;
    }

}

InnObj * nnObjManager::getObj(size_t x, size_t y)
{
    nnObj * res = nullptr;
    hashkey hkey;
    if (range(x, y))
        if (genHashKey(x, y, hkey)) {
            hashObjTable::iterator it = find(hkey);
            if (it != end()) {
                res = dynamic_cast<nnObj *> (it->second);
            }
        }
    return res;
}

bool nnObjManager::addObj(size_t x, size_t y, InnObj * obj)
{
    bool res = false;
    hashkey hkey;
    if (range(x, y) && obj != nullptr)
        if (genHashKey(x, y, hkey)) {
            hashObjTable::iterator it = find(hkey);
            if (it == end()) {
                std::pair<hashkey, InnObj *> p(hkey, obj);
                insert(p);
                obj->setXpos(x);
                obj->setYpos(y);
                res = linkObj(x, y, obj);
             }
        }
    return res;
}

bool nnObjManager::removeObj(size_t x, size_t y)
{
    bool res = false;
    hashkey hkey;
    if (range(x, y))
        if (genHashKey(x, y, hkey)) {
            hashObjTable::iterator it = find(hkey);
            if (it != end()) {
                InnObj * obj = it->second;
                if (obj != nullptr) {
                    erase(it); // remove first break recursion
                    unlinkObj(x, y, obj);
                    delete obj;
                    res = true;
                }
            }
        }
    return res;
}


InnObj * nnObjManager::outObj(size_t x, size_t y)
{
    InnObj * obj = nullptr;
    hashkey hkey;
    if (range(x, y))
        if (genHashKey(x, y, hkey)) {
            hashObjTable::iterator it = find(hkey);
            if (it != end()) {
                obj = it->second;
                if (obj != nullptr) {
                    erase(it); // remove first break recursion
                    unlinkObj(x, y, obj);				
                }
            }
        }
    return obj;
}


bool nnObjManager::replaceObj(size_t x, size_t y, InnObj * obj)
{
    bool res = false;
    hashkey hkey;
    if (range(x, y) && obj != nullptr)
        if (genHashKey(x, y, hkey)) {
            hashObjTable::iterator it = find(hkey);
            if (it != end()) {
                delete it->second;
                it->second = obj;
                obj->setXpos(x);
                obj->setYpos(y);
                if (it->second->isComponent()) {
                    linkObj(x, y, obj);
                    res = true;
                }
            }
        }
    return res;
}

bool nnObjManager::removeAll(void)
{
    bool res = false;
    if (size() > 0) {
        for (auto n : * this) {
            delete n.second;
        }
        res = true;
        clear();
    }
    return res;
}

bool nnObjManager::setupPower(void)
{
    bool res = false;
    size_t i;
    for (i = 0; i < v_width; i++) {
        nnPowerUp *up = new nnPowerUp();
        nnPowerDown *down = new nnPowerDown();
        if (up != nullptr)
            res = addObj(i, 0, up);
        if (!res)break;
        if (down != nullptr)
            res = addObj(i, v_height - 1, down);
        if (!res)break;
    }
    return res;
}

bool nnObjManager::genHashKey(size_t x, size_t y, hashkey &key)
{
    bool res = false;
    x <<= 1;
    y <<= 1;
    x |= 1;
    y |= 1;
    x &= mask_width;
    y &= mask_height;
    key = x;
    key <<= 29;
    key |= y;
    res = key != 0;
    return res;
}

bool nnObjManager::range(size_t x, size_t y)
{
    bool res = false;
    if (x < v_width && y < v_height)
        res = true;
    return res;
}

bool nnObjManager::linkObj(size_t x, size_t y, InnObj *obj)
{
    bool res = false;
    if (obj != nullptr)
    {
        InnObj *neighbourUp = nullptr;
        InnObj *neighbourDw = nullptr;
        if (y > 0) 
        {
            neighbourUp = getObj(x, y - 1);
        }
        if (y + 1 < v_height)
        {
            neighbourDw = getObj(x, y + 1);
        }
        
        if (obj->isComponent())
        {
            obj->setUpNeighbour(neighbourUp);
            obj->setDownNeighbour(neighbourDw);
            if (neighbourUp != nullptr)
            {
                neighbourUp->setDownNeighbour(obj);
            }
            if (neighbourDw != nullptr)
            {
                neighbourDw->setUpNeighbour(obj);
            }
        }
        else
        {
            if (neighbourUp != nullptr && neighbourDw != nullptr)
            {
                if (neighbourUp->isComponent() && !neighbourDw->isComponent())
                {
                    obj->setDownNeighbour(neighbourDw);
                    neighbourDw->setUpNeighbour(obj);
                }
                else
                    if (!neighbourUp->isComponent() && neighbourDw->isComponent())
                    {
                        obj->setUpNeighbour(neighbourUp);
                        neighbourUp->setDownNeighbour(obj);
                    }
                    else
                        if (neighbourUp->isComponent() && neighbourDw->isComponent())
                        {
                            obj->setDownNeighbour(neighbourDw);
                            neighbourDw->setUpNeighbour(obj);							
                            neighbourUp->setDownNeighbour(obj);
                        }
                        else
                        {
                            InnWire *w1 = dynamic_cast<InnWire *>(neighbourUp);
                            InnWire *w2 = dynamic_cast<InnWire *>(neighbourDw);
                            if (w1 != nullptr && w2 != nullptr)
                            {
                                wireConnectionException e(w1->getNum(), w2->getNum());
                                throw (e);
                            }
                            else
                            {
                                wireConnectionException e(0, 0);
                                throw (e);
                            }
                        }

            }
            else
            if (neighbourUp != nullptr )
            {
                obj->setUpNeighbour(neighbourUp);
                neighbourUp->setDownNeighbour(obj);
            }
            else
            if (neighbourDw != nullptr )
            {
                obj->setDownNeighbour(neighbourDw);
                neighbourDw->setUpNeighbour(obj);
            }
        }
        res = true;
    }
    return res;
}

bool nnObjManager::unlinkObj(size_t x, size_t y, InnObj *obj)
{
    bool res = false;

    if (y > 0) {
        InnObj *neighbour = getObj(x, y - 1);
        obj->setUpNeighbour(nullptr);
        if (neighbour != nullptr) {
            if (neighbour->isComponent()) {
                neighbour->setDownNeighbour(nullptr);
            }
            else {
                removeObj(x, y - 1);
            }

        }
    }
    if (y + 1 < v_height) {
        InnObj *neighbour = getObj(x, y + 1);
        obj->setDownNeighbour(nullptr);
        if (neighbour != nullptr) {
            if (neighbour->isComponent()) {
                neighbour->setUpNeighbour(nullptr);
            }
            else {
                removeObj(x, y + 1);
            }
        }
    }
    return true;
}



bool nnObjManager::moveObj(n2Point from, n2Point to)
{
    bool res = false;
    InnObj *fromObj = getObj(from.x, from.y);
    InnObj *toObj = getObj(to.x, to.y);
    if (toObj == nullptr && fromObj!=nullptr && fromObj->isComponent())
    {
        fromObj =outObj(from.x, from.y);
        res = addObj(to.x, to.y, fromObj);
    }
    return res;
}

bool nnObjManager::swapObj(n2Point from, n2Point to)
{
    bool res = false;
    InnObj *fromObj = getObj(from.x, from.y);
    InnObj *toObj = getObj(to.x, to.y);
    if (toObj != nullptr && fromObj != nullptr && toObj->isComponent() && fromObj->isComponent())
    {
        fromObj = outObj(from.x, from.y);
        toObj= outObj(to.x, to.y);
        res = addObj(to.x, to.y, fromObj) & addObj(from.x, from.y, toObj);
    }
    return res;
}

