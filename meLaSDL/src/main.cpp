#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <unistd.h>
#include <stdio.h>
#include <limits.h>
#include <n2logger.h>
#include <n2interfaces.h>
#include <n2iface.h>
#include <images.h>
#include <n2appmanager.h>
#include <n2exception.h>
#include <n2statistics.h>

#define DUMP_BMP_TO_FILE 0

static IAppManager *n2App = nullptr;

static SDL_Texture *update_n2app(SDL_Renderer *rend);

static void hook(void *obj, size_t v, IParam *param)
{
    const char *p = nullptr;
    switch (v)
    {
    case action_redraw:
        p = "ACTION REDRAW";
        {
            // clears the screen
            SDL_Renderer *rend = (SDL_Renderer *)obj;
            update_n2app(rend);
        }
        break;
    case action_update_statusbars_info:
        p = "ACTION UPDATE STATUSBARS INFO";
        break;
    case action_update_selected_panes:
        p = "ACTION UPDATE SELECTED PANES";
        break;
    case action_update_scroller_panes:
        p = "ACTION UPDATE SCROLLED PANES";
        break;
    case action_align_windows:
        p = "ACTION ALIGN WINDOWS";
        break;
    case action_iconize_windows:
        p = "ACTION ICONIZE WINDOWS";
        break;
    case action_medialize_windows:
        p = "ACTION MEDIALIZE WINDOWS";
        break;
    case action_maximize_windows:
        p = "ACTION MAXIMIZE WINDOWS";
        break;
    case action_close_windows:
        p = "ACTION CLOSE WINDOWS";
        break;
    case action_move_window:
        p = "ACTION MOVE WINDOWS";
        break;
    case action_select_position:
        p = "ACTION SELECT POSITION";
        break;
    case action_host_command:
        p = "ACTION HOST COMMAND";
        break;
    default:
        p = "UNKNOW ACTION";
        break;
    }
    if (param)
    {
        Log5("hook %s : Obj:%p S:%lu P:%p", p, obj, v, param);
    }
    else
    {
        Log5("hook %s : Obj:%p S:%lu ", p, obj, v);
    }
}

static void openlog(void)
{
#if _LOGGER_
    ILogger *logger = new nnLogger();
    IPrinter *printer = new nnDefaultPrinter();
    logger->setOutput(printer);
    Log3("log Start ... ",nullptr);

#if _STATISTICS_
    IStatistics *stat = new nnStatistics();
#endif

#endif
}

static SDL_Point link_n2app()
{
    SDL_Point sdlp;
    sdlp.x = sdlp.y = 0;
    IChild *n2Client = nullptr;
    char cwd[PATH_MAX];
    getcwd(cwd, sizeof(cwd));
    n2App = new n2AppManager();
    STRING s("conf_utf8.xml");
    STRING p("resource/");
    n2Client = n2App->createObjects(s, p);
    if (n2App)
    {
        n2Client = nullptr;
        n2Client = n2App->active();
        if (n2Client != nullptr)
        {
            nnPoint p = n2Client->getView()->getPhySize();
            sdlp.x = p.x;
            sdlp.y = p.y;
        }
    }
    return sdlp;
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
        SDL_RenderClear(rend);
        unsigned int width = bmp.getWidth();
        unsigned int height = bmp.getHeight();
        unsigned int depth = bmp.getBitsPerPixel();
        unsigned int pitch = bmp.getPitch();
        SDL_Surface *surface = SDL_CreateRGBSurfaceWithFormatFrom(bmp.getBits(),
                               width,
                               height,
                               depth,
                               pitch,
                               SDL_PIXELFORMAT_RGBA8888 );
        tex = SDL_CreateTextureFromSurface(rend, surface);
        SDL_FreeSurface(surface);
        if (tex != nullptr)
        {
            SDL_RenderCopy(rend, tex, nullptr, nullptr);
            SDL_DestroyTexture(tex);
        }
    }
    return tex;
}

static bool ctrl = false;
static bool shift = false;
static bool alt = false;
static int blocknum = 0;

