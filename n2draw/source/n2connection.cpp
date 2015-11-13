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
				{
					size_t u, src_up, src_dw, dst_up, dst_dw;
					src_up = c_src->getUpWire().getNum();
					src_dw = c_src->getDownWire().getNum();
					dst_up = c_dst->getUpWire().getNum();
					dst_dw = c_dst->getDownWire().getNum();
					if (p_src.x == p_dst.x && p_src.y != p_dst.y)
					{
						//vert connection

						if (p_src.y > p_dst.y)
						{
							if (src_up != 0 && dst_dw == 0)
							{
								for (u = p_dst.y + 1; u < p_src.y; u++)
								{
									v = manager->getObj(p_src.x, u);
									if (v == nullptr)
									{
										v = new nnObjWire(eWire::noWire);
										manager->addObj(p_src.x, u, v);
									}
									else
										if (v->isComponent())
										{
											positionBusyException e(p_src.x, u);
											throw (e);
										}
								}
								res = true;
							}
							else
								if (src_up == 0 && dst_dw != 0)
								{
									for (u = p_dst.y - 1; u < p_src.y; u--)
									{
										v = manager->getObj(p_src.x, u);
										if (v == nullptr)
										{
											v = new nnObjWire(eWire::noWire);
											manager->addObj(p_src.x, u, v);
										}
										else
											if (v->isComponent())
											{
												positionBusyException e(p_src.x, u);
												throw (e);
											}
									}
									res = true;
								}
								else
									if (src_up == 0 && dst_dw == 0)
									{

										for (u = p_dst.y; u < p_src.y; u++)
										{
											v = manager->getObj(p_src.x, u);
											if (v == nullptr)
											{
												v = new nnObjWire(eWire::noWire);
												manager->addObj(p_src.x, u, v);
											}
											else
												if (u > p_dst.y && v->isComponent())
												{
													positionBusyException e(p_src.x, u);
													throw (e);
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
								for (u = p_src.y + 1; u < p_dst.y; u++)
								{
									v = manager->getObj(p_src.x, u);
									if (v == nullptr)
									{
										v = new nnObjWire(eWire::noWire);
										manager->addObj(p_src.x, u, v);
									}
									else
										if (v->isComponent())
										{
											positionBusyException e(p_src.x, u);
											throw (e);
										}
								}
								res = true;
							}
							else
								if (src_dw == 0 && dst_up != 0)
								{
									for (u = p_dst.y - 1; u > p_src.y; u--)
									{
										v = manager->getObj(p_src.x, u);
										if (v == nullptr)
										{
											v = new nnObjWire(eWire::noWire);
											manager->addObj(p_src.x, u, v);
										}
										else
											if (v->isComponent())

											{
												positionBusyException e(p_src.x, u);
												throw (e);
											}
									}
									res = true;
								}
								else
									if (src_dw == 0 && dst_up == 0)
									{

										for (u = p_src.y; u < p_dst.y; u++)
										{
											v = manager->getObj(p_src.x, u);
											if (v == nullptr)
											{
												v = new nnObjWire(eWire::noWire);
												manager->addObj(p_src.x, u, v);
											}
											else
												if (u > p_src.y && v->isComponent())
												{
													positionBusyException e(p_src.x, u);
													throw (e);
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
							InnObj *near_src ;
							InnObj *near_dst;
							if(p_src.x < p_dst.x)
							{

								if (p_src.y > 1)
								{
									near_src = manager->getObj(p_src.x, p_src.y - 1);
									near_dst = manager->getObj(p_dst.x, p_dst.y - 1);
									if (near_src == nullptr && near_dst == nullptr)
									{
										size_t i;
										v = new nnObjWire(eWire::wireAngleDownRight);
										manager->addObj(p_src.x, p_src.y - 1,v);
										for (i = p_src.x + 1; i < p_dst.x-1; i++)
										{
											v = new nnObjWire(eWire::noWire);
											manager->addObj(i, p_src.y - 1, v);
										}
										v = new nnObjWire(eWire::wireAngleDownLeft);
										manager->addObj(i, p_src.y - 1, v);
									}
								}
								if (p_src.y < manager->getHeight() - 1)
								{
									near_src = manager->getObj(p_src.x, p_src.y + 1);
									near_dst = manager->getObj(p_dst.x, p_dst.y + 1);
									if (near_src == nullptr && near_dst == nullptr)
									{
										size_t i;
										v = new nnObjWire(eWire::wireAngleUpRight);
										manager->addObj(p_src.x, p_src.y + 1, v);
										for (i = p_src.x + 1; i < p_dst.x - 1; i++)
										{
											v = new nnObjWire(eWire::noWire);
											manager->addObj(i, p_src.y + 1, v);
										}
										v = new nnObjWire(eWire::wireAngleUpLeft);
										manager->addObj(i, p_src.y + 1, v);
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