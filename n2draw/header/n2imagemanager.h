
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

#include <string>
#include <vector>
#include "images.h"
#include "n2miniXml.h"
#include <map>
#include "n2exception.h"



typedef  std::map<int, std::wstring> objImageList;


class IImageManager
{
public:
    virtual std::wstring  getDefaulPath(void) const = 0;
    virtual bool readConfiguration(miniXmlNode *node) = 0;
    virtual bool loadImages(int w, int h) = 0;
    virtual const listImage * getImageList(void) = 0;
    virtual const  objImageList * getAvailObj(void) = 0;
};




class nnImageManager
    : public IImageManager
{
    std::wstring path;
    objImageList availObj;
    listImage allImages;
public:
    nnImageManager();
    ~nnImageManager();
    inline  std::wstring  getDefaulPath(void) const { return path; };
    bool readConfiguration(miniXmlNode *node);
    bool loadImages(int w, int h);
    inline  const listImage * getImageList(void) { return &allImages; }
    inline  const objImageList * getAvailObj(void) { return &availObj; }
};


// no conf images 
#endif
