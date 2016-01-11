#include <math.h>
#include <time.h>
#ifndef _MSC_VER
#include <string.h>
#endif
#include "images.h"
#include <codecvt>
#include <locale>

#ifndef _MSC_VER
#define FOPEN fopen
#define _fileno fileno
#define _stat stat
#define _fstat fstat
#include <sys/stat.h>
#else
#define FOPEN fopen
#endif


#ifndef min
#define min(a,b) a>b?b:a
#endif

#ifndef __func__
#define __func__  __FUNCTION__
#endif



//CUT CODE FROM FreeImageLib


#define RBLOCK		128

#define TRACE

typedef enum tag_free_color_type {
    FIC_MINISWHITE = 0,		//! min value is white
    FIC_MINISBLACK = 1,		//! min value is black
    FIC_RGB = 2,		//! RGB color model
    FIC_PALETTE = 3,		//! color map indexed
    FIC_RGBALPHA = 4,		//! RGB color model with alpha channel
    FIC_CMYK = 5		//! CMYK color model
} FREE_IMAGE_COLOR_TYPE;

/// Max function
template <class T> T tMAX(const T &a, const T &b) {
    return (a > b) ? a : b;
}

/// Min function
template <class T> T tMIN(const T &a, const T &b) {
    return (a < b) ? a : b;
}

#define ID_RGBA_ALPHA			3
#define ID_RGBA_RED				2
#define ID_RGBA_GREEN			1
#define ID_RGBA_BLUE			0

#define LUMA_REC709(r, g, b)	(0.2126F * r + 0.7152F * g + 0.0722F * b)

#define GREY(r, g, b) (unsigned char )(LUMA_REC709(r, g, b) + 0.5F)

#define CREATE_GREYSCALE_PALETTE_REVERSE(palette, entries) \
    for (unsigned i = 0, v = 0x00FFFFFF; i < entries; i++, v -= (0x00FFFFFF / (entries - 1))) { \
        ((unsigned *)palette)[i] = v; \
                                        }

static inline  unsigned char
HINIBBLE(unsigned char byte) {
    return byte & 0xF0;
}

static inline  unsigned char
LOWNIBBLE(unsigned char byte) {
    return byte & 0x0F;
}

static inline  unsigned char *
CalculateScanLine(unsigned char *bits, unsigned pitch, int scanline) {
    return bits ? (bits + ((size_t)pitch * scanline)) : nullptr;
}



static inline  unsigned
CalculateLine(unsigned width, unsigned bitdepth) {
    return (unsigned)(((unsigned long long)width * bitdepth + 7) / 8);
}

static inline  unsigned
CalculatePitch(unsigned line) {
    return line + 3 & ~3;
}


static inline  void
convertLine1To24(unsigned char  *target, unsigned char  *source, int width_in_pixels, RGBQUAD *palette) {
    for (int cols = 0; cols < width_in_pixels; cols++) {
        unsigned char  index = (source[cols >> 3] & (0x80 >> (cols & 0x07))) != 0 ? 1 : 0;

        target[ID_RGBA_BLUE] = palette[index].rgbBlue;
        target[ID_RGBA_GREEN] = palette[index].rgbGreen;
        target[ID_RGBA_RED] = palette[index].rgbRed;

        target += 3;
    }
}

static inline   void
convertLine4To24(unsigned char  *target, unsigned char  *source, int width_in_pixels, RGBQUAD *palette) {
    bool low_nibble = false;
    int x = 0;

    for (int cols = 0; cols < width_in_pixels; ++cols) {
        if (low_nibble) {
            target[ID_RGBA_BLUE] = palette[LOWNIBBLE(source[x])].rgbBlue;
            target[ID_RGBA_GREEN] = palette[LOWNIBBLE(source[x])].rgbGreen;
            target[ID_RGBA_RED] = palette[LOWNIBBLE(source[x])].rgbRed;

            x++;
        }
        else {
            target[ID_RGBA_BLUE] = palette[HINIBBLE(source[x]) >> 4].rgbBlue;
            target[ID_RGBA_GREEN] = palette[HINIBBLE(source[x]) >> 4].rgbGreen;
            target[ID_RGBA_RED] = palette[HINIBBLE(source[x]) >> 4].rgbRed;
        }

        low_nibble = !low_nibble;

        target += 3;
    }
}

static inline   void
convertLine8To24(unsigned char  *target, unsigned char  *source, int width_in_pixels, RGBQUAD *palette) {
    for (int cols = 0; cols < width_in_pixels; cols++) {
        target[ID_RGBA_BLUE] = palette[source[cols]].rgbBlue;
        target[ID_RGBA_GREEN] = palette[source[cols]].rgbGreen;
        target[ID_RGBA_RED] = palette[source[cols]].rgbRed;

        target += 3;
    }
}



static inline   void
convertLine32To24(unsigned char  *target, unsigned char  *source, int width_in_pixels) {
    for (int cols = 0; cols < width_in_pixels; cols++) {
        target[ID_RGBA_BLUE] = source[ID_RGBA_BLUE];
        target[ID_RGBA_GREEN] = source[ID_RGBA_GREEN];
        target[ID_RGBA_RED] = source[ID_RGBA_RED];

        target += 3;
        source += 4;
    }
}







static inline  void
AssignPixel(unsigned char * dst, const unsigned char * src, unsigned bytesperpixel) {
    switch (bytesperpixel) {
    case 1:
        *dst = *src;
        break;
    case 3:
        *(reinterpret_cast<short*>(dst)) = *(reinterpret_cast<const short*> (src));
        dst[2] = src[2];
        break;
    case 4:
        *(reinterpret_cast<unsigned long*>(dst)) = *(reinterpret_cast<const unsigned long*> (src));
        break;
    default:
        break;
    }
}






#define hasPixels  ((getWidth()*getHeight()) != 0)

bmpImage::bmpImage()
{
    m_hBitmap = nullptr;
}


bmpImage::bmpImage(bmpImage & b)
{
    m_hBitmap = nullptr;
    m_hBitmap = cloneImage(b.m_hBitmap);

}


bmpImage::~bmpImage()
{
    clear();
}





bool bmpImage::copyFrombmpImage(bmpImage & b)
{
    if (isValid())clear();
    m_hBitmap = cloneImage(b.m_hBitmap);
    return (m_hBitmap != nullptr);
}






bool bmpImage::create(int width, int height, unsigned char  color)
{
    bool res = false;
    size_t size;
    if (isValid())clear();
    // Create the image
    setSize(width, height);
    try {
        size = getWidth() *getHeight() * 3;  //24bpp
        memset(getBits(), color, size);
        res = true;
    }
    catch (...) {}
    return res;
}


bool bmpImage::isValid(void)
{
    bool res = false;
    if (m_hBitmap != nullptr && (*(short *)(m_hBitmap) == 0x4d42))
        res = true;
    return res;
}


unsigned long bmpImage::getTotalSize()
{
    if (m_hBitmap != nullptr)
        return m_hBitmap->bfSize;
    else
        return 0;
}

bool bmpImage::clear(void)
{
    bool res = false;
    if (m_hBitmap != nullptr)
    {
        freeBitmap(m_hBitmap);
        m_hBitmap = nullptr;
        res = true;
    }
    return res;
}

bool bmpImage::detach(void)
{
    m_hBitmap = nullptr;
    return true;
}


bool bmpImage::attach(bmpImage & b)
{
    clear();
    m_hBitmap = (LPBITMAPFILEHEADER)b;
    b.detach();
    return true;
}

bool bmpImage::attach(LPBITMAPFILEHEADER p)
{
    clear();
    m_hBitmap = p;
    return true;
}



bool bmpImage::portrait(bool mirror)
{
    bool res = false;
    if (getWidth() > getHeight())
    {
        res = rotate(270.0);
    }
    else
    {
        if (mirror)
            res = rotate(180.0);
        else
            res = true;
    }
    return res;
}

