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
    virtual nnPoint getCoordPhy(size_t x,size_t y) = 0;
    virtual nnPoint getMirrorCoordPhy(size_t x, size_t y) = 0;
    virtual nnPoint getCoordLog(nnPoint & phyPoint) = 0;
    virtual bool readConfiguration(miniXmlNode & node) = 0;
    virtual bool selectStart(size_t xpos, size_t ypos) = 0;
    virtual bool selectStop(size_t xpos1, size_t ypos1) = 0;
    virtual bool selectStart(nnPoint pos) = 0;
    virtual bool selectStop(nnPoint pos1) = 0;
    virtual bool select(nnPoint pos1, nnPoint pos2) = 0;
    virtual bool handlerMouseMove(nn_mouse_buttons buttons, nnPoint phyPoint,nnPoint &start,nnPoint & stop) = 0;
    virtual bool handlerMouseButtonDown(nn_mouse_buttons buttons, nnPoint phyPoint,nnPoint &start,nnPoint & stop) = 0;
    virtual bool handlerMouseButtonUp(nn_mouse_buttons buttons, nnPoint phyPoint,nnPoint &start,nnPoint & stop) = 0;
    virtual bool handlerScrollHorz(size_t pos) = 0;
    virtual bool handlerScrollVert(size_t pos) = 0;
    virtual bool handlerEscapeButton(bool shift,bool ctrl,bool alt,nnPoint &start, nnPoint & stop) = 0;
    virtual bool handlerHomeButton(bool shitf,bool ctrl,bool alt,nnPoint &pos )=0;
    virtual bool handlerEndButton(bool shitf,bool ctrl,bool alt,nnPoint &pos)=0;
    virtual bool handlerPageUpButton(bool shitf,bool ctrl,bool alt,nnPoint &pos )=0;
    virtual bool handlerPageDownButton(bool shitf,bool ctrl,bool alt,nnPoint &pos )=0;
    virtual bool handlerLeftButton(bool shitf,bool ctrl,bool alt,nnPoint &start, nnPoint & stop,bool & needScroll)=0;
    virtual bool handlerRightButton(bool shitf,bool ctrl,bool alt,nnPoint &start, nnPoint & stop,bool & needScroll)=0;
    virtual bool handlerUpButton(bool shitf,bool ctrl,bool alt,nnPoint &start, nnPoint & stop,bool & needScroll)=0;
    virtual bool handlerDownButton(bool shitf,bool ctrl,bool alt,nnPoint &start, nnPoint & stop,bool & needScroll)=0;
    virtual bool unselect() = 0;
    virtual bool getSelectAreaPhy(size_t & width, size_t & height) = 0;
    virtual bool getSelectStartPhy(size_t & x, size_t & y) = 0;
    virtual bool isStartValid(void) = 0;
    virtual bool isStopValid(void) = 0;
    virtual IManager *getManager(void) = 0;
    virtual void setManager(IManager *mn) = 0;
    virtual bool getSelectArea(nnPoint &start, nnPoint &stop) = 0;
    virtual bmpImage & getDraw(void) = 0;
    virtual bool updateDraw(void) = 0;
    virtual nnPoint getOffsetView(void) = 0;
    virtual nnPoint getMap(void) = 0;
    virtual bool resize(size_t w, size_t h) = 0;
    virtual bool needScrollBarHorz(void)=0;
    virtual bool needScrollBarVert(void)=0;
    virtual bool isSelectAreaPhyVisible(nnRect & result,nnPoint & start,nnPoint & stop)=0;
    virtual size_t getScrollableHorzSize(void)=0;
    virtual size_t getScrollableVertSize(void)=0;
    virtual size_t getPageWidth(void)=0;
    virtual size_t getPageHeight(void)=0;
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
    size_t const_x;
    size_t const_y;
    size_t p_width;
    size_t p_height;
    size_t offset_x;
    size_t offset_y;

    IManager  *manager;
    IImageManager *images;
    IView     *view;
    status_select status;
    

public:
    nnViewGlue(IManager  *_manager, IImageManager *_images);
    ~nnViewGlue();
    nnPoint getCoordPhy(nnPoint & logPoint);
    nnPoint getCoordPhy(size_t x, size_t y);
    nnPoint getMirrorCoordPhy(size_t x, size_t y);
    nnPoint getCoordLog(nnPoint & phyPoint);
    bool readConfiguration(miniXmlNode & node);
    bool selectStart(size_t xpos, size_t ypos);
    bool selectStop(size_t xpos1, size_t ypos1);
    bool selectStart(nnPoint pos);
    bool selectStop(nnPoint pos);
    bool unselect();
    bool getSelectAreaPhy(size_t & width, size_t & height);
    bool getSelectStartPhy(size_t & x, size_t & y);
    bool handlerMouseMove(nn_mouse_buttons buttons, nnPoint phyPoint,nnPoint &start,nnPoint & stop);
    bool handlerMouseButtonDown(nn_mouse_buttons buttons, nnPoint phyPoint,nnPoint &start,nnPoint & stop);
    bool handlerMouseButtonUp(nn_mouse_buttons buttons, nnPoint phyPoint,nnPoint &start,nnPoint & stop);
    bool handlerScrollHorz(size_t pos);
    bool handlerScrollVert(size_t pos);
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
    inline nnPoint getOffsetView(void) { return nnPoint(offset_x, offset_y); }
    inline nnPoint getMap(void) { return nnPoint((p_width / const_x), (p_height / const_y)); }
    bool resize(size_t w, size_t h);
    bool needScrollBarHorz(void);
    bool needScrollBarVert(void);
    bool isSelectAreaPhyVisible(nnRect & result,nnPoint & start,nnPoint & stop);
    size_t getScrollableHorzSize(void);
    size_t getScrollableVertSize(void);
    inline size_t getPageWidth(void) { return p_width / const_x;  }
    inline size_t getPageHeight(void){ return p_height / const_y; }

private:
    bool getVisibleArea(nnRect & area);
    bool moveSelectArea(const int vx, const int vy, bool &needScroll);
    bool resizeSelectArea(const int vx,const int vy);
};


#endif




