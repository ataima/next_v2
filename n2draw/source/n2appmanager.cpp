#include "n2draw.h"
#include "n2miniXml.h"
#include "n2drawmanager.h"
#include "n2imagemanager.h"
#include "n2view.h"
#include "n2viewglue.h"
#include "n2exception.h"
#include "n2fontmanager.h"
#include "n2fontlist.h"

#include "n2appmanager.h"

int nnAppManager::UID = 1;

nnAppManager::nnAppManager():selected(-1)
{
    configuration = new xmlConfig();
}

nnAppManager::~nnAppManager()
{
    clean();
    delete configuration;
    configuration=nullptr;
}


childApps * nnAppManager::createObjects(STRING & conf_file_name)
{
    bool res = false;
    childApps * child = new childApps();
    MEMCHK(childApps, child);
    child->imageManager = nullptr;
    child->object_manager = nullptr;
    child->view = nullptr;
    child->fonts = nullptr;
    try
    {
        res = createInternalObjects(conf_file_name, *child);
    }
    catch (n2exception *e)
    {
        child->clean();
        delete child;
        child = nullptr;
        throw(e);
    }
    if(res==false)
    {
        child->clean();
        delete child;
        child = nullptr;
    }
    if (child != nullptr)
    {
        selected = static_cast<int>(UID);
        childs[UID++] = child;
    }
    // OK is Valid;
    if(child != nullptr)
    {
        bool res;
        nnPoint p=child->view->getCoordPhy(1,1);
        if(p.x != 0 && p.y != 0)
        {
            res=child->imageManager->loadImages(p.x, p.y);
            if(!res)
            {
                child->clean();
                delete child;
                child = nullptr;
                appManagerConfigureLoadImageException *e=new appManagerConfigureLoadImageException();
                throw(e);
            }
            res=child->view->loadImages(child->imageManager->getDefaulPath().c_str());
            if(!res)
            {
                child->clean();
                delete child;
                child = nullptr;
                appManagerConfigureLoadImageException *e=new appManagerConfigureLoadImageException();
                throw(e);
            }
            res = child->fonts->loadImages();
            if (!res)
            {
                child->clean();
                delete child;
                child = nullptr;
                appManagerConfigureLoadImageException *e = new appManagerConfigureLoadImageException();
                throw(e);
            }
        }
    }
    return child;
}

bool nnAppManager::createInternalObjects(STRING & conf_file_name, childApps & child)
{
    bool res = false;
    if(configuration!=nullptr)
    {
        res = configuration->readConfiguration(conf_file_name.c_str());
        if (res == true)
        {
            IXmlNode * root=configuration->getRoot();
            if(root!=nullptr)
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
                            child.object_manager = new nnObjManager(default_w, default_h);
                            MEMCHK(IManager, child.object_manager);
                            conf_manager = root->find(X("MANAGER"));
                            if (conf_manager)
                            {
                                res = child.object_manager->readConfiguration(conf_manager);
                                if (res)
                                {
                                    line = __LINE__;
                                    child.imageManager = new nnImageManager();
                                    MEMCHK(IImageManager, child.imageManager);
                                    conf_manager = root->find(X("IMAGES"));
                                    if (conf_manager)
                                    {
                                        res = child.imageManager->readConfiguration(conf_manager);
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
                                                child.fonts = new nnFontList();
                                                IFontList *flist = child.fonts;
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
                                                    child.view = new nnViewGlue(child.object_manager, child.imageManager,child.fonts);
                                                    IViewGlue *viewglue = child.view;
                                                    MEMCHK(IViewGlue, viewglue);
                                                    res = viewglue->readConfiguration(conf_manager);
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
    return res;
}




bool nnAppManager::closeAll(void)
{
    clean();
    selected = -1;
    return true;
}


childApps *nnAppManager::activate(int v)
{
    childApps * res = nullptr;
    if (!childs.empty())
    {
        listChild::iterator it = childs.find(v);
        if (it != childs.end())
        {
            res = it->second;
            selected = v;
            res->view->updateDraw();
        }
    }
    return res;
}


childApps *nnAppManager::active(void)
{
    childApps * res = nullptr;
    if (!childs.empty() && selected>0)
    {
        listChild::iterator it = childs.find(selected);
        if (it != childs.end())
        {
            res = it->second;
            res->view->updateDraw();
        }
    }
    return res;
}






void childApps::clean(void)
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
}



bool nnAppManager::clean(void)
{
    for (auto i : childs)
    {
        i.second->clean();
    }
    return childs.empty();
}

bool nnAppManager::setExtHandler(childApps * child, handler_exec type, extHandler _hook, void *unkObj)
{
    bool res=false;
    if(child)
    {
       res=child->view->addExtHandler(type,_hook,unkObj);
    }
    return res;
}
