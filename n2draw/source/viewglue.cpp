#include <stdio.h>
#include <string>
#include "miniXml.h"
#include "n2drawmanager.h"
#include "viewglue.h"

nnViewGlue::nnViewGlue(IManager  *_manager):manager(_manager)
{
	const_x = const_y = width = height = 0;
	unselect();
}


bool nnViewGlue::unselect()
{
	select_start.x = -1;
	select_start.y = -1;
	select_stop.x = -1;
	select_stop.y = -1;
	return true;
}


nnViewGlue::~nnViewGlue()
{
	const_x = const_y = width = height = 0;
}

n2Point nnViewGlue::getCoordPhy(n2Point & logPoint)
{
	n2Point res(0, 0);
	res.x = logPoint.x*const_x;
	res.y = logPoint.y*const_y;
	return res;
}

n2Point nnViewGlue::getCoordLog(n2Point & phyPoint)
{
	n2Point res(0, 0);
	res.x = phyPoint.x / const_x;
	res.y = phyPoint.y / const_y;
	return res;
}

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
	}
	else
	{
		phyGlueConfigurationException e;
		throw (e);
	}
	return res;
}

bool nnViewGlue::selectStart(size_t xpos, size_t ypos)
{
	bool res = false;
	n2Point p(xpos, ypos);
	selectStart(p);
	return res;
}

bool nnViewGlue::selectStop(size_t xpos, size_t ypos)
{
	bool res = false;
	n2Point p(xpos, ypos);
	selectStop(p);
	return res;
}

bool nnViewGlue::selectStart(n2Point pos)
{
	bool res = false;
	if (manager != nullptr)
	{
		size_t log_height=manager->getHeight(); //logic coord
		size_t log_width = manager->getWidth(); //logic coord
		if (pos.x < log_width && pos.y < log_width)
		{
			select_start = pos;
		}
	}
	return res;
}

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
		}
	}
	return res;
}


bool nnViewGlue::getSelectArea(size_t & width, size_t & height)
{
	bool res = false;
	if (select_start != select_stop)
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
	}
	else
	{
			width  = const_x;
			height = const_y;		
	}
	return res;
}

bool nnViewGlue::handlerMouseMove(nn_mouse_buttons buttons, n2Point phyPoint)
{
	bool res = false;
	return res;
}

bool nnViewGlue::handlerMouseButtonDown(nn_mouse_buttons buttons, n2Point phyPoint)
{
	bool res = false;
	return res;
}

bool nnViewGlue::handlerMouseButtonUp(nn_mouse_buttons buttons, n2Point phyPoint)
{
	bool res = false;
	return res;
}
