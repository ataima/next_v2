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
    res.x = (x*const_x);
    res.y = (y*const_y);
    return res;
}

nnPoint nnViewGlue::getMirrorCoordPhy(size_t x, size_t y)
{
    nnPoint res(0, 0);
    res.x = p_width-(x*const_x)-const_x;
    res.y = p_height-(y*const_y) - const_y;
    return res;
}


//TestviewGlue.cpp : T2
nnPoint nnViewGlue::getCoordLog(nnPoint & phyPoint)
{
    nnPoint res(0, 0);
    res.x = offset_x+phyPoint.x / const_x;
    res.y = offset_y+phyPoint.y / const_y;
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
            case 0:
                view = new nnTextView();
                break;
            case 1:
                view = new nnView(images);
                break;
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
        if (pos.x < log_width && pos.y < log_height)
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
        if (pos.x < log_width && pos.y < log_height)
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
        else if (isStartValid() && !isStopValid())
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
        p.x -= offset_x;
        p.y -= offset_y;
        p = getCoordPhy(p);
        x = p.x;
        y = p.y;
        res = true;
    }
    else if (isStartValid() && !isStopValid())
    {
        p = select_start;
        p.x -= offset_x;
        p.y -= offset_y;
        p = getCoordPhy(p);
        x = p.x;
        y = p.y;
        res = true;
    }
    else if (!isStartValid() && isStopValid())
    {
        p = select_stop;
        p.x -= offset_x;
        p.y -= offset_y;
        p = getCoordPhy(p);
        x = p.x;
        y = p.y;
        res = true;

    }
    return res;
}


bmpImage & nnViewGlue::getDraw(void)
{
    return view->getMainBitmap();
}

bool nnViewGlue::updateDraw(void)
{
    bool res = false;
    if (view != nullptr && manager != nullptr)
    {
        res = view->draw(manager, this);
    }
    return res;
}


bool nnViewGlue::handlerMouseMove(nn_mouse_buttons buttons, nnPoint phyPoint,nnPoint &start,nnPoint & stop)
{
    bool res = false;
    if(buttons==nn_m_button_left)
    {
        if (status == start_activate)
            status = start_resize;
        if (status == start_resize)
        {
            nnPoint p = getCoordLog(phyPoint);
            if (p != select_stop)
            {
                select_stop = p;
                getSelectArea(start,stop);
                res=true;
            }
        }

    }
    return res;
}


bool nnViewGlue::handlerMouseButtonDown(nn_mouse_buttons buttons, nnPoint phyPoint,nnPoint &start,nnPoint & stop)
{
    bool res = false;
    if(buttons==nn_m_button_left)
    {
        if (status == s_unselect || status == selected)
            status = start_activate;
        selectStart(getCoordLog(phyPoint));
        select_stop = select_start;
        getSelectArea(start,stop);
        res=true;
    }
    return res;
}

bool nnViewGlue::handlerMouseButtonUp(nn_mouse_buttons buttons, nnPoint phyPoint, nnPoint &start, nnPoint &stop)
{
    (buttons);
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
    else if (status == start_activate)
    {
        nnPoint p = getCoordLog(phyPoint);
        if (p != select_stop)
        {
            select_stop = p;
        }
        status = selected;
        getSelectArea(start,stop);
    }
    return res;
}

bool nnViewGlue::handlerScrollHorz(size_t pos)
{
    //abs position
    bool res = false;
    if(pos>=0)
    {
        size_t w = getScrollableHorzSize();
        if(pos>w)
            pos=w;
        if (manager != nullptr && offset_x != pos )
        {
            offset_x = pos;
            updateDraw();
            res=true;
        }
    }
    return res;
}

bool nnViewGlue::handlerScrollVert(size_t pos)
{
    //abs position
    bool res = false;
    if(pos>=0)
    {
        size_t h = getScrollableVertSize();
        if(pos>h)
            pos=h;
        if (manager != nullptr && offset_y != pos )
        {
            offset_y = pos;
            updateDraw();
            res=true;
        }
    }
    return res;
}



bool nnViewGlue::resize(size_t w, size_t h)
{
    bool res=false;
    p_height=((h/const_y)+1)*const_y;
    p_width=((w/const_x)+1)*const_x;
    res=view->remapMainBitmap(p_width,p_height);
    if(res)
        res=updateDraw();
    return res;
}


bool nnViewGlue::needScrollBarHorz(void)
{
    size_t w=manager->getWidth()*const_x;
    return (p_width<w);
}

