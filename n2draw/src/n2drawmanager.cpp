#include <stdio.h>
#include "n2drawmanager.h"
#include <iostream>
#include "n2miniXml.h"
#include <stdarg.h>
#include "n2draw.h"
#include "n2utoatou.h"
#include "n2exception.h"

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

nnObjManager::nnObjManager(int x, int y)
    : v_width(x), v_height(y),
      mask_width(0), mask_height(0)
{
    computeMask();
}

nnObjManager::~nnObjManager()
{
    removeAll();
}



bool nnObjManager::computeMask(void)
{
    nnObjWire::resetUI();
    int i = v_width & 0x0fffffff;
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
    managerUR.setHost(this);
    return true;
}


InnObj * nnObjManager::getObj(int x, int y)
{
    nnObj * res = nullptr;
    hashkey hkey;
    if (range(x, y))
        if (genHashKey(x, y, hkey))
        {
            hashObjTable::iterator it = find(hkey);
            if (it != end())
            {
                res = dynamic_cast<nnObj *> (it->second);
            }
        }
    return res;
}

bool nnObjManager::addObj(int x, int y, InnObj * obj)
{
    bool res = false;
    hashkey hkey;
    if (range(x, y) && obj != nullptr)
        if (genHashKey(x, y, hkey))
        {
            hashObjTable::iterator it = find(hkey);
            if (it == end())
            {
                std::pair<hashkey, InnObj *> p(hkey, obj);
                insert(p);
                obj->setXpos(x);
                obj->setYpos(y);
                res = linkObj(x, y, obj);
                undo_redo_unit u(eAction::removeObjAction, x, y, nullptr);
                managerUR.record(u);
            }
        }
    return res;
}

bool nnObjManager::removeObj(int x, int y)
{
    bool res = false;
    hashkey hkey;
    if (range(x, y))
        if (genHashKey(x, y, hkey))
        {
            hashObjTable::iterator it = find(hkey);
            if (it != end())
            {
                InnObj * obj = it->second;
                if (obj != nullptr)
                {
                    erase(it); // remove first break recursion
                    unlinkObj(x, y, obj);
                    undo_redo_unit u(eAction::addObjAction, x, y, obj);
                    managerUR.record(u);
                    res = true;
                }
            }
        }
    return res;
}


InnObj * nnObjManager::outObj(int x, int y)
{
    InnObj * obj = nullptr;
    hashkey hkey;
    if (range(x, y))
        if (genHashKey(x, y, hkey))
        {
            hashObjTable::iterator it = find(hkey);
            if (it != end())
            {
                obj = it->second;
                if (obj != nullptr)
                {
                    erase(it); // remove first break recursion
                    unlinkObj(x, y, obj);
                    undo_redo_unit u(eAction::outObjAction, x, y, obj);
                    managerUR.record(u);
                }
            }
        }
    return obj;
}

bool nnObjManager::addCoil(int x, nnObjCoil *obj)
{
    bool res = false;
    if(obj!=nullptr)
    {
        if (obj->getContext() == objCoil)
        {
            if (getObj(x, v_height-1) == nullptr)
            {
                int i,fp;
                // y pos is relative :
                // coil pos = y-1, for first empty position to y-1 wire to connect
                // find first empty cell
                for (i = 0; i < v_height-1; i++)
                {
                    InnObj *obj=getObj(x, i);
                    if (obj == nullptr)break;
                }
                for (; i < v_height-1; i++)
                {
                    nnObjWire *wire = new nnObjWire(eWire::wireVertical);
                    res = addWire(x, i, wire);
                    if (!res)break;
                }
                if (res)
                {

                    fp = v_height - 1;
                    hashkey hkey;
                    if (range(x,fp) && obj != nullptr)
                    {
                        if (genHashKey(x, fp, hkey))
                        {
                            hashObjTable::iterator it = find(hkey);
                            if (it == end())
                            {
                                std::pair<hashkey, InnObj *> p(hkey, obj);
                                insert(p);
                                obj->setXpos(x);
                                obj->setYpos(fp);
                                res = linkObj(x, fp, obj);
                                undo_redo_unit u(eAction::removeObjAction, x, fp, nullptr);
                                managerUR.record(u);
                            }
                        }
                    }
                }
            }
        }
    }
    return res;
}

