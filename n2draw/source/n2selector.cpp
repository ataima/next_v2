
#include "images.h"
#include "n2draw.h"
#include "n2selector.h"
#include "n2exception.h"



nnSelector::nnSelector(void)
{
    hide();
    error = false;
    toAlpha = "ABCDEFGHIJLMNOPQRSTUVXYZ";
}


bool nnSelector::translateY( int p, std::string & out )
{
    bool res = false;
    out.clear();
    int w = toAlpha.size();
    int maxW = w;
    if (p > w)
    {
        while (maxW < p)
        {
            maxW *= w;
        }
        while (p > w)
        {
            int t = p / maxW;
            if (t > 0)
            {
                out += toAlpha[t - 1];
            }
            p %= maxW;
            maxW /= w;
        }
        out += toAlpha[p];
    }
    else
    {
        out += toAlpha[p];
    }
    res = (out.size() > 0);
    return res;
}

void nnSelector::draw(bmpImage & image, 
   const nnPoint &sel_start, const nnPoint &sel_stop, IViewGlue * glue)
{
    if (visible)
    {
        nnPoint start, stop,size;
        size = glue->getConstPhy();
        start = glue->getCoordPhy( sel_start);
        stop  = glue->getCoordPhy( sel_stop);        
        stop += size;
        unsigned int height=image.getHeight();
        start.y = height - start.y;
        stop.y = height - stop.y;
        start-=2;
        stop += 2;
        if (error)
        {
            image.frameRect(start.x, start.y, stop.x, stop.y, 255, 0, 0, 0xcccccccc);
            start += 4;
            stop -= 4;
            image.frameRect(start.x,start.y, stop.x, stop.y, 255, 0, 0, 0x66666666);
        }
        else
        {
            image.frameRect(start.x, start.y, stop.x, stop.y, 0, 128, 0, 0xcccccccc);
            start += 4;
            stop -= 4;
            image.frameRect(start.x,start.y, stop.x, stop.y, 0, 0, 128, 0x66666666);
        }
        if (font)
        {
            bmpImage * strImage;
            std::string conv;
            char buff[128];
            if (translateY(sel_start.x, conv))
            {
                sprintf(buff, "%s:%d", conv.c_str(), sel_start.x);
                strImage = font->getImage(buff, 16, 16, 224);
                image.drawMaskSprite(*strImage, start.x+2, start.y -14, 0, 0, 0);
                delete strImage;
            }
            if (translateY(sel_stop.x, conv))
            {
                sprintf(buff, "%s:%d", conv.c_str(), sel_stop.x);
                strImage = font->getImage(buff, 16, 224, 16);
                image.drawMaskSprite(*strImage, stop.x - (8 * strlen(buff))-2, stop.y +4, 0, 0, 0);
                delete strImage;
            }
        }
    }
}

