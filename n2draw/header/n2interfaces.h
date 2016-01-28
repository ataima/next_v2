#ifndef N2INTERFACES
#define N2INTERFACES

#include <string>
#include <sstream>
#include <vector>
#include <list>
#include <map>

#include "n2point.h"
//////////////////////////////////////////////////////

typedef enum tag_handler_Action
{
    action_redraw,
    //action_update_from_ext_scroolbars,
    action_update_statusbars_info,
    action_update_statusbars_panes,
    action_align_windows,
    action_host_command = 5000
} handlerAction;


typedef void (*extHandler)(void *,handlerAction ,size_t);


class IExtHandler
{
public:
    virtual void doHandler(handlerAction Tparam,size_t Uparam=0)=0;
};

typedef enum tag_handler_exec
{
    handler_exec_command,       // hook to host command
} handler_exec;


class IExtHandlerList
{
public:
    virtual bool add(handler_exec type,IExtHandler *handler)=0;
    virtual bool remove(unsigned int type)=0;
    virtual bool clear(void)=0;
    virtual IExtHandler *get(unsigned int type)=0;
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
    show_toolbar,
    show_scroller_horz,
    show_scroller_vert,
}show_status;


class IHandler
{
public:
    virtual bool handlerMouseMove(nn_mouse_buttons buttons, nnPoint phyPoint) = 0;
    virtual bool handlerMouseButtonDown(nn_mouse_buttons buttons, nnPoint phyPoint) = 0;
    virtual bool handlerMouseButtonUp(nn_mouse_buttons buttons, nnPoint phyPoint) = 0;
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
    virtual bool handlerRequestCommand(nnPoint phyPoint, int & command) = 0;
};



//////////////////////////////////////////////////////

#define  BUFFLENGTH  512

#ifdef _DOUBLE_UNICODE
typedef std::basic_stringstream<char32_t> 	u32stringstream;
#define  XCHAR    char32_t
#define  STRSTR   std::char_traits<char32_t>::find
#define  STRLEN   std::char_traits<char32_t>::length
#define  STRING   std::u32string
#define  SSTREAM  u32stringstream
#define  STRCMP   std::char_traits<char32_t>::compare
#define  ATOL     atol
#define  FROMQSTRING()   toStdU32String()
#ifdef _MSC_VER
#define  FOPEN    fopen
#else
#define  FOPEN    fopen
#endif
#define  X(msg)   l##msg
#else
#ifdef _UNICODE
typedef std::basic_stringstream<char16_t> 	u16stringstream;
#define  XCHAR     char16_t
#define  STRSTR   std::char_traits<char16_t>::find
#define  STRLEN   std::char_traits<char16_t>::length
#define  STRING   std::u16string
#define  SSTREAM  u16stringstream
#define  STRCMP   std::char_traits<char16_t>::compare
#define  ATOL     atol
#define  FROMQSTRING()   toStdU16String()
#ifdef _MSC_VER
#define  FOPEN    fopen
#else
#define  FOPEN    fopen
#endif
#define  X(msg)   u##msg
#else
#define  XCHAR    char
#define  STRSTR   std::char_traits<char>::find
#define  STRLEN   std::char_traits<char>::length
#define  STRING   std::string
#define  SSTREAM  std::stringstream
#define  SSTREAMOUT  std::cout
#define  ATOL     atol
#define  FROMQSTRING()   toStdString()
#ifdef _MSC_VER
#define  FOPEN    fopen
#else
#define  FOPEN    fopen
#endif
#define  STRCMP   std::char_traits<char>::compare
#define  X(msg)   msg
#endif
#endif

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

};

class IConfig
{
public:
    virtual bool readConfiguration(const XCHAR *name) = 0;
    virtual bool writeConfiguration(const XCHAR *name) = 0;
    virtual IXmlNode * getRoot(void) = 0;
};


//////////////////////////////////////////////////////
class bmpImage;

