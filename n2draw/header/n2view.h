#ifndef N2VIEW_HEADER
#define N2VIEW_HEADER


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


#include "images.h"

typedef struct tag_cur_draw_point
{
    size_t off_x;
    size_t off_y;
    size_t width;
    size_t height;
    size_t rows;
    size_t colunms;
    size_t max_phy_width;
    size_t max_phy_height;
    size_t const_phy_x;
    size_t const_phy_y;

}drawContext;

// interface
class IView
{
public:
    virtual bool readConfiguration(miniXmlNode *node) = 0;
    virtual bool draw(IManager *manager, void * context) = 0;
    virtual bool createMainBitmap(size_t w, size_t h) = 0;
    virtual bmpImage & getMainBitmap(void) = 0;
};



class nnTextView
    :public IView
{
    bmpImage image;
public:
    bool draw(IManager * manager, void * context);
    bool readConfiguration(miniXmlNode *node) { return false; }
    virtual bool createMainBitmap(size_t w, size_t h) { return true; }
    virtual bmpImage & getMainBitmap(void) { return image; }
};


class nnView
    :public IView
{
    int n_thread;
    bmpImage page;
    size_t p_width;
    size_t p_height;
public:
    nnView();
    ~nnView();
    bool draw(IManager * manager, void * context);
    bool readConfiguration(miniXmlNode *node);
    bool createMainBitmap(size_t w, size_t h);
    inline bmpImage & getMainBitmap(void) { return page; }
private:
    bool drawObj(InnObj * obj, size_t & x, size_t & y, drawContext* ctx);
    bool drawBkg( size_t & x, size_t & y, drawContext* ctx);
};



// no conf phy view 
class phyViewConfigurationException
    :public std::runtime_error
{
    std::wstring node;
public:
    explicit phyViewConfigurationException(const wchar_t *_node) throw()
        :runtime_error("phyGlueConfigurationException"), node(_node) {}

};
#endif
