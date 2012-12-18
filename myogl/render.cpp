
#include "render.h"

#include "texture.h"
#include "ui/bitmap_font.h"

using namespace MyOGL;

// extern pointer to render class
CRender *MyOGL::Render=NULL;
RenderStates MyOGL::GL;

// constructor
CRender::CRender(){
    // Check OpenGL?
    // nit variables
    // Window parameters
    m_width=0;
    m_height=0;
    m_bpp=0;
    m_full_screen=false;
    // render context
    Context=NULL;
    zNearPlane=0.1;
    zFarPlane=500;
}

// destructor
CRender::~CRender(){
    Free();
    SDL_Quit();
}

// clear all data
void CRender::Free(){
    if(Context!=NULL){
        SDL_FreeSurface(Context);
        Context=NULL;
    }
}

// set start OpenGL states
void CRender::InitGL(){
    glColor3f(1,0,0);
    glClearColor(0,0,0,1);
    glClearDepth(1.0f);
    // now - not need - ViewPort cahgend in Set2D
    //glViewport(0, 0, this->m_width, this->m_height);
    glEnable(GL_TEXTURE_2D);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);
    //
    glColorMaterial(GL_FRONT, GL_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    glDisable(GL_LIGHTING);
    glDisable(GL_BLEND);
    //glBlendEquation(GL_FUNC_ADD);

    Set2D(true);    // Set2D projection
    GL.GetCurrentStates();
    GL.Debug();
}

// Render class realisation
bool CRender::Init(int width, int height, int bpp, bool full_screen, const char *title){
    this->m_width=width;
    this->m_height=height;
    this->m_bpp=bpp;
    this->m_full_screen=full_screen;

    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        Log->puts("CRender::Init SDL_Init(): false\n");
        return false;
    }
#ifdef __WIN32__
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE,            5);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,          5);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,           5);
//    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,          8);

    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,          16);
//    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE,         32);
//    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS,  1);
//    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES,  2);
#endif
    window_flags = SDL_HWSURFACE | SDL_OPENGL;
    #ifdef MYOGL_DOUBLE_BUFFER
        window_flags |= SDL_GL_DOUBLEBUFFER;
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        printf("Double Buffer Enabled!\n");
    #else
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 0);
        printf("Dounble Buffer Disabled\n");
    #endif

    #ifdef MYOGL_RESIZABLE_WINDOW
        window_flags |= SDL_RESIZABLE;
    #endif

    if(this->m_full_screen){
        window_flags |= SDL_FULLSCREEN;
    }
    // Set Caption
    SetWinCaption(title);
    // create render context
    if((Context = SDL_SetVideoMode(width, height, bpp, window_flags)) == NULL) {
        Log->puts("CRender::Init SDL_SetVideoMode(): false\n");
        return false;
    }
    // Check double buffer
    int tmp;
    SDL_GL_GetAttribute(SDL_GL_DOUBLEBUFFER, &tmp);
    printf("Double Buffering: %s\n",(tmp)?"Enabled":"Disabled");
    // show Opengl Info
    this->gl_vendor = (char *) glGetString(GL_VENDOR);
    Log->puts("OpenGL Vendor: %s\n",gl_vendor);
    gl_version = (char *) glGetString(GL_VERSION);
    Log->puts("OpenGL Version: %s\n",gl_version);
    if(!strcmp(gl_vendor,"Microsoft Corporation")){
       Log->puts("Need install Video Driver!\n");
//        return false;
    }else{

        gl_extensions = (char *) glGetString(GL_EXTENSIONS);
        Log->puts("OpenGL Extensions:\n");
        for(int i=0;gl_extensions[i]!=0;i++){
            if(gl_extensions[i]==32) { gl_extensions[i]=10; }
        }
        Log->puts(gl_extensions);
    }

    // Init OpenGL
    InitGL();

    return true;
}

// Set Orthogonal projection
void CRender::Set2D(bool force){
    if(force || GL.mode3d){
        glViewport (0, 0, this->m_width, this->m_height);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, this->m_width, this->m_height, 0, 1, -1);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        GL.mode3d=false;
        GL.Disable(GL_DEPTH_TEST);
    //    Log->puts("Set 2D projection\n");
    }
}

