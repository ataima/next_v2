// meLaWin32.cpp : definisce il punto di ingresso dell'applicazione.
//

#include <windows.h>
#include "..\..\n2draw\header\n2draw.h"
#include "..\..\n2draw\header\n2interfaces.h"
#include "..\..\n2draw\header\n2appmanager.h"
#include "..\..\bmpImage\header\images.h"

#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include "meLaWin32.h"

#define MAX_LOADSTRING 100

// Variabili globali:
HINSTANCE hInst;                                // istanza corrente
WCHAR szTitle[MAX_LOADSTRING];                  // Testo della barra del titolo
WCHAR szWindowClass[MAX_LOADSTRING];            // nome della classe di finestre principale

// Dichiarazioni con prototipo delle funzioni incluse in questo modulo di codice:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);


    // Inizializzare le stringhe globali
    //LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MELAWIN32, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Eseguire l'inizializzazione dall'applicazione:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    //HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MELAWIN32));

    MSG msg;

    // Ciclo di messaggi principale:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, nullptr, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNZIONE: MyRegisterClass()
//
//  SCOPO: registra la classe di finestre.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon =  LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MELAWIN32));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName = 0;// MAKEINTRESOURCEW(IDC_MELAWIN32);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm =  LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNZIONE: InitInstance(HINSTANCE, int)
//
//   SCOPO: salva l'handle di istanza e crea la finestra principale
//
//   COMMENTI:
//
//        In questa funzione l'handle di istanza viene salvato in una variabile globale e
//        la finestra di programma principale viene creata e visualizzata.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Memorizzare l'handle di istanza nella variabile globale

   HWND hWnd = CreateWindowExW(WS_EX_TRANSPARENT,szWindowClass, nullptr, WS_BORDER,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   
   SetWindowLong(hWnd, GWL_STYLE, 0);

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}



static nnAppManager *n2app = nullptr;
//
//  FUNZIONE: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  SCOPO:  elabora i messaggi per la finestra principale.
//
//  WM_COMMAND - elabora il menu dell'applicazione
//  WM_PAINT - disegna la finestra principale
//  WM_DESTROY - inserisce un messaggio di uscita e restituisce un risultato
//
//


void refreshPixmap(HWND hWnd)
{
    RECT rect;
    GetClientRect(hWnd, &rect);
    InvalidateRect(hWnd,&rect,0);
}


void directCommand(HWND hWnd, IParam *user_param)
{
    nnAbstractParam<int> *t = static_cast<nnAbstractParam<int>*>(user_param);
    if (t)
    {
        // from conf...xml toolbars
        int v = t->value();
        nnLOG1(int,v);
        switch (v)
        {
        case 2000:
            n2app->active()->Capture(2000, contactGenericAnd);
            break;
        case 2001:
            n2app->active()->Capture(2001, contactGenericOr);
            break;
        case 3000:
            n2app->active()->Capture(3000, coilGeneric);
            break;
        
        }
    }
}