typedef struct nn_tag_command_item
{
    nnPoint pos;
    int  command;
    nnRect rect;
    unsigned char maskR;
    unsigned char maskG;
    unsigned char maskB;
    std::string file;
    std::string info;
} commandItem;

typedef std::vector<commandItem> listCommandItem;
class IFontManager;
class IViewGlue;


class ICommander
{
public:
    virtual bool readConfiguration(IXmlNode *node) = 0;
    virtual void setFont(IFontManager *_font) = 0;
    virtual bool loadImages(const XCHAR *path)=0;
    virtual bool draw(bmpImage & bkg, nnPoint & pos, IViewGlue * glue) = 0;
    virtual bool handlerRequestCommand(nnPoint & pos, int & command) = 0;
    virtual bool handlerMouseMove(nnPoint & pos, IExtHandler *hook) = 0;
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
    virtual const STRING toString(void) const = 0;
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
    virtual const STRING toString(void) const = 0;
    virtual void save(IXmlNode *root) = 0;
    virtual void load(IXmlNode *root) = 0;
    virtual void setBaseVCPU(pMerlinoVCPU vcpu) = 0;
    virtual pMerlinoVCPU getBaseVCPU(void) = 0;
    virtual ~InnVCPU() {}

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
};


class IUndoRedo
{
public :
    virtual bool undo(void) = 0;
    virtual bool redo(void) = 0;
    virtual ~IUndoRedo() {}
};

//////////////////////////////////////////////////////
typedef  std::map<int, STRING> objImageList;

class listImage;

class IImageManager
{
public:
    virtual bool setPath(const XCHAR * _path) =0;
    virtual STRING  & getDefaulPath(void) const = 0;
    virtual bool readConfiguration(IXmlNode *node) = 0;
    virtual bool loadImages(void) = 0;
    virtual bool loadImages(objImageList *objs) = 0;
    virtual bmpImage * getImage(int id) = 0;
    virtual bmpImage * getImage(const XCHAR * name) = 0;
    virtual const  objImageList * getAvailObj(void) = 0;
    virtual ~IImageManager() {}
};


class IFontManager
{
public:
    virtual bool setPath(const XCHAR * _path) = 0;
    virtual STRING  & getDefaulPath(void) const = 0;
    virtual bool readConfiguration(IXmlNode *node) = 0;
    virtual bool loadImages(void) = 0;
    virtual bmpImage * getImage(const char * msg,unsigned char red, unsigned char green, unsigned char blue) = 0;
    virtual const  objImageList * getAvailObj(void) = 0;
    virtual ~IFontManager() {}
};

typedef std::list<std::string> fontNameList;


class IFontList
{
public:
    virtual bool readConfiguration(IXmlNode *node) = 0;
    virtual bool loadImages(void)=0;
    virtual IFontManager* getManager(const char *name) = 0;
    virtual bool add(const char *name, IFontManager*) = 0;
    virtual bool remove(const char *name) = 0;
    virtual bool getFontNameList(fontNameList & list) = 0;
};
//////////////////////////////////////////////////////

class IToolView
{
public:
    virtual bool readConfiguration(IXmlNode *node) = 0;
    virtual bool draw(bmpImage & bkg, IViewGlue * glue) = 0;
    virtual bool handlerRequestCommand(nnPoint & pos, int & command) = 0;
    virtual bool handlerMouseMove(nnPoint & pos, IExtHandler *hook) = 0;
    virtual ICommander *getActiveCommander(void)=0;
    virtual bool loadImages(const XCHAR *path)=0;
    virtual bool checkIntCommand(int command)=0;
    virtual void setDrawPosition(nnPoint & pos)=0;
    virtual void setFont(IFontManager *_font) = 0;
    virtual ~IToolView() {}
};

//////////////////////////////////////////////////////

class ISelector
{
public:
    virtual void draw(bmpImage & image,
        const nnPoint &start, const nnPoint &stop, IViewGlue * glue)=0;
    virtual void hide(void)=0;
    virtual void show(void)=0;
    virtual bool getStatus(void)=0;
    virtual void setError(bool st)=0;
    virtual void setFont(IFontManager *font) = 0;
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
}scrollerMode;

