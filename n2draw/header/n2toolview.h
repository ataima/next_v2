#ifndef N2TOOLVIEW
#define N2TOOLVIEW



#include "images.h"
#include "n2imagemanager.h"
#include "n2commander.h"


typedef std::map<int ,ICommander *> commanderList ;

class nnToolView
        :public IToolView
{
    nnPoint phyPos;
    commanderList commands;
    ICommander *active;
    IFontManager *font;
public:
    nnToolView();
    ~nnToolView();
    bool readConfiguration(IXmlNode *node);
    bool draw(bmpImage & bkg, IViewGlue * glue);
    bool handlerRequestCommand( nnPoint & pos,int & command);
    bool handlerMouseMove( nnPoint & pos,IExtHandler *hook);
    inline ICommander *getActiveCommander(void){return active;}
    bool loadImages(const XCHAR *path);
    bool checkIntCommand(int command);
    inline void setFont(IFontManager *_font) { font = _font; }
    inline void setDrawPosition(nnPoint & p){ phyPos=p;}
};


#endif // N2TOOLVIEW

