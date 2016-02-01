#include <stdio.h>
#include <string>
#include "n2appmanager.h"
#include "n2miniXml.h"
#include "n2imagemanager.h"
#include "n2draw.h"
#include "n2view.h"
#include "n2drawmanager.h"
#include "n2toolview.h"
#include "n2exception.h"
#include "n2exthandler.h"
#include "n2selector.h"
#include "n2scroller.h"
#include "n2viewglue.h"
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

//TestviewGlue.cpp : T1
nnViewGlue::nnViewGlue(IChild *_parent)
    :parent(_parent),vscroller(nullptr),hscroller(nullptr),view(nullptr)
{
    if (parent)
    {
        selector = new nnSelector();
        select_start.set(-1);
        select_stop.set(-1);
        const_Size.set(0);
        setPhyView(0, 0);
        offset_Pos.set(0);

        unselect();
    }
}

//TestviewGlue.cpp : T3
bool nnViewGlue::unselect()
{
    select_start.x = -1;
    select_start.y = -1;
    select_stop.x = -1;
    select_stop.y = -1;
    status = s_unselect;
    show_cmd= show_none;
    if(selector)
        selector->hide();
    if (vscroller)
        vscroller->hide();
    if (hscroller)
        hscroller->hide();
    return true;
}


nnViewGlue::~nnViewGlue()
{
    select_start.set(-1);
    select_stop.set(-1);
    const_Size.set(0);
    setPhyView(0, 0);
    offset_Pos.set(0);
    if(selector)
    {
        selector->setFont(nullptr);
        delete selector;
        selector=nullptr;
    }
    if (vscroller)
    {
        delete vscroller;
        vscroller = nullptr;
    }
    if (hscroller)
    {
        delete hscroller;
        hscroller = nullptr;
    }
}


IFontManager * nnViewGlue::getFontFromName(const char *fname)
{
    IFontManager *res = nullptr;
    if (parent)
    {
        IFontList *fonts = parent->getFont();
        if (fonts)
        {
            fontNameList f_list;
            std::string fontName;
            if (fonts->getFontNameList(f_list))
            {
                for (auto i : f_list)
                {
                    if (i.find(fname))
                    {
                        fontName = i;
                        break;
                    }
                }
            }
            if (fontName.size() > 0)
            {
                res = fonts->getManager(fontName.c_str());
            }
        }
    }
    return res;
}

//TestviewGlue.cpp : T2
nnPoint nnViewGlue::getCoordPhy(const nnPoint & logPoint)
{
    nnPoint res(0, 0);
    nnPoint log = logPoint;
    log -= offset_Pos;
    res=  log*const_Size;
    return res;
}

nnPoint nnViewGlue::getMirrorCoordPhy(int height,int x, int y)
{
    nnPoint res(0, 0);
    res.x = ((x-offset_Pos.x)*const_Size.x);
    res.y = height-((y - offset_Pos.y)*const_Size.y)-const_Size.y;
    return res;
}


//TestviewGlue.cpp : T2
nnPoint nnViewGlue::getCoordLog(const nnPoint & phyPoint)
{
    nnPoint res(0, 0);
    res=offset_Pos+(phyPoint/const_Size);
    return res;
}

