#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <unistd.h>
#include <stdio.h>
#include <limits.h>
#include <n2logiface.h>
#include <n2interfaces.h>
#include <n2iface.h>
#include <images.h>
#include <n2appmanager.h>
#include <n2exception.h>

#define PAGE_W  1000
#define PAGE_H  800

#define DUMP_BMP_TO_FILE 1

static IAppManager *n2App = nullptr;

static void exthandler(void *obj, size_t v, IParam *p)
{
}



static void link_n2app(size_t w, size_t h)
{
    IChild *n2Client = nullptr;
    char cwd[PATH_MAX];
    getcwd(cwd, sizeof(cwd));
    n2App = new n2AppManager();
    STRING s("conf_utf8.xml");
    STRING p("resource/");
    n2Client = n2App->createObjects(s,p);
    if (n2App)
    {
        n2Client = nullptr;
        n2Client = n2App->active();
        if (n2Client != nullptr)
        {
            n2Client->setExtHandler(exthandler, nullptr);
            n2Client->getView()->resize(w, h);
        }
    }
}

static SDL_Texture *update_n2app(SDL_Renderer *rend)
{
    SDL_Texture *tex = nullptr;
    IChild *child = n2App->active();
    if (child)
    {
        child->getView()->updateDraw();
        bmpImage &bmp = child->getView()->getDraw();
#if DUMP_BMP_TO_FILE
        bmp.copyToFile("/tmp/image.bmp");
#endif
        unsigned int width=bmp.getWidth();
        unsigned int height=bmp.getHeight();
        unsigned int depth=bmp.getBitsPerPixel();
        unsigned int pitch=bmp.getPitch();
        SDL_Surface *surface = SDL_CreateRGBSurfaceWithFormatFrom(bmp.getBits(),
                               width,
                               height,
                               depth,
                               pitch,
                               SDL_PIXELFORMAT_RGB24);
        tex = SDL_CreateTextureFromSurface(rend, surface);
        SDL_FreeSurface(surface);
    }
    return tex;
}

int main(int argc, char *argv[])
{
    try
    {
        // returns zero on success else non-zero
        if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        {
            printf("error initializing SDL: %s\n", SDL_GetError());
        }
        SDL_Window *win = SDL_CreateWindow("test", // creates a window
                                           SDL_WINDOWPOS_CENTERED,
                                           SDL_WINDOWPOS_CENTERED,
                                           PAGE_W, PAGE_H, 0);

        // triggers the program that controls
        // your graphics hardware and sets flags
        Uint32 render_flags = SDL_RENDERER_ACCELERATED;

        // creates a renderer to render our images
        SDL_Renderer *rend = SDL_CreateRenderer(win, -1, render_flags);

        link_n2app(PAGE_W, PAGE_H);

        // clears main-memory

        // let us control our image position
        // so that we can move it with our keyboard.
        SDL_Rect dest;
        dest.x = 0;
        dest.y = 0;
        dest.w = PAGE_W;
        dest.h = PAGE_H;

        // controls animation loop
        int close = 0;

        // speed of box
        int speed = 300;

        // animation loop
        while (!close)
        {
            SDL_Event event;

            // Events management
            while (SDL_PollEvent(&event))
            {
                switch (event.type)
                {

                case SDL_QUIT:
                    // handling of close button
                    close = 1;
                    break;

                case SDL_KEYDOWN:
                    break;
                }
            }

            // clears the screen
            SDL_RenderClear(rend);
            SDL_Texture *tex = update_n2app(rend);
            if (tex != nullptr)
            {
                SDL_RenderCopy(rend, tex, NULL, &dest);
                SDL_DestroyTexture(tex);
            }

            // triggers the double buffers
            // for multiple rendering
            SDL_RenderPresent(rend);

            // calculates to 60 fps
            SDL_Delay(1000 / 30);
        }

        // destroy renderer
        SDL_DestroyRenderer(rend);

        // destroy window
        SDL_DestroyWindow(win);

        // close SDL
        SDL_Quit();

        n2App->closeAll();;
    }
    catch (n2exception *e)
    {
        printf("ERROR : exception: %s\n",e->what());
        delete e;
    }

    catch (bmpImageException *e)
    {
        printf("ERROR : exception: %s\n",e->what());
        delete e;
    }

    return 0;
}