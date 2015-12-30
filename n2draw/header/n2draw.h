
#ifndef NEW_NEXT_DRAW_HEADER
#define NEW_NEXT_DRAW_HEADER

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


#include "n2exception.h"



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
    /*
    inline bool intoX (int minV,int maxV)
    {
        bool res=false;
        if(x>minV && x<maxV)
            res=true;
        return res;
    }
    inline bool  intoY (int minV,int maxV)
    {
        bool res=false;
        if(x>=minV && x<maxV)
            res=true;
        return res;
    }
    */
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

} nnRect;


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

class miniXmlNode;

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
    virtual const std::u16string toString(void) const = 0;
    virtual eWireDirection getDirection(InnObj * b) = 0;
    virtual eConnections & getConnections(void) = 0;
    virtual void setConnections(int  v) = 0;
    virtual bool connect(InnObj* b) = 0;
    virtual bool powerConnect(int num) = 0;
    virtual bool disconnect(InnObj* b) = 0;
    virtual bool isComponent(void) = 0;
    virtual void save(miniXmlNode *root) = 0;
    virtual void load(miniXmlNode *root) = 0;
    virtual ~InnObj(){}
};


class  nnObj
    :public InnObj
{
protected:
    ObjContext v_context;
public:
    nnObj(ObjContext c) {
        v_context = c;
    }
    inline ObjContext getContext(void) {
        return v_context;
    }
    inline void setContext(ObjContext & c) {
        v_context = c;
    }
    const std::u16string toString(void) const;
    virtual void save(miniXmlNode *root);
    virtual void load(miniXmlNode *root);
};


class  nnObjPos
    :public nnObj
{
protected:
    int v_Xpos;
    int v_Ypos;
public:
    nnObjPos(ObjContext c) :nnObj(c), v_Xpos(0), v_Ypos(0) { }
    inline  int getXpos(void) {
        return v_Xpos;
    }
    inline  int getYpos(void) {
        return v_Ypos;
    }
    inline  void setXpos(int pX) {
        v_Xpos = pX;
    }
    inline  void setYpos(int pY) {
        v_Ypos = pY;
    }
    inline  void setPos(int pX, int pY) {
        v_Xpos = pX;
        v_Ypos = pY;
    }
    const  std::u16string toString(void) const;
    virtual void save(miniXmlNode *root);
    virtual void load(miniXmlNode *root);
    eWireDirection getDirection(InnObj * pb);
};

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
    wireCross
};

typedef tag_wire eWire;


class InnWire
{
public:
    virtual eWire getWire(void) = 0;
    virtual void setWire(eWire c) = 0;
    virtual ~InnWire(){}
};



// wire up !=wire down



template<class T>
inline void marshallObj(T* ptr, const char* f, int l)
{
    if (ptr == nullptr)
    {
        failMemoryException *e= new failMemoryException(f, l);
        throw(e);
    }
}

#if (_MSC_VER < 1900)
#define __func__ __FUNCTION__
#define snprintf  _snprintf
#endif

#define MEMCHK(type,ptr) marshallObj<type>(ptr,__func__,__LINE__)

typedef enum tag_specialization
{
    no_component = 0,
    contact_generic_unknow = 100000,
    coil_generic_unknow = 200000,
    contact_start_spec = 1000000,
    // add here new input components
    contactGenericAnd,
    contactGenericOr,
    // end contact spec
    coil_start_spec = 2000000,
    // add here new input components
    coilGeneric,

} custom_obj;



class  nnObjConn
    :public nnObjPos
{
protected:
    eConnections v_num;
    static int uid_num;

public:
    nnObjConn(ObjContext c) :nnObjPos(c), v_num(0) {}
    inline eConnections & getConnections(void) {
        return v_num;
    }
    virtual inline void setConnections(int n) {
        v_num.push_back(n);
    }
    const  std::u16string toString(void) const;
    virtual void save(miniXmlNode *root);
    virtual void load(miniXmlNode *root);
    static void resetUI(void) {
        uid_num = 2;
    }
    static int getUI(void) {
        return ++uid_num;
    }
    static void setUI(long u) {
        uid_num = u;
    }
    bool powerConnect(int num);
    static InnObj * getObjFromIds(custom_obj specific, ObjContext context);

};



