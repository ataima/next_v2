#include <stdio.h>
#include <stdlib.h>
#include "n2draw.h"
#include "miniXml.h"

/**************************************************************
Copyright(c) 2015 Angelo Coppi

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files(the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and / or sell
copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions :

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
********************************************************************/

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


void nnObj::save(miniXmlNode *root)
{
	if (root != nullptr)
	{
		root->add(X("Context"),v_context);
	}
}



void nnObj::load(miniXmlNode *root)
{
	if (root != nullptr)
	{
		miniXmlNode * node=root->find(X("Context"));
		if (node != nullptr)
		{
			v_context = (ObjContext)node->getLong();
		}
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const  std::wstring nnObjPos::toString(void) const
{
	std::wostringstream s;
	s << L"X:" << v_Xpos << L" - Y:" <<
		v_Ypos << " - " << nnObj::toString();
	return s.str();
}



void nnObjPos::save(miniXmlNode *root)
{
	if (root != nullptr)
	{
		nnObj::save(root);
		root->add(X("X_Position"), v_Xpos);
		root->add(X("Y_Position"), v_Ypos);
	}
}

void nnObjPos::load(miniXmlNode *root)
{
	if (root != nullptr)
	{
		nnObj::load(root);
		miniXmlNode * node = root->find(X("X_Position"));
		if (node != nullptr)
		{
			v_Xpos = node->getLong();
		}
		node = root->find(X("Y_Position"));
		if (node != nullptr)
		{
			v_Ypos = node->getLong();
		}
	}
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


InnObj * nnObjConn::getObjFromIds(spec_obj specific, ObjContext context)
{
	InnObj *obj = nullptr;
	switch (context)
	{
	case objWire: obj=new nnObjWire(eWire::noWire); break;
	case objContact: 
	{
		switch (specific)
		{
		case isGeneric: obj = new nnObjContact();
		}
	}
		 break;
	case objCoil:
	{
		switch (specific)
		{
		case isGeneric: obj = new nnObjCoil();
		}
	}
		break;
	}
	return obj;
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
				res = connectFromUp(pb->getConnections().front());
				if(res)
					res=wire->connectFromDown(pb->getConnections().front());
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


void nnObjConn::save(miniXmlNode *root)
{
	if (root != nullptr)
	{
		std::stringstream s;
		nnObjPos::save(root);
		for (auto i : v_num)
		{
			s << i << " ";
		}		
		root->add(X("Connections"), (XCHAR *)s.str().c_str());
	}
}


void nnObjConn::load(miniXmlNode *root)
{
	if (root != nullptr)
	{
		nnObjPos::load(root);
		miniXmlNode * node = root->find(X("Connections"));
		if (node != nullptr)
		{
			v_num.clear();
			const XCHAR *value = node->getValue();
			size_t len = STRLEN(value);
			if (value != nullptr && len>0 )
			{
				do{
					int i = ATOL(value);
					v_num.push_back(i);
					while (*value!=' ' && len>0)
					{ 
						value++;
						len--;
					}
				} while (*value != X('\0') && len>0);
			}
		}		
	}
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


void nnObjWire::save(miniXmlNode *root)
{
	if (root != nullptr)
	{
		nnObjConn::save(root);
		root->add(X("Connection_Type"), v_wire);
	}
}

void nnObjWire::load(miniXmlNode *root)
{
	if (root != nullptr)
	{
		nnObjConn::load(root);
		miniXmlNode *node = root->find(X("Connection_Type"));
		if (node != nullptr)
		{
			v_wire = (eWire)node->getLong();
		}
	}
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

void nnObjContact::save(miniXmlNode *root)
{
	if (root != nullptr)
	{
		root->add(X("Spec"), v_spec);
		nnObjConn::save(root);
		nnObjVCPU::save(root);
	}
}

void nnObjContact::load(miniXmlNode *root)
{
	if (root != nullptr)
	{
		miniXmlNode *node = root->find(X("Spec"));
		if (node != nullptr)
		{
			v_spec = (spec_obj)node->getLong();
			nnObjConn::load(root);
			nnObjVCPU::load(root);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const  std::wstring nnObjCoil::toString(void) const
{
	std::wostringstream s;
	s << "COIL : " << std::endl;
	s << nnObjComponent::toString();
	return s.str();
}

void nnObjCoil::save(miniXmlNode *root)
{
	if (root != nullptr)
	{
		root->add(X("Spec"), v_spec);
		nnObjConn::save(root);
	}
}

void nnObjCoil::load(miniXmlNode *root)
{
	if (root != nullptr)
	{
		miniXmlNode *node = root->find(X("Spec"));
		if (node != nullptr)
		{
			v_spec = (spec_obj)node->getLong();
			nnObjConn::load(root);
		}
	}
}

const std::wstring nnObjVCPU::toString(void) const
{
	std::wstringstream os;
	if (v_vcpu != nullptr)
	{		
		for (auto i : v_reg)
		{
			if (i < v_vcpu->memSIZE)
				os << "R(" << i << ") = " << std::dec << v_vcpu->u.memBASE[i] << "0X" << std::hex << v_vcpu->u.memBASE[i] << std::dec << std::endl;
			else
				os << "Error: index = " << i << " Out of bounds" << std::endl;
		}
	}
	return os.str();
}

void nnObjVCPU::save(miniXmlNode * root)
{
	if (root != nullptr)
	{
		miniXmlNode *child=root->add(X("VCPU"),(XCHAR *)X(""));
		SSTRING s;
		for (auto i : v_reg)
		{
			s << i << " ";
		}
		child->add(X("Regs"), (XCHAR *)s.str().c_str());
	}
}

void nnObjVCPU::load(miniXmlNode * root)
{
	if (root != nullptr)
	{

		miniXmlNode * node = root->find(X("VCPU"));
		if (node != nullptr)
		{
			node = node->find(X("Regs"));
			if (node != nullptr)
			{
				v_reg.clear();
				const XCHAR *value = node->getValue();
				size_t len = STRLEN(value);
				if (value != nullptr && len > 0)
				{
					do {
						int i = ATOL(value);
						v_reg.push_back(i);
						while (*value != ' ' && len > 0)
						{
							value++;
							len--;
						}
					} while (*value != X('\0') && len > 0);
				}
			}
		}
	}
}
