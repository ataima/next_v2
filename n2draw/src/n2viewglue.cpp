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
#include "n2caption.h"
#include "n2viewglue.h"
#include "n2capturepos.h"


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
    :parent(_parent),view(nullptr),
     vscroller(nullptr),
     hscroller(nullptr),
     curFont(nullptr)
{
    if (parent)
    {
        selector = new nnSelector(parent);
        const_Size.set(0);
        setPhyView(0, 0);
        offset_Pos.set(0);
        caption = new nnCaption(parent);
        capture = new nnCapturePos(parent,255,255,255);
        unselect();
    }
}

//TestviewGlue.cpp : T3
bool nnViewGlue::unselect()
{
    show_cmd = show_none;
    if (selector)
        selector->unselect();
    if (vscroller)
        vscroller->hide();
    if (hscroller)
        hscroller->hide();
    if (caption)
        caption->hide();
    focus=nullptr;
    return true;
}


nnViewGlue::~nnViewGlue()
{
    curFont = nullptr;
    const_Size.set(0);
    setPhyView(0, 0);
    offset_Pos.set(0);
    if(selector)
    {
        selector->setFont(nullptr);
        delete selector;
        selector=nullptr;
    }
    if (caption)
    {
        caption->setFont(nullptr);
        delete caption;
        caption = nullptr;
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
    if (capture)
    {
        capture->setFont(nullptr);
        delete capture;
        capture = nullptr;
    }
    focus=nullptr;
}



bool nnViewGlue::select(nnPoint pos1, nnPoint pos2)
{
    bool res = false;
    if (selector)
        res = selector->select(pos1, pos2);
    return res;
}

bool nnViewGlue::getSelectArea(nnPoint &start, nnPoint &stop)
{
    bool res = false;
    if (selector)
        res = selector->getSelectArea(start,stop);
    return res;
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
                    if (i==fname)
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

nnPoint nnViewGlue::getMirrorCoordPhy(nnPoint size,int x, int y)
{
    nnPoint res(0, 0);
    if(orientation.x)
    {
        res.x = size.x-((x-offset_Pos.x)*const_Size.x)-const_Size.x;
    }
    else
    {
        res.x = ((x-offset_Pos.x)*const_Size.x);
    }
    if(orientation.y)
    {
        res.y = size.x-((y - offset_Pos.y)*const_Size.y)-const_Size.y;
    }
    else
    {
        res.y = ((y-offset_Pos.y)*const_Size.y);
    }
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
    int w=0, h=0,sx=0,sy=0;
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
                        maintool = new nnToolView();
                        compotool = new nnToolView();
                        break;
                    default:
                    {
                        xmlConfigurationNodeException *pe = new xmlConfigurationNodeException(X("TYPE"));
                        throw (pe);
                    }
                    break;
                    }
                    MEMCHK(IView, view);
                    MEMCHK(IToolView, maintool);
                    MEMCHK(IToolView, compotool);
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
                t = conf->find(X("SWAPX"));
                if (t)
                {
                    sx = t->getLong();
                }
                else
                {
                    xmlConfigurationNodeException *pe = new xmlConfigurationNodeException(X("SWAPX"));
                    throw (pe);
                }
                t = conf->find(X("SWAPY"));
                if (t)
                {
                    sy = t->getLong();
                }
                else
                {
                    xmlConfigurationNodeException *pe = new xmlConfigurationNodeException(X("SWAPY"));
                    throw (pe);
                }
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
                    conf = node->find(X("MAIN_TOOLBARS"));
                    if (conf)
                    {
                        if (maintool)
                        {
                            res = maintool->readConfiguration(conf);
                        }
                    }
                    else
                    {
                        res = false;
                        xmlConfigurationNodeException *pe = new xmlConfigurationNodeException(X("MAIN_TOOLBARS"));
                        throw (pe);
                    }
                    if (res)
                    {
                        conf = node->find(X("COMP_TOOLBARS"));
                        if (conf)
                        {
                            if (compotool)
                            {
                                res = compotool->readConfiguration(conf);
                            }
                        }
                        else
                        {
                            res = false;
                            xmlConfigurationNodeException *pe = new xmlConfigurationNodeException(X("COMP_TOOLBARS"));
                            throw (pe);
                        }
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
                    hscroller = new nnScroller(parent,0, w);
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
                    vscroller = new nnScroller(parent,0, h);
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
    if (res)
    {
        curFont = getFontFromName("Courier_10_14");
        selector->setFont(curFont);
        caption->setFont(curFont);
        maintool->setFont(curFont);
        compotool->setFont(curFont);
        view->setFont(curFont);
        capture->setFont(curFont);
    }
    return res;
}


//TestviewGlue.cpp : T1 T3
bool nnViewGlue::getSelectAreaPhy(int & width, int & height)
{
    bool res = false;
    if (selector)
    {
        int diff = selector->isSelected();
        if (diff>1)
        {

            nnPoint phyStart = getCoordPhy(selector->getSelectStart());
            nnPoint phyStop = getCoordPhy(selector->getSelectStop());
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
        {
            if (selector->isStartValid())
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
    }
    return res;
}



bmpImage & nnViewGlue::getDraw(void)
{
    bmpImage & image = view->getMainBitmap();
    if (image.isValid())
    {
        if(focus!=nullptr)
            focus->draw(image, this);
        else // to port code to draw selector and set focus to selector
            if (selector)
            {
                if (selector->isSelectedValid())
                {
                    selector->show();
                    selector->draw(image, this);
                }
                else
                    selector->hide();
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


bool nnViewGlue::handlerMouseMove(nn_mouse_buttons buttons, nnPoint & phyPoint)
{
    bool res = false;
    //nnLOG(show_status,"handlerMouseMove in : ", show_cmd);
    if (parent)
    {
        IExtHandler *hook = parent->getHandler();
        if (show_cmd == show_none)
        {
            if (buttons == nn_m_button_left)
            {
                if (selector)
                {
                    nnPoint nn=getCoordLog(phyPoint);
                    selector->handlerMouseMove(nn);
                }
            }
            else if (buttons == nn_m_button_unknow)
            {
                if(caption)
                    res=caption->handlerMouseMove(phyPoint, show_cmd, hook);
                if (!res)
                {
                    if (hscroller)
                        res = hscroller->handlerMouseMove(phyPoint, show_cmd, hook);
                    if (!res)
                    {
                        if (vscroller)
                            res = vscroller->handlerMouseMove(phyPoint, show_cmd, hook);
                    }
                }
            }
        }
        else if(focus != nullptr)
        {
            res = focus->handlerMouseMove(phyPoint, show_cmd, hook);
        }
    }
    //nnLOG(show_status, "handlerMouseMove out : ", show_cmd);
    return res;
}


bool nnViewGlue::handlerMouseButtonDown(nn_mouse_buttons buttons, nnPoint & phyPoint)
{
    bool res = false;
    //nnLOG(show_status, "handlerMouseButtonDown in : ", show_cmd);
    if (parent)
    {
        IExtHandler *hook = parent->getHandler();
        if (buttons == nn_m_button_left )
        {
            if (show_cmd == show_none)
            {
                if (selector)
                {
                    nnPoint nn=getCoordLog(phyPoint);
                    res = selector->handlerMouseButtonDown(nn, show_cmd);
                }
            }
            else if (focus !=nullptr )
            {
                res = focus->handlerMouseButtonDown(phyPoint, show_cmd, hook);
                if ( show_cmd == show_capture  && selector!=nullptr )
                {
                    nnPoint nn=getCoordLog(phyPoint);
                    res = selector->handlerMouseButtonDown(nn, show_cmd);
                }
            }
        }
        else if (buttons == nn_m_button_right)
        {
            if ( show_cmd == show_none)
            {
                if (selector)
                {
                    if (selector->isSelectedComponent())
                    {
                        show_cmd = show_toolbar_compo;
                        compotool->show(phyPoint);
                        focus=compotool;
                    }
                    else
                    {
                        show_cmd = show_toolbar_main;
                        maintool->show(phyPoint);
                        focus=maintool;
                    }
                    res = true;
                }
            }
            else if ( focus !=nullptr )
            {
                focus->hide();
                unselect();
            }
            //
            if (hook)
            {
                if (hook)hook->doHandler(action_redraw);
            }
        }
    }
    //nnLOG(show_status, "handlerMouseButtonDown out : ", show_cmd);
    return res;
}

bool nnViewGlue::handlerMouseButtonUp(nn_mouse_buttons buttons, nnPoint & phyPoint)
{
    bool res = true;
    //nnLOG(show_status, "handlerMouseButtonUp in : ", show_cmd);
    if (selector)
    {
        if (show_cmd == show_none)
        {
            nnPoint nn=getCoordLog(phyPoint);
            res = selector->handlerMouseButtonUp(buttons, nn);
        }
    }
    //nnLOG(show_status, "handlerMouseButtonUp out : ", show_cmd);
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
    nnPoint t;
    bool res = false;
    IManager *manager = nullptr;
    nnPoint logSize;
    if (parent)
    {
        manager = parent->getManager();
        if(manager)
            logSize=manager->getSchema();
    }
    if (const_Size.y != 0)
    {
        if (h % const_Size.y)
            t.y = ((h / const_Size.y) )*const_Size.y;
        else
            t.y = h;
    }
    else
        t.y = h;

    if (const_Size.x != 0)
    {
        if (w % const_Size.x)
            t.x = ((w / const_Size.x) )*const_Size.x;
        else
            t.x = w;
    }
    else
        t.x = w;
    if (logSize.isValid())
    {
        if (const_Size.y)
        {
            if ((t.y / const_Size.y) > logSize.y)
            {
                t.y = logSize.y*const_Size.y;
            }
        }
        if (const_Size.x)
        {
            if ((t.x / const_Size.x) > logSize.x)
            {
                t.x = logSize.x*const_Size.x;
            }
        }
    }
    if (t != phy_Size)
    {
        phy_Size = t;
        res = true;
    }
    return res;
}


bool nnViewGlue::setOrientation(int h, int v)
{
    orientation.x=h;
    orientation.y=v;
    return true;
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
                auto *t = new nnAbstractParam<nnPoint>(phy_Size);
                hook->doHandler(action_align_windows, t);
            }
            res = view->remapMainBitmap(phy_Size);
            if (res)
            {
                if(caption)
                    caption->setArea(phy_Size);
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
                                hscroller = new nnScroller(parent,0, w);
                                hscroller->hide();
                            }
                            if (hscroller)
                            {
                                bmpImage *img = images->getImage(X("ScrollHorzLeft"));
                                if (img)
                                {
                                    hscroller->addImage(1, img);
                                }
                                img = images->getImage(X("ScrollHorzRight"));
                                if (img)
                                {
                                    hscroller->addImage(2, img);
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
                                vscroller = new nnScroller(parent,0, h);
                                vscroller->hide();
                            }
                            if (vscroller)
                            {
                                bmpImage *img = images->getImage(X("ScrollVertUp"));
                                if (img)
                                {
                                    vscroller->addImage(1, img);
                                }
                                img = images->getImage(X("ScrollVertDown"));
                                if (img)
                                {
                                    vscroller->addImage(2, img);
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
            int h = manager->getHeight()*const_Size.y;
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
            res = manager->getHeight() - getPageHeight() ;
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
            res=unselect();
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
                offset_Pos.x = offset_Pos.y = 0;
                updateDraw();
                IExtHandler *hook = parent->getHandler();
                if (vscroller)
                    vscroller->update(0);
                if (hscroller)
                    hscroller->update(0);
                if (hook)
                {
                    auto * p = new nnAbstractParam<nnPoint>(offset_Pos);
                    hook->doHandler(action_update_scroller_panes, p);
                    hook->doHandler(action_redraw);
                }
                res = true;
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
                offset_Pos.x = getScrollableHorzSize();
                offset_Pos.y = getScrollableVertSize();
                updateDraw();
                IExtHandler *hook = parent->getHandler();
                if (vscroller)
                    vscroller->update(offset_Pos.y);
                if (hscroller)
                    hscroller->update(offset_Pos.x);
                if (hook)
                {
                    auto * p = new nnAbstractParam<nnPoint>(offset_Pos);
                    hook->doHandler(action_update_scroller_panes, p);
                    hook->doHandler(action_redraw);
                }
                res = true;
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
                int p = getPageHeight();
                if (offset_Pos.y > p)
                    offset_Pos.y += p;
                else
                    offset_Pos.y = 0;
                updateDraw();
                IExtHandler *hook = parent->getHandler();
                if (vscroller)
                    vscroller->update(offset_Pos.y);
                if (hook)
                {
                    auto * p = new nnAbstractParam<nnPoint>(offset_Pos);
                    hook->doHandler(action_update_scroller_panes, p);
                    hook->doHandler(action_redraw);
                }
                res = true;
            }
            else if (!alt && !ctrl && shitf)
            {
                int p = getPageHeight();
                bool scroll = false;
                res = moveSelectArea(0, p, scroll);
                if (selector && selector->getStatus())
                    selector->setError(!res);
                updateDraw();
                IExtHandler *hook = parent->getHandler();
                if (vscroller)
                    vscroller->update(offset_Pos.y);
                if (hook)
                {
                    auto * p = new nnAbstractParam<nnPoint>(offset_Pos);
                    hook->doHandler(action_update_scroller_panes, p);
                    hook->doHandler(action_redraw);
                }
                res = true;
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
                int h = getScrollableVertSize();
                int p = getPageHeight();
                offset_Pos.y -= p;
                if (offset_Pos.y > h)
                    offset_Pos.y = h;
                updateDraw();
                IExtHandler *hook = parent->getHandler();
                if (vscroller)
                    vscroller->update(offset_Pos.y);
                if (hook)
                {
                    auto * p = new nnAbstractParam<nnPoint>(offset_Pos);
                    hook->doHandler(action_update_scroller_panes, p);
                    hook->doHandler(action_redraw);
                }
                res = true;
            }
            else if (!alt && !ctrl && shitf)
            {
                int p = getPageHeight();
                bool scroll = false;
                res = moveSelectArea(0, -p, scroll);
                if (selector && selector->getStatus())
                    selector->setError(!res);
                updateDraw();
                IExtHandler *hook = parent->getHandler();
                if (vscroller)
                    vscroller->update(offset_Pos.y);
                if (hook)
                {
                    auto * p = new nnAbstractParam<nnPoint>(offset_Pos);
                    hook->doHandler(action_update_scroller_panes, p);
                    hook->doHandler(action_redraw);
                }
                res = true;
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
                int w = getScrollableHorzSize();
                int p = getPageWidth();
                offset_Pos.x += p;
                if (offset_Pos.x > w)
                    offset_Pos.x = w;
                updateDraw();
                IExtHandler *hook = parent->getHandler();
                if (hscroller)
                    hscroller->update(offset_Pos.x);
                if (hook)
                {
                    auto * p = new nnAbstractParam<nnPoint>(offset_Pos);
                    hook->doHandler(action_update_scroller_panes, p);
                    hook->doHandler(action_redraw);
                }
                res = true;
            }
            else if (!alt && !ctrl && shitf)
            {
                int p = getPageWidth();
                bool scroll = false;
                res = moveSelectArea(p, 0, scroll);
                if (selector && selector->getStatus())
                    selector->setError(!res);
                updateDraw();
                IExtHandler *hook = parent->getHandler();
                if (hscroller)
                    hscroller->update(offset_Pos.x);
                if (hook)
                {
                    auto * p = new nnAbstractParam<nnPoint>(offset_Pos);
                    hook->doHandler(action_update_scroller_panes, p);
                    hook->doHandler(action_redraw);
                }
                res = true;
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
                int p = getPageWidth();
                if (offset_Pos.x > p)
                    offset_Pos.x -= p;
                else
                    offset_Pos.x = 0;
                updateDraw();
                IExtHandler *hook = parent->getHandler();
                if (hscroller)
                    hscroller->update(offset_Pos.x);
                if (hook)
                {
                    auto * p = new nnAbstractParam<nnPoint>(offset_Pos);
                    hook->doHandler(action_update_scroller_panes, p);
                    hook->doHandler(action_redraw);
                }
                res = true;
            }
            else if (!alt && !ctrl && shitf)
            {
                int p = getPageWidth();
                bool scroll = false;
                res = moveSelectArea(-p, 0, scroll);
                if (selector && selector->getStatus())
                    selector->setError(!res);
                updateDraw();
                IExtHandler *hook = parent->getHandler();
                if (hscroller)
                    hscroller->update(offset_Pos.x);
                if (hook)
                {
                    auto * p = new nnAbstractParam<nnPoint>(offset_Pos);
                    hook->doHandler(action_update_scroller_panes, p);
                    hook->doHandler(action_redraw);
                }
                res = true;
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
                if (selector)
                {
                    selector->resizeSelectArea(-1, 0);
                    res = true;
                }
            }
            else if (!shift && !ctrl && !alt)
            {
                bool scroll = false;
                res = moveSelectArea(-1, 0, scroll);
                if (selector && selector->getStatus())
                    selector->setError(!res);
                if (scroll && hscroller)
                    hscroller->update(offset_Pos.x);
                if (hook)
                {
                    auto * p = new nnAbstractParam<nnPoint>(offset_Pos);
                    hook->doHandler(action_update_scroller_panes, p);
                    hook->doHandler(action_redraw);
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
                if (selector)
                {
                    selector->resizeSelectArea(1, 0);
                    res = true;
                }
            }
            else if (!shift && !ctrl && !alt)
            {
                if (manager)
                {
                    bool scroll = false;
                    res = moveSelectArea(1, 0, scroll);
                    if (selector && selector->getStatus())
                        selector->setError(!res);
                    if (scroll && hscroller)
                        hscroller->update(offset_Pos.x);
                    if (hook)
                    {
                        auto * p = new nnAbstractParam<nnPoint>(offset_Pos);
                        hook->doHandler(action_update_scroller_panes, p);
                        hook->doHandler(action_redraw);
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
        IExtHandler *hook = parent->getHandler();
        if (show_cmd == show_none)
        {
            if (shift && !ctrl && !alt)
            {
                if (selector)
                {
                    selector->resizeSelectArea(0, 11);
                    res = true;
                }
            }
            else if (!shift && !ctrl && !alt)
            {
                bool scroll = false;
                res = moveSelectArea(0, 1, scroll);
                if (selector && selector->getStatus())
                    selector->setError(!res);
                if (scroll && vscroller)
                    vscroller->update(offset_Pos.y);
                if (hook)
                {
                    auto * p = new nnAbstractParam<nnPoint>(offset_Pos);
                    hook->doHandler(action_update_scroller_panes, p);
                    hook->doHandler(action_redraw);
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
        IExtHandler *hook = parent->getHandler();
        if (show_cmd == show_none)
        {
            if (shift && !ctrl && !alt)
            {
                if (selector)
                {
                    selector->resizeSelectArea(0, -1);
                    res = true;
                }
            }
            else if (!shift && !ctrl && !alt)
            {
                bool scroll = false;
                res = moveSelectArea(0, -1, scroll);
                if (selector && selector->getStatus())
                    selector->setError(!res);
                if (scroll && vscroller)
                    vscroller->update(offset_Pos.y);
                if (hook)
                {
                    auto * p = new nnAbstractParam<nnPoint>(offset_Pos);
                    hook->doHandler(action_update_scroller_panes, p);
                    hook->doHandler(action_redraw);
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
    if (parent && selector)
    {
        IManager  * manager = parent->getManager();
        if (selector->isSelectedValid() && selector->getStatus()==true)
        {
            nnRect vis;
            nnPoint  select_start = selector->getSelectStart();
            nnPoint  select_stop = selector->getSelectStop();
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
                        if (vx > 0 && select_stop.x >= (vis.stop.x /*- 1*/) && offset_Pos.x < w)
                        {
                            offset_Pos.x += (vx);
                            if (offset_Pos.x > sw)
                                offset_Pos.x = sw;
                            updateDraw();
                            needScroll = true;
                        }
                        if (vx < 0 && select_start.x < (vis.start.x /*- 1*/) && offset_Pos.x>0)
                        {
                            offset_Pos.x += (vx);
                            if (offset_Pos.x < 0)
                                offset_Pos.x = 0;
                            updateDraw();
                            needScroll = true;
                        }
                        selector->select(select_start, select_stop);
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
                        if (vy > 0 && select_stop.y >= (vis.stop.y /*- 1*/) && offset_Pos.y < h)
                        {
                            offset_Pos.y += (vy);
                            if (offset_Pos.y > sh)
                                offset_Pos.y = sh;
                            updateDraw();
                            needScroll = true;
                        }
                        if (vy < 0 && select_start.y < vis.start.y && offset_Pos.y>0)
                        {
                            offset_Pos.y += (vy);
                            if (offset_Pos.y < 0)
                                offset_Pos.y = 0;
                            updateDraw();
                            needScroll = true;
                        }
                        selector->select(select_start, select_stop);
                        res = true;
                    }
                }
            }
        }
    }
    return res;
}







bool nnViewGlue::loadImages(STRING & _path)
{
    bool res=false;
    if(maintool)
    {
        res=maintool->loadImages(_path);
    }
    if (compotool)
    {
        res = compotool->loadImages(_path);
    }
    if (parent)
    {
        IImageManager *images = parent->getImage();
        if (hscroller)
        {
            bmpImage *img = images->getImage(X("ScrollHorzLeft"));
            if (img)
            {
                hscroller->addImage(1, img);
            }
            img = images->getImage(X("ScrollHorzRight"));
            if (img)
            {
                hscroller->addImage(2, img);
            }
        }
        if (vscroller)
        {
            bmpImage *img = images->getImage(X("ScrollVertUp"));
            if (img)
            {
                vscroller->addImage(1, img);
            }
            img = images->getImage(X("ScrollVertDown"));
            if (img)
            {
                vscroller->addImage(2, img);
            }
        }
        if (caption)
        {
            bmpImage *img = images->getImage(X("IconizeWindow"));
            if (img)
            {
                caption->addImage(0, img);
            }
            img = images->getImage(X("MedializeWindow"));
            if (img)
            {
                caption->addImage(1, img);
            }
            img = images->getImage(X("MaximizeWindow"));
            if (img)
            {
                caption->addImage(2, img);
            }
            img = images->getImage(X("CloseWindow"));
            if (img)
            {
                caption->addImage(3, img);
            }
        }
    }
    return res;
}


bool  nnViewGlue::Capture(int command,unsigned int image)
{
    bool res = false;
    if (capture)
    {
        capture->setCommand(command,image,selector->getSelectStart());
        show_cmd = show_capture;
    }
    return res;
}


bool nnViewGlue::handlerCancelButton(bool shift, bool ctrl, bool alt)
{
    bool res = false;
    if (parent)
    {
        IExtHandler *hook = parent->getHandler();
        if (show_cmd == show_none && !shift && !ctrl && !alt && selector && selector->getStatus())
        {
            int sel = selector->isSelected();
            if (sel != 0)
            {
                IManager *manager = parent->getManager();
                if (manager)
                {
                    int i, u;
                    nnPoint start,stop;
                    getSelectArea(start, stop);
                    if (sel > 1)
                    {
                        for (i = start.x; i < stop.x; i++)
                        {
                            for (u = start.y; u < stop.y; u++)
                            {
                                manager->removeObj(i, u);
                            }
                        }
                    }
                    else
                    {
                        manager->removeObj(start.x, start.y);
                    }
                }
                updateDraw();
                if (hook)
                {
                    if (hook)hook->doHandler(action_redraw);
                }
            }
        }
    }
    return res;
}
