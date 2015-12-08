#ifndef VIEW_CONNECTION_HEADER
#define VIEW_CONNECTION_HEADER


#include "n2draw.h"
#include "n2view.h"
#include "images.h"



typedef enum tag_mouse_button_def
{
    nn_m_button_unknow = 0,
    nn_m_button_left = 1,
    nn_m_button_right = 2,
    nn_m_button_middle = 4
} nn_mouse_buttons;


class miniXmlNode;

class IViewGlue
{
public:
    virtual nnPoint getCoordPhy(nnPoint & logPoint) = 0;
    virtual nnPoint getCoordLog(nnPoint & phyPoint) = 0;
    virtual bool readConfiguration(miniXmlNode & node) = 0;
    virtual bool selectStart(size_t xpos, size_t ypos) = 0;
    virtual bool selectStop(size_t xpos1, size_t ypos1) = 0;
    virtual bool selectStart(nnPoint pos) = 0;
    virtual bool selectStop(nnPoint pos1) = 0;
    virtual bool select(nnPoint pos1, nnPoint pos2) = 0;
    virtual bool handlerMouseMove(nn_mouse_buttons buttons, nnPoint phyPoint) = 0;
    virtual bool handlerMouseButtonDown(nn_mouse_buttons buttons, nnPoint phyPoint) = 0;
    virtual bool handlerMouseButtonUp(nn_mouse_buttons buttons, nnPoint phyPoint) = 0;
    virtual bool handlerScrollHorz(size_t pos) = 0;
    virtual bool handlerScrollVert(size_t pos) = 0;
    virtual bool handlerEscapeButton(void) = 0;
    virtual bool unselect() = 0;
    virtual bool getSelectAreaPhy(size_t & width, size_t & height) = 0;
    virtual bool getSelectStartPhy(size_t & x, size_t & y) = 0;
    virtual bool isStartValid(void) = 0;
    virtual bool isStopValid(void) = 0;
    virtual IManager *getManager(void) = 0;
    virtual void setManager(IManager *mn) = 0;
    virtual bool getSelectArea(nnPoint &start, nnPoint &stop) = 0;
    virtual bool createDrawPage(size_t w, size_t h) = 0;
    virtual bmpImage & getDraw(void) = 0;
    virtual bool updateDraw(void) = 0;
};


class nnViewGlue
    :public IViewGlue
{


    typedef enum tag_status_select
    {
        s_unselect,
        start_activate,
        start_resize,
        stop_resize,
        selected

    }status_select;


    nnPoint select_start;
    nnPoint select_stop;
    size_t const_x;
    size_t const_y;
    size_t width;
    size_t height;
    size_t offset_x;
    size_t offset_y;

    IManager  *manager;
    IView     *view;
    status_select status;
    bmpImage page;

public:
    nnViewGlue(IManager  *_manager);
    ~nnViewGlue();
    nnPoint getCoordPhy(nnPoint & logPoint);
    nnPoint getCoordLog(nnPoint & phyPoint);
    bool readConfiguration(miniXmlNode & node);
    bool selectStart(size_t xpos, size_t ypos);
    bool selectStop(size_t xpos1, size_t ypos1);
    bool selectStart(nnPoint pos);
    bool selectStop(nnPoint pos);
    bool unselect();
    bool getSelectAreaPhy(size_t & width, size_t & height);
    bool getSelectStartPhy(size_t & x, size_t & y);
    bool handlerMouseMove(nn_mouse_buttons buttons, nnPoint phyPoint);
    bool handlerMouseButtonDown(nn_mouse_buttons buttons, nnPoint phyPoint);
    bool handlerMouseButtonUp(nn_mouse_buttons buttons, nnPoint phyPoint);
    bool handlerScrollHorz(size_t pos);
    bool handlerScrollVert(size_t pos);
    bool handlerEscapeButton(void);
    inline bool isStartValid(void) { return select_start != -1; }
    inline bool isStopValid(void) { return select_stop != -1; }
    inline IManager *getManager(void) { return manager; }
    inline void setManager(IManager *mn) { manager = mn; }
    inline bool select(nnPoint pos1, nnPoint pos2) { return selectStart(pos1) && selectStop(pos2); }
    inline bool getSelectArea(nnPoint &start, nnPoint &stop) { start = select_start; stop = select_stop; return true; }
    bool createDrawPage(size_t w, size_t h);
    inline bmpImage & getDraw(void) { return page; }
    bool updateDraw(void);
};


// no conf phy view 
class phyGlueConfigurationException
    :public std::runtime_error
{
    std::wstring node;
public:
    explicit phyGlueConfigurationException(const wchar_t *_node) throw()
        :runtime_error("phyGlueConfigurationException"),node(_node) {}

};

#endif