bool bmpImage::landscape(bool mirror)
{
    bool res = false;
    if (getWidth() < getHeight())
    {
        res = rotate(270.0);
    }
    else
    {
        if (mirror)
            res = rotate(180.0);
        else
            res = true;
    }
    return res;
}



bool bmpImage::check(unsigned int Width, unsigned int Height)
{
    return (Width == getWidth() && Height == getHeight());
}

LPBITMAPINFOHEADER bmpImage::getInfoHeader(LPBITMAPFILEHEADER bI)
{
    LPBITMAPINFOHEADER bm = nullptr;
    if (bI)
    {
        unsigned char  *p = (unsigned char *)(bI);
        p += sizeof(BITMAPFILEHEADER);
        bm = (LPBITMAPINFOHEADER)(p);
    }
    return bm;
}

LPBITMAPINFOHEADER  bmpImage::getInfoHeader() const
{
    return getInfoHeader(m_hBitmap);
}

LPBITMAPINFO bmpImage::getInfo(LPBITMAPFILEHEADER pI)
{
    LPBITMAPINFO p = nullptr;
    if (pI)
    {
        p = (LPBITMAPINFO)getInfoHeader(pI);
    }
    return p;
}

LPBITMAPINFO bmpImage::getInfo() const
{
    return getInfo(m_hBitmap);
}


void bmpImage::setHorizontalResolution(double value) {
    if (m_hBitmap)
    {
        LPBITMAPINFOHEADER bm = getInfoHeader();
        if (bm)
        {
            bm->biXPelsPerMeter = (unsigned)(value * 100 + 0.5);
        }
    }
}

void bmpImage::setVerticalResolution(double value) {
    if (m_hBitmap)
    {
        LPBITMAPINFOHEADER bm = getInfoHeader();
        if (bm)
        {
            bm->biYPelsPerMeter = (unsigned)(value * 100 + 0.5);
        }
    }
}


unsigned int bmpImage::getBitsPerPixel(LPBITMAPFILEHEADER bI)
{
    unsigned int res = 0;
    if (bI)
    {
        LPBITMAPINFOHEADER  bm = getInfoHeader(bI);
        res = bm->biBitCount;
    }
    return  res;
}


unsigned int bmpImage::getBitsPerPixel(void) const
{
    return  getBitsPerPixel(m_hBitmap);
}



LPRGBQUAD  bmpImage::getPalette(LPBITMAPFILEHEADER pI)
{
    LPRGBQUAD p = nullptr;
    if (pI)
    {
        if (getBitsPerPixel(pI) < 16)
            p = (LPRGBQUAD)(((unsigned char  *)getInfoHeader(pI)) + sizeof(BITMAPINFOHEADER));
    }
    return p;
}

LPRGBQUAD  bmpImage::getPalette(void)
{
    return getPalette(m_hBitmap);
}

unsigned int bmpImage::getColorsUsed(LPBITMAPFILEHEADER bI)
{
    unsigned int res = 0;
    if (bI)
    {
        BITMAPINFOHEADER * bm = getInfoHeader(bI);
        res = bm->biClrUsed;
    }
    return  res;
}

unsigned int bmpImage::getColorsUsed(void) const
{
    return  getColorsUsed(m_hBitmap);
}


bool bmpImage::replace(LPBITMAPFILEHEADER new_dib) {
    if (new_dib == nullptr)
        return false;
    if (new_dib != m_hBitmap)
    {
        if (m_hBitmap)
            freeBitmap(m_hBitmap);
        m_hBitmap = new_dib;
    }
    return true;
}


bool bmpImage::setSize(unsigned width, unsigned height) {
    if (m_hBitmap) {
        clear();
    }
    m_hBitmap = allocateBitmap(width, height);
    if (!m_hBitmap)
        return false;
    return true;
}

unsigned int bmpImage::getWidth(LPBITMAPFILEHEADER bI)
{
    unsigned int res = 0;
    if (bI)
    {
        BITMAPINFOHEADER * bm = getInfoHeader(bI);
        res = bm->biWidth;
    }
    return  res;
}

unsigned int bmpImage::getWidth() const
{
    return  getWidth(m_hBitmap);
}

unsigned int bmpImage::getHeight(void)const
{
    return getHeight(m_hBitmap);
}

unsigned int bmpImage::getHeight(LPBITMAPFILEHEADER bI)
{
    unsigned int res = 0;
    if (bI)
    {
        BITMAPINFOHEADER * bm = getInfoHeader(bI);
        res = bm->biHeight;
    }
    return  res;
}

unsigned char * bmpImage::getScanLine(LPBITMAPFILEHEADER bI, unsigned scanline)
{
    if (bI)
    {
        if (scanline < getHeight(bI)) {
            return CalculateScanLine(getBits(bI), getPitch(bI), scanline);
        }
    }
    return nullptr;
}


unsigned char * bmpImage::getScanLine(unsigned scanline)
{
    return getScanLine(m_hBitmap, scanline);
}



unsigned int bmpImage::getLine(LPBITMAPFILEHEADER bI)
{
    unsigned int res = 0;
    if (bI)
    {
        res = ((getWidth(bI) * getBitsPerPixel(bI)) + 7) / 8;
    }
    return  res;
}

unsigned int bmpImage::getLine(void) const
{
    return getLine(m_hBitmap);
}

unsigned int bmpImage::getPitch(LPBITMAPFILEHEADER bI)
{
    unsigned int res = 0;
    if (bI)
    {
        res = CalculatePitch(getLine(bI));
    }
    return  res;
}

unsigned int  bmpImage::getPitch(void) const
{
    return  getPitch(m_hBitmap);
}

unsigned char  * bmpImage::getBits(LPBITMAPFILEHEADER bI)
{
    unsigned char  *res = nullptr;
    if (bI)
    {
        size_t lp, wp;
        // returns the pixels aligned on a FIBITMAP_ALIGNMENT bytes alignment boundary
        lp = (size_t)getInfoHeader(bI);
        lp += sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * getColorsUsed(bI);
        wp = (size_t)(bI)+(size_t)(bI->bfOffBits);
#ifdef _DEBUG
        if (lp != wp)
        {
            TRACE("%s WARNING WP!=LP\n", __func__);
        }
#endif
        res = (unsigned char  *)(tMIN<size_t>(lp, wp));
    }
    return res;
}


unsigned char  * bmpImage::getBits(void)
{
    return getBits(m_hBitmap);
}


unsigned int  bmpImage::getColorType(void)
{
    return getColorType(m_hBitmap);
}


unsigned int bmpImage::getColorType(LPBITMAPFILEHEADER bI)
{
    RGBQUAD *rgb;

    // standard image type
    if (bI)
    {
        switch (getBitsPerPixel(bI)) {
        case 1:
        {
            rgb = getPalette(bI);

            if ((rgb->rgbRed == 0) && (rgb->rgbGreen == 0) && (rgb->rgbBlue == 0)) {
                rgb++;

                if ((rgb->rgbRed == 255) && (rgb->rgbGreen == 255) && (rgb->rgbBlue == 255)) {
                    return FIC_MINISBLACK;
                }
            }

            if ((rgb->rgbRed == 255) && (rgb->rgbGreen == 255) && (rgb->rgbBlue == 255)) {
                rgb++;

                if ((rgb->rgbRed == 0) && (rgb->rgbGreen == 0) && (rgb->rgbBlue == 0)) {
                    return FIC_MINISWHITE;
                }
            }

            return FIC_PALETTE;
        }

        case 4:
        case 8:	// Check if the DIB has a color or a greyscale palette
        {
            int ncolors = getColorsUsed(bI);
            int minisblack = 1;
            rgb = getPalette(bI);

            for (int i = 0; i < ncolors; i++) {
                if ((rgb->rgbRed != rgb->rgbGreen) || (rgb->rgbRed != rgb->rgbBlue)) {
                    return FIC_PALETTE;
                }

                // The DIB has a color palette if the greyscale isn't a linear ramp
                // Take care of reversed grey images
                if (rgb->rgbRed != i) {
                    if ((ncolors - i - 1) != rgb->rgbRed) {
                        return FIC_PALETTE;
                    }
                    else {
                        minisblack = 0;
                    }
                }

                rgb++;
            }

            return minisblack ? FIC_MINISBLACK : FIC_MINISWHITE;
        }

        case 16:
        case 24:
            return FIC_RGB;

        case 32:
        {
            // check for fully opaque alpha layer
            if (getHeight(bI) > 0)
            {
                rgb = (RGBQUAD *)getScanLine(bI, 0);

                for (unsigned int x = 0; x < getWidth(bI); x++)
                {
                    if (rgb[x].rgbReserved != 0xFF)
                    {
                        return FIC_RGBALPHA;
                    }
                }
                return FIC_RGB;
            }

            return FIC_RGBALPHA;
        }

        }
    }
    return FIC_MINISBLACK;
}


