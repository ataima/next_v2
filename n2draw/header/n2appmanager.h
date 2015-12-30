#ifndef NEW_NEXT_APP_MANAGER_HEADER
#define NEW_NEXT_APP_MANAGER_HEADER

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

#include "n2miniXml.h"
#include "n2draw.h"
#include "n2drawmanager.h"
#include "n2imagemanager.h"
#include "n2view.h"
#include "n2viewglue.h"
#include "n2utoatou.h"





typedef struct tag_app_child
{
    IManager                *object_manager;
    IViewGlue               *view;
    IImageManager           *imageManager;
    void clean(void);
} childApps;



class IEvent
{
public:
    //stub for windows
    virtual bool create(unsigned int  message, unsigned int wparam, unsigned long lparam) = 0;
    virtual ~IEvent(){}
};



class WinEvent
    :public IEvent
{
public:
    bool create(unsigned int  message, unsigned int wparam, unsigned long lparam);
};


class IAppManager
{
public:
    virtual childApps * createObjects(std::u16string & conf_file_name) = 0;
    virtual bool routeEvents(IEvent * event) = 0;
    virtual bool closeAll(void) = 0;
    virtual childApps *activate(int v) = 0;
    virtual childApps *active(void) = 0;
    virtual ~IAppManager(){}
};

typedef std::map<int, childApps *> listChild;


class nnAppManager
    :public IAppManager
{
    listChild childs;
    xmlConfig configuration; 
    int selected;
    static int UID;
public:
    nnAppManager();
    ~nnAppManager();    
    childApps * createObjects(std::u16string & conf_file_name);
    bool routeEvents(IEvent * event);
    bool closeAll(void);
    childApps *activate(int v);
    childApps *active(void);
protected:
    bool clean(void);
    bool createInternalObjects(std::u16string & conf_file_name, childApps & child);

};





#endif
