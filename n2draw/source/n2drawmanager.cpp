
#include "n2drawmanager.h"
#include <algorithm>
#include <iostream>
#include <vector>
#include "miniXml.h"
#include <stdarg.h> 

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




bool xmlConfig::readConfiguration(const wchar_t *name)
{
	bool res = false;
	return res;
}

bool xmlConfig::writeConfiguration(const wchar_t *name)
{
	bool res = false;
	return res;
}
//////////////////////////////////////////////////////////////////////
nnObjManager::nnObjManager(size_t x, size_t y)
	: v_width(x), v_height(y),
	mask_width(0), mask_height(0), 
	undoredoMode(false)
{
	nnObjWire::resetUI();
	size_t i = v_width & 0x0fffffff;
	mask_width = 1;
	while (i > 0)
	{
		i >>= 1;
		mask_width <<= 1;
		mask_width |= 1;
	}
	i = v_height & 0x0fffffff;
	mask_height = 1;
	while (i > 0)
	{
		i >>= 1;
		mask_height <<= 1;
		mask_height |= 1;
	}

}

nnObjManager::~nnObjManager()
{
	 removeAll(); 
	 clearUndoObjs();
	 clearRedoObjs();
}

InnObj * nnObjManager::getObj(size_t x, size_t y)
{
	nnObj * res = nullptr;
	hashkey hkey;
	if (range(x, y))
		if (genHashKey(x, y, hkey)) {
			hashObjTable::iterator it = find(hkey);
			if (it != end()) {
				res = dynamic_cast<nnObj *> (it->second);
			}
		}
	return res;
}

bool nnObjManager::addObj(size_t x, size_t y, InnObj * obj)
{
	bool res = false;
	hashkey hkey;
	if (range(x, y) && obj != nullptr)
		if (genHashKey(x, y, hkey)) {
			hashObjTable::iterator it = find(hkey);
			if (it == end()) {
				std::pair<hashkey, InnObj *> p(hkey, obj);
				insert(p);
				obj->setXpos(x);
				obj->setYpos(y);
				res = linkObj(x, y, obj);
				undo_redo_unit u(eAction::removeObjAction, x, y,nullptr);
				record(u);
			}
		}
	return res;
}

bool nnObjManager::removeObj(size_t x, size_t y)
{
	bool res = false;
	hashkey hkey;
	if (range(x, y))
		if (genHashKey(x, y, hkey)) {
			hashObjTable::iterator it = find(hkey);
			if (it != end()) {
				InnObj * obj = it->second;
				if (obj != nullptr) {
					erase(it); // remove first break recursion
					unlinkObj(x, y, obj);
					undo_redo_unit u(eAction::addObjAction, x, y,obj);
					record(u);
					res = true;
				}
			}
		}
	return res;
}


InnObj * nnObjManager::outObj(size_t x, size_t y)
{
	InnObj * obj = nullptr;
	hashkey hkey;
	if (range(x, y))
		if (genHashKey(x, y, hkey)) {
			hashObjTable::iterator it = find(hkey);
			if (it != end()) {
				obj = it->second;
				if (obj != nullptr) {
					erase(it); // remove first break recursion
					unlinkObj(x, y, obj);
					undo_redo_unit u(eAction::outObjAction, x, y, obj);
					record(u);
				}
			}
		}
	return obj;
}


bool nnObjManager::replaceObj(size_t x, size_t y, InnObj * obj)
{
	bool res = false;
	hashkey hkey;
	if (range(x, y) && obj != nullptr)
		if (genHashKey(x, y, hkey)) {
			hashObjTable::iterator it = find(hkey);
			if (it != end()) {
				undo_redo_unit u(eAction::addObjAction, x, y, it->second);
				record(u);
				it->second = obj;
				obj->setXpos(x);
				obj->setYpos(y);
				if (it->second->isComponent()) {
					linkObj(x, y, obj);
					undo_redo_unit v(eAction::removeObjAction, x, y, nullptr);
					record(v);
					res = true;
				}
			}
		}
	return res;
}

