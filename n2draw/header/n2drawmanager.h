#ifndef NEW_NEXT_DRAW_MANAGER_HEADER
#define NEW_NEXT_DRAW_MANAGER_HEADER

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
	virtual void save(std::string & name) = 0;
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
	void save(std::string & name);
	void load(std::string & name);
protected:
	bool genHashKey(size_t x, size_t y, hashkey & key);
	bool range(size_t x, size_t y);
	bool linkObj(size_t x, size_t y, InnObj *obj);
	bool unlinkObj(size_t x, size_t y, InnObj *obj);

};





#endif