// Set Orthogonal projection
void CRender::Set3D(bool force){
    if(force || !GL.mode3d){
        glViewport (0, 0, this->m_width, this->m_height);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective( 45.0f, (GLfloat)this->m_width/(GLfloat)this->m_height, 0.1f, 100.0f );
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        GL.mode3d=true;
        Log->puts("Set 3D projection\n");
    }
}

// Swap render buffers
void CRender::SwapBuffers(){
    SDL_GL_SwapBuffers();
}

// Window functions
void CRender::SetWinCaption(const char *title){
   SDL_WM_SetCaption(title,NULL);
}

bool CRender::SetWinIcon(const char *file_name){
    SDL_Surface* surface;
//Load Bitmap
    if(!(surface = SDL_LoadBMP(file_name))) {
        Log->puts("Error load icon file name: %s\n",file_name);
        return false;
    }
    SDL_WM_SetIcon(surface, NULL);
    SDL_FreeSurface(surface);
    return true;
}

void CRender::BindTexture(GLuint TextureID, bool force){
    if(GL.CurrentTexure!=TextureID || force){
	// Bind the texture object
        glBindTexture( GL_TEXTURE_2D, TextureID );
        GL.CurrentTexure=TextureID;
    }
}

void CRender::SetBlendMode(MyGlBlendMode mode){
    if(GL.CurrentBlendMode==mode) return;   // no need change
    // set new blend mode
    switch(mode){
        case blNone:
            GL.Disable(GL_BLEND);
            break;
        case blSource:
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            GL.Enable(GL_BLEND);
            break;
        case blConstColor:
            glBlendFunc(GL_CONSTANT_COLOR, GL_DST_COLOR);
            GL.Enable(GL_BLEND);
            break;
        case blAdditive:
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            GL.Enable(GL_BLEND);
        default:
            //printf("warnind: unknown blend mode %d\n",mode);
            Log->puts("warnind: unknown blend mode %x\n",mode);
    }
}

bool CRender::OnResize(int width, int height){
//#ifdef __WIN32__
//    Log->puts("Not supported Windoew resize in Windows\n");
//#else
    SDL_Surface *old_context;
    old_context=Context;
    if((Context = SDL_SetVideoMode(width, height, m_bpp, window_flags)) == NULL) {
        Log->puts("CRender::Init SDL_SetVideoMode(): false\n");
        Context=old_context;
        return false;
    }

    this->m_width=width;
    this->m_height=height;
    SDL_FreeSurface(old_context);
#ifdef __WIN32__
    Log->puts("for Windows need reinicialize OGL states and recreate texture!\n");
    InitGL();
    // recreate textures
    for(unsigned int i=0;i<TexturesList.size();i++){
        // free old from video memory
        //GLuint texture_id=TexturesList[i]->GetID();
        //glDeleteTextures(1,&texture_id);
        // create new in video memory
        TexturesList[i]->CreateFromMemory();
        Log->puts("texture %d recreated. ", TexturesList[i]->GetID());
        Log->puts("file: %s\n", TexturesList[i]->GetFileName());
    }
    // recreate text display lists
    for(unsigned int i=0;i<TextsList.size();i++){
        TextsList[i]->CreateDisplayList();
        Log->puts("CText object display list recreated\n");
    }
#else
    // in normal OS need only resize Viewport
    glViewport(0,0,this->m_width,this->m_height);
        // set projection - force reset viewport
    if(GL.mode3d){
        Set3D(true);
    }else{
        Set2D(true);
    }
#endif
    printf("window resized to %dx%d\n", width, height);
    return true;
}

