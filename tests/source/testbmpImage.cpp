#include "stdio.h"
#include "CPPtester.h"
#include "images.h"
#ifdef _MSC_VER
#define SRCCOPY             (unsigned long)0x00CC0020 /* dest = source                   */
#define SRCPAINT            (unsigned long)0x00EE0086 /* dest = source OR dest           */
#define SRCAND              (unsigned long)0x008800C6 /* dest = source AND dest          */
#define SRCINVERT           (unsigned long)0x00660046 /* dest = source XOR dest          */
#define SRCERASE            (unsigned long)0x00440328 /* dest = source AND (NOT dest )   */
#define NOTSRCCOPY          (unsigned long)0x00330008 /* dest = (NOT source)             */
extern "C" int __stdcall StretchDIBits( void * hdc,  int xDest,  int yDest,  int DestWidth,  int DestHeight,  int xSrc,  int ySrc,  int SrcWidth,  int SrcHeight,
    const void  * lpBits, LPBITMAPINFO lpbmi,  unsigned int  iUsage,  unsigned long  rop);
extern"C" void *__stdcall  GetDC( void * hWnd);
extern "C" int __stdcall  ReleaseDC(void * hWnd,void *DC);
#endif


///////////////////// HEADER //////////////////////

class test_bmpImage_class
    : public caTester
{
    CA_TEST_SUITE(test_bmpImage_class);
    CA_TEST(test_bmpImage_class::test1, "verifica attach");
    CA_TEST(test_bmpImage_class::test2, "verifica create");
    CA_TEST(test_bmpImage_class::test3, "verifica bitmapfileheader");
    CA_TEST(test_bmpImage_class::test4, "verifica bitmapfileinfo");
    CA_TEST(test_bmpImage_class::test5, "verifica read/write to file");
    CA_TEST(test_bmpImage_class::test6, "verifica copyFromImage");
    CA_TEST(test_bmpImage_class::test7, "verifica isValid");
    CA_TEST(test_bmpImage_class::test8, "verifica clear");
    CA_TEST(test_bmpImage_class::test9, "verifica detach");
    CA_TEST(test_bmpImage_class::test10, "verifica reate attach clear");
    CA_TEST(test_bmpImage_class::test11, "verifica metodo portrait");
    CA_TEST(test_bmpImage_class::test12, "verifica landscape");
    CA_TEST(test_bmpImage_class::test13, "verifica dimensioni");
    CA_TEST_SUITE_END();
    void draw(bmpImage*  bmp);
    void setUp(void);
    void tearDown(void);
    void test1(void);
    void test2(void);
    void test3(void);
    void test4(void);
    void test5(void);
    void test6(void);
    void test7(void);
    void test8(void);
    void test9(void);
    void test10(void);
    void test11(void);
    void test12(void);
    void test13(void);
};
///////////////////////////////////////////////////

REGISTER_CLASS(test_bmpImage_class);


void test_bmpImage_class::draw(bmpImage * bmp)
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

void test_bmpImage_class::setUp(void)
{
#ifdef _SAFE_MEMORY_LEAK
    START_RECORD_MEM(stderr);
#endif
}

void test_bmpImage_class::tearDown(void)
{
#ifdef _SAFE_MEMORY_LEAK
    int res = GET_LEAK_NUM();
    END_RECORD_MEM();
    CA_ASSERT_M("Warning memory leaks !", res == 0);
#endif
}

void test_bmpImage_class::test1(void)
{
    _START();
    _INFO("verifica interrna alla classe: metodo attach");
    _AUTHOR("Coppi Angelo bmpImage library ");
    _STOP();
    bmpImage s;
    CA_ASSERT_M("Pointer to bitmapfileheader test is  null", (nullptr == (LPBITMAPFILEHEADER)s));
    LPBITMAPFILEHEADER p;
    p= (LPBITMAPFILEHEADER) new unsigned char[sizeof(BITMAPFILEHEADER)];
    CA_ASSERT_M("Test Attach method", s.attach(p) == true);
    CA_ASSERT_M("Pointer to bitmapfileheader is valid", (nullptr != (LPBITMAPFILEHEADER)s));

}





void test_bmpImage_class::test2(void)
{
    _START();
    _INFO("verifica interna alla classe: metodo create");
    _AUTHOR("Coppi Angelo bmpImage library ");
    _STOP();
    bmpImage s;
    CA_ASSERT(s.create(100, 100,  255) == true);
    CA_ASSERT((LPBITMAPFILEHEADER)s != nullptr);
}

void test_bmpImage_class::test3(void)
{
    _START();
    _INFO("verifica interrna alla classe : verifica struttura bitmapfileheader");
    _AUTHOR("Coppi Angelo bmpImage library ");
    _STOP();
    bmpImage s;
    s.create(100, 100, 255);
    LPBITMAPFILEHEADER v = (LPBITMAPFILEHEADER)s;
    CA_ASSERT(v->bfType == 0x4d42);
    CA_ASSERT(v->bfOffBits == ((int)(sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER))));
    CA_ASSERT(v->bfSize == ((int)(sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + (100 * 100 * 3))));
}