static void keyCommand(SDL_Event *e)
{
    if (e)
    {
        SDL_KeyboardEvent *key = &e->key;
        if (key->type == SDL_KEYDOWN)
        {
            Log5("KEYDOWN EVENT %s (%d): %d W:%d S:%d R:%d", SDL_GetScancodeName(key->keysym.scancode),
                 key->keysym.scancode,
                 key->timestamp, key->windowID, key->state, key->repeat);
            if (key->keysym.scancode == SDL_SCANCODE_LCTRL || key->keysym.scancode == SDL_SCANCODE_RCTRL)
            {
                ctrl = true;
            }
            else if (key->keysym.scancode == SDL_SCANCODE_LSHIFT || key->keysym.scancode == SDL_SCANCODE_RSHIFT)
            {
                shift = true;
            }
            else if (key->keysym.scancode == SDL_SCANCODE_LALT || key->keysym.scancode == SDL_SCANCODE_RALT)
            {
                alt = true;
            }
            else if (key->keysym.scancode == SDL_SCANCODE_NUMLOCKCLEAR)
            {
                if (blocknum == 0)
                {
                    blocknum++;
                }
                else if (blocknum == 2)
                {
                    blocknum = 0;
                }
            }
            IHandler *handler = n2App->active();
            if (handler)
            {
                SDL_Scancode code = key->keysym.scancode;
                if (blocknum == 2)
                {
                    if (code == SDL_SCANCODE_KP_3)
                    {
                        code = SDL_SCANCODE_PAGEDOWN;
                    }
                    else if (code == SDL_SCANCODE_KP_9)
                    {
                        code = SDL_SCANCODE_PAGEUP;
                    }
                }
                switch (code)
                {
                case SDL_SCANCODE_ESCAPE:
                    Log5("exec handlerEscapeButton",nullptr);
                    handler->handlerEscapeButton(shift, ctrl, alt);
                    break;
                case SDL_SCANCODE_HOME:
                    Log5("exec handlerHomeButton",nullptr);
                    handler->handlerHomeButton(shift, ctrl, alt);
                    break;
                case SDL_SCANCODE_END:
                    Log5("exec handlerEndButton",nullptr);
                    handler->handlerEndButton(shift, ctrl, alt);
                    break;
                case SDL_SCANCODE_PAGEUP:
                    Log5("exec handlerPageUpButton",nullptr);
                    handler->handlerPageUpButton(shift, ctrl, alt);
                    break;
                case SDL_SCANCODE_PAGEDOWN:
                    Log5("exec handlerPageDownButton",nullptr);
                    handler->handlerPageDownButton(shift, ctrl, alt);
                    break;
                case SDL_SCANCODE_LEFT:
                    Log5("exec handlerLeftButton",nullptr);
                    handler->handlerLeftButton(shift, ctrl, alt);
                    break;
                case SDL_SCANCODE_UP:
                    Log5("exec handlerUpButton",nullptr);
                    handler->handlerUpButton(shift, ctrl, alt);
                    break;
                case SDL_SCANCODE_RIGHT:
                    Log5("exec handlerRightButton",nullptr);
                    handler->handlerRightButton(shift, ctrl, alt);
                    break;
                case SDL_SCANCODE_DOWN:
                    Log5("exec handlerDownButton",nullptr);
                    handler->handlerDownButton(shift, ctrl, alt);
                    break;
                }
            }
        }
        else if (key->type == SDL_KEYUP)
        {
            Log5("KEYUP EVENT %s (%d): %d W:%d S:%d R:%d", SDL_GetScancodeName(key->keysym.scancode),
                 key->keysym.scancode,
                 key->timestamp, key->windowID, key->state, key->repeat);
            if (key->keysym.scancode == SDL_SCANCODE_LCTRL || key->keysym.scancode == SDL_SCANCODE_RCTRL)
            {
                ctrl = false;
            }
            else if (key->keysym.scancode == SDL_SCANCODE_LSHIFT || key->keysym.scancode == SDL_SCANCODE_RSHIFT)
            {
                shift = false;
            }
            else if (key->keysym.scancode == SDL_SCANCODE_LALT || key->keysym.scancode == SDL_SCANCODE_RALT)
            {
                alt = false;
            }
            else if (key->keysym.scancode == SDL_SCANCODE_NUMLOCKCLEAR)
            {
                if (blocknum == 1)
                {
                    blocknum++;
                }
            }
        }
        else
        {
            printf("Unknow key code type event\n");
        }
    }
}

static void mouseMove(SDL_Event *e)
{
    if (e)
    {
        SDL_MouseMotionEvent *m = &e->motion;
        /*Log5("MOUSE MOTION EVENT  %d W:%d S:%d w:%d X:%d Y:%d Sx=%d Sy:%d",
             m->timestamp, m->windowID, m->state, m->which,
             m->x, m->y, m->xrel, m->yrel);
             */

        IHandler *handler = n2App->active();
        if (handler)
        {
            nnPoint pos(m->x, m->y);
            if (m->state == SDL_BUTTON_LMASK)
            {
                // Log5("exec handlerMouseMove with left button down");
                handler->handlerMouseMove(nn_m_button_left, pos);
            }
            else if (m->state == 0)
            {
                // Log5("exec handlerMouseMove");
                handler->handlerMouseMove(nn_m_button_unknow, pos);
            }
        }
    }
}

