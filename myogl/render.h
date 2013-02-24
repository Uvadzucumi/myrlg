#ifndef RENDER_H_INCLUDED
#define RENDER_H_INCLUDED

#include <stdio.h>
#include <SDL/SDL.h>

//#include <GL/gl.h>
//#include <GL/glu.h>
//#include <GL/glext.h>

#ifdef __ANDROID_API__
    #define GL_GLEXT_PROTOTYPES
    #include <GLES2/gl2.h>
    #include <GLES2/gl2ext.h>
#else
    #include <SDL/SDL_opengl.h>
#endif

#include "config.h"
#include "vector_types.h"
#include "log.h"

#ifdef __WIN32__
    #define GL_MAX_TEXTURE_UNITS                    0x84E2
    #define GL_BGR                                  0x80E0
    #define GL_BGRA                                 0x80E1
#endif

//typedef void (*GLfunction)();
//extern GLfunction glXGetProcAddressARB(const GLubyte *procName);
//void (*glXGetProcAddressARB(const GLubyte *procName))();


// FBO functions
extern PFNGLGENFRAMEBUFFERSEXTPROC         glGenFramebuffersEXT;
extern PFNGLGENRENDERBUFFERSEXTPROC        glGenRenderbuffersEXT;
extern PFNGLBINDFRAMEBUFFEREXTPROC         glBindFramebufferEXT;
extern PFNGLBINDRENDERBUFFEREXTPROC        glBindRenderbufferEXT;
extern PFNGLRENDERBUFFERSTORAGEEXTPROC     glRenderbufferStorageEXT;
extern PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC glFramebufferRenderbufferEXT;
extern PFNGLFRAMEBUFFERTEXTURE2DEXTPROC    glFramebufferTexture2DEXT;
extern PFNGLGENERATEMIPMAPEXTPROC          glGenerateMipmapEXT;
extern PFNGLDELETERENDERBUFFERSEXTPROC     glDeleteRenderbuffersEXT;
extern PFNGLDELETEFRAMEBUFFERSEXTPROC      glDeleteFramebuffersEXT;
extern PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC  glCheckFramebufferStatusEXT;

// VBO functions (GL_ARB_vertex_buffer_object)
extern PFNGLGENBUFFERSARBPROC       glGenBuffersARB;
extern PFNGLBINDBUFFERARBPROC       glBindBufferARB;
extern PFNGLMAPBUFFERARBPROC        glMapBufferARB;
extern PFNGLUNMAPBUFFERARBPROC      glUnmapBufferARB;
extern PFNGLBUFFERDATAARBPROC       glBufferDataARB;
extern PFNGLBUFFERSUBDATAARBPROC    glBufferSubDataARB;
extern PFNGLDELETEBUFFERSARBPROC    glDeleteBuffersARB;
extern PFNGLGETBUFFERSUBDATAARBPROC glGetBufferSubDataARB;

// GL_ARB_shading_language_100, GL_ARB_shader_objects, GL_ARB_fragment_shader, GL_ARB_vertex_shader
extern PFNGLCREATEPROGRAMOBJECTARBPROC  glCreateProgramObjectARB;
extern PFNGLDELETEOBJECTARBPROC         glDeleteObjectARB;
extern PFNGLUSEPROGRAMOBJECTARBPROC     glUseProgramObjectARB;
extern PFNGLCREATESHADEROBJECTARBPROC   glCreateShaderObjectARB;
extern PFNGLSHADERSOURCEARBPROC         glShaderSourceARB;
extern PFNGLCOMPILESHADERARBPROC        glCompileShaderARB;
extern PFNGLGETOBJECTPARAMETERIVARBPROC glGetObjectParameterivARB;
extern PFNGLATTACHOBJECTARBPROC         glAttachObjectARB;
extern PFNGLGETINFOLOGARBPROC           glGetInfoLogARB;
extern PFNGLLINKPROGRAMARBPROC          glLinkProgramARB;
extern PFNGLGETUNIFORMLOCATIONARBPROC   glGetUniformLocationARB;
extern PFNGLUNIFORM1FARBPROC            glUniform1fARB;
extern PFNGLUNIFORM2FARBPROC            glUniform2fARB;
extern PFNGLUNIFORM3FARBPROC            glUniform3fARB;
extern PFNGLUNIFORM4FARBPROC            glUniform4fARB;
extern PFNGLUNIFORM1FVARBPROC           glUniform1fvARB;
extern PFNGLUNIFORM2FVARBPROC           glUniform2fvARB;
extern PFNGLUNIFORM3FVARBPROC           glUniform3fvARB;
extern PFNGLUNIFORM4FVARBPROC           glUniform4fvARB;
extern PFNGLUNIFORM1IARBPROC            glUniform1iARB;
extern PFNGLBINDATTRIBLOCATIONARBPROC   glBindAttribLocationARB;
extern PFNGLGETACTIVEUNIFORMARBPROC     glGetActiveUniformARB;
extern PFNGLGETSHADERIVPROC             glGetShaderiv;
extern PFNGLGETPROGRAMIVPROC            glGetProgramiv;

