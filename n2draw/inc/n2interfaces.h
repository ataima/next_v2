#ifndef N2INTERFACES
#define N2INTERFACES

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

#include <string>
#include <sstream>
#include <vector>
#include <list>
#include <map>
#include "n2point.h"
#include "n2logiface.h"


#ifndef _MSC_VER
#pragma pack(0)
#endif
//////////////////////////////////////////////////////

#define  BUFFLENGTH  512

#ifdef _DOUBLE_UNICODE
typedef std::basic_stringstream<char32_t> 	u32stringstream;
#define  XCHAR    char32_t
#define  KXCHAR   const char32_t
#define  STRSTR   std::char_traits<char32_t>::find
#define  STRLEN   std::char_traits<char32_t>::length
#define  STRING   std::u32string
#define  SSTREAM  u32stringstream
#define  STRCMP   std::char_traits<char32_t>::compare
#define  ATOL     atol
#define  FROMQSTRING()   toStdU32String()
#define  X(msg)   l##msg
#else
#ifdef _UNICODE
typedef std::basic_stringstream<char16_t> 	u16stringstream;
#define  XCHAR    char16_t
#define  KXCHAR   const char16_t
#define  STRSTR   std::char_traits<char16_t>::find
#define  STRLEN   std::char_traits<char16_t>::length
#define  STRCPY   std::char_traits<char16_t>::copy
#define  STRING   std::u16string
#define  SSTREAM  u16stringstream
#define  STRCMP   std::char_traits<char16_t>::compare
#define  ATOL     atol
#define  FROMQSTRING()   toStdU16String()
#define  X(msg)   u##msg
#else
#define  XCHAR    char
#define  KXCHAR   const char
#define  STRSTR   std::char_traits<char>::find
#define  STRLEN   std::char_traits<char>::length
#define  STRCPY   std::char_traits<char>::copy
#define  STRING   std::string
#define  SSTREAM  std::stringstream
#define  SSTREAMOUT  std::cout
#define  ATOL     atol
#define  FROMQSTRING()   toStdString()
#define  STRCMP   std::char_traits<char>::compare
#define  X(msg)   msg
#endif
#endif

#define _EOL    std::endl

#define TOSTRING(__CLASS__)  const char *toString(void) final{ return #__CLASS__;}

#define TOSTRING_v(__CLASS__)  virtual const char *toString(void) { return #__CLASS__;}

typedef enum tag_handler_Action
{
    action_redraw=0,
    action_update_statusbars_info,
    action_update_selected_panes,
    action_update_scroller_panes,
    action_align_windows,
    action_iconize_windows,
    action_medialize_windows,
    action_maximize_windows,
    action_close_windows,
    action_move_window,
    action_select_position,
    action_host_command = 1000
} handlerAction;


class IParam
{
public:
    virtual void utf8(std::string & out) = 0;
    virtual ~IParam() {}
};

template < class T> class nnAbstractParam
    :public IParam
{
    T _value;
public:
    nnAbstractParam(T & v):_value(v) {}
    inline T value(void)
    {
        return _value;
    }
    inline void utf8(std::string & out)
    {
        std::stringstream s;
        s << _value;
        out = s.str();
    }
    virtual ~nnAbstractParam() {}
};


class nnAbstractParamList
    :public IParam
    ,public std::vector<IParam *>
{

public:
    nnAbstractParamList() {}
    void add(IParam * p)
    {
        if (p)
            push_back(p);
    }
    void utf8(std::string & out)
    {
        std::stringstream s;
        for (auto & i : *this)
        {
            std::string o;
            if (i)
                i->utf8(o);
            s << o << std::endl;
        }
        out = s.str();
    }
    virtual ~nnAbstractParamList()
    {
        for (auto & i : *this)
        {
            delete i;
        }
        clear();
    }
};




typedef void  (*extHandler)(void *,size_t,IParam *);

typedef std::pair<size_t, IParam *> ePair;


class IExtHandler
{
public:
    virtual  void doHandler(size_t Tparam,IParam* in=nullptr)=0;
    virtual ~IExtHandler() {}
    virtual const char * toString(void)=0;
};



//////////////////////////////////////////////////////

