#include "stdio.h"
#include "CPPtester.h"
#include "images.h"
#include "n2draw.h"
#include "n2drawmanager.h"
#include "n2miniXml.h"
#include "n2imagemanager.h"
#include "n2viewglue.h"
#include "n2appmanager.h"
#include "n2connection.h"
#ifdef _MSC_VER
#include <direct.h>
#endif


#ifndef _MSC_VER
#define MKDIR(a,b) mkdir(a,b)
#include <sys/stat.h>
#else
#define MKDIR(a,b) mkdir(a)
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


class test_app_manager
    : public caTester
{
    CA_TEST_SUITE(test_app_manager)
        CA_TEST(test_app_manager::test1, "verifica loadbitmap");
       CA_TEST(test_app_manager::test2, "verifica scroll");
    CA_TEST_SUITE_END()
        void setUp(void) {}
    void tearDown(void) {}
    void draw(bmpImage * bmp);
    void test1(void);
    void test2(void);
};
///////////////////////////////////////////////////

REGISTER_CLASS(test_app_manager);

void test_app_manager::draw(bmpImage * bmp)
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




void test_app_manager::test1(void)
{
    _START();
    _INFO("Verify of internal class nnAppManager:load and display a draw");
    _AUTHOR("Coppi Angelo n2draw library ");
    _STOP();
    nnAppManager app;
#ifdef _UNICODE
    STRING name(X("conf_utf16.xml"));
#else
    STRING name(X("conf_ut8.xml"));
#endif
    childApps *childs = app.createObjects(name);
    CA_ASSERT(childs != nullptr);
    bool res;
    nnPoint p=childs->view->getCoordPhy(1,1);
    CA_ASSERT(p.x != 0 );
    CA_ASSERT(p.y != 0);
    res=childs->imageManager->loadImages(p.x, p.y);
    CA_ASSERT(res == true);
    nnContactNO *v = new nnContactNO();
    nnObjManager *mn = dynamic_cast<nnObjManager *>(childs->object_manager);
    res = mn->addContact(10, 0, v);
    CA_ASSERT(res == true);
    CA_ASSERT((int)mn->size() == (int)1);
    nnGenericCoil *u = new nnGenericCoil();
    res = mn->addCoil(10, u);
    CA_ASSERT(res == true);
    CA_ASSERT((int)mn->size() == (int)10);
    nnContactNC *v1 = new nnContactNC();
    res = mn->addContact(12, 0, v1);
    CA_ASSERT(res == true);
    nnPoint p1(12, 0);
    nnPoint p2(10, 0);
    nnConnection::connectComponent(childs->object_manager, p1, p2);
    res = childs->view->updateDraw();
    CA_ASSERT(res == true);
    bmpImage &bdraw = childs->view->getDraw();
    draw(&bdraw);
    MKDIR(".\\bmp",S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    bdraw.copyToFile(X(".\\bmp\\test1_app.bmp"));
}




void test_app_manager::test2(void)
{
    _START();
    _INFO("Verify of internal class nnAppManager:load and display a draw, and scroll it");
    _AUTHOR("Coppi Angelo n2draw library ");
    _STOP();
    nnAppManager app;
#ifdef _UNICODE
    STRING name(X("conf_utf16.xml"));
#else
    STRING name(X("conf_ut8.xml"));
#endif
    childApps *childs = app.createObjects(name);
    CA_ASSERT(childs != nullptr);
    bool res;
    nnPoint p = childs->view->getCoordPhy(1, 1);
    CA_ASSERT(p.x != 0);
    CA_ASSERT(p.y != 0);
    res = childs->imageManager->loadImages(p.x, p.y);
    CA_ASSERT(res == true);
    nnContactNO *v = new nnContactNO();
    nnObjManager *mn = dynamic_cast<nnObjManager *>(childs->object_manager);
    res = mn->addContact(10, 0, v);
    CA_ASSERT(res == true);
    CA_ASSERT((int)mn->size() == (int)1);
    nnGenericCoil *u = new nnGenericCoil();
    res = mn->addCoil(10, u);
    CA_ASSERT(res == true);
    CA_ASSERT((int)mn->size() == (int)10);
    nnContactNC *v1 = new nnContactNC();
    res = mn->addContact(12, 0, v1);
    CA_ASSERT(res == true);
    nnPoint p1(12, 0);
    nnPoint p2(10, 0);
    nnConnection::connectComponent(childs->object_manager, p2, p1);
    res = childs->view->updateDraw();
    CA_ASSERT(res == true);
    bmpImage &bdraw = childs->view->getDraw();
    draw(&bdraw);
    childs->view->handlerScrollHorz(2);
    res = childs->view->updateDraw();
    //CA_ASSERT(res == true);
    bdraw = childs->view->getDraw();
    draw(&bdraw);
    childs->view->handlerScrollVert(2);
    res = childs->view->updateDraw();
    CA_ASSERT(res == true);
    bdraw = childs->view->getDraw();
    draw(&bdraw);
}
