#include "n2draw.h"
#include "n2miniXml.h"
#include "n2drawmanager.h"
#include "n2imagemanager.h"
#include "n2view.h"
#include "n2viewglue.h"
#include "n2exception.h"
#include "n2fontmanager.h"
#include "n2fontlist.h"
#include "n2ExtHandler.h"
#include "n2childapp.h"

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



nnChildApp::nnChildApp(unsigned int _id )
{

    object_manager=nullptr;
    fonts = nullptr;
    view = nullptr;
    imageManager = nullptr;
    externalHandler = nullptr;
    id=_id;
}

nnChildApp::~nnChildApp()
{
    clean();

}

void nnChildApp::clean(void)
{
    if (object_manager)
    {
        delete object_manager;
        object_manager = nullptr;
    }
    if (imageManager)
    {
        delete imageManager;
        imageManager = nullptr;
    }
    if (fonts)
    {
        delete fonts;
        fonts = nullptr;
    }
    if (view)
    {
        delete view;
        view = nullptr;
    }
    externalHandler = nullptr;
    if (handlers)
    {
        handlers->clear();
        delete handlers;
        handlers = nullptr;
    }
    id = -1;
}


bool nnChildApp::createObjects(IConfig *configuration,STRING & conf_file_name)
{
    bool res = false;
    if (configuration != nullptr)
    {
        res = configuration->readConfiguration(conf_file_name.c_str());
        if (res == true)
        {
            IXmlNode * root = configuration->getRoot();
            if (root != nullptr)
            {
                IXmlNode *conf_manager = root->find(X("APP"));
                if (conf_manager != nullptr)
                {
                    //int line = __LINE__;
                    IXmlNode *size_default = conf_manager->find(X("DEFAULT_WIDTH"));
                    if (size_default != nullptr)
                    {
                        int default_w = size_default->getLong();
                        size_default = conf_manager->find(X("DEFAULT_HEIGHT"));
                        if (size_default != nullptr)
                        {
                            int default_h = size_default->getLong();
                            int line = __LINE__;
                            object_manager = new nnObjManager(default_w, default_h);
                            IManager *obj = object_manager;
                            MEMCHK(IManager , obj);
                            conf_manager = root->find(X("MANAGER"));
                            if (conf_manager)
                            {
                                res = object_manager->readConfiguration(conf_manager);
                                if (res)
                                {
                                    line = __LINE__;
                                    imageManager = new nnImageManager();
                                    IImageManager *img = imageManager;
                                    MEMCHK(IImageManager, img);
                                    conf_manager = root->find(X("IMAGES"));
                                    if (conf_manager)
                                    {
                                        res = imageManager->readConfiguration(conf_manager);
                                    }
                                    else
                                    {
                                        xmlConfigurationNodeException  *e = new xmlConfigurationNodeException(X("IMAGES"));
                                        throw(e);
                                    }
                                    if (res)
                                    {
                                        conf_manager = root->find(X("FONTS"));
                                        if (conf_manager)
                                        {
                                            line = __LINE__;
                                            fonts = new nnFontList();       
                                            IFontList *flist = fonts;
                                            MEMCHK(IFontList, flist);
                                            res = flist->readConfiguration(conf_manager);
                                        }
                                        else
                                        {
                                            xmlConfigurationNodeException  *e = new xmlConfigurationNodeException(X("FONTS"));
                                            throw(e);
                                        }
                                        if (res)
                                        {
                                            conf_manager = root->find(X("PHY_MAP"));
                                            if (conf_manager)
                                            {
                                                line = __LINE__;
                                                view = new nnViewGlue(this);                                                MEMCHK(IViewGlue, view);
                                                res = view->readConfiguration(conf_manager);
                                            }
                                            else
                                            {
                                                xmlConfigurationNodeException  *e = new xmlConfigurationNodeException(X("PHY_MAP"));
                                                throw(e);
                                            }

                                        }
                                    }
                                }
                            }
                            else
                            {
                                xmlConfigurationNodeException  *e = new xmlConfigurationNodeException(X("MANAGER"));
                                throw(e);
                            }
                        }
                        else
                        {
                            xmlConfigurationNodeException  *e = new xmlConfigurationNodeException(X("DEFAULT_HEIGHT"));
                            throw(e);
                        }
                    }
                    else
                    {
                        xmlConfigurationNodeException  *e = new xmlConfigurationNodeException(X("DEFAULT_WIDTH"));
                        throw(e);
                    }
                }
                else
                {
                    xmlConfigurationNodeException  *e = new xmlConfigurationNodeException(X("MANAGER"));
                    throw(e);
                }
            }
            else
            {
                appManagerConfigureParseXmlFileException  *e = new appManagerConfigureParseXmlFileException(conf_file_name);
                throw(e);
            }
        }
        else
        {
            appManagerConfigureParseXmlFileException  *e = new appManagerConfigureParseXmlFileException(conf_file_name);
            throw(e);
        }
    }
    //TODO have to add exception Management .....
    if (res)
    {
        res = loadImages();
    }
    if (res)
    {
        res = view->createDraw();
    }
    if (res)
    {
        handlers = new nnExtHandlerList();
        MEMCHK(IExtHandlerList, handlers);
        if(handlers)
           res = addExtHandler(handler_exec_command, &nnChildApp::internalCommandRuote, this);
    }
    return res;
}


