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



nnScroller::nnScroller(int _min, int _max):
    minPos(_min),maxPos(_max),visible(false),
    currentPos(0),image1(nullptr),image2(nullptr),
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




bool nnScroller::draw(bmpImage & bkg, IViewGlue * glue)
{
    bool res = false;
    int height = bkg.getHeight();
    if (visible)
    {
        if (image1)
        {
            if (mode == scrollerMode::mode_scroller_horz)
            {
                res = bkg.drawMaskSprite(*image1, phyArea.start.x, height-phyArea.start.y-image1->getHeight(), 0, 0, 0);
                bt1Rect.set(phyArea.start.x, phyArea.start.y, phyArea.start.x + image1->getWidth(), phyArea.start.y + image1->getHeight());
            }
            else
            if (mode == scrollerMode::mode_scroller_vert)
            {
                res = bkg.drawMaskSprite(*image1, phyArea.start.x, height - phyArea.start.y-image2->getHeight(), 0, 0, 0);
                bt1Rect.set(phyArea.start.x, phyArea.start.y, phyArea.start.x + image1->getWidth(), phyArea.start.y + image1->getHeight());
            }
        }
        if ( image2)
        {
            if (mode == scrollerMode::mode_scroller_horz)
            {
                res = bkg.drawMaskSprite(*image2, phyArea.stop.x - image2->getWidth(), height - phyArea.start.y - image2->getHeight(), 0, 0, 0);
                bt2Rect.set(phyArea.stop.x - image2->getWidth(), phyArea.start.y, phyArea.stop.x, phyArea.start.y + image1->getHeight());
            }
            else
            if (mode == scrollerMode::mode_scroller_vert)
            {
                res = bkg.drawMaskSprite(*image2, phyArea.start.x , height-phyArea.stop.y, 0, 0, 0);
                bt2Rect.set(phyArea.start.x , phyArea.stop.y - image2->getHeight(), phyArea.start.x+image2->getWidth(), phyArea.stop.y);
            }
        }
    }
    return res;
}

bool nnScroller::handlerMouseMove( nnPoint phyPoint, show_status & status, IExtHandler *hook)
{
    bool res = false;
    if (phyArea.into(phyPoint))
    {
        if (status == show_none)
        {
            show();
            if (mode == scrollerMode::mode_scroller_horz)
            {
                status = show_scroller_horz;
                if (hook)
                    hook->doHandler(action_redraw);
            }
            else
                if (mode == scrollerMode::mode_scroller_vert)
            { 
                status = show_scroller_vert;
                if (hook)
                    hook->doHandler(action_redraw);
            }

        }
    }
    else
    {
        hide();
        if (status == show_scroller_horz && 
            mode == scrollerMode::mode_scroller_horz)
        {
            if (hook)
                hook->doHandler(action_redraw);
            status = show_none;
        }
        else
        if (status == show_scroller_vert && 
            mode == scrollerMode::mode_scroller_vert)
        {
            if (hook)
                hook->doHandler(action_redraw);
            status = show_none;
        }
    }
    return res;
}

bool nnScroller::handlerMouseButtonDown( nnPoint phyPoint, IViewGlue * glue)
{
    bool res = false;
    if (bt1Rect.into(phyPoint))
    {
        if (glue)
        {
            if (mode == scrollerMode::mode_scroller_horz)
            {
                res=glue->handlerPageLeftButton(false, false, false);
            }
            else
                if (mode == scrollerMode::mode_scroller_vert)
                {
                    res = glue->handlerPageUpButton(false, false, false);
                }
        }
    }
    else
    if (bt2Rect.into(phyPoint))
    {
        if (glue)
        {
            if (mode == scrollerMode::mode_scroller_horz)
            {
                res = glue->handlerPageRightButton(false, false, false);
            }
            else
                if (mode == scrollerMode::mode_scroller_vert)
                {
                    res = glue->handlerPageDownButton(false, false, false);
                }
        }
    }
    return res;
}


void nnScroller::setHorzArea(int w, int h)
{
    int start_y = h - 48;
    if (w < 480)
        w = 480;
    int start_x = (w / 5)*2;
    int stop_x = (w / 5) * 3;
    phyArea.set(start_x, start_y, stop_x,h);
    mode = scrollerMode::mode_scroller_horz;
}


void nnScroller::setVertArea(int w, int h)
{
    int start_x = w - 48;
    if (h < 480)
        h = 480;
    int start_y =( h / 5)*2;
    int stop_y = (h / 5) * 3;
    phyArea.set(start_x, start_y, w, stop_y);
    mode = scrollerMode::mode_scroller_vert;
}