#include <stdio.h>
#include <string>
#include "n2miniXml.h"
#include "n2imagemanager.h"
#include "n2drawmanager.h"
#include "n2viewglue.h"

//TestviewGlue.cpp : T1
nnViewGlue::nnViewGlue(IManager  *_manager,IImageManager *_images)
    :manager(_manager),images(_images)
{
    const_x = const_y = p_width = p_height = offset_x = offset_y = 0;
    unselect();
}

//TestviewGlue.cpp : T3
bool nnViewGlue::unselect()
{
    select_start.x = -1;
    select_start.y = -1;
    select_stop.x = -1;
    select_stop.y = -1;
    status = s_unselect;
    return true;
}


nnViewGlue::~nnViewGlue()
{
    const_x = const_y = p_width = p_height = offset_x = offset_y = 0;
}


//TestviewGlue.cpp : T2
nnPoint nnViewGlue::getCoordPhy(nnPoint & logPoint)
{
    nnPoint res(0, 0);
    res.x = logPoint.x*const_x;
    res.y = logPoint.y*const_y;
    return res;
}

nnPoint nnViewGlue::getCoordPhy(size_t x,size_t y)
{
    nnPoint res(0, 0);
    res.x = p_width-(x*const_x);
    res.y = p_height-(y*const_y);
    return res;
}

//TestviewGlue.cpp : T2
nnPoint nnViewGlue::getCoordLog(nnPoint & phyPoint)
{
    nnPoint res(0, 0);
    res.x = phyPoint.x / const_x;
    res.y = phyPoint.y / const_y;
    return res;
}

//TestviewGlue.cpp : T2
bool nnViewGlue::readConfiguration(miniXmlNode & node)
{
    bool res = false;
    miniXmlNode *conf = node.find(X("PHY_MAP"));
    if (conf)
    {
        miniXmlNode *t = conf->find(X("TYPE"));
        if (t)
        {
            switch (t->getLong())
            {
            case 0: view = new nnTextView(); break;
            case 1: view = new nnView(images); break;
            default:
            {
                phyGlueConfigurationException *pe = new phyGlueConfigurationException(X("TYPE"));
                throw (pe);
            }
            break;
            }
            MEMCHK(IView, view);
        }
        else
        {
            phyGlueConfigurationException *pe = new phyGlueConfigurationException(X("TYPE"));
            throw (pe);
        }
        t = conf->find(X("X"));
        if (t)
        {
            const_x = t->getLong();
        }
        else
        {
            phyGlueConfigurationException *pe = new phyGlueConfigurationException(X("X"));
            throw (pe);
        }
        t = conf->find(X("Y"));
        if (t)
        {
            const_y = t->getLong();
        }
        else
        {
            phyGlueConfigurationException  *pe = new phyGlueConfigurationException(X("Y"));
            throw (pe);
        }
        t = conf->find(X("WIDTH"));
        if (t)
        {
            p_width = t->getLong();
        }
        else
        {
            phyGlueConfigurationException  *pe = new phyGlueConfigurationException(X("WIDTH"));
            throw (pe);
        }
        t = conf->find(X("HEIGHT"));
        if (t)
        {
            p_height = t->getLong();
        }
        else
        {
            phyGlueConfigurationException *pe = new phyGlueConfigurationException(X("HEIGHT"));
            throw (pe);
        }
        res = view->createMainBitmap(p_width, p_height);
    }
    else
    {
        phyGlueConfigurationException *pe = new phyGlueConfigurationException(X("PHY_MAP"));
        throw (pe);
    }
    if (res)
    {
        conf = node.find(X("PHY_VIEW"));
        if (conf)
        {
            if (view)
                view->readConfiguration(conf);
        }
    }
    return res;
}

//TestviewGlue.cpp : T3
bool nnViewGlue::selectStart(size_t xpos, size_t ypos)
{
    bool res = false;
    nnPoint p(xpos, ypos);
    selectStart(p);
    return res;
}
//TestviewGlue.cpp : T3
bool nnViewGlue::selectStop(size_t xpos, size_t ypos)
{
    bool res = false;
    nnPoint p(xpos, ypos);
    selectStop(p);
    return res;
}
//TestviewGlue.cpp : T3
bool nnViewGlue::selectStart(nnPoint pos)
{
    bool res = false;
    if (manager != nullptr)
    {
        size_t log_height = manager->getHeight(); //logic coord
        size_t log_width = manager->getWidth(); //logic coord
        if (pos.x < log_width && pos.y < log_width)
        {
            select_start = pos;
            res = true;
        }
    }
    return res;
}
//TestviewGlue.cpp : T3
bool nnViewGlue::selectStop(nnPoint pos)
{
    bool res = false;
    if (manager != nullptr)
    {
        size_t log_height = manager->getHeight(); //logic coord
        size_t log_width = manager->getWidth(); //logic coord
        if (pos.x < log_width && pos.y < log_width)
        {
            select_stop = pos;
            res = true;
        }
    }
    return res;
}