//TestviewGlue.cpp : T2
bool nnViewGlue::readConfiguration(IXmlNode *node)
{
    int w=0, h=0;
    bool res = false;
    if (parent)
    {
        if (node)
        {
            IXmlNode *conf = node->find(X("PHY_MAP"));
            if (conf)
            {
                IXmlNode *t = conf->find(X("TYPE"));
                if (t)
                {
                    switch (t->getLong())
                    {
                    case 1:
                        view = new nnView(parent);
                        toolview = new nnToolView();
                        break;
                    default:
                    {
                        xmlConfigurationNodeException *pe = new xmlConfigurationNodeException(X("TYPE"));
                        throw (pe);
                    }
                    break;
                    }
                    MEMCHK(IView, view);
                    MEMCHK(IToolView, toolview);
                }
                else
                {
                    xmlConfigurationNodeException *pe = new xmlConfigurationNodeException(X("TYPE"));
                    throw (pe);
                }
                t = conf->find(X("X"));
                if (t)
                {
                    const_Size.x = t->getLong();
                }
                else
                {
                    xmlConfigurationNodeException *pe = new xmlConfigurationNodeException(X("X"));
                    throw (pe);
                }
                t = conf->find(X("Y"));
                if (t)
                {
                    const_Size.y = t->getLong();
                }
                else
                {
                    xmlConfigurationNodeException  *pe = new xmlConfigurationNodeException(X("Y"));
                    throw (pe);
                }
                t = conf->find(X("WIDTH"));
                if (t)
                {
                    w = t->getLong();
                }
                else
                {
                    xmlConfigurationNodeException  *pe = new xmlConfigurationNodeException(X("WIDTH"));
                    throw (pe);
                }
                t = conf->find(X("HEIGHT"));
                if (t)
                {
                    h = t->getLong();
                }
                else
                {
                    xmlConfigurationNodeException *pe = new xmlConfigurationNodeException(X("HEIGHT"));
                    throw (pe);
                }
                setPhyView(w, h);
                res = true;
            }
            else
            {
                xmlConfigurationNodeException *pe = new xmlConfigurationNodeException(X("PHY_MAP"));
                throw (pe);
            }
            if (res)
            {
                conf = node->find(X("PHY_VIEW"));
                if (conf)
                {
                    if (view)
                        res = view->readConfiguration(conf);
                }
                else
                {
                    res = false;
                    xmlConfigurationNodeException *pe = new xmlConfigurationNodeException(X("PHY_VIEW"));
                    throw (pe);
                }
                if (res)
                {
                    conf = node->find(X("PHY_TOOLBARS"));
                    if (conf)
                    {
                        if (toolview)
                        {
                            IFontManager *font = getFontFromName("810");
                            selector->setFont(font);
                            toolview->setFont(font);
                            view->setFont(font);
                            res = toolview->readConfiguration(conf);
                        }
                    }
                    else
                    {
                        res = false;
                        xmlConfigurationNodeException *pe = new xmlConfigurationNodeException(X("PHY_TOOLBARS"));
                        throw (pe);
                    }
                }
            }
        }
        if (res)
        {
            if (needScrollBarHorz())
            {
                int w = getScrollableHorzSize();
                if (w)
                {
                    hscroller = new nnScroller(0, w);
                    //hscroller->setHorzArea(phy_Size.x, phy_Size.y);
                    hscroller->hide();
                }
                else
                {
                    hscroller = nullptr;
                }
            }
            if (needScrollBarHorz())
            {
                int h = getScrollableVertSize();
                if (h)
                {
                    vscroller = new nnScroller(0, h);
                    //vscroller->setVertArea(phy_Size.x, phy_Size.y);
                    vscroller->hide();
                }
                else
                {
                    vscroller = nullptr;
                }
            }
        }
    }
    return res;
}

//TestviewGlue.cpp : T3
bool nnViewGlue::selectStart(int xpos, int ypos)
{
    bool res = false;
    nnPoint p(xpos, ypos);
    selectStart(p);
    return res;
}
//TestviewGlue.cpp : T3
bool nnViewGlue::selectStop(int xpos, int ypos)
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
    if (parent)
    {
        IManager  * manager = parent->getManager();
        if (manager)
        {
            int log_height = manager->getHeight(); //logic coord
            int log_width = manager->getWidth(); //logic coord
            if (pos.x < log_width && pos.y < log_height)
            {
                select_start = pos;
                res = true;
            }
        }
    }
    return res;
}
//TestviewGlue.cpp : T3
bool nnViewGlue::selectStop(nnPoint pos)
{
    bool res = false;
    if (parent != nullptr)
    {
        IManager  * manager = parent->getManager();
        if (manager)
        {
            int log_height = manager->getHeight(); //logic coord
            int log_width = manager->getWidth(); //logic coord
            if (pos.x < log_width && pos.y < log_height)
            {
                select_stop = pos;
                res = true;
            }
        }
    }
    return res;
}

//TestviewGlue.cpp : T1 T3
bool nnViewGlue::getSelectAreaPhy(int & width, int & height)
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
            width = const_Size.x;
            height = const_Size.y;
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
            width = const_Size.x;
            height = const_Size.y;
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
bool nnViewGlue::getSelectStartPhy(int & x, int & y)
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
    else if (isStartValid() && !isStopValid())
    {
        p = select_start;
        p = getCoordPhy(p);
        x = p.x;
        y = p.y;
        res = true;
    }
    else if (!isStartValid() && isStopValid())
    {
        p = select_stop;
        p = getCoordPhy(p);
        x = p.x;
        y = p.y;
        res = true;

    }
    return res;
}


bmpImage & nnViewGlue::getDraw(void)
{
    bmpImage & image = view->getMainBitmap();
    if (image.isValid())
    {
        if (show_cmd == show_scroller_horz)
        {
            if (hscroller)
                hscroller->draw(image, this);
        }
        if (show_cmd == show_scroller_vert)
        {
            if (vscroller)
                vscroller->draw(image, this);
        }
        if (show_cmd == show_toolbar)
        {
            if (toolview)
                toolview->draw(image, this);
        }
        else
            {
                if (selector)
                {
                    if (isStartValid() && isStopValid())
                    {
                        selector->show();
                        selector->draw(image, select_start, select_stop, this);                        
                    }
                    else
                        selector->hide();
                }
            }
    }
    return image;
}


bool nnViewGlue::updateDraw(void)
{
    bool res = false;
    if (view)
    {
        res = view->draw();
    }
    return res;
}