static void mouseButtonDown(SDL_Event *e)
{
    if (e)
    {
        SDL_MouseButtonEvent *m = &e->button;
        Log5("MOUSE BUTTON DOWN EVENT  %d W:%d S:%d w:%d B:%d X:%d Y:%d ",
             m->timestamp, m->windowID, m->state, m->which,
             m->button, m->x, m->y);

        IHandler *handler = n2App->active();
        if (handler)
        {
            nn_mouse_buttons bt = nn_m_button_unknow;
            switch (m->button)
            {
            case SDL_BUTTON_LMASK:
                bt = nn_m_button_left;
                break;
            case SDL_BUTTON_MIDDLE:
                bt = nn_m_button_middle;
                break;
            case SDL_BUTTON_RIGHT:
                bt = nn_m_button_right;
                break;
            }
            if (bt != nn_m_button_unknow)
            {
                nnPoint pos(m->x, m->y);
                Log5("exec handlerMouseButtonDown",nullptr);
                handler->handlerMouseButtonDown(bt, pos);
            }
        }
    }
}

static void mouseButtonUp(SDL_Event *e)
{
    if (e)
    {
        SDL_MouseButtonEvent *m = &e->button;
        Log5("MOUSE BUTTON UP EVENT  %d W:%d S:%d w:%d B:%d X:%d Y:%d ",
             m->timestamp, m->windowID, m->state, m->which,
             m->button, m->x, m->y);

        IHandler *handler = n2App->active();
        if (handler)
        {
            nn_mouse_buttons bt = nn_m_button_unknow;
            switch (m->button)
            {
            case SDL_BUTTON_LMASK:
                bt = nn_m_button_left;
                break;
            case SDL_BUTTON_MIDDLE:
                bt = nn_m_button_middle;
                break;
            case SDL_BUTTON_RIGHT:
                bt = nn_m_button_right;
                break;
            }
            if (bt != nn_m_button_unknow)
            {
                nnPoint pos(m->x, m->y);
                Log5("exec handlerMouseButtonUp",nullptr);
                handler->handlerMouseButtonUp(bt, pos);
            }
        }
    }
}

int main(int argc, char *argv[])
{
    openlog();
    try
    {
        SDL_Point p = link_n2app();
        if (p.x < 200 || p.y < 200)
        {
            printf("window size on configuration file too little\n");
            exit(-1);
        }
        Log5("WINDOW SIZE : %d - %d", p.x, p.y);
        // returns zero on success else non-zero
        if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        {
            printf("error initializing SDL: %s\n", SDL_GetError());
            exit(-2);
        }
        SDL_Window *win = SDL_CreateWindow("test", // creates a window
                                           SDL_WINDOWPOS_CENTERED,
                                           SDL_WINDOWPOS_CENTERED,
                                           p.x, p.y, SDL_WINDOW_RESIZABLE);

        // triggers the program that controls
        // your graphics hardware and sets flags
        Uint32 render_flags = SDL_RENDERER_ACCELERATED;

        // creates a renderer to render our images
        SDL_Renderer *rend = SDL_CreateRenderer(win, -1, render_flags);

        n2App->active()->setExtHandler(hook, rend);
        // clears main-memory

        // let us control our image position
        // so that we can move it with our keyboard.
        // controls animation loop
        int close = 0;

        // speed of box
        int speed = 300;
        hook(rend, action_redraw, nullptr);
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
                case SDL_KEYUP:
                    keyCommand(&event);
                    break;
                case SDL_MOUSEMOTION:
                    mouseMove(&event);
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    mouseButtonDown(&event);
                    break;
                case SDL_MOUSEBUTTONUP:
                    mouseButtonUp(&event);
                    break;
                case SDL_MOUSEWHEEL:
                    break;
                }
            }

            // triggers the double buffers
            // for multiple rendering
            SDL_RenderPresent(rend);

            // calculates to 10 fps
            SDL_Delay(1000 / 10);
        }

        // destroy renderer
        SDL_DestroyRenderer(rend);

        // destroy window
        SDL_DestroyWindow(win);

        // close SDL
        SDL_Quit();
        Log3("log Stop ... ",nullptr);
        n2App->closeAll();
        ;
    }
    catch (n2exception *e)
    {
        printf("ERROR - %s\n", e->msg());
        delete e;
    }

    catch (bmpImageException *e)
    {
        printf("ERROR - %s\n", e->msg());
        delete e;
    }

    return 0;
}