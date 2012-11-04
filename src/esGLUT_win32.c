#include "esGLUT/esGLUT.h"

#ifdef ESGLUT_OS_WIN32

#include <malloc.h>

#include "esGLUT_internal.h"

#define REPEATED_KEYMASK	(1<<30)
#define EXTENDED_KEYMASK	(1<<24)

static HCURSOR _curArrow;
static unsigned char _vkMap[0xFF];
static unsigned char _isDown[0xFF];
static unsigned long _mouseButton;
static LPARAM _mouseMovePrevLParam;
static void _InitOSWinResources() {
    static GLboolean _initialized = GL_FALSE;
    if (_initialized) return;
    _initialized = GL_TRUE;
    _curArrow = LoadCursor(NULL,IDC_ARROW);
    memset(_isDown,0,sizeof(_isDown));
    memset(_vkMap,0,sizeof(_vkMap));
    _mouseButton = 0;
    _mouseMovePrevLParam = ~0;
    _vkMap[VK_UP] = GLUT_KEY_UP;
    _vkMap[VK_DOWN] = GLUT_KEY_DOWN;
    _vkMap[VK_RIGHT] = GLUT_KEY_RIGHT;
    _vkMap[VK_LEFT] = GLUT_KEY_LEFT;
    _vkMap[VK_PRIOR] = GLUT_KEY_PAGE_UP;
    _vkMap[VK_NEXT] = GLUT_KEY_PAGE_DOWN;
    _vkMap[VK_HOME] = GLUT_KEY_HOME;
    _vkMap[VK_END] = GLUT_KEY_END;
    _vkMap[VK_INSERT] = GLUT_KEY_INSERT;
    _vkMap[VK_DELETE] = GLUT_KEY_DELETE;
    _vkMap[VK_F1] = GLUT_KEY_F1;
    _vkMap[VK_F2] = GLUT_KEY_F2;
    _vkMap[VK_F3] = GLUT_KEY_F3;
    _vkMap[VK_F4] = GLUT_KEY_F4;
    _vkMap[VK_F5] = GLUT_KEY_F5;
    _vkMap[VK_F6] = GLUT_KEY_F6;
    _vkMap[VK_F7] = GLUT_KEY_F7;
    _vkMap[VK_F8] = GLUT_KEY_F8;
    _vkMap[VK_F9] = GLUT_KEY_F9;
    _vkMap[VK_F10] = GLUT_KEY_F10;
    _vkMap[VK_F11] = GLUT_KEY_F11;
    _vkMap[VK_F12] = GLUT_KEY_F12;
    /* _vkMap[VK_NUMLOCK] = GLUT_KEY_NUM_LOCK; */
    _vkMap[VK_RSHIFT] = GLUT_KEY_SHIFT_R;
    _vkMap[VK_LSHIFT] = GLUT_KEY_SHIFT_L;
    _vkMap[VK_RCONTROL] = GLUT_KEY_CTRL_R;
    _vkMap[VK_LCONTROL] = GLUT_KEY_CTRL_L;
    _vkMap[VK_RMENU] = GLUT_KEY_ALT_R;
    _vkMap[VK_LMENU] = GLUT_KEY_ALT_L;
}
static WPARAM _TranslateKeyWParam(WPARAM wParam, LPARAM lParam) {
    switch (wParam) {
    case VK_CONTROL:
        return (lParam & EXTENDED_KEYMASK) ? VK_RCONTROL : VK_LCONTROL;
    case VK_MENU:
        return (lParam & EXTENDED_KEYMASK) ? VK_RMENU : VK_LMENU;
    case VK_SHIFT:
        // There's basically no sane robust way to differentiate the Left
        // and Right Shift in Windows. (short of using DirectInput)
        return VK_LSHIFT;
    default:
        return wParam;
    }
}

