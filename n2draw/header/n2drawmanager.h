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

#include "n2miniXml.h"
#include "n2draw.h"
#include "n2miniXml.h"
#include <stdint.h>
typedef union tag_hash_key
{
uint32_t v1;
uint32_t v2;
uint64_t v12;
} hashkey;


struct lessHashKey : public std::binary_function<hashkey ,hashkey  , bool>
  {
    bool
    operator()(const hashkey & a, const hashkey & b) const
    { return a.v12 < b.v12; }
  };

typedef std::map<hashkey, InnObj *,lessHashKey> hashObjTable;
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
    virtual bool readConfiguration(miniXmlNode & node) = 0;
    virtual bool revIndexes(hashkey & key,int & x, int & y) = 0;
    virtual ~IManager(){}
    
};


class IUndoRedo
{
public :
    virtual bool undo(void) = 0;
    virtual bool redo(void) = 0;
    virtual ~IUndoRedo(){}
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
    int x_pos;
    int y_pos;
    tag_action(eAction a, int x, int y, InnObj* _obj = nullptr)
        : action(a), obj(_obj), x_pos(x), y_pos(y) {}
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




class nnObjManager
    : public IManager
    , public hashObjTable
{
    int v_width;
    int v_height;
    int mask_width;
    int mask_height;
    nnObjUndoRedo managerUR;

public:
    nnObjManager(int x, int y);
    ~nnObjManager();
    InnObj * getObj(int x, int y);
    InnObj * outObj(int x, int y);
    bool addCoil(int x,  nnObjCoil * obj);
    bool addWire(int x, int y, InnObj * obj);
    bool addContact(int x, int y, nnObjContact * obj);
    bool addObj(int x, int y, InnObj * obj);
    bool removeObj(int x, int y);
    bool replaceObj(int x, int y, InnObj * obj);
    bool removeAll(void);
    bool moveObj(nnPoint from, nnPoint to);
    bool swapObj(nnPoint from, nnPoint to);
    inline int getWidth(void) { return v_width; }
    inline int getHeight(void) { return v_height; }
    nnPoint getStartPoint(void);
    nnPoint getStopPoint(void);
    bool save(const STRING & name);
    bool load(const STRING & name);
    bool undo(void);
    bool redo(void);
    bool insertRow(int y_pos);
    bool insertCol(int x_pos);
    bool removeRow(int y_pos);
    bool removeCol(int x_pos);
    bool removeEmptyCol(void);
    inline vectorUR & getUndoObjs(void) { return managerUR.getUndoObjs(); }
    inline vectorUR & geRedoObjs(void) { return managerUR.getRedoObjs(); }
    bool ResizeHeight(int h);
    bool ResizeWidth(int w);
    bool Resize(int w, int h);
    bool readConfiguration(miniXmlNode & node) ;
    inline bool revIndexes(hashkey & key, int & x, int & y)
                { return revHashKey(key, x, y); }

protected:
    bool genHashKey(int x, int y, hashkey & key);
    bool revHashKey(hashkey & key, int & x, int &y);
    bool range(int x, int y);
    bool rangeContact(int x, int y);
    bool linkObj(int x, int y, InnObj *obj);
    bool unlinkObj(int x, int y, InnObj *obj);
    bool checkRemovableCol(int x);
    bool computeMask(void);
};





#endif
