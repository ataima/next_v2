#ifndef N2COMMANDER_HEADER
#define N2COMMANDER_HEADER


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


class nnCommander
    :public ICommander
{
private:
    listCommandItem items;
    IImageManager *images;
    commandItem * curItem;
    IFontManager *font;
    unsigned int bmpHeight;
    unsigned int bmpWidth;
    nnPoint space;
    nnPoint maxExt;
public:
    nnCommander();
    ~nnCommander();
    bool readConfiguration(IXmlNode *node) ;
    bool checkRequestCommand( nnPoint & pos,int & command);
    bool handlerMouseMove( nnPoint & pos,IExtHandler *hook);
    bool loadImages(STRING & path);
    bool draw(bmpImage & bkg, nnPoint & pos, IViewGlue * glue);
    inline void setFont(IFontManager *_font)
    {
        font = _font;
    }
private :
    bool drawTips(bmpImage & bkg, nnPoint & pos, IViewGlue * glue);
    void getQuadrant(nnPoint & pos,nnPoint & res);
};



#endif // N2COMMANDER