// Set Inc color
void CRender::SetColor(float r, float g, float b, float a, bool force){
    if(GL.Color.r!=r || GL.Color.g!=g || GL.Color.b != b || GL.Color.a!=a || force){
        GL.Color.r=r; GL.Color.g=g; GL.Color.b=b; GL.Color.a=a;
        glColor4f(r,g,b,a);
    }
}
// Set Inc color
void CRender::SetColor(Vector3i color, bool force){
    this->SetColor(((float)color.r)/255,((float)color.g)/255,((float)color.b)/255,1.0, force);
}
// Set Paper color
void CRender::SetClearColor(float r, float g, float b, float a){
    if(GL.ClearColor.r==r && GL.ClearColor.g==g && GL.ClearColor.b == b && GL.ClearColor.a==a){
        // nop
    }else{
        GL.ClearColor.r=r; GL.ClearColor.g=g; GL.ClearColor.b=b; GL.ClearColor.a=a;
        glClearColor(r,g,b,a);
    }
}

// Set Blend color
void CRender::SetBlendColor(float r, float g, float b, float a, bool force){
    if(GL.BlendColor.r!=r || GL.BlendColor.g!=g || GL.BlendColor.b != b || GL.BlendColor.a!=a || force){
        GL.BlendColor.r=r; GL.BlendColor.g=g; GL.BlendColor.b=b; GL.BlendColor.a=a;
        glBlendColor(r,g,b,a);
    }
}

// cashed OGL states
void RenderStates::Enable(GLenum cap){
    bool *param;
    param=GetGLParam(cap);
    if(param && !*param){
        *param=true;
        glEnable(cap);
//        printf(" Enable\n");
    }
//    else{
//        printf(" Skip\n");
//    }
}

// cashed OGL states
void RenderStates::Disable(GLenum cap){
    bool *param;
    param=GetGLParam(cap);
    if(param && *param){
        *param=false;
        glDisable(cap);
//        printf(" Disable\n");
    }
//    else{
//        printf(" Skip\n");
//    }
}

bool *RenderStates::GetGLParam(GLenum cap){
    switch(cap){
        case GL_COLOR_MATERIAL:
            //printf("color material: ");
            return &ColorMaterial;
        case GL_TEXTURE_2D:
            //printf("texture2d: ");
            return &Texture2D;
        case GL_DEPTH_TEST:
            return &DepthTest;
        case GL_LIGHTING:
            return &Lighting;
        case GL_BLEND:
            return &Blend;
        default:
            Log->puts("Wrong GL.Enable parameter: %x\n",cap);
            return NULL;
    }
}
//  Enabled:=Opengl1x.glisEnabled(GL_DEPTH_TEST);
void RenderStates::GetCurrentStates(){
//
   ColorMaterial=glIsEnabled(GL_COLOR_MATERIAL);
   Texture2D=glIsEnabled(GL_TEXTURE_2D);
   DepthTest=glIsEnabled(GL_DEPTH_TEST);
   Lighting=glIsEnabled(GL_LIGHTING);
   Blend=glIsEnabled(GL_BLEND);
   // blend color
   glGetFloatv(GL_BLEND_COLOR, BlendColor.data);
   // get current color
   glGetFloatv(GL_CURRENT_COLOR, Color.data);
   // -//- clear color
   glGetFloatv(GL_COLOR_CLEAR_VALUE, ClearColor.data);
   // -//- texture
   // get max texture units
   glGetIntegerv(GL_MAX_TEXTURE_UNITS,&MaxTextureUnits);
}

void RenderStates::Debug(void){
    Log->puts("---[ OpenGL states Debug ]---\n");
    Log->puts("GL_COLOR_MATERIAL: %s\n",(ColorMaterial)?"true":"false");
    Log->puts("GL_TEXTURE_2D: %s\n",(Texture2D)?"true":"false");
    Log->puts("GL_DEPTH_TEST: %s\n",(DepthTest)?"true":"false");
    Log->puts("GL_LIGHTING: %s\n",(Lighting)?"true":"false");
    Log->puts("GL_BLEND: %s\n",(Blend)?"true":"false");
    Log->puts("GL_BLEND_COLOR: (%f,%f,%f,%f)\n",&BlendColor);
    Log->puts("GL_CURRENT_COLOR: (%f,%f,%f,%f)\n",&Color);
    Log->puts("GL_COLOR_CLEAR_VALUE: (%f,%f,%f,%f)\n",&ClearColor);
    Log->puts("GL_MAX_TEXTURE_UNITS: %d\n",MaxTextureUnits);
    Log->puts("----------------------------\n");
}
