#include "n2draw.h"
#include "n2drawmanager.h"
#include "n2connection.h"
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

bool nnConnection::connectComponent(IManager * manager, nnPoint & p_src, nnPoint & p_dst)
{
    bool res = false;
    if (manager != nullptr)
    {
        //InnObj *v = nullptr;
        InnObj *src = manager->getObj(p_src.x, p_src.y);
        InnObj *dst = manager->getObj(p_dst.x, p_dst.y);
        if (src != nullptr && dst != nullptr)
        {
            if (src->isComponent() && dst->isComponent())
            {
                nnObjComponent *c_src, *c_dst;
                c_src = dynamic_cast<nnObjComponent*>(src);
                c_dst = dynamic_cast<nnObjComponent*>(dst);
                if (c_src != nullptr && c_dst != nullptr)
                {
                    //TWO COMPONENTS ANY TYPE...
                    if (p_src.x == p_dst.x && p_src.y != p_dst.y)
                    {
                        res = connectVertComponent(manager, p_src, p_dst);
                    }
                    else if (p_src.y == p_dst.y && p_src.x != p_dst.x)
                    {
                        if (p_src.x < p_dst.x)
                        {
                            res = connectHorzIncrUpSideComponent(manager, p_src, p_dst);
                            if (res)
                                res = connectHorzIncrDownSideComponent(manager, p_src, p_dst);
                        }
                        else
                        {
                            res = connectHorzDecrUpSideComponent(manager, p_src, p_dst);
                            if (res)
                                res = connectHorzDecrDownSideComponent(manager, p_src, p_dst);
                        }

                    }
                }
            }
            else
            {
                // 1 component + wire
                if (p_src.x == p_dst.x && p_src.y != p_dst.y)
                {
                    res = connectVertWireComponent(manager, p_src, p_dst);
                }
                else
                {
                    wireNoAvaiableConnection *pe=new wireNoAvaiableConnection();
                    throw(pe);
                }
            }
        }
        //else connection up to row 0
        else if (p_dst.y==0 && p_dst.x==p_src.x &&  dst == nullptr && src != nullptr)
        {
            // connection to power up
            res=connectToPower(manager, p_src, p_dst);
        }
    }
    return res;
}



bool nnConnection::connectToPower(IManager *manager, nnPoint &p_src, nnPoint & p_dst)
{
    bool res = false;
    int u;
    InnObj *v = nullptr;
    //vert connection
    if (p_src.y > p_dst.y)
    {
        for (u = 0; u < p_src.y; u++)
        {
            v = manager->getObj(p_src.x, u);
            if (v == nullptr)
            {
                v = new nnObjWire(eWire::wireVertical);
                MEMCHK(InnObj, v);
                v->setConnections(1);
                res = manager->addWire(p_src.x, u, v);
                if (!res)break;
            }
            else if (v->isComponent())
            {
                positionBusyException *pe = new positionBusyException(p_src.x, u);
                throw (pe);
            }
        }
    }
    else
    {
        for (u = 0; u < p_dst.y; u++)
        {
            v = manager->getObj(p_src.x, u);
            if (v == nullptr)
            {
                v = new nnObjWire(eWire::wireVertical);
                MEMCHK(InnObj, v);
                res = manager->addWire(p_src.x, u, v);
                if (!res)break;
            }
            else if (v->isComponent())
            {
                positionBusyException *pe = new positionBusyException(p_src.x, u);
                throw (pe);
            }
        }
    }
    return res;
}


bool nnConnection::connectVertComponent(IManager *manager, nnPoint &p_src, nnPoint & p_dst)
{
    bool res = false;
    int u;
    InnObj *v = nullptr;
    //vert connection
    if (p_src.y > p_dst.y)
    {
        for (u = p_dst.y + 1; u < p_src.y; u++)
        {
            v = manager->getObj(p_src.x, u);
            if (v == nullptr)
            {
                v = new nnObjWire(eWire::noWire);
                MEMCHK(InnObj, v);
                res = manager->addWire(p_src.x, u, v);
                if (!res)break;
            }
            else if (v->isComponent())
            {
                positionBusyException *pe=new positionBusyException(p_src.x, u);
                throw (pe);
            }
        }
    }
    else
    {
        for (u = p_src.y + 1; u < p_dst.y; u++)
        {
            v = manager->getObj(p_src.x, u);
            if (v == nullptr)
            {
                v = new nnObjWire(eWire::noWire);
                MEMCHK(InnObj, v);
                res = manager->addWire(p_src.x, u, v);
                if (!res)break;
            }
            else if (v->isComponent())
            {
                positionBusyException *pe = new positionBusyException(p_src.x, u);
                throw (pe);
            }
        }
    }
    return res;
}

