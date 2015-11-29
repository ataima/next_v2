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

#include <list>

#include "n2draw.h"
#include "miniXml.h"


class nnObjCoil;
class nnObjContact;
class IManager
{
public:
	// 
	virtual bool addCoil(size_t x,  nnObjCoil * obj) = 0;
	virtual bool addWire(size_t x, size_t y, InnObj * obj) = 0;
	virtual bool addContact(size_t x, size_t y, nnObjContact * obj) = 0;
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
	virtual void save(STRING & name) = 0;
	virtual void load(STRING & name) = 0;
	virtual bool undo(void) = 0;
	virtual bool redo(void) = 0;
	virtual bool insertRow(size_t y_pos) = 0;
	virtual bool insertCol(size_t x_pos) = 0;
	virtual bool removeRow(size_t y_pos) = 0;
	virtual bool removeCol(size_t x_pos) = 0;
	virtual bool removeEmptyCol(void) = 0;
	virtual bool ResizeHeight(size_t h) = 0;
	virtual bool ResizeWidth(size_t w) = 0;
	virtual bool Resize(size_t w, size_t h) = 0;
	
};


class IUndoRedo
{
public :
	virtual bool undo(void) = 0;
	virtual bool redo(void) = 0;
};


typedef enum tag_action_type
{
	addObjAction = 1,
	removeObjAction,
	outObjAction,
}eAction;


typedef struct tag_action
{
	eAction action;
	InnObj *obj;
	size_t x_pos;
	size_t y_pos;
	tag_action(eAction a, size_t x, size_t y, InnObj* _obj = nullptr)
		: action(a), x_pos(x), y_pos(y), obj(_obj) {}
} undo_redo_unit;

typedef std::list<undo_redo_unit> vectorUR;


class nnObjUndoRedo
	: public IUndoRedo
{
	vectorUR undoObjs;
	vectorUR redoObjs;
	bool undoredoMode;
	IManager *manager;
public:
	nnObjUndoRedo(IManager *_manager=nullptr);
	~nnObjUndoRedo();
	bool setHost(IManager *_manager);
	bool undo(void);
	bool redo(void);
	inline vectorUR & getUndoObjs(void) { return undoObjs; }
	inline vectorUR & getRedoObjs(void) { return redoObjs; }
	void record(undo_redo_unit u);
private:
	void clearUndoObjs(void);
	void clearRedoObjs(void);
};


typedef unsigned long long int hashkey;
typedef std::map<hashkey, InnObj *> hashObjTable;

class nnObjManager
	: public IManager
	, public hashObjTable
{
	size_t v_width;
	size_t v_height;
	size_t mask_width;
	size_t mask_height;
	nnObjUndoRedo managerUR;
	xmlConfig configuration;
public:
	nnObjManager(size_t x, size_t y);
	~nnObjManager();
	InnObj * getObj(size_t x, size_t y);
	InnObj * outObj(size_t x, size_t y);
	bool addCoil(size_t x,  nnObjCoil * obj);
	bool addWire(size_t x, size_t y, InnObj * obj);
	bool addContact(size_t x, size_t y, nnObjContact * obj);
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
	void save(STRING & name);
	void load(STRING & name);
	bool undo(void);
	bool redo(void);
	bool insertRow(size_t y_pos);
	bool insertCol(size_t x_pos);
	bool removeRow(size_t y_pos);
	bool removeCol(size_t x_pos);
	bool removeEmptyCol(void);
	inline vectorUR & getUndoObjs(void) { return managerUR.getUndoObjs(); }
	inline vectorUR & geRedoObjs(void) { return managerUR.getRedoObjs(); }
	bool ResizeHeight(size_t h);
	bool ResizeWidth(size_t w);
	bool Resize(size_t w, size_t h);

protected:
	bool genHashKey(size_t x, size_t y, hashkey & key);
	bool revHashKey(hashkey & key, size_t & x, size_t &y);
	bool range(size_t x, size_t y); 
	bool rangeContact(size_t x, size_t y);
	bool linkObj(size_t x, size_t y, InnObj *obj);
	bool unlinkObj(size_t x, size_t y, InnObj *obj);
	bool checkRemovableCol(size_t x);
};





#endif
