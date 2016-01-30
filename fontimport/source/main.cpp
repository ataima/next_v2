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

#include <stdio.h>
#include <string>
#include <sstream>
#include "images.h"
#include <direct.h>
typedef  void *HFONT;
typedef  void *HDC;
typedef  void *HGDIOBJ;
typedef  void *HBRUSH;
typedef  void *HANDLE;
typedef  unsigned long DWORD;
typedef  void *HBITMAP;
typedef struct tagRECT
{
    long left;
    long top;
    long right;
    long bottom;
} 	RECT;


typedef struct tagBITMAP
{
    long        bmType;
    long        bmWidth;
    long        bmHeight;
    long        bmWidthBytes;
    short       bmPlanes;
    short       bmBitsPixel;
    void *      bmBits;
} BITMAP;

extern "C"   HFONT __stdcall CreateFontA(
     int     nHeight,
     int     nWidth,
     int     nEscapement,
     int     nOrientation,
     int     fnWeight,
     DWORD   fdwItalic,
     DWORD   fdwUnderline,
     DWORD   fdwStrikeOut,
     DWORD   fdwCharSet,
     DWORD   fdwOutputPrecision,
     DWORD   fdwClipPrecision,
     DWORD   fdwQuality,
     DWORD   fdwPitchAndFamily,
     const char * lpszFace
    );

extern"C" void *__stdcall GetDC(void * hWnd);
extern "C" int __stdcall  ReleaseDC(void * hWnd, void *DC);
extern "C" HGDIOBJ __stdcall SelectObject( HDC hdc,  HGDIOBJ h);
extern "C" int  __stdcall Rectangle( HDC hdc, int left, int top, int right,  int bottom);
extern "C" DWORD __stdcall  SetTextColor(HDC hdc, DWORD color);
extern "C" int  __stdcall TextOutA(HDC hdc, int x, int y, const char * lpString, int c);
extern "C" int __stdcall DeleteObject(HGDIOBJ ho);
extern "C" int __stdcall SetBkMode(HDC hdc, int mode);
extern "C" HDC __stdcall CreateCompatibleDC(HDC hdc);
extern "C" HBITMAP __stdcall CreateCompatibleBitmap(HDC hdc, int cx, int cy);
extern "C" int __stdcall GetObjectA( HANDLE h,  int c, char * pv);
extern "C" int __stdcall GetDIBits( HDC hdc,  HBITMAP hbm,  
    unsigned int start, unsigned int cLines,
     void * lpvBits,  LPBITMAPINFO lpbmi,  unsigned int  usage);  
extern "C" int __stdcall DeleteDC(HDC hdc);
extern "C" int __stdcall StretchDIBits(void * hdc, int xDest, int yDest, int DestWidth, int DestHeight, int xSrc, int ySrc, int SrcWidth, int SrcHeight,
    const void  * lpBits, LPBITMAPINFO lpbmi, unsigned int  iUsage, unsigned long  rop);


#define SRCCOPY             (unsigned long)0x00CC0020

bool drawImage(HDC dc, bmpImage & image, int x, int y)
{
    bool res = false;
    if (dc!=0)
    {
        LPBITMAPINFO info = image.getInfo();
        void *bits = image.getBits();
        unsigned int size_x = image.getWidth();
        unsigned int size_y = image.getHeight();
        res = (0 != ::StretchDIBits(dc, x, y,
            size_x, size_y,
            0, 0, size_x, size_y,
            bits, info, 0, SRCCOPY));

    }
    return res;
}



bool copyFromBitmap(HDC dc,bmpImage &image,HBITMAP m_tmpBitmap)
{
    bool res = false;
    if (m_tmpBitmap!=0)
    {
        int Success;
        BITMAP bm;
        // Get informations about the bitmap
        if (GetObjectA(m_tmpBitmap, sizeof(BITMAP), (char *)&bm))
        {            
            // Create the image
            image.create( (unsigned int)bm.bmWidth, 
                (unsigned int )bm.bmHeight,bm.bmBitsPixel,0);
                // The GetDIBits function clears the biClrUsed and biClrImportant BITMAPINFO members (dont't know why) 
                // So we save these infos below. This is needed for palettized images only. 
                int nColors = image.getColorsUsed();
                // Copy the pixels
                Success = GetDIBits(dc,								// handle to DC
                    m_tmpBitmap,						// handle to bitmap
                    0,							// first scan line to set
                    image.getHeight(),	// number of scan lines to copy
                    image.getBits(),	// array for bitmap bits
                    image.getInfo(),	// bitmap data buffer
                    0				    // RGB 
                    );
                LPBITMAPINFOHEADER m_Infobm = image.getInfoHeader();
                if (m_Infobm)
                {
                    m_Infobm->biClrUsed = nColors;
                    m_Infobm->biClrImportant = nColors;
                }            
            res = (Success != 0);
        }
    }
    return res;
}



