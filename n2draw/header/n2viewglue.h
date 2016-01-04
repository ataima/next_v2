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


#include "images.h"
#include "n2imagemanager.h"
#include "n2interfaces.h"





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
    IToolView *toolview;
    status_select status;
    IExtHandlerList * handlers;
    bool show_cmd;
    

public:
    nnViewGlue(IManager  *_manager, IImageManager *_images);
    ~nnViewGlue();
    nnPoint getCoordPhy(nnPoint & logPoint);
    nnPoint getCoordPhy(int x, int y);
    nnPoint getMirrorCoordPhy(int x, int y);
    nnPoint getCoordLog(nnPoint & phyPoint);
    bool readConfiguration(IXmlNode * node);
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
    bool handlerRequestCommand(nnPoint phyPoint, int &command);
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
    bool addExtHandler(handler_exec type, extHandler  _hook, void *unkObj);
    bool loadImages(const XCHAR *_path);
private:
    bool getVisibleArea(nnRect & area);
    bool moveSelectArea(const int vx, const int vy, bool &needScroll);
    bool resizeSelectArea(const int vx,const int vy);
};


#endif




