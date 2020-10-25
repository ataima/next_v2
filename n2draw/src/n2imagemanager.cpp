#include "stdio.h"
#include "images.h"
#include "n2draw.h"
#include "n2imagemanager.h"
#include "n2exception.h"
#include "n2resource.h"


#ifndef _MSC_VER
#include <unistd.h>
#define _MAX_PATH  4096
#else
#include <direct.h>
#endif
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


nnImageManager::nnImageManager(STRING &_path)
{
    availObj.clear();
    allImages.clear();
    path.clear();
    if(_path.size())
        path=_path;
}

nnImageManager::~nnImageManager()
{
    availObj.clear();
    allImages.clear();
    path.clear();
}
#if (_WIN32 || _WIN64)
#define getcwd  _getcwd
#endif


int nnImageManager::internalStringToValue(const XCHAR *ptr)
{
    int res = 0;
    STRING value = ptr;
    if (value == X("CloseWindow"))
        res = 50;
    else if (value == X("MaximizeWindow"))
        res = 51;
    else if (value == X("MedializeWindow"))
        res = 52;
    else if (value == X("IconizeWindow"))
        res = 53;
    if (value == X("ScrollHorzLeft"))
        res=100;
    else if (value == X("ScrollHorzRight"))
        res = 101;
    else if (value == X("ScrollVertUp"))
        res = 102;
    else if (value == X("ScrollVertDown"))
        res = 103;
    return res;
}



bool nnImageManager::readConfiguration(IXmlNode *node)
{
    bool res = false;
    long offset;
    STRING filename;
    IXmlNode *conf = node->find(X("IMAGES"));
    if (conf) {
        IXmlNode *xpath = conf->find(X("PATH"));
        if (xpath) {
            STRING v= xpath->getValue();
            path+=v;
            path+=X("/");
        }
        //////////////////
        IXmlNode *internal = conf->find(X("INTERNAL"));
        if (internal) {
            IXmlNode *t = internal->find(X("OBJ"));
            if (t) {
                do {
                    filename.clear();
                    offset = -1;
                    IXmlNode *e = t->find(X("VALUE"));
                    if (e) {
                        offset = internalStringToValue(e->getValue());
                    }
                    e = t->find(X("FILE"));
                    if (e) {
                        filename = e->getValue();
                    }
                    if (offset >= 0 && !filename.empty()) {
                        if (availObj.find(offset) == availObj.end())
                            availObj[offset] = filename;
                        else {
                            imagesConfigurationAlreadyLoadException *pe = new imagesConfigurationAlreadyLoadException(offset);
                            throw (pe);
                        }
                    } else {
                        imagesConfigurationBadFormatException *pe = new imagesConfigurationBadFormatException();
                        throw (pe);
                    }
                } while ((t = t->getNext()) != nullptr);
            }
        } else {
            xmlConfigurationNodeException *pe = new xmlConfigurationNodeException(X("INTERNAL"));
            throw (pe);
        }

        //////////////////
        IXmlNode *wire = conf->find(X("WIRE"));
        if (wire) {
            IXmlNode *t = wire->find(X("OBJ"));
            if (t) {
                do {
                    filename.clear();
                    offset = -1;
                    IXmlNode *e = t->find(X("VALUE"));
                    if (e) {
                        offset = nnObjWire::wireStringToEnum(e->getValue());
                    }
                    e = t->find(X("FILE"));
                    if (e) {
                        filename = e->getValue();
                    }
                    if (offset >= 0 && !filename.empty()) {
                        if (availObj.find(offset)== availObj.end())
                            availObj[offset] = filename;
                        else {
                            imagesConfigurationAlreadyLoadException *pe = new imagesConfigurationAlreadyLoadException(offset);
                            throw (pe);
                        }
                    } else {
                        imagesConfigurationBadFormatException *pe=new imagesConfigurationBadFormatException();
                        throw (pe);
                    }
                } while ((t = t->getNext()) != nullptr);
            }
        } else {
            xmlConfigurationNodeException *pe = new xmlConfigurationNodeException(X("WIRE"));
            throw (pe);
        }
        IXmlNode *contact = conf->find(X("CONTACT"));
        if (contact) {
            IXmlNode *t = contact->find(X("OBJ"));
            if (t) {
                do {
                    filename.clear();
                    offset = -1;
                    IXmlNode *e = t->find(X("VALUE"));
                    if (e) {
                        offset = nnObjComponent::getCustomizationFromName(e->getValue());
                    }
                    e = t->find(X("FILE"));
                    if (e) {
                        filename = e->getValue();
                    }
                    if (offset >0 && !filename.empty()) {
                        if (availObj.find(offset) == availObj.end())
                            availObj[offset] = filename;
                        else {
                            imagesConfigurationAlreadyLoadException *pe = new imagesConfigurationAlreadyLoadException(offset);
                            throw (pe);
                        }
                    } else {
                        imagesConfigurationBadFormatException *pe = new imagesConfigurationBadFormatException();
                        throw (pe);
                    }
                } while ((t = t->getNext()) != nullptr);
            }
        } else {
            xmlConfigurationNodeException *pe = new xmlConfigurationNodeException(X("CONTACT"));
            throw (pe);
        }
        IXmlNode *coil = conf->find(X("COIL"));
        if (coil) {
            IXmlNode *t = coil->find(X("OBJ"));
            if (t) {
                do {
                    filename.clear();
                    offset = -1;
                    IXmlNode *e = t->find(X("VALUE"));
                    if (e) {
                        offset = nnObjComponent::getCustomizationFromName(e->getValue());
                    }
                    e = t->find(X("FILE"));
                    if (e) {
                        filename = e->getValue();
                    }
                    if (offset > 0 && !filename.empty()) {
                        if (availObj.find(offset) == availObj.end())
                            availObj[offset] = filename;
                        else {
                            imagesConfigurationAlreadyLoadException *pe = new imagesConfigurationAlreadyLoadException(offset);
                            throw (pe);
                        }
                    } else {
                        imagesConfigurationBadFormatException *pe = new imagesConfigurationBadFormatException();
                        throw (pe);
                    }
                } while ((t = t->getNext()) != nullptr);
            }
        } else {
            xmlConfigurationNodeException *pe = new xmlConfigurationNodeException(X("COIL"));
            throw (pe);
        }
    } else {
        xmlConfigurationNodeException *pe = new xmlConfigurationNodeException(X("IMAGES"));
        throw (pe);
    }

    return res= !availObj.empty();
}

