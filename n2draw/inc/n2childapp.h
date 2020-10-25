#ifndef N2CHILDAPP_HEADER
#define N2CHILDAPP_HEADER


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



class nnChildApp
    : public IChild
{

    IManager                *object_manager;
    IFontList               *fonts;
    IViewGlue               *view;
    IImageManager           *imageManager;
    IExtHandler             *externalHandler;    
    IExtHandler             *defaultHandler;
    unsigned int            id;
public:
    nnChildApp(unsigned int _id=0);
    ~nnChildApp();
    void clean(void);
    bool createObjects(IConfig *configuration,STRING & conf_file_name,STRING &confPath);
    bool setExtHandler( extHandler  _hook, void *unkObj);
    //IHandler * getHandler(void) { return static_cast<IHandler *>(this); }
    inline  IManager * getManager(void)                { return object_manager; }
    inline  IFontList * getFont(void)                  { return fonts; }
    inline  IViewGlue * getView(void)                  { return view; }
    inline  IImageManager * getImage(void)             { return imageManager; }
    inline  IExtHandler * getHandler(void)             { return defaultHandler; }
    // remap handler
    bool handlerMouseMove(nn_mouse_buttons buttons, nnPoint & phyPoint) ;
    bool handlerMouseButtonDown(nn_mouse_buttons buttons, nnPoint & phyPoint) ;
    bool handlerMouseButtonUp(nn_mouse_buttons buttons, nnPoint & phyPoint) ;
    bool handlerScrollHorz(int pos) ;
    bool handlerScrollVert(int pos) ;
    bool handlerEscapeButton(bool shift, bool ctrl, bool alt) ;
    bool handlerHomeButton(bool shitf, bool ctrl, bool alt) ;
    bool handlerEndButton(bool shitf, bool ctrl, bool alt) ;
    bool handlerPageUpButton(bool shitf, bool ctrl, bool alt) ;
    bool handlerPageDownButton(bool shitf, bool ctrl, bool alt) ;
    bool handlerPageRightButton(bool shitf, bool ctrl, bool alt) ;
    bool handlerPageLeftButton(bool shitf, bool ctrl, bool alt) ;
    bool handlerLeftButton(bool shitf, bool ctrl, bool alt) ;
    bool handlerRightButton(bool shitf, bool ctrl, bool alt) ;
    bool handlerUpButton(bool shitf, bool ctrl, bool alt) ;
    bool handlerDownButton(bool shitf, bool ctrl, bool alt) ;
    bool handlerCancelButton(bool shitf, bool ctrl, bool alt);
    inline  bool getCurrentFile(std::string & filename) { filename = "prova"; return true; }
    void defaultProcess( size_t type_param, IParam *user_param);
private:
    static void  defaultCommandRequest(void * dest, size_t type_param, IParam *user_param);
    bool Capture(int command, unsigned int image);
    bool addContact(nnPoint & pos, nnObjContact * contact);
    bool addCoil(nnPoint & pos, nnObjCoil * coil);
    bool connect(nnPoint & start, nnPoint & end);
    bool loadImages(void);
};



#endif // N2CHILDAPP_HEADER