typedef enum tag_mouse_button_def
{
    nn_m_button_unknow = 0,
    nn_m_button_left = 1,
    nn_m_button_right = 2,
    nn_m_button_middle = 4
} nn_mouse_buttons;

typedef enum tag_show_status
{
    show_none = 0,
    show_toolbar_main,
    show_toolbar_compo,
    show_scroller_horz,
    show_scroller_vert,
    show_caption,
    show_capture,
} show_status;



inline std::ostream & operator<<(std::ostream & os, const show_status & st)
{
    switch (st)
    {
    case show_none:
        os << "show_none(0)";
        break;
    case show_toolbar_main :
        os << "show_toolbar_main(1)";
        break;
    case show_toolbar_compo:
        os << "show_toolbar_compo(2)";
        break;
    case show_scroller_horz:
        os << "show_scroller_horz(3)";
        break;
    case show_scroller_vert:
        os << "show_scroller_vert(4)";
        break;
    case show_caption:
        os << "show_caption(5)";
        break;
    case show_capture:
        os << "show_capture(6)";
        break;
    }
    return os;
}

class IHandler
{
public:
    virtual bool handlerMouseMove(nn_mouse_buttons buttons, nnPoint & phyPoint) = 0;
    virtual bool handlerMouseButtonDown(nn_mouse_buttons buttons, nnPoint & phyPoint) = 0;
    virtual bool handlerMouseButtonUp(nn_mouse_buttons buttons, nnPoint & phyPoint) = 0;
    virtual bool handlerScrollHorz(int pos) = 0;
    virtual bool handlerScrollVert(int pos) = 0;
    virtual bool handlerEscapeButton(bool shift, bool ctrl, bool alt) = 0;
    virtual bool handlerHomeButton(bool shitf, bool ctrl, bool alt) = 0;
    virtual bool handlerEndButton(bool shitf, bool ctrl, bool alt) = 0;
    virtual bool handlerPageUpButton(bool shitf, bool ctrl, bool alt) = 0;
    virtual bool handlerPageDownButton(bool shitf, bool ctrl, bool alt) = 0;
    virtual bool handlerPageRightButton(bool shitf, bool ctrl, bool alt) = 0;
    virtual bool handlerPageLeftButton(bool shitf, bool ctrl, bool alt) = 0;
    virtual bool handlerLeftButton(bool shitf, bool ctrl, bool alt) = 0;
    virtual bool handlerRightButton(bool shitf, bool ctrl, bool alt) = 0;
    virtual bool handlerUpButton(bool shitf, bool ctrl, bool alt) = 0;
    virtual bool handlerDownButton(bool shitf, bool ctrl, bool alt) = 0;
    virtual bool handlerCancelButton(bool shitf, bool ctrl, bool alt) = 0;
    virtual ~IHandler() {}
};



//////////////////////////////////////////////////////



class IXmlNode
{
public:
    virtual IXmlNode * find(const XCHAR *name, int idx)=0;
    virtual IXmlNode * find(const XCHAR *_name)=0;
    virtual IXmlNode * add(const XCHAR *name, XCHAR *value)=0;
    virtual IXmlNode * add(const XCHAR *name, int value)=0;
    virtual IXmlNode * add(const XCHAR *name, int idx, int value)=0;
    virtual long  getLong(void)=0;
    virtual const XCHAR * getValue(void)=0;
    virtual IXmlNode * getNext(void)=0;
    virtual const XCHAR  * getName(void)=0;
    virtual IXmlNode * getParent(void)=0;
    virtual IXmlNode * getChild(void)=0;
    virtual ~IXmlNode() {}
};

class IConfig
{
public:
    virtual bool readConfiguration(const XCHAR *name) = 0;
    virtual bool writeConfiguration(const XCHAR *name) = 0;
    virtual IXmlNode * getRoot(void) = 0;
    virtual ~IConfig() {}
};


//////////////////////////////////////////////////////
class bmpImage;

typedef struct nn_tag_command_item
{
    nnPoint pos;
    int  command;
    nnRect rect;
    nnRect btRect;
    unsigned char maskR;
    unsigned char maskG;
    unsigned char maskB;
    std::string   file;
    std::string   info;
    nn_tag_command_item()
    {
        maskR=maskG=maskB;
        file=info="";
        command=0;
    }



} commandItem;

