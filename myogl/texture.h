#ifndef TEXTURE_H_INCLUDED
#define TEXTURE_H_INCLUDED

#include <SDL/SDL.h>
#include <GL/gl.h>
#include <vector>

#include "render.h"

namespace MyOGL{

    class CTexture{
            GLuint TextureID;

        #ifdef USE_SDL_BMP_LOADER
            SDL_Surface *m_data;
        #else
            unsigned char *m_data;
        #endif
            // texture parameters
            GLuint minFilter;
            GLuint magFilter;
            unsigned long int m_width, m_height; // Texture width & height
            GLuint m_bytes_pp;   // Texture butes per pixel
            GLuint m_texture_format; // GL_RGBA, etc...
            bool m_alpha;
            char m_file_name[100];
        public:
            CTexture(){
                TextureID = 0;
                m_data=NULL;
                minFilter=GL_LINEAR; magFilter=GL_LINEAR;
                m_alpha=false;
            }
            ~CTexture();
            void Free();    // clear video memory and memory
            void Bind();
            bool LoadFromFile(const char *file_name);
            unsigned char* LoadBitmap32(const char *file_name);
            bool CreateFromMemory(void);
            int GetWidth(){ if (m_data){return m_width;}else{return 0;}};
            int GetHeight(){ if(m_data){return m_height;}else{ return 0;}};
            bool IsAlpha(){ return m_alpha; }
            GLuint GetID(){ return TextureID; }
            char *GetFileName(){ return m_file_name;}
    };

    extern std::vector<CTexture*>  TexturesList;

}

#endif // TEXTURE_H_INCLUDED