bool bmpImage::invert(void)
{
    bool res = false;
    if (hasPixels)
    {

        unsigned i, x, y, k;
        const unsigned width = getWidth();
        const unsigned height = getHeight();
        const unsigned bpp = getBitsPerPixel();
        switch (bpp) {
        case 1:
        case 4:
        case 8:
        {
            if (getColorType() == FIC_PALETTE) {
                RGBQUAD *pal = getPalette();

                for (i = 0; i < getColorsUsed(); i++) {
                    pal[i].rgbRed = 255 - pal[i].rgbRed;
                    pal[i].rgbGreen = 255 - pal[i].rgbGreen;
                    pal[i].rgbBlue = 255 - pal[i].rgbBlue;
                }
            }
            else {
                for (y = 0; y < height; y++) {
                    unsigned char  *bits = getScanLine(y);
                    for (x = 0; x < getLine(); x++) {
                        bits[x] = ~bits[x];
                    }
                }
            }
        }
        break;

        case 24:
        case 32:
        {
            // Calculate the number of bytes per pixel (3 for 24-bit or 4 for 32-bit)
            const unsigned bytespp = getLine() / width;

            for (y = 0; y < height; y++) {
                unsigned char  *bits = getScanLine(y);
                for (x = 0; x < width; x++) {
                    for (k = 0; k < bytespp; k++) {
                        bits[k] = ~bits[k];
                    }
                    bits += bytespp;
                }
            }

        }
        break;
        }
    }
    return res;
}


size_t bmpImage::getInternalImageSize(unsigned int width, unsigned int height)
{
    size_t dib_size = sizeof(BITMAPFILEHEADER);
    dib_size += sizeof(BITMAPINFOHEADER);
    // palette is aligned on a 16 bytes boundary
    const size_t header_size = dib_size;
    // pixels are aligned on a 16 bytes boundary
    dib_size += (size_t)CalculatePitch(CalculateLine(width, bitPerPlane)) * (size_t)height;
    const double dPitch = floor(((double)bitPerPlane * width + 31.0) / 32.0) * 4.0;
    const double dImageSize = (double)header_size + dPitch * height;
    if (dImageSize != (double)dib_size) {
        // here, we are sure to encounter a malloc overflow: try to avoid it ...
        return 0;
    }
    return dib_size;
}

LPBITMAPFILEHEADER  bmpImage::allocateBitmap(unsigned  int width, unsigned int height)
{
    LPBITMAPFILEHEADER res = nullptr;

    size_t dib_size = getInternalImageSize(width, height);

    if (dib_size > 0)
    {

        res = (LPBITMAPFILEHEADER)malloc(dib_size);
        res->bfSize = (unsigned long)(dib_size & 0xffffffff);
        res->bfType = 0x4d42;
        res->bfOffBits = res->bfReserved1 = res->bfReserved2 = 0;
        BITMAPINFOHEADER *bih = getInfoHeader(res);
        bih->biSize = sizeof(BITMAPINFOHEADER);
        bih->biWidth = width;
        bih->biHeight = height;
        bih->biPlanes = 1;
        bih->biCompression = 0L;
        bih->biBitCount = (short)bitPerPlane;
        bih->biClrUsed = 0;
        bih->biClrImportant = bih->biClrUsed;
        bih->biXPelsPerMeter = 2835;	// 72 dpi
        bih->biYPelsPerMeter = 2835;	// 72 dpi
        bih->biSizeImage = res->bfSize - sizeof(BITMAPFILEHEADER) - sizeof(BITMAPINFOHEADER);
        res->bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + (sizeof(RGBQUAD) * bih->biClrUsed);

    }
    return res;
}

void bmpImage::freeBitmap(LPBITMAPFILEHEADER bI)
{
    if (bI)
    {
        free(bI);
    }
}

bool bmpImage::convertTo24Bits(void)
{
    bool res = false;
    if (hasPixels)
    {
        const unsigned bpp = getBitsPerPixel();

        const int width = getWidth();
        const int height = getHeight();


        if (bpp == 24)
        {
            res = true;
        }
        else
        {
            LPBITMAPFILEHEADER new_dib = allocateBitmap(width, height);
            if (new_dib != nullptr)
            {

                switch (bpp) {
                case 1:
                {
                    for (int rows = 0; rows < height; rows++) {
                        convertLine1To24(getScanLine(new_dib, rows), getScanLine(rows), width, getPalette());
                    }
                    freeBitmap(m_hBitmap);
                    m_hBitmap = new_dib;
                    res = true;
                }
                break;
                case 4:
                {
                    for (int rows = 0; rows < height; rows++) {
                        convertLine4To24(getScanLine(new_dib, rows), getScanLine(rows), width, getPalette());
                    }
                    freeBitmap(m_hBitmap);
                    m_hBitmap = new_dib;
                    res = true;
                }
                break;

                case 8:
                {
                    for (int rows = 0; rows < height; rows++) {
                        convertLine8To24(getScanLine(new_dib, rows), getScanLine(rows), width, getPalette());
                    }
                    freeBitmap(m_hBitmap);
                    m_hBitmap = new_dib;
                    res = true;
                }
                break;


                case 32:
                {
                    for (int rows = 0; rows < height; rows++) {
                        convertLine32To24(getScanLine(new_dib, rows), getScanLine(rows), width);
                    }
                    freeBitmap(m_hBitmap);
                    m_hBitmap = new_dib;
                    res = true;
                }
                break;
                }

            }

        }

    }
    return res;
}








bool bmpImage::rotate(double angle)
{
    bool res = false;
    if (isValid() && hasPixels && getBitsPerPixel() == bitPerPlane)
    {
        LPBITMAPFILEHEADER rotated = rotateInt(m_hBitmap, angle);
        res = replace(rotated);

    }
    return res;
}

