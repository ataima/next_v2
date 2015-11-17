
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

#include <string>
#include <stdexcept>
#include <sstream>
#include <vector>
#include <string.h>

typedef struct tag_n2_point
{
	size_t x;
	size_t y;
	tag_n2_point(size_t _x, size_t _y) :x(_x), y(_y) {}
} n2Point;


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

typedef std::vector<size_t >  eConnections;

class miniXmlNode;

class  InnObj
{
public:
	virtual ObjContext getContext(void) = 0;
	virtual void setContext(ObjContext & c) = 0;
	virtual size_t getXpos(void) = 0;
	virtual size_t getYpos(void) = 0;
	virtual void setXpos(size_t pX) = 0;
	virtual void setYpos(size_t pY) = 0;
	virtual void setPos(size_t pX, size_t pY) = 0;
	virtual const std::wstring toString(void) const = 0;
	virtual eWireDirection getDirection(InnObj * b) = 0;
	virtual eConnections & getConnections(void) = 0;
	virtual void setConnections(size_t  v) = 0;
	virtual bool connect(InnObj* b) = 0;
	virtual bool powerConnect(size_t num) = 0;
	virtual bool disconnect(InnObj* b) = 0;
	virtual bool isComponent(void) = 0;
	virtual void save(miniXmlNode *root) = 0;
};


class  nnObj
	:public InnObj
{
protected:
	ObjContext v_context;
public:
	nnObj(ObjContext c) { v_context = c; }
	inline ObjContext getContext(void) { return v_context; }
	inline void setContext(ObjContext & c) { v_context = c; }
	const std::wstring toString(void) const;
	virtual void save(miniXmlNode *root);
};


class  nnObjPos
	:public nnObj
{
protected:
	size_t v_Xpos;
	size_t v_Ypos;
public:
	nnObjPos(ObjContext c) :nnObj(c), v_Xpos(0), v_Ypos(0) { }
	inline  size_t getXpos(void) { return v_Xpos; }
	inline  size_t getYpos(void) { return v_Ypos; }
	inline  void setXpos(size_t pX) { v_Xpos = pX; }
	inline  void setYpos(size_t pY) { v_Ypos = pY; }
	inline  void setPos(size_t pX, size_t pY) { v_Xpos = pX; v_Ypos = pY; }
	const  std::wstring toString(void) const;
	virtual void save(miniXmlNode *root);
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
};

// wire up !=wire down 
class wireConnectionException
	:public std::runtime_error
{
public:
	eConnections up, down;
	explicit wireConnectionException(eConnections _up, eConnections _down) throw()
		:runtime_error("wireConnectionException"), up(_up), down(_down) {}

};

class positionBusyException
	:public std::runtime_error
{
public:
	size_t x, y;
	explicit positionBusyException(size_t _x, size_t _y) throw()
		:runtime_error("positionBusyException"), x(_x), y(_y) {}
};

class failMemoryException
	:public std::runtime_error
{
public:
	char fun[256];
	int line;
	explicit failMemoryException(const char *funct, int _line) throw()
		:runtime_error("failMemoryException"), line(_line) {
		size_t l = strlen(funct);
		size_t t = l < 255 ? l : 255;
		memcpy(fun, funct,t);
		fun[t] = 0;
	}
};


class wireNoAvaiableConnection
	:public std::runtime_error
{
public:
	explicit wireNoAvaiableConnection(void) throw()
		:runtime_error("wireNoAvaiableConnection") {}
};



template<class T>
inline void marshallObj(T* ptr,const char* f,int l)
{
	if (ptr == nullptr)
	{
		failMemoryException e(f,l);
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
	isGeneric
} spec_obj;

class  nnObjConn
	:public nnObjPos
{
protected:
	eConnections v_num;
	static size_t uid_num;

public:
	nnObjConn(ObjContext c) :nnObjPos(c), v_num(0) {}
	inline eConnections & getConnections(void) { return v_num; }
	virtual inline void setConnections(size_t n) { v_num.push_back(n); }
	const  std::wstring toString(void) const;
	virtual void save(miniXmlNode *root);
	static void resetUI(void) { uid_num = 2; }
	static size_t getUI(void) { return ++uid_num; }
	bool powerConnect(size_t num);
	static InnObj * getObjFromIds(spec_obj specific, ObjContext context);

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
	inline eWire getWire(void) { return v_wire; }
	inline virtual void setWire(eWire c) { v_wire = c; }
	const  std::wstring toString(void) const;
	virtual void save(miniXmlNode *root);
	inline bool isComponent(void) { return false; }
	bool connect(InnObj * b);
	bool disconnect(InnObj * b);
	bool connectFromUp(size_t b);
	bool connectFromDown(size_t b);
	void setConnections(size_t n);
protected:

	bool connectFromLeft(size_t b);
	bool connectFromRight(size_t b);

	bool disconnectFromUp(size_t b);
	bool disconnectFromDown(size_t b);
	bool disconnectFromLeft(size_t b);
	bool disconnectFromRight(size_t b);
};




class  nnObjComponent
	:public nnObjConn
{
public:
	nnObjComponent(ObjContext c) : nnObjConn(c) {}
	inline bool isComponent(void) { return true; }
	bool connect(InnObj *b);
	bool disconnect(InnObj *b);
	bool connectFromUp(size_t b);
	bool connectFromDown(size_t b);
protected:
	bool disconnectFromUp(void);
	bool disconnectFromDown(void);
};


class  nnObjContact
	:public nnObjComponent
{
	spec_obj v_spec;
public:
	nnObjContact(spec_obj _v=spec_obj::isGeneric) :
		nnObjComponent(ObjContext::objContact),v_spec(_v) {}
	const  std::wstring toString(void) const;
	virtual void save(miniXmlNode *root);
};


class  nnObjCoil
	:public nnObjComponent
{
	spec_obj v_spec;
public:
	nnObjCoil(spec_obj _v = spec_obj::isGeneric) :
		nnObjComponent(ObjContext::objCoil), v_spec(_v) {}
	const  std::wstring toString(void) const;
	virtual void save(miniXmlNode *root);
};


#endif
