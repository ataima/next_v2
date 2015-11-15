#include "n2draw.h"
#include "n2drawmanager.h"
#include "n2connection.h"

bool n2Connection::connectComponent(IManager * manager, n2Point & p_src, n2Point & p_dst)
{
	bool res = false;
	if (manager != nullptr)
	{
		InnObj *v = nullptr;
		InnObj *src = manager->getObj(p_src.x, p_src.y);
		InnObj *dst = manager->getObj(p_dst.x, p_dst.y);
		if (src != nullptr && dst != nullptr)
		{
			if (src->isComponent() && dst->isComponent())
			{
				nnObjComponent *c_src, *c_dst;
				c_src = dynamic_cast<nnObjComponent*>(src);
				c_dst = dynamic_cast<nnObjComponent*>(dst);
				if (c_src != nullptr && c_dst != nullptr)
				{   //TWO COMPONENTS ANY TYPE...
					if (p_src.x == p_dst.x && p_src.y != p_dst.y)
					{
						res = connectVertComponent(manager, p_src, p_dst);
					}
					else
						if (p_src.y == p_dst.y && p_src.x != p_dst.x)
						{
							if (p_src.x < p_dst.x)
							{
								res = connectHorzIncrUpSideComponet(manager, p_src, p_dst);
								if (res)
									res = connectHorzIncrDownSideComponet(manager, p_src, p_dst);
							}
							else
							{
								res = connectHorzDecrUpSideComponet(manager, p_src, p_dst);
								if (res)
									res = connectHorzDecrDownSideComponet(manager, p_src, p_dst);
							}

						}
				}
			}
			else
			{
				// 1 component + wire
				if (p_src.x == p_dst.x && p_src.y != p_dst.y)
				{
					res = connectVertWireComponent(manager, p_src, p_dst);
				}
				else
				{
					wireNoAvaiableConnection e;
					throw(e);
				}
			}
		}
	}
	return res;
}


bool n2Connection::connectVertComponent(IManager *manager, n2Point &p_src, n2Point & p_dst)
{
	bool res = false;
	size_t u;
	InnObj *v = nullptr;
	//vert connection
	if (p_src.y > p_dst.y)
	{
		for (u = p_dst.y + 1; u < p_src.y; u++)
		{
			v = manager->getObj(p_src.x, u);
			if (v == nullptr)
			{
				v = new nnObjWire(eWire::noWire);
				MEMCHK(InnObj, v);
				res = manager->addObj(p_src.x, u, v);
				if (!res)break;
			}
			else
				if (v->isComponent())
				{
					positionBusyException e(p_src.x, u);
					throw (e);
				}
		}
	}
	else
	{
		for (u = p_src.y + 1; u < p_dst.y; u++)
		{
			v = manager->getObj(p_src.x, u);
			if (v == nullptr)
			{
				v = new nnObjWire(eWire::noWire);
				MEMCHK(InnObj, v);
				res = manager->addObj(p_src.x, u, v);
				if (!res)break;
			}
			else
				if (v->isComponent())
				{
					positionBusyException e(p_src.x, u);
					throw (e);
				}
		}
	}
	return res;
}

bool n2Connection::connectHorzIncrUpSideComponet(IManager * manager, n2Point & p_src, n2Point & p_dst)
{
	bool res = false;
	size_t  i, num;
	InnObj *v = nullptr;
	InnObj *near_src = nullptr;
	InnObj *near_dst = nullptr;
	if (p_src.y > 0)
	{	//UP SIDE
		near_src = manager->getObj(p_src.x, p_src.y - 1);
		near_dst = manager->getObj(p_dst.x, p_dst.y - 1);
		if (near_src == nullptr && near_dst == nullptr)
		{
			v = new nnObjWire(eWire::wireAngleDownRight);
			MEMCHK(InnObj, v);
			if (manager->addObj(p_src.x, p_src.y - 1, v))
			{
				num = v->getConnections().front();
				for (i = p_src.x + 1; i < p_dst.x; i++)
				{
					v = new nnObjWire(eWire::noWire);
					MEMCHK(InnObj, v);
					v->setConnections(num);
					res = manager->addObj(i, p_src.y - 1, v);
					if (!res)break;
				}
				if (res)
				{
					v = new nnObjWire(eWire::wireAngleDownLeft);
					MEMCHK(InnObj, v);
					v->setConnections(num);
					res = manager->addObj(i, p_src.y - 1, v);
				}
			}
		}
		else
			if (near_src != nullptr && near_dst == nullptr)
			{
				num = near_src->getConnections().front();
				if (num == 1)
				{
					//vert to power positive
					for (i = 0; i < p_src.y - 1; i++)
					{
						v = new nnObjWire(eWire::wireVertical);
						MEMCHK(InnObj, v);
						v->setConnections(1);
						res = manager->addObj(p_src.x, i, v);
						if (!res)break;
					}
				}
				else
				{
					v = new nnObjWire(eWire::noWire);
					MEMCHK(InnObj, v);
					v->setConnections(num);
					if (manager->addObj(p_src.x + 1, p_src.y - 1, v))
					{
						for (i = p_src.x + 2; i < p_dst.x; i++)
						{
							v = new nnObjWire(eWire::noWire);
							MEMCHK(InnObj, v);
							v->setConnections(num);
							res = manager->addObj(i, p_src.y - 1, v);
							if (!res)break;
						}
						if (res)
						{
							v = new nnObjWire(eWire::wireAngleDownLeft);
							MEMCHK(InnObj, v);
							v->setConnections(num);
							res = manager->addObj(i, p_src.y - 1, v);
						}
					}
				}
			}
	}
	return res;
}

