// meLaWin32.cpp : definisce il punto di ingresso dell'applicazione.
//

#include <windows.h>
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
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: inserire qui il codice.

    // Inizializzare le stringhe globali
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MELAWIN32, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Eseguire l'inizializzazione dall'applicazione:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MELAWIN32));

    MSG msg;

    // Ciclo di messaggi principale:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MELAWIN32));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MELAWIN32);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}



static nnAppManager *n2app = new nnAppManager();
static childApps * client = nullptr;
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


void directCommand(HWND hWnd,size_t user_param)
{
    // from conf...xml toolbars
    switch (user_param)
    {
    case 4000:
        break;
    case 4001:
        break;
    case 4002:
        break;
    case 4003:
        break;
    case 4004:
        break;
    case 4005:
        break;
    }
}


void externCommandRequest(void * dest, handlerAction type_param, size_t user_param)
{
    if (dest)
    {
        HWND hWnd = static_cast< HWND>(dest);
            switch (type_param)
            {
            case action_host_command:
                directCommand(hWnd,user_param);
                break;
            case action_update_statusbars_info:
            case action_update_statusbars_panes:
            case action_redraw:
                refreshPixmap(hWnd);
                break;
            }
    }
}



LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        if (lParam != 0)
        {
            CREATESTRUCT * cs = (CREATESTRUCT*)lParam;
            try {
                STRING s("conf_utf8.xml");
                client = n2app->createObjects(s);
                if (client)
                {
                    n2app->setExtHandler(client, handler_exec_command,
                        externCommandRequest,
                        hWnd
                        );
                    client->view->resize(cs->cx, cs->cy);
                }
            }
            catch (...) {}
        }
        break;
    case WM_SIZE:
    {
        int w = lParam & 0xffff;
        int h = lParam >> 16;
        if (client)
            client->view->resize(w, h);
    }
        break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Analizzare le selezioni di menu:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
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
            if (client != nullptr)
            {
                bmpImage *bmp = &client->view->getDraw();
                ::StretchDIBits(hdc, 0, 0,
                    bmp->getWidth(), bmp->getHeight(),
                    0, 0, bmp->getWidth(), bmp->getHeight(),
                    bmp->getBits(), (const BITMAPINFO *)bmp->getInfo(), 0, SRCCOPY);
            }
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_MOUSEMOVE:
    {
        int x = lParam & 0xffff;
        int y = lParam >> 16;
        if (client != nullptr)
        {
            nnPoint pos(x, y);
            if (wParam == MK_LBUTTON)
            {
                client->view->handlerMouseMove(nn_m_button_left, pos);
            }
            else
                if (wParam == 0)
                {
                    client->view->handlerMouseMove(nn_m_button_unknow, pos);
                }
        }
    }
    break;
    case WM_LBUTTONDOWN:
    {
        int x = lParam & 0xffff;
        int y = lParam >> 16;
        nnPoint pos(x, y);
        client->view->handlerMouseButtonDown(nn_m_button_left, pos);
    }
        break;
    case WM_LBUTTONUP:
    {
        int x = lParam & 0xffff;
        int y = lParam >> 16;
        nnPoint pos(x, y);
        client->view->handlerMouseButtonUp(nn_m_button_left, pos);
    }
    break;
    case WM_RBUTTONDOWN:
    {
        int x = lParam & 0xffff;
        int y = lParam >> 16;
        nnPoint pos(x, y);
        client->view->handlerMouseButtonDown(nn_m_button_right, pos);
    }
    break;
    case WM_RBUTTONUP:
    {
        int x = lParam & 0xffff;
        int y = lParam >> 16;
        nnPoint pos(x, y);
        client->view->handlerMouseButtonUp(nn_m_button_right, pos);
    }
    break;
    case WM_KEYDOWN: 
    {
        if (client)
        {

            bool alt = false, ctrl = false, shift = false;
            if (wParam == VK_HOME)
                client->view->handlerHomeButton(shift, ctrl, alt);
            if (wParam == VK_ESCAPE)
                client->view->handlerEscapeButton(shift, ctrl, alt);
            if (wParam == VK_END)
                client->view->handlerEndButton(shift, ctrl, alt);
            if (wParam == VK_LEFT)
                client->view->handlerLeftButton(shift, ctrl, alt);
            if (wParam == VK_UP)
                client->view->handlerUpButton(shift, ctrl, alt);
            if (wParam == VK_RIGHT)
                client->view->handlerRightButton(shift, ctrl, alt);
            if (wParam == VK_DOWN)
                client->view->handlerDownButton(shift, ctrl, alt);
            if (wParam == VK_PRIOR)
                client->view->handlerPageUpButton(shift, ctrl, alt);
            if (wParam == VK_NEXT)
                client->view->handlerPageDownButton(shift, ctrl, alt);
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
