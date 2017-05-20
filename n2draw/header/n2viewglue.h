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
   
    nnPoint const_Size;
    nnPoint phy_Size;
    nnPoint offset_Pos;

    IChild *parent;
    IView     *view;
    IToolView *maintool;
    IToolView *compotool;
    ISelector *selector;
    IScroller *vscroller;
    IScroller *hscroller;
    ICaption *caption;
    ICapture *capture;
    IFontManager *curFont;
    IComponent* focus;
    show_status show_cmd;
public:
    nnViewGlue(IChild *_parent);
    ~nnViewGlue();
    bool setPhyView(int w, int h);
    nnPoint getCoordPhy(const nnPoint & logPoint);
    nnPoint getMirrorCoordPhy(int height, int x, int y);
    nnPoint getCoordLog(const nnPoint & phyPoint);
    inline  nnPoint getConstPhy(void) { return const_Size; }
    bool readConfiguration(IXmlNode * node);
    bool getSelectAreaPhy(int & width, int & height);
    bool handlerMouseMove(nn_mouse_buttons buttons, nnPoint & phyPoint);
    bool handlerMouseButtonDown(nn_mouse_buttons buttons, nnPoint & phyPoint);
    bool handlerMouseButtonUp(nn_mouse_buttons buttons, nnPoint & phyPoint);
    bool handlerScrollHorz(int pos);
    bool handlerScrollVert(int pos);
    bool handlerEscapeButton(bool shift,bool ctrl,bool alt);
    bool handlerHomeButton(bool shitf, bool ctrl, bool alt);
    bool handlerEndButton(bool shitf,bool ctrl,bool alt);
    bool handlerPageUpButton(bool shitf,bool ctrl,bool alt);
    bool handlerPageDownButton(bool shitf,bool ctrl,bool alt);
    bool handlerPageRightButton(bool shitf, bool ctrl, bool alt);
    bool handlerPageLeftButton(bool shitf, bool ctrl, bool alt);
    bool handlerLeftButton(bool shitf,bool ctrl,bool alt);
    bool handlerRightButton(bool shitf, bool ctrl, bool alt);
    bool handlerUpButton(bool shitf, bool ctrl, bool alt);
    bool handlerDownButton(bool shitf, bool ctrl, bool alt);
    bool handlerCancelButton(bool shitf, bool ctrl, bool alt);
    bool select(nnPoint pos1, nnPoint pos2);
    bool getSelectArea(nnPoint &start, nnPoint &stop);
    bool unselect();
    bmpImage & getDraw(void);
    bool updateDraw(void);
    inline nnPoint getOffsetView(void) { return offset_Pos; }
    nnPoint getMap(void);
    bool resize(int w, int h);
    inline int getPageWidth(void) { return phy_Size.x / const_Size.x;  }
    inline int getPageHeight(void){ return phy_Size.y / const_Size.y; }
    bool loadImages(STRING &_path);
    inline  bool createDraw(void) { return view->createMainBitmap(phy_Size); }
    bool Capture(int command,unsigned int image);
    inline IFontManager *getCurrentFont(void) { return curFont; }
private:
    bool getVisibleArea(nnRect & area);
    bool moveSelectArea(const int vx, const int vy, bool &needScroll);
    bool needScrollBarHorz(void);
    bool needScrollBarVert(void);
    int  getScrollableHorzSize(void);
    int  getScrollableVertSize(void);
    IFontManager * getFontFromName(const char *fname);
 };


#endif




