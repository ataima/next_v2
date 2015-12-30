#ifndef VIEW_CONNECTION_HEADER
#define VIEW_CONNECTION_HEADER

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


#include "n2draw.h"
#include "n2view.h"
#include "images.h"
#include "n2imagemanager.h"



typedef enum tag_mouse_button_def
{
    nn_m_button_unknow = 0,
    nn_m_button_left = 1,
    nn_m_button_right = 2,
    nn_m_button_middle = 4
} nn_mouse_buttons;


class miniXmlNode;

class IViewGlue
{
public:
    virtual nnPoint getCoordPhy(nnPoint & logPoint) = 0;
    virtual nnPoint getCoordPhy(int x,int y) = 0;
    virtual nnPoint getMirrorCoordPhy(int x, int y) = 0;
    virtual nnPoint getCoordLog(nnPoint & phyPoint) = 0;
    virtual bool readConfiguration(miniXmlNode & node) = 0;
    virtual bool selectStart(int xpos, int ypos) = 0;
    virtual bool selectStop(int xpos1, int ypos1) = 0;
    virtual bool selectStart(nnPoint pos) = 0;
    virtual bool selectStop(nnPoint pos1) = 0;
    virtual bool select(nnPoint pos1, nnPoint pos2) = 0;
    virtual bool handlerMouseMove(nn_mouse_buttons buttons, nnPoint phyPoint,nnPoint &start,nnPoint & stop) = 0;
    virtual bool handlerMouseButtonDown(nn_mouse_buttons buttons, nnPoint phyPoint,nnPoint &start,nnPoint & stop) = 0;
    virtual bool handlerMouseButtonUp(nn_mouse_buttons buttons, nnPoint phyPoint,nnPoint &start,nnPoint & stop) = 0;
    virtual bool handlerScrollHorz(int pos) = 0;
    virtual bool handlerScrollVert(int pos) = 0;
    virtual bool handlerEscapeButton(bool shift,bool ctrl,bool alt,nnPoint &start, nnPoint & stop) = 0;
    virtual bool handlerHomeButton(bool shitf,bool ctrl,bool alt,nnPoint &pos )=0;
    virtual bool handlerEndButton(bool shitf,bool ctrl,bool alt,nnPoint &pos)=0;
    virtual bool handlerPageUpButton(bool shitf,bool ctrl,bool alt,nnPoint &pos )=0;
    virtual bool handlerPageDownButton(bool shitf,bool ctrl,bool alt,nnPoint &pos )=0;
    virtual bool handlerLeftButton(bool shitf,bool ctrl,bool alt,nnPoint &start, nnPoint & stop,bool & needScroll)=0;
    virtual bool handlerRightButton(bool shitf,bool ctrl,bool alt,nnPoint &start, nnPoint & stop,bool & needScroll)=0;
    virtual bool handlerUpButton(bool shitf,bool ctrl,bool alt,nnPoint &start, nnPoint & stop,bool & needScroll)=0;
    virtual bool handlerDownButton(bool shitf,bool ctrl,bool alt,nnPoint &start, nnPoint & stop,bool & needScroll)=0;
    virtual bool handlerRequestCommand(nnPoint phyPoint)=0;
    virtual bool unselect() = 0;
    virtual bool getSelectAreaPhy(int & width, int & height) = 0;
    virtual bool getSelectStartPhy(int & x, int & y) = 0;
    virtual bool isStartValid(void) = 0;
    virtual bool isStopValid(void) = 0;
    virtual IManager *getManager(void) = 0;
    virtual void setManager(IManager *mn) = 0;
    virtual bool getSelectArea(nnPoint &start, nnPoint &stop) = 0;
    virtual bmpImage & getDraw(void) = 0;
    virtual bool updateDraw(void) = 0;
    virtual nnPoint getOffsetView(void) = 0;
    virtual nnPoint getMap(void) = 0;
    virtual bool resize(int w, int h) = 0;
    virtual bool needScrollBarHorz(void)=0;
    virtual bool needScrollBarVert(void)=0;
    virtual bool isSelectAreaPhyVisible(nnRect & result,nnPoint & start,nnPoint & stop)=0;
    virtual int getScrollableHorzSize(void)=0;
    virtual int getScrollableVertSize(void)=0;
    virtual int getPageWidth(void)=0;
    virtual int getPageHeight(void)=0;
    virtual nnPoint getPageSize(void)=0;
    virtual ~IViewGlue(){}
};


