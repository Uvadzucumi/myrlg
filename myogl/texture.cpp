
#include "texture.h"
#include "log.h"

#include "fileformats/bmpimage.h"

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
//        SDL_FreeSurface(m_data);
        free(m_data);
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

    if(!m_data){
        Log->puts("CTexture::CreateFromMemory() Error: texture data = NULL\n");
        return false;
    }
	// Check that the image's width is a power of 2
    if ( (m_width & (m_width - 1)) != 0 ) {
        Log->printf("warning: %s's width is not a power of 2\n", m_file_name);
    }

	// Also check if the height is a power of 2
    if ( (m_height & (m_height - 1)) != 0 ) {
        Log->printf("warning: %s's height is not a power of 2\n", m_file_name);
    }

	// Have OpenGL generate a texture object handle for us
    glGenTextures( 1, &TextureID );
    Log->puts("Gen texture ID=%d\n",TextureID);

	// force bind the texture object
    Render->BindTexture(TextureID,true);

	// Set the texture's stretching properties
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->minFilter );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->magFilter );

	// Create 2D texture
	#ifdef USE_SDL_BMP_LOADER
    glTexImage2D( GL_TEXTURE_2D, 0, m_bytes_pp, m_width, m_height, 0,
                      m_texture_format, GL_UNSIGNED_BYTE, m_data->pixels );
    #else
    glTexImage2D( GL_TEXTURE_2D, 0, m_bytes_pp, m_width, m_height, 0,
                     m_texture_format, GL_UNSIGNED_BYTE, (void *)m_data );
    #endif
    return true;
}

// load image from  file filename,
// save in data SDL surface
// create texture in video memory
bool CTexture::LoadFromFile(const char *file_name){
    strcpy(m_file_name,file_name);
#ifdef USE_SDL_BMP_LOADER
    if( ( m_data = SDL_LoadBMP(m_file_name) ) ) {
        // set width, height & bytes per pixel
        m_width=m_data->w;
        m_height=m_data->h;
        m_bytes_pp=m_data->format->BytesPerPixel;
        // set texture format
        if (m_data->format->Rmask == 0x0000ff)
            m_texture_format = GL_RGBA;
        else if( m_data->format->Rmask == 0xff0000){
            m_texture_format = GL_BGRA;
        }else{
            Log->puts("ERROR! CTexture::LoadFromFile() Wrong texture format. using default GL_RGBA\n");
            printf("BitsPerPixel: %d\nRMASK: 0x%x\nGMASK: 0x%x\nBMASK: 0x%x\nAMASK: 0x%x\n",m_data->format->BitsPerPixel, m_data->format->Rmask,m_data->format->Gmask,m_data->format->Bmask,m_data->format->Amask);
            m_texture_format=GL_RGBA;
        }
#else
   if( ( m_data = LoadBitmap32(m_file_name) ) ) {
#endif
        // create texture
        CreateFromMemory();
    } else {
        Log->printf("CTexture::LoadFromFile() could not load image %s",file_name);
        return false;
    }
    Log->printf("Created Texture %d (%s)\n",TextureID, file_name);
    TexturesList.push_back(this);   // add to global texture ids list (for autoclean)
    return true;
}

unsigned char* CTexture::LoadBitmap32(const char *file_name){
    CBitMapImage image;
    if(image.LoadFromFile(file_name)){
        m_width=image.GetWidth();
        m_height=image.GetHeight();
        m_bytes_pp=image.GetBytesPerPixel();
        m_texture_format=image.GetPixelFormat();
        return image.GetPixelData(m_data);
    }
    // create pixel data array and return pointer
    return NULL;
}
