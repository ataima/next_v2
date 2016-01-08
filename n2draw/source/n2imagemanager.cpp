#include "stdio.h"
#include "images.h"
#include "n2draw.h"
#include "n2imagemanager.h"
#include "n2exception.h"

#ifndef _MSC_VER
#include <unistd.h>
#define _MAX_PATH  4096
#else
#include <direct.h>
#endif


nnImageManager::nnImageManager(const XCHAR *_path)
{
    availObj.clear();
    allImages.clear();
    path.clear();
    if(_path)
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

bool nnImageManager::readConfiguration(IXmlNode *node)
{
    bool res = false;
    long offset;
    STRING filename;
    IXmlNode *conf = node->find(X("IMAGES"));
    if (conf)
    {
        IXmlNode *xpath = conf->find(X("PATH"));
        if (xpath)
        {
            char buff[_MAX_PATH];
            const XCHAR *v = xpath->getValue();
            if(getcwd(buff,_MAX_PATH))
            {
                std::string cpath(buff);
                AtoU toU(cpath);
                path=toU.utf16();
                path+=X("/");
                path+=v;
                path+=X("/");
            }
            else
            {
                path=v;
            }
        }
        IXmlNode *wire = conf->find(X("WIRE"));
        if (wire)
        {
            IXmlNode *t = wire->find(X("OBJ"));
            if (t)
            {
                do {
                    filename.clear();
                    offset = -1;
                    IXmlNode *e = t->find(X("VALUE"));
                    if (e)
                    {
                        offset = nnObjWire::wireStringToEnum(e->getValue());
                    }
                    e = t->find(X("FILE"));
                    if (e)
                    {
                        filename = e->getValue();
                    }
                    if (offset >= 0 && !filename.empty())
                    {
                        if (availObj.find(offset)== availObj.end())
                            availObj[offset] = filename;
                        else
                        {
                            imagesConfigurationAlreadyLoadException *pe = new imagesConfigurationAlreadyLoadException(offset);
                            throw (pe);
                        }
                    }
                    else
                    {
                        imagesConfigurationBadFormatException *pe=new imagesConfigurationBadFormatException();
                        throw (pe);
                    }
                } while ((t = t->getNext()) != nullptr);
            }
        }
        else
        {
            imagesConfigurationNoWireException *pe=new imagesConfigurationNoWireException();
            throw (pe);
        }
        IXmlNode *contact = conf->find(X("CONTACT"));
        if (contact)
        {
            IXmlNode *t = contact->find(X("OBJ"));
            if (t)
            {
                do {
                    filename.clear();
                    offset = -1;
                    IXmlNode *e = t->find(X("VALUE"));
                    if (e)
                    {
                        offset = nnObjComponent::getCustomizationFromName(e->getValue());
                    }
                    e = t->find(X("FILE"));
                    if (e)
                    {
                        filename = e->getValue();
                    }
                    if (offset >0 && !filename.empty())
                    {
                        if (availObj.find(offset) == availObj.end())
                            availObj[offset] = filename;
                        else
                        {
                            imagesConfigurationAlreadyLoadException *pe = new imagesConfigurationAlreadyLoadException(offset);
                            throw (pe);
                        }
                    }
                    else
                    {
                        imagesConfigurationBadFormatException *pe = new imagesConfigurationBadFormatException();
                        throw (pe);
                    }
                } while ((t = t->getNext()) != nullptr);
            }
        }
        else
        {
            imagesConfigurationNoContactException *pe=new imagesConfigurationNoContactException();
            throw (pe);
        }
        IXmlNode *coil = conf->find(X("COIL"));
        if (coil)
        {
            IXmlNode *t = coil->find(X("OBJ"));
            if (t)
            {
                do {
                    filename.clear();
                    offset = -1;
                    IXmlNode *e = t->find(X("VALUE"));
                    if (e)
                    {
                        offset = nnObjComponent::getCustomizationFromName(e->getValue());
                    }
                    e = t->find(X("FILE"));
                    if (e)
                    {
                        filename = e->getValue();
                    }
                    if (offset > 0 && !filename.empty())
                    {
                        if (availObj.find(offset) == availObj.end())
                            availObj[offset] = filename;
                        else
                        {
                            imagesConfigurationAlreadyLoadException *pe = new imagesConfigurationAlreadyLoadException(offset);
                            throw (pe);
                        }
                    }
                    else
                    {
                        imagesConfigurationBadFormatException *pe = new imagesConfigurationBadFormatException();
                        throw (pe);
                    }
                } while ((t = t->getNext()) != nullptr);
            }
        }
        else
        {
            imagesConfigurationNoCoilException *pe=new imagesConfigurationNoCoilException();
            throw (pe);
        }
    }
    else
    {
        imagesConfigurationException *pe=new imagesConfigurationException();
        throw (pe);
    }
    
    return res= !availObj.empty();
}

bool nnImageManager::loadImages(int w, int h)
{
    bool res = false;
    if (availObj.size() > 0)
    {
        STRING filenameabs;
        objImageList::iterator it = availObj.begin();
        objImageList::iterator _end = availObj.end();
        while (it != _end)
        {
            bmpImage image;
            filenameabs = path;
            filenameabs += it->second;
            if (image.copyFromFile(filenameabs.c_str()))
            {
                if (w!= -1 && h!=-1 && (image.getWidth() != w || image.getHeight() != h))
                {
                    imagesConfigurationBadSizeException *pe=new imagesConfigurationBadSizeException(filenameabs.c_str(),image.getWidth(),image.getHeight());
                    throw (pe);
                }
                else
                {
                    if (image.getBitsPerPixel() != 24)
                        image.convertTo24Bits();
                    //TO DO STRECT TO FIT
                    allImages.Add(it->first,image);
                }
            }
            else
            {
                imagesConfigurationUnknowFileException *pe=new imagesConfigurationUnknowFileException(filenameabs);
                throw (pe);
            }
            it++;
        }
        res = true;
    }
    else
    {
        imagesConfigurationListEmptyException *pe= new imagesConfigurationListEmptyException();
        throw (pe);
    }
    return res;
}

bool nnImageManager::loadImages(objImageList * extlist)
{
    bool res = false;
    if(extlist)
    {
        if(availObj.size() > 0)
            availObj.clear();
        availObj=*extlist;
        res=loadImages(-1,-1);
    }
    return res;
}

bmpImage * nnImageManager::getImage(int id)
{
    bmpImage *res=nullptr;
    if (allImages.size()>0)
    {
        listImage::const_iterator it = allImages.find(id);
        if (it != allImages.end())
        {
            res = it->second;
        }
    }
    return res;
}

bool nnImageManager::setPath(const XCHAR *_path)
{
    path.clear();
    path=_path;
    return true;
}