class nnViewGlue
    :public IViewGlue
{


    typedef enum tag_status_select
    {
        s_unselect,
        start_activate,
        start_resize,
        stop_resize,
        selected
    }status_select;


    nnPoint select_start;
    nnPoint select_stop;
    nnPoint const_Size;
    nnPoint phy_Size;
    nnPoint offset_Pos;


    IManager  *manager;
    IImageManager *images;
    IView     *view;
    status_select status;
    

public:
    nnViewGlue(IManager  *_manager, IImageManager *_images);
    ~nnViewGlue();
    nnPoint getCoordPhy(nnPoint & logPoint);
    nnPoint getCoordPhy(int x, int y);
    nnPoint getMirrorCoordPhy(int x, int y);
    nnPoint getCoordLog(nnPoint & phyPoint);
    bool readConfiguration(miniXmlNode & node);
    bool selectStart(int xpos, int ypos);
    bool selectStop(int xpos1, int ypos1);
    bool selectStart(nnPoint pos);
    bool selectStop(nnPoint pos);
    bool unselect();
    bool getSelectAreaPhy(int & width, int & height);
    bool getSelectStartPhy(int & x, int & y);
    bool handlerMouseMove(nn_mouse_buttons buttons, nnPoint phyPoint,nnPoint &start,nnPoint & stop);
    bool handlerMouseButtonDown(nn_mouse_buttons buttons, nnPoint phyPoint,nnPoint &start,nnPoint & stop);
    bool handlerMouseButtonUp(nn_mouse_buttons buttons, nnPoint phyPoint,nnPoint &start,nnPoint & stop);
    bool handlerScrollHorz(int pos);
    bool handlerScrollVert(int pos);
    bool handlerRequestCommand(nnPoint phyPoint);
    bool handlerEscapeButton(bool shift,bool ctrl,bool alt,nnPoint &start, nnPoint & stop);
    bool handlerHomeButton(bool shitf,bool ctrl,bool alt,nnPoint & pos);
    bool handlerEndButton(bool shitf,bool ctrl,bool alt,nnPoint & pos);
    bool handlerPageUpButton(bool shitf,bool ctrl,bool alt,nnPoint & pos);
    bool handlerPageDownButton(bool shitf,bool ctrl,bool alt,nnPoint & pos);
    bool handlerLeftButton(bool shitf,bool ctrl,bool alt,nnPoint &start, nnPoint & stop,bool & needScroll);
    bool handlerRightButton(bool shitf,bool ctrl,bool alt,nnPoint &start, nnPoint & stop,bool & needScroll);
    bool handlerUpButton(bool shitf,bool ctrl,bool alt,nnPoint &start, nnPoint & stop,bool & needScroll);
    bool handlerDownButton(bool shitf,bool ctrl,bool alt,nnPoint &start, nnPoint & stop,bool & needScroll);
    inline bool isStartValid(void) { return select_start != -1; }
    inline bool isStopValid(void) { return select_stop != -1; }
    inline IManager *getManager(void) { return manager; }
    inline void setManager(IManager *mn) { manager = mn; }
    inline bool select(nnPoint pos1, nnPoint pos2) { return selectStart(pos1) && selectStop(pos2); }
    inline bool getSelectArea(nnPoint &start, nnPoint &stop) { start = select_start; stop = select_stop; return true; }
    bmpImage & getDraw(void);
    bool updateDraw(void);
    inline nnPoint getOffsetView(void) { return nnPoint(offset_Pos.x, offset_Pos.y); }
    inline nnPoint getMap(void) { return nnPoint((phy_Size.x / const_Size.x), (phy_Size.y / const_Size.y)); }
    bool resize(int w, int h);
    bool needScrollBarHorz(void);
    bool needScrollBarVert(void);
    bool isSelectAreaPhyVisible(nnRect & result,nnPoint & start,nnPoint & stop);
    int getScrollableHorzSize(void);
    int getScrollableVertSize(void);
    inline int getPageWidth(void) { return phy_Size.x / const_Size.x;  }
    inline int getPageHeight(void){ return phy_Size.y / const_Size.y; }
    inline nnPoint getPageSize(void) { return phy_Size / const_Size; }

private:
    bool getVisibleArea(nnRect & area);
    bool moveSelectArea(const int vx, const int vy, bool &needScroll);
    bool resizeSelectArea(const int vx,const int vy);
};


#endif




