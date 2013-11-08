#include "esGLUT/esGLUT.h"

#ifndef ESGLUT_OS_WEB

#include "esGLUT_internal.h"

#ifdef _MSC_VER
// warning C4204: nonstandard extension used : non-constant aggregate initializer
#pragma warning(disable : 4204)
#endif

//---------------------------------------------------------------------------
//
// Event Function Pointers
//
//---------------------------------------------------------------------------
void (* _callback_glutKeyboardFunc)( unsigned char, int, int ) = NULL;
void glutKeyboardFunc(void (*callback)( unsigned char, int, int )) {
    _callback_glutKeyboardFunc = callback;
}

void (* _callback_glutKeyboardUpFunc)( unsigned char, int, int ) = NULL;
void glutKeyboardUpFunc(void (*callback)( unsigned char, int, int )) {
    _callback_glutKeyboardUpFunc = callback;
}

void (* _callback_glutSpecialFunc)( int, int, int ) = NULL;
void glutSpecialFunc(void (*callback)( int, int, int )) {
    _callback_glutSpecialFunc = callback;
}

void (* _callback_glutSpecialUpFunc)( int, int, int ) = NULL;
void glutSpecialUpFunc(void (*callback)( int, int, int )) {
    _callback_glutSpecialUpFunc = callback;
}

void (* _callback_glutReshapeFunc)( int, int ) = NULL;
void glutReshapeFunc(void (*callback)( int, int )) {
    _callback_glutReshapeFunc = callback;
}

void (* _callback_glutDisplayFunc)( void ) = NULL;
void glutDisplayFunc(void (*callback)( void )) {
    _callback_glutDisplayFunc = callback;
}

void (* _callback_glutMouseFunc)( int, int, int, int ) = NULL;
void glutMouseFunc(void (*callback)( int, int, int, int )) {
    _callback_glutMouseFunc = callback;
}

void (* _callback_glutMouseWheelFunc)( int, int, int ) = NULL;
void glutMouseWheelFunc(void (*callback)( int, int, int )) {
    _callback_glutMouseWheelFunc = callback;
}

void (* _callback_glutMotionFunc)( int, int ) = NULL;
void glutMotionFunc(void (*callback)( int, int )) {
    _callback_glutMotionFunc = callback;
}

void (* _callback_glutPassiveMotionFunc)( int, int ) = NULL;
void glutPassiveMotionFunc(void (*callback)( int, int )) {
    _callback_glutPassiveMotionFunc = callback;
}

void (* _callback_glutIdleFunc)( void ) = NULL;
void glutIdleFunc(void (*callback)( void )) {
    _callback_glutIdleFunc = callback;
}

//---------------------------------------------------------------------------
//
// Initialize
//
//---------------------------------------------------------------------------
int _glutWindowWidth = 0;
int _glutWindowHeight = 0;
unsigned int _glutDisplayMode = 0;
EGLNativeWindowType _glutHWND = NULL;
EGLDisplay  _glutEGLDisplay = NULL;
EGLContext  _glutEGLContext = NULL;
EGLSurface  _glutEGLSurface = NULL;

static EGLBoolean _CreateEGLContext(
    EGLNativeWindowType hWnd, EGLDisplay* eglDisplay,
    EGLContext* eglContext, EGLSurface* eglSurface,
    EGLint* configAttribList, EGLint* surfaceAttribList)
{
   EGLint numConfigs;
   EGLint majorVersion;
   EGLint minorVersion;
   EGLDisplay display;
   EGLContext context;
   EGLSurface surface;
   EGLConfig config;
   EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE, EGL_NONE };

   // Get Display
   display = eglGetDisplay(GetDC(hWnd));
   if (display == EGL_NO_DISPLAY) {
      return EGL_FALSE;
   }

   // Initialize EGL
   if (!eglInitialize(display, &majorVersion, &minorVersion)) {
      return EGL_FALSE;
   }

   // Get configs
   if (!eglGetConfigs(display, NULL, 0, &numConfigs)) {
      return EGL_FALSE;
   }

   // Choose config
   if (!eglChooseConfig(display, configAttribList, &config, 1, &numConfigs)) {
      return EGL_FALSE;
   }

   // Create a surface
   surface = eglCreateWindowSurface(display, config, (EGLNativeWindowType)hWnd, surfaceAttribList);
   if (surface == EGL_NO_SURFACE) {
      return EGL_FALSE;
   }

   // Create a GL context
   context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs );
   if (context == EGL_NO_CONTEXT) {
      return EGL_FALSE;
   }

   // Make the context current
   if (!eglMakeCurrent(display, surface, surface, context)) {
      return EGL_FALSE;
   }

   *eglDisplay = display;
   *eglSurface = surface;
   *eglContext = context;
   return EGL_TRUE;
}

void ESGLUT_API glutInit(int* pargc, char** argv) {
    _glutWindowWidth = 300;
    _glutWindowHeight = 300;
    _glutDisplayMode = GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH;
    _glutHWND = NULL;
    _glutEGLDisplay = NULL;
    _glutEGLContext = NULL;
    _glutEGLSurface = NULL;
}

void ESGLUT_API glutInitWindowSize(int width, int height) {
    _glutWindowWidth = width;
    _glutWindowHeight = height;
}

void ESGLUT_API glutInitDisplayMode(unsigned int displayMode) {
    _glutDisplayMode = displayMode;
}

int ESGLUT_API glutCreateWindow(const char* title ) {
    const unsigned long flags = _glutDisplayMode;
    EGLint configAttribList[] = {
        EGL_RED_SIZE,       (flags & GLUT_RGBA) ? 8 : 5,
        EGL_GREEN_SIZE,     (flags & GLUT_RGBA) ? 8 : 6,
        EGL_BLUE_SIZE,      (flags & GLUT_RGBA) ? 8 : 5,
        EGL_ALPHA_SIZE,     (flags & GLUT_RGBA) ? 8 : EGL_DONT_CARE,
        EGL_DEPTH_SIZE,     (flags & GLUT_DEPTH) ? 24 : EGL_DONT_CARE,
        EGL_STENCIL_SIZE,   (flags & GLUT_STENCIL) ? 8 : EGL_DONT_CARE,
        EGL_SAMPLE_BUFFERS, (flags & GLUT_MULTISAMPLE) ? 1 : 0,
        EGL_NONE
    };
    EGLint surfaceAttribList[] = {
        EGL_NONE, EGL_NONE
    };

    if (!_glutOSWinCreate(title)) {
        return GL_FALSE;
    }

    if (!_CreateEGLContext(
            _glutHWND,
            &_glutEGLDisplay,
            &_glutEGLContext,
            &_glutEGLSurface,
            configAttribList,
            surfaceAttribList))
    {
        return GL_FALSE;
    }

    return GL_TRUE;
}

//---------------------------------------------------------------------------
//
// MainLoop
//
//---------------------------------------------------------------------------
void ESGLUT_API glutPostRedisplay(void) {
}

void ESGLUT_API glutSwapBuffers(void) {
    eglSwapBuffers(_glutEGLDisplay, _glutEGLSurface);
}

void ESGLUT_API glutMainLoop(void) {
    if (_callback_glutReshapeFunc) {
        _callback_glutReshapeFunc(_glutWindowWidth,_glutWindowHeight);
    }
    _glutOSWinLoop();
}

void ESGLUT_API glutFullScreen() {
    /* TODO */
    glutLogMessage("W/glutFullScreen not implemented !");
}

#endif // #ifndef ESGLUT_OS_WEB