LPBITMAPFILEHEADER bmpImage::Rotate90(LPBITMAPFILEHEADER src)
{
    LPBITMAPFILEHEADER res = nullptr;
    const unsigned bpp = getBitsPerPixel(src);

    const unsigned src_width = getWidth(src);
    const unsigned src_height = getHeight(src);
    const unsigned dst_width = src_height;
    const unsigned dst_height = src_width;


    // allocate and clear dst image
    LPBITMAPFILEHEADER dst = allocateBitmap(dst_width, dst_height);
    if (dst)
    {

        // get src and dst scan width
        const unsigned src_pitch = getPitch(src);
        const unsigned dst_pitch = getPitch(dst);

        if (bpp == 1) {
            // speedy rotate for BW images

            unsigned char  *bsrc = getBits(src);
            unsigned char  *bdest = getBits(dst);

            unsigned char  *dbitsmax = bdest + dst_height * dst_pitch - 1;

            for (unsigned y = 0; y < src_height; y++) {
                // figure out the column we are going to be copying to
                const div_t div_r = div(y, 8);
                // set bit pos of src column byte
                const unsigned char  bitpos = (unsigned char)(128 >> div_r.rem);
                unsigned char  *srcdisp = bsrc + y * src_pitch;
                for (unsigned x = 0; x < src_pitch; x++) {
                    // get source bits
                    unsigned char  *sbits = srcdisp + x;
                    // get destination column
                    unsigned char  *nrow = bdest + (dst_height - 1 - (x * 8)) * dst_pitch + div_r.quot;
                    for (int z = 0; z < 8; z++) {
                        // get destination byte
                        unsigned char  *dbits = nrow - z * dst_pitch;
                        if ((dbits < bdest) || (dbits > dbitsmax)) break;
                        if (*sbits & (128 >> z)) *dbits |= bitpos;
                    }
                }
            }
            res = dst;
        }
        else if ((bpp == 8) || (bpp == 24) || (bpp == 32)) {
            // anything other than BW :
            // This optimized version of rotation rotates image by smaller blocks. It is quite
            // a bit faster than obvious algorithm, because it produces much less CPU cache misses.
            // This optimization can be tuned by changing block size (RBLOCK). 96 is good value for current
            // CPUs (tested on Athlon XP and Celeron D). Larger value (if CPU has enough cache) will increase
            // speed somehow, but once you drop out of CPU's cache, things will slow down drastically.
            // For older CPUs with less cache, lower value would yield better results.

            unsigned char  *bsrc = getBits(src);  // source pixels
            unsigned char  *bdest = getBits(dst);  // destination pixels

            // calculate the number of bytes per pixel (1 for 8-bit, 3 for 24-bit or 4 for 32-bit)
            const unsigned bytespp = getLine(src) / getWidth(src);

            // for all image blocks of RBLOCK*RBLOCK pixels

            // x-segment
            for (unsigned xs = 0; xs < dst_width; xs += RBLOCK) {
                // y-segment
                for (unsigned ys = 0; ys < dst_height; ys += RBLOCK) {
                    for (unsigned y = ys; y < tMIN(dst_height, ys + RBLOCK); y++) {    // do rotation
                        const unsigned y2 = dst_height - y - 1;
                        // point to src pixel at (y2, xs)
                        unsigned char  *src_bits = bsrc + (xs * src_pitch) + (y2 * bytespp);
                        // point to dst pixel at (xs, y)
                        unsigned char  *dst_bits = bdest + (y * dst_pitch) + (xs * bytespp);
                        for (unsigned x = xs; x < tMIN(dst_width, xs + RBLOCK); x++) {
                            // dst.SetPixel(x, y, src.GetPixel(y2, x));
                            AssignPixel(dst_bits, src_bits, bytespp);
                            dst_bits += bytespp;
                            src_bits += src_pitch;
                        }
                    }
                }
            }
            res = dst;
        }
    }
    return res;
}


LPBITMAPFILEHEADER bmpImage::Rotate180(LPBITMAPFILEHEADER src)
{

    LPBITMAPFILEHEADER res = nullptr;
    unsigned int x, y, k, pos;

    const unsigned int bpp = getBitsPerPixel(src);

    const unsigned int src_width = getWidth(src);
    const unsigned int src_height = getHeight(src);
    const unsigned int dst_width = src_width;
    const unsigned int dst_height = src_height;


    LPBITMAPFILEHEADER dst = allocateBitmap(dst_width, dst_height);
    if (NULL != dst)
    {
        if (bpp == 1)
        {
            for (y = 0; y < src_height; y++) {
                unsigned char  *src_bits = getScanLine(src, y);
                unsigned char  *dst_bits = getScanLine(dst, dst_height - y - 1);
                for (x = 0; x < src_width; x++) {
                    // get bit at (x, y)
                    k = (src_bits[x >> 3] & (0x80 >> (x & 0x07))) != 0;
                    // set bit at (dst_width - x - 1, dst_height - y - 1)
                    pos = dst_width - x - 1;
                    k ? dst_bits[pos >> 3] |= (0x80 >> (pos & 0x7)) : dst_bits[pos >> 3] &= (0xFF7F >> (pos & 0x7));
                }
            }
            res = dst;
        }
        else if ((bpp == 8) || (bpp == 24) || (bpp == 32))
        {
            // Calculate the number of bytes per pixel
            const int bytespp = getLine(src) / getWidth(src);

            for (y = 0; y < src_height; y++) {
                unsigned char  *src_bits = getScanLine(src, y);
                unsigned char  *dst_bits = getScanLine(dst, dst_height - y - 1) + (dst_width - 1) * bytespp;
                for (x = 0; x < src_width; x++) {
                    // get pixel at (x, y)
                    // set pixel at (dst_width - x - 1, dst_height - y - 1)
                    AssignPixel(dst_bits, src_bits, bytespp);
                    src_bits += bytespp;
                    dst_bits -= bytespp;
                }
            }
            res = dst;
        }
    }
    return dst;
}




LPBITMAPFILEHEADER bmpImage::Rotate270(LPBITMAPFILEHEADER src)
{

    LPBITMAPFILEHEADER res = nullptr;

    int x2, dlineup;

    const unsigned bpp = getBitsPerPixel(src);

    const unsigned src_width = getWidth(src);
    const unsigned src_height = getHeight(src);
    const unsigned dst_width = src_height;
    const unsigned dst_height = src_width;

    LPBITMAPFILEHEADER dst = allocateBitmap(dst_width, dst_height);
    if (NULL != dst)
    {

        // get src and dst scan width
        const unsigned src_pitch = getPitch(src);
        const unsigned dst_pitch = getPitch(dst);

        if (bpp == 1) {
            // speedy rotate for BW images

            unsigned char  *bsrc = getBits(src);
            unsigned char  *bdest = getBits(dst);
            unsigned char  *dbitsmax = bdest + dst_height * dst_pitch - 1;
            dlineup = 8 * dst_pitch - dst_width;

            for (unsigned y = 0; y < src_height; y++) {
                // figure out the column we are going to be copying to
                const div_t div_r = div(y + dlineup, 8);
                // set bit pos of src column byte
                const unsigned char  bitpos = (unsigned char)(1 << div_r.rem);
                const unsigned char  *srcdisp = bsrc + y * src_pitch;
                for (unsigned x = 0; x < src_pitch; x++) {
                    // get source bits
                    const unsigned char  *sbits = srcdisp + x;
                    // get destination column
                    unsigned char  *nrow = bdest + (x * 8) * dst_pitch + dst_pitch - 1 - div_r.quot;
                    for (unsigned z = 0; z < 8; z++) {
                        // get destination byte
                        unsigned char  *dbits = nrow + z * dst_pitch;
                        if ((dbits < bdest) || (dbits > dbitsmax)) break;
                        if (*sbits & (128 >> z)) *dbits |= bitpos;
                    }
                }
            }
            res = dst;
        }
        else if ((bpp == 8) || (bpp == 24) || (bpp == 32)) {
            // anything other than BW :
            // This optimized version of rotation rotates image by smaller blocks. It is quite
            // a bit faster than obvious algorithm, because it produces much less CPU cache misses.
            // This optimization can be tuned by changing block size (RBLOCK). 96 is good value for current
            // CPUs (tested on Athlon XP and Celeron D). Larger value (if CPU has enough cache) will increase
            // speed somehow, but once you drop out of CPU's cache, things will slow down drastically.
            // For older CPUs with less cache, lower value would yield better results.

            unsigned char  *bsrc = getBits(src);  // source pixels
            unsigned char  *bdest = getBits(dst);  // destination pixels

            // Calculate the number of bytes per pixel (1 for 8-bit, 3 for 24-bit or 4 for 32-bit)
            const unsigned bytespp = getLine(src) / getWidth(src);

            // for all image blocks of RBLOCK*RBLOCK pixels

            // x-segment
            for (unsigned xs = 0; xs < dst_width; xs += RBLOCK) {
                // y-segment
                for (unsigned ys = 0; ys < dst_height; ys += RBLOCK) {
                    for (unsigned x = xs; x < tMIN(dst_width, xs + RBLOCK); x++) {    // do rotation
                        x2 = dst_width - x - 1;
                        // point to src pixel at (ys, x2)
                        unsigned char  *src_bits = bsrc + (x2 * src_pitch) + (ys * bytespp);
                        // point to dst pixel at (x, ys)
                        unsigned char  *dst_bits = bdest + (ys * dst_pitch) + (x * bytespp);
                        for (unsigned y = ys; y < tMIN(dst_height, ys + RBLOCK); y++) {
                            // dst.SetPixel(x, y, src.GetPixel(y, x2));
                            AssignPixel(dst_bits, src_bits, bytespp);
                            src_bits += bytespp;
                            dst_bits += dst_pitch;
                        }
                    }
                }
            }
            res = dst;
        }
    }
    return dst;
}