//TestviewGlue.cpp : T1 T3
bool nnViewGlue::getSelectAreaPhy(size_t & width, size_t & height)
{
    bool res = false;
    if (select_start != select_stop)
    {
        if (isStartValid() && isStopValid())
        {
            nnPoint phyStart = getCoordPhy(select_start);
            nnPoint phyStop = getCoordPhy(select_stop);
            if (phyStop.x > phyStart.x)
            {
                width = phyStop.x - phyStart.x;
            }
            else
            {
                width = phyStart.x - phyStop.x;
            }
            if (phyStop.y > phyStart.y)
            {
                height = phyStop.y - phyStart.y;
            }
            else
            {
                height = phyStart.y - phyStop.y;
            }
            res = true;
        }
        else
            if (isStartValid() && !isStopValid())
            {
                width = const_x;
                height = const_y;
                res = true;
            }
            else
            {
                width = height = 0;
                res = true;
            }
    }
    else
    {
        if (isStartValid())
        {
            width = const_x;
            height = const_y;
            res = true;
        }
        else
        {
            width = height = 0;
            res = true;
        }
    }
    return res;
}

//TestviewGlue.cpp : T1 T3
bool nnViewGlue::getSelectStartPhy(size_t & x, size_t & y)
{
    nnPoint p(-1, -1);
    bool res = false;
    if (isStartValid() && isStopValid())
    {
        p = select_start.intersect(select_stop);
        p = getCoordPhy(p);
        x = p.x;
        y = p.y;
        res = true;
    }
    else
        if (isStartValid() && !isStopValid())
        {
            p = select_start;
            p = getCoordPhy(p);
            x = p.x;
            y = p.y;
            res = true;
        }
        else
            if (!isStartValid() && isStopValid())
            {
                p = select_stop;
                p = getCoordPhy(p);
                x = p.x;
                y = p.y;
                res = true;

            }
    return res;
}

bool nnViewGlue::handlerMouseMove(nn_mouse_buttons buttons, nnPoint phyPoint)
{


    bool res = true;
    if (status == start_activate)
        status = start_resize;
    if (status == start_resize)
    {
        nnPoint p = getCoordLog(phyPoint);
        if (p != select_stop)
        {
            select_stop = p;
        }
    }

    return res;
}


bool nnViewGlue::handlerEscapeButton(void)
{
    status = s_unselect;
    return unselect();
}


bmpImage & nnViewGlue::getDraw(void) 
{
    return view->getMainBitmap(); 
}

bool nnViewGlue::updateDraw(void)
{
    bool res = false;
    if (view != nullptr  && manager != nullptr)
    {
        
        res = view->draw(manager, this);
    }
    return res;
}


bool nnViewGlue::handlerMouseButtonDown(nn_mouse_buttons buttons, nnPoint phyPoint)
{
    bool res = true;
    if (status == s_unselect)
        status = start_activate;
    selectStart(getCoordLog(phyPoint));
    select_stop = select_start;
    return res;
}

bool nnViewGlue::handlerMouseButtonUp(nn_mouse_buttons buttons, nnPoint phyPoint)
{

    bool res = true;
    if (status == start_resize)
    {
        status = stop_resize;
        nnPoint p = getCoordLog(phyPoint);
        if (p != select_stop)
        {
            select_stop = p;
        }
        status = selected;
    }
    else
        if (status == start_activate)
        {
            nnPoint p = getCoordLog(phyPoint);
            if (p != select_stop)
            {
                select_stop = p;
            }
            status = selected;
        }
    return res;
}

bool nnViewGlue::handlerScrollHorz(size_t pos)
{
    //abs position 
    bool res = false;
    size_t page_width = p_width / const_x;
    if (manager != nullptr && offset_x != pos && pos < (manager->getWidth() - page_width) && pos >= 0)
    {
        offset_x = pos;
        updateDraw();
    }
    return res;
}

bool nnViewGlue::handlerScrollVert(size_t pos)
{
    //abs position 
    bool res = false;
    size_t page_height = p_height / const_y;
    if (manager != nullptr && offset_y != pos && pos < (manager->getHeight() - page_height) && pos >= 0)
    {
        offset_y = pos;
        updateDraw();
    }
    return res;
}
