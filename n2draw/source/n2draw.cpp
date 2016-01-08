#include <stdio.h>
#include <stdlib.h>
#include "n2draw.h"
#include "n2miniXml.h"
#include "n2utoatou.h"



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

const STRING nnObj::toString(void) const
{
    SSTREAM s;
    switch (v_context)
    {
    case objNone:  s << X("objNone"); break;
    case objWire: s << X("objWire"); break;
    case objContact: s << X("objContact"); break;
    case objCoil: s << X("objCoiX("); break;
    default: s << X("unknow"); break;
    }
    return s.str();
}


void nnObj::save(IXmlNode *root)
{
    if (root != nullptr)
    {
        root->add(X("Context"),v_context);
    }
}



void nnObj::load(IXmlNode *root)
{
    if (root != nullptr)
    {
        IXmlNode * node=root->find(X("Context"));
        if (node != nullptr)
        {
            v_context = (ObjContext)node->getLong();
        }
    }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const  STRING nnObjPos::toString(void) const
{
    SSTREAM s;
    s << X("X:") << v_Xpos << X(" - Y:") <<
        v_Ypos << " - " << nnObj::toString();
    return s.str();
}



void nnObjPos::save(IXmlNode *root)
{
    if (root != nullptr)
    {
        nnObj::save(root);
        root->add(X("X_Position"), v_Xpos);
        root->add(X("Y_Position"), v_Ypos);
    }
}

void nnObjPos::load(IXmlNode *root)
{
    if (root != nullptr)
    {
        nnObj::load(root);
        IXmlNode * node = root->find(X("X_Position"));
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
bool nnObjConn::powerConnect(int num)
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


InnObj * nnObjConn::getObjFromIds(custom_obj specific, ObjContext context)
{
    InnObj *obj = nullptr;
    switch (context)
    {
    case objWire: obj=new nnObjWire(eWire::noWire); break;
    case objContact: 
    {
        switch (specific)
        {
        case contact_generic_unknow: obj = new nnObjContact();
        }
    }
         break;
    case objCoil:
    {
        switch (specific)
        {
        case coil_generic_unknow: obj = new nnObjCoil();
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



bool nnObjWire::connectFromUp(int num)
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




bool nnObjWire::disconnectFromUp(int num)
{
    (num);
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



bool nnObjWire::connectFromDown(int num)
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



bool nnObjWire::disconnectFromDown(int num)
{
    (num);
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



bool nnObjWire::connectFromLeft(int num)
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


bool nnObjWire::disconnectFromLeft(int num)
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


bool nnObjWire::connectFromRight(int num)
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




bool nnObjWire::disconnectFromRight(int num)
{
    (num);
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


void nnObjWire::setConnections(int n)
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


eWire nnObjWire::wireStringToEnum(const XCHAR *name)
{
    STRING value = name;
    if (value == X("wireHorizzontal"))
        return wireHorizzontal;
    if (value == X("wireVertical"))
        return wireVertical;
    if (value == X("wireAngleUpRight"))
        return wireAngleUpRight;
    if (value == X("wireAngleUpLeft"))
        return wireAngleUpLeft;
    if (value == X("wireAngleDownRight"))
        return wireAngleDownRight;
    if (value == X("wireAngleDownLeft"))
        return wireAngleDownLeft;
    if (value == X("wireTHorizDown"))
        return wireTHorizDown;
    if (value == X("wireTHorizUp"))
        return wireTHorizUp;
    if (value == X("wireTVertRight"))
        return wireTVertRight;
    if (value == X("wireTVertLeft"))
        return wireTVertLeft;
    if (value == X("wireCross"))
        return wireCross;
    return noWire;
}

int nnObjConn::uid_num = 2;


const STRING nnObjConn::toString(void) const
{
    SSTREAM s;
    if (v_num.size() > 0)
        s << "N:" << v_num.front() << ":" << v_num.back() << " - " << nnObjPos::toString();
    else
        s << "N:" << "0:0" << " - " << nnObjPos::toString();
    return s.str();
}


void nnObjConn::save(IXmlNode *root)
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


void nnObjConn::load(IXmlNode *root)
{
    if (root != nullptr)
    {
        nnObjPos::load(root);
        IXmlNode * node = root->find(X("Connections"));
        if (node != nullptr)
        {
            v_num.clear();
            const XCHAR *value = node->getValue();
            size_t len = STRLEN(value);
            if (value != nullptr && len>0 )
            {
                do{
                    UtoA toA(value);
                    int i = ATOL(toA.utf8());
                    v_num.push_back(i);
                    while (*value!=' ' && len>0)
                    { 
                        value++;
                        len--;
                    }
                    while (*value==' ' && len>0)
                    {
                        value++;
                        len--;
                    }
                } while (*value != X('\0') && len>0);
            }
        }		
    }
}

const  STRING nnObjWire::toString(void) const
{
    SSTREAM s;
    s << nnObjConn::toString() << " - ";
    switch (v_wire)
    {
    case noWire: s << X("noWire"); break;
    case wireHorizzontal: s << X("wireHorizzontal"); break;
    case wireVertical: s << X("wireVerticaL"); break;
    case wireAngleUpRight: s << X("wireAngleUpRight"); break;
    case wireAngleUpLeft: s << X("wireAngleUpLeft"); break;
    case wireAngleDownRight: s << X("wireAngleDownRight"); break;
    case wireAngleDownLeft: s << X("wireAngleDownLeft"); break;
    case wireTHorizDown: s << X("wireTHorizDown"); break;
    case wireTHorizUp: s << X("wireTHorizUp"); break;
    case wireTVertRight: s << X("wireTVertRight"); break;
    case wireTVertLeft:s << X("wireTVertLeft"); break;
    case wireCross:s << X("wireCross"); break;
    default: s << X("unknow"); break;
    }
    return s.str();
}


void nnObjWire::save(IXmlNode *root)
{
    if (root != nullptr)
    {
        nnObjConn::save(root);
        root->add(X("Connection_Type"), v_wire);
    }
}

void nnObjWire::load(IXmlNode *root)
{
    if (root != nullptr)
    {
        nnObjConn::load(root);
        IXmlNode *node = root->find(X("Connection_Type"));
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
            int nconn = nnObjConn::getUI();
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
            //front ->isWire
            int nconn = from->getConnections().front();
            if (nconn == 0 && (v_num.size() < 2 || v_num.front() == 0))
            {
                nconn = nnObjConn::getUI();
                from->setConnections(nconn);
            }
            result = connectFromUp(nconn);
        }
    }
    else
        if (res == eWireDirection::wire_from_down)
        {
            if (from->isComponent())
            {
                int nconn = nnObjConn::getUI();
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
                //front ->isWire
                int nconn = from->getConnections().front();
                if (nconn == 0 && (v_num.size() < 2 || v_num.back() == 0))
                {
                    nconn = nnObjConn::getUI();
                    from->setConnections(nconn);
                }
                result = connectFromDown(nconn);
            }

        }
    return result;
}

bool nnObjComponent::connectFromUp(int b)
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

bool nnObjComponent::connectFromDown(int b)
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

custom_obj nnObjComponent::getCustomizationFromName(const XCHAR * s)
{
    STRING name = s;
    if (name == X("contactGenericAnd"))
        return contactGenericAnd;
    else
        if (name == X("contactGenericOr"))
            return contactGenericOr;
        else
            if (name == X("coilGeneric"))
                return coilGeneric;
    return no_component;
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
const  STRING nnObjContact::toString(void) const
{
    SSTREAM s;
    s << X("CONTACT : ");
    s << nnObjComponent::toString()<< std::endl;
    s << nnObjVCPU::toString() << std::endl;
    return s.str();
}

const  STRING nnContactNO::toString(void) const
{
    SSTREAM s;
    s << X("N.O. ") << nnObjContact::toString();
    return s.str();
}

const  STRING nnContactNC::toString(void) const
{
    SSTREAM s;
    s << X("N.C. ") << nnObjContact::toString();
    return s.str();
}

void nnObjContact::save(IXmlNode *root)
{
    if (root != nullptr)
    {
        root->add(X("Custom"), v_spec);
        nnObjConn::save(root);
        nnObjVCPU::save(root);
    }
}

void nnObjContact::load(IXmlNode *root)
{
    if (root != nullptr)
    {
        IXmlNode *node = root->find(X("Custom"));
        if (node != nullptr)
        {
            v_spec = (custom_obj)node->getLong();
            nnObjConn::load(root);
            nnObjVCPU::load(root);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const  STRING nnObjCoil::toString(void) const
{
    SSTREAM s;
    s << X("COIL : ");
    s << nnObjComponent::toString() << std::endl;
    s << nnObjVCPU::toString() << std::endl;
    return s.str();
}

const  STRING nnGenericCoil::toString(void) const
{
    SSTREAM s;
    s << X("GEN ") <<nnObjCoil::toString() << std::endl;
    return s.str();
}


void nnObjCoil::save(IXmlNode *root)
{
    if (root != nullptr)
    {
        root->add(X("Custom"), v_spec);
        nnObjConn::save(root);
    }
}

void nnObjCoil::load(IXmlNode *root)
{
    if (root != nullptr)
    {
        IXmlNode *node = root->find(X("Custom"));
        if (node != nullptr)
        {
            v_spec = (custom_obj)node->getLong();
            nnObjConn::load(root);
        }
    }
}

const STRING nnObjVCPU::toString(void) const
{
    SSTREAM os;
    if (v_vcpu != nullptr)
    {		
        for (auto i : v_reg)
        {
            if (i < v_vcpu->memSIZE)
                os << X("R(") << i << X(") = ") << std::dec << v_vcpu->u.memBASE[i] << "0X" << std::hex << v_vcpu->u.memBASE[i] << std::dec << std::endl;
            else
                os << X("Error: index = ") << i << X(" Out of bounds") << std::endl;
        }
    }
    return os.str();
}

void nnObjVCPU::save(IXmlNode *root)
{
    if (root != nullptr)
    {
        IXmlNode *child=root->add(X("VCPU"),(XCHAR *)X(""));
        SSTREAM s;
        for (auto i : v_reg)
        {
            s << i << " ";
        }
        child->add(X("Regs"), (XCHAR *)s.str().c_str());
    }
}

void nnObjVCPU::load(IXmlNode *root)
{
    if (root != nullptr)
    {

        IXmlNode * node = root->find(X("VCPU"));
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
                        UtoA toA(value);
                        int i = ATOL(toA.utf8());
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
