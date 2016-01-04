#ifndef N2TOOLVIEW
#define N2TOOLVIEW



#include "images.h"
#include "n2imagemanager.h"
#include "n2commander.h"


typedef std::vector<ICommander *> commanderList ;

class nnToolView
        :public IToolView
{
    commanderList commands;
    ICommander *active;
    int pos_sel;
public:
    nnToolView();
    ~nnToolView();
    bool readConfiguration(IXmlNode *node);
    bool draw(bmpImage & bkg,nnPoint & pos,void * context);
    bool handlerRequestCommand( nnPoint & pos,int & command);
    inline ICommander *getActiveCommander(void){return active;}
    bool loadImages(const XCHAR *path);
};


#endif // N2TOOLVIEW

