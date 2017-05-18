#include "n2draw.h"
#include "n2miniXml.h"
#include "n2drawmanager.h"
#include "n2imagemanager.h"
#include "n2view.h"
#include "n2viewglue.h"
#include "n2exception.h"
#include "n2fontmanager.h"
#include "n2fontlist.h"
#include "n2exthandler.h"
#include "n2childapp.h"
#include "n2connection.h"
#include<dirent.h>

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

void  nnChildApp::defaultCommandRequest(void * dest, size_t type_param, IParam *user_param)
{
    if (dest) {
        IChild *rel = reinterpret_cast<IChild*>(dest);
        if (rel)
            rel->defaultProcess(type_param, user_param);
    }
}


nnChildApp::nnChildApp(unsigned int _id )
{

    object_manager=nullptr;
    fonts = nullptr;
    view = nullptr;
    imageManager = nullptr;
    externalHandler = nullptr;
    extHandler _hook = static_cast<extHandler>(nnChildApp::defaultCommandRequest);
    defaultHandler = new nnExtHandler(_hook,this,true);
    id=_id;
}

nnChildApp::~nnChildApp()
{
    clean();

}

void nnChildApp::clean(void)
{
    if (object_manager) {
        delete object_manager;
        object_manager = nullptr;
    }
    if (imageManager) {
        delete imageManager;
        imageManager = nullptr;
    }
    if (fonts) {
        delete fonts;
        fonts = nullptr;
    }
    if (view) {
        delete view;
        view = nullptr;
    }
    if (externalHandler) {
        delete externalHandler;
        externalHandler = nullptr;
    }
    if (defaultHandler) {
        delete defaultHandler;
        defaultHandler = nullptr;
    }
    id = -1;
}




bool nnChildApp::createObjects(IConfig *configuration, STRING & conf_file_name, STRING & confPath)
{
    bool res = false;
    if (configuration != nullptr) {
        STRING conf=confPath+conf_file_name;
        res = configuration->readConfiguration(conf.c_str());
        if (res == true) {
            IXmlNode * root = configuration->getRoot();
            if (root != nullptr) {
                IXmlNode *conf_manager = root->find(X("APP"));
                if (conf_manager != nullptr) {
                    IXmlNode *size_default = conf_manager->find(X("DEFAULT_WIDTH"));
                    if (size_default != nullptr) {
                        int default_w = size_default->getLong();
                        size_default = conf_manager->find(X("DEFAULT_HEIGHT"));
                        if (size_default != nullptr) {
                            int default_h = size_default->getLong();
                            object_manager = new nnObjManager(default_w, default_h);
                            IManager *obj = object_manager;
                            MEMCHK(IManager , obj);
                            conf_manager = root->find(X("MANAGER"));
                            if (conf_manager) {
                                res = object_manager->readConfiguration(conf_manager);
                                if (res) {
                                    imageManager = new nnImageManager(confPath);
                                    IImageManager *img = imageManager;
                                    MEMCHK(IImageManager, img);
                                    conf_manager = root->find(X("IMAGES"));
                                    if (conf_manager) {
                                        res = imageManager->readConfiguration(conf_manager);
                                    } else {
                                        xmlConfigurationNodeException  *e = new xmlConfigurationNodeException(X("IMAGES"));
                                        throw(e);
                                    }
                                    if (res) {
                                        conf_manager = root->find(X("FONTS"));
                                        if (conf_manager) {
                                            fonts = new nnFontList();
                                            IFontList *flist = fonts;
                                            MEMCHK(IFontList, flist);
                                            res = flist->readConfiguration(conf_manager,confPath);
                                        } else {
                                            xmlConfigurationNodeException  *e = new xmlConfigurationNodeException(X("FONTS"));
                                            throw(e);
                                        }
                                        if (res) {
                                            conf_manager = root->find(X("PHY_MAP"));
                                            if (conf_manager) {
                                                view = new nnViewGlue(this);
                                                MEMCHK(IViewGlue, view);
                                                res = view->readConfiguration(conf_manager);
                                            } else {
                                                xmlConfigurationNodeException  *e = new xmlConfigurationNodeException(X("PHY_MAP"));
                                                throw(e);
                                            }

                                        }
                                    }
                                }
                            } else {
                                xmlConfigurationNodeException  *e = new xmlConfigurationNodeException(X("MANAGER"));
                                throw(e);
                            }
                        } else {
                            xmlConfigurationNodeException  *e = new xmlConfigurationNodeException(X("DEFAULT_HEIGHT"));
                            throw(e);
                        }
                    } else {
                        xmlConfigurationNodeException  *e = new xmlConfigurationNodeException(X("DEFAULT_WIDTH"));
                        throw(e);
                    }
                } else {
                    xmlConfigurationNodeException  *e = new xmlConfigurationNodeException(X("MANAGER"));
                    throw(e);
                }
            } else {
                appManagerConfigureParseXmlFileException  *e = new appManagerConfigureParseXmlFileException(conf);
                throw(e);
            }
        } else {
            appManagerConfigureParseXmlFileException  *e = new appManagerConfigureParseXmlFileException(conf);
            throw(e);
        }
    }
    //TODO have to add exception Management .....
    if (res) {
        res = loadImages();
    }
    if (res) {
        res = view->createDraw();
    }
    return res;
}


