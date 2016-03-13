#include "images.h"
#include "n2draw.h"
#include "n2exception.h"
#include "n2utils.h"
#include "n2moreinfo.h"

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
nnMoreInfo::nnMoreInfo(IChild *_parent)
    :parent(_parent)
{
    lastPos.set(-1,-1);
    info.clear();
    color=0xffffff;
}

bool nnMoreInfo::handlerMouseButtonDown(nnPoint &logPoint, int _color, IExtHandler *hook)
{
    bool  res = false;
    lastPos = logPoint;
    color = _color;
    if (parent)
    {
        IManager *manager = parent->getManager();
        if (manager)
        {
            InnObj *obj = manager->getObj(lastPos.x, lastPos.y);
            if (obj != nullptr)
            {
                std::string  str = obj->toString();
                info.clear();
                info.push_back(str);
                show();
                if (hook)
                    hook->doHandler(action_redraw);
                res = true;
            }
            else
            {
                lastPos.set(-1, -1);
                hide();
            }
        }
    }
    return res;
}

void nnMoreInfo::draw(bmpImage & image, IViewGlue * glue)
{
    if (visible)
    {
        if (glue)
        {           
            nnUtils::drawBottomRightTips(image, *glue->getCurrentFont(), info.front());
        }
    }
}
