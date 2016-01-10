#ifndef N2SELECTOR_HEADER
#define N2SELECTOR_HEADER


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

class nnSelector
    : public ISelector
{
    bool visible;
    bool error;
    IFontManager * font;
    std::string toAlpha;
public:
    nnSelector();
    void draw(bmpImage & image, 
             const nnPoint &sel_start, const nnPoint &sel_stop, IViewGlue * glue);
    inline void hide(void) { visible = false; }
    inline void show(void) { visible = true; }
    inline bool getStatus(void) { return visible; }
    inline void setError(bool st) { error = st; }
    inline void setFont(IFontManager *_font) { font = _font; }
private:
    bool translateY(int p, std::string & out);
};



#endif 