bool nnChildApp::loadImages(void)
{
    bool res = false;
    if (imageManager) {
        res = imageManager->loadImages();
        if (!res) {
            appManagerConfigureLoadImageException *e = new appManagerConfigureLoadImageException();
            throw(e);
        }
        if (view) {
            res = view->loadImages(imageManager->getDefaulPath());
            if (!res) {
                appManagerConfigureLoadImageException *e = new appManagerConfigureLoadImageException();
                throw(e);
            }
            if (fonts) {
                res = fonts->loadImages();
                if (!res) {
                    appManagerConfigureLoadImageException *e = new appManagerConfigureLoadImageException();
                    throw(e);
                }
            }
        }
    }
    return res;
}

bool nnChildApp::setExtHandler( extHandler _hook, void *unkObj)
{
    bool res = false;
    nnExtHandler  *nh = new nnExtHandler( _hook, unkObj,false);
    if (nh) {
        externalHandler = nh;
    }
    return res;
}


bool nnChildApp::Capture(int command,unsigned int image)
{
    bool res = false;
    if (view) {
        res = view->Capture(command,image);
    }
    return res;
}



bool nnChildApp::addContact(nnPoint & pos, nnObjContact * contact)
{
    bool res = false;
    if (object_manager && contact) {
        InnObj *obj = object_manager->getObj(pos.x, pos.y);
        if (obj == nullptr) {
            res = object_manager->addContact(pos.x, pos.y, contact);
            view->updateDraw();
        } else {
            if (!obj->isComponent()) {
                //iswire
                res = object_manager->replaceObj(pos.x, pos.y, contact);
                view->updateDraw();
            }
        }
    }
    return res;
}

bool nnChildApp::addCoil(nnPoint & pos, nnObjCoil * coil)
{
    bool res = false;
    if (object_manager && coil) {
        res = object_manager->addCoil(pos.x, coil);
        view->updateDraw();
    }
    return res;
}

bool nnChildApp::connect(nnPoint & start, nnPoint & end)
{
    bool res = false;
    if (object_manager ) {
        res = nnConnection::connectComponent(object_manager, start, end);
        view->updateDraw();
    }
    return res;
}

bool nnChildApp::handlerMouseMove(nn_mouse_buttons buttons, nnPoint & phyPoint)
{
    struct pulse_caller {
        static void hook(IViewGlue *  view, nn_mouse_buttons  buttons, nnPoint   phyPoint)
        {
            view->handlerMouseMove(buttons, phyPoint);
        }
        pulse_caller(IViewGlue *  view, nn_mouse_buttons  buttons, nnPoint   phyPoint )
        {
            std::thread th( pulse_caller::hook, view, buttons, phyPoint);
            th.detach();
        }
    };
    bool res = false;
    if (view) {
        nnPoint p=phyPoint;
        pulse_caller(view, buttons, p);
        res = true;
    }
    return res;
}

