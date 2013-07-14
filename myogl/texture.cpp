
#include "texture.h"
#include "log.h"

#include "fileformats/bmpimage.h"

using namespace MyOGL;

// decodePNG function
extern int decodePNG(std::vector<unsigned char>& out_image, unsigned long& image_width, unsigned long& image_height, const unsigned char* in_png, size_t in_size, bool convert_to_rgba32 = true);

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
    m_image_data.clear();
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

    if(!m_image_data.size()){
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
    glTexImage2D( GL_TEXTURE_2D, 0, m_bytes_pp, m_width, m_height, 0,
                     m_texture_format, GL_UNSIGNED_BYTE, &m_image_data[0] );
    return true;
}

// load image from  file filename,
// save in data SDL surface
// create texture in video memory
bool CTexture::LoadFromFile(const char *file_name){
    strcpy(m_file_name,file_name);
#ifdef USE_PNG_IMAGE
    if(!LoadPNGImage(m_file_name)){
#else // locad bmp image
    if( !LoadBitmapImage(m_file_name) ) {
#endif
        // create texture
        CreateFromMemory();
    } else {
        Log->printf("CTexture::LoadFromFile() could not load image %s\n",file_name);
        return false;
    }
    Log->printf("Created Texture %d (%s)\n",TextureID, file_name);
    TexturesList.push_back(this);   // add to global texture ids list (for autoclean)
    return true;
}

// load file to vector buffer
void CTexture::loadFile(std::vector<unsigned char>& buffer, const std::string& filename){
  std::ifstream file(filename.c_str(), std::ios::in|std::ios::binary|std::ios::ate);

  //get filesize
  std::streamsize size = 0;
  if(file.seekg(0, std::ios::end).good()) size = file.tellg();
  if(file.seekg(0, std::ios::beg).good()) size -= file.tellg();

  //read contents of the file into the vector
  if(size > 0){
    buffer.resize((size_t)size);
    file.read((char*)(&buffer[0]), size);
  }
  else buffer.clear();
}

//load and decode PNG image
int CTexture::LoadPNGImage(const char *file_name){
    std::vector<unsigned char> buffer;
    loadFile(buffer, file_name);
    unsigned long w, h;
    int error = decodePNG(m_image_data, w, h, buffer.empty() ? 0 : &buffer[0], (unsigned long)buffer.size());
    if(!error){
        m_width=w; m_height=h;
        m_texture_format=GL_RGBA;
        m_bytes_pp=4;
    }
    return error;
}

int CTexture::LoadBitmapImage(const char *file_name){
    CBitMapImage image;
    if(image.LoadFromFile(file_name)){
        m_width=image.GetWidth();
        m_height=image.GetHeight();
        m_bytes_pp=image.GetBytesPerPixel();
        m_texture_format=image.GetPixelFormat();
        // create pixel data array and return pointer
        image.GetPixelData(m_image_data);
    }
    return 0;
}