bool nnChildApp::loadImages(void)
{
    bool res = false;
    if (imageManager)
    {
        res = imageManager->loadImages();
        if (!res)
        {
            appManagerConfigureLoadImageException *e = new appManagerConfigureLoadImageException();
            throw(e);
        }
        if (view)
        {
            res = view->loadImages(imageManager->getDefaulPath().c_str());
            if (!res)
            {
                appManagerConfigureLoadImageException *e = new appManagerConfigureLoadImageException();
                throw(e);
            }
            if (fonts)
            {
                res = fonts->loadImages();
                if (!res)
                {
                    appManagerConfigureLoadImageException *e = new appManagerConfigureLoadImageException();
                    throw(e);
                }
            }
        }
    }
    return res;
}

bool nnChildApp::setExtHandler(handler_exec type, extHandler _hook, void *unkObj)
{
    bool res = false;
    nnExtHandler  *nh = new nnExtHandler(type, _hook, unkObj);
    if (nh)
    {
        externalHandler = nh;
    }
    return res;
}


void nnChildApp::internalCommandRuote(void * dest, handlerAction type_param, size_t user_param)
{
    if (dest)
    {
        IChild *child = static_cast<IChild *>(dest);
        if (child)
        {
            child->commandRuote( type_param,  user_param);
        }
    }
}



void nnChildApp::commandRuote( handlerAction type_param, size_t user_param)
{
    if (type_param == action_host_command)
    {
        switch (user_param)
        {
        case 4000:
            break;
        case 4001:
            break;
        case 4002:
            break;
        case 4003:
            break;
        case 4004:
            break;
        case 4005:
            break;
        default:
            if (externalHandler)
                externalHandler->doHandler(type_param, user_param);
            break;
        }
    }
    else
    {
        if (externalHandler)
            externalHandler->doHandler(type_param, user_param);
    }

}



bool nnChildApp::handlerMouseMove(nn_mouse_buttons buttons, nnPoint phyPoint)
{
    bool res = false;
    if (view)
        res=view->handlerMouseMove(buttons, phyPoint);
    return res;
}

bool nnChildApp::handlerMouseButtonDown(nn_mouse_buttons buttons, nnPoint phyPoint)
{
    bool res = false;
    if (view)
        res = view->handlerMouseButtonDown(buttons, phyPoint);
    return res;
}

bool nnChildApp::nnChildApp::handlerMouseButtonUp(nn_mouse_buttons buttons, nnPoint phyPoint)
{
    bool res = false;
    if (view)
        res = view->handlerMouseButtonUp(buttons, phyPoint);
    return res;
}

bool nnChildApp::handlerScrollHorz(int pos)
{
    bool res = false;
    if (view)
        res = view->handlerScrollHorz(pos);
    return res;
}

bool nnChildApp::handlerScrollVert(int pos)
{
    bool res = false;
    if (view)
        res = view->handlerScrollVert(pos);
    return res;
}

bool nnChildApp::handlerEscapeButton(bool shift, bool ctrl, bool alt)
{
    bool res = false;
    if (view)
        res = view->handlerEscapeButton(shift,ctrl,alt);
    return res;
}

bool nnChildApp::handlerHomeButton(bool shift, bool ctrl, bool alt)
{
    bool res = false;
    if (view)
        res = view->handlerHomeButton(shift, ctrl, alt);
    return res;
}

bool nnChildApp::handlerEndButton(bool shift, bool ctrl, bool alt)
{
    bool res = false;
    if (view)
        res = view->handlerEndButton(shift, ctrl, alt);
    return res;
}

bool nnChildApp::handlerPageUpButton(bool shift, bool ctrl, bool alt)
{
    bool res = false;
    if (view)
        res = view->handlerPageUpButton(shift, ctrl, alt);
    return res;

}

bool nnChildApp::handlerPageDownButton(bool shift, bool ctrl, bool alt)
{
    bool res = false;
    if (view)
        res = view->handlerPageDownButton(shift, ctrl, alt);
    return res;
}

bool nnChildApp::handlerPageRightButton(bool shift, bool ctrl, bool alt)
{
    bool res = false;
    if (view)
        res = view->handlerPageRightButton(shift, ctrl, alt);
    return res;
}

bool nnChildApp::handlerPageLeftButton(bool shift, bool ctrl, bool alt)
{
    bool res = false;
    if (view)
        res = view->handlerPageLeftButton(shift, ctrl, alt);
    return res;
}

bool nnChildApp::handlerLeftButton(bool shift, bool ctrl, bool alt)
{
    bool res = false;
    if (view)
        res = view->handlerLeftButton(shift, ctrl, alt);
    return res;
}

bool nnChildApp::handlerRightButton(bool shift, bool ctrl, bool alt)
{
    bool res = false;
    if (view)
        res = view->handlerRightButton(shift, ctrl, alt);
    return res;
}

bool nnChildApp::handlerUpButton(bool shift, bool ctrl, bool alt)
{
    bool res = false;
    if (view)
        res = view->handlerUpButton(shift, ctrl, alt);
    return res;
}

bool nnChildApp::handlerDownButton(bool shift, bool ctrl, bool alt)
{
    bool res = false;
    if (view)
        res = view->handlerDownButton(shift, ctrl, alt);
    return res;
}

bool nnChildApp::handlerRequestCommand(nnPoint phyPoint, int & command)
{
    bool res = false;
    if (view)
        res = view->handlerRequestCommand(phyPoint, command);
    return res;
}

bool nnChildApp::addExtHandler(handler_exec type,
    extHandler  _hook,
    void *unkObj)
{
    bool res = false;
    nnExtHandler  *nh = new nnExtHandler(type, _hook, unkObj);
    res = handlers->add(type, nh);
    return res;
}
