#include "images.h"
#include "n2draw.h"
#include "n2exception.h"
#include "n2caption.h"

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


const char *nnCaption::msg[nnCaption::num_item] = {
    "Iconize current window",
    "Medialize current window",
    "Maximize current window",
    "Close current window"
};



nnCaption::nnCaption(IChild *_parent):
    parent(_parent),font(nullptr),
    visible(true),hI(0),
    wI(0),curItem(-1)
{
}

nnCaption::~nnCaption()
{
    curItem=-1;
}


void nnCaption::setTitle(XCHAR *_name)
{
    if (_name)
    {
        UtoA m(_name);
        title = m.utf8();
    }
}


void nnCaption::setArea(nnPoint & phy)
{       
        hI = 0;
        wI = 0;
        for (int i = 0; i < num_item; i++)
        {
            if (image[i])
            {
                int h=image[i]->getHeight();
                if (h > hI)
                    hI = h;
                int w = image[i]->getWidth();
                if (w > wI)
                    wI = w;
            }
        }
        phyArea.set(0, 0, phy.x, hI/3);
}

bool nnCaption::draw(bmpImage & bkg, IViewGlue *)
{
    bool res = false;
    bmpImage caption;
    int start = phyArea.width() - ((wI*num_item) * 3) / 2;
    int end = start - wI;
    if (end > 0)
    {
        if (caption.create(end, phyArea.height() / 2, 32, 255))
        {
            std::string f,file = "meLa : ";
            if (parent)
                parent->getCurrentFile(f);
            file += f;
            bmpImage *strImage = font->getImage(file.c_str(), 32, 32, 32);
            int offY = 0;
            int height1 = caption.getHeight();
            int height2 = strImage->getHeight();
            if (height1 > height2)
            {
                offY = (height1 - height2) / 2;
            }
            caption.drawMaskSprite(*strImage,height1, offY, 0, 0, 0);
            delete strImage;
            caption.frameRect(0, 0, end - 1, height1 - 1,0,0,0,0xffffffff);
            bkg.drawSprite(caption, 0, bkg.getHeight() - phyArea.height() / 2);
        }
    }
    for (int i = 0; i < num_item; i++)
    {
        bkg.drawMaskSprite(*image[i], start, bkg.getHeight()-hI,255,255,255);
        btRect[i].start.x = start;
        btRect[i].start.y = 0;
        btRect[i].stop.x = start + image[i]->getWidth();
        btRect[i].stop.y = image[i]->getHeight();
#if 0
        bkg.frameRect(btRect[i].start.x, bkg.getHeight() - btRect[i].start.y-5,
            btRect[i].stop.x, bkg.getHeight() - btRect[i].stop.y-5, 255, 0, 0, 0xffffffff);
#endif
        start += ((wI * 3) / 2);
    }
    if (curItem != -1)
    {
        drawTips(bkg);
    }
    return res;
}

void nnCaption::addImage(int pos, bmpImage * _image)
{
    if (pos < num_item && pos>=0)
    {
        image[pos] = _image;
    }
}

bool nnCaption::handlerMouseMove(nnPoint phyPoint, show_status & status, IExtHandler *hook)
{
    bool res = false;
    if (phyArea.into(phyPoint))
    {
        int t = itemFromPoint(phyPoint);

        if (status == show_none)
        {
            curItem = t;
            show();
            status = show_caption;
            if (hook)
                hook->doHandler(action_redraw);
            phyArea.stop.y=hI;
        }
        else
        {
            if (t != curItem)
            {
                curItem = t;
                if (hook)
                    hook->doHandler(action_redraw);
            }
        }
    }
    else
    {
        hide();
        if (status == show_caption )
        {
            if (hook)
                hook->doHandler(action_redraw);
            status = show_none;
            phyArea.stop.y = hI/3;
        }
    }
    return res;
}

int  nnCaption::itemFromPoint(nnPoint phyPoint)
{
    int i;
    for (i = 0; i < num_item; i++)
    {
        if (btRect[i].into(phyPoint))
        {
            return i;
        }
    }
    return -1;
}

bool nnCaption::handlerMouseButtonDown(nnPoint phyPoint, IViewGlue * )
{
    bool res = false;
    int i;
    if (parent)
    {
        curItem = itemFromPoint(phyPoint);
        IExtHandler *hook = parent->getHandler();
        if (hook)
        {
            switch (curItem)
            {
            case 0:
                hook->doHandler(action_iconize_windows);
                break;
            case 1:
                hook->doHandler(action_medialize_windows);
                break;
            case 2:
                hook->doHandler(action_maximize_windows);
                break;
            case 3:
                hook->doHandler(action_close_windows);
                break;
            }
        }
        res = true;            
    }
    return res;
}



bool nnCaption::drawTips(bmpImage & bkg)
{
   
    bool res = false;
    if (curItem != -1 && font != nullptr)
    {
        int len = strlen(msg[curItem]);
        nnPoint sizeStr(font->getFontWidth()* len, font->getFontHeight());
        const int offsetX = 20;
        const int offsetY = 4;
        bmpImage rectbkg;
        res = rectbkg.create(sizeStr.x + 2 * offsetX, sizeStr.y + 2 * offsetY, 32, 255);
        if (res)
        {
            bmpImage * strImage = font->getImage(msg[curItem], 0, 0, 255);
            res = rectbkg.drawMaskSprite(*strImage, offsetX, offsetY, 0, 0, 0);
            delete strImage;
            if (res)
            {
                rectbkg.frameRect(0, 0, rectbkg.getWidth() - 1, rectbkg.getHeight() - 1, 0, 0, 0, 0xffffffff);
                int posX = (bkg.getWidth() - rectbkg.getWidth() - offsetX);
                res = bkg.drawSprite(rectbkg, posX, offsetY);
            }
        }
    }

    return res;
}
