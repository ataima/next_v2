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



eWireDirection nnObjPos::getDirection(InnObj * pb)
{
	eWireDirection res = direction_unknow;
	if (pb == nullptr)return res;
	if (v_Xpos == pb->getXpos())
	{
		//same row
		if (v_Ypos < pb->getYpos())
			res = wire_from_down;
		else
			res = wire_from_up;
	}
	else
		if (v_Ypos == pb->getYpos())
		{
			//same col
			if (v_Xpos < pb->getXpos())
				res = wire_from_right;
			else
				res = wire_from_left;
		}
	return res;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool nnObjConn::powerConnect(size_t num)
{
	bool res = false;
	if (num == 1)
	{
		if (isComponent())
		{
			nnObjComponent *cc = dynamic_cast<nnObjComponent *>(this);
			if (cc != nullptr)
			{
				res = cc->connectFromUp(1);
			}
		}
		else
		{
			nnObjWire *ww = dynamic_cast<nnObjWire *>(this);
			if (ww != nullptr)
			{
				res = ww->connectFromUp(1);
			}
		}
	}
	else
		if (num == 2)
		{
			if (isComponent())
			{
				nnObjComponent *cc = dynamic_cast<nnObjComponent *>(this);
				if (cc != nullptr)
				{
					res = cc->connectFromDown(2);
				}
			}
			else
			{
				nnObjWire *ww = dynamic_cast<nnObjWire *>(this);
				if (ww != nullptr)
				{
					res = ww->connectFromDown(2);
				}
			}
		}
	return res;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




bool nnObjWire::connect(InnObj * pb)
{
	bool res = false;
	if (pb != nullptr)
	{
		ObjContext context = pb->getContext();
		if (context == ObjContext::objWire)
		{
			nnObjWire *wire = dynamic_cast<nnObjWire *>(pb);
			eWireDirection w = getDirection(pb);
			switch (w)
			{
			case wire_from_up:
			{
				res = connectFromUp(pb->getConnections().back());
				if(res)
					res=wire->connectFromDown(pb->getConnections().back());
			}
			break;
			case wire_from_down:
			{
				res = connectFromDown(pb->getConnections().front());
				if(res)
					res=wire->connectFromUp(pb->getConnections().front());
			}
			break;
			case wire_from_left:
			{
				res = connectFromLeft(pb->getConnections().front());
				if(res)
					res=wire->connectFromRight(pb->getConnections().front());
			}
			break;
			case wire_from_right:
			{
				res = connectFromRight(pb->getConnections().front());
				if(res)
					res= wire->connectFromLeft(pb->getConnections().front());
			}
			break;
			}

		}
		else
			if (context == ObjContext::objContact || context == ObjContext::objCoil)
			{

				eWireDirection w = getDirection(pb);
				switch (w)
				{
				case wire_from_up:
				{
					res = connectFromUp(pb->getConnections().back());
				}
				break;
				case wire_from_down:
				{
					res = connectFromDown(pb->getConnections().front());
				}
				break;
				}

			}
	}
	return res;
}



bool nnObjWire::connectFromUp(size_t num)
{
	bool res = false;
	switch (getWire())
	{
	case eWire::noWire:
	{
		setWire(eWire::wireVertical);
		if (num)
		{
			setConnections(num);
			res = true;
		}
	}
	break;
	case eWire::wireHorizzontal:
		setConnections(num);
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
		setConnections(num);
		res = true;
		break;
	case eWire::wireAngleDownRight:
		setConnections(num);
		setWire(eWire::wireTVertRight);
		res = true;
		break;
	case eWire::wireAngleDownLeft:
		setConnections(num);
		setWire(eWire::wireTVertLeft);
		res = true;
		break;
	case eWire::wireTHorizDown:
		setConnections(num);
		setWire(eWire::wireCross);
		res = true;
		break;
	}
	return res;
}




bool nnObjWire::disconnectFromUp(size_t num)
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
		setConnections(0);
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



bool nnObjWire::connectFromDown(size_t num)
{
	bool res = false;
	switch (getWire())
	{
	case eWire::noWire:
	{
		setWire(eWire::wireVertical);
		if (num)
		{
			setConnections(num);
			res = true;
		}
	}
	break;
	case eWire::wireHorizzontal:
		setConnections(num);
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
		setConnections(num);
		res = true;
		break;
	case eWire::wireAngleUpRight:
		setConnections(num);
		setWire(eWire::wireTVertRight);
		res = true;
		break;
	case eWire::wireAngleUpLeft:
		setConnections(num);
		setWire(eWire::wireTVertLeft);
		res = true;
		break;
	case eWire::wireTHorizUp:
		setConnections(num);
		setWire(eWire::wireCross);
		res = true;
		break;
	}
	return res;
}



bool nnObjWire::disconnectFromDown(size_t num)
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
		setConnections(0);
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



bool nnObjWire::connectFromLeft(size_t num)
{
	bool res = false;
	switch (getWire())
	{
	case eWire::noWire:
	{
		setWire(eWire::wireHorizzontal);
		if (num)
		{
			setConnections(num);
			res = true;
		}
	}
	break;
	case eWire::wireVertical:
		setConnections(num);
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
		setConnections(num);
		res = true;
		break;
	case eWire::wireAngleUpRight:
		setConnections(num);
		setWire(eWire::wireTHorizUp);
		res = true;
		break;
	case eWire::wireAngleDownRight:
		setConnections(num);
		setWire(eWire::wireTHorizDown);
		res = true;
		break;
	case eWire::wireTVertRight:
		setConnections(num);
		setWire(eWire::wireCross);
		res = true;
		break;
	}
	return res;
}


bool nnObjWire::disconnectFromLeft(size_t num)
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
		setConnections(num);
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


bool nnObjWire::connectFromRight(size_t num)
{
	bool res = false;
	switch (getWire())
	{
	case eWire::noWire:
	{
		setWire(eWire::wireHorizzontal);
		if (num)
		{
			setConnections(num);
			res = true;
		}
	}
	break;
	case eWire::wireVertical:
		setConnections(num);
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
		setConnections(num);
		res = true;
		break;
	case eWire::wireAngleUpLeft:
		setConnections(num);
		setWire(eWire::wireTHorizUp);
		res = true;
		break;
	case eWire::wireAngleDownLeft:
		setConnections(num);
		setWire(eWire::wireTHorizDown);
		res = true;
		break;
	case eWire::wireTVertLeft:
		setConnections(num);
		setWire(eWire::wireCross);
		res = true;
		break;
	}
	return res;
}




bool nnObjWire::disconnectFromRight(size_t num)
{
	bool res = false;
	switch (getWire())
	{
	case eWire::wireAngleUpLeft:
	case eWire::wireHorizzontal:
	case eWire::wireAngleDownLeft:
		setWire(eWire::noWire);
	case eWire::noWire:
		setConnections(0);
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


void nnObjWire::setConnections(size_t n)
{
	if (v_num.size() == 0)
	{
		v_num.push_back(n);
	}
	else
	{
			v_num[0] = n;

	}
}


size_t nnObjConn::uid_num = 2;


const std::wstring nnObjConn::toString(void) const
{
	std::wostringstream s;
	if (v_num.size() > 0)
		s << "N:" << v_num.front() << ":" << v_num.back() << " - " << nnObjPos::toString();
	else
		s << "N:" << "0:0" << " - " << nnObjPos::toString();
	return s.str();
}


const  std::wstring nnObjWire::toString(void) const
{
	std::wostringstream s;
	s << nnObjConn::toString() << " - ";
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
			eWireDirection w = getDirection(pb);
			switch (w)
			{
			case wire_from_up:
			{
				res = disconnectFromUp(pb->getConnections().front());
			}
			break;
			case wire_from_down:
			{
				res = disconnectFromDown(pb->getConnections().front());
			}
			break;
			case wire_from_left:
			{
				res = disconnectFromLeft(pb->getConnections().front());
			}
			break;
			case wire_from_right:
			{
				res = disconnectFromUp(pb->getConnections().front());
			}
			break;
			}
		}
		else
			if (context == ObjContext::objContact || context == ObjContext::objCoil)
			{
				eWireDirection w = getDirection(pb);
				switch (w)
				{
				case wire_from_up:
				{
					res = disconnectFromUp(pb->getConnections().front());
				}
				break;
				case wire_from_down:
				{
					res = disconnectFromDown(pb->getConnections().back());
				}
				break;
				}

			}
	}
	return res;
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool nnObjComponent::disconnect(InnObj *from)
{
	bool result = false;
	eWireDirection res = getDirection(from);
	if (res == eWireDirection::wire_from_up)
	{
		return disconnectFromUp();
	}
	else
		if (res == eWireDirection::wire_from_down)
		{
			return disconnectFromDown();
		}
	return result;
}





bool nnObjComponent::connect(InnObj * from)
{
	bool result = false;
	eWireDirection res = getDirection(from);
	if (res == eWireDirection::wire_from_up)
	{
		if (from->isComponent())
		{
			size_t nconn = nnObjConn::getUI();
			nnObjComponent *comp = dynamic_cast<nnObjComponent *>(from);
			if (comp != nullptr)
			{
				result = comp->connectFromDown(nconn);
				if(result)
					result=connectFromUp(nconn);
			}
		}
		else
		{
				result = connectFromUp(from->getConnections().front());
		}
	}
	else
		if (res == eWireDirection::wire_from_down)
		{
			if (from->isComponent())
			{
				size_t nconn = nnObjConn::getUI();
				nnObjComponent *comp = dynamic_cast<nnObjComponent *>(from);
				if (comp != nullptr)
				{
					result = comp->connectFromUp(nconn);
					if(result)
						result=connectFromDown(nconn);
				}
			}
			else
			{
					result = connectFromDown(from->getConnections().front());
			}

		}
	return result;
}

bool nnObjComponent::connectFromUp(size_t b)
{
	bool res = false;
	if (v_num.size() == 0)
	{
		v_num.push_back(b);
		v_num.push_back(0);
	}
	else
	{
		v_num[0] = b;
	}
	return res;
}

bool nnObjComponent::connectFromDown(size_t b)
{
	bool res = false;
	if (v_num.size() == 0)
	{
		v_num.push_back(0);
		v_num.push_back(b);
		res = true;
	}
	else
		if (v_num.size() == 2)
		{
			v_num[1] = b;
			res = true;
		}
	return res;
}

bool nnObjComponent::disconnectFromUp(void)
{
	bool res = false;
	if (v_num.size() != 0)
	{
		v_num[0] = 0;
		res = true;
	}
	return false;
}

bool nnObjComponent::disconnectFromDown(void)
{
	bool res = false;
	if (v_num.size() == 2)
	{
		v_num[1] = 0;
		res = true;
	}
	return false;
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

