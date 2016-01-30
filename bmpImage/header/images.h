#ifndef IMAGE_STUB_HEADER
#define IMAGE_STUB_HEADER

#ifndef _WINDOWS_


#ifdef _MSC_VER
#include <pshpack2.h>
#endif


#ifndef tagBITMAPFILEHEADER
typedef struct tagBITMAPFILEHEADER {
    short			bfType;
    unsigned long   bfSize;
    short			bfReserved1;
    short			bfReserved2;
    unsigned long   bfOffBits;
} BITMAPFILEHEADER;



#ifdef _MSC_VER
#include <poppack.h>
#endif

typedef BITMAPFILEHEADER *LPBITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER {
    unsigned long      biSize;
    long			   biWidth;
    long			   biHeight;
    short			   biPlanes;
    short			   biBitCount;
    unsigned long      biCompression;
    unsigned long      biSizeImage;
    long               biXPelsPerMeter;
    long               biYPelsPerMeter;
    unsigned long      biClrUsed;
    unsigned long      biClrImportant;
} BITMAPINFOHEADER;

typedef  BITMAPINFOHEADER *LPBITMAPINFOHEADER;


typedef struct tagRGBQUAD {
    unsigned char    rgbBlue;
    unsigned char    rgbGreen;
    unsigned char    rgbRed;
    unsigned char    rgbReserved;
} RGBQUAD;

typedef RGBQUAD * LPRGBQUAD;

typedef struct tagBITMAPINFO {
    BITMAPINFOHEADER    bmiHeader;
    RGBQUAD             bmiColors[1];
} BITMAPINFO;

typedef BITMAPINFO *LPBITMAPINFO;


#endif
#endif


#ifdef _UNICODE
#define  XCHAR char16_t
#else
#define  XCHAR char
#endif