bool bmpImage::flipHorizontal(void)
{
    bool res = false;
    if (hasPixels)
    {

        unsigned line = getLine();
        unsigned width = getWidth();
        unsigned height = getHeight();

        unsigned bytespp = getLine() / getWidth();

        // copy between aligned memories
        unsigned char  *new_bits = (unsigned char  *)malloc(line * sizeof(unsigned char));

        if (new_bits)
        {
            // mirror the buffer
            for (unsigned y = 0; y < height; y++) {
                unsigned char  *bits = getScanLine(y);
                memcpy(new_bits, bits, line);

                switch (getBitsPerPixel())
                {
                case 1:
                {
                    for (unsigned x = 0; x < width; x++) {
                        // get pixel at (x, y)
                        bool value = (new_bits[x >> 3] & (0x80 >> (x & 0x07))) != 0;
                        // set pixel at (new_x, y)
                        unsigned new_x = width - 1 - x;
                        value ? bits[new_x >> 3] |= (0x80 >> (new_x & 0x7)) : bits[new_x >> 3] &= (0xff7f >> (new_x & 0x7));
                    }
                }
                res = true;
                break;

                case 4:
                {
                    for (unsigned c = 0; c < line; c++) {
                        bits[c] = new_bits[line - c - 1];

                        unsigned char  nibble = (bits[c] & 0xF0) >> 4;

                        bits[c] = bits[c] << 4;
                        bits[c] |= nibble;
                    }
                }
                res = true;
                break;

                case 8:
                {
                    unsigned char  *dst_data = (unsigned char *)bits;
                    unsigned char  *src_data = (unsigned char *)(new_bits + line - bytespp);
                    for (unsigned c = 0; c < width; c++) {
                        *dst_data++ = *src_data--;
                    }
                }
                res = true;
                break;


                case 24:
                case 32:
                {
                    unsigned char  *dst_data = (unsigned char *)bits;
                    unsigned char  *src_data = (unsigned char *)(new_bits + line - bytespp);
                    for (unsigned c = 0; c < width; c++) {
                        for (unsigned k = 0; k < bytespp; k++) {
                            *dst_data++ = src_data[k];
                        }
                        src_data -= bytespp;
                    }
                }
                res = true;
                break;
                }
            }
            free(new_bits);
        }
    }
    return res;
}


bool bmpImage::flipVertical(void)
{
    bool res = false;
    if (hasPixels)
    {
        unsigned char  *From, *Mid;

        unsigned pitch = getPitch();
        unsigned height = getHeight();

        // copy between aligned memories
        Mid = (unsigned char *)malloc(pitch * sizeof(unsigned char));
        if (Mid)
        {

            From = getBits();

            unsigned int line_s = 0;
            unsigned int line_t = (height - 1) * pitch;

            for (unsigned y = 0; y < height / 2; y++) {

                memcpy(Mid, From + line_s, pitch);
                memcpy(From + line_s, From + line_t, pitch);
                memcpy(From + line_t, Mid, pitch);

                line_s += pitch;
                line_t -= pitch;

            }
            res = true;
            free(Mid);
        }
    }
    return res;
}

LPBITMAPFILEHEADER bmpImage::RotateAny(LPBITMAPFILEHEADER src, double dAngle)
{
    LPBITMAPFILEHEADER res = nullptr;
    if (src) {
        LPBITMAPFILEHEADER image = src;

        while (dAngle >= 360) {
            // Bring angle to range of (-INF .. 360)
            dAngle -= 360;
        }
        while (dAngle < 0) {
            // Bring angle to range of [0 .. 360)
            dAngle += 360;
        }
        if ((dAngle > 45) && (dAngle <= 135)) {
            // Angle in (45 .. 135]
            // Rotate image by 90 degrees into temporary image,
            // so it requires only an extra rotation angle
            // of -45 .. +45 to complete rotation.
            image = Rotate90(src);
            dAngle -= 90;
        }
        else if ((dAngle > 135) && (dAngle <= 225)) {
            // Angle in (135 .. 225]
            // Rotate image by 180 degrees into temporary image,
            // so it requires only an extra rotation angle
            // of -45 .. +45 to complete rotation.
            image = Rotate180(src);
            dAngle -= 180;
        }
        else if ((dAngle > 225) && (dAngle <= 315)) {
            // Angle in (225 .. 315]
            // Rotate image by 270 degrees into temporary image,
            // so it requires only an extra rotation angle
            // of -45 .. +45 to complete rotation.
            image = Rotate270(src);
            dAngle -= 270;
        }

        res = image;
    }
    return res;
}



LPBITMAPFILEHEADER bmpImage::rotateInt(LPBITMAPFILEHEADER bI, double angle)
{
    LPBITMAPFILEHEADER res = nullptr;

    if (0 == (int)(angle * 100))
    {
        res = bI;
    }
    else
    {
        //angle *= -1;
        unsigned bpp = getBitsPerPixel(bI);
        if (bpp == 1) {
            // only rotate for integer multiples of 90 degree
            if (fmod(angle, 90) == 0)
            {

                // perform the rotation
                LPBITMAPFILEHEADER dst = RotateAny(bI, angle);
                if (dst != nullptr)
                {
                    // build a greyscale palette
                    RGBQUAD *dst_pal = getPalette(dst);
                    if (dst_pal)
                    {
                        if (getColorType(bI) == FIC_MINISBLACK) {
                            dst_pal[0].rgbRed = dst_pal[0].rgbGreen = dst_pal[0].rgbBlue = 0;
                            dst_pal[1].rgbRed = dst_pal[1].rgbGreen = dst_pal[1].rgbBlue = 255;
                        }
                        else {
                            dst_pal[0].rgbRed = dst_pal[0].rgbGreen = dst_pal[0].rgbBlue = 255;
                            dst_pal[1].rgbRed = dst_pal[1].rgbGreen = dst_pal[1].rgbBlue = 0;
                        }
                    }
                    res = dst;
                }
            }
        }
        else if ((bpp == 8) || (bpp == 24) || (bpp == 32)) {
            LPBITMAPFILEHEADER dst = RotateAny(bI, angle);
            if (dst != nullptr)
            {
                if (bpp == 8) {
                    // copy original palette to rotated bitmap
                    RGBQUAD *src_pal = getPalette(bI);
                    RGBQUAD *dst_pal = getPalette(dst);
                    if (dst_pal)
                        memcpy(&dst_pal[0], &src_pal[0], 256 * sizeof(RGBQUAD));

                }
                res = dst;
            }
        }
    }
    return res;
}



bool bmpImage::pasteSubImage(LPBITMAPFILEHEADER src, int left, int top)
{
    return pasteSubImage(m_hBitmap, src, left, top);
}

bool bmpImage::pasteSubImage(LPBITMAPFILEHEADER dst, LPBITMAPFILEHEADER src, int left, int top)
{
    return copyBits(dst, src, left, top, getWidth(src), getHeight(src));
}

bool bmpImage::translateColor(unsigned char oriRed, unsigned char oriGreen,
    unsigned char oriBlue, unsigned char newRed,
    unsigned char newGreen, unsigned char newBlue)
{
    return translateColor(m_hBitmap, oriRed, oriGreen,
        oriBlue, newRed,
         newGreen,newBlue);
}




bool bmpImage::copyBits(bmpImage & dst, size_t left, size_t top, size_t right, size_t bottom)
{
    return copyBits(m_hBitmap, (LPBITMAPFILEHEADER)dst, left, top, right - left, bottom - top);
}


bool bmpImage::drawSprite( bmpImage & sprite, int left, int top)
{
    return drawSprite(m_hBitmap, sprite, left, top);
}

