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

nnCapturePos::nnCapturePos(IChild *_parent)
    :parent(_parent),command(0)
    ,off_image(0),curImage(nullptr)
{
    logPos.set(-1, -1);
}

nnCapturePos::~nnCapturePos()
{
    command = 0;
    off_image = 0;
    curImage = nullptr;
    logPos.set(-1, -1);
}

void nnCapturePos::setCommand(int c, unsigned int image)
{ 
    command = c; 
    off_image = image; 
    if (parent)
    {
        IImageManager *images = parent->getImage();
        curImage = images->getImage(off_image);
    }
}

void nnCapturePos::draw(bmpImage & image, IViewGlue * glue)
{
    if (glue)
    {
        nnPoint phy = glue->getCoordPhy(logPos);
        image.drawSprite(*curImage, phy.x, image.getHeight()-curImage->getHeight() - phy.y);
    }
    drawTips(image);
}

bool nnCapturePos::handlerMouseMove(nnPoint &phyPoint, show_status & status, IExtHandler *hook)
{
    bool res = false;
    if (parent)
    {
        IViewGlue *view = parent->getView();
        nnPoint current_pos = view->getCoordLog(phyPoint);
        if (logPos != current_pos)
        {
            logPos = current_pos;
            if (hook)
                hook->doHandler(action_redraw);
            res = true;
        }
    }
    return res;
}

bool nnCapturePos::handlerMouseButtonDown(nnPoint &phyPoint, show_status & status, IExtHandler *hook)
{
    bool res = false;
    if (parent)
    {
        IViewGlue *view = parent->getView();
        nnPoint current_pos = view->getCoordLog(phyPoint);
        if (logPos != current_pos)
        {
            logPos = current_pos;
        }
        if (logPos.isValid())
        {
            status = show_none;
            if (hook)
            {
                auto *par = new nnAbstractParam<nnPoint>(logPos);
                auto *par1 = new nnAbstractParam<int>(command);
                auto pars = new nnAbstractParamList();
                pars->add(par);
                pars->add(par1);
                hook->doHandler(action_select_position, pars);
                hook->doHandler(action_redraw);
            }
            command = -1;
            logPos.set(-1, -1);
            curImage = nullptr;
        }
    }
    return res;
}



bool nnCapturePos::drawTips(bmpImage & bkg)
{
    bool res = false;
    if (logPos.isValid() &&  font != nullptr)
    {
        std::stringstream s;
        s << "ROW:" << logPos.x << "   -   COL:" << logPos.y;
        res = nnUtils::drawBottomLeftTips(bkg, *font, s.str());
    }
    return res;
}
