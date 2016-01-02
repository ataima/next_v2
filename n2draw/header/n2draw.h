
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


#include "n2interfaces.h"



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
    const STRING toString(void) const;
    virtual void save(IXmlNode *root);
    virtual void load(IXmlNode *root);
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
    const  STRING toString(void) const;
    virtual void save(IXmlNode *root);
    virtual void load(IXmlNode *root);
    eWireDirection getDirection(InnObj * pb);
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
    const  STRING toString(void) const;
    virtual void save(IXmlNode *root);
    virtual void load(IXmlNode *root);
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
    const  STRING toString(void) const;
    virtual void save(IXmlNode *root);
    virtual void load(IXmlNode *root);
    inline bool isComponent(void) {
        return false;
    }
    bool connect(InnObj * b);
    bool disconnect(InnObj * b);
    bool connectFromUp(int b);
    bool connectFromDown(int b);
    void setConnections(int n);
    static eWire wireStringToEnum(const XCHAR *name);
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
    static custom_obj getCustomizationFromName(const XCHAR * s);

protected:
    bool disconnectFromUp(void);
    bool disconnectFromDown(void);
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
    const STRING toString(void) const;
    void save(IXmlNode *root);
    void load(IXmlNode *root);
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
    const  STRING toString(void) const;
    virtual void save(IXmlNode *root);
    virtual void load(IXmlNode *root);

};

class nnContactNO
    :public nnObjContact
{
public:
    nnContactNO():nnObjContact(contactGenericAnd) {}
    const  STRING toString(void) const;
};

class nnContactNC
    :public nnObjContact
{
public:
    nnContactNC() :nnObjContact(contactGenericOr) {}
    const  STRING toString(void) const;
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
    const  STRING toString(void) const;
    virtual void save(IXmlNode *root);
    virtual void load(IXmlNode *root);
};



class nnGenericCoil
    : public nnObjCoil
{
public:
    nnGenericCoil() :nnObjCoil(coilGeneric) {}
    const  STRING toString(void) const;
};


#endif