bool printText(bmpImage & image, HDC dc, HFONT font, 
    DWORD color, DWORD x, DWORD y, const char *text)
{
    bool res = false;
    if (image.isValid() && dc!=NULL && text != NULL && strlen(text)>0)
    { 
            HDC memDC = CreateCompatibleDC(dc);
            if (memDC != 0)
            {
                HBITMAP bmDefault = CreateCompatibleBitmap(dc, image.getWidth(), image.getHeight());
                if (bmDefault != 0)
                {
                    SelectObject(memDC, bmDefault);    
                    drawImage(memDC, image, 0,0);
                    int oldMOde = SetBkMode(memDC, 1);
                    HFONT oldFont = (HFONT)SelectObject(memDC, font);
                    SetTextColor(memDC, color);
                    ::TextOutA(memDC, x, y, text, strlen(text));
                    ::SelectObject(memDC, oldFont);
                    ::SetBkMode(memDC, oldMOde);
                    ::SelectObject(dc, bmDefault);
                    
                    if (copyFromBitmap(memDC,image,bmDefault))
                    {
                    res = true;                    
#if TEST
                    {
                        drawImage(dc, image, 10,10);
                    }
#endif
                    }
                    DeleteObject(bmDefault);
                }
                DeleteDC(memDC);
            }
    }
    return res;
}




int  main(int argc, char *argv[])
{
#if TEST1
    bmpImage t0,t1, t2;
    t0.copyFromFile("test.bmp");
    LPBITMAPINFO i0=t0.getInfo();
    t1.create(10, 14, 24, 255);
    LPBITMAPINFO i1 = t1.getInfo();
    HDC dc = ::GetDC(nullptr);
    drawImage(dc, t0, 0, 0);
    drawImage(dc, t1, 20, 0);
    t1.copyToFile("t1.bmp");
    drawImage(dc, t1, 40, 0);
    t2.copyFromFile("t1.bmp");
    LPBITMAPINFO i2 = t2.getInfo();
    drawImage(dc, t2,60, 0);
    ::ReleaseDC(nullptr, dc);
    return 1;
#endif
    if (argc < 2)
        return -1;
    else
    {
        int height=12, width=8;
        std::string fontName = argv[1];
        if (argc > 2)
        {
            height = ::atoi(argv[2]);
            if (height == 0)
                height = 12;
        }
        if (argc > 3)
        {
            width = ::atoi(argv[3]);
            if (width == 0)
                width = 8;
        }
        if (!fontName.empty())
        {
            HFONT curFont = CreateFontA(-height, width, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, fontName.c_str());
            HDC dc = ::GetDC(nullptr);
            if (dc && curFont)
            {
                std::stringstream path;
                path << fontName << "_" << width << "_" << height;
                mkdir(path.str().c_str());
                path<< ".xml";
                FILE *file = fopen(path.str().c_str(), "wb");
                if (file)
                    {
                        fwrite("<ITEM_xx>\n", 10,1, file);
                        fwrite("\t<NAME>", 7, 1, file);
                        path.str("");
                        path << fontName << "_" << width << "_" << height;
                        fwrite(path.str().c_str(),path.str().size(), 1, file);
                        fwrite("</NAME>\n", 8, 1, file);
                    }
                int i;
                for (i = 0; i < 255; i++)
                {
                    if (isprint(i))
                    {
                        char str[2];
                        str[0] = (char)i;
                        str[1] = 0;
                        bmpImage image;
                        image.create(10, 14,32, 0);                      
                        if (printText(image, dc, curFont, 0x00ffffff, 0, 0, str))
                        {
                            std::stringstream ss;
                            ss <<path.str().c_str()<<"\\"<< i << "_" << width << "_" <<height<<".bmp";
                            LPBITMAPINFO info = image.getInfo();
#if TEST
                            drawImage(dc, image, 50,0);
#endif
                            image.copyToFile(ss.str().c_str());
                            if (file)
                            {
                                fwrite("\t<SYM>\n", 7, 1, file);
                                fwrite("\t\t<VALUE>", 9, 1, file);
                                char t = (char)i;
                                fwrite(&t, 1, 1, file);
                                fwrite("</VALUE>\n", 9, 1, file);
                                fwrite("\t\t<FILE>", 8, 1, file);
                                ss.str("");
                                ss << i << "_" << width << "_" << height << ".bmp";
                                fwrite(ss.str().c_str(),ss.str().size(),  1, file);
                                fwrite("</FILE>\n", 8, 1, file);
                                fwrite("\t</SYM>\n", 8, 1, file);
                            }
#if TEST
                            bmpImage verify;
                            verify.copyFromFile(ss.str().c_str());
                            drawImage(dc, verify, 200,0);
                            verify.clear();
#endif
                        }
                        image.clear();
                    }
                }
                ::DeleteObject((HGDIOBJ)curFont);
                ::ReleaseDC(nullptr, dc);
                if (file)
                {
                    fwrite("</ITEM_xx>\n", 11, 1,file);
                    fflush(file);
                    fclose(file);
                }
            }
        }
    }
}