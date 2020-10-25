#ifndef N2SELECTOR_HEADER
#define N2SELECTOR_HEADER


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

#include "n2interfaces.h"

typedef enum tag_status_select
{
    s_unselect,
    start_activate,
    start_resize,
    stop_resize,
    selected
} status_select;

inline std::ostream & operator<<(std::ostream & os, const status_select & st)
{
    switch (st)
    {
    case s_unselect:
        os << "s_unselect(0)";
        break;
    case start_activate:
        os << "start_activate(1)";
        break;
    case start_resize:
        os << "start_resize(2)";
        break;
    case stop_resize:
        os << "stop_resize(3)";
        break;
    case selected:
        os << "selected(4)";
        break;
    }
    return os;
}



class nnSelector
    : public ISelector
{

    bool visible;
    bool error;
    IFontManager * font;
    std::string toAlpha;
    status_select status;
    IChild *parent;
    nnPoint select_start;
    nnPoint select_stop;
    IMoreInfo *info;
public:
    nnSelector(IChild * _parent);
    ~nnSelector();
    void draw(bmpImage & image, IViewGlue * glue);
    inline void hide(void)
    {
        visible = false;
    }
    inline void show(void)
    {
        visible = true;
    }
    inline bool getStatus(void)
    {
        return visible;
    }
    inline void setError(bool st)
    {
        error = st;
    }
    inline void setFont(IFontManager *_font)
    {
        font = _font;
    }
    bool handlerMouseMove(nnPoint & logPoint);
    bool handlerMouseButtonDown(nnPoint & logPoint,show_status & status);
    bool handlerMouseButtonUp(nn_mouse_buttons buttons, nnPoint  & logPoint);
    bool unselect();
    inline bool isSelectedValid(void)
    {
        return (isStartValid() && isStopValid() );
    }
    inline bool isStartValid(void)
    {
        return select_start != -1;
    }
    inline bool isStopValid(void)
    {
        return select_stop != -1;
    }
    inline nnPoint & getSelectStart(void)
    {
        return select_start;
    }
    inline nnPoint & getSelectStop(void)
    {
        return select_stop;
    }
    int isSelected(void);
    bool isSelectedComponent(void);
    bool select(nnPoint pos1, nnPoint pos2);
    inline bool getSelectArea(nnPoint &start, nnPoint &stop)
    {
        start = select_start;
        stop = select_stop;
        return true;
    }
    bool resizeSelectArea(const int vx, const int vy);
private:
    bool translateY(int p, std::string & out);
    bool selectStart(nnPoint pos);
    bool selectStop(nnPoint pos);
};



#endif

