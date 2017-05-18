#include "images.h"
#include "n2draw.h"
#include "n2exception.h"
#include "n2scroller.h"

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



nnScroller::nnScroller(IChild *_parent, int _min, int _max):
    parent(_parent),
    minPos(_min),maxPos(_max),
    currentPos(0),visible(false),
    mode(scrollerMode::mode_scroller_unknow)
{
    phyArea.set(0, 0, 0, 0);
}

nnScroller::~nnScroller()
{
    minPos = maxPos = currentPos = -1;
    visible = false;
    phyArea.set(0, 0, 0, 0);
}





bool nnScroller::handlerMouseMove( nnPoint &phyPoint, show_status & status, IExtHandler *hook)
{
    bool res = false;
    if (phyArea.into(phyPoint)) {
        res = true;
        if (status == show_none) {
            show();
            if (mode == scrollerMode::mode_scroller_horz) {
                status = show_scroller_horz;
                if (hook)
                    hook->doHandler(action_redraw);
            } else if (mode == scrollerMode::mode_scroller_vert) {
                status = show_scroller_vert;
                if (hook)
                    hook->doHandler(action_redraw);
            }

        }
    } else {
        hide();
        if (status == show_scroller_horz &&
            mode == scrollerMode::mode_scroller_horz) {
            if (hook)
                hook->doHandler(action_redraw);
            status = show_none;
            res = true;
        } else if (status == show_scroller_vert &&
                   mode == scrollerMode::mode_scroller_vert) {
            if (hook)
                hook->doHandler(action_redraw);
            status = show_none;
            res = true;
        }
    }
    return res;
}

bool nnScroller::handlerMouseButtonDown( nnPoint &phyPoint, show_status & /*status*/, IExtHandler */*hook*/)
{
    bool res = false;
    if (parent) {
        if (bt1Rect.into(phyPoint)) {
            if (mode == scrollerMode::mode_scroller_horz) {
                res = parent->handlerPageLeftButton(false, false, false);
            } else if (mode == scrollerMode::mode_scroller_vert) {
                res = parent->handlerPageUpButton(false, false, false);
            }
        } else if (bt2Rect.into(phyPoint)) {
            if (mode == scrollerMode::mode_scroller_horz) {
                res = parent->handlerPageRightButton(false, false, false);
            } else if (mode == scrollerMode::mode_scroller_vert) {
                res = parent->handlerPageDownButton(false, false, false);
            }
        }
    }
    return res;
}




void nnScroller::setHorzArea(nnPoint & phy)
{
    if (image1!=nullptr && image2 != nullptr) {
        int w = phy.x;
        if (w < 480)
            w = 480;
        int hI1 = image1->getHeight();
        int hI2 = image2->getHeight();
        if (hI2 > hI1)
            hI1 = hI2;
        int start_y = phy.y - hI1 - 4;
        int start_x = (w / 5) * 2;
        int stop_x = (w / 5) * 3;
        start_x -= image1->getWidth();
        stop_x += image2->getWidth();
        phyArea.set(start_x, start_y, stop_x, phy.y);
        mode = scrollerMode::mode_scroller_horz;
    }
}


void nnScroller::setVertArea(nnPoint & phy)
{
    if(image1!=nullptr && image2!=nullptr) {
        int h = phy.y;
        if (h < 480)
            h = 480;
        int wI1 = image1->getWidth();
        int wI2 = image2->getWidth();
        if (wI2 > wI1)
            wI1 = wI2;
        int start_x = phy.x - wI1 - 4;
        int start_y = (h / 5) * 2;
        int stop_y = (h / 5) * 3;
        start_y -= image1->getHeight();
        stop_y += image2->getHeight();
        phyArea.set(start_x, start_y, phy.x, stop_y);
        mode = scrollerMode::mode_scroller_vert;
    }
}

void nnScroller::addImage(int pos, bmpImage * image)
{
    if (pos == 1)
        image1 = image;
    else if (pos == 2)
        image2 = image;

}