bool n2Connection::connectHorzIncrDownSideComponet(IManager * manager, n2Point & p_src, n2Point & p_dst)
{
	bool res = false;
	size_t  i, num;
	InnObj *v = nullptr;
	InnObj *near_src = nullptr;
	InnObj *near_dst = nullptr;
	if (p_src.y < manager->getHeight())
	{  //DOWN SIDE
		near_src = manager->getObj(p_src.x, p_src.y + 1);
		near_dst = manager->getObj(p_dst.x, p_dst.y + 1);
		if (near_src == nullptr && near_dst == nullptr)
		{
			v = new nnObjWire(eWire::wireAngleUpRight);
			MEMCHK(InnObj, v);
			if (manager->addObj(p_src.x, p_src.y + 1, v))
			{
				num = v->getConnections().front();
				for (i = p_src.x + 1; i < p_dst.x; i++)
				{
					v = new nnObjWire(eWire::noWire);
					MEMCHK(InnObj, v);
					v->setConnections(num);
					res = manager->addObj(i, p_src.y + 1, v);
					if (!res)break;
				}
				if (res)
				{
					v = new nnObjWire(eWire::wireAngleUpLeft);
					MEMCHK(InnObj, v);
					v->setConnections(num);
					res = manager->addObj(i, p_src.y + 1, v);
				}
			}
		}
		else
			if (near_src != nullptr && near_dst == nullptr)
			{
				num = near_src->getConnections().back();
				if (num == 2)
				{
					//vert to power positive
					for (i = p_src.y + 1; i <= manager->getHeight(); i++)
					{
						v = new nnObjWire(eWire::wireVertical);
						MEMCHK(InnObj, v);
						v->setConnections(2);
						res = manager->addObj(p_src.x, i, v);
						if (!res)break;
					}
				}
				else
				{
					v = new nnObjWire(eWire::noWire);
					MEMCHK(InnObj, v);
					v->setConnections(num);
					if (manager->addObj(p_src.x + 1, p_src.y + 1, v))
					{
						for (i = p_src.x + 2; i < p_dst.x; i++)
						{
							v = new nnObjWire(eWire::noWire);
							MEMCHK(InnObj, v);
							v->setConnections(num);
							res = manager->addObj(i, p_src.y + 1, v);
							if (!res)break;
						}
						if (res)
						{
							v = new nnObjWire(eWire::wireAngleUpLeft);
							MEMCHK(InnObj, v);
							v->setConnections(num);
							res = manager->addObj(i, p_src.y + 1, v);
						}
					}
				}
			}
	}
	return res;
}




bool n2Connection::connectHorzDecrUpSideComponet(IManager * manager, n2Point & p_src, n2Point & p_dst)
{
	bool res = false;
	size_t  i, num;
	InnObj *v = nullptr;
	InnObj *near_src = nullptr;
	InnObj *near_dst = nullptr;
	if (p_src.y > 0)
	{	//UP SIDE
		near_src = manager->getObj(p_src.x, p_src.y - 1);
		near_dst = manager->getObj(p_dst.x, p_dst.y - 1);
		if (near_src == nullptr && near_dst == nullptr)
		{
			v = new nnObjWire(eWire::wireAngleDownLeft);
			MEMCHK(InnObj, v);
			if (manager->addObj(p_src.x, p_src.y - 1, v))
			{
				num = v->getConnections().front();
				for (i = p_src.x - 1; i > p_dst.x; i--)
				{
					v = new nnObjWire(eWire::noWire);
					MEMCHK(InnObj, v);
					v->setConnections(num);
					res = manager->addObj(i, p_src.y - 1, v);
					if (!res)break;
				}
				if (res)
				{
					v = new nnObjWire(eWire::wireAngleDownRight);
					MEMCHK(InnObj, v);
					v->setConnections(num);
					res = manager->addObj(i, p_src.y - 1, v);
				}
			}
		}
		else
			if (near_src != nullptr && near_dst == nullptr)
			{
				num = near_src->getConnections().front();
				if (num == 1)
				{
					//vert to power positive
					for (i = 0; i < p_src.y - 1; i++)
					{
						v = new nnObjWire(eWire::wireVertical);
						MEMCHK(InnObj, v);
						v->setConnections(1);
						res = manager->addObj(p_src.x, i, v);
						if (!res)break;
					}
				}
				else
				{
					v = new nnObjWire(eWire::noWire);
					MEMCHK(InnObj, v);
					v->setConnections(num);
					if (manager->addObj(p_src.x - 1, p_src.y - 1, v))
					{
						for (i = p_src.x - 2; i > p_dst.x; i--)
						{
							v = new nnObjWire(eWire::noWire);
							MEMCHK(InnObj, v);
							v->setConnections(num);
							res = manager->addObj(i, p_src.y - 1, v);
							if (!res)break;
						}
						if (res)
						{
							v = new nnObjWire(eWire::wireAngleDownRight);
							MEMCHK(InnObj, v);
							v->setConnections(num);
							res = manager->addObj(i, p_src.y - 1, v);
						}
					}
				}
			}
	}
	return res;
}