bool nnImageManager::loadImages(void)
{
    bool res = false;
    if (availObj.size() > 0) {
        STRING filenameabs;
        objImageList::iterator it = availObj.begin();
        objImageList::iterator _end = availObj.end();
        while (it != _end) {
            const unsigned char *lpbmp=nullptr;
            size_t sizebmp=0;
            bmpImage image;
            filenameabs = path;
            filenameabs += it->second;
            res=nnResource::Get(filenameabs.c_str(),&lpbmp,&sizebmp);
            if(res==false) {
                image.clone((LPBITMAPFILEHEADER)(lpbmp));
                if (image.getBitsPerPixel() < 32)
                    image.convertTo32Bits();
#if _MSC_VER
                image.show(10, 10);
#endif
                //TO DO STRECT TO FIT
                allImages.Add(it->first,image);
            } else {
                imagesConfigurationUnknowFileException *pe=new imagesConfigurationUnknowFileException(filenameabs);
                throw (pe);
            }
            it++;
        }
        res = true;
    } else {
        imagesConfigurationListEmptyException *pe= new imagesConfigurationListEmptyException();
        throw (pe);
    }
    return res;
}

bool nnImageManager::loadImages(const listCommandItem * items)
{
    bool res = false;
    if(items!=nullptr ) {
        availObj.clear();
        size_t i,s=items->size();
        if(s) {
            for(i=0; i<s; i++) {
                commandItem * it= items->at(i);
                availObj[it->command]=it->file;
            }
        }
    }
    res=loadImages();
    return res;
}

bmpImage * nnImageManager::getImage(const XCHAR * name)
{
    int index = internalStringToValue(name);
    return getImage(index);
}


bmpImage * nnImageManager:: getImage(int id)
{
    bmpImage *res=nullptr;
    if (allImages.size()>0) {
        listImage::const_iterator it = allImages.find(id);
        if (it != allImages.end()) {
            res = it->second;
        }
    }
    return res;
}

bool nnImageManager::setPath(STRING &_path)
{
    path.clear();
    path=_path;
    return true;
}