bool nnChildApp::handlerMouseButtonDown(nn_mouse_buttons buttons, nnPoint & phyPoint)
{
    struct pulse_caller {
        static void hook(IViewGlue *  view, nn_mouse_buttons  buttons, nnPoint  phyPoint)
        {
            view->handlerMouseButtonDown(buttons, phyPoint);
        }
        pulse_caller(IViewGlue *  view, nn_mouse_buttons  buttons, nnPoint  phyPoint )
        {
            std::thread th( pulse_caller::hook, view, buttons, phyPoint);
            th.detach();
        }
    };
    bool res = false;
    if (view) {
        nnPoint p=phyPoint;
        pulse_caller(view, buttons, p);
        res = true;
    }
    return res;
}

bool nnChildApp::handlerMouseButtonUp(nn_mouse_buttons buttons, nnPoint & phyPoint)
{
    //bool res = false;
    //if (view)
    //    res = view->handlerMouseButtonUp(buttons, phyPoint);
    //return res;

    struct pulse_caller {
        static void hook(IViewGlue *  view, nn_mouse_buttons  buttons, nnPoint  phyPoint)
        {
            view->handlerMouseButtonUp(buttons, phyPoint);
        }
        pulse_caller(IViewGlue *  view, nn_mouse_buttons  buttons, nnPoint  phyPoint )
        {
            std::thread th( pulse_caller::hook, view, buttons, phyPoint);
            th.detach();
        }
    };

    bool res = false;
    if (view) {
        nnPoint p=phyPoint;
        pulse_caller(view, buttons, p);
        res = true;
    }
    return res;
}

bool nnChildApp::handlerScrollHorz(int pos)
{
    struct  pulse_caller {
        static void hook(IViewGlue *  view, int pos)
        {
            view->handlerScrollHorz(pos);
        }
        pulse_caller(IViewGlue *  view, int pos)
        {
            std::thread th( pulse_caller::hook, view,pos);
            th.detach();
        }
    };
    bool res = false;
    if (view) {
        pulse_caller(view,pos);
        res = true;
    }
    return res;
}

bool nnChildApp::handlerScrollVert(int pos)
{
    struct  pulse_caller {
        static void hook(IViewGlue *  view, int pos)
        {
            view->handlerScrollVert(pos);
        }
        pulse_caller(IViewGlue *  view, int pos)
        {
            std::thread th( pulse_caller::hook, view,pos);
            th.detach();
        }
    };
    bool res = false;
    if (view) {
        pulse_caller(view, pos);
        res = true;
    }
    return res;
}

bool nnChildApp::handlerEscapeButton(bool shift, bool ctrl, bool alt)
{
    struct  pulse_caller {
        static void hook(IViewGlue *  view, bool shift, bool ctrl, bool alt)
        {
            view->handlerEscapeButton(shift,ctrl,alt);
        }
        pulse_caller(IViewGlue *  view, bool shift, bool ctrl, bool alt)
        {
            std::thread th( pulse_caller::hook, view,shift,ctrl,alt);
            th.detach();
        }
    };
    bool res = false;
    if (view) {
        pulse_caller(view, shift, ctrl, alt);
        res = true;
    }
    return res;
}

bool nnChildApp::handlerHomeButton(bool shift, bool ctrl, bool alt)
{
    struct  pulse_caller {
        static void hook(IViewGlue *  view, bool shift, bool ctrl, bool alt)
        {
            view->handlerHomeButton(shift, ctrl, alt);
        }
        pulse_caller(IViewGlue *  view, bool shift, bool ctrl, bool alt)
        {
            std::thread th( pulse_caller::hook, view,shift,ctrl,alt);
            th.detach();
        }
    };
    bool res = false;
    if (view) {
        pulse_caller(view, shift, ctrl, alt);
        res = true;
    }
    return res;
}

bool nnChildApp::handlerEndButton(bool shift, bool ctrl, bool alt)
{
    struct  pulse_caller {
        static void hook(IViewGlue *  view, bool shift, bool ctrl, bool alt)
        {
            view->handlerEndButton(shift, ctrl, alt);
        }
        pulse_caller(IViewGlue *  view, bool shift, bool ctrl, bool alt)
        {
            std::thread th( pulse_caller::hook, view,shift,ctrl,alt);
            th.detach();
        }
    };
    bool res = false;
    if (view) {
        pulse_caller(view, shift, ctrl, alt);
        res = true;
    }
    return res;
}

