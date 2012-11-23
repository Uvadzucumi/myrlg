#ifndef RENDER_H_INCLUDED
#define RENDER_H_INCLUDED

#include <stdio.h>
#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "config.h"
#include "vector_types.h"
#include "log.h"

#ifdef __WIN32__
    #define GL_MAX_TEXTURE_UNITS                    0x84E2
    #define GL_BGR                                  0x80E0
    #define GL_BGRA                                 0x80E1
#endif

namespace MyOGL{

// Transparent Types
    enum MyGlBlendMode{
        blNone=0,
        blSource,
        blConstColor,
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
            int width, height;
            char *title;
            bool full_screen;
        public:
            CWindow(int width, int height, const char *title);
    };


    class CRender{
            int width, height, bpp;
            bool full_screen;
            SDL_Surface *Context;
            Uint32 window_flags;
            char *gl_vendor;
            char *gl_version;
            char *gl_extensions;
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
            int GetWidth(void){ return width;}
            int GetHeight(void){ return width;}
        // resize window
            bool OnResize(int width, int height);
        // Textures
            void BindTexture(GLuint TextureID, bool force=false);
            void SetBlendMode(MyGlBlendMode mode);
        // DrawColor
            void SetColor(float r, float g, float b, float a=1.0, bool force=false);
            void SetColor(Vector3i color, bool force=false);
            void SetClearColor(float r, float g, float b, float a=1.0);
    };

    extern CRender *Render;
}

#endif // RENDER_H_INCLUDED