bool nnObjManager::removeAll(void)
{
	bool res = false;
	if (size() > 0)
	{
		for (auto n : *this)
		{
			delete n.second;
		}
		res = true;
		clear();
	}
	return res;
}


void nnObjManager::clearUndoObjs(void)
{
	if (undoObjs.size() > 0) 
	{
		for (auto n : undoObjs)
		{
			if(n.action==eAction::addObjAction && n.obj != nullptr)
				delete n.obj;
		}
		undoObjs.clear();
	}
}


void nnObjManager::clearRedoObjs(void)
{
	if (redoObjs.size() > 0)
	{
		for (auto n : redoObjs)
		{
			if (n.action == eAction::addObjAction && n.obj != nullptr)
				delete n.obj;
		}
		redoObjs.clear();
	}

}



void nnObjManager::save(STRING & name)
{
	size_t num_obj = 0;
	if (!name.empty())
	{
		try{
#ifdef _MSC_VER
			FILE *out = nullptr;
			FOPEN(&out, name.c_str(), X("w+"));
#else
			FILE *out = fopen(name.c_str(), "w+");
#endif
			if (out != NULL)
			{
				miniXmlNode root(X("next_v2"), X("1.0.0.0 Copyright(c) 2015 Angelo Coppi"));
				root.add(X("Wire_UID"), nnObjConn::getUI());
				root.add(X("Width"), v_width);
				root.add(X("Height"), v_height);
				root.add(X("Size"), size() + 1);
				hashObjTable::iterator it = begin();
				hashObjTable::iterator _end = end();
				while (it != _end)
				{
					miniXmlNode *child = root.add(X("Obj_UID_"), ++num_obj, num_obj);
					it->second->save(child);
					it++;
				}
				root.print(out);
				fclose(out);
			}
		} catch (...)
		{ }
	}
}



void nnObjManager::load(STRING & name)
{
	size_t num_obj = 0;
	if (!name.empty())
	{
		removeAll();
		try{
				miniXmlNode root;
				root.load(name.c_str(), &root);
				STRING name = root.getName();
				STRING value = root.getValue();
				if (name == X("next_v2") && value == X("1.0.0.0 Copyright(c) 2015 Angelo Coppi"))
				{
					miniXmlNode *t = root.find(X("Wire_UID"));
					if (t != nullptr)
					{
						nnObjConn::setUI(t->getLong());
					}
					t = root.find(X("Width"));
					if (t != nullptr)
					{
						v_width=t->getLong();
					}
					t = root.find(X("Height"));
					if (t != nullptr)
					{
						v_height = t->getLong();
					}
					miniXmlNode *size = root.find(X("Size"));
					if (size != nullptr)
					{
						size_t i,numObj = size->getLong();
						for (i = 1; i < numObj; i++)
						{
							miniXmlNode *child = root.find(X("Obj_UID_"), i);
							if (child != nullptr)
							{
								if (i == child->getLong())
								{
									miniXmlNode *spec = child->find(X("Spec"));
									if (spec != nullptr)
									{
										miniXmlNode *context = child->find(X("Context"));
										if (context != nullptr)
										{
											InnObj *obj = nnObjConn::getObjFromIds((spec_obj)spec->getLong(), (ObjContext)context->getLong());
											if (obj != nullptr)
											{
												obj->load(child);
												addObj(obj->getXpos(), obj->getYpos(), obj);
											}
										}
									}
								}
							}
						}
					}

				}
		}
		catch (...)
		{
		}
	}
}

bool nnObjManager::genHashKey(size_t x, size_t y, hashkey &key)
{
	bool res = false;
	x <<= 1;
	y <<= 1;
	x |= 1;
	y |= 1;
	x &= mask_width;
	y &= mask_height;
	key = x;
	key <<= 29;
	key |= y;
	res = key != 0;
	return res;
}

bool nnObjManager::range(size_t x, size_t y)
{
	bool res = false;
	if (x < v_width && y < v_height)
		res = true;
	return res;
}

