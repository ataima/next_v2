#include "n2draw.h"
#include "n2drawmanager.h"
#include "n2connection.h"

bool n2Connection::connectComponent(IManager * manager, n2Point p_src, n2Point p_dst)
{
	bool res = false;
	if (manager != nullptr)
	{
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
					size_t u, src_up, src_dw, dst_up, dst_dw;
					src_up = c_src->getUpWire().getNum();
					src_dw = c_src->getDownWire().getNum();
					dst_up = c_dst->getUpWire().getNum();
					dst_dw = c_dst->getDownWire().getNum();
					if (p_src.x == p_dst.x && p_src.y != p_dst.y)
					{
						//vert connection
						if (p_src.y < p_dst.y)
						{
							if (src_up != 0 && dst_dw == 0)
							{
								for (u = p_src.y + 1; u < p_dst.y; u++)
								{
									InnObj *v = manager->getObj(p_src.x, u);
									if (v == nullptr)
									{
										InnObj *w = new nnObjWire(eWire::noWire);
										manager->addObj(p_src.x, u, w);
									}
								}
								res = true;
							}
							else
								if (src_up == 0 && dst_dw != 0)
								{
									for (u = p_dst.y - 1; u < p_src.y; u--)
									{
										InnObj *v = manager->getObj(p_src.x, u);
										if (v == nullptr)
										{
											InnObj *w = new nnObjWire(eWire::noWire);
											manager->addObj(p_src.x, u, w);
										}
									}
									res = true;
								}
								else
									if (src_up == 0 && dst_dw == 0)
									{

										for (u = p_src.y; u < p_dst.y; u++)
										{
											InnObj *v = manager->getObj(p_src.x, u);
											if (v == nullptr)
											{
												InnObj *w = new nnObjWire(eWire::noWire);
												manager->addObj(p_src.x, u, w);
											}
										}
										res = true;
									}
									else
									{
										wireConnectionException e(src_up, dst_dw);
										throw (e);
									}
						}
						else
						{
							if (src_dw != 0 && dst_up == 0)
							{
								for (u = p_dst.y + 1; u < p_src.y; u++)
								{
									InnObj *v = manager->getObj(p_src.x, u);
									if (v == nullptr)
									{
										InnObj *w = new nnObjWire(eWire::noWire);
										manager->addObj(p_src.x, u, w);
									}
								}
								res = true;
							}
							else
								if (src_dw == 0 && dst_up != 0)
								{
									for (u = p_src.y - 1; u < p_src.y; u--)
									{
										InnObj *v = manager->getObj(p_src.x, u);
										if (v == nullptr)
										{
											InnObj *w = new nnObjWire(eWire::noWire);
											manager->addObj(p_src.x, u, w);
										}
									}
									res = true;
								}
								else
									if (src_dw == 0 && dst_up == 0)
									{

										for (u = p_dst.y; u < p_src.y; u++)
										{
											InnObj *v = manager->getObj(p_src.x, u);
											if (v == nullptr)
											{
												InnObj *w = new nnObjWire(eWire::noWire);
												manager->addObj(p_src.x, u, w);
											}
										}
										res = true;
									}
									else
									{
										wireConnectionException e(src_dw, dst_up);
										throw (e);
									}
						}
					}
					else
						if (p_src.y == p_dst.y && p_src.x != p_dst.x)
						{

						}

				}
			}
		}
	}
	return res;
}