bool nnViewGlue::handlerMouseMove(nn_mouse_buttons buttons, nnPoint phyPoint)
{
    bool res = false;
    if (parent)
    {
            IExtHandler *hook = parent->getHandler();
            if (show_cmd == show_none)
            {
                if (buttons == nn_m_button_left)
                {
                    if (status == start_activate)
                        status = start_resize;
                    if (status == start_resize)
                    {
                        nnPoint p = getCoordLog(phyPoint);
                        if (p != select_stop)
                        {

                            IManager *manager = parent->getManager();
                            if (manager)
                            {
                                nnPoint maxStop = manager->getSchema();
                                maxStop -= 1;
                                bool error = false;
                                if (p.x > maxStop.x)
                                {
                                    error = true;
                                    p.x = maxStop.x;
                                }
                                if (p.y > maxStop.y)
                                {
                                    error = true;
                                    p.y = maxStop.y;
                                }
                                selector->setError(error);
                                if (p.x < select_start.x || p.y < select_start.y)
                                {
                                    select_stop = select_start;
                                    select_start = p;
                                }
                                else
                                {
                                    select_stop = p;
                                }
                                if (hook)hook->doHandler(action_update_statusbars_panes, 0);
                                res = true;
                            }
                        }
                    }
                }
                else
                    if (buttons == nn_m_button_unknow)
                    {
                        if (hscroller)
                            res = hscroller->handlerMouseMove(phyPoint, show_cmd, hook);
                        if (vscroller)
                            res = vscroller->handlerMouseMove(phyPoint, show_cmd, hook);
                    }
            }
            else
            if (show_cmd == show_toolbar)
            {
                if (toolview)
                {
                    res = toolview->handlerMouseMove(phyPoint, hook);
                }
            }
            else
            if (show_cmd == show_scroller_horz)
            {
                if (hscroller)
                    res = hscroller->handlerMouseMove(phyPoint, show_cmd, hook);
            }
            else
            if (show_cmd == show_scroller_vert)
            {
                if (vscroller)
                    res = vscroller->handlerMouseMove(phyPoint, show_cmd, hook);
            }
    }
    return res;
}


bool nnViewGlue::handlerMouseButtonDown(nn_mouse_buttons buttons, nnPoint phyPoint)
{
    bool res = false;
    if (parent)
    {
            IExtHandler *hook = parent->getHandler();
            if (buttons == nn_m_button_left && show_cmd == show_none)
            {
                if (status == s_unselect || status == selected)
                    status = start_activate;
                    IManager *manager = parent->getManager();
                    if (manager)
                    {
                        nnPoint p = getCoordLog(phyPoint);
                        nnPoint maxStop = manager->getSchema();
                        maxStop -= 1;
                        bool error = false;
                        if (p.x > maxStop.x)
                        {
                            error = true;
                            p.x = maxStop.x;
                        }
                        if (p.y > maxStop.y)
                        {
                            error = true;
                            p.y = maxStop.y;
                        }
                        selector->setError(error);
                        selectStart(p);
                        select_stop = select_start;
                        if (hook)hook->doHandler(action_update_statusbars_panes, 0);
                        res = true;
                    }
            }
            else if (buttons == nn_m_button_left && show_cmd == show_toolbar)
            {
                int command = 0;
                bmpImage & bkg = view->getMainBitmap();
                nnPoint point = phyPoint;
                point.y = bkg.getHeight() - point.y;
                res = handlerRequestCommand(point, command);
                if (res)
                {
                    if (toolview)
                    {
                        if (toolview->checkIntCommand(command))
                        {
                            show_cmd = show_toolbar;
                            toolview->setDrawPosition(phyPoint);
                            res = true;
                        }
                        else
                        {
                            res = toolview->checkIntCommand(0);
                            if (hook)
                            {
                                nnAbstractParam<int> *t=new nnAbstractParam<int>(command);
                                hook->doHandler(action_host_command, t);
                            }
                            show_cmd = show_none;
                        }
                    }
                    if (hook)
                    {
                        if (hook)hook->doHandler(action_redraw);
                    }
                }
            }
            else if (buttons == nn_m_button_right && show_cmd == show_none)
            {
                if (toolview && view)
                {
                    show_cmd = show_toolbar;
                    toolview->setDrawPosition(phyPoint);
                    res = true;
                    if (hook)
                    {
                        if (hook)hook->doHandler(action_redraw);
                    }
                }
            }
            else if (buttons == nn_m_button_right && show_cmd == show_toolbar)
            {
                if (view)
                {
                    res = toolview->checkIntCommand(0);
                    show_cmd = show_none;
                    if (hook)
                    {
                        if (hook)hook->doHandler(action_redraw);
                    }
                }
            }
            else
                if (buttons == nn_m_button_left && show_cmd == show_scroller_horz)
                {
                    if (hscroller)
                        res = hscroller->handlerMouseButtonDown(phyPoint, this);
                }
                else
                    if (buttons == nn_m_button_left && show_cmd == show_scroller_vert)
                    {
                        if (vscroller)
                            res = vscroller->handlerMouseButtonDown(phyPoint, this);
                    }

    }
    return res;
}

