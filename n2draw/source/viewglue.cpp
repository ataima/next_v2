#include <stdio.h>
#include <string>
#include "miniXml.h"
#include "n2drawmanager.h"
#include "viewglue.h"

//TestviewGlue.cpp : T1
nnViewGlue::nnViewGlue(IManager  *_manager) :manager(_manager)
{
	const_x = const_y = width = height = 0;
	unselect();
}

//TestviewGlue.cpp : T3
bool nnViewGlue::unselect()
{
	select_start.x = -1;
	select_start.y = -1;
	select_stop.x = -1;
	select_stop.y = -1;
	status = s_unselect;
	return true;
}


nnViewGlue::~nnViewGlue()
{
	const_x = const_y = width = height = 0;
}


//TestviewGlue.cpp : T2
n2Point nnViewGlue::getCoordPhy(n2Point & logPoint)
{
	n2Point res(0, 0);
	res.x = logPoint.x*const_x;
	res.y = logPoint.y*const_y;
	return res;
}

//TestviewGlue.cpp : T2
n2Point nnViewGlue::getCoordLog(n2Point & phyPoint)
{
	n2Point res(0, 0);
	res.x = phyPoint.x / const_x;
	res.y = phyPoint.y / const_y;
	return res;
}

//TestviewGlue.cpp : T2
bool nnViewGlue::readConfiguration(miniXmlNode *node)
{
	bool res = false;
	miniXmlNode *conf = node->find(X("PHY_MAP"));
	if (conf)
	{
		miniXmlNode *t = conf->find(X("X"));
		if (t)
		{
			const_x = t->getLong();
		}
		else
		{
			phyGlueConfigurationException e;
			throw (e);
		}
		t = conf->find(X("Y"));
		if (t)
		{
			const_y = t->getLong();
		}
		else
		{
			phyGlueConfigurationException e;
			throw (e);
		}
		t = conf->find(X("WIDTH"));
		if (t)
		{
			width = t->getLong();
		}
		else
		{
			phyGlueConfigurationException e;
			throw (e);
		}
		t = conf->find(X("HEIGHT"));
		if (t)
		{
			height = t->getLong();
		}
		else
		{
			phyGlueConfigurationException e;
			throw (e);
		}
		res = true;
	}
	else
	{
		phyGlueConfigurationException e;
		throw (e);
	}
	return res;
}

//TestviewGlue.cpp : T3
bool nnViewGlue::selectStart(size_t xpos, size_t ypos)
{
	bool res = false;
	n2Point p(xpos, ypos);
	selectStart(p);
	return res;
}
//TestviewGlue.cpp : T3
bool nnViewGlue::selectStop(size_t xpos, size_t ypos)
{
	bool res = false;
	n2Point p(xpos, ypos);
	selectStop(p);
	return res;
}
//TestviewGlue.cpp : T3
bool nnViewGlue::selectStart(n2Point pos)
{
	bool res = false;
	if (manager != nullptr)
	{
		size_t log_height = manager->getHeight(); //logic coord
		size_t log_width = manager->getWidth(); //logic coord
		if (pos.x < log_width && pos.y < log_width)
		{
			select_start = pos;
			res = true;
		}
	}
	return res;
}
//TestviewGlue.cpp : T3
bool nnViewGlue::selectStop(n2Point pos)
{
	bool res = false;
	if (manager != nullptr)
	{
		size_t log_height = manager->getHeight(); //logic coord
		size_t log_width = manager->getWidth(); //logic coord
		if (pos.x < log_width && pos.y < log_width)
		{
			select_stop = pos;
			res = true;
		}
	}
	return res;
}

//TestviewGlue.cpp : T1 T3
bool nnViewGlue::getSelectAreaPhy(size_t & width, size_t & height)
{
	bool res = false;
	if (select_start != select_stop)
	{
		if (isStartValid() && isStopValid())
		{
			n2Point phyStart = getCoordPhy(select_start);
			n2Point phyStop = getCoordPhy(select_stop);
			if (phyStop.x > phyStart.x)
			{
				width = phyStop.x - phyStart.x;
			}
			else
			{
				width = phyStart.x - phyStop.x;
			}
			if (phyStop.y > phyStart.y)
			{
				height = phyStop.y - phyStart.y;
			}
			else
			{
				height = phyStart.y - phyStop.y;
			}
			res = true;
		}
		else
			if (isStartValid() && !isStopValid())
			{
				width = const_x;
				height = const_y;
				res = true;
			}
			else
			{
				width = height = 0;
				res = true;
			}
	}
	else
	{
		if (isStartValid())
		{
			width = const_x;
			height = const_y;
			res = true;
		}
		else
		{
			width = height = 0;
			res = true;
		}
	}
	return res;
}

//TestviewGlue.cpp : T1 T3
bool nnViewGlue::getSelectStartPhy(size_t & x, size_t & y)
{
	n2Point p(-1, -1);
	bool res = false;
	if (isStartValid() && isStopValid())
	{
		p = select_start.intersect(select_stop);
		p = getCoordPhy(p);
		x = p.x;
		y = p.y;
		res = true;
	}
	else
		if (isStartValid() && !isStopValid())
		{
			p = select_start;
			p = getCoordPhy(p);
			x = p.x;
			y = p.y;
			res = true;
		}
		else
			if (!isStartValid() && isStopValid())
			{
				p = select_stop;
				p = getCoordPhy(p);
				x = p.x;
				y = p.y;
				res = true;

			}
	return res;
}

bool nnViewGlue::handlerMouseMove(nn_mouse_buttons buttons, n2Point phyPoint)
{


	bool res = true;
	if (status == start_activate)
		status = start_resize;
	if (status == start_resize)
	{
		n2Point p = getCoordLog(phyPoint);
		if (p != select_stop)
		{
			select_stop = p;
		}
	}

	return res;
}


bool nnViewGlue::handlerEscapeButton(void)
{
	status = s_unselect;
	return unselect();
}


bool nnViewGlue::handlerMouseButtonDown(nn_mouse_buttons buttons, n2Point phyPoint)
{
	bool res = true;
	if (status == s_unselect)
		status = start_activate;
	selectStart(getCoordLog(phyPoint));
	select_stop = select_start;
	return res;
}

bool nnViewGlue::handlerMouseButtonUp(nn_mouse_buttons buttons, n2Point phyPoint)
{

	bool res = true;
	if (status == start_resize)
	{
		status = stop_resize;
		n2Point p = getCoordLog(phyPoint);
		if (p != select_stop)
		{
			select_stop = p;
		}
		status = selected;
	}
	else
		if (status == start_activate)
		{
			n2Point p = getCoordLog(phyPoint);
			if (p != select_stop)
			{
				select_stop = p;
			}
			status = selected;
		}

	return res;
}
