#ifndef N2POINT
#define N2POINT
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


#include <sstream>

typedef struct tag_nn_point
{
    int x;
    int y;
    tag_nn_point(int _x = 0, int _y = 0) :x(_x), y(_y) {}
    inline void operator = (const struct tag_nn_point b)
    {
        x = b.x;
        y = b.y;
    }
    inline bool operator == (const struct tag_nn_point b)
    {
        return (x == b.x) || (y == b.y);
    }
    inline bool operator != (const struct tag_nn_point b)
    {
        return (x != b.x) || (y != b.y);
    }
    inline bool operator != (const int v)
    {
        return (x != v) || (y != v);
    }
    inline bool  isValid(void)
    {
        return (x != -1 && y != -1);
    }
    inline void set(int _x, int _y)
    {
        x = _x;
        y = _y;
    }
    inline void set(int _v)
    {
        x = _v;
        y = _v;
    }

    tag_nn_point intersect(const tag_nn_point & p)
    {

        tag_nn_point res;
        res.x = x < p.x ? x : p.x;
        res.y = y < p.y ? y : p.y;
        return res;
    }
    friend tag_nn_point operator+(const tag_nn_point & a, const tag_nn_point & b)
    {
        tag_nn_point res;
        res.x=a.x+b.x;
        res.y=a.y+b.y;
        return res;
    }
    friend tag_nn_point operator-(const tag_nn_point & a, const tag_nn_point & b)
    {
        tag_nn_point res;
        res.x=a.x-b.x;
        res.y=a.y-b.y;
        return res;
    }
    friend tag_nn_point operator/(const tag_nn_point & a, const tag_nn_point & b)
    {
        tag_nn_point res;
        res.x=a.x/b.x;
        res.y=a.y/b.y;
        return res;
    }
    friend tag_nn_point operator*(const tag_nn_point & a, const tag_nn_point & b)
    {
        tag_nn_point res;
        res.x=a.x*b.x;
        res.y=a.y*b.y;
        return res;
    }

    friend bool operator<(const tag_nn_point & a, const tag_nn_point & b)
    {
        if (a.x < b.x)
            if (a.y < b.y)
                return true;
        return false;
    }
    friend bool operator>(const tag_nn_point & a, const tag_nn_point & b)
    {
        if (a.x > b.x)
            if (a.y > b.y)
                return true;
        return false;
    }
    friend bool operator<=(const tag_nn_point & a, const tag_nn_point & b)
    {
        if (a.x <= b.x)
            if (a.y <= b.y)
                return true;
        return false;
    }
    friend bool operator>=(const tag_nn_point & a, const tag_nn_point & b)
    {
        if (a.x >= b.x)
            if (a.y >= b.y)
                return true;
        return false;
    }
    inline void operator += (const tag_nn_point & a)
    {
        x += a.x;
        y += a.y;
    }
    inline void operator -= (const tag_nn_point & a)
    {
        x -= a.x;
        y -= a.y;
    }
    inline void operator += (const int v)
    {
        x += v;
        y += v;
    }
    inline void operator -= (const int v)
    {
        x -= v;
        y -= v;
    }
    inline void adjust(const int _x, const int _y)
    {
        x += _x;
        y += _y;
    }
    friend  inline  std::ostream & operator<<(std::ostream & os, const tag_nn_point & point)
    {
        os << "POINT[x:" << point.x << "-y:" << point.y << "]";
        return os;
    }
} nnPoint;


typedef struct tag_nn_rect
{
    nnPoint start;
    nnPoint stop;
    tag_nn_rect(int top=-1, int left=- 1, int down= -1, int right= -1)
        :start(top, left), stop(down, right) {}
    tag_nn_rect(const nnPoint& p1, const nnPoint & p2)
        :start(p1), stop(p2) {}
    inline void operator = (const struct tag_nn_rect b)
    {
        start = b.start;
        stop = b.stop;
    }
    inline bool operator != (const struct tag_nn_rect b)
    {
        return (start != b.start) || (stop != b.stop);
    }
    inline bool operator != (const int v)
    {
        return (start != v) || (stop != v);
    }
    inline bool  isValid(void)
    {
        return (start.isValid()) && (stop.isValid());
    }
    inline void set(int _left, int _top, int _right, int _down)
    {
        start.set(_left,_top);
        stop.set( _right,_down);
    }
    inline void set(const tag_nn_point & p1, const tag_nn_point & p2)
    {
        start=p1;
        stop=p2;
    }
    inline int width(void)
    {
        if(start.x<stop.x)
            return stop.x-start.x;
        else
            return start.x-stop.x;
    }
    inline int height(void)
    {
        if(start.y<stop.y)
            return stop.y-start.y;
        else
            return start.y-stop.y;
    }

    tag_nn_rect intersect(const tag_nn_rect & b)
    {

        tag_nn_rect res;
        res.start = start.intersect(b.start);
        res.stop = stop.intersect(b.stop);
        return res;
    }

    tag_nn_rect in(const tag_nn_rect & b)
    {
        tag_nn_rect res;
        int left,top,right,bottom;
        left=b.start.x;
        right=b.stop.x;
        top=b.start.y;
        bottom=b.stop.y;
        //
        if( left>stop.x ||
                right< start.x ||
                top>stop.y ||
                bottom<start.y)
        {
            top=bottom=right=left=-1;
        }
        else
        {
            /*
            if(left<start.x && right> start.x)
                left=start.x;
            if(left<stop.x && right>stop.x)
                right=stop.y;
            if(top<start.y && bottom> start.y)
                top=start.y;
            if(top<stop.y && bottom> stop.y)
                bottom=stop.y;
                */
        }
        //
        res.set(left,top,right,bottom);
        return res;
    }
    inline bool into(const nnPoint & b)
    {
        bool res=false;
        if(start.x < b.x && stop.x > b.x)
        {
            if(start.y < b.y && stop.y > b.y)
            {
                res=true;
            }
        }
        return res;
    }
    friend inline std::ostream & operator<<(std::ostream & os, const tag_nn_rect & rect)
    {
        os << "RECT:START["<<rect.start<<"]- STOP["<<rect.stop<<"]";
        return os;
    }

} nnRect;


#endif // N2POINT