bool nnConnection::connectHorzIncrUpSideComponent(IManager * manager, nnPoint & p_src, nnPoint & p_dst)
{
    bool res = false;
    int  i, num;
    InnObj *v = nullptr;
    InnObj *near_src = nullptr;
    InnObj *near_dst = nullptr;
    if (p_src.y > 0)
    {
        //UP SIDE
        near_src = manager->getObj(p_src.x, p_src.y - 1);
        near_dst = manager->getObj(p_dst.x, p_dst.y - 1);
        if (near_src == nullptr && near_dst == nullptr)
        {
            // change for diff == 1
            if ((p_dst.x - p_src.x) > 1)
            {
                v = new nnObjWire(eWire::wireAngleDownRight);
                MEMCHK(InnObj, v);
                if (manager->addWire(p_src.x, p_src.y - 1, v))
                {
                    num = v->getConnections().front();
                    for (i = p_src.x + 1; i < p_dst.x; i++)
                    {
                        v = new nnObjWire(eWire::noWire);
                        MEMCHK(InnObj, v);
                        v->setConnections(num);
                        res = manager->addWire(i, p_src.y - 1, v);
                        if (!res)break;
                    }
                    if (res)
                    {
                        v = new nnObjWire(eWire::wireAngleDownLeft);
                        MEMCHK(InnObj, v);
                        v->setConnections(num);
                        res = manager->addWire(i, p_src.y - 1, v);
                    }
                }
            }
            else
            {
                v = new nnObjWire(eWire::wireAngleDownRight);
                MEMCHK(InnObj, v);
                if (manager->addWire(p_src.x, p_src.y - 1, v))
                {
                    num = v->getConnections().front();
                    v = new nnObjWire(eWire::wireAngleDownLeft);
                    MEMCHK(InnObj, v);
                    v->setConnections(num);
                    res = manager->addWire(p_src.x+1, p_src.y - 1, v);
                }
            }
        }
        else if (near_src != nullptr && near_dst == nullptr)
        {
            num = near_src->getConnections().front();
            if (num == 1)
            {
                //vert to power positive
                for (i = 0; i < p_src.y ; i++)
                {
                    v = new nnObjWire(eWire::wireVertical);
                    MEMCHK(InnObj, v);
                    v->setConnections(1);
                    res = manager->addWire(p_dst.x, i, v);
                    if (!res)break;
                }
            }
            else
            {
                if ((p_dst.x - p_src.x) > 1)
                {
                    v = new nnObjWire(eWire::noWire);
                    MEMCHK(InnObj, v);
                    v->setConnections(num);
                    if (manager->addWire(p_src.x + 1, p_src.y - 1, v))
                    {
                        for (i = p_src.x + 2; i < p_dst.x; i++)
                        {
                            v = new nnObjWire(eWire::noWire);
                            MEMCHK(InnObj, v);
                            v->setConnections(num);
                            res = manager->addWire(i, p_src.y - 1, v);
                            if (!res)break;
                        }
                        if (res)
                        {
                            v = new nnObjWire(eWire::wireAngleDownLeft);
                            MEMCHK(InnObj, v);
                            v->setConnections(num);
                            res = manager->addWire(i, p_src.y - 1, v);
                        }
                    }
                }
                else
                {
                    v = new nnObjWire(eWire::wireAngleDownLeft);
                    MEMCHK(InnObj, v);
                    v->setConnections(num);
                    res = manager->addWire(p_src.x+1, p_src.y - 1, v);
                }
            }
        }
    }
    else if (p_src.y == 0)
    {
        res = true;
    }
    return res;
}

