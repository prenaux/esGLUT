#ifndef __ESGLUT_INTERNAL_H_C67988E2_76C1_4A7F_BB62_F87B1E5BF3E7__
#define __ESGLUT_INTERNAL_H_C67988E2_76C1_4A7F_BB62_F87B1E5BF3E7__

#if !defined ESGLUT_OS_WEB

#include "esGLUT/EGL/egl.h"

#ifdef _MSC_VER
#define inline __inline
#endif

ESGLUT_EXTERN void (* _callback_glutKeyboardFunc)( unsigned char, int, int );
ESGLUT_EXTERN void (* _callback_glutKeyboardUpFunc)( unsigned char, int, int );
ESGLUT_EXTERN void (* _callback_glutSpecialFunc)( int, int, int );
ESGLUT_EXTERN void (* _callback_glutSpecialUpFunc)( int, int, int );
ESGLUT_EXTERN void (* _callback_glutReshapeFunc)( int, int );
ESGLUT_EXTERN void (* _callback_glutDisplayFunc)( void );
ESGLUT_EXTERN void (* _callback_glutMouseFunc)( int, int, int, int );
ESGLUT_EXTERN void (* _callback_glutMouseWheelFunc)( int, int, int );
ESGLUT_EXTERN void (* _callback_glutMotionFunc)( int, int );
ESGLUT_EXTERN void (* _callback_glutPassiveMotionFunc)( int, int );
ESGLUT_EXTERN void (* _callback_glutIdleFunc)( void );

ESGLUT_EXTERN GLint        _glutWindowWidth;
ESGLUT_EXTERN GLint        _glutWindowHeight;
ESGLUT_EXTERN unsigned int _glutDisplayMode;
ESGLUT_EXTERN EGLNativeWindowType _glutHWND;
ESGLUT_EXTERN EGLDisplay  _glutEGLDisplay;
ESGLUT_EXTERN EGLContext  _glutEGLContext;
ESGLUT_EXTERN EGLSurface  _glutEGLSurface;

ESGLUT_EXTERN GLboolean _glutOSWinCreate(const char* title);
ESGLUT_EXTERN void _glutOSWinLoop();

#endif

#endif // __ESGLUT_INTERNAL_H_C67988E2_76C1_4A7F_BB62_F87B1E5BF3E7__