bool bmpImage::drawMaskSprite( bmpImage & sprite, int left, int top,unsigned char Rmask,unsigned char Gmask,unsigned char Bmask)
{
    return drawMaskSprite(m_hBitmap, sprite, left, top,Rmask,Gmask,Bmask);
}


bool bmpImage::drawSprite(LPBITMAPFILEHEADER dst,  LPBITMAPFILEHEADER sprite, size_t left, size_t top)
{
    bool bResult = false;
    if (sprite != nullptr  && dst != nullptr)
    {
        unsigned int src_width = getWidth(sprite);
        unsigned int src_height = getHeight(sprite);
        unsigned int src_pitch = getPitch(sprite);
        unsigned int src_line = getLine(sprite);
        unsigned int dst_width = getWidth(dst);
        unsigned int dst_height = getHeight(dst);
        unsigned int dst_pitch = getPitch(dst);
        LPBITMAPINFOHEADER dst_info = getInfoHeader(dst);
        unsigned int dst_line = getLine(dst);
        LPBITMAPINFOHEADER src_info = getInfoHeader(sprite);
        unsigned  int depth = dst_line / dst_width;
        if (depth != src_line / src_width)
            return false;
        // check the size of src image

        if (dst_info != nullptr  && src_info != nullptr  && dst_info->biBitCount == src_info->biBitCount)
        {
            unsigned int height, width;
            if (( left < dst_width) && ( top < dst_height)) {
                unsigned char  *dst_bits = getBits(dst) + (top * dst_pitch) + (left * depth);
                unsigned char  *src_bits = getBits(sprite);
                // combine images
                if (top + src_height > dst_height)
                    height = dst_height - top;
                else
                    height = src_height;

                if (left + src_width>dst_width)
                    width = dst_width - left;
                else
                    width = src_width;
                unsigned int sizeR= width*depth;
                for (unsigned int rows = 0; rows < height; rows++) {
                    memcpy(dst_bits, src_bits, sizeR);
                    dst_bits += dst_pitch;
                    src_bits += src_pitch;
                }
                bResult = true;
            }
        }
    }
    return bResult;
}





static inline   void
copyMaskLine(unsigned int depth,unsigned char  *target, unsigned char  *source,
               unsigned char Rmask,unsigned char Gmask,
               unsigned char Bmask,unsigned int width_in_pixels)
{
    unsigned int cols;
    if(depth==3)
    {
        for (cols = 0; cols < width_in_pixels; cols++) 
        {
            if (!(source[ID_RGBA_BLUE] == Bmask &&
                source[ID_RGBA_GREEN] == Gmask &&
                source[ID_RGBA_RED] == Rmask))
            {
                target[ID_RGBA_BLUE] = source[ID_RGBA_BLUE];
                target[ID_RGBA_GREEN] = source[ID_RGBA_GREEN];
                target[ID_RGBA_RED] = source[ID_RGBA_RED];
            }
            target += 3;
            source += 3;
        }
    }
    else
    if (depth == 4)
    {
        unsigned int mask = (Rmask << 16) | (Gmask << 8) | Bmask;
        unsigned int *dest_t = reinterpret_cast<unsigned int *>(target);
        unsigned int *src_t = reinterpret_cast<unsigned int *>(source);
        for (cols = 0; cols < width_in_pixels; cols++)
        {
            if ((*src_t & 0xffffff) != mask)
            {
                *dest_t = *src_t;
            }
            dest_t++;
            src_t++;
        }
    }
}

bool bmpImage::drawMaskSprite(LPBITMAPFILEHEADER dst, LPBITMAPFILEHEADER sprite, size_t left, size_t top, unsigned char Rmask,unsigned char Gmask,unsigned char Bmask)
{
    bool bResult = false;
    if (sprite != nullptr  && dst != nullptr)
    {
        unsigned int src_width = getWidth(sprite);
        unsigned int src_height = getHeight(sprite);
        unsigned int src_pitch = getPitch(sprite);
        unsigned int src_line = getLine(sprite);
        unsigned int dst_width = getWidth(dst);
        unsigned int dst_height = getHeight(dst);
        unsigned int dst_pitch = getPitch(dst);
        LPBITMAPINFOHEADER dst_info = getInfoHeader(dst);
        unsigned int dst_line = getLine(dst);
        LPBITMAPINFOHEADER src_info = getInfoHeader(sprite);
        unsigned  int depth = dst_line / dst_width;
        if (depth != src_line / src_width)
            return false;
        // check the size of src image

        if (dst_info != nullptr  && src_info != nullptr  && dst_info->biBitCount == src_info->biBitCount)
        {
            unsigned int height, width;
            if (( left < dst_width) && ( top < dst_height)) {
                unsigned char  *dst_bits = getBits(dst) + (top * dst_pitch) + (left * depth);
                unsigned char  *src_bits = getBits(sprite);

                // combine images
                if (top + src_height > dst_height)
                    height = dst_height - top;
                else
                    height = src_height;

                if (left + src_width>dst_width)
                    width = dst_width - left;
                else
                    width = src_width;
                for (unsigned int rows = 0; rows < height; rows++)
                {
                    copyMaskLine(depth,dst_bits, src_bits, Rmask,Gmask,Bmask,width);
                    dst_bits += dst_pitch;
                    src_bits += src_pitch;
                }
                bResult = true;
            }
        }
    }
    return bResult;
}



bool bmpImage::copyBits(LPBITMAPFILEHEADER src, LPBITMAPFILEHEADER dst, size_t left, size_t top, size_t width, size_t height)
{
    bool bResult = false;
    if (src != nullptr  && dst != nullptr)
    {
        unsigned int src_width = getWidth(src);
        unsigned int src_height = getHeight(src);
        unsigned int src_pitch = getPitch(src);
        unsigned int src_line = getLine(src);
        unsigned int dst_width = getWidth(dst);
        unsigned int dst_height = getHeight(dst);
        unsigned int dst_pitch = getPitch(dst);
        LPBITMAPINFOHEADER dst_info = getInfoHeader(dst);
        unsigned int dst_line = getLine(dst);
        LPBITMAPINFOHEADER src_info = getInfoHeader(src);
        unsigned  int depth = dst_line / dst_width;
        if (depth != src_line / src_width)
            return false;
        // check the size of src image
        // check the size of src image
        if (dst_info != nullptr  && src_info != nullptr  && dst_info->biBitCount == src_info->biBitCount)
        {
            if ((left < src_width && left < dst_width) && (top < src_height && top < dst_height)) {
                unsigned char  *dst_bits = getBits(dst) + (top * dst_pitch) + (left * depth);
                unsigned char  *src_bits = getBits(src) + (top * src_pitch) + (left * depth);
                // combine images
                if (top + height > src_height)
                    height = src_height - top;
                if (top + height > dst_height)
                    height = dst_height - top;
                if (left + width > src_width)
                    width = src_width - left;
                if (left + width>dst_width)
                    width = dst_width - left;
                for (unsigned int rows = 0; rows < height; rows++) {
                    memcpy(dst_bits, src_bits, width*depth);
                    dst_bits += dst_pitch;
                    src_bits += src_pitch;
                }
                bResult = true;
            }
        }
    }
    return bResult;
}