typedef std::vector<commandItem *> listCommandItem;
class IFontManager;
class IViewGlue;


class ICommander
{
public:
    virtual bool readConfiguration(IXmlNode *node) = 0;
    virtual void setFont(IFontManager *_font) = 0;
    virtual bool loadImages(STRING &path)=0;
    virtual bool draw(bmpImage & bkg, nnPoint & pos, IViewGlue * glue) = 0;
    virtual bool checkRequestCommand(nnPoint & pos, int & command) = 0;
    virtual bool handlerMouseMove(nnPoint & pos, IExtHandler *hook) = 0;
    virtual const char *toString(void)=0;
    virtual ~ICommander() {}
};

//////////////////////////////////////////////////////



enum tag_obj_context
{
    objNone = 0,
    objWire,
    objContact,
    objCoil
};


typedef tag_obj_context ObjContext;


enum tag_wire_direction
{
    //0,0,------N,0
    //
    //0,N-------N,N
    direction_unknow,
    wire_from_up,
    wire_from_down,
    wire_from_left,
    wire_from_right,
};

typedef tag_wire_direction  eWireDirection;

typedef std::vector<int >  eConnections;



class  InnObj
{
public:
    virtual ObjContext getContext(void) = 0;
    virtual void setContext(ObjContext & c) = 0;
    virtual int getXpos(void) = 0;
    virtual int getYpos(void) = 0;
    virtual void setXpos(int pX) = 0;
    virtual void setYpos(int pY) = 0;
    virtual void setPos(int pX, int pY) = 0;
    virtual const char* toString(void) = 0;
    virtual eWireDirection getDirection(InnObj * b) = 0;
    virtual eConnections & getConnections(void) = 0;
    virtual void setConnections(int  v) = 0;
    virtual bool connect(InnObj* b) = 0;
    virtual bool powerConnect(int num) = 0;
    virtual bool disconnect(InnObj* b) = 0;
    virtual bool isComponent(void) = 0;
    virtual void save(IXmlNode *root) = 0;
    virtual void load(IXmlNode *root) = 0;
    virtual ~InnObj() {}
};

//////////////////////////////////////////////////////


enum tag_wire
{
    noWire = 0,
    wireHorizzontal,
    wireVertical,
    wireAngleUpRight,
    wireAngleUpLeft,
    wireAngleDownRight,
    wireAngleDownLeft,
    wireTHorizDown,
    wireTHorizUp,
    wireTVertRight,
    wireTVertLeft,
    wireCross,
    connectComponent,
};

typedef tag_wire eWire;


class InnWire
{
public:
    virtual eWire getWire(void) = 0;
    virtual void setWire(eWire c) = 0;
    virtual ~InnWire() {}
};



//////////////////////////////////////////////////////



typedef std::vector<unsigned int > eVCPUregister;


#include "MerlinoVCPU.h"


class InnVCPU
{
public:
    virtual eVCPUregister &getVCPUregister(void) = 0;
    virtual void setVCPUregister(eVCPUregister & r) = 0;
    virtual void save(IXmlNode *root) = 0;
    virtual void load(IXmlNode *root) = 0;
    virtual void setBaseVCPU(pMerlinoVCPU vcpu) = 0;
    virtual pMerlinoVCPU getBaseVCPU(void) = 0;
    virtual ~InnVCPU() {}
    virtual const char * toString(void) = 0;
};

//////////////////////////////////////////////////////

typedef union tag_hash_key
{
    struct tag_int_key
    {
        uint32_t v1;
        uint32_t v2;
    } i;
    uint64_t v12;
} hashkey;

