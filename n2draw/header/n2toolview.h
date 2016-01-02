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
    IImageManager *images;
    ICommander *active;
    int pos_sel;
public:
    nnToolView(IImageManager *_images);
    ~nnToolView();
    bool readConfiguration(IXmlNode *node);
    bool draw(bmpImage & bkg,int x, int y,void * context);
    bool handlerRequestCommand( nnPoint & pos,int & command);
    inline ICommander *getActiveCommander(void){return active;}
};


#endif // N2TOOLVIEW

