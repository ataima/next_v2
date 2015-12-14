#include "n2AppManager.h"

bool WinEvent::create(unsigned int message, unsigned int wparam, unsigned long lparam)
{
    return false;
}

nnAppManager::nnAppManager()
{
   
}

nnAppManager::~nnAppManager()
{
    clean();
}


childApps * nnAppManager::createObjects(std::wstring & conf_file_name)
{
    bool res = false;
    size_t line = __LINE__;
    childApps * child = new childApps();
    MEMCHK(childApps, child);
    child->imageManager = nullptr;
    child->object_manager = nullptr;
    child->view = nullptr;
    try
    {
        res = createInternalObjects(conf_file_name, *child);
    }
    catch (std::runtime_error *e)
    {
        if (child->object_manager)
            delete child->object_manager;
        if (child->view)
        {
            delete child->view;
        }
        if (child->imageManager)
            delete child->imageManager;
        delete child;
        child = nullptr;
        throw(e);
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
            size_t line = __LINE__;
            miniXmlNode *size_default = conf_manager->find(X("DEFAULT_WIDTH"));
            if (size_default != nullptr)
            {
                size_t default_w = size_default->getLong();
                size_default = conf_manager->find(X("DEFAULT_HEIGHT"));
                if (size_default != nullptr)
                {
                    size_t default_h = size_default->getLong();
                    size_t line = __LINE__;
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
    return res;
}



bool nnAppManager::routeEvents(IEvent * event)
{
    return false;
}

bool nnAppManager::closeAll(void)
{
    return false;
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
        it->second.clean();
        it++;
    }
    return childs.empty();
}


