#ifndef TEXTURE_H_INCLUDED
#define TEXTURE_H_INCLUDED

#include <SDL/SDL.h>
#include <GL/gl.h>
#include <vector>

#include "render.h"

namespace MyOGL{

    class CTexture{
            GLuint TextureID;
            SDL_Surface *data;
            // texture parameters
            GLuint minFilter;
            GLuint magFilter;
            bool textureAlpha;
        public:
            CTexture(){
                TextureID = 0;
                data=NULL;
                minFilter=GL_LINEAR; magFilter=GL_LINEAR;
                textureAlpha=false;
            }
            ~CTexture();
            void Free();    // clear video memory and memory
            void Bind();
            bool LoadFromFile(const char *file_name);
            int GetWidth(){ if (data){return data->w;}else{return 0;}};
            int GetHeight(){ if(data){return data->h;}else{ return 0;}};
            bool IsAlpha(){ return textureAlpha; }
            GLuint GetID(){ return TextureID; }
    };

    extern std::vector<CTexture*>  TexturesList;

}

#endif // TEXTURE_H_INCLUDED