bool nnObjManager::addWire(int x, int y, InnObj * obj)
{
    bool res = false;
    if (obj->getContext() == objWire)
    {
        hashkey hkey;
        if (rangeContact(x, y) && obj != nullptr)
        {
            if (genHashKey(x, y, hkey))
            {
                hashObjTable::iterator it = find(hkey);
                if (it == end())
                {
                    std::pair<hashkey, InnObj *> p(hkey, obj);
                    insert(p);
                    obj->setXpos(x);
                    obj->setYpos(y);
                    res = linkObj(x, y, obj);
                    undo_redo_unit u(eAction::removeObjAction, x, y, nullptr);
                    managerUR.record(u);
                }
            }
        }
    }
    return res;
}

bool nnObjManager::addContact(int x, int y, nnObjContact *obj)
{
    bool res = false;
    if(obj!=nullptr)
    {
        if (obj->getContext() == objContact)
        {
            hashkey hkey;
            if (rangeContact(x, y) && obj != nullptr)
            {
                if (genHashKey(x, y, hkey))
                {
                    hashObjTable::iterator it = find(hkey);
                    if (it == end())
                    {
                        std::pair<hashkey, InnObj *> p(hkey, obj);
                        insert(p);
                        obj->setXpos(x);
                        obj->setYpos(y);
                        res = linkObj(x, y, obj);
                        undo_redo_unit u(eAction::removeObjAction, x, y, nullptr);
                        managerUR.record(u);
                    }
                }
            }
        }
    }
    return res;
}


bool nnObjManager::replaceObj(int x, int y, InnObj * obj)
{
    bool res = false;
    hashkey hkey;
    if (range(x, y) && obj != nullptr)
        if (genHashKey(x, y, hkey))
        {
            hashObjTable::iterator it = find(hkey);
            if (it != end())
            {
                undo_redo_unit u(eAction::addObjAction, x, y, it->second);
                managerUR.record(u);
                it->second = obj;
                obj->setXpos(x);
                obj->setYpos(y);
                if (it->second->isComponent())
                {
                    linkObj(x, y, obj);
                    undo_redo_unit v(eAction::removeObjAction, x, y, nullptr);
                    managerUR.record(v);
                    res = true;
                }
            }
        }
    return res;
}

bool nnObjManager::removeAll(void)
{
    bool res = false;
    if (size() > 0)
    {
        for (auto n : *this)
        {
            delete n.second;
        }
        res = true;
        clear();
    }
    return res;
}







bool nnObjManager::save(const STRING & name)
{
    bool res=false;
    int num_obj = 0;
    if (!name.empty())
    {
#ifdef __ANDROID__
        STRING newname="data:/"+name;
#else
        STRING newname=name;
#endif
        UtoA toA(newname);
        FILE *out = fopen(toA.utf8(), "w+");
        if (out != NULL)
        {
            miniXmlNode root(X("next_v2"), (XCHAR *)X("1.0.0.0 Copyright(c) 2015 Angelo Coppi"));
            root.add(X("Wire_UID"), nnObjConn::getUI());
            root.add(X("Width"), v_width);
            root.add(X("Height"), v_height);
            root.add(X("Size"), (int)(size()) + 1);
            hashObjTable::iterator it = begin();
            hashObjTable::iterator _end = end();
            while (it != _end)
            {
                num_obj++;
                IXmlNode *child = root.add(X("Obj_UID_"), num_obj, num_obj);
                it->second->save(child);
                it++;
            }
            root.print(out);
            fclose(out);
            res=true;
        }
    }
    return res;
}



