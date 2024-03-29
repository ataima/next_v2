#include "images.h"
#include "n2draw.h"
#include "n2exception.h"
#include "n2capturepos.h"
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

nnCapturePos::nnCapturePos(IChild *_parent,unsigned char _Rmask,
                           unsigned char _Gmask, unsigned char _Bmask)
    :parent(_parent),
     Rmask(_Rmask),Gmask(_Gmask),Bmask(_Bmask),
     command(0),off_image(0),curImage(nullptr)
{
    endLogPos.set(-1, -1);
    startLogPos.set(-1, -1);
}

nnCapturePos::~nnCapturePos()
{
    command = 0;
    off_image = 0;
    curImage = nullptr;
    Rmask = Gmask = Bmask = 0;
    endLogPos.set(-1, -1);
    startLogPos.set(-1, -1);
}

void nnCapturePos::setCommand(int c, unsigned int image,nnPoint & _startLogPos)
{
    TFUNCI();
    command = c;
    off_image = image;
    startLogPos = _startLogPos;
    if (parent)
    {
        IImageManager *images = parent->getImage();
        curImage = images->getImage(off_image);
    }
    TFUNCO();
}

void nnCapturePos::draw(bmpImage & image, IViewGlue * glue)
{
    TFUNCI();
    if (glue)
    {
        nnPoint phy = glue->getCoordPhy(endLogPos);
        image.drawMaskSprite(*curImage, phy.x, image.getHeight()-curImage->getHeight() - phy.y,
                             Rmask,Gmask,Bmask
                            );
    }
    drawTips(image);
    TFUNCO();
}

bool nnCapturePos::handlerMouseMove(nnPoint &phyPoint, show_status & /*status*/, IExtHandler *hook)
{
    TFUNCI();
    bool res = false;
    if (parent)
    {
        IViewGlue *view = parent->getView();
        nnPoint current_pos = view->getCoordLog(phyPoint);
        if (endLogPos != current_pos)
        {
            endLogPos = current_pos;
            if (hook)
                hook->doHandler(action_redraw);
            res = true;
        }
    }
    TFUNCO();
    return res;
}

bool nnCapturePos::handlerMouseButtonDown(nnPoint &phyPoint, show_status & status, IExtHandler *hook)
{
    TFUNCI();
    bool res = false;
    if (parent)
    {
        IViewGlue *view = parent->getView();
        nnPoint current_pos = view->getCoordLog(phyPoint);
        if (endLogPos != current_pos)
        {
            endLogPos = current_pos;
        }
        if (endLogPos.isValid())
        {
            status = show_none;
            if (hook)
            {
                auto *par1 = new nnAbstractParam<nnPoint>(startLogPos);
                auto *par2 = new nnAbstractParam<nnPoint>(endLogPos);
                auto *par3 = new nnAbstractParam<int>(command);
                auto pars = new nnAbstractParamList();
                pars->add(par1);
                pars->add(par2);
                pars->add(par3);
                hook->doHandler(action_select_position, pars);
                hook->doHandler(action_redraw);
            }
            command = -1;
            endLogPos.set(-1, -1);
            curImage = nullptr;
        }
    }
    TFUNCO();
    return res;
}



bool nnCapturePos::drawTips(bmpImage & bkg)
{
    TFUNCI();
    bool res = false;
    if (endLogPos.isValid() &&  font != nullptr)
    {
        std::stringstream s;
        s << "ROW:" << endLogPos.x << "   -   COL:" << endLogPos.y;
        std::string l= s.str();
        res = nnUtils::drawBottomLeftTips(bkg, *font, l );
    }
    TFUNCO();
    return res;
}
