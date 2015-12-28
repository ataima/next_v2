#ifndef MDISELECTOR
#define MDISELECTOR


#include <QWidget>
#include <QStylePainter>

class mdiSelector : public QObject
{

    nnRect pos;
    nnPoint stop,start;
    bool visible;
public :
    mdiSelector(void);
    /* mdi child area coord */
    void SetArea(nnRect &rect,nnPoint &_start,nnPoint & _stop );
    void draw(QPainter & p);
    inline void hide(void) {visible=false;}
    inline void show(void){visible=true;}
    inline bool getStatus(void) {return visible;}
};


#endif // MDISELECTOR

