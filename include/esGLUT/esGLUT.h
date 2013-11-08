#ifndef __ESGLUT_H_BEB040E8_250A_4206_A33B_306672C9B0EA__
#define __ESGLUT_H_BEB040E8_250A_4206_A33B_306672C9B0EA__

#include "GLES2/gl2.h"

#if defined __JSCC_WEB__
#define ESGLUT_OS_WEB
#elif defined __WIN32__ || defined _MSC_VER
#define ESGLUT_OS_WIN32
#elif defined(__APPLE_CC__) && defined(__APPLE__) &&                \
    defined(__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__) &&       \
    !defined __IOS__ && !defined __IPHONE_OS_VERSION_MIN_REQUIRED
#define ESGLUT_OS_OSX
#elif defined __linux__
#define ESGLUT_OS_LINUX
#else
#error "Unknown Platform !"
#endif

#ifdef __cplusplus
#define ESGLUT_EXTERN extern "C"
#else
#define ESGLUT_EXTERN extern
#endif
#define ESGLUT_CALLBACK __cdecl
#define ESGLUT_API __cdecl

/*
 * GLUT API macro definitions -- the special key codes:
 */
#define GLUT_KEY_F1                 0x0001
#define GLUT_KEY_F2                 0x0002
#define GLUT_KEY_F3                 0x0003
#define GLUT_KEY_F4                 0x0004
#define GLUT_KEY_F5                 0x0005
#define GLUT_KEY_F6                 0x0006
#define GLUT_KEY_F7                 0x0007
#define GLUT_KEY_F8                 0x0008
#define GLUT_KEY_F9                 0x0009
#define GLUT_KEY_F10                0x000A
#define GLUT_KEY_F11                0x000B
#define GLUT_KEY_F12                0x000C
#define GLUT_KEY_LEFT               0x0064
#define GLUT_KEY_UP                 0x0065
#define GLUT_KEY_RIGHT              0x0066
#define GLUT_KEY_DOWN               0x0067
#define GLUT_KEY_PAGE_UP            0x0068
#define GLUT_KEY_PAGE_DOWN          0x0069
#define GLUT_KEY_HOME               0x006A
#define GLUT_KEY_END                0x006B
#define GLUT_KEY_INSERT             0x006C
/* #define GLUT_KEY_NUM_LOCK           0x006D */
/* #define GLUT_KEY_BEGIN              0x006E */
#define GLUT_KEY_DELETE             0x006F
#define GLUT_KEY_SHIFT_L            0x0070
#define GLUT_KEY_SHIFT_R            0x0071
#define GLUT_KEY_CTRL_L             0x0072
#define GLUT_KEY_CTRL_R             0x0073
#define GLUT_KEY_ALT_L              0x0074
#define GLUT_KEY_ALT_R              0x0075

/*
 * GLUT API macro definitions -- mouse state definitions
 */
#define  GLUT_LEFT_BUTTON           0x0000
#define  GLUT_MIDDLE_BUTTON         0x0001
#define  GLUT_RIGHT_BUTTON          0x0002
#define  GLUT_DOWN                  0x0000
#define  GLUT_UP                    0x0001
/* #define  GLUT_LEFT                  0x0000 */
/* #define  GLUT_ENTERED               0x0001 */

/*
 * GLUT API macro definitions -- the display mode definitions
 */
/// Double buffer, always on in ES
#define GLUT_DOUBLE        0x0002
/// RGB color buffer (initialize R5G6B5 back buffer)
#define GLUT_RGB           0x0000
/// ALPHA color buffer (initialize R8G8B8A8 back buffer)
#define GLUT_RGBA          0x0008
/// depth buffer
#define GLUT_DEPTH         0x0010
/// stencil buffer
#define GLUT_STENCIL       0x0020
/// multi-sample buffer
#define GLUT_MULTISAMPLE   0x0080

ESGLUT_EXTERN void ESGLUT_API glutInit( int* pargc, char** argv );
ESGLUT_EXTERN void ESGLUT_API glutInitWindowSize( int width, int height );
ESGLUT_EXTERN void ESGLUT_API glutInitDisplayMode( unsigned int displayMode );
ESGLUT_EXTERN void ESGLUT_API glutInitDisplayString( const char* displayMode );
ESGLUT_EXTERN void ESGLUT_API glutMainLoop( void );

ESGLUT_EXTERN int  ESGLUT_API glutCreateWindow( const char* title );
ESGLUT_EXTERN void ESGLUT_API glutPostRedisplay( void );
ESGLUT_EXTERN void ESGLUT_API glutSwapBuffers( void );

ESGLUT_EXTERN void ESGLUT_API glutKeyboardFunc( void (ESGLUT_CALLBACK *callback)( unsigned char, int, int ) );
ESGLUT_EXTERN void ESGLUT_API glutKeyboardUpFunc( void (ESGLUT_CALLBACK *callback)( unsigned char, int, int ) );

ESGLUT_EXTERN void ESGLUT_API glutSpecialFunc( void (ESGLUT_CALLBACK *callback)( int, int, int ) );
ESGLUT_EXTERN void ESGLUT_API glutSpecialUpFunc( void (ESGLUT_CALLBACK *callback)( int, int, int ) );

ESGLUT_EXTERN void ESGLUT_API glutReshapeFunc( void (ESGLUT_CALLBACK *callback)( int, int ) );
ESGLUT_EXTERN void ESGLUT_API glutDisplayFunc( void (ESGLUT_CALLBACK *callback)( void ) );
ESGLUT_EXTERN void ESGLUT_API glutMouseFunc( void (ESGLUT_CALLBACK *callback)( int, int, int, int ) );
ESGLUT_EXTERN void ESGLUT_API glutMouseWheelFunc( void (ESGLUT_CALLBACK *callback)( int, int, int ) );
ESGLUT_EXTERN void ESGLUT_API glutMotionFunc( void (ESGLUT_CALLBACK *callback)( int, int ) );
ESGLUT_EXTERN void ESGLUT_API glutPassiveMotionFunc( void (ESGLUT_CALLBACK *callback)( int, int ) );

ESGLUT_EXTERN void ESGLUT_API glutIdleFunc( void (ESGLUT_CALLBACK *callback)( void ) );

/*
 * GLUT API macro definitions -- the glutGet parameters
 */
#define  GLUT_ELAPSED_TIME                  0x02BC

ESGLUT_EXTERN int ESGLUT_API glutGet(GLenum query);

/*
 * Log
 */
#ifdef ESGLUT_OS_WEB
#include <stdio.h>
#define glutLogMessage(...) {                   \
        printf(__VA_ARGS__);                      \
        printf("\n");                           \
        fflush(stdout);                         \
}
#else
ESGLUT_EXTERN void ESGLUT_API glutLogMessage(const char *formatStr, ...);
#endif

ESGLUT_EXTERN void ESGLUT_API glutFullScreen();

#endif // __ESGLUT_H_BEB040E8_250A_4206_A33B_306672C9B0EA__