void externCommandRequest(void * dest, size_t type_param, IParam *user_param)
{
    if (dest)
    {
        HWND hWnd = static_cast< HWND>(dest);
            switch (type_param)
            {
            case action_host_command:
                directCommand(hWnd,user_param);
                break;
            case action_update_selected_panes:
                break;
            case action_update_scroller_panes:
                break;
            case action_update_statusbars_info:
                break;
            case action_redraw:
                refreshPixmap(hWnd);
                break;
            case action_align_windows:
                {
                    nnAbstractParam<nnPoint> *t = static_cast<nnAbstractParam<nnPoint>*>(user_param);
                    if (t)
                    {
                        int x = t->value().x;
                        int y = t->value().y;
                        ::SetWindowPos(hWnd, 0, 0, 0, x, y, SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
                    }

                }
                break;
            case action_close_windows:  
                n2app->closeAll();
                delete n2app;
                n2app = nullptr;
                ::PostMessage(hWnd, WM_CLOSE, 0, 0L);
                break;
            case action_move_window:
                {
                    nnAbstractParam<nnPoint> *t = static_cast<nnAbstractParam<nnPoint>*>(user_param);
                    if (t)
                    {
                        RECT rect;
                        ::GetWindowRect(hWnd, &rect);
                        int x = t->value().x+rect.left;
                        int y = t->value().y+rect.top;
                        ::MoveWindow(hWnd, x, y, rect.right - rect.left, rect.bottom - rect.top, TRUE);
                        //::SetWindowPos(hWnd, 0, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);                        
                    }
                }
            break;
            case action_maximize_windows:
                ::ShowWindow(hWnd, SW_MAXIMIZE);
                break;
            case action_iconize_windows:
                ::ShowWindow(hWnd, SW_MINIMIZE);
                break;
            case action_medialize_windows:
                ::ShowWindow(hWnd, SW_SHOWDEFAULT);
                break;
            case action_select_position:
                {
                    if (user_param)
                    {
                        nnAbstractParamList *list = static_cast<nnAbstractParamList *>(user_param);
                        nnAbstractParam<nnPoint> *p1 = static_cast<nnAbstractParam<nnPoint> *>(list->at(0));
                        nnAbstractParam<int> *p2 = static_cast<nnAbstractParam<int> *>(list->at(1));
                        nnPoint pos = p1->value();
                        int command = p2->value();
                        nnLOG1(nnPoint, pos);
                        nnLOG1(int, command);
                        switch (command)
                        {
                        case 2000:
                            n2app->active()->addContact(pos, new nnContactNO());
                            break;
                        case 2001:
                            n2app->active()->addContact(pos, new nnContactNC());
                            break;
                        case 3000:
                            n2app->active()->addCoil(pos, new nnGenericCoil());
                            break;
                        }
                    }
                }
                break;
            }
    }
}

#if _LOGGER_
class testLogger
    :public IPrinter
{
    HWND editor;
public :
    testLogger(HWND hwnd) :editor(hwnd) {}
    void out(std::string & msg) {

        // move the caret to the end of the text
        int outLength = GetWindowTextLength(editor);
        if (outLength > 10000)
        {
            int pos = outLength - 10000;
            SendMessage(editor, EM_SETSEL, 0, pos);
            SendMessage(editor, EM_REPLACESEL, FALSE, 0L);
            outLength = GetWindowTextLength(editor);
        }
        // insert the text at the new caret position
        SendMessage(editor, EM_SETSEL, outLength, outLength);
        SendMessage(editor, EM_REPLACESEL, FALSE, reinterpret_cast<LPARAM>(msg.c_str()));

    }
};
#endif

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HWND edit=0;
    switch (message)
    {
    case WM_CREATE:
        if (lParam != 0)
        {
            CREATESTRUCT * cs = (CREATESTRUCT*)lParam;
            try {
                n2app = new nnAppManager();
                STRING s("conf_utf8.xml");
                STRING p(".\\");
                IChild *client = n2app->createObjects(s,p);
                if (client)
                {
                    client->setExtHandler( 
                        externCommandRequest,
                        hWnd
                        );
                }
#if _LOGGER_
                if (!edit)
                {
                    edit = CreateWindowA("EDIT", nullptr, WS_VSCROLL | WS_BORDER | WS_CHILD | ES_MULTILINE| ES_AUTOVSCROLL| ES_AUTOHSCROLL| ES_READONLY,
                        50, 50, 100, 100, hWnd, 0, 0, 0);
                    testLogger *tL = new testLogger(edit);
                    n2app->setPrinter(tL);
                }
#endif
            }
            catch (...) {}
        }
        break;
    case WM_SIZE:
    {
        int w = (int)(lParam & 0xffff);
        int h = (int)(lParam >> 16);
        if (n2app)
        {
            IChild * child = n2app->active();
            if(child)
                if (child->getView()->resize(w, h))
                {                    
                    if (edit)
                    {
                        MoveWindow(edit, 50,50, w-100, h-100, TRUE);
                    }
                }

        }
    }
        break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Analizzare le selezioni di menu:
            switch (wmId)
            {            
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            if (n2app != nullptr)
            {
                //SetMapMode(hdc, MM_LOENGLISH);
                IChild * child = n2app->active();
                if (child)
                {
                    bmpImage & bmp = child->getView()->getDraw();
                    if (bmp)
                    {
                        ::StretchDIBits(hdc, 0, 0,
                            bmp.getWidth(), bmp.getHeight(),
                            0, 0, bmp.getWidth(), bmp.getHeight(),
                            bmp.getBits(), (const BITMAPINFO *)bmp.getInfo(), 0, SRCCOPY);
                    }
                }
            }
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_MOUSEMOVE:
    {
        int x = (int)(lParam & 0xffff);
        int y = (int)(lParam >> 16);
        if (n2app != nullptr)
        {
            IHandler *handler = n2app->active();
            if (handler)
            {
                nnPoint pos(x, y);
                if (wParam == MK_LBUTTON)
                {
                    handler->handlerMouseMove(nn_m_button_left, pos);
                }
                else
                    if (wParam == 0)
                    {
                        handler->handlerMouseMove(nn_m_button_unknow, pos);
                    }
            }
        }
    }
    break;
    case WM_LBUTTONDOWN:
    {
        int x = (int)(lParam & 0xffff);
        int y = (int)(lParam >> 16);
        nnPoint pos(x, y);
        if (n2app != nullptr)
        {
            IHandler *handler = n2app->active();
            if(handler)
                handler->handlerMouseButtonDown(nn_m_button_left, pos);
        }
    }
        break;
    case WM_LBUTTONUP:
    {
        int x = (int)(lParam & 0xffff);
        int y = (int)(lParam >> 16);
        nnPoint pos(x, y);
        if (n2app != nullptr)
        {
            IHandler *handler = n2app->active();
            if(handler)
                handler->handlerMouseButtonUp(nn_m_button_left, pos);
        }
    }
    break;
    case WM_RBUTTONDOWN:
    {
        int x = (int)(lParam & 0xffff);
        int y = (int)(lParam >> 16);
        nnPoint pos(x, y);
        if (n2app != nullptr)
        {
            IHandler *handler = n2app->active();
            if(handler)
                handler->handlerMouseButtonDown(nn_m_button_right, pos);
        }
    }
    break;
    case WM_RBUTTONUP:
    {
        int x = (int)(lParam & 0xffff);
        int y = (int)(lParam >> 16);
        nnPoint pos(x, y);
        if (n2app != nullptr)
        {
            IHandler *handler = n2app->active();
            if(handler)
                handler->handlerMouseButtonUp(nn_m_button_right, pos);
        }
    }
    break;
    case WM_KEYDOWN: 
    {
#if _LOGGER_
        if (edit)
        {
            if (wParam == VK_F2)
                ::ShowWindow(edit, SW_SHOW);
            if (wParam == VK_F3)
                ::ShowWindow(edit, SW_HIDE);
        }
#endif
        if (n2app!=nullptr)
        {
            IHandler *handler = n2app->active();
            if (handler)
            {
                bool alt = false, ctrl = false, shift = false;
                if (wParam == VK_HOME)
                    handler->handlerHomeButton(shift, ctrl, alt);
                if (wParam == VK_ESCAPE)
                    handler->handlerEscapeButton(shift, ctrl, alt);
                if (wParam == VK_END)
                    handler->handlerEndButton(shift, ctrl, alt);
                if (wParam == VK_LEFT)
                    handler->handlerLeftButton(shift, ctrl, alt);
                if (wParam == VK_UP)
                    handler->handlerUpButton(shift, ctrl, alt);
                if (wParam == VK_RIGHT)
                    handler->handlerRightButton(shift, ctrl, alt);
                if (wParam == VK_DOWN)
                    handler->handlerDownButton(shift, ctrl, alt);
                if (wParam == VK_PRIOR)
                    handler->handlerPageUpButton(shift, ctrl, alt);
                if (wParam == VK_NEXT)
                    handler->handlerPageDownButton(shift, ctrl, alt);
            }
        }
    }
    break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Gestore dei messaggi della finestra Informazioni su.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