bool bmpImage::copyFromFile(const XCHAR *name)
{
    bool res = false;
    try {
        FILE *file = nullptr;
        std::string sname;
#ifdef _UNICODE
        std::wstring_convert<std::codecvt_utf8_utf16<XCHAR>,XCHAR> convert;
        sname = convert.to_bytes(name);
#else
        sname=name;
#endif
        file = FOPEN(sname.c_str(),"rb");
        if ( file != nullptr)
        {
            short mask = 0;
            struct _stat  file_status;
            int err = _fstat(_fileno(file), &file_status);
            if (err == 0)
            {
                err = fread(&mask, 1, sizeof(short), file);
                fflush(file);
                fclose(file);
            }
            if (err == sizeof(short) && mask == 0x4d42)
            {
                file = FOPEN(sname.c_str(),"rb");
                if ( file != nullptr)
                {
                    if (file_status.st_size > 0 && file_status.st_size < (1024 * 1024 * 32)) //32Mbit max
                    {

                        if (isValid())clear();
                        m_hBitmap = (LPBITMAPFILEHEADER)malloc((size_t)file_status.st_size);
                        if (m_hBitmap)
                        {
                            unsigned char  *p = (unsigned char  *)m_hBitmap;
                            size_t temp = (size_t)file_status.st_size;
                            size_t start = 0;
                            do {
                                err = fread(&p[start], 1, 256, file);
                                if (!err)
                                {
                                    break;
                                }
                                start += err;
                                temp -= err;
                            } while (temp > 0);
                            if (temp > 0)
                            {
                                clear();
                            }
                        }
                    }
                    fflush(file);
                    fclose(file);
                }
            }
        }
    }
    catch (...)
    {
    }
    return res = (m_hBitmap != nullptr);
}


bool bmpImage::copyToFile(const XCHAR *name)
{
    bool res = false;
    if (isValid())
    {
        try {
            FILE *file = nullptr;
            std::string sname;
#ifdef _UNICODE
            std::wstring_convert<std::codecvt_utf8_utf16<XCHAR>,XCHAR> convert;
            sname = convert.to_bytes(name);
#else
            sname=name;
#endif
            file = FOPEN(sname.c_str(), "w+");
            if ( file != nullptr)
            {
                auto err = fwrite(m_hBitmap, 1, m_hBitmap->bfSize, file);
                fflush(file);
                fclose(file);
                res = (err == m_hBitmap->bfSize);
            }
        }
        catch (...)
        {
        }
    }
    return res;
}


LPBITMAPFILEHEADER bmpImage::cloneImage(LPBITMAPFILEHEADER pI)
{
    LPBITMAPFILEHEADER copy = nullptr;
    if (pI != nullptr  && pI->bfType == 0x4d42)
    {
        copy = allocateBitmap(getWidth(pI), getHeight(pI));
        if (copy != nullptr  && copy->bfSize == pI->bfSize)
        {
            memcpy(copy, pI, pI->bfSize);
        }
    }
    return copy;
}







void bmpImage::operator =(bmpImage & b)
{
    clear(); //reset this;
    if (b.isValid())
    {
        if (b.m_hBitmap->bfSize < m_hBitmap->bfSize)
            memcpy(m_hBitmap, b.m_hBitmap, b.m_hBitmap->bfSize);
        else
        {
            clear();
            cloneImage((LPBITMAPFILEHEADER)b);
        }
    }
}






bool bmpImage::swaptoBGR(void)
{
    bool res = false;
    if (isValid())
    {
        if (getBitsPerPixel() != 24)
        {
            res = convertTo24Bits();
            if (!res)
                return res;
        }
        res = swapto(m_hBitmap, 0, 2);
    }
    return res;
}

bool bmpImage::swaptoRBG(void)
{
    bool res = false;
    if (isValid())
    {
        if (getBitsPerPixel() != 24)
        {
            res = convertTo24Bits();
            if (!res)
                return res;
        }
        res = swapto(m_hBitmap, 1, 2);
    }
    return res;
}

bool bmpImage::swaptoGRB(void)
{
    bool res = false;
    if (isValid())
    {
        if (getBitsPerPixel() != 24)
        {
            res = convertTo24Bits();
            if (!res)
                return res;
        }
        res = swapto(m_hBitmap, 0, 1);
    }
    return res;
}

bool bmpImage::swapto(LPBITMAPFILEHEADER pI, int iSorg, int iDest)
{
    bool res = false;
    if (pI != nullptr)
    {
        unsigned int bpp = getBitsPerPixel(pI);
        if (bpp == 24 || bpp == 32)
        {
            // must be 24 RGBRGBRGB
            const unsigned bytespp = getLine(pI) / getWidth(pI);
            const unsigned int size = getWidth(pI)*getHeight(pI)*bytespp;
            unsigned int xs;
            unsigned char * ptr = getBits(pI);
            for (xs = 0; xs < size; xs += bytespp) {
                unsigned char r = ptr[xs + iSorg];
                ptr[xs + iSorg] = ptr[xs + iDest];
                ptr[xs + iDest] = r;
            }
            res = true;
        }
    }
    return res;
}


///////////////////////////////////////////////////
bool bmpImage::setPixel(unsigned int _x,unsigned int _y,unsigned char red,unsigned char green,unsigned char blue)
{
    return setPixel(m_hBitmap,_x,_y,red,green,blue);
}

bool bmpImage::setPixel(LPBITMAPFILEHEADER dest,unsigned int x,unsigned int y,unsigned char red,unsigned char green,unsigned char blue)
{
    bool res=false;
    unsigned int width,height;
    width=getWidth(dest);
    height=getHeight(dest);
    if(x>width)
        x=width;
    if(y>height)
        y=height;
    unsigned int pitch = getPitch(dest);
    unsigned int line = getLine(dest);
    unsigned  int depth = line / width;
    unsigned char  *bits = getBits(dest) + (y * pitch) + (x * depth);
    bits[ID_RGBA_BLUE]=blue;
    bits[ID_RGBA_GREEN]=green;
    bits[ID_RGBA_RED]=red;
    res=true;
    return res;
}

bool bmpImage::getPixel(unsigned int x,unsigned int y,unsigned char  & red,unsigned char & green,unsigned char & blue)
{
    return getPixel(m_hBitmap,x,y,red,green,blue);
}


bool bmpImage::getPixel(LPBITMAPFILEHEADER dest,unsigned int x,unsigned int y,unsigned char  & red,unsigned char & green,unsigned char & blue)
{
    bool res=false;
    unsigned int width,height;
    width=getWidth(dest);
    height=getHeight(dest);
    if(x>width)
        x=width;
    if(y>height)
        y=height;
    unsigned int pitch = getPitch(dest);
    unsigned int line = getLine(dest);
    unsigned int depth = line / width;
    unsigned char *bits = getBits(dest) + (y * pitch) + (x * depth);
    blue=bits[ID_RGBA_BLUE];
    green=bits[ID_RGBA_GREEN];
    red=bits[ID_RGBA_RED];
    res=true;
    return res;
}

bool bmpImage::line( int x1, int y1, int x2, int y2,
                    unsigned char red,unsigned char green,
                     unsigned char blue,unsigned int mask)
{
    return line(m_hBitmap,x1,y1,x2,y2,red,green,blue,mask);
}