bool nnViewGlue::handlerMouseButtonUp(nn_mouse_buttons buttons, nnPoint phyPoint)
{
    (buttons);
    bool res = true;
    if (parent)
    {
        if (show_cmd == show_none)
        {
        IExtHandler *hook = parent->getHandler();
        if (status == start_resize)
        {
            if (parent)
            {
                IManager *manager = parent->getManager();
                if (manager)
                {
                    status = stop_resize;
                    nnPoint p = getCoordLog(phyPoint);
                    nnPoint maxStop = manager->getSchema();
                    maxStop -= 1;
                    bool error = false;
                    if (p.x > maxStop.x)
                    {
                        error = true;
                        p.x = maxStop.x;
                    }
                    if (p.y > maxStop.y)
                    {
                        error = true;
                        p.y = maxStop.y;
                    }
                    selector->setError(error);
                    if (p != select_stop)
                    {
                        select_stop = p;
                    }
                    if (hook)hook->doHandler(action_update_statusbars_panes, 0);
                    status = selected;
                }
            }
        }
        else if (status == start_activate)
        {
            nnPoint p = getCoordLog(phyPoint);
            if (p != select_stop)
            {
                select_stop = p;
            }
            if (hook)hook->doHandler(action_update_statusbars_panes, 0);
            status = selected;
            }

        }
    }
    return res;
}

bool nnViewGlue::handlerScrollHorz(int pos)
{
    //abs position
    bool res = false;
    if(show_cmd==show_none)
    {
        if (parent)
        {
            IManager *manager = parent->getManager();
            if (manager)
            {
                if (pos >= 0)
                {
                    int w = getScrollableHorzSize();
                    if (pos > w)
                        pos = w;
                    if (manager != nullptr && offset_Pos.x != pos)
                    {
                        offset_Pos.x = pos;
                        updateDraw();
                        res = true;
                    }
                }
            }
        }
    }
    return res;
}

bool nnViewGlue::handlerScrollVert(int pos)
{
    //abs position
    bool res = false;
    if(show_cmd==show_none)
    {
        if (parent)
        {
            IManager *manager = parent->getManager();
            if (manager)
            {
                if (pos >= 0)
                {
                    int h = getScrollableVertSize();
                    if (pos > h)
                        pos = h;
                    if (manager != nullptr && offset_Pos.y != pos)
                    {
                        offset_Pos.y = pos;
                        updateDraw();
                        res = true;
                    }
                }
            }
        }
    }
    return res;
}

nnPoint nnViewGlue::getMap(void) 
{
    nnPoint res = phy_Size / const_Size;
    if (parent)
    {
        IManager *manager = parent->getManager();
        if (manager)
        {
            nnPoint size = manager->getSchema();
            if ((res.y + offset_Pos.y) > size.y)
                res.y = size.y - offset_Pos.y;
            if ((res.x + offset_Pos.x) > size.x)
                res.x = size.x - offset_Pos.x;
        }
    }
    return res;
}


bool nnViewGlue::setPhyView(int w, int h)
{
    nnPoint v(w, h);
    bool res = false;
    if (const_Size.y != 0)
        {
            if (h % const_Size.y)
                phy_Size.y = ((h / const_Size.y) + 1)*const_Size.y;
            else
                phy_Size.y = h;
        }
    else
        phy_Size.y = h;

    if (const_Size.x != 0)
        {
            if (w % const_Size.x)
                phy_Size.x = ((w / const_Size.x) + 1)*const_Size.x;
            else
                phy_Size.x = w;
        }
    else
        phy_Size.x = w;

    if (v != phy_Size)
    {
        res = true;
    }
    return res;
}


