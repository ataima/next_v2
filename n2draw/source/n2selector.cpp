
#include "images.h"
#include "n2draw.h"
#include "n2selector.h"
#include "n2exception.h"




nnSelector::nnSelector(void)
{
    hide();
    error = false;
}

void nnSelector::draw(bmpImage & image, const nnPoint &start, const nnPoint &stop,
   const nnPoint &sel_start, const nnPoint &sel_stop )
{
    if (visible)
    {
        nnPoint start1 = start;
        nnPoint stop1 = stop;
        start1 += 2;
        stop1 -= 2;
        unsigned int height = image.getHeight();
        if (error)
        {
            image.frameRect(start.x, height - start.y, stop.x, height - stop.y, 255, 0, 0, 0xcccccccc);
            image.frameRect(start1.x, height - start1.y, stop1.x, height - stop1.y, 255, 0, 0, 0x66666666);
        }
        else
        {
            image.frameRect(start.x, height - start.y, stop.x, height - stop.y, 128, 128, 128, 0xcccccccc);
            image.frameRect(start1.x, height - start1.y, stop1.x, height - stop1.y, 128, 128, 128, 0x66666666);
        }
        if (font)
        {
            char buff[128];
            sprintf(buff, "%d:%d", sel_start.x, sel_start.y);
            bmpImage * strImage = font->getImage(buff,16,16,224);
            image.drawMaskSprite(*strImage,start.x, height-start.y+2,0,0,0);
            delete strImage;
            sprintf(buff, "%d:%d", sel_stop.x, sel_stop.y);
            strImage = font->getImage(buff, 16, 16, 224);
            image.drawMaskSprite(*strImage, stop.x-(8*strlen(buff)), height - stop.y-12, 0, 0, 0);
            delete strImage;
        }
    }
}