class nnObjCoil;
class nnObjContact;
class IManager
{
public:
    //
    virtual bool addCoil(int x,  nnObjCoil * obj) = 0;
    virtual bool addWire(int x, int y, InnObj * obj) = 0;
    virtual bool addContact(int x, int y, nnObjContact * obj) = 0;
    virtual bool addObj(int x, int y, InnObj * obj) = 0;
    virtual bool removeObj(int x, int y) = 0;
    virtual InnObj * getObj(int x, int y) = 0;
    virtual InnObj * outObj(int x, int y) = 0;
    virtual bool replaceObj(int x, int y, InnObj * obj) = 0;
    virtual bool removeAll(void) = 0;
    virtual bool moveObj(nnPoint from, nnPoint to) = 0;
    virtual bool swapObj(nnPoint from, nnPoint to) = 0;
    virtual int getWidth(void) = 0;
    virtual int getHeight(void) = 0;
    virtual nnPoint getStartPoint(void) = 0;
    virtual nnPoint getStopPoint(void) = 0;
    virtual nnPoint getSchema(void) = 0;
    virtual bool save(const STRING & name) = 0;
    virtual bool load(const STRING & name) = 0;
    virtual bool undo(void) = 0;
    virtual bool redo(void) = 0;
    virtual bool insertRow(int y_pos) = 0;
    virtual bool insertCol(int x_pos) = 0;
    virtual bool removeRow(int y_pos) = 0;
    virtual bool removeCol(int x_pos) = 0;
    virtual bool removeEmptyCol(void) = 0;
    virtual bool ResizeHeight(int h) = 0;
    virtual bool ResizeWidth(int w) = 0;
    virtual bool Resize(int w, int h) = 0;
    virtual bool revIndexes(hashkey & key,int & x, int & y) = 0;
    virtual bool readConfiguration(IXmlNode *node)=0;
    virtual ~IManager() {}
    virtual const char * toString(void)=0;
};


class IUndoRedo
{
public :
    virtual bool undo(void) = 0;
    virtual bool redo(void) = 0;
    virtual ~IUndoRedo() {}
    virtual const char *toString(void)=0;
};

//////////////////////////////////////////////////////
typedef  std::map<int, STRING> objImageList;

class listImage;

class IImageManager
{
public:
    virtual bool setPath(STRING & _path) =0;
    virtual STRING  & getDefaulPath(void) const = 0;
    virtual bool readConfiguration(IXmlNode *node) = 0;
    virtual bool loadImages(void) = 0;
    virtual  bool loadImages(const listCommandItem * items) = 0;
    virtual bmpImage * getImage(int id) = 0;
    virtual bmpImage * getImage(const XCHAR * name) = 0;
    virtual const  objImageList * getAvailObj(void) = 0;
    virtual int getMaxWidth(void) = 0;
    virtual int getMaxHeight(void) = 0;
    virtual ~IImageManager() {}
    virtual const char * toString(void)=0;
};


class IFontManager
{
public:
    virtual bool setPath(STRING & _path) = 0;
    virtual STRING  & getDefaulPath(void) const = 0;
    virtual bool readConfiguration(IXmlNode *node) = 0;
    virtual bool loadImages(void) = 0;
    virtual bmpImage * getImage(const char * msg,unsigned char red, unsigned char green, unsigned char blue) = 0;
    virtual const  objImageList * getAvailObj(void) = 0;
    virtual ~IFontManager() {}
    virtual int getFontWidth(void) = 0;
    virtual int getFontHeight(void) = 0;
    virtual const char * toString(void) =0;

};

typedef std::list<std::string> fontNameList;


class IFontList
{
public:
    virtual bool readConfiguration(IXmlNode *node,STRING  &path) = 0;
    virtual bool loadImages(void)=0;
    virtual IFontManager* getManager(const char *name) = 0;
    virtual bool add(const char *name, IFontManager*) = 0;
    virtual bool remove(const char *name) = 0;
    virtual bool getFontNameList(fontNameList & list) = 0;
    virtual ~IFontList() {}
    virtual const char *toString(void)=0;
};
//////////////////////////////////////////////////////



class IComponent
{
public:
    virtual bool draw(bmpImage & bkg, IViewGlue * glue) = 0;
    virtual bool handlerMouseMove(nnPoint &phyPoint, show_status & status, IExtHandler *hook) = 0;
    virtual bool handlerMouseButtonDown(nnPoint &phyPoint, show_status & status, IExtHandler *hook) = 0;
    virtual void addImage(int pos, bmpImage *image) = 0;
    virtual void hide(void) = 0;
    virtual void show(void) = 0;
    virtual ~IComponent() {}
};

