#ifndef NEW_NEXT_DRAW_MANAGER_HEADER
#define NEW_NEXT_DRAW_MANAGER_HEADER



#include <map>
#include "n2draw.h"

class IManager
{
public:
	// 
	virtual bool addObj(size_t x, size_t y, InnObj * obj) = 0;
	virtual bool removeObj(size_t x, size_t y) = 0;
	virtual InnObj * getObj(size_t x, size_t y) = 0;
	virtual InnObj * outObj(size_t x, size_t y) = 0;
	virtual bool replaceObj(size_t x, size_t y, InnObj * obj) = 0;
	virtual bool removeAll(void) = 0;
	virtual bool moveObj(n2Point from, n2Point to) = 0;
	virtual bool swapObj(n2Point from, n2Point to) = 0;
	virtual size_t getWidth(void) = 0;
	virtual size_t getHeight(void) = 0;
	virtual n2Point getStartPoint(void) = 0;
	virtual n2Point getStopPoint(void) = 0;
};

typedef unsigned long long int hashkey;
typedef std::map<hashkey, InnObj *> hashObjTable;


class nnObjManager
	: public IManager
	, public hashObjTable
{
protected:
	size_t v_width;
	size_t v_height;
	size_t mask_width;
	size_t mask_height;

public:
	nnObjManager(size_t x, size_t y);
	~nnObjManager() { removeAll(); }
	InnObj * getObj(size_t x, size_t y);
	InnObj * outObj(size_t x, size_t y);
	bool addObj(size_t x, size_t y, InnObj * obj);
	bool removeObj(size_t x, size_t y);
	bool replaceObj(size_t x, size_t y, InnObj * obj);
	bool removeAll(void);
	bool moveObj(n2Point from, n2Point to);
	bool swapObj(n2Point from, n2Point to);
	inline size_t getWidth(void) { return v_width; }
	inline size_t getHeight(void) { return v_height; }
	n2Point getStartPoint(void);
	n2Point getStopPoint(void);
protected:
	bool genHashKey(size_t x, size_t y, hashkey & key);
	bool range(size_t x, size_t y);
	bool linkObj(size_t x, size_t y, InnObj *obj);
	bool unlinkObj(size_t x, size_t y, InnObj *obj);

};





#endif