bool nnViewGlue::needScrollBarVert(void)
{
    size_t h=manager->getHeight()*const_y;
    return (p_height<h);
}


bool nnViewGlue::isSelectAreaPhyVisible(nnRect &result, nnPoint &start, nnPoint &stop)
{
    bool res=false;
    if(isStartValid() && isStopValid())
    {
        nnRect area;
        if(getVisibleArea(area))
        {
            if(getSelectArea(start,stop))
            {
                //stop+=1;
                nnRect sel(start,stop);
                //sel=area.in(sel);
                if(sel.isValid())
                {
                    sel.start.x-=offset_x;
                    sel.stop.x-=offset_x;
                    sel.start.y-=offset_y;
                    sel.stop.y-=offset_y;
                    result.start=getCoordPhy(sel.start);
                    result.stop=getCoordPhy(sel.stop);
                    result.stop.x+=const_x;
                    result.stop.y+=const_y;
                    res=true;
                }
            }
        }
    }
    return res;
}



bool nnViewGlue::getVisibleArea(nnRect & area)
{
    area.set(offset_x,offset_y,offset_x+getPageWidth(),offset_y+getPageHeight());
    return true;
}


size_t nnViewGlue::getScrollableHorzSize(void)
{
    if(manager)
    {
        return manager->getWidth()-getPageWidth()+1;
    }
    else
    {
        return 0;
    }
}

size_t nnViewGlue::getScrollableVertSize(void)
{
    if(manager)
    {
        return manager->getHeight()-getPageHeight()+1;
    }
    else
    {
        return 0;
    }
}


bool nnViewGlue::handlerEscapeButton(bool shift, bool ctrl, bool alt, nnPoint &start, nnPoint &stop)
{
    bool res=false;
    if(!alt && !ctrl && !shift)
    {
        status = s_unselect;
        res=unselect();
        start=select_start;
        stop=select_stop;
    }
    return res;
}


bool nnViewGlue::handlerHomeButton(bool shitf, bool ctrl, bool alt, nnPoint &pos)
{
    bool res=false;
    if(!alt && !ctrl && !shitf)
    {
        if (manager != nullptr )
        {
            offset_x=offset_y=0;
            updateDraw();
            pos.x=offset_x;
            pos.y=offset_y;
            res=true;
        }
    }
    return res;
}

bool nnViewGlue::handlerEndButton(bool shitf, bool ctrl, bool alt, nnPoint &pos)
{
    bool res=false;
    if(!alt && !ctrl && !shitf)
    {
        if (manager != nullptr )
        {
            offset_x = getScrollableHorzSize();
            offset_y = getScrollableVertSize();
            updateDraw();
            pos.x=offset_x;
            pos.y=offset_y;
            res=true;
        }
    }
    return res;
}

bool nnViewGlue::handlerPageUpButton(bool shitf, bool ctrl, bool alt, nnPoint &pos)
{
    bool res=false;
    if(!alt && !ctrl && shitf)
    {
        if (manager != nullptr )
        {
            size_t p=getPageHeight();
            if(offset_y>p)
                offset_y-=p;
            else
                offset_y=0;
            updateDraw();
            pos.x=offset_x;
            pos.y=offset_y;
            res=true;
        }
    }
    return res;
}

bool nnViewGlue::handlerPageDownButton(bool shitf, bool ctrl, bool alt, nnPoint &pos)
{
    bool res=false;
    if(!alt && !ctrl && shitf)
    {
        if (manager != nullptr )
        {
            size_t h = getScrollableVertSize();
            size_t p=getPageHeight();
            offset_y+=p;
            if(offset_y>h)
                offset_y=h;
            updateDraw();
            pos.x=offset_x;
            pos.y=offset_y;
            res=true;
        }
    }
    return res;
}


bool nnViewGlue::handlerLeftButton(bool shift,bool ctrl,bool alt,nnPoint &start, nnPoint & stop,bool & needScroll)
{
    bool res = false;
    if(shift && !ctrl && !alt)
    {
        if (status == selected)
        {
            if(manager)
            {
                resizeSelectArea(-1,0);
                getSelectArea(start,stop);
                res=true;
            }
        }
    }
    else if(!shift && !ctrl && !alt)
    {
        if (status == selected)
        {
            if(manager)
            {
                res=moveSelectArea(-1,0,needScroll);
                getSelectArea(start,stop);
            }
        }
    }
    return res;
}

