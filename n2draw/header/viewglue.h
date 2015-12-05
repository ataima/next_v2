#ifndef VIEW_CONNECTION_HEADER
#define VIEW_CONNECTION_HEADER


#include "n2draw.h"

typedef enum tag_mouse_button_def
{
	nn_m_button_unknow = 0,
	nn_m_button_left = 1,
	nn_m_button_right = 2,
	nn_m_button_middle = 4
} nn_mouse_buttons;


class miniXmlNode;

class IViewGlue
{
public:
	virtual n2Point getCoordPhy(n2Point & logPoint) = 0;
	virtual n2Point getCoordLog(n2Point & phyPoint) = 0;
	virtual bool readConfiguration(miniXmlNode *node) = 0;
	virtual bool selectStart(size_t xpos, size_t ypos)=0;
	virtual bool selectStop(size_t xpos1, size_t ypos1) = 0;
	virtual bool selectStart(n2Point pos) = 0;
	virtual bool selectStop(n2Point pos1) = 0;
	virtual bool select(n2Point pos1, n2Point pos2) = 0;
	virtual bool handlerMouseMove(nn_mouse_buttons buttons, n2Point phyPoint) = 0;
	virtual bool handlerMouseButtonDown(nn_mouse_buttons buttons, n2Point phyPoint) = 0;
	virtual bool handlerMouseButtonUp(nn_mouse_buttons buttons, n2Point phyPoint) = 0;
	virtual bool handlerEscapeButton(void) = 0;
	virtual bool unselect()=0;
	virtual bool getSelectAreaPhy(size_t & width, size_t & height)=0;
	virtual bool getSelectStartPhy(size_t & x, size_t & y)=0;
	virtual bool isStartValid(void) = 0;
	virtual bool isStopValid(void) = 0;
	virtual IManager *getManager(void) = 0;
	virtual void setManager(IManager *mn) = 0;
	virtual bool getSelectArea(n2Point &start,n2Point &stop) = 0;
};


class nnViewGlue
	:public IViewGlue
{


	typedef enum tag_status_select
	{
		s_unselect,
		start_activate,
		start_resize,
		stop_resize,
		selected

	}status_select;


	n2Point select_start;
	n2Point select_stop;
	size_t const_x;
	size_t const_y;
	size_t width;
	size_t height;

	IManager  *manager;
	status_select status;


public:
	nnViewGlue(IManager  *);
	~nnViewGlue();
	n2Point getCoordPhy(n2Point & logPoint);
	n2Point getCoordLog(n2Point & phyPoint);
	bool readConfiguration(miniXmlNode *node);
	bool selectStart(size_t xpos, size_t ypos);
	bool selectStop(size_t xpos1, size_t ypos1);
	bool selectStart(n2Point pos);
	bool selectStop(n2Point pos);
	bool unselect();
	bool getSelectAreaPhy(size_t & width, size_t & height);
	bool getSelectStartPhy(size_t & x, size_t & y);
	bool handlerMouseMove(nn_mouse_buttons buttons, n2Point phyPoint);
	bool handlerMouseButtonDown(nn_mouse_buttons buttons, n2Point phyPoint);
	bool handlerMouseButtonUp(nn_mouse_buttons buttons, n2Point phyPoint);
	bool handlerEscapeButton(void);
	inline bool isStartValid(void) { return select_start != -1; }
	inline bool isStopValid(void) { return select_stop != -1; }
	inline IManager *getManager(void) { return manager; }
	inline void setManager(IManager *mn) { manager=mn; }
	inline bool select(n2Point pos1, n2Point pos2) { return selectStart(pos1) && selectStop(pos2);}
	inline bool getSelectArea(n2Point &start, n2Point &stop) { start = select_start; stop = select_stop; return true; }
};


// no conf phy view 
class phyGlueConfigurationException
	:public std::runtime_error
{
public:
	explicit phyGlueConfigurationException(void) throw()
		:runtime_error("phyGlueConfigurationException") {}

};

#endif




