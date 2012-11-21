
#include "texture.h"
#include "log.h"

using namespace MyOGL;

std::vector<CTexture*> MyOGL::TexturesList;
//CRender *MyOGL::Render=NULL;
void CTexture::Free(){
    // clear video memory
    if(TextureID){
        Log->puts("unload texture %d\n",TextureID);
        glDeleteTextures(1,&TextureID);
        TextureID=0;
    }
    // clear memory
    if(m_data!=NULL){
        SDL_FreeSurface(m_data);
        m_data=NULL;
    }
}

CTexture::~CTexture(){
    Free();
    // clear texture from texture list
    for(unsigned int i=0;i<TexturesList.size();i++){
        if(TexturesList[i]==this){
            TexturesList.erase(TexturesList.begin()+i);
            break;
        }
    }
    Log->puts("Deleted texture object\n");
}

void CTexture::Bind(){
    if(!TextureID){ Log->puts("Error CTexture::Bind() - zerro texture number!\n");}
    Render->BindTexture(TextureID);
}

// create texture in video memory
bool CTexture::CreateFromMemory(void){
    GLuint texture_format;
    GLuint nOfColors;   // Texture butes per pixel

    if(!m_data){
        Log->puts("CTexture::CreateFromMemory() Error: texture data = NULL\n");
        return false;
    }
	// Check that the image's width is a power of 2
    if ( (m_data->w & (m_data->w - 1)) != 0 ) {
        Log->puts("warning: %s's width is not a power of 2\n", m_file_name);
    }

	// Also check if the height is a power of 2
    if ( (m_data->h & (m_data->h - 1)) != 0 ) {
        Log->puts("warning: %s's height is not a power of 2\n", m_file_name);
    }

    // get the number of channels in the SDL surface
    nOfColors = m_data->format->BytesPerPixel;

    if (nOfColors == 4){     // contains an alpha channel
        textureAlpha=true;
        if (m_data->format->Rmask == 0x000000ff)
            texture_format = GL_RGBA;
        else
            texture_format = GL_BGRA;
        Log->puts("Found 4 Color in texture. Format: %s\n",(texture_format==GL_RGBA)?"RGBA":"BGRA");
    } else if (nOfColors == 3) {    // no alpha channel
                textureAlpha=false;
                if (m_data->format->Rmask == 0x000000ff)
                        texture_format = GL_RGB;
                else
                        texture_format = GL_BGR;
                Log->puts("Found 3 Color in texture. Format: %s\n",(texture_format==GL_RGB)?"RGB":"BGR");
    } else {
                Log->puts("warning: the image %s is not truecolor..  this will probably break\n",m_file_name);
                // this error should not go unhandled
                return false;
    }

	// Have OpenGL generate a texture object handle for us
    glGenTextures( 1, &TextureID );
    Log->puts("Gen texture ID=%d\n",TextureID);

	// force bind the texture object
    Render->BindTexture(TextureID,true);

	// Set the texture's stretching properties
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->minFilter );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->magFilter );

	// Edit the texture object's image data using the information SDL_Surface gives us
    glTexImage2D( GL_TEXTURE_2D, 0, nOfColors, m_data->w, m_data->h, 0,
                      texture_format, GL_UNSIGNED_BYTE, m_data->pixels );
    return true;
}

// load image from  file filename,
// save in data SDL surface
// create texture in video memory
bool CTexture::LoadFromFile(const char *file_name){
    strcpy(m_file_name,file_name);
    if ( (m_data = SDL_LoadBMP(file_name)) ) {

        CreateFromMemory();

    } else {
        Log->puts("SDL could not load image.bmp: %s\n", SDL_GetError());
        return false;
    }
    Log->puts("Created Texture %d (%s)\n",TextureID, file_name);
    TexturesList.push_back(this);   // add to global texture ids list (for autoclean)
    return true;
}