class IScroller
{
public:
    virtual bool draw(bmpImage & bkg, IViewGlue * glue) = 0;
    virtual void setHorzArea(nnPoint & phy) = 0;
    virtual void setVertArea(nnPoint & phy) = 0;
    virtual void setImage(bmpImage *one, bmpImage *two) = 0;
    virtual bool setScrollSize(int maximun, int minimum) = 0;
    virtual int getPosition(void) = 0;
    virtual void update(int _pos) = 0;
    virtual bool handlerMouseMove(nnPoint phyPoint, show_status & status, IExtHandler *hook) = 0;
    virtual bool handlerMouseButtonDown(nnPoint phyPoint, IViewGlue * glue) = 0;
    virtual void hide(void) = 0;
    virtual void show(void) = 0;
};

class IViewGlue
    : public IHandler
{
public:
    virtual bool setPhyView(int w, int h) = 0;
    virtual nnPoint getCoordPhy(const nnPoint & logPoint) = 0;
    virtual nnPoint getMirrorCoordPhy(int height,int x, int y) = 0;
    virtual nnPoint getCoordLog(const nnPoint & phyPoint) = 0;
    virtual nnPoint getConstPhy(void)=0;
    virtual bool readConfiguration(IXmlNode * node) = 0;
    virtual bool selectStart(int xpos, int ypos) = 0;
    virtual bool selectStop(int xpos1, int ypos1) = 0;
    virtual bool selectStart(nnPoint pos) = 0;
    virtual bool selectStop(nnPoint pos1) = 0;
    virtual bool select(nnPoint pos1, nnPoint pos2) = 0;
    virtual bool handlerMouseMove(nn_mouse_buttons buttons, nnPoint phyPoint) = 0;
    virtual bool handlerMouseButtonDown(nn_mouse_buttons buttons, nnPoint phyPoint) = 0;
    virtual bool handlerMouseButtonUp(nn_mouse_buttons buttons, nnPoint phyPoint) = 0;
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
    virtual bool handlerRequestCommand(nnPoint phyPoint, int & command) = 0;
    virtual bool unselect() = 0;
    virtual bool getSelectAreaPhy(int & width, int & height) = 0;
    virtual bool getSelectStartPhy(int & x, int & y) = 0;
    virtual bool isStartValid(void) = 0;
    virtual bool isStopValid(void) = 0;
    virtual bool getSelectArea(nnPoint &start, nnPoint &stop) = 0;
    virtual bmpImage & getDraw(void) = 0;
    virtual bool updateDraw(void) = 0;
    virtual nnPoint getOffsetView(void) = 0;
    virtual nnPoint getMap(void) = 0;
    virtual bool resize(int w, int h) = 0;
    virtual int getPageWidth(void)=0;
    virtual int getPageHeight(void)=0;
    virtual bool loadImages(const XCHAR *_path)=0;
    virtual bool createDraw(void) = 0;
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
    virtual IExtHandler * getExternalHandler(void) = 0;
    virtual IExtHandlerList *getHandlers(void) = 0;
    virtual bool addExtHandler(handler_exec type, extHandler  _hook, void *unkObj) = 0;
    virtual void clean(void) = 0;
    virtual bool createObjects(IConfig *configuration,STRING & conf_file_name) = 0;    
    virtual bool setExtHandler(handler_exec type, extHandler  _hook, void *unkObj) = 0;
    virtual void commandRuote(handlerAction type_param, size_t user_param) = 0;
};


class IAppManager
{
public:
    virtual IChild * createObjects(STRING & conf_file_name) = 0;
    virtual bool closeAll(void) = 0;
    virtual IChild *activate(int v) = 0;
    virtual IChild *active(void) = 0;
    virtual ~IAppManager() {}
};





#endif // N2INTERFACES

