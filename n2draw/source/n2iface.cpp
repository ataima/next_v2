#include "n2iface.h"
#include "n2appmanager.h"

static nnAppManager * current =nullptr;




IAppManager *  n2draw_get_instance(void)
{
    if(current==nullptr) {
        try {
            current=static_cast<nnAppManager*>(IAppManager::getInstance(0));
            if (current) {
                STRING conf("conf_utf8.xml");
                current->create(conf);
            }
        } catch(...) {
            current=nullptr;
        }
    }
    return current;
}

bool   n2draw_close_all_childs(void)
{
    bool res=false;
    if (current) {
        try {
            res=current->closeAll();
        } catch(...) {
            res=false;
        }
    }
    return res;
}

IChild * n2draw_create_child(const char *file)
{
    IChild *res=nullptr;
    if(file!=nullptr && current !=nullptr ) {
        try {
            STRING conf(file);
            res=current->create(conf);
        } catch(...) {
            res=nullptr;
        }
    }
    return res;
}

IChild * n2draw_activate_child(int v)
{
    IChild *res=nullptr;
    if( current !=nullptr ) {
        try {
            res=current->activate(v);
        } catch(...) {
            res=nullptr;
        }
    }
    return res;
}

IChild * n2draw_get_active_child(void)
{
    IChild *res=nullptr;
    if( current !=nullptr ) {
        try {
            res=current->active();
        } catch(...) {
            res=nullptr;
        }
    }
    return res;
}


