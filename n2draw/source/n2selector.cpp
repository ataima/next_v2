
#include "images.h"
#include "n2draw.h"
#include "n2moreinfo.h"
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

nnSelector::nnSelector(IChild *_parent):
    parent(_parent)
{
    hide();
    error = false;
    toAlpha = "ABCDEFGHILMNOPQRSTUVXYZ";
    select_start.set(-1);
    select_stop.set(-1);
    info = new nnMoreInfo(_parent);
}


nnSelector::~nnSelector()
{
    hide();
    error = false;
    select_start.set(-1);
    select_stop.set(-1);
    if (info) {
        delete info;
        info = nullptr;
    }
}

bool nnSelector::translateY( int p, std::string & out )
{
    bool res = false;
    out.clear();
    int w = toAlpha.size();
    int maxW = w;
    if (p >= w) {
        while (maxW < p) {
            maxW *= w;
        }
        while (p >= w) {
            int t = p / maxW;
            if (t > 0) {
                out += toAlpha[t - 1];
            }
            p %= maxW;
            maxW /= w;
        }
        out += toAlpha[p];
    } else {
        out += toAlpha[p];
    }
    res = (out.size() > 0);
    return res;
}

void nnSelector::draw(bmpImage & image,IViewGlue * glue)
{
    if (visible) {
        nnPoint start, stop,size;
        size = glue->getConstPhy();
        start = glue->getCoordPhy( select_start);
        stop  = glue->getCoordPhy( select_stop);
        stop += size;
        unsigned int height=image.getHeight();
        start.y = height - start.y;
        stop.y = height - stop.y;
        start-=2;
        stop += 2;
        if (error) {
            image.frameRect(start.x, start.y, stop.x, stop.y, 255, 0, 0, 0xcccccccc);
            start += 4;
            stop -= 4;
            image.frameRect(start.x,start.y, stop.x, stop.y, 255, 0, 0, 0x66666666);
        } else {
            image.frameRect(start.x, start.y, stop.x, stop.y, 0, 128, 0, 0xcccccccc);
            start += 4;
            stop -= 4;
            image.frameRect(start.x,start.y, stop.x, stop.y, 0, 0, 128, 0x66666666);
        }
        if (font) {
            nnPoint diff = select_stop - select_start;
            if (diff.y >= 2 || diff.x >= 2) {
                bmpImage * strImage;
                std::string conv;
                char buff[128];
                if (translateY(select_start.x, conv)) {
                    sprintf(buff, "%s:%d", conv.c_str(), select_start.y);
                    strImage = font->getImage(buff, 16, 16, 224);
                    image.drawMaskSprite(*strImage, start.x + 2, start.y - 14, 0, 0, 0);
                    delete strImage;
                }
                if (translateY(select_stop.x, conv)) {
                    sprintf(buff, "%s:%d", conv.c_str(), select_stop.y);
                    strImage = font->getImage(buff, 16, 224, 16);
                    image.drawMaskSprite(*strImage, stop.x - (8 * strlen(buff)) - 2, stop.y + 4, 0, 0, 0);
                    delete strImage;
                }
            }
        }
    }
    if (!error && info)
        info->draw(image, glue);
}

bool nnSelector::handlerMouseMove(nnPoint & logPoint)
{
    bool res = false;
    if (status == start_activate)
        status = start_resize;
    if (status == start_resize) {
        if (parent) {
            IExtHandler *hook = parent->getHandler();
            if (logPoint != select_stop) {

                IManager *manager = parent->getManager();
                if (manager) {
                    nnPoint maxStop = manager->getSchema();
                    maxStop -= 1;
                    error = false;
                    if (logPoint.x > maxStop.x) {
                        error = true;
                        logPoint.x = maxStop.x;
                    }
                    if (logPoint.y > maxStop.y) {
                        error = true;
                        logPoint.y = maxStop.y;
                    }
                    if (logPoint.x < select_start.x || logPoint.y < select_start.y) {
                        select_stop = select_start;
                        select_start = logPoint;
                    } else {
                        select_stop = logPoint;
                    }
                    if (hook) {
                        nnRect v(select_start, select_stop);
                        auto *p = new nnAbstractParam<nnRect>(v);
                        hook->doHandler(action_update_selected_panes, p);
                        hook->doHandler(action_redraw);
                    }
                    res = true;
                }
            }
        }
    }
    return res;
}