namespace MyOGL{

// Transparent Types
    enum MyGlBlendMode{
        blNone=0,
        blSource,
        blConstColor,
        blAdditive,
        blCount
    };


    extern struct RenderStates{
        protected:
            int MaxTextureUnits;
// get Opengl cache state pointer
            bool *GetGLParam(GLenum cap);
        public:
            bool mode3d;                // true - 3D projection, false - glOrtho
            GLuint CurrentTexure;
            MyGlBlendMode CurrentBlendMode;    // blNone, blSource
            Vector4f Color;
            Vector4f ClearColor;
            //Vector4f BlendColor;
            bool ColorMaterial;         // enabled color material mode
            bool Texture2D;
            bool DepthTest;
            bool Lighting;
            bool Blend;
// methods
            void Enable(GLenum cap);
            void Disable(GLenum cap);
            void GetCurrentStates();
            void Debug(void);
    } GL;

    // window class
    class CWindow{
            int m_width, m_height;
            char *title;
            bool full_screen;
        public:
            CWindow(int width, int height, const char *title);
    };


    class CRender{
            int m_width, m_height, m_bpp;
            bool m_full_screen;
            SDL_Surface *Context;
            Uint32 window_flags;
            char *gl_vendor;
            char *gl_version;
            char *gl_extensions;
            bool m_fbo;
            bool m_vbo;
            bool m_shaders;
       //     CWindow *win;
            float zNearPlane, zFarPlane;
            void InitGL();  // set start OpenGL states, save states to GL object
        public:
            CRender(); //{width=0; height=0; bpp=0; full_screen=false;Context=NULL;};
            ~CRender();
        // create window, inicialize render context
            bool Init(int width, int height, int bpp=32, bool full_screen=false, const char *title=NULL);
            void Free(void);    // deinicialize data
        // execute application
            void Run(void);
        // set 2D/3D Projection
            void Set2D(bool force=false);
            void Set3D(bool force=false);
        // swap render buffers (if needed)
            void SwapBuffers();
            void SetWinCaption(const char *title);
            bool SetWinIcon(const char *file_name);
        // get context size
            int GetWidth(void){ return m_width;}
            int GetHeight(void){ return m_height;}
        // resize window
            bool OnResize(int width, int height);
        // Textures
            void BindTexture(GLuint TextureID, bool force=false);
            void SetBlendMode(MyGlBlendMode mode);
        // DrawColor
            void SetColor(float r, float g, float b, float a=1.0, bool force=false);
            void SetColor(Vector4f color, bool force=false);
            void SetColor(Vector3i color, bool force=false);

            void SetClearColor(float r, float g, float b, float a=1.0);
            // check OpenGL Extension
            bool isExtensionSupported ( const char * ext );
            static void  * GetProcAddress ( const char * name ){
/*
#ifdef	__WIN32__
                return wglGetProcAddress ( name );
#else
                return (void *)glXGetProcAddressARB ( (const GLubyte *)name );
#endif
*/
                return (void *)SDL_GL_GetProcAddress( name );
            };
            bool EnableFBOFunctions(); // Enable FBO Extension functions
            bool EnableVBOFunctions();  // Enable VBO Extension functions
            bool EnableShadersFunctions(); // Enable Shader Extension functions
    };

    extern CRender *Render;
}

#endif // RENDER_H_INCLUDED
