#ifndef N2_CAPTION_HEADER
#define N2_CAPTION_HEADER


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


class nnCaption final
    :public ICaption
{
    typedef enum tag_status_caption
    {
        status_caption_none,
        status_caption_move,
    }status_caption;
    static const int num_item = 5;
    std::string title;
    bool visible;
    int hI;
    int wI;
    int curItem;
    nnRect phyArea;
    nnRect btRect[num_item];
    bmpImage  *image[num_item];
    static const char *msg[num_item];
    IChild *parent;
    IFontManager *font;
    status_caption status;
    nnPoint lastPoint;
public:
    nnCaption(IChild *_parent);
    ~nnCaption();
    void setTitle(XCHAR *_name);
    bool draw(bmpImage & bkg, IViewGlue *);
    void addImage(int pos, bmpImage * image);
    bool handlerMouseMove(nnPoint &phyPoint, show_status & status, IExtHandler *hook);
    bool handlerMouseButtonDown(nnPoint &phyPoint, show_status & status, IExtHandler *hook);
    void setArea(nnPoint & phy);
    inline void setFont(IFontManager *_font) { font = _font; }
    inline void hide(void)
    {
        visible = false;
    }
    inline void show(void)
    {
        visible = true;
    }
private :
    int itemFromPoint(nnPoint phyPoint);
    bool drawTips(bmpImage & bkg);
    inline void setStatus(status_caption  st) { status = st; }
    inline status_caption & getStatus(void) { return status; }
};



#endif
