
#ifndef N2IMAGE_MANAGER__HEADER
#define N2VIEWN2IMAGE_MANAGER__HEADER_HEADER


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



typedef  std::map<long, std::wstring> objImageList;


class IImageManager
{
public:
    virtual std::wstring  getDefaulPath(void) const = 0;
    virtual bool readConfiguration(miniXmlNode *node) = 0;
    virtual bool loadImages(size_t w, size_t h) = 0;
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
    bool loadImages(size_t w, size_t h);
    inline  const listImage * getImageList(void) { return &allImages; }
    inline  const objImageList * getAvailObj(void) { return &availObj; }
};


// no conf images 
class imagesConfigurationException
    :public std::runtime_error
{
public:
    explicit imagesConfigurationException(void) throw()
        :runtime_error("imagesConfigurationException") {}

};


class imagesConfigurationNoWireException
    :public std::runtime_error
{
public:
    explicit imagesConfigurationNoWireException(void) throw()
        :runtime_error("imagesConfigurationNoWireException") {}
};


class imagesConfigurationNoContactException
    :public std::runtime_error
{
public:
    explicit class imagesConfigurationNoContactException(void) throw()
        :runtime_error("imagesConfigurationNoContactException") {}
};

class imagesConfigurationNoCoilException
    :public std::runtime_error
{
public:
    explicit class imagesConfigurationNoCoilException(void) throw()
        :runtime_error("imagesConfigurationNoCoilException") {}
};


class imagesConfigurationListEmptyException
    :public std::runtime_error
{
public:
    explicit class imagesConfigurationListEmptyException(void) throw()
        :runtime_error("imagesConfigurationListEmptyException") {}
};


class imagesConfigurationBadSizeException
    :public std::runtime_error
{
    size_t w, h;
public:
    explicit class imagesConfigurationBadSizeException(size_t _w, size_t _h) throw()
        :runtime_error("imagesConfigurationBadSizeException"), w(_w), h(_h) {}
};


class imagesConfigurationBadFormatException
    :public std::runtime_error
{
public:
    explicit class imagesConfigurationBadFormatException(void) throw()
        :runtime_error("imagesConfigurationBadFormatException") {}
};


class imagesConfigurationUnknowFileException
    :public std::runtime_error
{
    std::wstring filename;
public:
    explicit class imagesConfigurationUnknowFileException(std::wstring _filename) throw()
        :runtime_error("imagesConfigurationUnknowFileException"), filename(_filename) {}
};
#endif