class IToolView
    : public IComponent
{
public:
    virtual bool readConfiguration(IXmlNode *node) = 0;
    virtual ICommander *getActiveCommander(void)=0;
    virtual bool loadImages(STRING & path)=0;
    virtual bool checkIntCommand(int command)=0;
    virtual void show(nnPoint & pos)=0;
    virtual void setFont(IFontManager *_font) = 0;
    virtual ~IToolView() {}
};


//////////////////////////////////////////////////////

class ISelector
{
public:
    virtual void draw(bmpImage & image,IViewGlue * glue)=0;
    virtual bool handlerMouseMove(nnPoint & logPoint) = 0;
    virtual bool handlerMouseButtonDown(nnPoint &logPoint,
                                        show_status & status) = 0;
    virtual bool handlerMouseButtonUp(nn_mouse_buttons buttons, nnPoint & logPoint)=0;
    virtual void hide(void)=0;
    virtual void show(void)=0;
    virtual bool getStatus(void)=0;
    virtual void setError(bool st)=0;
    virtual void setFont(IFontManager *font) = 0;
    virtual ~ISelector() {}
    virtual bool isSelectedValid(void) = 0;
    virtual bool isStartValid(void) = 0;
    virtual bool isStopValid(void) = 0;
    virtual int  isSelected(void) = 0;
    virtual bool isSelectedComponent(void) = 0;
    virtual bool unselect() = 0;
    virtual bool getSelectArea(nnPoint &start, nnPoint &stop) = 0;
    virtual bool select(nnPoint pos1, nnPoint pos2) = 0;
    virtual nnPoint & getSelectStart(void) = 0;
    virtual nnPoint & getSelectStop(void) = 0;
    virtual bool resizeSelectArea(const int vx, const int vy)=0;

};

class ICapture
{
public:
    virtual void setCommand(int c, unsigned int image,nnPoint & pos) = 0;
    virtual void draw(bmpImage & image, IViewGlue * glue) = 0;
    virtual bool handlerMouseMove(nnPoint &phyPoint,
                                  show_status & status, IExtHandler *hook) = 0;
    virtual bool handlerMouseButtonDown(nnPoint &phyPoint,
                                        show_status & status,IExtHandler *hook) = 0;
    virtual void setFont(IFontManager *font) = 0;
    virtual ~ICapture() {};
    virtual const char *toString(void)=0;
};



class IMoreInfo
{
public:
    virtual bool handlerMouseButtonDown(nnPoint &logPoint, int color, IExtHandler *hook) = 0;
    virtual void draw(bmpImage & image, IViewGlue * glue)=0;
    virtual void hide(void) = 0;
    virtual void show(void) = 0;
    virtual ~IMoreInfo() {}

};

//////////////////////////////////////////////////////
class IView
{
public:
    virtual bool readConfiguration(IXmlNode *node) = 0;
    virtual bool draw(void) = 0;
    virtual bool createMainBitmap(nnPoint & size) = 0;
    virtual bmpImage & getMainBitmap(void) = 0;
    virtual bool remapMainBitmap(nnPoint & size)=0;
    virtual void setFont(IFontManager *_font) = 0;
    virtual ~IView() {}
};


//////////////////////////////////////////////////////

typedef enum tag_scroller_mode
{
    mode_scroller_unknow,
    mode_scroller_horz= 100,
    mode_scroller_vert = 200
} scrollerMode;



class IScroller
    :public IComponent
{
public:
    virtual int getPosition(void)=0;
    virtual void update(int _pos)=0;
    virtual void setHorzArea(nnPoint & phy) = 0;
    virtual void setVertArea(nnPoint & phy) = 0;
    virtual bool setScrollSize(int maximun, int minimum) = 0;
    virtual ~IScroller() {}
};


class ICaption
    :public IComponent
{
public:
    virtual void setTitle(XCHAR *_name)=0;
    virtual void setArea(nnPoint & phy) = 0;
    virtual void setFont(IFontManager *_font) = 0;
    virtual const char *toString(void)=0;
    virtual ~ICaption() {}
};