class  nnObjWire
    : public nnObjConn
    , public InnWire
{
protected:
    eWire v_wire;
public:
    nnObjWire(eWire c) :nnObjConn(ObjContext::objWire)
        , v_wire(c) {}
    inline eWire getWire(void) {
        return v_wire;
    }
    inline virtual void setWire(eWire c) {
        v_wire = c;
    }
    const  std::u16string toString(void) const;
    virtual void save(miniXmlNode *root);
    virtual void load(miniXmlNode *root);
    inline bool isComponent(void) {
        return false;
    }
    bool connect(InnObj * b);
    bool disconnect(InnObj * b);
    bool connectFromUp(int b);
    bool connectFromDown(int b);
    void setConnections(int n);
    static eWire wireStringToEnum(const char16_t *name);
protected:

    bool connectFromLeft(int b);
    bool connectFromRight(int b);

    bool disconnectFromUp(int b);
    bool disconnectFromDown(int b);
    bool disconnectFromLeft(int b);
    bool disconnectFromRight(int b);
};




class  nnObjComponent
    :public nnObjConn
{
protected:
    custom_obj v_spec;
public:
    nnObjComponent(ObjContext c) : nnObjConn(c) {}
    inline bool isComponent(void) {
        return true;
    }
    bool connect(InnObj *b);
    bool disconnect(InnObj *b);
    bool connectFromUp(int b);
    bool connectFromDown(int b);
    inline custom_obj getCustomization(void) {
        return v_spec;
    }
    static custom_obj getCustomizationFromName(const char16_t * s);

protected:
    bool disconnectFromUp(void);
    bool disconnectFromDown(void);
};

typedef std::vector<unsigned int > eVCPUregister;


#include "MerlinoVCPU.h"


class InnVCPU
{
public:
    virtual eVCPUregister &getVCPUregister(void) = 0;
    virtual void setVCPUregister(eVCPUregister & r) = 0;
    virtual const std::u16string toString(void) const = 0;
    virtual void save(miniXmlNode *root) = 0;
    virtual void load(miniXmlNode *root) = 0;
    virtual void setBaseVCPU(pMerlinoVCPU vcpu) = 0;
    virtual pMerlinoVCPU getBaseVCPU(void) = 0;
    virtual ~InnVCPU(){}

};

class nnObjVCPU
    :public InnVCPU
{
    pMerlinoVCPU v_vcpu;
    eVCPUregister v_reg;
public:
    nnObjVCPU() :v_vcpu(nullptr) {
        v_reg.clear();
    }
    inline eVCPUregister &getVCPUregister(void) {
        return  v_reg;
    }
    inline void setVCPUregister(eVCPUregister & r) {
        v_reg.clear();
        v_reg = r;
    }
    const std::u16string toString(void) const;
    void save(miniXmlNode *root);
    void load(miniXmlNode *root);
    virtual void setBaseVCPU(pMerlinoVCPU _vcpu) {
        v_vcpu = _vcpu;
    };
    virtual pMerlinoVCPU getBaseVCPU(void) {
        return v_vcpu;
    }
};




class  nnObjContact
    : public nnObjComponent
    , public nnObjVCPU
{

public:
    nnObjContact(custom_obj _v = custom_obj::contact_generic_unknow) :
        nnObjComponent(ObjContext::objContact) {
        v_spec = _v;
    }
    const  std::u16string toString(void) const;
    virtual void save(miniXmlNode *root);
    virtual void load(miniXmlNode *root);

};

class nnContactNO
    :public nnObjContact
{
public:
    nnContactNO():nnObjContact(contactGenericAnd) {}
    const  std::u16string toString(void) const;
};

class nnContactNC
    :public nnObjContact
{
public:
    nnContactNC() :nnObjContact(contactGenericOr) {}
    const  std::u16string toString(void) const;
};


class  nnObjCoil
    : public nnObjComponent
    , public nnObjVCPU
{

public:
    nnObjCoil(custom_obj _v = custom_obj::coil_generic_unknow) :
        nnObjComponent(ObjContext::objCoil) {
        v_spec = _v;
    }
    const  std::u16string toString(void) const;
    virtual void save(miniXmlNode *root);
    virtual void load(miniXmlNode *root);
};



class nnGenericCoil
    : public nnObjCoil
{
public:
    nnGenericCoil() :nnObjCoil(coilGeneric) {}
    const  std::u16string toString(void) const;
};


#endif
