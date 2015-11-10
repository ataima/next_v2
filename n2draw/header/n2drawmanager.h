#ifndef NEW_NEXT_DRAW_MANAGER_HEADER
#define NEW_NEXT_DRAW_MANAGER_HEADER



#include <map>
#include <vector>
#include "n2draw.h"

class IManager
{
public:
	// 
	virtual bool addObj(size_t x, size_t y, InnObj * obj) = 0;
	virtual bool removeObj(size_t x, size_t y) = 0;
	virtual bool replaceObj(size_t x, size_t y, InnObj * obj) = 0;
	virtual bool removeAll( void ) = 0;
	virtual bool setupPower(void) = 0;
	virtual InnObj * getObj(size_t x, size_t y) = 0;


};

typedef std::map<std::string, InnObj *> hashObjTable;
typedef std::vector<char> hashkeytable;
class nnObjManager
	: public IManager
	, public hashObjTable
{
protected:
	size_t v_width;
	size_t v_height;
        hashkeytable table;
public:
	nnObjManager(size_t x, size_t y);
	~nnObjManager() { removeAll(); }
	 InnObj * getObj(size_t x, size_t y);
	 bool addObj(size_t x, size_t y, InnObj * obj);
	 bool removeObj(size_t x, size_t y);
	 bool replaceObj(size_t x, size_t y, InnObj * obj);
	 bool removeAll( void );
	 bool setupPower(void);
protected:
	bool genHashKey(size_t x, size_t y, std::string & out);
	bool range(size_t x, size_t y);
	bool linkObj(size_t x, size_t y, InnObj *obj);
	bool unlinkObj(size_t x, size_t y, InnObj *obj);

};





#endif
