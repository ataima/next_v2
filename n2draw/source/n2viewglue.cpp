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
#include "n2viewglue.h"

//TestviewGlue.cpp : T1
nnViewGlue::nnViewGlue(IManager  *_manager,IImageManager *_images, IFontList *_fonts)
    :manager(_manager),images(_images),fonts(_fonts)
{
    handlers = new nnExtHandlerList();
    selector= new nnSelector();
    select_start.set(-1);
    select_stop.set(-1);
    const_Size.set(0);
    phy_Size.set(0);
    offset_Pos.set(0);
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
    show_cmd=false;
    if(selector)
        selector->hide();
    return true;
}


nnViewGlue::~nnViewGlue()
{
    select_start.set(-1);
    select_stop.set(-1);
    const_Size.set(0);
    phy_Size.set(0);
    offset_Pos.set(0);
    if(handlers)
    {
        handlers->clear();
        delete handlers;
        handlers=nullptr;
    }
    if(selector)
    {
        selector->setFont(nullptr);
        delete selector;
        selector=nullptr;
    }
}


IFontManager * nnViewGlue::getFontFromName(const char *fname)
{
    IFontManager *res = nullptr;
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
    bool res = false;
    if(node)
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
                    view = new nnView(images);
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
                phy_Size.x = t->getLong();
            }
            else
            {
                xmlConfigurationNodeException  *pe = new xmlConfigurationNodeException(X("WIDTH"));
                throw (pe);
            }
            t = conf->find(X("HEIGHT"));
            if (t)
            {
                phy_Size.y = t->getLong();
            }
            else
            {
                xmlConfigurationNodeException *pe = new xmlConfigurationNodeException(X("HEIGHT"));
                throw (pe);
            }
            res = view->createMainBitmap(phy_Size.x, phy_Size.y);
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
                    res=view->readConfiguration(conf);
            }
            else
            {
                res=false;
                xmlConfigurationNodeException *pe = new xmlConfigurationNodeException(X("PHY_VIEW"));
                throw (pe);
            }
            if(res)
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
                    res=false;
                    xmlConfigurationNodeException *pe = new xmlConfigurationNodeException(X("PHY_TOOLBARS"));
                    throw (pe);
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
    if (manager != nullptr)
    {
        int log_height = manager->getHeight(); //logic coord
        int log_width = manager->getWidth(); //logic coord
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
        int log_height = manager->getHeight(); //logic coord
        int log_width = manager->getWidth(); //logic coord
        if (pos.x < log_width && pos.y < log_height)
        {
            select_stop = pos;
            res = true;
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
    if(show_cmd)
    {
        toolview->draw(image,this);
    }
    else
    {
        if(selector && image.isValid())
        {
            if( isStartValid() && isStopValid())
            {
                selector->draw(image,select_start,select_stop,this);
                selector->show();
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
    if (view != nullptr && manager != nullptr)
    {
        res = view->draw(manager, this);
    }
    return res;
}


bool nnViewGlue::handlerMouseMove(nn_mouse_buttons buttons, nnPoint phyPoint)
{
    bool res = false;
    if(show_cmd==false)
    {
        if(buttons==nn_m_button_left && show_cmd==false)
        {
            IExtHandler *hook=handlers->get(handler_exec_command);
            if (status == start_activate)
                status = start_resize;
            if (status == start_resize)
            {
                nnPoint p = getCoordLog(phyPoint);
                if (p != select_stop)
                {
                    nnPoint maxStop = manager->getSchema();
                    maxStop -= 1;
                    bool error = false;
                    if (p.x > maxStop.x )
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
                    select_stop = p;
                    if(hook)hook->doHandler(action_update_statusbars_panes,0);
                    res=true;
                }
            }
        }
    }
    else
    {
        if(toolview)
        {
            IExtHandler *hook=handlers->get(handler_exec_command);
            bmpImage & bkg= view->getMainBitmap();
            nnPoint point=phyPoint;
            point.y=bkg.getHeight()-point.y;
            res=toolview->handlerMouseMove(point,hook);
        }
    }
    return res;
}


bool nnViewGlue::handlerMouseButtonDown(nn_mouse_buttons buttons, nnPoint phyPoint)
{
    bool res = false;
    IExtHandler *hook=handlers->get(handler_exec_command);
    if(buttons==nn_m_button_left && show_cmd==false)
    {
        if (status == s_unselect || status == selected)
            status = start_activate;

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
        if(hook)hook->doHandler(action_update_statusbars_panes,0);
        res=true;
    }
    else if(buttons==nn_m_button_left && show_cmd==true)
    {
        int command=0;
        bmpImage & bkg= view->getMainBitmap();
        nnPoint point=phyPoint;
        point.y=bkg.getHeight()-point.y;
        res=handlerRequestCommand(point,command);
        if(res)
        {
            if(toolview)
            {
                if(toolview->checkIntCommand(command))
                {
                    show_cmd=true;
                    toolview->setDrawPosition(phyPoint);
                    res=true;
                }
                else
                {
                    res=toolview->checkIntCommand(0);
                    if(hook)hook->doHandler(action_host_command,command);
                    show_cmd=false;
                }
            }
            if(hook)
            {
                if(hook)hook->doHandler(action_update_from_ext_scroolbars);
                if(hook)hook->doHandler(action_redraw);
            }
        }
    }
    else if(buttons==nn_m_button_right && show_cmd==false)
    {
        if(toolview && view)
        {
            show_cmd=true;
            toolview->setDrawPosition(phyPoint);
            res=true;
            if(hook)
            {
                if(hook)hook->doHandler(action_redraw);
            }
        }
    }
    else if(buttons==nn_m_button_right && show_cmd==true)
    {
        if(view)
        {
            res=toolview->checkIntCommand(0);
            show_cmd=false;
            if(hook)
            {
                if(hook)hook->doHandler(action_update_from_ext_scroolbars);
                if(hook)hook->doHandler(action_redraw);
            }
        }
    }
    return res;
}

bool nnViewGlue::handlerMouseButtonUp(nn_mouse_buttons buttons, nnPoint phyPoint)
{
    (buttons);
    bool res = true;
    if(show_cmd==false)
    {
        IExtHandler *hook=handlers->get(handler_exec_command);
        if (status == start_resize)
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
            if(hook)hook->doHandler(action_update_statusbars_panes,0);
            status = selected;
        }
        else if (status == start_activate)
        {
            nnPoint p = getCoordLog(phyPoint);
            if (p != select_stop)
            {
                select_stop = p;
            }
            if(hook)hook->doHandler(action_update_statusbars_panes,0);
            status = selected;
        }
    }
    return res;
}

bool nnViewGlue::handlerScrollHorz(int pos)
{
    //abs position
    bool res = false;
    if(show_cmd==false)
    {
        if(pos>=0)
        {
            int w = getScrollableHorzSize();
            if(pos>w)
                pos=w;
            if (manager != nullptr && offset_Pos.x != pos )
            {
                offset_Pos.x = pos;
                updateDraw();
                res=true;
            }
        }
    }
    return res;
}

bool nnViewGlue::handlerScrollVert(int pos)
{
    //abs position
    bool res = false;
    if(show_cmd==false)
    {
        if(pos>=0)
        {
            int h = getScrollableVertSize();
            if(pos>h)
                pos=h;
            if (manager != nullptr && offset_Pos.y != pos )
            {
                offset_Pos.y = pos;
                updateDraw();
                res=true;
            }
        }
    }
    return res;
}

nnPoint nnViewGlue::getMap(void) 
{
    nnPoint res = phy_Size / const_Size;
    if (manager)
    {
        nnPoint size=manager->getSchema();
        if ((res.y+offset_Pos.y) > size.y)
            res.y = size.y- offset_Pos.y;
        if ((res.x+ offset_Pos.x) > size.x)
            res.x = size.x- offset_Pos.x;
    }
    return res;
}


bool nnViewGlue::resize(int w, int h)
{
    bool res=false;
    phy_Size.y=((h/const_Size.y)+1)*const_Size.y;
    phy_Size.x=((w/const_Size.x)+1)*const_Size.x;
    res=view->remapMainBitmap(phy_Size.x,phy_Size.y);
    if (res)
    {
        res = updateDraw();
        IExtHandler *hook = handlers->get(handler_exec_command);
        if (hook)hook->doHandler(action_redraw, 0);
    }
    return res;
}


bool nnViewGlue::needScrollBarHorz(void)
{
    int w=manager->getWidth()*const_Size.x;
    return (phy_Size.x<w);
}

bool nnViewGlue::needScrollBarVert(void)
{
    int h=manager->getHeight()*const_Size.y;
    return (phy_Size.y<h);
}

bool nnViewGlue::getVisibleArea(nnRect & area)
{
    area.set(offset_Pos.x,offset_Pos.y,offset_Pos.x+getPageWidth(),offset_Pos.y+getPageHeight());
    return true;
}


int nnViewGlue::getScrollableHorzSize(void)
{
    int res=0;
    if(manager)
    {
        res = manager->getWidth()-getPageWidth()+1;
        if(res<0)
            res=0;
    }
    return res;
}

int nnViewGlue::getScrollableVertSize(void)
{
    int res=0;
    if(manager)
    {
        res = manager->getHeight()-getPageHeight()+1;
        if(res<0)
            res=0;
    }
    return res;
}


bool nnViewGlue::handlerEscapeButton(bool shift, bool ctrl, bool alt)
{
    bool res=false;
    if(show_cmd==false)
    {
        if(!alt && !ctrl && !shift)
        {
            status = s_unselect;
            res=unselect();
            IExtHandler *hook=handlers->get(handler_exec_command);
            if(hook)hook->doHandler(action_update_statusbars_panes,0);
        }
    }
    return res;
}


bool nnViewGlue::handlerHomeButton(bool shitf, bool ctrl, bool alt)
{
    bool res=false;
    if(show_cmd==false)
    {
        if(!alt && !ctrl && !shitf)
        {
            if (manager != nullptr )
            {
                offset_Pos.x=offset_Pos.y=0;
                updateDraw();
                IExtHandler *hook=handlers->get(handler_exec_command);
                if(hook)hook->doHandler(action_update_statusbars_panes,0);
                if(hook)hook->doHandler(action_adjust_horz_scrollbar,0);
                if(hook)hook->doHandler(action_adjust_vert_scrollbar,0);
                res=true;
            }
        }
    }
    return res;
}

bool nnViewGlue::handlerEndButton(bool shitf, bool ctrl, bool alt)
{
    bool res=false;
    if(show_cmd==false)
    {
        if(!alt && !ctrl && !shitf)
        {
            if (manager != nullptr )
            {
                offset_Pos.x = getScrollableHorzSize();
                offset_Pos.y = getScrollableVertSize();
                updateDraw();
                IExtHandler *hook=handlers->get(handler_exec_command);
                if(hook)hook->doHandler(action_update_statusbars_panes,0);
                if(hook)hook->doHandler(action_adjust_horz_scrollbar,offset_Pos.x);
                if(hook)hook->doHandler(action_adjust_vert_scrollbar,offset_Pos.y);
                res=true;
            }
        }
    }
    return res;
}

bool nnViewGlue::handlerPageUpButton(bool shitf, bool ctrl, bool alt)
{
    bool res=false;
    if(show_cmd==false)
    {
        if(!alt && !ctrl && !shitf)
        {
            if (manager != nullptr )
            {
                int p=getPageHeight();
                if(offset_Pos.y>p)
                    offset_Pos.y-=p;
                else
                    offset_Pos.y=0;
                updateDraw();
                IExtHandler *hook=handlers->get(handler_exec_command);
                if(hook)hook->doHandler(action_update_statusbars_panes,0);
                if(hook)hook->doHandler(action_adjust_vert_scrollbar,offset_Pos.y);
                res=true;
            }
        }
        else
            if(!alt && !ctrl && shitf)
            {
                if (manager != nullptr )
                {
                    int p=getPageHeight();
                    bool scroll = false;
                    res = moveSelectArea(-p, 0, scroll);
                    if (selector && selector->getStatus())
                        selector->setError(!res);
                    updateDraw();
                    IExtHandler *hook=handlers->get(handler_exec_command);
                    if(hook)hook->doHandler(action_update_statusbars_panes,0);
                    if(hook)hook->doHandler(action_adjust_vert_scrollbar,offset_Pos.y);
                    res=true;
                }
            }

    }

    return res;
}

bool nnViewGlue::handlerPageDownButton(bool shitf, bool ctrl, bool alt)
{
    bool res=false;
    if(show_cmd==false)
    {
        if(!alt && !ctrl && !shitf)
        {
            if (manager != nullptr )
            {
                int h = getScrollableVertSize();
                int p=getPageHeight();
                offset_Pos.y+=p;
                if(offset_Pos.y>h)
                    offset_Pos.y=h;
                updateDraw();
                IExtHandler *hook=handlers->get(handler_exec_command);
                if(hook)hook->doHandler(action_update_statusbars_panes,0);
                if(hook)hook->doHandler(action_adjust_vert_scrollbar,offset_Pos.y);
                res=true;
            }
        }
        else
            if (!alt && !ctrl && shitf)
            {
                if (manager != nullptr)
                {
                    int p = getPageHeight();
                    bool scroll = false;
                    res = moveSelectArea(p, 0, scroll);
                    if (selector && selector->getStatus())
                        selector->setError(!res);
                    updateDraw();
                    IExtHandler *hook = handlers->get(handler_exec_command);
                    if (hook)hook->doHandler(action_update_statusbars_panes, 0);
                    if (hook)hook->doHandler(action_adjust_vert_scrollbar, offset_Pos.y);
                    res = true;
                }
            }


    }
    return res;
}


bool nnViewGlue::handlerLeftButton(bool shift, bool ctrl, bool alt)
{
    bool res = false;
    IExtHandler *hook=handlers->get(handler_exec_command);
    if(show_cmd==false)
    {
        if(shift && !ctrl && !alt)
        {
            if (status == selected)
            {
                if(manager)
                {
                    resizeSelectArea(-1,0);
                    if(hook)hook->doHandler(action_update_statusbars_panes,0);
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
                    bool scroll=false;
                    res=moveSelectArea(-1,0,scroll);
                    if(selector && selector->getStatus())
                        selector->setError(!res);
                    if(hook)hook->doHandler(action_update_statusbars_panes,0);
                    if(scroll)
                        if(hook)hook->doHandler(action_adjust_horz_scrollbar,offset_Pos.x);
                }
            }
        }
    }
    return res;
}

bool nnViewGlue::handlerRightButton(bool shift, bool ctrl, bool alt)
{
    bool res = false;
    IExtHandler *hook=handlers->get(handler_exec_command);
    if(show_cmd==false)
    {
        if(shift && !ctrl && !alt)
        {
            if (status == selected)
            {
                if(manager)
                {
                    resizeSelectArea(1,0);
                    if(hook)hook->doHandler(action_update_statusbars_panes,0);
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
                    bool scroll=false;
                    res=moveSelectArea(1,0,scroll);
                    if(selector && selector->getStatus())
                        selector->setError(!res);
                    if(hook)hook->doHandler(action_update_statusbars_panes,0);
                    if(scroll)
                        if(hook)hook->doHandler(action_adjust_horz_scrollbar,offset_Pos.x);
                }
            }
        }
    }
    return res;
}

bool nnViewGlue::handlerUpButton(bool shift, bool ctrl, bool alt)
{
    bool res = false;
    IExtHandler *hook=handlers->get(handler_exec_command);
    if(show_cmd==false)
    {
        if(shift && !ctrl && !alt)
        {
            if (status == selected)
            {
                if(manager)
                {
                    resizeSelectArea(0,-1);
                    if(hook)hook->doHandler(action_update_statusbars_panes,0);
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
                    bool scroll=false;
                    res=moveSelectArea(0,-1,scroll);
                    if(selector && selector->getStatus())
                        selector->setError(!res);
                    if(hook)hook->doHandler(action_update_statusbars_panes,0);
                    if(scroll)
                        if(hook)hook->doHandler(action_adjust_vert_scrollbar,offset_Pos.y);
                }
            }
        }
    }
    return res;
}

bool nnViewGlue::handlerDownButton(bool shift, bool ctrl, bool alt)
{
    bool res = false;
    IExtHandler *hook=handlers->get(handler_exec_command);
    if(show_cmd==false)
    {
        if(shift && !ctrl && !alt)
        {
            if (status == selected)
            {
                if(manager)
                {
                    resizeSelectArea(0,1);
                    if(hook)hook->doHandler(action_update_statusbars_panes,0);
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
                    bool scroll=false;
                    res=moveSelectArea(0,1,scroll);
                    if(selector && selector->getStatus())
                        selector->setError(!res);
                    if(hook)hook->doHandler(action_update_statusbars_panes,0);
                    if(scroll)
                        if(hook)hook->doHandler(action_adjust_vert_scrollbar,offset_Pos.y);
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
    if(isStartValid() && isStopValid())
    {
        nnRect vis;
        if(vx!=0)
        {
            int sw = getScrollableHorzSize();
            int w = manager->getWidth()-1;
            if( (vx>0 && select_start.x<w && select_stop.x<w) ||
                    (vx<0 && select_start.x>0 && select_stop.x>0) )
            {
                select_start.x+=vx;
                select_stop.x+=vx;
                getVisibleArea(vis);
                if(vx>0 && select_stop.x>=(vis.stop.x-1) && offset_Pos.x<w)
                {
                    offset_Pos.x+=(vx);
                    if(offset_Pos.x>sw)
                        offset_Pos.x=sw;
                    updateDraw();
                    needScroll=true;
                }
                if(vx<0 && select_start.x<=(vis.start.x-1) && offset_Pos.x>0)
                {
                    offset_Pos.x+=(vx);
                    if(offset_Pos.x<0)
                        offset_Pos.x=0;
                    updateDraw();
                    needScroll=true;
                }
                res=true;
            }
        }
        if(vy!=0)
        {
            int sh = getScrollableVertSize();
            int h = manager->getHeight()-1;
            if( (vy>0 && select_start.y<h && select_stop.y<h) ||
                    (vy<0 && select_start.y>0 && select_stop.y>0) )
            {
                select_start.y+=vy;
                select_stop.y+=vy;
                getVisibleArea(vis);
                if(vy>0 && select_stop.y>=(vis.stop.y-1) && offset_Pos.y<h)
                {
                    offset_Pos.y+=(vy);
                    if(offset_Pos.y>sh)
                        offset_Pos.y=sh;
                    updateDraw();
                    needScroll=true;
                }
                if(vy<0 && select_start.y<=vis.start.y && offset_Pos.y>0)
                {
                    offset_Pos.y+=(vy);
                    if(offset_Pos.y<0)
                        offset_Pos.y=0;
                    updateDraw();
                    needScroll=true;
                }
                res=true;
            }
        }
    }
    return res;
}


bool nnViewGlue::resizeSelectArea(const int vx,const int vy)
{
    bool res=false;
    if(isStartValid() && isStopValid() && manager)
    {
        if(vx!=0)
        {
            int w=manager->getWidth()-1;
            if(vx<0)
            {
                nnPoint diff=select_stop-select_start;
                if(diff.x>0)
                {
                    select_stop.x+=vx;
                    res=true;
                }
            }
            if(vx>0)
            {
                if(select_stop.x<w)
                    select_stop.x+=vx;
                res=true;
            }
        }
        if(vy!=0)
        {
            int h=manager->getHeight()-1;
            if(vy<0)
            {
                nnPoint diff=select_stop-select_start;
                if(diff.y>0)
                {
                    select_stop.y+=vy;
                    res=true;
                }
            }
            if(vy>0)
            {
                if(select_stop.y <h)
                    select_stop.y+=vy;
                res=true;
            }
        }
    }
    return res;
}


bool nnViewGlue::handlerRequestCommand(nnPoint phyPoint,int & command)
{
    bool res=false;
    if(toolview && show_cmd==true)
    {
        if(toolview->getActiveCommander()!=nullptr)
        {
            res=toolview->handlerRequestCommand(phyPoint,command);
        }
    }
    return res;
}


bool nnViewGlue::addExtHandler(handler_exec type,
                               extHandler  _hook,
                               void *unkObj)
{
    bool res=false;
    nnExtHandler  *nh= new nnExtHandler(type,_hook,unkObj);
    res=handlers->add(type,nh);
    return res;
}


bool nnViewGlue::loadImages(const XCHAR * _path)
{
    bool res=false;
    if(toolview)
    {
        res=toolview->loadImages(_path);
    }
    return res;
}