bool nnChildApp::handlerPageUpButton(bool shift, bool ctrl, bool alt)
{
    struct  pulse_caller {
        static void hook(IViewGlue *  view, bool shift, bool ctrl, bool alt)
        {
            view->handlerPageUpButton(shift, ctrl, alt);
        }
        pulse_caller(IViewGlue *  view, bool shift, bool ctrl, bool alt)
        {
            std::thread th( pulse_caller::hook, view,shift,ctrl,alt);
            th.detach();
        }
    };
    bool res = false;
    if (view) {
        pulse_caller(view, shift, ctrl, alt);
        res = true;
    }
    return res;
}

bool nnChildApp::handlerPageDownButton(bool shift, bool ctrl, bool alt)
{
    //bool res = false;
    //if (view)
    //    res = view->handlerPageDownButton(shift, ctrl, alt);
    //return res;
    struct  pulse_caller {
        static void hook(IViewGlue *  view, bool shift, bool ctrl, bool alt)
        {
            view->handlerPageDownButton(shift, ctrl, alt);
        }
        pulse_caller(IViewGlue *  view, bool shift, bool ctrl, bool alt)
        {
            std::thread th( pulse_caller::hook, view,shift,ctrl,alt);
            th.detach();
        }
    };
    bool res = false;
    if (view) {
        pulse_caller(view, shift, ctrl, alt);
        res = true;
    }
    return res;
}

bool nnChildApp::handlerPageRightButton(bool shift, bool ctrl, bool alt)
{
    struct  pulse_caller {
        static void hook(IViewGlue *  view, bool shift, bool ctrl, bool alt)
        {
            view->handlerPageRightButton(shift, ctrl, alt);
        }
        pulse_caller(IViewGlue *  view, bool shift, bool ctrl, bool alt)
        {
            std::thread th( pulse_caller::hook, view,shift,ctrl,alt);
            th.detach();
        }
    };
    bool res = false;
    if (view) {
        pulse_caller(view, shift, ctrl, alt);
        res = true;
    }
    return res;
}

bool nnChildApp::handlerPageLeftButton(bool shift, bool ctrl, bool alt)
{

    struct  pulse_caller {
        static void hook(IViewGlue *  view, bool shift, bool ctrl, bool alt)
        {
            view->handlerPageLeftButton(shift, ctrl, alt);
        }
        pulse_caller(IViewGlue *  view, bool shift, bool ctrl, bool alt)
        {
            std::thread th( pulse_caller::hook, view,shift,ctrl,alt);
            th.detach();
        }
    };
    bool res = false;
    if (view) {
        pulse_caller(view, shift, ctrl, alt);
        res = true;
    }
    return res;
}

bool nnChildApp::handlerLeftButton(bool shift, bool ctrl, bool alt)
{
    struct  pulse_caller {
        static void hook(IViewGlue *  view, bool shift, bool ctrl, bool alt)
        {
            view->handlerLeftButton(shift, ctrl, alt);
        }
        pulse_caller(IViewGlue *  view, bool shift, bool ctrl, bool alt)
        {
            std::thread th( pulse_caller::hook, view,shift,ctrl,alt);
            th.detach();
        }
    };
    bool res = false;
    if (view) {
        pulse_caller(view, shift, ctrl, alt);
        res = true;
    }
    return res;
}

bool nnChildApp::handlerRightButton(bool shift, bool ctrl, bool alt)
{
    struct  pulse_caller {
        static void hook(IViewGlue *  view, bool shift, bool ctrl, bool alt)
        {
            view->handlerRightButton(shift, ctrl, alt);
        }
        pulse_caller(IViewGlue *  view, bool shift, bool ctrl, bool alt)
        {
            std::thread th( pulse_caller::hook, view,shift,ctrl,alt);
            th.detach();
        }
    };
    bool res = false;
    if (view) {
        pulse_caller(view, shift, ctrl, alt);
        res = true;
    }
    return res;
}

