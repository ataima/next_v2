#include "n2appmanager.h"

bool WinEvent::create(unsigned int message, unsigned int wparam, unsigned long lparam)
{
    (message);
    (wparam);
    (lparam);
    return false;
}


int nnAppManager::UID = 1;

nnAppManager::nnAppManager():selected(-1)
{
    
}

nnAppManager::~nnAppManager()
{
    clean();
}


childApps * nnAppManager::createObjects(std::wstring & conf_file_name)
{
    bool res = false;
    childApps * child = new childApps();
    MEMCHK(childApps, child);
    child->imageManager = nullptr;
    child->object_manager = nullptr;
    child->view = nullptr;
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
        }
    }
    return child;
}

bool nnAppManager::createInternalObjects(std::wstring & conf_file_name, childApps & child)
{
    bool res = false;
    res = configuration.readConfiguration(conf_file_name.c_str());
    if (res == true)
    {
        miniXmlNode *conf_manager = configuration.getRoot().find(X("APP"));
        if (conf_manager != nullptr)
        {
            //int line = __LINE__;
            miniXmlNode *size_default = conf_manager->find(X("DEFAULT_WIDTH"));
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
                    conf_manager = configuration.getRoot().find(X("MANAGER"));
                    if (conf_manager)
                    {
                        res = child.object_manager->readConfiguration(*conf_manager);
                        if (res)
                        {
                            line = __LINE__;
                            child.imageManager = new nnImageManager();
                            MEMCHK(IImageManager, child.imageManager);
                            conf_manager = configuration.getRoot().find(X("IMAGES"));
                            if (conf_manager)
                            {
                                res = child.imageManager->readConfiguration(conf_manager);
                            }
                            else
                            {
                                appManagerConfigureFileMissingNodeException  *e = new appManagerConfigureFileMissingNodeException(X("IMAGES"));
                                throw(e);
                            }
                            if (res)
                            {
                                conf_manager = configuration.getRoot().find(X("PHY_MAP"));
                                if (conf_manager)
                                {
                                    line = __LINE__;
                                    child.view = new nnViewGlue(child.object_manager, child.imageManager );
                                    IViewGlue *viewglue = child.view;
                                    MEMCHK(IViewGlue, viewglue);
                                    res = viewglue->readConfiguration(*conf_manager);
                                }
                                else
                                {
                                    appManagerConfigureFileMissingNodeException  *e = new appManagerConfigureFileMissingNodeException(X("PHY_MAP"));
                                    throw(e);
                                }
                            }
                        }
                    }
                    else
                    {
                        appManagerConfigureFileMissingNodeException  *e = new appManagerConfigureFileMissingNodeException(X("MANAGER"));
                        throw(e);
                    }
                }
                else
                {
                    appManagerConfigureFileMissingNodeException  *e = new appManagerConfigureFileMissingNodeException(X("DEFAULT_HEIGHT"));
                    throw(e);
                }
            }
            else
            {
                appManagerConfigureFileMissingNodeException  *e = new appManagerConfigureFileMissingNodeException(X("DEFAULT_WIDTH"));
                throw(e);
            }
        }
        else
        {
            appManagerConfigureFileMissingNodeException  *e = new appManagerConfigureFileMissingNodeException(X("MANAGER"));
            throw(e);
        }
    }
    else
    {
        appManagerConfigureFileUnknow  *e = new appManagerConfigureFileUnknow(conf_file_name);
        throw(e);
    }
    return res;
}



bool nnAppManager::routeEvents(IEvent * event)
{
    (event);
    return false;
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
    if (view)
    {
        delete view;
        view = nullptr;
    }
}



bool nnAppManager::clean(void)
{
    listChild::iterator it = childs.begin();
    listChild::iterator _end = childs.end();
    while (it != _end)
    {
        it->second->clean();
        delete it->second;
        it++;
    }
    return childs.empty();
}


