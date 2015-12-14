#include <stdio.h>
#include "n2drawmanager.h"
#include <iostream>
#include <vector>
#include "n2miniXml.h"
#include <stdarg.h> 

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

nnObjManager::nnObjManager(size_t x, size_t y)
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
    size_t i = v_width & 0x0fffffff;
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
                undo_redo_unit u(eAction::removeObjAction, x, y, nullptr);
                managerUR.record(u);
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
                    undo_redo_unit u(eAction::addObjAction, x, y, obj);
                    managerUR.record(u);
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
                    undo_redo_unit u(eAction::outObjAction, x, y, obj);
                    managerUR.record(u);
                }
            }
        }
    return obj;
}

bool nnObjManager::addCoil(size_t x, nnObjCoil * obj)
{
    bool res = false;
    if (obj->getContext() == objCoil)
    {
        if (getObj(x, v_height-1) == nullptr)
        {
            size_t i,fp;
            // y pos is relative :
            // coil pos = y-1, for first empty position to y-1 wire to connect
            // find first empty cell
            for (i = 0; i < v_height-1; i++)
                if (getObj(x, i) == nullptr)break;
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
                    if (genHashKey(x, fp, hkey)) {
                        hashObjTable::iterator it = find(hkey);
                        if (it == end()) {
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
    return res;
}

bool nnObjManager::addWire(size_t x, size_t y, InnObj * obj)
{
    bool res = false;
    if (obj->getContext() == objWire)
    {
        hashkey hkey;
        if (rangeContact(x, y) && obj != nullptr)
        {
            if (genHashKey(x, y, hkey)) {
                hashObjTable::iterator it = find(hkey);
                if (it == end()) {
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

bool nnObjManager::addContact(size_t x, size_t y, nnObjContact * obj)
{
    bool res = false;
    if (obj->getContext() == objContact)
    {
        hashkey hkey;
        if (rangeContact(x, y) && obj != nullptr)
        {
            if (genHashKey(x, y, hkey)) {
                hashObjTable::iterator it = find(hkey);
                if (it == end()) {
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


bool nnObjManager::replaceObj(size_t x, size_t y, InnObj * obj)
{
    bool res = false;
    hashkey hkey;
    if (range(x, y) && obj != nullptr)
        if (genHashKey(x, y, hkey)) {
            hashObjTable::iterator it = find(hkey);
            if (it != end()) {
                undo_redo_unit u(eAction::addObjAction, x, y, it->second);
                managerUR.record(u);
                it->second = obj;
                obj->setXpos(x);
                obj->setYpos(y);
                if (it->second->isComponent()) {
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







void nnObjManager::save(STRING & name)
{
    size_t num_obj = 0;
    if (!name.empty())
    {
        try {
#ifdef _MSC_VER
            FILE *out = nullptr;
            FOPEN(&out, name.c_str(), X("w+"));
#else
            FILE *out = FOPEN(name.c_str(), "w+");
#endif
            if (out != NULL)
            {
                miniXmlNode root(X("next_v2"), (XCHAR *)X("1.0.0.0 Copyright(c) 2015 Angelo Coppi"));
                root.add(X("Wire_UID"), nnObjConn::getUI());
                root.add(X("Width"), v_width);
                root.add(X("Height"), v_height);
                root.add(X("Size"), size() + 1);
                hashObjTable::iterator it = begin();
                hashObjTable::iterator _end = end();
                while (it != _end)
                {
                    miniXmlNode *child = root.add(X("Obj_UID_"), ++num_obj, num_obj);
                    it->second->save(child);
                    it++;
                }
                root.print(out);
                fclose(out);
            }
        }
        catch (...)
        {
        }
    }
}



void nnObjManager::load(STRING & name)
{
    size_t num_obj = 0;
    if (!name.empty())
    {
        removeAll();
        try {
            miniXmlNode root;
            root.load(name.c_str(), &root);
            STRING name = root.getName();
            STRING value = root.getValue();
            if (name == X("next_v2") && value == X("1.0.0.0 Copyright(c) 2015 Angelo Coppi"))
            {
                miniXmlNode *t = root.find(X("Wire_UID"));
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
                miniXmlNode *size = root.find(X("Size"));
                if (size != nullptr)
                {
                    size_t i, numObj = size->getLong();
                    for (i = 1; i < numObj; i++)
                    {
                        miniXmlNode *child = root.find(X("Obj_UID_"), i);
                        if (child != nullptr)
                        {
                            if (i == child->getLong())
                            {
                                miniXmlNode *spec = child->find(X("Custom"));
                                if (spec != nullptr)
                                {
                                    miniXmlNode *context = child->find(X("Context"));
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
        }
        catch (...)
        {
        }
    }
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
bool nnObjManager::rangeContact(size_t x, size_t y)
{
    bool res = false;
    if (x < v_width && y < v_height - 1)
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
            if (neighbourUp != nullptr)
                obj->connect(neighbourUp);
            if (neighbourDw != nullptr)
                obj->connect(neighbourDw);
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
                else
                    if (!neighbourUp->isComponent() && neighbourDw->isComponent())
                    {
                        res = obj->connect(neighbourUp);
                        if (res)
                            res = neighbourDw->connect(obj);
                    }
                    else
                        if (neighbourUp->isComponent() && neighbourDw->isComponent())
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
                            wireConnectionException *pe= new wireConnectionException(neighbourUp->getConnections(), neighbourUp->getConnections());
                            throw (pe);
                        }
            }
            else
                if (neighbourUp != nullptr)
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
                else
                    if (neighbourDw != nullptr)
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
                    else
                    {
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
                    }
            res = true;
        }

    }
    return res;
}

bool nnObjManager::unlinkObj(size_t x, size_t y, InnObj *obj)
{
    bool res = false;
    InnObj *neighbour = nullptr;
    if (y > 0) {
        neighbour = getObj(x, y - 1);
        if (neighbour != nullptr) {
            if (neighbour->isComponent()) {
                neighbour->disconnect(obj);
            }
            else {
                removeObj(x, y - 1);
            }
        }
    }
    if (y + 1 < v_height) {
        neighbour = getObj(x, y + 1);
        if (neighbour != nullptr) {
            if (neighbour->isComponent()) {
                neighbour->disconnect(obj);
            }
            else {
                removeObj(x, y + 1);
            }
        }
    }
    if (!obj->isComponent())
    {
        if (x > 0) {
            neighbour = getObj(x - 1, y);
            if (neighbour != nullptr) {
                if (!neighbour->isComponent()) {
                    removeObj(x - 1, y);
                }
            }
        }
        if (x + 1 < v_width) {
            neighbour = getObj(x + 1, y);
            if (neighbour != nullptr) {
                if (!neighbour->isComponent()) {
                    removeObj(x + 1, y);
                }
            }
        }

    }
    return true;
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


bool nnObjManager::revHashKey(hashkey & key, size_t & x, size_t &y)
{
    bool res = false;
    y = key & 0xfffffff;
    y = y / 2;
    x = (key >> 30) & 0xfffffff;
    res = (y != 0 && x != 0);
    return res;
}

nnPoint  nnObjManager::getStartPoint(void)
{
    size_t x, y;
    hashObjTable::iterator it = begin();
    hashkey key = it->first;
    revHashKey(key, x, y);
    return nnPoint(x, y);

}


nnPoint  nnObjManager::getStopPoint(void)
{
    size_t x, y;
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

bool nnObjManager::insertRow(size_t y_pos)
{
    bool res = false;
    iterator it = begin();
    iterator _end = end();
    while (it != _end)
    {
        size_t x, y;
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

bool nnObjManager::insertCol(size_t x_pos)
{
    bool res = false;
    iterator it = begin();
    iterator _end = end();
    while (it != _end)
    {
        size_t x, y;
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


bool nnObjManager::removeRow(size_t y_pos)
{
    bool res = false;
    size_t x;
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
            size_t x, y;
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

bool nnObjManager::checkRemovableCol(size_t x_pos)
{
    bool res = false;
    size_t y;
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

bool nnObjManager::removeCol(size_t x_pos)
{
    bool res = checkRemovableCol(x_pos);
    if (res)
    {
        size_t y;
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
            size_t x, y;
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
    size_t vx;
    for (vx = v_width - 1; vx > 0; vx--)
    {
        res = checkRemovableCol(vx);
        if (res)
        {
            size_t y;
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
                size_t x, y;
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


bool nnObjManager::ResizeHeight(size_t h)
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

bool nnObjManager::ResizeWidth(size_t w)
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

bool nnObjManager::Resize(size_t w, size_t h)
{
    bool res = false;
    res = ResizeWidth(w);
    if (res)
        res = ResizeHeight(h);
    return res;
}

bool nnObjManager::readConfiguration(miniXmlNode & node)
{
    return true;
}



////////////////////////////////////////////////////////////////////////////////
nnObjUndoRedo::nnObjUndoRedo(IManager *_manager) 
    :manager(_manager),
    undoredoMode(false)
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