void test_bmpImage_class::test4(void)
{
    _START();
    _INFO("verifica interrna alla classe : struttura bitmapfileinfo");
    _AUTHOR("Coppi Angelo bmpImage library ");
    _STOP();
    bmpImage s;
    s.create(100, 100,  255);
    LPBITMAPINFOHEADER v = s.getInfoHeader();
    CA_ASSERT(v != NULL);
    CA_ASSERT(v->biBitCount == 24);
    CA_ASSERT(v->biHeight == 100);
    CA_ASSERT(v->biWidth == 100);
    draw(&s);
}

void test_bmpImage_class::test5(void)
{
    _START();
    _INFO("verifica interrna alla classe : metodi copytofile and copyfromfile");
    _AUTHOR("Coppi Angelo bmpImage library ");
    _STOP();
    remove("./whiteImage.bmp");
    bmpImage s;
    s.create(100, 100,  255);
    draw(&s);
    bool res = s.copyToFile(L"./whiteImage.bmp");
    CA_ASSERT(res==true);
    bmpImage t;
    t.copyFromFile(L"./whiteImage.bmp");
    int res1 = memcmp((LPBITMAPFILEHEADER)s, (LPBITMAPFILEHEADER)t, ((LPBITMAPFILEHEADER)(s))->bfSize);
    CA_ASSERT(res1==0);
    draw(&t);
    remove("./whiteImage.bmp");
}

void test_bmpImage_class::test6(void)
{
    _START();
    _INFO("verifica interrna alla classe: metodo copyfrom bmpImage");
    _AUTHOR("Coppi Angelo bmpImage library ");
    _STOP();
    bmpImage s;
    s.create(100, 100,  255);
    bmpImage v;
    v.copyFrombmpImage(s);
    int res = memcmp((LPBITMAPFILEHEADER)s, (LPBITMAPFILEHEADER)v, ((LPBITMAPFILEHEADER)(s))->bfSize);
    CA_ASSERT(res== 0);
    draw(&v);

}

void test_bmpImage_class::test7(void)
{
    _START();
    _INFO("verifica interrna alla classe : metodo isValid()");
    _AUTHOR("Coppi Angelo bmpImage library ");
    _STOP();
    bmpImage s;
    s.create(100, 100, 255);
    CA_ASSERT(s.isValid()==true);
}

void test_bmpImage_class::test8(void)
{
    _START();
    _INFO("verifica interrna alla classe: metodo clear");
    _AUTHOR("Coppi Angelo bmpImage library ");
    _STOP();
    bmpImage s;
    s.create(100, 100,  255);
    CA_ASSERT(s.clear()==true);
    CA_ASSERT((LPBITMAPFILEHEADER)(s)==nullptr);
}

void test_bmpImage_class::test9(void)
{
    _START();
    _INFO("verifica interrna alla classe:metodo detach");
    _AUTHOR("Coppi Angelo bmpImage library ");
    _STOP();
    bmpImage s;
    s.create(100, 100,  255);
    LPBITMAPFILEHEADER v = (LPBITMAPFILEHEADER)s;
    CA_ASSERT(s.detach()==true);
    CA_ASSERT((LPBITMAPFILEHEADER)(s)==nullptr);
    delete [](v);
}

void test_bmpImage_class::test10(void)
{
    _START();
    _INFO("verifica interrna alla classe: create attach clear");
    _AUTHOR("Coppi Angelo bmpImage library ");
    _STOP();
    bmpImage s;
    s.create(100, 100,  255);
    bmpImage t;
    t.create(100, 100, 255);
    LPBITMAPFILEHEADER v = (LPBITMAPFILEHEADER)s;
    t.clear();
    t.attach(s);
    CA_ASSERT(v == (LPBITMAPFILEHEADER)t);
}

void test_bmpImage_class::test11(void)
{
    _START();
    _INFO("verifica interrna alla classe:metodo portrait");
    _AUTHOR("Coppi Angelo bmpImage library ");
    _STOP();
    bmpImage s;
    s.create(100, 50,  255);
    CA_ASSERT((int)s.getWidth()==100);
    CA_ASSERT((int)s.getHeight()== 50);
    draw(&s);
    s.portrait(false);
    CA_ASSERT((int)s.getWidth()==50);
    CA_ASSERT((int)s.getHeight()==100);
    draw(&s);

}

void test_bmpImage_class::test12(void)
{
    _START();
    _INFO("verifica interrna alla classe:metodo landscape");
    _AUTHOR("Coppi Angelo bmpImage library ");
    _STOP();
    bmpImage s;
    s.create(50, 100,  255);
    CA_ASSERT((int)s.getWidth()==50);
    CA_ASSERT((int)s.getHeight() == 100);
    s.landscape(false);
    CA_ASSERT((int)s.getWidth()== 100);
    CA_ASSERT((int)s.getHeight()== 50);
}

void test_bmpImage_class::test13(void)
{
    _START();
    _INFO("verifica interrna alla classe:dimensioni");
    _AUTHOR("Coppi Angelo bmpImage library ");
    _STOP();
    bmpImage s;
    s.create(50, 100,  255);
    CA_ASSERT(s.check(50, 100)==true);
    CA_ASSERT(s.check(150, 100)==false);
}