bool nnScroller::draw(bmpImage & bkg, IViewGlue *)
{
    bool res = false;
    int height = bkg.getHeight();
    nnPoint start, stop;
    if (image1 != nullptr && image2 != nullptr) {
        int height_1 = image1->getHeight();
        int height_2 = image2->getHeight();
        int width_1 = image1->getWidth();
        int width_2 = image2->getWidth();
        if (visible) {
            if (image1) {
                if (mode == scrollerMode::mode_scroller_horz) {
                    res = bkg.drawMaskSprite(*image1, phyArea.start.x, height - phyArea.start.y - image1->getHeight(), 0, 0, 0);
                    bt1Rect.set(phyArea.start.x, phyArea.start.y, phyArea.start.x + width_1, phyArea.start.y + height_1);
                    start.x = phyArea.start.x + width_1 + width_1 / 4;
                    start.y = (height - phyArea.start.y) - height_1 / 2 - height_1 / 6;
                } else if (mode == scrollerMode::mode_scroller_vert) {
                    res = bkg.drawMaskSprite(*image1, phyArea.start.x, height - phyArea.start.y - height_1, 0, 0, 0);
                    bt1Rect.set(phyArea.start.x, phyArea.start.y, phyArea.start.x + width_1, phyArea.start.y + height_1);
                    start.x = phyArea.start.x + width_1 / 2 - width_1 / 5;
                    start.y = height - phyArea.start.y - height_1-height_1/4;
                }
            }
            if (image2) {
                if (mode == scrollerMode::mode_scroller_horz) {
                    res = bkg.drawMaskSprite(*image2, phyArea.stop.x - width_2, height - phyArea.start.y - height_2, 0, 0, 0);
                    bt2Rect.set(phyArea.stop.x - width_2, phyArea.start.y, phyArea.stop.x, phyArea.start.y + height_2);
                    stop.x = phyArea.stop.x - width_2 - width_2 / 4;
                    stop.y = (height - phyArea.start.y) - height_2 / 2 + height_2 / 6;
                } else if (mode == scrollerMode::mode_scroller_vert) {
                    res = bkg.drawMaskSprite(*image2, phyArea.start.x, height - phyArea.stop.y, 0, 0, 0);
                    bt2Rect.set(phyArea.start.x, phyArea.stop.y - height_2, phyArea.stop.x, phyArea.stop.y);
                    stop.x = phyArea.start.x + width_1 / 2 + width_1 / 5;
                    stop.y = height-phyArea.stop.y  +height_2 +  height_2/4;
                }
            }
            if (start != stop) {
                drawBar(bkg, start, stop);
            }
            // todo ... I will to create a class showsTips to show info . Must be a class on separate thread
            // to implement code as : info = new nnTips("prova" ,100) ; info.run();
            // this class auto manage the process the show and auto clean
#if 0
            bkg.frameRect(bt1Rect.start.x, height - bt1Rect.start.y, bt1Rect.stop.x, height - bt1Rect.stop.y, 255, 0, 0, 0xffffffff);
            bkg.frameRect(bt2Rect.start.x, height - bt2Rect.start.y, bt2Rect.stop.x, height - bt1Rect.stop.y, 255, 0, 0, 0xffffffff);
            bkg.frameRect(phyArea.start.x, height - phyArea.start.y, phyArea.stop.x, height - phyArea.stop.y, 0, 0, 255, 0xffffffff);
#endif
        }
    }
    return res;
}


void nnScroller::drawBar(bmpImage & bkg, nnPoint & start, nnPoint & stop)
{
    bmpImage pmp,curs;
    if (stop.y < start.y) {
        auto t = stop.y;
        stop.y = start.y;
        start.y = t;
    }
    int pos,width = stop.x - start.x;
    int height = stop.y - start.y;
    pmp.create(width,height, 32, 255);
    pmp.border(128, 128, 128, 0xffffffff);
    if (mode == scrollerMode::mode_scroller_horz) {
        pos = ((width*currentPos) / (maxPos - minPos));
        if (pos) {
            curs.create(pos-1, height - 6, 32, 192);
            pmp.drawSprite(curs, 3, 3);
        }
    } else {
        pos =((height*currentPos) / (maxPos - minPos));
        if (pos) {
            curs.create(width - 6, pos-6, 32, 192);
            pmp.drawSprite(curs, 3, 3);
        }
    }
    bkg.drawSprite(pmp, start.x,  start.y);

}
