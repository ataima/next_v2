
#include "images.h"
#include "n2draw.h"
#include "n2selector.h"
#include "n2exception.h"


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

nnSelector::nnSelector(void)
{
    hide();
    error = false;
    toAlpha = "ABCDEFGHILMNOPQRSTUVXYZ";
}


bool nnSelector::translateY( int p, std::string & out )
{
    bool res = false;
    out.clear();
    int w = toAlpha.size();
    int maxW = w;
    if (p >= w)
    {
        while (maxW < p)
        {
            maxW *= w;
        }
        while (p >= w)
        {
            int t = p / maxW;
            if (t > 0)
            {
                out += toAlpha[t - 1];
            }
            p %= maxW;
            maxW /= w;
        }
        out += toAlpha[p];
    }
    else
    {
        out += toAlpha[p];
    }
    res = (out.size() > 0);
    return res;
}

void nnSelector::draw(bmpImage & image, 
   const nnPoint &sel_start, const nnPoint &sel_stop, IViewGlue * glue)
{
    if (visible)
    {
        nnPoint start, stop,size;
        size = glue->getConstPhy();
        start = glue->getCoordPhy( sel_start);
        stop  = glue->getCoordPhy( sel_stop);        
        stop += size;
        unsigned int height=image.getHeight();
        start.y = height - start.y;
        stop.y = height - stop.y;
        start-=2;
        stop += 2;
        if (error)
        {
            image.frameRect(start.x, start.y, stop.x, stop.y, 255, 0, 0, 0xcccccccc);
            start += 4;
            stop -= 4;
            image.frameRect(start.x,start.y, stop.x, stop.y, 255, 0, 0, 0x66666666);
        }
        else
        {
            image.frameRect(start.x, start.y, stop.x, stop.y, 0, 128, 0, 0xcccccccc);
            start += 4;
            stop -= 4;
            image.frameRect(start.x,start.y, stop.x, stop.y, 0, 0, 128, 0x66666666);
        }
        if (font)
        {
            nnPoint diff = sel_stop - sel_start;
            if (diff.y >= 2 || diff.x >= 2)
            {
                bmpImage * strImage;
                std::string conv;
                char buff[128];
                if (translateY(sel_start.x, conv))
                {
                    sprintf(buff, "%s:%d", conv.c_str(), sel_start.x);
                    strImage = font->getImage(buff, 16, 16, 224);
                    image.drawMaskSprite(*strImage, start.x + 2, start.y - 14, 0, 0, 0);
                    delete strImage;
                }
                if (translateY(sel_stop.x, conv))
                {
                    sprintf(buff, "%s:%d", conv.c_str(), sel_stop.x);
                    strImage = font->getImage(buff, 16, 224, 16);
                    image.drawMaskSprite(*strImage, stop.x - (8 * strlen(buff)) - 2, stop.y + 4, 0, 0, 0);
                    delete strImage;
                }
            }
        }
    }
}