bool nnObjManager::linkObj(size_t x, size_t y, InnObj *obj)
{
	bool res = false;
	if (obj != nullptr)
	{
		InnObj *neighbourUp = nullptr;
		InnObj *neighbourDw = nullptr;
		if (y > 0)
		{
			neighbourUp = getObj(x, y - 1);
		}
		else
		{
			obj->powerConnect(1);
		}
		if (y + 1 < v_height)
		{
			neighbourDw = getObj(x, y + 1);
		}
		else
		{
			obj->powerConnect(2);
		}

		// VERT
		if (obj->isComponent())
		{
			if (neighbourUp != nullptr)
				obj->connect(neighbourUp);
			if (neighbourDw != nullptr)
				obj->connect(neighbourDw);
			res = true;
		}
		else
		{
			if (neighbourUp != nullptr && neighbourDw != nullptr)
			{
				if (neighbourUp->isComponent() && !neighbourDw->isComponent())
				{
					res = obj->connect(neighbourDw);
					if (res)
						res = neighbourUp->connect(obj);
				}
				else
					if (!neighbourUp->isComponent() && neighbourDw->isComponent())
					{
						res = obj->connect(neighbourUp);
						if (res)
							res = neighbourDw->connect(obj);
					}
					else
						if (neighbourUp->isComponent() && neighbourDw->isComponent())
						{
							obj->setConnections(nnObjConn::getUI());
							res = neighbourUp->connect(obj);
							if (res)
								res = neighbourDw->connect(obj);
						}
						else
						{
							InnObj *w1 = dynamic_cast<InnObj *>(neighbourUp);
							InnObj *w2 = dynamic_cast<InnObj *>(neighbourDw);
							wireConnectionException e(neighbourUp->getConnections(), neighbourUp->getConnections());
							throw (e);
						}
			}
			else
				if (neighbourUp != nullptr)
				{
					if (neighbourUp->isComponent())
					{
						if (obj->getConnections().empty())
						{
							obj->setConnections(nnObjConn::getUI());
						}
						res = neighbourUp->connect(obj);
						if (res)
							res = obj->connect(neighbourUp);
					}
					else
					{
						obj->connect(neighbourUp);
					}
				}
				else
					if (neighbourDw != nullptr)
					{
						if (neighbourDw->isComponent())
						{
							if (obj->getConnections().empty())
							{
								obj->setConnections(nnObjConn::getUI());
							}
							res = neighbourDw->connect(obj);
							if (res)
								res = obj->connect(neighbourDw);
						}
						else
						{
							obj->connect(neighbourDw);
						}
					}
					else
					{
						//up & down =null 
						// check for horizzontal connection on wire
						InnObj *neighbourLeft = nullptr;
						InnObj *neighbourRight = nullptr;
						if (x > 0)
						{
							neighbourLeft = getObj(x - 1, y);
						}
						if (x + 1 < v_width)
						{
							neighbourRight = getObj(x + 1, y);
						}
						if (neighbourLeft != nullptr && !neighbourLeft->isComponent())
							res = obj->connect(neighbourLeft);
						if (neighbourRight != nullptr && !neighbourRight->isComponent())
							res = obj->connect(neighbourRight);
					}
			res = true;
		}

	}
	return res;
}

bool nnObjManager::unlinkObj(size_t x, size_t y, InnObj *obj)
{
	bool res = false;
	InnObj *neighbour = nullptr;
	if (y > 0) {
		neighbour = getObj(x, y - 1);
		if (neighbour != nullptr) {
			if (neighbour->isComponent()) {
				neighbour->disconnect(obj);
			}
			else {
				removeObj(x, y - 1);
			}
		}
	}
	if (y + 1 < v_height) {
		neighbour = getObj(x, y + 1);
		if (neighbour != nullptr) {
			if (neighbour->isComponent()) {
				neighbour->disconnect(obj);
			}
			else {
				removeObj(x, y + 1);
			}
		}
	}
	if (!obj->isComponent())
	{
		if (x > 0) {
			neighbour = getObj(x - 1, y);
			if (neighbour != nullptr) {
				if (!neighbour->isComponent()) {
					removeObj(x - 1, y);
				}
			}
		}
		if (x + 1 < v_width) {
			neighbour = getObj(x + 1, y);
			if (neighbour != nullptr) {
				if (!neighbour->isComponent()) {
					removeObj(x + 1, y);
				}
			}
		}

	}
	return true;
}



