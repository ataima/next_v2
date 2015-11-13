#include "n2draw.h"

const std::wstring nnObj::toString(void) const
{
	std::wostringstream s;
	switch (v_context)
	{
	case objNone:  s << L"objNone"; break;
	case objWire: s << L"objWire"; break;
	case objContact: s << L"objContact"; break;
	case objCoil: s << L"objCoil"; break;
	default: s << L"unknow"; break;
	}
	return s.str();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const  std::wstring nnObjPos::toString(void) const
{
	std::wostringstream s;
	s << L"X:" << v_Xpos << L" - Y:" <<
		v_Ypos << " - " << nnObj::toString();
	return s.str();
}



eWireDirection nnObjPos::getDirection(InnObj & b)
{
	eWireDirection res = direction_unknow;
	if (v_Xpos == b.getXpos())
	{
		//same row
		if (v_Ypos < b.getYpos())
			res = wire_from_down;
		else
			res = wire_from_up;
	}
	else
		if (v_Ypos == b.getYpos())
		{
			//same col
			if (v_Xpos < b.getXpos())
				res = wire_from_right;
			else
				res = wire_from_left;
		}
	return res;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
size_t  nnObjWire::getNum(nnObjWire & b)
{
	if (b.getNum() != 0 && getNum() == 0)
		return b.getNum();
	else
		if (b.getNum() == 0 && getNum() == 0)
		{
			return ++uid_num;
		}
	return 0;
}


bool nnObjWire::connect(InnObj * pb)
{
	bool res = false;
	if (pb != nullptr)
	{
		ObjContext context = pb->getContext();
		if (context == ObjContext::objWire)
		{
			nnObjWire *pW = dynamic_cast<nnObjWire *>(pb);
			if (pW != nullptr)
			{
				nnObjWire & bw = *pW;
				eWireDirection w = getDirection(bw);
				switch (w)
				{
				case wire_from_up:
				{
					res = connectFromUp(bw);
					if (res)
						res = bw.connectFromDown(*this);
				}
				break;
				case wire_from_down:
				{
					res = connectFromDown(bw);
					if (res)
						res = bw.connectFromUp(*this);
				}
				break;
				case wire_from_left:
				{
					res = connectFromLeft(bw);
					if (res)
						res = bw.connectFromRight(*this);
				}
				break;
				case wire_from_right:
				{
					res = connectFromUp(bw);
					if (res)
						res = bw.connectFromDown(*this);
				}
				break;
				}
			}
		}
		else
			if (context == ObjContext::objContact || context == ObjContext::objCoil)
			{
				nnObjComponent *pW = dynamic_cast<nnObjComponent *>(pb);
				if (pW != nullptr)
				{
					eWireDirection w = getDirection(*pb);
					switch (w)
					{
					case wire_from_up:
					{
						nnObjWire  & bw = pW->getDownWire();
						res = connectFromUp(bw);
						if (res)
							res = bw.connect(this);
					}
					break;
					case wire_from_down:
					{
						nnObjWire  & bw = pW->getUpWire();
						res = connectFromDown(bw);
						if (res)
							res = bw.connect(this);
					}
					break;
					}
				}
			}
	}
	return res;
}



bool nnObjWire::connectFromUp(nnObjWire & b)
{
	bool res = false;
	switch (getWire())
	{
	case eWire::noWire:
	{
		setWire(eWire::wireVertical);
		size_t num = getNum(b);
		if (num)
		{
			setNum(num);
			b.setNum(num);
			res = true;
		}
	}
	break;
	case eWire::wireHorizzontal:
		setNum(b.getNum());
		setWire(eWire::wireTHorizUp);
		res = true;
		break;
	case eWire::wireTVertRight:
	case eWire::wireTVertLeft:
	case eWire::wireCross:
	case eWire::wireTHorizUp:
	case eWire::wireAngleUpRight:
	case eWire::wireAngleUpLeft:
	case eWire::wireVertical:
		setNum(b.getNum());
		res = true;
		break;
	case eWire::wireAngleDownRight:
		setNum(b.getNum());
		setWire(eWire::wireTVertRight);
		res = true;
		break;
	case eWire::wireAngleDownLeft:
		setNum(b.getNum());
		setWire(eWire::wireTVertLeft);
		res = true;
		break;
	case eWire::wireTHorizDown:
		setNum(b.getNum());
		setWire(eWire::wireCross);
		res = true;
		break;
	}
	return res;
}




bool nnObjWire::disconnectFromUp(nnObjWire & b)
{
	bool res = false;
	switch (getWire())
	{
	case eWire::wireTHorizDown:
	case eWire::wireAngleDownLeft:
	case eWire::wireAngleDownRight:
	case eWire::wireHorizzontal:
		res = true;
		break;
	case eWire::wireAngleUpLeft:
	case eWire::wireAngleUpRight:
	case eWire::wireVertical:
		setWire(eWire::noWire);
	case eWire::noWire:
		setNum(0);
		res = true;
		break;
	case eWire::wireTVertRight:
		setWire(wireAngleDownRight);
		res = true;
		break;

	case eWire::wireTVertLeft:
		setWire(wireAngleDownLeft);
		res = true;
		break;
	case eWire::wireCross:
		setWire(wireTHorizDown);
		res = true;
		break;
	case eWire::wireTHorizUp:
		setWire(wireHorizzontal);
		res = true;
		break;
	}
	return res;
}



bool nnObjWire::connectFromDown(nnObjWire & b)
{
	bool res = false;
	switch (getWire())
	{
	case eWire::noWire:
	{
		setWire(eWire::wireVertical);
		size_t num = getNum(b);
		if (num)
		{
			setNum(num);
			b.setNum(num);
			res = true;
		}
	}
	break;
	case eWire::wireHorizzontal:
		setNum(b.getNum());
		setWire(eWire::wireTHorizDown);
		res = true;
		break;
	case eWire::wireTVertRight:
	case eWire::wireTVertLeft:
	case eWire::wireCross:
	case eWire::wireTHorizDown:
	case eWire::wireAngleDownRight:
	case eWire::wireAngleDownLeft:
	case eWire::wireVertical:
		setNum(b.getNum());
		res = true;
		break;
	case eWire::wireAngleUpRight:
		setNum(b.getNum());
		setWire(eWire::wireTVertRight);
		res = true;
		break;
	case eWire::wireAngleUpLeft:
		setNum(b.getNum());
		setWire(eWire::wireTVertLeft);
		res = true;
		break;
	case eWire::wireTHorizUp:
		setNum(b.getNum());
		setWire(eWire::wireCross);
		res = true;
		break;
	}
	return res;
}



bool nnObjWire::disconnectFromDown(nnObjWire & b)
{
	bool res = false;
	switch (getWire())
	{
	case eWire::wireTHorizUp:
	case eWire::wireAngleUpLeft:
	case eWire::wireAngleUpRight:
	case eWire::wireHorizzontal:
		res = true;
		break;
	case eWire::wireAngleDownLeft:
	case eWire::wireAngleDownRight:
	case eWire::wireVertical:
		setWire(eWire::noWire);
	case eWire::noWire:
		setNum(0);
		res = true;
		break;
	case eWire::wireTVertRight:
		setWire(wireAngleUpRight);
		res = true;
		break;

	case eWire::wireTVertLeft:
		setWire(wireAngleUpLeft);
		res = true;
		break;
	case eWire::wireCross:
		setWire(wireTHorizUp);
		res = true;
		break;
	case eWire::wireTHorizDown:
		setWire(wireHorizzontal);
		res = true;
		break;
	}

	return res;
}



bool nnObjWire::connectFromLeft(nnObjWire & b)
{
	bool res = false;
	switch (getWire())
	{
	case eWire::noWire:
	{
		setWire(eWire::wireHorizzontal);
		size_t num = getNum(b);
		if (num)
		{
			setNum(num);
			b.setNum(num);
			res = true;
		}
	}
	break;
	case eWire::wireVertical:
		setNum(b.getNum());
		setWire(eWire::wireTVertLeft);
		res = true;
		break;
	case eWire::wireTHorizDown:
	case eWire::wireTHorizUp:
	case eWire::wireCross:
	case eWire::wireTVertLeft:
	case eWire::wireAngleUpLeft:
	case eWire::wireAngleDownLeft:
	case eWire::wireHorizzontal:
		setNum(b.getNum());
		res = true;
		break;
	case eWire::wireAngleUpRight:
		setNum(b.getNum());
		setWire(eWire::wireTHorizUp);
		res = true;
		break;
	case eWire::wireAngleDownRight:
		setNum(b.getNum());
		setWire(eWire::wireTHorizDown);
		res = true;
		break;
	case eWire::wireTVertRight:
		setNum(b.getNum());
		setWire(eWire::wireCross);
		res = true;
		break;
	}
	return res;
}


bool nnObjWire::disconnectFromLeft(nnObjWire & b)
{
	bool res = false;
	switch (getWire())
	{
	case eWire::wireAngleUpLeft:
	case eWire::wireAngleDownLeft:
	case eWire::wireTVertLeft:
	case eWire::wireVertical:
		res = true;
		break;
	case eWire::wireAngleDownRight:
	case eWire::wireAngleUpRight:
	case eWire::wireHorizzontal:
		setWire(eWire::noWire);
	case eWire::noWire:
		setNum(0);
		res = true;
		break;
	case eWire::wireTHorizDown:
		setWire(eWire::wireAngleDownLeft);
		break;
	case eWire::wireTHorizUp:
		setWire(eWire::wireAngleUpLeft);
		break;
	case eWire::wireCross:
		setWire(eWire::wireTVertLeft);
		res = true;
		break;
	case eWire::wireTVertRight:
		setWire(eWire::wireVertical);
		res = true;
		break;
	}
	return res;
}


bool nnObjWire::connectFromRight(nnObjWire & b)
{
	bool res = false;
	switch (getWire())
	{
	case eWire::noWire:
	{
		setWire(eWire::wireHorizzontal);
		size_t num = getNum(b);
		if (num)
		{
			setNum(num);
			b.setNum(num);
			res = true;
		}
	}
	break;
	case eWire::wireVertical:
		setNum(b.getNum());
		setWire(eWire::wireTVertRight);
			res = true;
		break;
	case eWire::wireTHorizDown:
	case eWire::wireTHorizUp:
	case eWire::wireCross:
	case eWire::wireTVertRight:
	case eWire::wireAngleUpRight:
	case eWire::wireAngleDownRight:
	case eWire::wireHorizzontal:
		setNum(b.getNum());
		res = true;
		break;
	case eWire::wireAngleUpLeft:
		setNum(b.getNum());
		setWire(eWire::wireTHorizUp);
			res = true;
		break;
	case eWire::wireAngleDownLeft:
		setNum(b.getNum());
		setWire(eWire::wireTHorizDown);
			res = true;
		break;
	case eWire::wireTVertLeft:
		setNum(b.getNum());
		setWire(eWire::wireCross);
			res = true;
		break;
	}
	return res;
}




bool nnObjWire::disconnectFromRight(nnObjWire & b)
{
	bool res = false;
	switch (getWire())
	{
	case eWire::wireAngleUpLeft:
	case eWire::wireHorizzontal:
	case eWire::wireAngleDownLeft:
		setWire(eWire::noWire);
	case eWire::noWire:
		setNum(0);
		res = true;
		break;

	case eWire::wireTVertRight:
	case eWire::wireAngleUpRight:
	case eWire::wireAngleDownRight:
	case eWire::wireVertical:
		res = true;
		break;
	case eWire::wireTHorizDown:
		setWire(eWire::wireAngleDownLeft);
		res = true;
		break;
	case eWire::wireTHorizUp:
		setWire(eWire::wireAngleUpLeft);
		res = true;
		break;
	case eWire::wireCross:
		setWire(eWire::wireTVertLeft);
		res = true;
		break;
	case eWire::wireTVertLeft:
		setWire(eWire::wireVertical);
		res = true;
		break;
	}
	return res;
}


size_t nnObjWire::uid_num = 2;

const  std::wstring nnObjWire::toString(void) const
{
	std::wostringstream s;
	s << "N:" << v_num << " - " << nnObjPos::toString() << " - ";
	switch (v_wire)
	{
	case noWire: s << L"noWire"; break;
	case wireHorizzontal: s << L"wireHorizzontal"; break;
	case wireVertical: s << L"wireVertical"; break;
	case wireAngleUpRight: s << L"wireAngleUpRight"; break;
	case wireAngleUpLeft: s << L"wireAngleUpLeft"; break;
	case wireAngleDownRight: s << L"wireAngleDownRight"; break;
	case wireAngleDownLeft: s << L"wireAngleDownLeft"; break;
	case wireTHorizDown: s << L"wireTHorizDown"; break;
	case wireTHorizUp: s << L"wireTHorizUp"; break;
	case wireTVertRight: s << L"wireTVertRight"; break;
	case wireTVertLeft:s << L"wireTVertLeft"; break;
	case wireCross:s << L"wireCross"; break;
	default: s << L"unknow"; break;
	}
	return s.str();
}



bool nnObjWire::disconnect(InnObj * pb)
{
	bool res = false;
	if (pb != nullptr)
	{
		ObjContext context = pb->getContext();
		if (context == ObjContext::objWire)
		{
			nnObjWire *pW = dynamic_cast<nnObjWire *>(pb);
			if (pW != nullptr)
			{
				nnObjWire & bw = *pW;
				eWireDirection w = getDirection(bw);
				switch (w)
				{
				case wire_from_up:
				{
					res = disconnectFromUp(bw);
					if (res)
						res = bw.disconnectFromDown(*this);
				}
				break;
				case wire_from_down:
				{
					res = disconnectFromDown(bw);
					if (res)
						res = bw.disconnectFromUp(*this);
				}
				break;
				case wire_from_left:
				{
					res = disconnectFromLeft(bw);
					if (res)
						res = bw.disconnectFromRight(*this);
				}
				break;
				case wire_from_right:
				{
					res = disconnectFromUp(bw);
					if (res)
						res = bw.disconnectFromDown(*this);
				}
				break;
				}
			}
		}
		else
			if (context == ObjContext::objContact || context == ObjContext::objCoil)
			{
				nnObjComponent *pW = dynamic_cast<nnObjComponent *>(pb);
				if (pW != nullptr)
				{
					eWireDirection w = getDirection(*pb);
					switch (w)
					{
					case wire_from_up:
					{
						nnObjWire  & bw = pW->getDownWire();
						res = disconnectFromUp(bw);
						if (res)
							res = bw.disconnect(this);
					}
					break;
					case wire_from_down:
					{
						nnObjWire  & bw = pW->getUpWire();
						res = disconnectFromDown(bw);
						if (res)
							res = bw.disconnect(this);
					}
					break;
					}
				}
			}
	}
	return res;
}

void nnObjWire::setUpNeighbour(InnObj *obj)
{
	if (obj != NULL)
	{
		connect(obj);
	}
}

void nnObjWire::setDownNeighbour(InnObj *obj)
{
	if (obj != NULL)
	{
		connect(obj);
	}
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const  std::wstring nnObjComponent::toString(void) const
{
	std::wostringstream s;
	s << nnObjPos::toString() << std::endl;
	s << "UP   : " << up_wire.toString() << std::endl;
	s << "DOWN : " << down_wire.toString() << std::endl;
	return s.str();
}

bool nnObjComponent::connect(InnObj * from)
{
	eWireDirection res = getDirection(*from);
	if (res == eWireDirection::wire_from_left)
		return false;
	else if (res == eWireDirection::wire_from_right)
		return false;
	else
		if (res == eWireDirection::wire_from_up)
			return up_wire.connect(from);
		else
			if (res == eWireDirection::wire_from_up)
				return down_wire.connect(from);
	return false;
}



void nnObjComponent::setUpNeighbour(InnObj *obj)
{
	if (obj == nullptr && up_neighbours != nullptr)
	{
		up_wire.disconnect(up_neighbours);
	}
	up_neighbours = obj;
	if (obj != nullptr)
	{
		up_wire.connect(obj);
	}

}

void nnObjComponent::setDownNeighbour(InnObj *obj)
{
	if (obj == nullptr && down_neighbours != nullptr)
	{
		down_wire.disconnect(down_neighbours);
	}
	down_neighbours = obj;
	if (obj != nullptr)
	{
		down_wire.connect(obj);
	}
}


void nnObjComponent::setXpos(size_t pX)
{
	nnObjPos::setXpos(pX);
	up_wire.setXpos(pX);
	down_wire.setXpos(pX);

}

void nnObjComponent::setYpos(size_t pY)
{
	nnObjPos::setYpos(pY);
	up_wire.setYpos(pY);
	down_wire.setYpos(pY);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const  std::wstring nnPowerUp::toString(void) const
{
	std::wostringstream s;
	s << nnObjWire::toString() << " - ";
	s << " Power Up" << std::endl;
	return s.str();
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const  std::wstring nnPowerDown::toString(void) const
{
	std::wostringstream s;
	s << nnObjWire::toString() << " - ";
	s << " Power Down" << std::endl;
	return s.str();
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const  std::wstring nnObjContact::toString(void) const
{
	std::wostringstream s;
	s << "CONTACT : " << std::endl;
	s << nnObjComponent::toString();
	return s.str();
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const  std::wstring nnObjCoil::toString(void) const
{
	std::wostringstream s;
	s << "COIL : " << std::endl;
	s << nnObjComponent::toString();
	return s.str();
}