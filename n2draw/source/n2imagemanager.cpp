#include "images.h"
#include "n2draw.h"
#include "n2ImageManager.h"

nnImageManager::nnImageManager()
{
    availObj.clear();
    allImages.clear();
    path.clear();
}

nnImageManager::~nnImageManager()
{
    availObj.clear();
    allImages.clear();
    path.clear();
}
#if (_WIN32 || _WIN64)
#define wgetcwd  _wgetcwd
#endif

bool nnImageManager::readConfiguration(miniXmlNode * node)
{
    bool res = false;
    long offset;
    std::wstring filename;
    miniXmlNode *conf = node->find(X("IMAGES"));

    if (conf)
    {
        miniXmlNode *xpath = conf->find(X("PATH"));
        if (xpath)
        {
            XCHAR buff[_MAX_PATH];
            const XCHAR *v = xpath->getValue();
            if(wgetcwd(buff,_MAX_PATH))
            {
             std::wstringstream ss;
             ss<<buff<<"/"<<v;
             path=ss.str().c_str();
            }
            else
            {
                path=v;
            }
        }
        miniXmlNode *wire = conf->find(X("WIRE"));
        if (wire)
        {
            miniXmlNode *t = wire->find(X("OBJ"));
            if (t)
            {
                do {
                    filename.clear();
                    offset = -1;
                    miniXmlNode *e = t->find(X("VALUE"));
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
        miniXmlNode *contact = conf->find(X("CONTACT"));
        if (contact)
        {
            miniXmlNode *t = contact->find(X("OBJ"));
            if (t)
            {
                do {
                    filename.clear();
                    offset = -1;
                    miniXmlNode *e = t->find(X("VALUE"));
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
        miniXmlNode *coil = conf->find(X("COIL"));
        if (coil)
        {
            miniXmlNode *t = coil->find(X("OBJ"));
            if (t)
            {
                do {
                    filename.clear();
                    offset = -1;
                    miniXmlNode *e = t->find(X("VALUE"));
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
        std::wstring filenameabs;
        objImageList::iterator it = availObj.begin();
        objImageList::iterator _end = availObj.end();
        while (it != _end)
        {
            bmpImage image;
            filenameabs = path;
            filenameabs += L"\\";
            filenameabs += it->second;
            if (image.copyFromFile(filenameabs.c_str()))
            {
                if (image.getWidth() != w || image.getHeight() != h)
                {
                    imagesConfigurationBadSizeException *pe=new imagesConfigurationBadSizeException(image.getWidth(),image.getHeight());
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