bool bmpImage::line(LPBITMAPFILEHEADER dest,  int x1,  int y1,  int x2,  int y2,
                    unsigned char red, unsigned char green, unsigned char blue, unsigned int maskDot)
{
    bool res=false;
    if(maskDot==0)return true;
    int width,height;
    width=(int)getWidth(dest);
    height = (int)getHeight(dest);
    if (width == 0)return false;
    if (height == 0)return false;
    unsigned int pitch = getPitch(dest);
    unsigned int line = getLine(dest);
    unsigned  int depth = line / width;
    unsigned char  *bits = getBits(dest);
    int dx,dy,end,x,y,mb;
    mb=1;
    int p;
    if(y1>y2)
        dy=y1-y2;
    else
        dy=y2-y1;
    if(x1>x2)
    {
        dx=x1-x2;
        x = x2;
        y = y2;
        if(dx>dy)
        {
            end = x1;
        }
        else
        {
            end = y1;
        }
    }
    else
    {
        dx=x2-x1;
        x = x1;
        y = y1;
        if(dx!=0)
        {
            if(dx>dy)
                end = x2;
            else
                end = y2;
        }
        else
        {
            if(y1>y2)
            {
                y=y2;
                x=x2;
                end=y1;
            }
            else
            {
                y=y1;
                x=x1;
                end=y2;
            }
        }
    }
    if(dy==0)
    {
        if (y>=0 && y < height)
        do {
            if(x<0)goto skip_for_clipy;
            if(x>=width)goto skip_for_clipy;
            if(y>=height)break;
            if (y >= height)break;
            register unsigned char  *pos=bits+(y * pitch) + (x * depth);
            if(mb&maskDot)
            {
                pos[ID_RGBA_BLUE]=blue;
                pos[ID_RGBA_GREEN]=green;
                pos[ID_RGBA_RED]=red;
            }
skip_for_clipy:
            x++;
            mb<<=1;
            if(!mb)mb=1;
        } while(x<end);
        res=true;
    }
    else if(dx==0)
    {
        if (x >= 0 && x < width)
        do {
            if(y<0)goto skip_for_clipx;
            if(y>=height)goto skip_for_clipx;
            register unsigned char  *pos=bits+(y * pitch) + (x * depth);
            if(mb&maskDot)
            {
                pos[ID_RGBA_BLUE]=blue;
                pos[ID_RGBA_GREEN]=green;
                pos[ID_RGBA_RED]=red;
            }
skip_for_clipx:
            y++;
            mb<<=1;
            if(!mb)mb=1;
        } while(y<end);
        res=true;
    }
    else
    {
        if(dx>dy)
        {
            p = 2 * dy - dx;
            do {
                if(x<0)goto skip_for_clip1;
                if(y<0)goto skip_for_clip1;
                if(x>=width)goto skip_for_clip1;
                if(y>=height)goto skip_for_clip1;
                register unsigned char  *pos=bits+(y * pitch) + (x * depth);
                if(mb&maskDot)
                {
                    pos[ID_RGBA_BLUE]=blue;
                    pos[ID_RGBA_GREEN]=green;
                    pos[ID_RGBA_RED]=red;
                }
skip_for_clip1:
                x++;
                mb<<=1;
                if(!mb)mb=1;
                if(p < 0)
                {
                    p = p + 2 * dy;
                }
                else
                {
                    y++;
                    p = p + 2 * (dy - dx);
                }
            } while(x<end);
            res=true;
        }
        else
        {
            p = 2 * dx - dy;
            do {
                if(x<0)goto skip_for_clip2;
                if(y<0)goto skip_for_clip2;
                if(x>=width)goto skip_for_clip2;
                if(y>=height)goto skip_for_clip2;
                register unsigned char  *pos=bits+(y * pitch) + (x * depth);
                if(mb&maskDot)
                {
                    pos[ID_RGBA_BLUE]=blue;
                    pos[ID_RGBA_GREEN]=green;
                    pos[ID_RGBA_RED]=red;
                }
skip_for_clip2:
                y++;
                mb<<=1;
                if(!mb)mb=1;
                if(p < 0)
                {
                    p = p + 2 * dx;
                }
                else
                {
                    x++;
                    p = p + 2 * (dx - dy);
                }
            } while(y<end);
            res=true;
        }
    }
    res=true;
    return res;
}

bool bmpImage::frameRect( int x1,  int y1,  int x2,  int y2, unsigned char red, unsigned char green, unsigned char blue,unsigned int mask)
{
    bool res=false;
    if(line(x1,y1,x2,y1,red,green,blue,mask))
    {
        if(line(x1,y2,x2,y2,red,green,blue,mask))
        {
            if(line(x1,y1,x1,y2,red,green,blue,mask))
            {
                res=line(x2,y1,x2,y2,red,green,blue,mask);
            }
        }
    }
    return res;
}

static inline   void
translateLine( unsigned int depth,unsigned char  *target, unsigned int width_in_pixels,
    unsigned char Rmask, unsigned char Gmask, unsigned char Bmask,
    unsigned char newR, unsigned char newG, unsigned char newB )
{
    unsigned int cols;
    if (depth == 3)
    {
        for (cols = 0; cols < width_in_pixels; cols++) {
            if (target[ID_RGBA_BLUE] == Bmask &&
                target[ID_RGBA_GREEN] == Gmask &&
                target[ID_RGBA_RED] == Rmask)
            {
                target[ID_RGBA_BLUE] = newB;
                target[ID_RGBA_GREEN] = newG;
                target[ID_RGBA_RED] = newR;
            }
            target += depth;
        }
    }
    else
    if (depth == 4)
    {
        unsigned int oldCol = (Rmask << 16) | (Gmask << 8) | Bmask;
        unsigned int newCol = (newR << 16) | (newG << 8) | newB;
        unsigned int *ptr = reinterpret_cast<unsigned int *>(target);
        for ( cols = 0; cols < width_in_pixels; cols++) {
            if ((*ptr&0xffffff) == oldCol)
                *ptr = newCol;
            ptr++;
        }
    }
}



bool bmpImage::translateColor(LPBITMAPFILEHEADER dest, unsigned char oriRed, unsigned char oriGreen,
    unsigned char oriBlue, unsigned char newRed,
    unsigned char newGreen , unsigned char  newBlue)
{
    bool bResult = false;
    if ( dest != nullptr)
    {
        unsigned int dst_width = getWidth(dest);
        unsigned int dst_height = getHeight(dest);
        unsigned int dst_pitch = getPitch(dest);
        LPBITMAPINFOHEADER dst_info = getInfoHeader(dest);
        unsigned int dst_line = getLine(dest);
        unsigned  int depth = dst_line / dst_width;
        // check the size of src image
        if (dst_info != nullptr  )
        {
            unsigned char  *dst_bits = getBits(dest);
                for (unsigned int rows = 0; rows < dst_height; rows++)
                {
                    translateLine(depth,dst_bits,dst_width, oriRed, oriGreen, oriBlue, newRed,newGreen,newBlue);
                    dst_bits += dst_pitch;
                }
                bResult = true;
        }
    }
    return bResult;
}



///////////////////////////////////////////////////////////////// IMAGE LIST
listImage::listImage()
{

}


listImage::~listImage()
{
    listImage::iterator _it = begin();
    listImage::iterator _end = end();
    while (_it != _end)
    {
        if (_it->second != nullptr)
            delete _it->second;
        _it++;
    }
    clear();
}


bool listImage::Add(int index, bmpImage & b)
{
    bool res = false;
    if (b.isValid())
    {
        bmpImage* t = new bmpImage();
        if (t != nullptr)
        {
            t->attach(b);
            (*this)[index] = t;
            res = true;
        }
    }
    return res;
}





bool bmpSprite::fromImages(bmpImage & obj, bmpImage & mask)
{
    int obj_width = obj.getWidth();
    int obj_height = obj.getHeight();
    int mask_width = mask.getWidth();
    int mask_height = mask.getHeight();
    int width = min(obj_width, mask_width);
    int height = min(obj_height, mask_height);
    obj.convertTo24Bits();
    mask.convertTo24Bits();
    create(width, height, 0);
    return toSprite(m_hBitmap, (LPBITMAPFILEHEADER)obj, (LPBITMAPFILEHEADER)mask);
}


static inline   void
maskLine24(unsigned char  *target, unsigned char  *source, unsigned char  *mask, int width_in_pixels) {
    for (int cols = 0; cols < width_in_pixels; cols++) {
        target[ID_RGBA_BLUE] = source[ID_RGBA_BLUE] & mask[ID_RGBA_BLUE];
        target[ID_RGBA_GREEN] = source[ID_RGBA_GREEN] & mask[ID_RGBA_GREEN];
        target[ID_RGBA_RED] = source[ID_RGBA_RED] & mask[ID_RGBA_RED];
        target += 3;
        source += 3;
        mask += 3;
    }
}

bool bmpSprite::toSprite(LPBITMAPFILEHEADER dest, LPBITMAPFILEHEADER image, LPBITMAPFILEHEADER mask)
{
    bool res = false;
    //must be all 24bpp
    //TODO REMOVE ...
    try
    {
        unsigned int rows, height = bmpImage::getHeight(dest);
        unsigned int width = bmpImage::getWidth(dest);
        for (rows = 0; rows < height; rows++)
        {
            maskLine24(bmpImage::getScanLine(rows), bmpImage::getScanLine(image, rows), bmpImage::getScanLine(mask, rows), width);
        }
        res = true;
    }
    catch (...) {}
    return res;
}
