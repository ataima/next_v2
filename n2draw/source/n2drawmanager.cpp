#include "n2drawmanager.h"
#include <algorithm>




InnObj * nnObjManager::getObj(size_t x, size_t y)
{
	nnObj * res = nullptr;
	std::string hkey;
	if (range(x, y))
		if (genHashKey(x, y, hkey))
		{
			hashObjTable::iterator it = find(hkey);
			if (it != end())
			{
				res = dynamic_cast<nnObj *>(it->second);
			}
		}
	return res;
}

bool nnObjManager::addObj(size_t x, size_t y, InnObj * obj)
{
	bool res = false;
	std::string hkey;
	if (range(x, y) && obj != nullptr)
		if (genHashKey(x, y, hkey))
		{
			hashObjTable::iterator it = find(hkey);
			if (it == end())
			{
				std::pair<std::string, InnObj *> p(hkey, obj);
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
	std::string hkey;
	if (range(x, y))
		if (genHashKey(x, y, hkey))
		{
			hashObjTable::iterator it = find(hkey);
			if (it != end())
			{
				InnObj * obj = it->second;
				if (obj != nullptr)
				{
					erase(it); // remove first break recursion
					unlinkObj(x, y, obj);
					delete obj;
					res = true;
				}
			}
		}
	return res;
}

bool nnObjManager::replaceObj(size_t x, size_t y, InnObj * obj)
{
	bool res = false;
	std::string hkey;
	if (range(x, y) && obj != nullptr)
		if (genHashKey(x, y, hkey))
		{
			hashObjTable::iterator it = find(hkey);
			if (it != end())
			{
				delete it->second;
				it->second = obj;
				obj->setXpos(x);
				obj->setYpos(y);
				if (it->second->isComponent())
				{
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

bool nnObjManager::setupPower(void)
{
	bool res = false;
	size_t i;
	for (i = 0; i < v_width; i++)
	{
		nnPowerUp *up = new nnPowerUp();
		nnPowerDown *down = new nnPowerDown();
		if (up != nullptr)
			res = addObj(i, 0, up);
		if (!res)break;
		if (down != nullptr)
			res = addObj(i, v_height - 1, down);
		if (!res)break;
	}
	return res;
}

bool nnObjManager::genHashKey(size_t x, size_t y, std::string & out)
{
	bool res = false;
	const char table[] = "LMNOPQRSTUVWXlmnop6789abcdefghiqrstuvwxYZABCDEF<>-.;^()GHIJ012345jyz£$%&?*[]{}";
	const int  sizetable = sizeof(table);
	size_t i;
	out.clear();
	i = 0;
	while (x / sizetable)
	{
		out += table[i++];
	}
	out += table[x%sizetable];
	i = sizetable - 1;
	while (y / sizetable)
	{
		out += table[i--];
	}
	out += table[sizetable - (y%sizetable)];
	res = (out.size() > 0);
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
		if (y > 0)
		{
			InnObj *neighbour = getObj(x, y - 1);
			obj->setUpNeighbour(neighbour);
			if (neighbour != nullptr)
			{
				neighbour->setDownNeighbour(obj);
			}
		}
		if (y + 1 < v_height)
		{
			InnObj *neighbour = getObj(x, y + 1);
			obj->setDownNeighbour(neighbour);
			if (neighbour != nullptr)
			{
				neighbour->setUpNeighbour(obj);
			}
		}
		res = true;
	}
	return res;
}

bool nnObjManager::unlinkObj(size_t x, size_t y, InnObj *obj)
{
	bool res = false;

	if (y > 0)
	{
		InnObj *neighbour = getObj(x, y - 1);
		obj->setUpNeighbour(nullptr);
		if (neighbour != nullptr)
		{
			if (neighbour->isComponent())
			{
				neighbour->setDownNeighbour(nullptr);
			}
			else
			{
				removeObj(x, y - 1);
			}

		}
	}
	if (y + 1 < v_height)
	{
		InnObj *neighbour = getObj(x, y + 1);
		obj->setDownNeighbour(nullptr);
		if (neighbour != nullptr)
		{
			if (neighbour->isComponent())
			{
				neighbour->setUpNeighbour(nullptr);
			}
			else
			{
				removeObj(x, y + 1);
			}
		}
	}
	return true;
}