bool nnViewGlue::resize(int w, int h)
{
    bool res=false;
    if (parent)
    {
           if (setPhyView(w, h))
            {
                // request  reallign windows to bitmaps...
                IExtHandler *hook = parent->getHandler();
                if (hook)
                {
                    nnAbstractParam<nnPoint> *t = new nnAbstractParam<nnPoint>(phy_Size);
                    hook->doHandler(action_align_windows, t);
                }
            }
            else
            {
                res = view->remapMainBitmap(phy_Size);
                if (res)
                {
                    IImageManager *images = parent->getImage();
                    if (images)
                    {
                        if (needScrollBarHorz())
                        {
                            if (!hscroller)
                            {
                                int w = getScrollableHorzSize();
                                if (w)
                                {
                                    hscroller = new nnScroller(0, w);
                                    hscroller->hide();
                                }
                                if (hscroller)
                                {
                                    bmpImage *one = images->getImage(X("ScrollHorzLeft"));
                                    bmpImage *two = images->getImage(X("ScrollHorzRight"));
                                    if (one && two)
                                    {
                                        hscroller->setImage(one, two);
                                    }
                                }
                            }
                            if (hscroller)
                            {
                                hscroller->setHorzArea(phy_Size);
                            }
                        }
                        else
                        {
                            if (hscroller)
                            {
                                delete hscroller;
                                hscroller = nullptr;
                            }
                            offset_Pos.x = 0;
                        }
                        if (needScrollBarVert())
                        {
                            if (!vscroller)
                            {
                                int h = getScrollableVertSize();
                                if (h)
                                {
                                    vscroller = new nnScroller(0, h);
                                    vscroller->hide();
                                }
                                if (vscroller)
                                {
                                    bmpImage *one = images->getImage(X("ScrollVertUp"));
                                    bmpImage *two = images->getImage(X("ScrollVertDown"));
                                    if (one && two)
                                    {
                                        vscroller->setImage(one, two);
                                    }
                                }
                            }

                            if (vscroller)
                            {
                                vscroller->setVertArea(phy_Size);
                            }
                        }
                        else
                        {
                            if (vscroller)
                            {
                                delete vscroller;
                                vscroller = nullptr;
                            }
                            offset_Pos.y = 0;
                        }
                        res = updateDraw();
                        IExtHandler *hook = parent->getHandler();
                        if (hook)
                            hook->doHandler(action_redraw);
                    }
                }
        }
    }
    return res;
}


bool nnViewGlue::needScrollBarHorz(void)
{
    bool res = false;
    if (parent)
    {
        IManager  * manager = parent->getManager();
        if (manager)
        {
            int w = manager->getWidth()*const_Size.x;

            res=(phy_Size.x < w);
        }
    }
    return res;
}

bool nnViewGlue::needScrollBarVert(void)
{
    bool res = false;
    if (parent)
    {
        IManager  * manager = parent->getManager();
        if (manager)
        {
            int h = (manager->getHeight() + 2)*const_Size.y;
            res= (phy_Size.y < h);
        }
    }
    return res;
}

bool nnViewGlue::getVisibleArea(nnRect & area)
{
    area.set(offset_Pos.x,offset_Pos.y,offset_Pos.x+getPageWidth(),offset_Pos.y+getPageHeight());
    return true;
}


int nnViewGlue::getScrollableHorzSize(void)
{
    int res=0;
    if (parent)
    {
        IManager  * manager = parent->getManager();
        if (manager)
        {
            res = manager->getWidth() - getPageWidth() + 1;
            if (res < 0)
                res = 0;
        }
    }
    return res;
}

int nnViewGlue::getScrollableVertSize(void)
{
    int res=0;
    if (parent)
    {
        IManager  * manager = parent->getManager();
        if (manager)
        {
            res = manager->getHeight() - getPageHeight() + 1;
            if (res < 0)
                res = 0;
        }
    }
    return res;
}


bool nnViewGlue::handlerEscapeButton(bool shift, bool ctrl, bool alt)
{
    bool res=false;
    if(show_cmd==show_none)
    {
        if(!alt && !ctrl && !shift)
        {
            status = s_unselect;
            res=unselect();
            if (parent)
            {
                    IExtHandler *hook = parent->getHandler();
                    if (hook)hook->doHandler(action_update_statusbars_panes, 0);
            }
        }
    }
    return res;
}


bool nnViewGlue::handlerHomeButton(bool shitf, bool ctrl, bool alt)
{
    bool res=false;
    if(show_cmd==show_none)
    {
        if(!alt && !ctrl && !shitf)
        {
            if (parent)
            {
                IManager  * manager = parent->getManager();
                if (manager != nullptr)
                {
                    offset_Pos.x = offset_Pos.y = 0;
                    updateDraw();
                    IExtHandler *hook = parent->getHandler();
                    if (hook)hook->doHandler(action_update_statusbars_panes, 0);
                    if (vscroller)
                        vscroller->update(0);
                    if (hscroller)
                        hscroller->update(0);
                    res = true;
                }
            }
        }
    }
    return res;
}

bool nnViewGlue::handlerEndButton(bool shitf, bool ctrl, bool alt)
{
    bool res=false;
    if(show_cmd==show_none)
    {
        if(!alt && !ctrl && !shitf)
        {
            if (parent)
            {
                IManager  * manager = parent->getManager();
                if (manager != nullptr)
                {
                    offset_Pos.x = getScrollableHorzSize();
                    offset_Pos.y = getScrollableVertSize();
                    updateDraw();
                    IExtHandler *hook = parent->getHandler();
                    if (hook)hook->doHandler(action_update_statusbars_panes, 0);
                    if (vscroller)
                        vscroller->update(offset_Pos.y);
                    if (hscroller)
                        hscroller->update(offset_Pos.x);
                    res = true;
                }
            }
        }
    }
    return res;
}

