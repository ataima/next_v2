
#ifndef N2IMAGE_MANAGER__HEADER
#define N2IMAGE_MANAGER__HEADER


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

#include "images.h"
#include "n2interfaces.h"







class nnImageManager
    : public IImageManager
{
    STRING path;
    objImageList availObj;
    listImage allImages;
public:
    nnImageManager(STRING & _path);
    ~nnImageManager();
    bool setPath(STRING &_path);
    TOSTRING(nnImageManager);
    inline STRING  & getDefaulPath(void) const
    {
        return *const_cast<STRING*>(&path);
    }
    bool readConfiguration(IXmlNode *node);
    bool loadImages(void);
    bool loadImages(const listCommandItem *items);
    bmpImage * getImage(int id);
    bmpImage * getImage(const XCHAR * name);
    inline  const objImageList * getAvailObj(void)
    {
        return &availObj;
    }
    inline int getMaxWidth(void)
    {
        return allImages.getMaxWidth();
    }
    inline int getMaxHeight(void)
    {
        return allImages.getMaxHeight();
    }
private :
    int internalStringToValue(const XCHAR *ptr);
};


#endif
