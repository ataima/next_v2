#include "stdio.h"
#include "CPPtester.h"
#include "images.h"
#include "n2miniXml.h"
#include "n2ImageManager.h"

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

///////////////////// HEADER //////////////////////

#ifdef _MSC_VER
#define SRCCOPY             (unsigned long)0x00CC0020 /* dest = source                   */
#define SRCPAINT            (unsigned long)0x00EE0086 /* dest = source OR dest           */
#define SRCAND              (unsigned long)0x008800C6 /* dest = source AND dest          */
#define SRCINVERT           (unsigned long)0x00660046 /* dest = source XOR dest          */
#define SRCERASE            (unsigned long)0x00440328 /* dest = source AND (NOT dest )   */
#define NOTSRCCOPY          (unsigned long)0x00330008 /* dest = (NOT source)             */
extern "C" int __stdcall StretchDIBits(void * hdc, int xDest, int yDest, int DestWidth, int DestHeight, int xSrc, int ySrc, int SrcWidth, int SrcHeight,
    const void  * lpBits, LPBITMAPINFO lpbmi, unsigned int  iUsage, unsigned long  rop);
extern"C" void *__stdcall  GetDC(void * hWnd);
extern "C" int __stdcall  ReleaseDC(void * hWnd, void *DC);
#endif


class test_image_manager
    : public caTester
{
    CA_TEST_SUITE(test_image_manager)
        CA_TEST(test_image_manager::test1, "verifica read configuration");
    CA_TEST(test_image_manager::test2, "verifica loadbitmap");
    CA_TEST_SUITE_END()
        void setUp(void) {}
    void tearDown(void) {}
    void draw(bmpImage * bmp);
    void test1(void);
    void test2(void);
};
///////////////////////////////////////////////////

REGISTER_CLASS(test_image_manager);

void test_image_manager::draw(bmpImage * bmp)
{
#ifdef _MSC_VER
    void* dc = ::GetDC(NULL);
    ::StretchDIBits(dc, 0, 0,
        bmp->getWidth(), bmp->getHeight(),
        0, 0, bmp->getWidth(), bmp->getHeight(),
        bmp->getBits(), bmp->getInfo(), 0, SRCCOPY);
    ReleaseDC(NULL, dc);
#endif
}

void test_image_manager::test1(void)
{
    _START();
    _INFO("Verify of internal class nnImageManager: method  read configuration");
    _AUTHOR("Coppi Angelo n2draw library ");
    _STOP();
    miniXmlNode  node(X("IMAGES"));
    node.add(X("PATH"), X("D:\\CPP_PRJ\\next_v2\\tests\\project\\images"));
    miniXmlNode *wire = node.add(X("WIRE"), X(""));
    miniXmlNode *obj = wire->add(X("OBJ"), X(""));
    obj->add(X("VALUE"), X("wireHorizzontal"));
    obj->add(X("FILE"), X("file1.bmp"));
    obj = wire->add(X("OBJ"), X(""));
    obj->add(X("VALUE"), X("wireVertical"));
    obj->add(X("FILE"), X("file2.bmp"));
    obj = wire->add(X("OBJ"), X(""));
    obj->add(X("VALUE"), X("wireAngleUpRight"));
    obj->add(X("FILE"), X("file3.bmp"));
    miniXmlNode *contact = node.add(X("CONTACT"), X(""));
    obj = contact->add(X("OBJ"), X(""));
    obj->add(X("VALUE"), 4);
    obj->add(X("FILE"), X("file4.bmp"));
    obj = contact->add(X("OBJ"), X(""));
    obj->add(X("VALUE"), 5);
    obj->add(X("FILE"), X("file5.bmp"));
    obj = contact->add(X("OBJ"), X(""));
    obj->add(X("VALUE"), 6);
    obj->add(X("FILE"), X("file6.bmp"));
    miniXmlNode *coil = node.add(X("COIL"), X(""));
    obj = coil->add(X("OBJ"), X(""));
    obj->add(X("VALUE"), 7);
    obj->add(X("FILE"), X("file7.bmp"));
    obj = coil->add(X("OBJ"), X(""));
    obj->add(X("VALUE"), 8);
    obj->add(X("FILE"), X("file8.bmp"));
    obj = coil->add(X("OBJ"), X(""));
    obj->add(X("VALUE"), 9);
    obj->add(X("FILE"), X("file9.bmp"));
    nnImageManager im;
    bool res = im.readConfiguration(&node);
    CA_ASSERT(res == true);
    const objImageList *objs = im.getAvailObj();
    CA_ASSERT(objs != nullptr);
    objImageList::const_iterator it = objs->begin();
    CA_ASSERT(it->first == 1);
    CA_ASSERT(it->second == X("file1.bmp"));
    it++;
    CA_ASSERT(it->first == 2);
    CA_ASSERT(it->second == X("file2.bmp"));
    it++;
    CA_ASSERT(it->first == 3);
    CA_ASSERT(it->second == X("file3.bmp"));
    it++;
    CA_ASSERT(it->first == 4);
    CA_ASSERT(it->second == X("file4.bmp"));
    it++;
    CA_ASSERT(it->first == 5);
    CA_ASSERT(it->second == X("file5.bmp"));
    it++;
    CA_ASSERT(it->first == 6);
    CA_ASSERT(it->second == X("file6.bmp"));
    it++;
    CA_ASSERT(it->first == 7);
    CA_ASSERT(it->second == X("file7.bmp"));
    it++;
    CA_ASSERT(it->first == 8);
    CA_ASSERT(it->second == X("file8.bmp"));
    it++;
    std::wstring path = im.getDefaulPath();
    CA_ASSERT(path == X("D:\\CPP_PRJ\\next_v2\\tests\\project\\images"));
}


void test_image_manager::test2(void)
{
    _START();
    _INFO("Verify of internal class nnImageManager: method  read configuration and load images");
    _AUTHOR("Coppi Angelo n2draw library ");
    _STOP();
    miniXmlNode  node;
    bool res = miniXmlNode::load(X("..\\..\\images\\conf.xml"),&node);
    CA_ASSERT(res == true);
    nnImageManager im;
    res = im.readConfiguration(&node);
    CA_ASSERT(res == true);
    res = im.loadImages(40, 48);
    CA_ASSERT(res == true);
    const listImage *images = im.getImageList();
    if (images != nullptr)
    {
        listImage::const_iterator it = images->begin();
        listImage::const_iterator _end = images->end();
        while (it != _end)
        {
            draw((it->second));
            it++;
        }
    }
}