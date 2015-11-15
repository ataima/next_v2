#include "n2draw.h"
#include "n2drawmanager.h"
#include "n2connection.h"

bool n2Connection::connectComponent(IManager * manager, n2Point & p_src, n2Point & p_dst)
{
	bool res = false;
	size_t u, i, num;
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
				{
					if (p_src.x == p_dst.x && p_src.y != p_dst.y)
					{
						//vert connection
						if (p_src.y > p_dst.y)
						{
							for (u = p_dst.y + 1; u < p_src.y; u++)
							{
								v = manager->getObj(p_src.x, u);
								if (v == nullptr)
								{
									v = new nnObjWire(eWire::noWire);
									res=manager->addObj(p_src.x, u, v);
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
									res=manager->addObj(p_src.x, u, v);
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
					}
					else
						if (p_src.y == p_dst.y && p_src.x != p_dst.x)
						{
							InnObj *near_src;
							InnObj *near_dst;
							if (p_src.x < p_dst.x)
							{

								if (p_src.y > 1)
								{
									near_src = manager->getObj(p_src.x, p_src.y - 1);
									near_dst = manager->getObj(p_dst.x, p_dst.y - 1);
									if (near_src == nullptr && near_dst == nullptr)
									{
										v = new nnObjWire(eWire::wireAngleDownRight);
										if (manager->addObj(p_src.x, p_src.y - 1, v))
										{
											num = v->getConnections().front();
											for (i = p_src.x + 1; i < p_dst.x; i++)
											{
												v = new nnObjWire(eWire::noWire);
												v->setConnections(num);
												manager->addObj(i, p_src.y - 1, v);
											}
											v = new nnObjWire(eWire::wireAngleDownLeft);
											v->setConnections(num);
											manager->addObj(i, p_src.y - 1, v);
										}
									}
								}
								if (p_src.y < manager->getHeight() - 1)
								{
									near_src = manager->getObj(p_src.x, p_src.y + 1);
									near_dst = manager->getObj(p_dst.x, p_dst.y + 1);
									if (near_src == nullptr && near_dst == nullptr)
									{
										v = new nnObjWire(eWire::wireAngleUpRight);
										if (manager->addObj(p_src.x, p_src.y + 1, v))
										{
											num = v->getConnections().front();
											for (i = p_src.x + 1; i < p_dst.x; i++)
											{
												v = new nnObjWire(eWire::noWire);
												v->setConnections(num);
												manager->addObj(i, p_src.y + 1, v);
											}
											v = new nnObjWire(eWire::wireAngleUpLeft);
											v->setConnections(num);
											manager->addObj(i, p_src.y + 1, v);
										}
									}
									res = true;
								}
							}
							else
							{
							}
						}
				}
			}
		}
	}
	return res;
}