bool nnConnection::connectHorzIncrDownSideComponent(IManager * manager, nnPoint & p_src, nnPoint & p_dst)
{
    bool res = true;
    int  i, num;
    InnObj *v = nullptr;
    InnObj *near_src = nullptr;
    InnObj *near_dst = nullptr;
    if (p_src.y < manager->getHeight())
    {
        //DOWN SIDE
        near_src = manager->getObj(p_src.x, p_src.y + 1);
        near_dst = manager->getObj(p_dst.x, p_dst.y + 1);
        if (near_src == nullptr && near_dst == nullptr)
        {
            // change for diff == 1
            if ((p_dst.x - p_src.x) > 1)
            {
                v = new nnObjWire(eWire::wireAngleUpRight);
                MEMCHK(InnObj, v);
                if (manager->addWire(p_src.x, p_src.y + 1, v))
                {
                    num = v->getConnections().front();
                    for (i = p_src.x + 1; i < p_dst.x; i++)
                    {
                        v = new nnObjWire(eWire::noWire);
                        MEMCHK(InnObj, v);
                        v->setConnections(num);
                        res = manager->addWire(i, p_src.y + 1, v);
                        if (!res)break;
                    }
                    if (res)
                    {
                        v = new nnObjWire(eWire::wireAngleUpLeft);
                        MEMCHK(InnObj, v);
                        v->setConnections(num);
                        res = manager->addWire(i, p_src.y + 1, v);
                    }
                }
            }
            else
            {
                v = new nnObjWire(eWire::wireAngleUpRight);
                MEMCHK(InnObj, v);
                if (manager->addWire(p_src.x, p_src.y + 1, v))
                {
                    num = v->getConnections().front();
                    v = new nnObjWire(eWire::wireAngleUpLeft);
                    MEMCHK(InnObj, v);
                    v->setConnections(num);
                    res = manager->addWire(p_dst.x, p_src.y + 1, v);
                }
            }
        }
        else if (near_src != nullptr && near_dst == nullptr)
        {
            num = near_src->getConnections().back();
            if (num == 2)
            {
                //vert to power positive
                for (i = p_src.y + 1; i <= manager->getHeight(); i++)
                {
                    v = new nnObjWire(eWire::wireVertical);
                    MEMCHK(InnObj, v);
                    v->setConnections(2);
                    res = manager->addWire(p_src.x, i, v);
                    if (!res)break;
                }
            }
            else
            {
                // change for diff == 1
                if ((p_dst.x - p_src.x) > 1)
                {
                    v = new nnObjWire(eWire::noWire);
                    MEMCHK(InnObj, v);
                    v->setConnections(num);
                    if (manager->addWire(p_src.x + 1, p_src.y + 1, v))
                    {
                        for (i = p_src.x + 2; i < p_dst.x; i++)
                        {
                            v = new nnObjWire(eWire::noWire);
                            MEMCHK(InnObj, v);
                            v->setConnections(num);
                            res = manager->addWire(i, p_src.y + 1, v);
                            if (!res)break;
                        }
                        if (res)
                        {
                            v = new nnObjWire(eWire::wireAngleUpLeft);
                            MEMCHK(InnObj, v);
                            v->setConnections(num);
                            res = manager->addWire(i, p_src.y + 1, v);
                        }
                    }

                }
                else
                {
                    v = new nnObjWire(eWire::wireAngleUpLeft);
                    MEMCHK(InnObj, v);
                    v->setConnections(num);
                    res = manager->addWire(p_src.x + 1, p_src.y + 1, v);
                }
            }
        }
        else if (near_src == nullptr && near_dst != nullptr)
        {
            res=true;
        }
    }
    return res;
}