bool nnSelector::handlerMouseButtonDown(nnPoint &logPoint, show_status & /*s_status*/)
{
    bool res = false;
    if (info)
        info->hide();
    if (status == s_unselect || status == selected)
        status = start_activate;
    if (parent) {
        IExtHandler *hook = parent->getHandler();
        IManager *manager = parent->getManager();
        if (manager ) {
            nnPoint maxStop = manager->getSchema();
            maxStop -= 1;
            error = false;
            if (logPoint.x > maxStop.x) {
                error = true;
                logPoint.x = maxStop.x;
            }
            if (logPoint.y > maxStop.y) {
                error = true;
                logPoint.y = maxStop.y;
            }
            selectStart(logPoint);
            select_stop = select_start;
            show();
            if (hook) {
                nnRect v(select_start, select_stop);
                auto *p = new nnAbstractParam<nnRect>(v);
                hook->doHandler(action_update_selected_panes, p);
                hook->doHandler(action_redraw);

            }
            res = true;
        }
    }
    return res;
}

//TestviewGlue.cpp : T3
bool nnSelector::selectStart(nnPoint pos)
{
    bool res = false;
    if (parent) {
        IManager  * manager = parent->getManager();
        if (manager) {
            int log_height = manager->getHeight(); //logic coord
            int log_width = manager->getWidth(); //logic coord
            if (pos.x < log_width && pos.y < log_height) {
                select_start = pos;
                res = true;
            }
        }
    }
    return res;
}
//TestviewGlue.cpp : T3
bool nnSelector::selectStop(nnPoint pos)
{
    bool res = false;
    if (parent != nullptr) {
        IManager  * manager = parent->getManager();
        if (manager) {
            int log_height = manager->getHeight(); //logic coord
            int log_width = manager->getWidth(); //logic coord
            if (pos.x < log_width && pos.y < log_height) {
                select_stop = pos;
                res = true;
            }
        }
    }
    return res;
}


//TestviewGlue.cpp : T3
bool nnSelector::unselect()
{
    select_start.x = -1;
    select_start.y = -1;
    select_stop.x = -1;
    select_stop.y = -1;
    status = s_unselect;
    hide();
    if (parent) {
        IExtHandler *hook = parent->getHandler();
        if (hook) {
            nnRect v(select_start, select_stop);
            auto *p = new nnAbstractParam<nnRect>(v);
            hook->doHandler(action_update_selected_panes, p);
            hook->doHandler(action_redraw);
        }
    }
    return true;
}


int nnSelector::isSelected(void)
{
    int res = 0;
    if (visible) {
        if (isStartValid() && isStopValid() && !error) {
            if (select_start != select_stop) {
                nnPoint diff = select_stop - select_start;
                res = diff.maxElem();
            } else {
                res = 1;
            }
        }
    }
    return res;
}



bool nnSelector::isSelectedComponent(void)
{
    bool res = false;
    int sel = isSelected();
    if (sel == 1 && parent) {
        IManager *manager = parent->getManager();
        if (manager) {
            InnObj * obj = manager->getObj(select_start.x, select_start.y);
            if (obj) {
                res = obj->isComponent();
            }
        }
    }
    return res;
}