bool nnObjManager::load(const STRING & name)
{
    bool res=false;
    if (!name.empty())
    {
        removeAll();
        miniXmlNode root;
        root.load(name.c_str(), &root);
        STRING name = root.getName();
        STRING value = root.getValue();
        if (name == X("next_v2") && value == X("1.0.0.0 Copyright(c) 2015 Angelo Coppi"))
        {
            IXmlNode *t = root.find(X("Wire_UID"));
            if (t != nullptr)
            {
                nnObjConn::setUI(t->getLong());
            }
            t = root.find(X("Width"));
            if (t != nullptr)
            {
                v_width = t->getLong();
            }
            t = root.find(X("Height"));
            if (t != nullptr)
            {
                v_height = t->getLong();
            }
            computeMask();
            IXmlNode *size = root.find(X("Size"));
            if (size != nullptr)
            {
                int i, numObj = size->getLong();
                for (i = 1; i < numObj; i++)
                {
                    IXmlNode *child = root.find(X("Obj_UID_"), i);
                    if (child != nullptr)
                    {
                        if (i == child->getLong())
                        {
                            IXmlNode *spec = child->find(X("Custom"));
                            if (spec != nullptr)
                            {
                                IXmlNode *context = child->find(X("Context"));
                                if (context != nullptr)
                                {
                                    InnObj *obj = nnObjConn::getObjFromIds((custom_obj)spec->getLong(), (ObjContext)context->getLong());
                                    if (obj != nullptr)
                                    {
                                        obj->load(child);
                                        addObj(obj->getXpos(), obj->getYpos(), obj);
                                    }
                                }
                            }
                        }
                    }
                }
            }

        }
        res=true;
    }
    return res;
}

bool nnObjManager::genHashKey(int x, int y, hashkey &key)
{
    bool res = false;
    x <<= 1;
    y <<= 1;
    x |= 1;
    y |= 1;
    x &= mask_width;
    y &= mask_height;
    key.i.v2 = y;
    key.i.v1 = x;
    res =( key.v12 != 0);
    return res;
}

bool nnObjManager::range(int x, int y)
{
    bool res = false;
    if (x < v_width && y < v_height)
        res = true;
    return res;
}
bool nnObjManager::rangeContact(int x, int y)
{
    bool res = false;
    if (x < v_width && y < v_height - 1)
        res = true;
    return res;
}

bool nnObjManager::linkObj(int x, int y, InnObj *obj)
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
        else
        {
            obj->powerConnect(1);
        }
        if (y + 1 < v_height)
        {
            neighbourDw = getObj(x, y + 1);
        }
        else
        {
            obj->powerConnect(2);
        }

        // VERT
        if (obj->isComponent())
        {
            if (neighbourUp == nullptr && neighbourDw == nullptr)
            {
                obj->setConnections(0);
            }
            else
            {
                if (neighbourUp != nullptr)
                {
                    obj->connect(neighbourUp);
                    if(neighbourUp->isComponent()==false)
                        neighbourUp->connect(obj);
                }
                else
                    obj->setConnections(0);
                if (neighbourDw != nullptr)
                {
                    obj->connect(neighbourDw);
                    if (neighbourDw->isComponent() == false)
                        neighbourDw->connect(obj);
                }
                else
                    obj->setConnections(0);
            }
            res = true;
        }
        else
        {
            if (neighbourUp != nullptr && neighbourDw != nullptr)
            {
                if (neighbourUp->isComponent() && !neighbourDw->isComponent())
                {
                    res = obj->connect(neighbourDw);
                    if (res)
                        res = neighbourUp->connect(obj);
                }
                else if (!neighbourUp->isComponent() && neighbourDw->isComponent())
                {
                    res = obj->connect(neighbourUp);
                    if (res)
                        res = neighbourDw->connect(obj);
                }
                else if (neighbourUp->isComponent() && neighbourDw->isComponent())
                {
                    obj->setConnections(nnObjConn::getUI());
                    res = neighbourUp->connect(obj);
                    if (res)
                        res = neighbourDw->connect(obj);
                }
                else
                {
                    InnObj *w1 = dynamic_cast<InnObj *>(neighbourUp);
                    InnObj *w2 = dynamic_cast<InnObj *>(neighbourDw);
                    wireConnectionException *pe= new wireConnectionException(w1->getConnections(), w2->getConnections());
                    throw (pe);
                }
            }
            else if (neighbourUp != nullptr)
            {
                if (neighbourUp->isComponent())
                {
                    if (obj->getConnections().empty())
                    {
                        obj->setConnections(nnObjConn::getUI());
                    }
                    res = neighbourUp->connect(obj);
                    if (res)
                        res = obj->connect(neighbourUp);
                }
                else
                {
                    obj->connect(neighbourUp);
                }
            }
            else if (neighbourDw != nullptr)
            {
                if (neighbourDw->isComponent())
                {
                    if (obj->getConnections().empty())
                    {
                        obj->setConnections(nnObjConn::getUI());
                    }
                    res = neighbourDw->connect(obj);
                    if (res)
                        res = obj->connect(neighbourDw);
                }
                else
                {
                    obj->connect(neighbourDw);
                }
            }
            //    else
            //    {
            //up & down =null
            // check for horizzontal connection on wire
            InnObj *neighbourLeft = nullptr;
            InnObj *neighbourRight = nullptr;
            if (x > 0)
            {
                neighbourLeft = getObj(x - 1, y);
            }
            if (x + 1 < v_width)
            {
                neighbourRight = getObj(x + 1, y);
            }
            if (neighbourLeft != nullptr && !neighbourLeft->isComponent())
                res = obj->connect(neighbourLeft);
            if (neighbourRight != nullptr && !neighbourRight->isComponent())
                res = obj->connect(neighbourRight);
            //    }
            res = true;
        }

    }
    return res;
}