bool nnViewGlue::handlerPageUpButton(bool shitf, bool ctrl, bool alt)
{
    bool res=false;
    if (parent)
    {
        if (show_cmd == show_none || show_cmd == show_scroller_vert)
        {
            if (!alt && !ctrl && !shitf)
            {
                IManager  * manager = parent->getManager();
                if (manager != nullptr)
                {
                    int p = getPageHeight();
                    if (offset_Pos.y > p)
                        offset_Pos.y -= p;
                    else
                        offset_Pos.y = 0;
                    updateDraw();
                    IExtHandler *hook = parent->getHandler();
                    if (hook)hook->doHandler(action_update_statusbars_panes, 0);
                    if (vscroller)
                        vscroller->update(offset_Pos.y);
                    res = true;
                }
            }
            else
            if (!alt && !ctrl && shitf)
            {
                IManager  * manager = parent->getManager();
                if (manager != nullptr)
                {
                    int p = getPageHeight();
                    bool scroll = false;
                    res = moveSelectArea(0, -p, scroll);
                    if (selector && selector->getStatus())
                        selector->setError(!res);
                    updateDraw();
                    IExtHandler *hook = parent->getHandler();
                    if (hook)hook->doHandler(action_update_statusbars_panes, 0);
                    if (vscroller)
                        vscroller->update(offset_Pos.y);
                    res = true;
                }
            }
        }
    }
    return res;
}



bool nnViewGlue::handlerPageDownButton(bool shitf, bool ctrl, bool alt)
{
    bool res=false;
    if (parent)
    {
        if (show_cmd == show_none || show_cmd == show_scroller_vert)
        {
            if (!alt && !ctrl && !shitf)
            {
                IManager  * manager = parent->getManager();
                if (manager != nullptr)
                {
                    int h = getScrollableVertSize();
                    int p = getPageHeight();
                    offset_Pos.y += p;
                    if (offset_Pos.y > h)
                        offset_Pos.y = h;
                    updateDraw();
                    IExtHandler *hook = parent->getHandler();
                    if (hook)hook->doHandler(action_update_statusbars_panes, 0);
                    if (vscroller)
                        vscroller->update(offset_Pos.y);
                    res = true;
                }
            }
            else
            if (!alt && !ctrl && shitf)
            {
                IManager  * manager = parent->getManager();
                if (manager != nullptr)
                {
                    int p = getPageHeight();
                    bool scroll = false;
                    res = moveSelectArea(0, p, scroll);
                    if (selector && selector->getStatus())
                        selector->setError(!res);
                    updateDraw();
                    IExtHandler *hook = parent->getHandler();
                    if (hook)hook->doHandler(action_update_statusbars_panes, 0);
                    if (vscroller)
                        vscroller->update(offset_Pos.y);
                    res = true;
                }
            }
        }
    }
    return res;
}


bool nnViewGlue::handlerPageRightButton(bool shitf, bool ctrl, bool alt)
{
    bool res = false;
    if (parent)
    {
        if (show_cmd == show_none || show_cmd == show_scroller_horz)
        {
            if (!alt && !ctrl && !shitf)
            {
                IManager  * manager = parent->getManager();
                if (manager != nullptr)
                {
                    int w = getScrollableHorzSize();
                    int p = getPageWidth();
                    offset_Pos.x += p;
                    if (offset_Pos.x > w)
                        offset_Pos.x = w;
                    updateDraw();
                    IExtHandler *hook = parent->getHandler();
                    if (hook)hook->doHandler(action_update_statusbars_panes, 0);
                    if (hscroller)
                        hscroller->update(offset_Pos.x);
                    res = true;
                }
            }
            else
                if (!alt && !ctrl && shitf)
                {
                    IManager  * manager = parent->getManager();
                    if (manager != nullptr)
                    {
                        int p = getPageWidth();
                        bool scroll = false;
                        res = moveSelectArea(p, 0, scroll);
                        if (selector && selector->getStatus())
                            selector->setError(!res);
                        updateDraw();
                        IExtHandler *hook = parent->getHandler();
                        if (hook)
                            hook->doHandler(action_update_statusbars_panes, 0);
                        if (hscroller)
                            hscroller->update(offset_Pos.x);
                        res = true;
                    }
                }
        }
    }
    return res;
}