bool nnChildApp::handlerUpButton(bool shift, bool ctrl, bool alt)
{
    //bool res = false;
    //if (view)
    //    res = view->handlerUpButton(shift, ctrl, alt);
    //return res;
    struct  pulse_caller {
        static void hook(IViewGlue *  view, bool shift, bool ctrl, bool alt)
        {
            view->handlerUpButton(shift, ctrl, alt);
        }
        pulse_caller(IViewGlue *  view, bool shift, bool ctrl, bool alt)
        {
            std::thread th( pulse_caller::hook, view,shift,ctrl,alt);
            th.detach();
        }
    };
    bool res = false;
    if (view) {
        pulse_caller(view, shift, ctrl, alt);
        res = true;
    }
    return res;
}

bool nnChildApp::handlerDownButton(bool shift, bool ctrl, bool alt)
{
    struct  pulse_caller {
        static void hook(IViewGlue *  view, bool shift, bool ctrl, bool alt)
        {
            view->handlerDownButton(shift, ctrl, alt);
        }
        pulse_caller(IViewGlue *  view, bool shift, bool ctrl, bool alt)
        {
            std::thread th( pulse_caller::hook, view,shift,ctrl,alt);
            th.detach();
        }
    };
    bool res = false;
    if (view) {
        pulse_caller(view, shift, ctrl, alt);
        res = true;
    }
    return res;
}

bool nnChildApp::handlerCancelButton(bool shift, bool ctrl, bool alt)
{
    struct  pulse_caller {
        static void hook(IViewGlue *  view, bool shift, bool ctrl, bool alt)
        {
            view->handlerCancelButton(shift, ctrl, alt);
        }
        pulse_caller(IViewGlue *  view, bool shift, bool ctrl, bool alt)
        {
            std::thread th( pulse_caller::hook, view,shift,ctrl,alt);
            th.detach();
        }
    };
    bool res = false;
    if (view) {
        pulse_caller(view, shift, ctrl, alt);
        res = true;
    }
    return res;
}




void  nnChildApp::defaultProcess(size_t type_param, IParam *user_param)
{

    switch (type_param) {
    case action_host_command: {
        nnAbstractParam<int> *t = static_cast<nnAbstractParam<int>*>(user_param);
        if (t) {
            // from conf...xml toolbars
            int v = t->value();
            nnLOG1(int, v);
            switch (v) {
            case 2000:// PLACE NO
                Capture(2000, contactGenericAnd);
                break;
            case 2001://PLACE NC
                Capture(2001, contactGenericOr);
                break;
            case 3000://PLACE COIL
                Capture(3000, coilGeneric);
                break;
            case 5000://CONNECT OBJ
                Capture(5000,connectComponent);
                break;
            case 5002://DELETE OBJ
                handlerCancelButton(false, false, false);
                break;
            default:
                if (externalHandler)
                    externalHandler->doHandler(type_param, user_param);
                break;
            }
        }

    }
    break;
    case action_select_position: {
        if (user_param) {
            nnAbstractParamList *list = static_cast<nnAbstractParamList *>(user_param);
            nnAbstractParam<nnPoint> *p1 = static_cast<nnAbstractParam<nnPoint> *>(list->at(0));
            nnAbstractParam<nnPoint> *p2 = static_cast<nnAbstractParam<nnPoint> *>(list->at(1));
            nnAbstractParam<int> *p3 = static_cast<nnAbstractParam<int> *>(list->at(2));
            nnPoint start = p1->value();
            nnPoint end = p2->value();
            int command = p3->value();
            nnLOG2(nnPoint, start,end);
            nnLOG1(int, command);
            switch (command) {
            case 2000:
                addContact(end, new nnContactNO());
                break;
            case 2001:
                addContact(end, new nnContactNC());
                break;
            case 3000:
                addCoil(end, new nnGenericCoil());
                break;
            case 5000:
                connect(start, end);
                break;
            default:
                if (externalHandler)
                    externalHandler->doHandler(type_param, user_param);
                break;
            }
        }
    }
    break;
    case action_update_selected_panes:
    case action_update_scroller_panes:
    case action_update_statusbars_info:
    case action_redraw:
    case action_align_windows:
    case action_close_windows:
    case action_move_window:
    case action_maximize_windows:
    case action_iconize_windows:
    case action_medialize_windows:
        if (externalHandler)
            externalHandler->doHandler(type_param, user_param);
        break;
    }
    if (user_param)
        delete user_param;
}