bool nnViewGlue::handlerRightButton(bool shift,bool ctrl,bool alt,nnPoint &start, nnPoint & stop,bool & needScroll)
{
    bool res = false;
    if(shift && !ctrl && !alt)
    {
        if (status == selected)
        {
            if(manager)
            {
                resizeSelectArea(1,0);
                getSelectArea(start,stop);
                res=true;
            }
        }
    }
    else if(!shift && !ctrl && !alt)
    {
        if (status == selected)
        {
            if(manager)
            {
                res=moveSelectArea(1,0,needScroll);
                getSelectArea(start,stop);
            }
        }
    }
    return res;
}

bool nnViewGlue::handlerUpButton(bool shift,bool ctrl,bool alt,nnPoint &start, nnPoint & stop,bool & needScroll)
{
    bool res = false;
    if(shift && !ctrl && !alt)
    {
        if (status == selected)
        {
            if(manager)
            {
                resizeSelectArea(0,-1);
                getSelectArea(start,stop);
                res=true;
            }
        }
    }
    else if(!shift && !ctrl && !alt)
    {
        if (status == selected)
        {
            if(manager)
            {
                res=moveSelectArea(0,-1,needScroll);
                getSelectArea(start,stop);
            }
        }
    }
    return res;
}

bool nnViewGlue::handlerDownButton(bool shift, bool ctrl, bool alt, nnPoint &start, nnPoint & stop, bool &needScroll)
{
    bool res = false;
    if(shift && !ctrl && !alt)
    {
        if (status == selected)
        {
            if(manager)
            {
                resizeSelectArea(0,1);
                getSelectArea(start,stop);
                res=true;
            }
        }
    }
    else if(!shift && !ctrl && !alt)
    {
        if (status == selected)
        {
            if(manager)
            {
                res=moveSelectArea(0,1,needScroll);
                getSelectArea(start,stop);
            }
        }
    }
    return res;
}

bool nnViewGlue::moveSelectArea(const int vx,const int vy,bool &needScroll)
{
    bool res=false;
    needScroll=false;
    if(select_start.isValid() && select_stop.isValid())
    {
        nnRect vis;
        if(vx!=0)
        {
            size_t sw = getScrollableHorzSize();
            size_t w = manager->getWidth();
            select_start.adjustX(1,w,vx);
            select_stop.adjustX(1,w,vx);
            getVisibleArea(vis);
            if(vx>0 && select_start.x>=vis.stop.x && select_start.x<w)
            {
                offset_x+=(vx);
                if(offset_x>sw)
                    offset_x=sw;
                updateDraw();
                needScroll=true;
            }
            if(vx<0 && select_start.x<=vis.start.x && select_start.x>0)
            {
                offset_x+=(vx);
                if(offset_x<0)
                    offset_x=0;
                updateDraw();
                needScroll=true;
            }
            res=true;
        }
        if(vy!=0)
        {
            size_t sh = getScrollableVertSize();
            size_t h = manager->getHeight();
            select_start.adjustY(1,h,vy);
            select_stop.adjustY(1,h,vy);
            getVisibleArea(vis);
            if(vy>0 && select_start.y>=vis.stop.y && select_start.y<h)
            {
                offset_y+=(vy);
                if(offset_y>sh)
                    offset_y=sh;
                updateDraw();
                needScroll=true;
            }
            if(vy<0 && select_start.y<=vis.start.y && select_start.y>0)
            {
                offset_y-=(vy);
                if(offset_y<0)
                    offset_y=0;
                updateDraw();
                needScroll=true;
            }
            res=true;
        }
    }
    return res;
}


bool nnViewGlue::resizeSelectArea(const int vx,const int vy)
{
    bool res=false;
    if(select_start.isValid() && select_stop.isValid())
    {
        if(vx!=0)
        {
            size_t w=manager->getWidth();
            if(vx<0)
            {
                nnPoint diff=select_stop-select_start;
                if(diff.x>0)
                {
                    select_stop.adjustX(1,w,-1);
                    res=true;
                }
            }
            if(vx>0)
            {
                if(manager)
                {
                    select_stop.adjustX(1,w,1);
                    res=true;
                }
            }
        }
        if(vy!=0)
        {
            size_t h=manager->getHeight();
            if(vy<0)
            {
                nnPoint diff=select_stop-select_start;
                if(diff.y>0)
                {
                    select_stop.adjustY(1,h,-1);
                    res=true;
                }
            }
            if(vy>0)
            {
                if(manager)
                {
                    select_stop.adjustY(1,h,1);
                    res=true;
                }
            }
        }
    }
    return res;
}