bool nnViewGlue::handlerPageLeftButton(bool shitf, bool ctrl, bool alt)
{
    bool res = false;
    if (parent)
    {
        if (show_cmd == show_none || show_cmd == show_scroller_horz)
        {
            if (!alt && !ctrl && !shitf)
            {
                IManager  * manager = parent->getManager();
                if (manager != nullptr)
                {
                    int p = getPageWidth();
                    if (offset_Pos.x > p)
                        offset_Pos.x -= p;
                    else
                        offset_Pos.x = 0;
                    updateDraw();
                    IExtHandler *hook = parent->getHandler();
                    if (hook)hook->doHandler(action_update_statusbars_panes, 0);
                    if (hscroller)
                        hscroller->update(offset_Pos.x);
                    res = true;
                }
            }
            else
            if (!alt && !ctrl && shitf)
            {
                IManager  * manager = parent->getManager();
                if (manager != nullptr)
                {
                    int p = getPageWidth();
                    bool scroll = false;
                    res = moveSelectArea(-p, 0, scroll);
                    if (selector && selector->getStatus())
                        selector->setError(!res);
                    updateDraw();
                    IExtHandler *hook = parent->getHandler();
                    if (hook)
                        hook->doHandler(action_update_statusbars_panes, 0);
                    if (hscroller)
                        hscroller->update(offset_Pos.x);
                    res = true;
                }
            }
        }
    }
    return res;
}


bool nnViewGlue::handlerLeftButton(bool shift, bool ctrl, bool alt)
{
    bool res = false;
    if (parent)
    {
            IExtHandler *hook = parent->getHandler();
            if (show_cmd == show_none)
            {
                if (shift && !ctrl && !alt)
                {
                    if (status == selected)
                    {
                        IManager  * manager = parent->getManager();
                        if (manager)
                        {
                            resizeSelectArea(-1, 0);
                            if (hook)hook->doHandler(action_update_statusbars_panes, 0);
                            res = true;
                        }
                    }
                }
                else if (!shift && !ctrl && !alt)
                {
                    if (status == selected)
                    {
                        IManager  * manager = parent->getManager();
                        if (manager)
                        {
                            bool scroll = false;
                            res = moveSelectArea(-1, 0, scroll);
                            if (selector && selector->getStatus())
                                selector->setError(!res);
                            if (hook)hook->doHandler(action_update_statusbars_panes, 0);
                            if (scroll && hscroller)
                                hscroller->update(offset_Pos.x);
                        }
                    }
                }
            }
    }
    return res;
}

bool nnViewGlue::handlerRightButton(bool shift, bool ctrl, bool alt)
{
    bool res = false;
    if (parent)
    {
        IManager  * manager = parent->getManager();
            IExtHandler *hook = parent->getHandler();
            if (show_cmd == show_none)
            {
                if (shift && !ctrl && !alt)
                {
                    if (status == selected)
                    {
                        if (manager)
                        {
                            resizeSelectArea(1, 0);
                            if (hook)hook->doHandler(action_update_statusbars_panes, 0);
                            res = true;
                        }
                    }
                }
                else if (!shift && !ctrl && !alt)
                {
                    if (status == selected)
                    {
                        if (manager)
                        {
                            bool scroll = false;
                            res = moveSelectArea(1, 0, scroll);
                            if (selector && selector->getStatus())
                                selector->setError(!res);
                            if (hook)hook->doHandler(action_update_statusbars_panes, 0);
                            if (scroll && hscroller)
                                hscroller->update(offset_Pos.x);
                        }
                    }
                }
            }
    }
    return res;
}

bool nnViewGlue::handlerUpButton(bool shift, bool ctrl, bool alt)
{
    bool res = false;
    if (parent)
    {
        IManager  * manager = parent->getManager();
            IExtHandler *hook = parent->getHandler();
            if (show_cmd == show_none)
            {
                if (shift && !ctrl && !alt)
                {
                    if (status == selected)
                    {
                        if (manager)
                        {
                            resizeSelectArea(0, -1);
                            if (hook)hook->doHandler(action_update_statusbars_panes, 0);
                            res = true;
                        }
                    }
                }
                else if (!shift && !ctrl && !alt)
                {
                    if (status == selected)
                    {
                        if (manager)
                        {
                            bool scroll = false;
                            res = moveSelectArea(0, -1, scroll);
                            if (selector && selector->getStatus())
                                selector->setError(!res);
                            if (hook)hook->doHandler(action_update_statusbars_panes, 0);
                            if (scroll && vscroller)
                                vscroller->update(offset_Pos.y);
                        }
                    }
                }
            }
    }
    return res;
}

bool nnViewGlue::handlerDownButton(bool shift, bool ctrl, bool alt)
{
    bool res = false;
    if (parent)
    {
        IManager  * manager = parent->getManager();
        IExtHandler *hook = parent->getHandler();
        if (show_cmd == show_none)
        {
            if (shift && !ctrl && !alt)
            {
                if (status == selected)
                {
                    if (manager)
                    {
                        resizeSelectArea(0, 1);
                        if (hook)hook->doHandler(action_update_statusbars_panes, 0);
                        res = true;
                    }
                }
            }
            else if (!shift && !ctrl && !alt)
            {
                if (status == selected)
                {
                    if (manager)
                    {
                        bool scroll = false;
                        res = moveSelectArea(0, 1, scroll);
                        if (selector && selector->getStatus())
                            selector->setError(!res);
                        if (hook)hook->doHandler(action_update_statusbars_panes, 0);
                        if (scroll && vscroller)
                            vscroller->update(offset_Pos.y);
                    }
                }
            }
        }
    }
    return res;
}