static LRESULT WINAPI _glutWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {
    case WM_CREATE:
        break;

    case WM_SIZE: {
        _glutWindowWidth = LOWORD( lParam );
        _glutWindowHeight = HIWORD( lParam );
        InvalidateRect(_glutHWND, NULL, FALSE);
        if (_callback_glutReshapeFunc) {
            _callback_glutReshapeFunc(_glutWindowWidth, _glutWindowHeight);
        }
        // fallthrough
    }

    case WM_PAINT: {
        if (_callback_glutDisplayFunc)
            _callback_glutDisplayFunc();
        if (_glutHWND)
            ValidateRect(_glutHWND, NULL);
        return TRUE;
    }

    case WM_DESTROY: {
        PostQuitMessage(0);
        return TRUE;
    }

    case WM_SETCURSOR: {
        if (LOWORD(lParam) == 1) { // mouse over client area ?
            SetCursor(_curArrow);
            return TRUE;
        }
        break;
    }

    case WM_SYSKEYDOWN: // for VK_MENU (Alt)
    case WM_KEYDOWN: {
        POINT      point;
        GetCursorPos(&point);
        wParam = _TranslateKeyWParam(wParam,lParam);
        _isDown[wParam] = TRUE;
        if (_vkMap[wParam]) {
            if (_callback_glutSpecialFunc) {
                _callback_glutSpecialFunc(
                    _vkMap[wParam],
                    (int)point.x,
                    (int)point.y);
            }
        }
        else {
            if (_callback_glutKeyboardFunc) {
                _callback_glutKeyboardFunc(
                    (unsigned char)wParam,
                    (int)point.x,
                    (int)point.y);
            }
        }
        return TRUE;
    }

    case WM_SYSKEYUP: // for VK_MENU (Alt)
    case WM_KEYUP: {
        POINT      point;
        GetCursorPos(&point);
        _isDown[wParam] = FALSE;
        if (_vkMap[wParam]) {
            if (_callback_glutSpecialUpFunc) {
                _callback_glutSpecialUpFunc(
                    _vkMap[wParam],
                    (int)point.x,
                    (int)point.y);
            }
        }
        else {
            if (_callback_glutKeyboardUpFunc) {
                _callback_glutKeyboardUpFunc(
                    (unsigned char)wParam,
                    (int)point.x,
                    (int)point.y);
            }
        }
        return TRUE;
    }

    case WM_MOUSEMOVE: {
        if (_mouseMovePrevLParam != lParam) {
            int x = ((int)(short)LOWORD(lParam));
            int y = ((int)(short)HIWORD(lParam));
            if (_mouseButton) {
                if (_callback_glutMotionFunc) {
                    _callback_glutMotionFunc(x,y);
                }
            }
            else {
                if (_callback_glutPassiveMotionFunc) {
                    _callback_glutPassiveMotionFunc(x,y);
                }
            }
            _mouseMovePrevLParam = lParam;
        }
        break;
    }

    case WM_LBUTTONDOWN: {
        int x = ((int)(short)LOWORD(lParam));
        int y = ((int)(short)HIWORD(lParam));
        _mouseButton |= (1<<GLUT_LEFT_BUTTON);
        if (_callback_glutMouseFunc) {
            _callback_glutMouseFunc(GLUT_LEFT_BUTTON,GLUT_DOWN,x,y);
        }
        break;
    }
    case WM_LBUTTONUP: {
        int x = ((int)(short)LOWORD(lParam));
        int y = ((int)(short)HIWORD(lParam));
        _mouseButton &= ~(1<<GLUT_LEFT_BUTTON);
        if (_callback_glutMouseFunc) {
            _callback_glutMouseFunc(GLUT_LEFT_BUTTON,GLUT_UP,x,y);
        }
        break;
    }

    case WM_MBUTTONDOWN: {
        int x = ((int)(short)LOWORD(lParam));
        int y = ((int)(short)HIWORD(lParam));
        _mouseButton |= (1<<GLUT_MIDDLE_BUTTON);
        if (_callback_glutMouseFunc) {
            _callback_glutMouseFunc(GLUT_MIDDLE_BUTTON,GLUT_DOWN,x,y);
        }
        break;
    }
    case WM_MBUTTONUP: {
        int x = ((int)(short)LOWORD(lParam));
        int y = ((int)(short)HIWORD(lParam));
        _mouseButton &= (1<<GLUT_MIDDLE_BUTTON);
        if (_callback_glutMouseFunc) {
            _callback_glutMouseFunc(GLUT_MIDDLE_BUTTON,GLUT_UP,x,y);
        }
        break;
    }

    case WM_RBUTTONDOWN: {
        int x = ((int)(short)LOWORD(lParam));
        int y = ((int)(short)HIWORD(lParam));
        _mouseButton |= (1<<GLUT_RIGHT_BUTTON);
        if (_callback_glutMouseFunc) {
            _callback_glutMouseFunc(GLUT_RIGHT_BUTTON,GLUT_DOWN,x,y);
        }
        break;
    }
    case WM_RBUTTONUP: {
        int x = ((int)(short)LOWORD(lParam));
        int y = ((int)(short)HIWORD(lParam));
        _mouseButton &= (1<<GLUT_RIGHT_BUTTON);
        if (_callback_glutMouseFunc) {
            _callback_glutMouseFunc(GLUT_RIGHT_BUTTON,GLUT_UP,x,y);
        }
        break;
    }


    default:
        break;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

GLboolean _glutOSWinCreate(const char* title)
{
    WNDCLASS wndclass = {0};
    DWORD    wStyle   = 0;
    RECT     windowRect;
    HINSTANCE hInstance = GetModuleHandle(NULL);

	TCHAR* windowTitle = NULL;
	TCHAR* titleBuffer = NULL;
	int bufferLength = 0;
#ifdef UNICODE
	bufferLength = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, title, -1, NULL, 0);
	if(bufferLength > 0) {
		titleBuffer = (WCHAR*)malloc(sizeof(WCHAR) * bufferLength);
		bufferLength = MultiByteToWideChar(CP_ACP, 0, title, -1, titleBuffer, bufferLength);
		if(bufferLength > 0) {
			windowTitle = titleBuffer;
		}
	}
#else
	windowTitle = title;
#endif

    _InitOSWinResources();

    wndclass.style         = CS_OWNDC;
    wndclass.lpfnWndProc   = (WNDPROC)_glutWindowProc;
    wndclass.hInstance     = hInstance;
    wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wndclass.lpszClassName = TEXT("esGLUT");
    if (!RegisterClass(&wndclass))
        return FALSE;

    wStyle = WS_VISIBLE | WS_POPUP | WS_BORDER | WS_SYSMENU | WS_CAPTION | WS_SIZEBOX;

    // Adjust the window rectangle so that the client area has
    // the correct number of pixels
    windowRect.left = 0;
    windowRect.top = 0;
    windowRect.right = _glutWindowWidth;
    windowRect.bottom = _glutWindowHeight;

    AdjustWindowRect(&windowRect, wStyle, FALSE);

    _glutHWND = CreateWindow(
        TEXT("esGLUT"),
        windowTitle,
        wStyle,
        0,
        0,
        windowRect.right - windowRect.left,
        windowRect.bottom - windowRect.top,
        NULL,
        NULL,
        hInstance,
        NULL);

	if(titleBuffer) {
		free(titleBuffer);
	}

    if (_glutHWND == NULL)
        return GL_FALSE;

    ShowWindow(_glutHWND, TRUE);
    return GL_TRUE;
}

void _glutOSWinLoop()
{
    MSG msg = { 0 };
    int done = 0;
    while (!done) {
        int gotMsg = (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0);
        if (gotMsg) {
            if (msg.message == WM_QUIT) {
                done = 1;
            }
            else {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else if (_glutHWND) {
            SendMessage(_glutHWND, WM_PAINT, 0, 0);
        }

        // Call update function if registered
        if (_callback_glutIdleFunc)
            _callback_glutIdleFunc();
    }
}

#endif // #ifdef ESGLUT_OS_WIN32
