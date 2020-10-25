#include "n2utils.h"
#include "n2utoatou.h"

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
bool nnUtils::drawBottomRightTips(bmpImage & bkg, IFontManager & font, std::string  & info)
{

    bool res = false;
    size_t len = info.size();
    nnPoint sizeStr(font.getFontWidth()* len, font.getFontHeight());
    const int offsetX = 20;
    const int offsetY = 4;
    bmpImage rectbkg;
    res = rectbkg.create(bkg.getWidth(), sizeStr.y + 2 * offsetY, 32, 255);
    if (res)
    {
        bmpImage * strImage = font.getImage(info.c_str(), 0, 0, 255);
        res = rectbkg.drawMaskSprite(*strImage, offsetX, offsetY, 0, 0, 0);
        delete strImage;
        if (res)
        {
            rectbkg.frameRect(0, 0, rectbkg.getWidth() - 1, rectbkg.getHeight() - 1, 0, 0, 0, 0xffffffff);
            res = bkg.drawSprite(rectbkg, 0, 0);
        }
    }
    return res;
}

bool nnUtils::drawBottomLeftTips(bmpImage & bkg, IFontManager & font, std::string  & info)
{

    bool res = false;
    size_t len = info.size();
    nnPoint sizeStr(font.getFontWidth()* len, font.getFontHeight());
    const int offsetX = 20;
    const int offsetY = 4;
    bmpImage rectbkg;
    int t_width = bkg.getWidth();
    res = rectbkg.create(t_width, sizeStr.y + 2 * offsetY, 32, 255);
    if (res)
    {
        bmpImage * strImage = font.getImage(info.c_str(), 0, 0, 255);
        res = rectbkg.drawMaskSprite(*strImage, t_width-2*offsetX- sizeStr.x, offsetY, 0, 0, 0);
        delete strImage;
        if (res)
        {
            rectbkg.frameRect(0, 0, rectbkg.getWidth() - 1, rectbkg.getHeight() - 1, 0, 0, 0, 0xffffffff);
            res = bkg.drawSprite(rectbkg, 0, 0);
        }
    }
    return res;
}