#include <sstream>
#include "images.h"
#include "n2draw.h"
#include "n2exception.h"
#include "n2caption.h"
#include "n2utils.h"

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


const char *nnCaption::msg[nnCaption::num_item] =
{
    "Iconize current window",
    "Medialize current window",
    "Maximize current window",
    "Close current window",
    "Move current window"
};



nnCaption::nnCaption(IChild *_parent) :
    visible(true), hI(0),
    wI(0),
    curItem(-1),
    parent(_parent),
    font(nullptr),
    status(status_caption_none)
{
    lastPoint.set(0,0);
    for(auto i=0; i<num_item; i++)
        image[i]=nullptr;
}

nnCaption::~nnCaption()
{
    curItem=-1;
}


void nnCaption::setTitle(XCHAR *_name)
{
    TFUNCI();
    if (_name)
    {
        UtoA m(_name);
        title = m.utf8();
    }
    TFUNCO();
}


void nnCaption::setArea(nnPoint & phy)
{
    TFUNCI();
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
    log_verbose("hI=%d - wI=%d",hI,wI);
    TFUNCO();
}

bool nnCaption::draw(bmpImage & bkg, IViewGlue *)
{
    bool res = false;
    TFUNCI();
    if(visible)
    {
        bmpImage caption;
        int start = phyArea.width() - ((wI*num_item) * 3) / 2;
        int end = start - wI;
        //nnLOG2(int ,start,end);
        if (end > 0)
        {
            std::string f,file = "meLa : ";
            if (parent)
                parent->getCurrentFile(f);
            file += f;
            bmpImage *strImage = font->getImage(file.c_str(), 32, 32, 32);
            if(strImage)
            {
                int offX,offY = 2;
                int height1 = strImage->getHeight()+2*offY;
                if (caption.create(end, height1, 32, 255))
                {
                    caption.drawMaskSprite(*strImage,height1, offY, 0, 0, 0);
                    delete strImage;
                    caption.border(0,0,0,0xffffffff);
                    if(image[0])
                    {
                        offY=(image[0]->getHeight()-height1)/2;
                        offX=2*offY;
                    }
                    //nnLOG2(int ,offY,offX);
                    bkg.drawSprite(caption, offX, (bkg.getHeight() - offY-height1));
                    btRect[num_item-1].set(offX, offY, end+offX, height1+offY);
                }
            }
        }
        for (int i = 0; i < num_item-1; i++)
        {
            bkg.drawMaskSprite(*image[i], start, bkg.getHeight()-hI,255,255,255);
            btRect[i].set(start, 0, start + image[i]->getWidth(), image[i]->getHeight());
#if 0
            bkg.frameRect(btRect[i].start.x, bkg.getHeight() - btRect[i].start.y,
                          btRect[i].stop.x, bkg.getHeight() - btRect[i].stop.y, 255, 0, 0, 0xffffffff);
#endif
            start += ((wI * 3) / 2);
        }
        if (curItem != -1)
        {
            drawTips(bkg);
        }
    }
    TFUNCO();
    return res;
}

void nnCaption::addImage(int pos, bmpImage * _image)
{
    TFUNCI();
    if (pos < num_item && pos>=0)
    {
        image[pos] = _image;
    }
    TFUNCO();
}

bool nnCaption::handlerMouseMove(nnPoint &phyPoint, show_status & status, IExtHandler *hook)
{
    TFUNCI();
    bool res = false;
    if(getStatus()==status_caption_none)
    {
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
                log_verbose("status=%d - %s ",status, "show_caption");
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
            res = true;
        }
        else
        {
            if (status == show_caption )
            {
                hide();
                if (hook)
                    hook->doHandler(action_redraw);
                status = show_none;
                phyArea.stop.y = hI/3;
                res = true;
                log_verbose("status=%d - %s ",status, "show_none");
            }
        }
    }
    else if (getStatus() == status_caption_move)
    {
        if (hook)
        {
            nnPoint diff =  phyPoint - lastPoint ;
            if (diff != 0)
            {
                auto *t = new nnAbstractParam<nnPoint>(diff);
                hook->doHandler(action_move_window, t);
                lastPoint = phyPoint-diff;
            }
            res = true;
        }
    }
    TFUNCO();
    return res;
}

int  nnCaption::itemFromPoint(nnPoint phyPoint)
{
    TFUNCI();
    int i,res=-1;
    for (i = 0; i < num_item; i++)
    {
        if (btRect[i].into(phyPoint))
        {
            res=i;
            break;
        }
    }
    TFUNCO();
    return res;
}

bool nnCaption::handlerMouseButtonDown(nnPoint &phyPoint, show_status & /*status*/, IExtHandler * hook)
{
    bool res = false;
    TFUNCI();
    if (getStatus() == status_caption_none)
    {
        if (hook)
        {
            curItem = itemFromPoint(phyPoint);
            switch (curItem)
            {
            case 4:
                lastPoint = phyPoint;
                setStatus(status_caption_move);
                log_verbose("status=%d - %s ",status, "status_caption_move");
                break;
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
            res = true;
        }
    }
    else
    {
        setStatus(status_caption_none);
        log_verbose("status=%d - %s ",status, "status_caption_none");
    }
    TFUNCO();
    return res;
}


bool nnCaption::drawTips(bmpImage & bkg)
{
    TFUNCI();
    bool res = false;
    if (curItem != -1 && font != nullptr)
    {
        std::string m(msg[curItem]);
        res = nnUtils::drawBottomLeftTips(bkg, *font, m);
    }
    TFUNCO();
    return res;
}


