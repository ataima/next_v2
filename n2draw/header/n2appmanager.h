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


#include "n2interfaces.h"





class nnAppManager
    :public IAppManager
{

typedef std::map<int, IChild *> listChild;

    listChild childs;
    IConfig *configuration;
    int selected;
    static int UID;    
public:
    nnAppManager();
    ~nnAppManager();    
    IChild * createObjects(STRING & conf_file_name);
    bool closeAll(void);
    IChild *activate(int v);
    IChild *active(void);

protected:
    bool clean(void);
};





#endif
