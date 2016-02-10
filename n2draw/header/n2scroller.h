#ifndef N2SCROLLER_HEADER
#define N2SCROLLER_HEADER


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

#include "n2interfaces.h"


class nnScroller final
:public IScroller
{
    int minPos;
    int maxPos;
    int currentPos;
    bool visible;
    nnRect phyArea;
    nnRect bt1Rect, bt2Rect;
    bmpImage  *image1;
    bmpImage  *image2;
    scrollerMode mode;
    IChild *parent;
public:
    nnScroller(IChild *_parent,int _min, int _max);
    ~nnScroller();
    bool draw(bmpImage & bkg, IViewGlue *);
    void setHorzArea(nnPoint & phy);
    void setVertArea(nnPoint & phy);
    void addImage(int pos, bmpImage * image);
    inline bool setScrollSize(int maximum, int minimum) 
                { maxPos = maximum; minPos = minimum; return true; }
    inline int getPosition(void) 
                { return currentPos; }
    inline void update(int _pos) 
                { currentPos = _pos; }
    bool handlerMouseMove( nnPoint &phyPoint, show_status & status, IExtHandler *hook) ;
    bool handlerMouseButtonDown( nnPoint &phyPoint, show_status & status, IExtHandler *hook) ;
    inline void hide(void) 
                { visible = false; }
    inline void show(void) 
                { visible = true; }
private:
    void drawBar(bmpImage & bkg, nnPoint & start, nnPoint & stop);
};



#endif