bool nnObjManager::unlinkObj(int x, int y, InnObj *obj)
{
    bool res = false;
    InnObj *neighbour = nullptr;
    if (y > 0)
    {
        neighbour = getObj(x, y - 1);
        if (neighbour != nullptr)
        {
            if (neighbour->isComponent())
            {
                res=neighbour->disconnect(obj);
            }
            else
            {
                res=removeObj(x, y - 1);
            }
        }
    }
    if (y + 1 < v_height)
    {
        neighbour = getObj(x, y + 1);
        if (neighbour != nullptr)
        {
            if (neighbour->isComponent())
            {
                res=neighbour->disconnect(obj);
            }
            else
            {
                res=removeObj(x, y + 1);
            }
        }
    }
    if (!obj->isComponent())
    {
        if (x > 0)
        {
            neighbour = getObj(x - 1, y);
            if (neighbour != nullptr)
            {
                if (!neighbour->isComponent())
                {
                    res=removeObj(x - 1, y);
                }
            }
        }
        if (x + 1 < v_width)
        {
            neighbour = getObj(x + 1, y);
            if (neighbour != nullptr)
            {
                if (!neighbour->isComponent())
                {
                    res=removeObj(x + 1, y);
                }
            }
        }

    }
    return res;
}



bool nnObjManager::moveObj(nnPoint from, nnPoint to)
{
    bool res = false;
    InnObj *fromObj = getObj(from.x, from.y);
    InnObj *toObj = getObj(to.x, to.y);
    if (toObj == nullptr && fromObj != nullptr && fromObj->isComponent())
    {
        fromObj = outObj(from.x, from.y);
        res = addObj(to.x, to.y, fromObj);
    }
    return res;
}

bool nnObjManager::swapObj(nnPoint from, nnPoint to)
{
    bool res = false;
    InnObj *fromObj = getObj(from.x, from.y);
    InnObj *toObj = getObj(to.x, to.y);
    if (toObj != nullptr && fromObj != nullptr && toObj->isComponent() && fromObj->isComponent())
    {
        fromObj = outObj(from.x, from.y);
        toObj = outObj(to.x, to.y);
        res = addObj(to.x, to.y, fromObj);
        if (res)
            res = addObj(from.x, from.y, toObj);
    }
    return res;
}


bool nnObjManager::revHashKey(hashkey & key, int & x, int &y)
{
    bool res = false;
    y = (key.i.v2)>>1 ;
    x = (key.i.v1)>>1 ;
    res = (y != 0 && x != 0);
    return res;
}

nnPoint  nnObjManager::getStartPoint(void)
{
    int x, y;
    hashObjTable::iterator it = begin();
    hashkey key = it->first;
    revHashKey(key, x, y);
    return nnPoint(x, y);

}


nnPoint  nnObjManager::getStopPoint(void)
{
    int x, y;
    hashObjTable::reverse_iterator r_it = rbegin();
    r_it++;
    hashObjTable::iterator it = r_it.base();
    hashkey key = it->first;
    revHashKey(key, x, y);
    return nnPoint(x, y);
}




bool nnObjManager::undo(void)
{
    return managerUR.undo();
}


bool nnObjManager::redo(void)
{
    return managerUR.redo();
}

bool nnObjManager::insertRow(int y_pos)
{
    bool res = false;
    iterator it = begin();
    iterator _end = end();
    while (it != _end)
    {
        int x, y;
        hashkey *key = (hashkey *)&it->first;
        revHashKey(*key, x, y);
        if (y >= y_pos)
        {
            y++;
            res = genHashKey(x, y, *key);
            if (!res)
                break;
        }
        it++;
    }
    if (res)
    {
        v_height++;
    }
    return res;
}