bool nnConnection::connectHorzDecrUpSideComponent(IManager * manager, nnPoint & p_src, nnPoint & p_dst)
{
    bool res = true;
    int  i, num;
    InnObj *v = nullptr;
    InnObj *near_src = nullptr;
    InnObj *near_dst = nullptr;
    if (p_src.y > 0)
    {
        //UP SIDE
        near_src = manager->getObj(p_src.x, p_src.y - 1);
        near_dst = manager->getObj(p_dst.x, p_dst.y - 1);
        if (near_src == nullptr && near_dst == nullptr)
        {
            if ((p_src.x - p_dst.x) > 1)
            {
                v = new nnObjWire(eWire::wireAngleDownLeft);
                MEMCHK(InnObj, v);
                if (manager->addWire(p_src.x, p_src.y - 1, v))
                {
                    num = v->getConnections().front();
                    for (i = p_src.x - 1; i > p_dst.x; i--)
                    {
                        v = new nnObjWire(eWire::noWire);
                        MEMCHK(InnObj, v);
                        v->setConnections(num);
                        res = manager->addWire(i, p_src.y - 1, v);
                        if (!res)break;
                    }
                    if (res)
                    {
                        v = new nnObjWire(eWire::wireAngleDownRight);
                        MEMCHK(InnObj, v);
                        v->setConnections(num);
                        res = manager->addWire(i, p_src.y - 1, v);
                    }
                }
            }
            else
            {
                v = new nnObjWire(eWire::wireAngleDownLeft);
                MEMCHK(InnObj, v);
                if (manager->addWire(p_src.x, p_src.y - 1, v))
                {
                    num = v->getConnections().front();
                    v = new nnObjWire(eWire::wireAngleDownRight);
                    MEMCHK(InnObj, v);
                    v->setConnections(num);
                    res = manager->addWire(p_src.x-1, p_src.y - 1, v);
                }
            }
        }
        else if (near_src != nullptr && near_dst == nullptr)
        {
            num = near_src->getConnections().front();
            if (num == 1)
            {
                //vert to power positive
                for (i = 0; i < p_src.y ; i++)
                {
                    v = new nnObjWire(eWire::wireVertical);
                    MEMCHK(InnObj, v);
                    v->setConnections(1);
                    res = manager->addWire(p_src.x, i, v);
                    if (!res)break;
                }
            }
            else
            {
                // change for diff == 1
                if ((p_src.x - p_dst.x) > 1)
                {
                    v = new nnObjWire(eWire::noWire);
                    MEMCHK(InnObj, v);
                    v->setConnections(num);
                    if (manager->addWire(p_src.x - 1, p_src.y - 1, v))
                    {
                        for (i = p_src.x - 2; i > p_dst.x; i--)
                        {
                            v = new nnObjWire(eWire::noWire);
                            MEMCHK(InnObj, v);
                            v->setConnections(num);
                            res = manager->addWire(i, p_src.y - 1, v);
                            if (!res)break;
                        }
                        if (res)
                        {
                            v = new nnObjWire(eWire::wireAngleDownRight);
                            MEMCHK(InnObj, v);
                            v->setConnections(num);
                            res = manager->addWire(i, p_src.y - 1, v);
                        }
                    }
                }
                else
                {
                    v = new nnObjWire(eWire::wireAngleDownRight);
                    MEMCHK(InnObj, v);
                    v->setConnections(num);
                    res = manager->addWire(p_src.x-1, p_src.y - 1, v);
                }
            }
        }
    }
    else if(p_src.y == 0 )
    {
        res = true;
    }
    return res;
}