class IViewGlue
    : public IHandler
{
public:
    virtual bool setPhyView(int w, int h) = 0;
    virtual bool setOrientation(int h,int v)=0;
    virtual nnPoint getOrientation(void)=0;
    virtual nnPoint getCoordPhy(const nnPoint & logPoint) = 0;
    virtual nnPoint getMirrorCoordPhy(nnPoint size,int x, int y) = 0;
    virtual nnPoint getCoordLog(const nnPoint & phyPoint) = 0;
    virtual nnPoint getConstPhy(void)=0;
    virtual nnPoint getPhySize(void)=0;
    virtual bool unselect() = 0;
    virtual bool readConfiguration(IXmlNode * node) = 0;
    virtual bool handlerScrollHorz(int pos) = 0;
    virtual bool handlerScrollVert(int pos) = 0;
    virtual bool handlerEscapeButton(bool shift, bool ctrl, bool alt) = 0;
    virtual bool handlerHomeButton(bool shitf, bool ctrl, bool alt) = 0;
    virtual bool handlerEndButton(bool shitf, bool ctrl, bool alt) = 0;
    virtual bool handlerPageUpButton(bool shitf, bool ctrl, bool alt) = 0;
    virtual bool handlerPageDownButton(bool shitf, bool ctrl, bool alt) = 0;
    virtual bool handlerPageRightButton(bool shitf, bool ctrl, bool alt) = 0;
    virtual bool handlerPageLeftButton(bool shitf, bool ctrl, bool alt) = 0;
    virtual bool handlerLeftButton(bool shitf, bool ctrl, bool alt) = 0;
    virtual bool handlerRightButton(bool shitf, bool ctrl, bool alt) = 0;
    virtual bool handlerUpButton(bool shitf, bool ctrl, bool alt) = 0;
    virtual bool handlerDownButton(bool shitf, bool ctrl, bool alt) = 0;
    virtual bool getSelectAreaPhy(int & width, int & height) = 0;
    virtual bool getSelectArea(nnPoint &start, nnPoint &stop) = 0;
    virtual bool select(nnPoint pos1, nnPoint pos2) = 0;
    virtual bmpImage & getDraw(void) = 0;
    virtual bool updateDraw(void) = 0;
    virtual nnPoint getOffsetView(void) = 0;
    virtual nnPoint getMap(void) = 0;
    virtual bool resize(int w, int h) = 0;
    virtual int getPageWidth(void)=0;
    virtual int getPageHeight(void)=0;
    virtual bool loadImages(STRING & _path)=0;
    virtual bool createDraw(void) = 0;
    virtual bool Capture(int command, unsigned int image) = 0;
    virtual IFontManager *getCurrentFont(void) = 0;
    virtual ~IViewGlue() {}
};

//////////////////////////////////////////////////////

//////////////////////////////////////////////////////

class IChild
    : public IHandler
{
public:
    virtual IManager * getManager(void) = 0;
    virtual IFontList * getFont(void) = 0;
    virtual IViewGlue * getView(void) = 0;
    virtual IImageManager * getImage(void) = 0;
    virtual IExtHandler * getHandler(void) = 0;
    virtual void clean(void) = 0;
    virtual bool createObjects(IConfig *configuration,STRING & conf_file_name,STRING & path_name) = 0;
    virtual bool setExtHandler(extHandler  _hook, void *unkObj) = 0;
    virtual bool getCurrentFile(std::string & filename) = 0;
    //virtual bool Capture(int command, unsigned int image) = 0;
    //virtual bool addContact(nnPoint & pos, nnObjContact * contact)=0;
    //virtual bool addCoil(nnPoint & pos, nnObjCoil * coil)=0;
    virtual void defaultProcess(size_t type_param, IParam *user_param)=0;
    virtual ~IChild() {}
    virtual const char *toString(void)=0;
};


class IAppManager
{
public:
    static  IAppManager * getInstance(int v=0);
    virtual IChild * createObjects(STRING & conf_file_name,STRING & path_name) = 0;
    virtual bool closeAll(void) = 0;
    virtual IChild *activate(int v) = 0;
    virtual IChild *active(void) = 0;
#if _LOGGER_
    virtual void setPrinter(IPrinter * printer)=0;
#endif
    virtual ~IAppManager() {}
    virtual const char *toString(void)=0;
};



#include <thread>

#endif // N2INTERFACES