bool nnObjManager::insertCol(int x_pos)
{
    bool res = false;
    iterator it = begin();
    iterator _end = end();
    while (it != _end)
    {
        int x, y;
        hashkey *key = (hashkey *)&it->first;
        revHashKey(*key, x, y);
        if (x >= x_pos)
        {
            x++;
            res = genHashKey(x, y, *key);
            if (!res)
                break;
        }
        it++;
    }
    if (res)
    {
        v_width++;
    }
    return res;
}


bool nnObjManager::removeRow(int y_pos)
{
    bool res = false;
    int x;
    for (x = 0; x < v_width; x++)
    {
        InnObj *obj = getObj(x, y_pos);
        if (obj != nullptr)
        {
            if (obj->getContext() == objWire)
            {
                InnWire *wire = dynamic_cast<InnWire *>(obj);
                if (wire->getWire() == eWire::wireVertical)
                    continue;
            }
            break;
        }
    }
    if (x == v_width)
    {
        for (x = 0; x < v_width; x++)
        {
            InnObj *obj = getObj(x, y_pos);
            if (obj != nullptr)
            {
                res = removeObj(x, y_pos);
                if (!res)
                    break;
            }
        }
    }
    if (x == v_width)
    {
        iterator it = begin();
        iterator _end = end();
        while (it != _end)
        {
            int x, y;
            hashkey *key = (hashkey *)&it->first;
            revHashKey(*key, x, y);
            if (y <= y_pos)
            {
                y--;
                res = genHashKey(x, y, *key);
                if (!res)
                    break;
            }
            it++;
        }
    }
    return res;
}

bool nnObjManager::checkRemovableCol(int x_pos)
{
    bool res = false;
    int y;
    for (y = 0; y < v_height; y++)
    {
        InnObj *obj = getObj(x_pos, y);
        if (obj != nullptr)
        {
            if (obj->getContext() == objWire)
            {
                InnWire *wire = dynamic_cast<InnWire *>(obj);
                if (wire->getWire() == eWire::wireHorizzontal)
                    continue;
            }
            break;
        }
    }
    res = (y == v_height);
    return res;
}

bool nnObjManager::removeCol(int x_pos)
{
    bool res = checkRemovableCol(x_pos);
    if (res)
    {
        int y;
        for (y = 0; y < v_height; y++)
        {
            InnObj *obj = getObj(x_pos, y);
            if (obj != nullptr)
            {
                res = removeObj(x_pos, y);
                if (!res)
                    break;
            }
        }
        res = y == v_height;
    }
    if (res)
    {
        iterator it = begin();
        iterator _end = end();
        while (it != _end)
        {
            int x, y;
            hashkey *key = (hashkey *)&it->first;
            revHashKey(*key, x, y);
            if (x >= x_pos)
            {
                x--;
                res = genHashKey(x, y, *key);
                if (!res)
                    break;
            }
            it++;
        }
    }
    return res;
}

bool nnObjManager::removeEmptyCol(void)
{
    bool res = false;
    int vx;
    for (vx = v_width - 1; vx > 0; vx--)
    {
        res = checkRemovableCol(vx);
        if (res)
        {
            int y;
            for (y = 0; y < v_height; y++)
            {
                InnObj *obj = getObj(vx, y);
                if (obj != nullptr)
                {
                    res = removeObj(vx, y);
                    if (!res)
                        break;
                }
            }
            res = y == v_height;
        }
        if (res)
        {
            iterator it = begin();
            iterator _end = end();
            while (it != _end)
            {
                int x, y;
                hashkey *key = (hashkey *)&it->first;
                revHashKey(*key, x, y);
                if (x >= vx)
                {
                    x--;
                    res = genHashKey(x, y, *key);
                    if (!res)
                        break;
                }
                it++;
            }
        }
    }
    return false;
}


bool nnObjManager::ResizeHeight(int h)
{
    bool res = false;
    if (v_height != h)
    {
        if (v_height < h)
        {
            while (v_height < h)
            {
                res = insertRow(v_height - 1);
                if (!res)
                    break;
                v_height++;
            }
        }
        else
        {
            while (h < v_height)
            {
                res = removeRow(v_height - 1);
                if (!res)
                    break;
                v_height--;
            }
        }
    }
    res = (v_height == h);
    return res;
}