bool nnConnection::connectHorzDecrDownSideComponent(IManager * manager, nnPoint & p_src, nnPoint & p_dst)
{
    bool res = true;
    int  i, num;
    InnObj *v = nullptr;
    InnObj *near_src = nullptr;
    InnObj *near_dst = nullptr;
    if (p_src.y < manager->getHeight())
    {
        //DOWN SIDE
        // SRC unconnect-component DST connected component
        near_src = manager->getObj(p_src.x, p_src.y + 1);
        near_dst = manager->getObj(p_dst.x, p_dst.y + 1);
        if (near_src == nullptr && near_dst == nullptr)
        {
            if ((p_src.x - p_dst.x) > 1)
            {
                v = new nnObjWire(eWire::wireAngleUpLeft);
                MEMCHK(InnObj, v);
                if (manager->addWire(p_src.x, p_src.y + 1, v))
                {
                    num = v->getConnections().front();
                    for (i = p_src.x - 1; i > p_dst.x; i--)
                    {
                        v = new nnObjWire(eWire::noWire);
                        MEMCHK(InnObj, v);
                        v->setConnections(num);
                        res = manager->addWire(i, p_src.y + 1, v);
                        if (!res)break;
                    }
                    if (res)
                    {
                        v = new nnObjWire(eWire::wireAngleUpRight);
                        MEMCHK(InnObj, v);
                        v->setConnections(num);
                        res = manager->addWire(i, p_src.y + 1, v);
                    }
                }
            }
            else
            {
                v = new nnObjWire(eWire::wireAngleUpLeft);
                MEMCHK(InnObj, v);
                if (manager->addWire(p_src.x, p_src.y + 1, v))
                {
                    num = v->getConnections().front();
                    v = new nnObjWire(eWire::wireAngleUpRight);
                    MEMCHK(InnObj, v);
                    v->setConnections(num);
                    res = manager->addWire(p_src.x-1, p_src.y + 1, v);
                }
            }
        }
        else if (near_src != nullptr && near_dst == nullptr)
        {
            num = near_src->getConnections().back();
            if (num == 2)
            {
                //vert to power positive
                for (i = p_src.y + 1; i <= manager->getHeight(); i++)
                {
                    v = new nnObjWire(eWire::wireVertical);
                    MEMCHK(InnObj, v);
                    v->setConnections(2);
                    res = manager->addWire(p_src.x, i, v);
                    if (!res)break;
                }
            }
            else
            {
                if ((p_src.x - p_dst.x) > 1)
                {
                    v = new nnObjWire(eWire::noWire);
                    MEMCHK(InnObj, v);
                    v->setConnections(num);
                    if (manager->addWire(p_src.x - 1, p_src.y + 1, v))
                    {
                        for (i = p_src.x - 2; i > p_dst.x; i--)
                        {
                            v = new nnObjWire(eWire::noWire);
                            MEMCHK(InnObj, v);
                            v->setConnections(num);
                            res = manager->addWire(i, p_src.y + 1, v);
                            if (!res)break;
                        }
                        if (res)
                        {
                            v = new nnObjWire(eWire::wireAngleUpRight);
                            MEMCHK(InnObj, v);
                            v->setConnections(num);
                            res = manager->addWire(i, p_src.y + 1, v);
                        }
                    }
                }
                else
                {
                    v = new nnObjWire(eWire::wireAngleUpRight);
                    MEMCHK(InnObj, v);
                    v->setConnections(num);
                    res = manager->addWire(p_src.x-1, p_src.y + 1, v);
                }
            }
        }
        else if (near_src == nullptr && near_dst != nullptr)
        {
            res=true;
        }
    }
    return res;
}

/////////////////////////////////////////////////////////////////////////
bool nnConnection::connectVertWireComponent(IManager *manager, nnPoint &p_src, nnPoint & p_dst)
{
    bool res = false;
    int u;
    InnObj *v = nullptr;
    int num = 0;
    InnObj *src = manager->getObj(p_src.x, p_src.y);
    InnObj *dst = manager->getObj(p_dst.x, p_dst.y);
    if (src != nullptr && !src->isComponent())
        num = src->getConnections().front();
    if (dst != nullptr && !dst->isComponent())
        num = dst->getConnections().front();
    //vert connection
    if (p_src.y > p_dst.y)
    {
        for (u = p_dst.y + 1; u < p_src.y; u++)
        {
            v = manager->getObj(p_src.x, u);
            if (v == nullptr)
            {
                v = new nnObjWire(eWire::noWire);
                MEMCHK(InnObj, v);
                v->setConnections(num);
                res = manager->addWire(p_src.x, u, v);
                if (!res)break;
            }
            else if (v->isComponent())
            {
                positionBusyException *pe=new positionBusyException(p_src.x, u);
                throw (pe);
            }
        }
    }
    else
    {
        for (u = p_src.y + 1; u < p_dst.y; u++)
        {
            v = manager->getObj(p_src.x, u);
            if (v == nullptr)
            {
                v = new nnObjWire(eWire::noWire);
                MEMCHK(InnObj, v);
                v->setConnections(num);
                res = manager->addWire(p_src.x, u, v);
                if (!res)break;
            }
            else if (v->isComponent())
            {
                positionBusyException *pe = new positionBusyException(p_src.x, u);
                throw (pe);
            }
        }
    }
    return res;
}