bool n2Connection::connectHorzDecrDownSideComponet(IManager * manager, n2Point & p_src, n2Point & p_dst)
{
	bool res = false;
	size_t  i, num;
	InnObj *v = nullptr;
	InnObj *near_src = nullptr;
	InnObj *near_dst = nullptr;
	if (p_src.y < manager->getHeight())
	{  //DOWN SIDE
		near_src = manager->getObj(p_src.x, p_src.y + 1);
		near_dst = manager->getObj(p_dst.x, p_dst.y + 1);
		if (near_src == nullptr && near_dst == nullptr)
		{
			v = new nnObjWire(eWire::wireAngleUpLeft);
			MEMCHK(InnObj, v);
			if (manager->addObj(p_src.x, p_src.y + 1, v))
			{
				num = v->getConnections().front();
				for (i = p_src.x - 1; i > p_dst.x; i--)
				{
					v = new nnObjWire(eWire::noWire);
					MEMCHK(InnObj, v);
					v->setConnections(num);
					res = manager->addObj(i, p_src.y + 1, v);
					if (!res)break;
				}
				if (res)
				{
					v = new nnObjWire(eWire::wireAngleUpRight);
					MEMCHK(InnObj, v);
					v->setConnections(num);
					res = manager->addObj(i, p_src.y + 1, v);
				}
			}
		}
		else
			if (near_src != nullptr && near_dst == nullptr)
			{
				num = near_src->getConnections().back();
				if (num == 2)
				{
					//vert to power positive
					for (i = p_src.y + 1; i <= manager->getHeight(); i++)
					{
						v = new nnObjWire(eWire::wireVertical);
						MEMCHK(InnObj, v);
						v->setConnections(2);
						res = manager->addObj(p_src.x, i, v);
						if (!res)break;
					}
				}
				else
				{
					v = new nnObjWire(eWire::noWire);
					MEMCHK(InnObj, v);
					v->setConnections(num);
					if (manager->addObj(p_src.x - 1, p_src.y + 1, v))
					{
						for (i = p_src.x - 2; i > p_dst.x; i--)
						{
							v = new nnObjWire(eWire::noWire);
							MEMCHK(InnObj, v);
							v->setConnections(num);
							res = manager->addObj(i, p_src.y + 1, v);
							if (!res)break;
						}
						if (res)
						{
							v = new nnObjWire(eWire::wireAngleUpRight);
							MEMCHK(InnObj, v);
							v->setConnections(num);
							res = manager->addObj(i, p_src.y + 1, v);
						}
					}
				}
			}
	}
	return res;
}

/////////////////////////////////////////////////////////////////////////
bool n2Connection::connectVertWireComponent(IManager *manager, n2Point &p_src, n2Point & p_dst)
{
	bool res = false;
	size_t u;
	InnObj *v = nullptr;
	size_t num = 0;
	InnObj *src = manager->getObj(p_src.x, p_src.y);
	InnObj *dst = manager->getObj(p_dst.x, p_dst.y);
	if (src != nullptr && !src->isComponent())
		num = src->getConnections().front();
	if (dst != nullptr && !dst->isComponent())
		num = dst->getConnections().front();
	//vert connection
	if (p_src.y > p_dst.y)
	{
		for (u = p_dst.y + 1; u < p_src.y; u++)
		{
			v = manager->getObj(p_src.x, u);
			if (v == nullptr)
			{
				v = new nnObjWire(eWire::noWire);
				MEMCHK(InnObj, v);
				v->setConnections(num);
				res = manager->addObj(p_src.x, u, v);
				if (!res)break;
			}
			else
				if (v->isComponent())
				{
					positionBusyException e(p_src.x, u);
					throw (e);
				}
		}
	}
	else
	{
		for (u = p_src.y + 1; u < p_dst.y; u++)
		{
			v = manager->getObj(p_src.x, u);
			if (v == nullptr)
			{
				v = new nnObjWire(eWire::noWire);
				MEMCHK(InnObj, v);
				v->setConnections(num);
				res = manager->addObj(p_src.x, u, v);
				if (!res)break;
			}
			else
				if (v->isComponent())
				{
					positionBusyException e(p_src.x, u);
					throw (e);
				}
		}
	}
	return res;
}