bool nnObjManager::ResizeWidth(int w)
{
    bool res = false;
    if (v_width != w)
    {
        if (v_width < w)
        {
            v_width = w;
            res = true;

        }
        else
        {
            while (w < v_width)
            {
                res = removeCol(v_width - 1);
                if (!res)
                    break;
                v_width--;
            }
            if (!res && w < v_width)
            {
                while (w < v_width)
                {
                    res = removeCol(0);
                    if (!res)
                        break;
                    v_width--;
                }
            }
            if (!res && w < v_width)
            {
                while (w < v_width)
                {
                    res = removeEmptyCol();
                    if (!res)
                        break;
                    v_width--;
                }
            }
        }
    }
    return res;
}

bool nnObjManager::Resize(int w, int h)
{
    bool res = false;
    res = ResizeWidth(w);
    if (res)
        res = ResizeHeight(h);
    return res;
}




////////////////////////////////////////////////////////////////////////////////
nnObjUndoRedo::nnObjUndoRedo(IManager *_manager)
    :undoredoMode(false),manager(_manager)

{

}

nnObjUndoRedo::~nnObjUndoRedo()
{
    clearUndoObjs();
    clearRedoObjs();
}

bool nnObjUndoRedo::undo(void)
{
    bool res = false;
    if (manager!=nullptr && undoObjs.size() > 0)
    {
        undo_redo_unit f = undoObjs.back();
        undoObjs.pop_back();
        switch (f.action)
        {
        case addObjAction:
            undoredoMode = true;
            res= manager->addObj(f.x_pos, f.y_pos, f.obj);
            undoredoMode = false;
            break;
        case removeObjAction:
            undoredoMode = true;
            res = manager->removeObj(f.x_pos, f.y_pos);
            undoredoMode = false;
            break;
        case outObjAction:
            undoredoMode = true;
            res = manager->addObj(f.x_pos, f.y_pos, f.obj);
            undoredoMode = false;
            break;
        }
    }
    return res;
}

bool nnObjUndoRedo::redo(void)
{
    bool res = false;
    if (manager != nullptr && redoObjs.size() > 0)
    {
        undo_redo_unit f = redoObjs.back();
        redoObjs.pop_back();
        switch (f.action)
        {
        case addObjAction:
            undoredoMode = false;
            res = manager->addObj(f.x_pos, f.y_pos, f.obj);
            break;
        case removeObjAction:
            undoredoMode = false;
            res = manager->removeObj(f.x_pos, f.y_pos);
            break;
        case outObjAction:
            undoredoMode = false;
            res = manager->addObj(f.x_pos, f.y_pos, f.obj);
            break;
        }
    }
    return res;

}

bool nnObjUndoRedo::setHost(IManager *_manager)
{
    manager = _manager;
    return true;
}

void nnObjUndoRedo::record(undo_redo_unit u)
{
    if (undoredoMode == false)
    {
        if (undoObjs.size() >= 100)
        {
            undo_redo_unit old = undoObjs.front();
            undoObjs.pop_front();
            if (old.obj != nullptr)
                delete old.obj;
        }
        undoObjs.push_back(u);
    }
    else
    {
        if (redoObjs.size() >= 100)
        {
            undo_redo_unit old = redoObjs.front();
            redoObjs.pop_front();
            if (old.obj != nullptr)
                delete old.obj;
        }
        redoObjs.push_back(u);
    }
}

void nnObjUndoRedo::clearUndoObjs(void)
{
    if (undoObjs.size() > 0)
    {
        for (auto n : undoObjs)
        {
            if (n.action == eAction::addObjAction && n.obj != nullptr)
                delete n.obj;
        }
        undoObjs.clear();
    }
}

void nnObjUndoRedo::clearRedoObjs(void)
{
    if (redoObjs.size() > 0)
    {
        for (auto n : redoObjs)
        {
            if (n.action == eAction::addObjAction && n.obj != nullptr)
                delete n.obj;
        }
        redoObjs.clear();
    }
}


bool nnObjManager::readConfiguration(IXmlNode *)
{
    // will need ?
    return true;
}
