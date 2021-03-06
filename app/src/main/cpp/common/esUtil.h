//
// Created by xucz on 2019-06-22.
//

#ifndef ANDROIDOPENGL3DEMO_ESUTIL_H
#define ANDROIDOPENGL3DEMO_ESUTIL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <GLES3/gl3.h>
#include <GLES2/gl2ext.h>
#include <EGL/egl.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <android/log.h>
#include <time.h>
#include <android/native_window.h>


#define MATH_PI 3.1415926535897932384626433832802


#define TEX_TYPE_OES 1
#define TEX_TYPE_2D  2

#define CAMERA_FACING_BACK  1
#define CAMERA_FACING_FRONT 2


/// esCreateWindow flag - RGB color buffer
#define ES_WINDOW_RGB           0
/// esCreateWindow flag - ALPHA color buffer
#define ES_WINDOW_ALPHA         1
/// esCreateWindow flag - depth buffer
#define ES_WINDOW_DEPTH         2
/// esCreateWindow flag - stencil buffer
#define ES_WINDOW_STENCIL       4
/// esCreateWindow flat - multi-sample buffer
#define ES_WINDOW_MULTISAMPLE   8
/// esCreateWindow flat - multi-sample buffer
#define ES_WINDOW_PBUFFER   16

#define LOG_TAG "AndroidOpenGL3Demo"

#define ALOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG, __VA_ARGS__)
#define ALOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__))
#define LOGCATE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

#define GO_CHECK_GL_ERROR(...)  \
{                               \
    int error = glGetError();   \
    if( error!= 0) LOGCATE("CHECK_GL_ERROR %s glGetError = %d, line = %d, ",  __FUNCTION__, error, __LINE__); \
}

typedef struct __ESContext {
    /// Put Image data here
    void *imageData[6];

    /// Put your user data here...
    void *userData;

    /// Window width
    GLint width;

    /// Window height
    GLint height;

    /// Display handle
    EGLNativeDisplayType eglNativeDisplay;

    /// Window handle
    EGLNativeWindowType eglNativeWindow;

    /// EGL display
    EGLDisplay eglDisplay;

    /// EGL context
    EGLContext eglContext;

    /// EGL surface
    EGLSurface eglSurface;

} ESContext;


void esLogMessage(const char *formatStr, ...);

GLboolean esCreateWindow(ESContext *esContext, ESContext *shareContext, GLuint flags, GLint width, GLint height);

GLuint
esLoadProgram2(const char *vertShaderSrc, const char *fragShaderSrc, void (*beforeLink)(GLuint));

GLuint esLoadProgram(const char *vertShaderSrc, const char *fragShaderSrc);

long esGetCurrClockTimeNs();

#ifdef __cplusplus
}
#endif
#endif //ANDROIDOPENGL3DEMO_ESUTIL_H
