#ifndef TEXTURE_H_INCLUDED
#define TEXTURE_H_INCLUDED

#include <SDL/SDL.h>
#include <GL/gl.h>
#include <vector>

#include "render.h"

namespace MyOGL{

    class CTexture{
            GLuint TextureID;
            SDL_Surface *m_data;
            // texture parameters
            GLuint minFilter;
            GLuint magFilter;
            bool textureAlpha;
            char m_file_name[100];
        public:
            CTexture(){
                TextureID = 0;
                m_data=NULL;
                minFilter=GL_LINEAR; magFilter=GL_LINEAR;
                textureAlpha=false;
            }
            ~CTexture();
            void Free();    // clear video memory and memory
            void Bind();
            bool LoadFromFile(const char *file_name);
            bool CreateFromMemory(void);
            int GetWidth(){ if (m_data){return m_data->w;}else{return 0;}};
            int GetHeight(){ if(m_data){return m_data->h;}else{ return 0;}};
            bool IsAlpha(){ return textureAlpha; }
            GLuint GetID(){ return TextureID; }
            char *GetFileName(){ return m_file_name;}
    };

    extern std::vector<CTexture*>  TexturesList;

}

#endif // TEXTURE_H_INCLUDED