bool nnObjManager::moveObj(n2Point from, n2Point to)
{
	bool res = false;
	InnObj *fromObj = getObj(from.x, from.y);
	InnObj *toObj = getObj(to.x, to.y);
	if (toObj == nullptr && fromObj != nullptr && fromObj->isComponent())
	{
		fromObj = outObj(from.x, from.y);
		res = addObj(to.x, to.y, fromObj);
	}
	return res;
}

bool nnObjManager::swapObj(n2Point from, n2Point to)
{
	bool res = false;
	InnObj *fromObj = getObj(from.x, from.y);
	InnObj *toObj = getObj(to.x, to.y);
	if (toObj != nullptr && fromObj != nullptr && toObj->isComponent() && fromObj->isComponent())
	{
		fromObj = outObj(from.x, from.y);
		toObj = outObj(to.x, to.y);
		res = addObj(to.x, to.y, fromObj);
		if(res)
			res=addObj(from.x, from.y, toObj);
	}
	return res;
}

n2Point  nnObjManager::getStartPoint(void)
{
	size_t x , y;
	hashObjTable::iterator it = begin();
	hashkey key = it->first;
	y = key & 0xfffffff;
	y = y/2;
	x = (key >> 30) & 0xfffffff;
	return n2Point(x, y);

}


n2Point  nnObjManager::getStopPoint(void)
{
	size_t x, y;
	hashObjTable::reverse_iterator r_it = rbegin();
	r_it++;
	hashObjTable::iterator it = r_it.base();
	hashkey key = it->first;
	y = key & 0xfffffff;
	y = y/2;
	x = (key >> 30) & 0xfffffff;
	return n2Point(x, y);
}


void nnObjManager::record(undo_redo_unit u)
{

	if (undoredoMode == false)
	{
		if (undoObjs.size() >= 100)
		{
			undo_redo_unit old = undoObjs.front();
			undoObjs.pop_front();
			if (old.obj != nullptr)
				delete old.obj;
		}
		undoObjs.push_back(u);
	}
	else
	{
		if (redoObjs.size() >= 100)
		{
			undo_redo_unit old = redoObjs.front();
			redoObjs.pop_front();
			if (old.obj != nullptr)
				delete old.obj;
		}
		redoObjs.push_back(u);
	}
}

bool nnObjManager::undo(void)
{
	bool res = false;
	if (undoObjs.size() > 0)
	{
		undo_redo_unit f = undoObjs.back();
		undoObjs.pop_back();
		switch(f.action)
		{
		case addObjAction:
			undoredoMode = true;
			return addObj(f.x_pos, f.y_pos, f.obj);
			undoredoMode = false;
			break;
		case removeObjAction:
			undoredoMode = true;
			return removeObj(f.x_pos, f.y_pos);
			undoredoMode = false;
			break;
		case outObjAction:
			undoredoMode = true;
			addObj(f.x_pos, f.y_pos, f.obj);
			undoredoMode = false;
			break;
		}
	}
	return res;
}


bool nnObjManager::redo(void)
{
	bool res = false;
	if (redoObjs.size() > 0)
	{
		undo_redo_unit f = redoObjs.back();
		redoObjs.pop_back();
		switch (f.action)
		{
		case addObjAction:
			undoredoMode = false;
			return addObj(f.x_pos, f.y_pos, f.obj);
			break;
		case removeObjAction:
			undoredoMode = false;
			return removeObj(f.x_pos, f.y_pos);
			break;
		case outObjAction:
			undoredoMode = false;
			addObj(f.x_pos, f.y_pos, f.obj);
			break;
		}
	}
	return res;
}

bool readConfiguration(const wchar_t *name)
{
	bool res = false;
	return false;
}


bool writeConfiguration(const wchar_t *name)
{
	bool res = false;
	return false;
}
