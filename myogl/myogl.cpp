#include "myogl.h"
#include "log.h"

#include <unistd.h> // for sleep

/* For detect user directory */
//#ifdef __LINUX__
//    #include <unistd.h> // getuid()
//    #include <pwd.h>    // getpwuid()
//#endif

using namespace MyOGL;

bool CApplication::Init(int width, int height, int bpp, bool full_screen, const char *title){

// set user home folder
#ifdef __LINUX__
//    printf("User Directory: %s\n", getenv("HOME"));
//    struct passwd *user = NULL;
//    uid_t user_id = getuid();
//    user = getpwuid(user_id);
//    printf("home dir: %s\n", user->pw_dir);
    user_home_dir=new char[strlen(getenv("HOME")+1)];
    strcpy(user_home_dir,getenv("HOME"));
#else
    // Win: %HOMEPATH%, %USERPROFILE%
    user_home_dir=new char[strlen(getenv("USERPROFILE")+1)];
    strcpy(user_home_dir,getenv("USERPROFILE"));
#endif
    printf("user home dir: %s\n",user_home_dir);

    Log=new CLog();
    Log->Init("log.txt");
    // init keys state array
    for(int i = 0; i < 322; i++) { // init them all to false
        KEYS[i] = false;
    }
    // create window
    if(Render!=NULL){
        Log->puts("CApp::Init - Render not NULL!\n");
        return false;
    }
    Render=new CRender();
    // init render object
    if(!Render->Init(width,height,bpp,full_screen,title)){
        return false;
    }
    return true;
};

int CApplication::Run(void){
    unsigned int currTime, lastTime, savedTime=0; // curren ticks count, previos framw tick count, saved tick count for frames calculator
    unsigned int frames=0;
    SDL_Event Event;
    // Check inicialized render class
    if(!Render){
        Log->puts("CApplication::Run() - Render class not Inicialized!\n");
        return -1;
    }
    this->Running=true;
    // inicialize fpc counter and dt calculation
    lastTime=SDL_GetTicks()-1;
    while(Running){
        currTime=SDL_GetTicks();
        DeltaTime=(float)(currTime-lastTime)/1000.0;
        if((currTime-savedTime)>100){
            FPS=frames*1000/(currTime-savedTime);
            frames=0;
            savedTime=currTime;
        }
        lastTime=currTime;
        // check events
        while(SDL_PollEvent(&Event)) {
            //if(Event.type == SDL_QUIT) {
            //    Running = false;
            //}else{
                this->Events(&Event, DeltaTime);
            //}
        }
        // render scene

        if(OnRender!=NULL){
            OnRender(DeltaTime);
        }

        // update all entity
        for(unsigned int i = 0;i < MyOGL::EntityList.size();i++) {
            if(!MyOGL::EntityList[i]) continue;
            MyOGL::EntityList[i]->OnLoop(DeltaTime);
        }

        // timer actions... animation and etc...
        if(OnLoop!=NULL){
            OnLoop(DeltaTime);
        }

        Render->SwapBuffers();
        usleep(10);
        frames++;
    }
    Free();    // FreeMemory
    return 0;
}

// parse all SDL events
void CApplication::Events(SDL_Event *Event, double DeltaTime){
    switch(Event->type) {
        case SDL_VIDEORESIZE: {     // window resize
                Render->OnResize(Event->resize.w,Event->resize.h);
            break;
        }
        case SDL_KEYDOWN:
                KEYS[Event->key.keysym.sym] = true;
                //Log->puts("Key %d pressed\n",Event->key.keysym.sym);
                break;
        case SDL_KEYUP:
                KEYS[Event->key.keysym.sym] = false;
                //Log->puts("Key %d unpressed\n",Event->key.keysym.sym);
                break;
    }
    // mouse events
    // check gui
    //if(Event->)
    // user events function
    if(OnEvent!=NULL) OnEvent(Event, DeltaTime);
    // send event to event class
    CEvent::OnEvent(Event);
}

// Clear All Allpication Data
void CApplication::Free(){
    // clear textures list
    Log->puts("Application::Free()\n");
    Log->puts("textures count: %d\n",TexturesList.size());
    for(int i = TexturesList.size()-1; i >=0;i--){
        if(!TexturesList[i]) continue;
        delete TexturesList[i];
    }
    TexturesList.clear();

    // clear Entity list
    Log->puts("entity count: %d\n",TexturesList.size());
    for(int i = EntityList.size()-1;i >=0 ;i--) {
        if(!EntityList[i]) continue;
        delete EntityList[i];
    }
    EntityList.clear();
    if(Render!=NULL) delete Render;
}