bool nnSelector::resizeSelectArea(const int vx, const int vy)
{
    bool res = false;
    if (parent) {
        if (status == selected) {
            IManager  * manager = parent->getManager();
            if (isStartValid() && isStopValid()) {
                if (manager) {
                    if (vx != 0) {
                        int w = manager->getWidth() - 1;
                        if (vx < 0) {
                            nnPoint diff = select_stop - select_start;
                            if (diff.x>0) {
                                select_stop.x += vx;
                                res = true;
                            }
                        }
                        if (vx > 0) {
                            if (select_stop.x < w)
                                select_stop.x += vx;
                            res = true;
                        }
                    }
                    if (vy != 0) {
                        int h = manager->getHeight() - 1;
                        if (vy < 0) {
                            nnPoint diff = select_stop - select_start;
                            if (diff.y>0) {
                                select_stop.y += vy;
                                res = true;
                            }
                        }
                        if (vy > 0) {
                            if (select_stop.y < h)
                                select_stop.y += vy;
                            res = true;
                        }
                    }
                }
            }
            if (res) {
                IExtHandler *hook = parent->getHandler();
                if (hook) {
                    nnRect v(select_start, select_stop);
                    auto *p = new nnAbstractParam<nnRect>(v);
                    hook->doHandler(action_update_selected_panes, p);
                    hook->doHandler(action_redraw);
                }
                if (!error &&  info && status == selected && select_start == select_stop) {
                    info->handlerMouseButtonDown(select_start, 0xffff00, hook);
                }
            }
        }
    }
    return res;
}


bool nnSelector::handlerMouseButtonUp(nn_mouse_buttons buttons, nnPoint & logPoint)
{
    bool res = true;
    if (parent && buttons==nn_mouse_buttons::nn_m_button_left) {
        IExtHandler *hook = parent->getHandler();
        if (status == start_resize) {
            if (parent) {
                IManager *manager = parent->getManager();
                if (manager) {
                    status = stop_resize;
                    nnPoint maxStop = manager->getSchema();
                    maxStop -= 1;
                    error = false;
                    if (logPoint.x > maxStop.x) {
                        error = true;
                        logPoint.x = maxStop.x;
                    }
                    if (logPoint.y > maxStop.y) {
                        error = true;
                        logPoint.y = maxStop.y;
                    }
                    if (logPoint != select_stop) {
                        select_stop = logPoint;
                    }
                    if (hook) {
                        nnRect v(select_start, select_stop);
                        auto *p = new nnAbstractParam<nnRect>(v);
                        hook->doHandler(action_update_selected_panes, p);
                        hook->doHandler(action_redraw);
                    }
                    status = selected;
                }
            }
        } else if (status == start_activate) {
            if (logPoint != select_stop) {
                select_stop = logPoint;
            }
            if (hook) {
                nnRect v(select_start, select_stop);
                auto *p = new nnAbstractParam<nnRect>(v);
                hook->doHandler(action_update_selected_panes, p);
                hook->doHandler(action_redraw);
            }
            status = selected;
        }
        if (info && status == selected && select_start == select_stop) {
            info->handlerMouseButtonDown(select_start, 0xffff00, hook);
        }
    }
    return res;
}

bool nnSelector::select(nnPoint pos1, nnPoint pos2)
{
    bool res = selectStart(pos1) && selectStop(pos2);
    if (parent) {
        IManager *manager = parent->getManager();
        if (manager) {
            nnPoint maxStop = manager->getSchema();
            error = false;
            if (select_start.x > maxStop.x) {
                error = true;
                select_start.x = maxStop.x;
            }
            if (select_start.y > maxStop.y) {
                error = true;
                select_start.y = maxStop.y;
            }
            if (select_stop.x > maxStop.x) {
                error = true;
                select_stop.x = maxStop.x;
            }
            if (select_stop.y > maxStop.y) {
                error = true;
                select_stop.y = maxStop.y;
            }
            if (!error && status == selected && select_start == select_stop && info) {
                IExtHandler *hook = parent->getHandler();
                info->hide();
                info->handlerMouseButtonDown(select_start, 0xffff00, hook);
            }
        }
    }
    return res;
}
