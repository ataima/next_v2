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


nnToolView::nnToolView(IImageManager *_images)
    :images(_images),active(nullptr),pos_sel(0)
{
    commands.clear();
}

nnToolView::~nnToolView()
{
    if(commands.size())
    {
        for(auto v: commands)
        {
            delete v;
        }
        commands.clear();
    }
    active=nullptr;
    pos_sel=-1;
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
                IXmlNode *h=v->find(X("COMMANDER"));
                if(h)
                {
                    nnCommander *command= new nnCommander();
                    MEMCHK(nnCommander,command);
                    if(command)
                    {
                        res=command->readConfiguration(h);
                        if(!res)break;
                    }
                    commands.push_back(command);
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
    else
    {
        res=true;
    }
    return res;
}

bool nnToolView::draw(bmpImage & bkg, int x, int y, void * context)
{

    return false;
}

bool nnToolView::handlerRequestCommand( nnPoint & pos,int & command)
{
    bool res=false;
    if(active)
    {
        res=active->handlerRequestCommand(pos,command);
        if(res)
        {
            pos_sel=0;
            active=nullptr;
        }
    }
    return res;
}





