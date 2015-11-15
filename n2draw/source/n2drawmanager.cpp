#include "n2drawmanager.h"
#include <algorithm>
#include <iostream>
#include <vector>

nnObjManager::nnObjManager(size_t x, size_t y)
	: v_width(x), v_height(y),
	mask_width(0), mask_height(0)
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
					delete obj;
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
				delete it->second;
				it->second = obj;
				obj->setXpos(x);
				obj->setYpos(y);
				if (it->second->isComponent()) {
					linkObj(x, y, obj);
					res = true;
				}
			}
		}
	return res;
}

bool nnObjManager::removeAll(void)
{
	bool res = false;
	if (size() > 0) {
		for (auto n : *this) {
			delete n.second;
		}
		res = true;
		clear();
	}
	return res;
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

