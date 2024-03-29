#ifndef N2_CAPTURE_HEADER
#define N2_CAPTURE_HEADER

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

class nnCapturePos
    : public ICapture
{
    IChild *parent;
    unsigned char Rmask;
    unsigned char Gmask;
    unsigned char Bmask;
    int command;
    unsigned int off_image;
    bmpImage * curImage;
    nnPoint startLogPos;
    nnPoint endLogPos;
    IFontManager * font;
public:
    nnCapturePos(IChild *_parent, unsigned char _Rmask,
                 unsigned char _Gmask, unsigned char _Bmask);
    ~nnCapturePos();
    void setCommand(int c, unsigned int image, nnPoint & _startLogPos);
    void draw(bmpImage & image, IViewGlue * glue);
    virtual bool handlerMouseMove(nnPoint &phyPoint,
                                  show_status & status, IExtHandler *hook);
    virtual bool handlerMouseButtonDown(nnPoint &phyPoint,
                                        show_status & status, IExtHandler *hook);
    inline void setFont(IFontManager *_font)
    {
        font = _font;
    }
    TOSTRING(nnCapturePos);
private:
    bool drawTips(bmpImage & bkg);
};
#endif // N2_CAPTURE_HEADER

