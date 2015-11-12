
#ifndef NEW_NEXT_DRAW_HEADER
#define NEW_NEXT_DRAW_HEADER



#include <string>
#include <stdexcept>
#include <sstream>


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


class  InnObj
{
public:
	virtual ObjContext getContext(void) = 0;
	virtual void setContext(ObjContext & c) = 0;
	virtual size_t getXpos(void) = 0;
	virtual size_t getYpos(void) = 0;
	virtual void setXpos(size_t pX) = 0;
	virtual void setYpos(size_t pY) = 0;
	virtual const std::wstring toString(void) const = 0;
	virtual eWireDirection getDirection(InnObj & b) = 0;
	virtual void setUpNeighbour(InnObj *obj) = 0;
	virtual void setDownNeighbour(InnObj *obj) = 0;
	virtual bool connect(InnObj* b) = 0;
	virtual bool disconnect(InnObj* b) = 0;
	virtual bool isComponent(void) = 0;
};


class  nnObj
	:public InnObj
{
protected:
	ObjContext v_context;
public:
	nnObj(ObjContext c){ v_context = c; }
	inline ObjContext getContext(void) { return v_context; }
	inline void setContext(ObjContext & c) { v_context = c; }
	const std::wstring toString(void) const;
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
	virtual inline  void setXpos(size_t pX) { v_Xpos = pX; }
	virtual inline  void setYpos(size_t pY) { v_Ypos = pY; }
	const  std::wstring toString(void) const;
	eWireDirection getDirection(InnObj & b);
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



enum tag_comp_conn
{
	conn_none,
	conn_up,
	conn_down
};

typedef tag_comp_conn eConnection;

class nnObjWire;


class InnWire
{
public:
	virtual eWire getWire(void) = 0;
	virtual void setWire(eWire c) = 0;
	virtual size_t getNum(void) = 0;
	virtual size_t getNum(nnObjWire & b) = 0;
	virtual void setNum(size_t n) = 0;

};

// wire up !=wire down 
class wireConnectionException
	:public std::runtime_error 
{
public :
	size_t up, down;
	explicit wireConnectionException(size_t _up, size_t _down) throw()
		:runtime_error("wireConnectionException"), up(_up),down(_down) {}

};

class positionBusyException
	:public std::runtime_error
{
public:
	size_t x, y;
	explicit positionBusyException(size_t _x, size_t _y) throw()
		:runtime_error("positionBusyException"), x(_x), y(_y) {}

};


class  nnObjWire
	:public nnObjPos
	, public InnWire
{
protected:
	eWire v_wire;
	size_t v_num;
	static size_t uid_num;

public:
	nnObjWire(eWire c) :nnObjPos(ObjContext::objWire)
		, v_wire(c), v_num(0) {}
	inline eWire getWire(void) { return v_wire; }
	inline virtual void setWire(eWire c) { v_wire = c; }
	inline size_t getNum(void) { return v_num; }
	inline virtual void setNum(size_t n) { v_num = n; }
	size_t getNum(nnObjWire & b);
	bool connect(InnObj * b);
	bool disconnect(InnObj * b);
	const  std::wstring toString(void) const;
	void setUpNeighbour(InnObj *obj);
	void setDownNeighbour(InnObj *obj);
	inline bool isComponent(void) { return false; }
	static void resetUI(void) { uid_num = 2; }
protected:
	bool connectFromUp(nnObjWire & b);
	bool connectFromDown(nnObjWire & b);
	bool connectFromLeft(nnObjWire & b);
	bool connectFromRight(nnObjWire & b);

	bool disconnectFromUp(nnObjWire & b);
	bool disconnectFromDown(nnObjWire & b);
	bool disconnectFromLeft(nnObjWire & b);
	bool disconnectFromRight(nnObjWire & b);

};


class  nnPowerUp
	:public nnObjWire
{
public:
	nnPowerUp() :nnObjWire(eWire::wireTHorizDown) { v_num = 1; }
	const  std::wstring toString(void) const;
	inline void setWire(eWire c) {}
	inline void setNum(size_t n) {}
protected:
	inline bool connectFromUp(nnObjWire & b) { return false; }
	inline bool connectFromDown(nnObjWire & b) { return true; }
	inline bool connectFromLeft(nnObjWire & b) { return false; }
	inline bool connectFromRight(nnObjWire & b) { return false; }
};


class  nnPowerDown
	:public nnObjWire
{
public:
	nnPowerDown() :nnObjWire(eWire::wireTHorizUp) { v_num = 2; }
	const  std::wstring toString(void) const;
	inline void setWire(eWire c) {}
	inline void setNum(size_t n) {}
protected:
	bool connectFromUp(nnObjWire & b) { return true; }
	bool connectFromDown(nnObjWire & b) { return false; }
	bool connectFromLeft(nnObjWire & b) { return false; }
	bool connectFromRight(nnObjWire & b) { return false; }
};




class  nnObjComponent
	:public nnObjPos
{
	nnObjWire up_wire;
	nnObjWire down_wire;
	InnObj* up_neighbours;
	InnObj* down_neighbours;
public:
	nnObjComponent(ObjContext c) : nnObjPos(c)
		, up_wire(eWire::noWire), down_wire(eWire::noWire)
		, up_neighbours(nullptr),down_neighbours(nullptr){}
	nnObjWire &getUpWire(void) { return up_wire; }
	nnObjWire &getDownWire(void) { return down_wire; }
	const  std::wstring toString(void) const;
	void setUpNeighbour(InnObj *obj);
	void setDownNeighbour(InnObj *obj);
	inline bool isComponent(void) { return true; }
	virtual bool connect(InnObj *b);
	virtual void setXpos(size_t pX);
	virtual void setYpos(size_t pY);
	virtual bool disconnect(InnObj *b) { return false; }

};


class  nnObjContact
	:public nnObjComponent
{

public:
	nnObjContact() :nnObjComponent(ObjContext::objContact) {}
	const  std::wstring toString(void) const;
};


class  nnObjCoil
	:public nnObjComponent
{
public:
	nnObjCoil() :nnObjComponent(ObjContext::objCoil) {}
	const  std::wstring toString(void) const;
};


#endif