bool nnViewGlue::moveSelectArea(const int vx,const int vy,bool &needScroll)
{
    bool res=false;
    needScroll=false;
    if (parent)
    {
        IManager  * manager = parent->getManager();
        if (isStartValid() && isStopValid())
        {
            nnRect vis;
            if (vx != 0)
            {
                if (manager)
                {
                    int sw = getScrollableHorzSize();
                    int w = manager->getWidth() - 1;
                    if ((vx > 0 && select_start.x < w && select_stop.x < w) ||
                        (vx < 0 && select_start.x>0 && select_stop.x>0))
                    {
                        select_start.x += vx;
                        select_stop.x += vx;
                        getVisibleArea(vis);
                        if (vx > 0 && select_stop.x >= (vis.stop.x - 1) && offset_Pos.x < w)
                        {
                            offset_Pos.x += (vx);
                            if (offset_Pos.x > sw)
                                offset_Pos.x = sw;
                            updateDraw();
                            needScroll = true;
                        }
                        if (vx < 0 && select_start.x <= (vis.start.x - 1) && offset_Pos.x>0)
                        {
                            offset_Pos.x += (vx);
                            if (offset_Pos.x < 0)
                                offset_Pos.x = 0;
                            updateDraw();
                            needScroll = true;
                        }
                        res = true;
                    }
                }
            }
            if (vy != 0)
            {
                int sh = getScrollableVertSize();
                if (manager)
                {
                    int h = manager->getHeight() - 1;
                    if ((vy > 0 && select_start.y < h && select_stop.y < h) ||
                        (vy < 0 && select_start.y>0 && select_stop.y>0))
                    {
                        select_start.y += vy;
                        select_stop.y += vy;
                        getVisibleArea(vis);
                        if (vy > 0 && select_stop.y >= (vis.stop.y - 1) && offset_Pos.y < h)
                        {
                            offset_Pos.y += (vy);
                            if (offset_Pos.y > sh)
                                offset_Pos.y = sh;
                            updateDraw();
                            needScroll = true;
                        }
                        if (vy < 0 && select_start.y <= vis.start.y && offset_Pos.y>0)
                        {
                            offset_Pos.y += (vy);
                            if (offset_Pos.y < 0)
                                offset_Pos.y = 0;
                            updateDraw();
                            needScroll = true;
                        }
                        res = true;
                    }
                }
            }
        }
    }
    return res;
}


bool nnViewGlue::resizeSelectArea(const int vx,const int vy)
{
    bool res=false;
    if (parent)
    {
        IManager  * manager = parent->getManager();
        if (isStartValid() && isStopValid() && parent)
        {
            if (manager)
            {
                if (vx != 0)
                {
                    int w = manager->getWidth() - 1;
                    if (vx < 0)
                    {
                        nnPoint diff = select_stop - select_start;
                        if (diff.x>0)
                        {
                            select_stop.x += vx;
                            res = true;
                        }
                    }
                    if (vx > 0)
                    {
                        if (select_stop.x < w)
                            select_stop.x += vx;
                        res = true;
                    }
                }
                if (vy != 0)
                {
                    int h = manager->getHeight() - 1;
                    if (vy < 0)
                    {
                        nnPoint diff = select_stop - select_start;
                        if (diff.y>0)
                        {
                            select_stop.y += vy;
                            res = true;
                        }
                    }
                    if (vy > 0)
                    {
                        if (select_stop.y < h)
                            select_stop.y += vy;
                        res = true;
                    }
                }
            }
        }
    }
    return res;
}


bool nnViewGlue::handlerRequestCommand(nnPoint phyPoint,int & command)
{
    bool res=false;
    if(toolview && show_cmd== show_toolbar)
    {
        if(toolview->getActiveCommander()!=nullptr)
        {
            res=toolview->handlerRequestCommand(phyPoint,command);
        }
    }
    return res;
}




bool nnViewGlue::loadImages(const XCHAR * _path)
{
    bool res=false;
    if(toolview)
    {
        res=toolview->loadImages(_path);
    }
    if (parent)
    {
        IImageManager *images = parent->getImage();
        if (hscroller)
        {
            bmpImage *one = images->getImage(X("ScrollHorzLeft"));
            bmpImage *two = images->getImage(X("ScrollHorzRight"));
            if (one && two)
            {
                hscroller->setImage(one, two);
            }
        }
        if (vscroller)
        {
            bmpImage *one = images->getImage(X("ScrollVertUp"));
            bmpImage *two = images->getImage(X("ScrollVertDown"));
            if (one && two)
            {
                vscroller->setImage(one, two);
            }
        }
    }
    return res;
}