class  bmpImage
{
   static const size_t bitPerPlane = 24;
public:
    bmpImage();
    bmpImage(bmpImage & b);
    ~bmpImage();
    unsigned long getTotalSize();
    bool copyFromFile(const XCHAR *name);
    bool copyFrombmpImage(bmpImage & b);
    bool copyToFile(const XCHAR *name);
    bool create(unsigned int width, unsigned int height,unsigned int deep,  unsigned char  color);
    bool isValid(void);
    bool clear(void);
    bool detach(void);
    bool attach(bmpImage &);
    bool attach(LPBITMAPFILEHEADER p);
    bool portrait(bool mirror = true);
    bool landscape(bool mirror = true);
    bool check(unsigned int Width, unsigned int Height);
    LPBITMAPINFOHEADER getInfoHeader(void) const;
    LPBITMAPINFO getInfo() const;
    LPRGBQUAD  getPalette(void);
    void setHorizontalResolution(double value);
    void setVerticalResolution(double value);
    unsigned int getBitsPerPixel(void) const;
    unsigned int getColorsUsed(void) const;
    unsigned int getWidth() const;
    unsigned int getHeight() const;
    unsigned int getLine(void) const;
    unsigned int getPitch(void) const;
    unsigned char * getScanLine(unsigned scanline);
    unsigned char * getBits();
    bool invert(void);
    unsigned int  getColorType(void);
    bool convertTo24Bits(void);
    bool rotate(double angle);
    bool flipHorizontal(void);
    bool flipVertical(void);
    bool pasteSubImage(LPBITMAPFILEHEADER src, int left, int top);
    inline operator LPBITMAPFILEHEADER() { return m_hBitmap; }
    void operator =(bmpImage & b);
    bool swaptoBGR(void);
    bool swaptoRBG(void);
    bool swaptoGRB(void);
    bool translateColor(unsigned char oriRed, unsigned char oriGreen,
        unsigned char oriBlue, unsigned char newRed,
        unsigned char newGreen, unsigned char newBlue);
    bool copyBits(bmpImage & dst, size_t left,size_t top,size_t right,size_t bottom);
    bool drawSprite( bmpImage & sprite, int left, int top);
    bool drawMaskSprite( bmpImage & sprite, int left, int top,
                unsigned char Rmask, unsigned char Gmask, unsigned char Bmask);
    bool setPixel(unsigned int _x,unsigned int _y,unsigned char red,unsigned char green,unsigned char blue);
    bool getPixel(unsigned int _x,unsigned int _y,unsigned char  & red,unsigned char & green,unsigned char & blue);
    bool line( int x1,  int y1,  int x2,  int y2, unsigned char red, unsigned char green, unsigned char blue, unsigned int mask);
    bool frameRect( int x1,  int y1,  int x2,  int y2, unsigned char red, unsigned char green, unsigned char blue,unsigned int mask);


protected:
    bool replace(LPBITMAPFILEHEADER new_dib);
    static size_t getInternalImageSize(unsigned int width, unsigned int height,unsigned int deep);
    static LPBITMAPFILEHEADER  allocateBitmap(unsigned int width, unsigned int height,unsigned int deep,unsigned char color);
    static void freeBitmap(LPBITMAPFILEHEADER bI);
    static LPBITMAPINFOHEADER  getInfoHeader(LPBITMAPFILEHEADER bI);
    static LPBITMAPINFO getInfo(LPBITMAPFILEHEADER bI);
    static LPRGBQUAD  getPalette(LPBITMAPFILEHEADER bI);
    static unsigned int getBitsPerPixel(LPBITMAPFILEHEADER bI);
    static unsigned char * getScanLine(LPBITMAPFILEHEADER bI, unsigned scanline);
    static unsigned char * getBits(LPBITMAPFILEHEADER bI);
    static unsigned int getHeight(LPBITMAPFILEHEADER bI);
    static unsigned int getWidth(LPBITMAPFILEHEADER bI);
    static unsigned int getPitch(LPBITMAPFILEHEADER bI);
    static unsigned int getLine(LPBITMAPFILEHEADER bI);
    static LPBITMAPFILEHEADER rotateInt(LPBITMAPFILEHEADER bI, double angle);
    static unsigned int getColorType(LPBITMAPFILEHEADER bI);
    static unsigned int getColorsUsed(LPBITMAPFILEHEADER bI);
    static LPBITMAPFILEHEADER RotateAny(LPBITMAPFILEHEADER src, double dAngle);
    static LPBITMAPFILEHEADER Rotate90(LPBITMAPFILEHEADER src);
    static LPBITMAPFILEHEADER Rotate180(LPBITMAPFILEHEADER src);
    static LPBITMAPFILEHEADER Rotate270(LPBITMAPFILEHEADER src);
    static LPBITMAPFILEHEADER cloneImage(LPBITMAPFILEHEADER pI);
    static bool pasteSubImage(LPBITMAPFILEHEADER dst, LPBITMAPFILEHEADER src, int left, int top);
    static bool copyBits(LPBITMAPFILEHEADER dst, LPBITMAPFILEHEADER src,size_t left, size_t top, size_t width, size_t height);
    static bool swapto(LPBITMAPFILEHEADER pI, int iSorg, int iDest);
    static bool drawSprite(LPBITMAPFILEHEADER dst,  LPBITMAPFILEHEADER sprite, size_t left, size_t top);
    static bool drawMaskSprite(LPBITMAPFILEHEADER dst,  LPBITMAPFILEHEADER sprite, size_t left, size_t top,
                      unsigned char Rmask, unsigned char Gmask, unsigned char Bmask);
    //
    static bool setPixel(LPBITMAPFILEHEADER dest,unsigned int x,unsigned int y,unsigned char red,unsigned char green,unsigned char blue);
    static bool getPixel(LPBITMAPFILEHEADER dest, unsigned int x, unsigned int y, unsigned char  & red, unsigned char & green, unsigned char & blue);
    static bool line(LPBITMAPFILEHEADER dest,  int x1,  int y1,  int x2,  int y2,
                        unsigned char red, unsigned char green, unsigned char blue, unsigned int maskDot);
    static bool translateColor(LPBITMAPFILEHEADER dest,unsigned char oriRed, unsigned char oriGreen,
        unsigned char oriBlue, unsigned char newRed,
        unsigned char newGreen, unsigned char  newBlue);

protected:
    BITMAPFILEHEADER * m_hBitmap;
};



#include <map>



class  listImage
    :public std::map<int ,bmpImage * >
{
public:
    listImage();
    ~listImage();
    bool Add(int index, bmpImage & b );
};


class bmpSprite
    : public bmpImage
{
public:
    bmpSprite() :bmpImage(){};
    bool fromImages(bmpImage & obj, bmpImage & mask);
protected:
    bool toSprite(LPBITMAPFILEHEADER dest, LPBITMAPFILEHEADER image, LPBITMAPFILEHEADER mask);
};

#endif
