
#ifndef N2FONT_LIST__HEADER
#define N2FONT_LIST__HEADER


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

typedef std::map<std::string, IFontManager *> fontList;

class nnFontList
    : public IFontList
{
    fontList fonts;
public:
    nnFontList();
    ~nnFontList();
    bool readConfiguration(IXmlNode *node, STRING  &src_path);
    bool loadImages(void);
    IFontManager* getManager(const char *name);
    bool add(const char *name, IFontManager*);
    bool remove(const char *name);
    bool getFontNameList(fontNameList & list);
private:
    void clear(void);
    TOSTRING(nnFontList);
};
#endif


