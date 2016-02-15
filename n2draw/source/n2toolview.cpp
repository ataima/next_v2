#include "n2draw.h"
#include "n2drawmanager.h"
#include "n2imagemanager.h"
#include "n2toolview.h"
#include "n2viewglue.h"
#include "n2commander.h"
#include <iostream>
#include "n2exception.h"


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


nnToolView::nnToolView()
    :active(nullptr)
{
    font = nullptr;
    commands.clear();
}

nnToolView::~nnToolView()
{
    if(commands.size())
    {
        for(auto v: commands)
        {
            delete v.second;
        }
        commands.clear();
    }
    active=nullptr;
    font = nullptr;
}

bool nnToolView::readConfiguration(IXmlNode *node)
{
    int numBars;
    bool res = false;
    IXmlNode *v,*t = node->find(X("TOOLBARS"));
    if (t)
    {
        numBars=t->getLong();
    }
    else
    {
        xmlConfigurationNodeException *pe = new xmlConfigurationNodeException(X("TOOLBARS"));
        throw (pe);
    }
    if(numBars>0)
    {
        int i;
        for(i=0; i<numBars; i++)
        {
            v=t->find(X("TOOLBARS_"),i);
            if(v)
            {
                int id=0;
                IXmlNode *h=v->find(X("ID"));
                if (h)
                {
                    id=h->getLong();
                }
                else
                {
                    xmlConfigurationNodeException *pe = new xmlConfigurationNodeException(X("ID"));
                    throw (pe);
                }
                h=v->find(X("COMMANDER"));
                if(h)
                {
                    nnCommander *command= new nnCommander();
                    MEMCHK(nnCommander,command);
                    if(command)
                    {
                        res=command->readConfiguration(h);
                        if(!res)break;
                    }
                    commands[id]=command;
                }
                else
                {
                    SSTREAM ssInfo;
                    ssInfo<<X("COMMANDER");
                    xmlConfigurationNodeException *pe = new xmlConfigurationNodeException(ssInfo.str().c_str());
                    throw (pe);
                }
            }
            else
            {
                SSTREAM ss;
                ss<<X("TOOLBARS_")<<i;
                xmlConfigurationNodeException *pe = new xmlConfigurationNodeException(ss.str().c_str());
                throw (pe);
            }
        }
    }
    if(res==true)
    {
        commanderList::iterator it=commands.find(0);
        if(it!=commands.end())
        {
            active=it->second;
            active->setFont(font);
        }
        else
        {
            active=nullptr;
        }
    }
    return res;
}

bool nnToolView::draw(bmpImage & bkg, IViewGlue * glue)
{
    bool res=false;
    if(active)
    {
        return active->draw(bkg, phyPos,glue);
    }
    return res;
}

bool nnToolView::handlerRequestCommand( nnPoint & pos,int & command)
{
    bool res=false;
    if(active)
    {
            res = active->checkRequestCommand(pos, command);
    }
    return res;
}


bool nnToolView::loadImages(STRING &path)
{
    bool res=false;
    commanderList::iterator it= commands.begin();
    commanderList::iterator _end= commands.end();
    while(it!=_end)
    {
        ICommander *temp=it->second;
        res=temp->loadImages(path);
        if(!res)
        {
            break;
        }
        it++;
    }
    res=(it==_end);
    return res;
}


bool nnToolView::handlerMouseMove( nnPoint & pos,IExtHandler *hook)
{
    bool res=false;
    if(active)
    {
            res = active->handlerMouseMove(pos, hook);
    }
    return res;
}



bool nnToolView::checkIntCommand(int command)
{
    bool res=false;
    commanderList::iterator it=commands.find(command);
    if(it!=commands.end())
    {
        active=it->second;
        active->setFont(font);
    }
    else
    {
        active=nullptr;
    }
    res=(active!=nullptr);
    return res;
}


bool nnToolView::handlerMouseButtonDown(nnPoint &phyPoint, show_status & status, IExtHandler *hook)
{
    bool res = false;
    int command = 0;
    res = handlerRequestCommand(phyPoint, command);
    if (res)
    {
        if (checkIntCommand(command))
        {
            status = show_toolbar;
            setDrawPosition(phyPoint);
            if (hook)
            {
                hook->doHandler(action_redraw);
            }
            res = true;
        }
        else
        {
            res = checkIntCommand(0);
            if (hook)
            {
                status = show_none;
                if (command > action_host_command)
                {
                    auto *t = new nnAbstractParam<int>(command);
                    hook->doHandler(action_host_command, t);
                    hook->doHandler(action_redraw);
                }
                else
                {
                    hook->doHandler(command);
                }
            }
        }        
    }
    return res;
}

