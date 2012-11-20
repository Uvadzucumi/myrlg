#ifdef __WIN32__
    #include <windows.h>
#else
/* For detect user directory */
    #include <unistd.h> // getuid()
    #include <pwd.h>    // getpwuid()
#endif
#include <stdlib.h>

//#include <wchar.h>
//#include <locale.h>

#include "game.h"

CHudSprite *herro_sprite;

void OnRender(double dt){
    /*
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glBegin(GL_QUADS);
        glColor3f(1, 0, 0); glVertex3f(0, 0, 0);
        glColor3f(1, 1, 0); glVertex3f(100, 0, 0);
        glColor3f(1, 0, 1); glVertex3f(100, 100, 0);
        glColor3f(1, 1, 1); glVertex3f(0, 100, 0);
    glEnd();

    SDL_GL_SwapBuffers();
*/
    static unsigned int savedTime=0;


    MyOGL::Vector4f font_color;
    static char tmp[400];

//    static unsigned int savedTime=0;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    dungeon->Render();
    //
    glTranslatef((herro->GetPosX()-dungeon->GetViewportLeft())*32,(herro->GetPosY()-dungeon->GetViewportTop())*32,0);
    herro->Render();


//    for(unsigned int i = 0;i < MyOGL::EntityList.size();i++) {
//        if(!MyOGL::EntityList[i]) continue;
//        MyOGL::EntityList[i]->OnRender();
//    }

    glLoadIdentity();       // for hud - need set default matrix

    font_color.r=0;font_color.g=0;font_color.b=0;font_color.a=1;
    font->PrintAt(301,11,"Мама мыла раму!\nРама блистела!",font_color);
    font_color.g=1;
    font->PrintAt(300,10,"Мама мыла раму!\nРама блистела!",font_color);

    sprintf(tmp,"FPS: %d",(int)App->GetFPS());
    font_color.r=1; font_color.g=0,font_color.b=0;
    font->PrintAt(0,0,tmp,font_color);

    sprintf(tmp,"Dungeon coords: [%d,%d]",dungeon->GetViewportLeft(), dungeon->GetViewportTop());
    font_color.r=1; font_color.g=0,font_color.b=0;
    font->PrintAt(0,14,tmp,font_color);

    // Display FPS evry 5 seconds

    if(savedTime==0){
        savedTime=SDL_GetTicks();
    }else{
        unsigned int tmp=SDL_GetTicks();
        if((tmp-savedTime)>5000){
            savedTime=tmp;
            printf("FPS: %f\n",App->GetFPS());
        }
    }


}

// Update objects
void OnLoop(double DeltaTime){
//    float speed=100;
/*
    if(App->IsKeyPressed(SDLK_LEFT)){
        //if(dungeon->GetViewportLeft()>0) dungeon->SetViewportPosition(dungeon->GetViewportLeft()-1,dungeon->GetViewportTop());
        dungeon->ChangeViewportPosition(-1,0);
    }
    if(App->IsKeyPressed(SDLK_RIGHT)){
        //if(dungeon->GetViewportLeft()<(dungeon->GetWidth()-dungeon->GetViewportWidth())) dungeon->SetViewportPosition(dungeon->GetViewportLeft()+1,dungeon->GetViewportTop());
        dungeon->ChangeViewportPosition(1,0);
    }
    if(App->IsKeyPressed(SDLK_UP)){
        dungeon->ChangeViewportPosition(0,-1);
    }
    if(App->IsKeyPressed(SDLK_DOWN)){
        dungeon->ChangeViewportPosition(0,1);;
    }
    */
}

void OnEvent(SDL_Event *Event, double DeltaTime){
    // change camera position if button pressed
    switch(Event->type) {
        case SDL_KEYDOWN:
            switch(Event->key.keysym.sym){
                case SDLK_ESCAPE:
                    App->OnExit();
                    break;
                case SDLK_SPACE:
                    if(dungeon->GetGlobalLight()<9){
                        dungeon->SetGlobalLight(dungeon->GetGlobalLight()+1);
                    }else{
                        dungeon->SetGlobalLight(0);
                    }
                    Log->puts("Current Global Light: %d\n",dungeon->GetGlobalLight());
                    break;
                case SDLK_UP:
                case SDLK_KP8:
                    if(dungeon->Map()[herro->GetPosX()][herro->GetPosY()-1].can_move){
                        herro->SetPosition(herro->GetPosX(),herro->GetPosY()-1);
                        dungeon->SetViewportToTarget(herro->GetPosX(),herro->GetPosY());
                        dungeon->CalculateLOS(herro->GetPosX(),herro->GetPosY());
                    }
                    break;
                case SDLK_DOWN:
                case SDLK_KP2:
                    if(dungeon->Map()[herro->GetPosX()][herro->GetPosY()+1].can_move){
                        herro->SetPosition(herro->GetPosX(),herro->GetPosY()+1);
                        dungeon->SetViewportToTarget(herro->GetPosX(),herro->GetPosY());
                        dungeon->CalculateLOS(herro->GetPosX(),herro->GetPosY());
                    }
                    break;
                case SDLK_RIGHT:
                case SDLK_KP6:
                    if(dungeon->Map()[herro->GetPosX()+1][herro->GetPosY()].can_move){
                        herro->SetPosition(herro->GetPosX()+1,herro->GetPosY());
                        dungeon->SetViewportToTarget(herro->GetPosX(),herro->GetPosY());
                        dungeon->CalculateLOS(herro->GetPosX(),herro->GetPosY());
                    }
                    break;
                case SDLK_LEFT:
                case SDLK_KP4:
                    if(dungeon->Map()[herro->GetPosX()-1][herro->GetPosY()].can_move){
                        herro->SetPosition(herro->GetPosX()-1,herro->GetPosY());
                        dungeon->SetViewportToTarget(herro->GetPosX(),herro->GetPosY());
                        dungeon->CalculateLOS(herro->GetPosX(),herro->GetPosY());
                    }
                    break;
                case SDLK_KP7:
                    if(dungeon->Map()[herro->GetPosX()-1][herro->GetPosY()-1].can_move){
                        herro->SetPosition(herro->GetPosX()-1,herro->GetPosY()-1);
                        dungeon->SetViewportToTarget(herro->GetPosX(),herro->GetPosY());
                        dungeon->CalculateLOS(herro->GetPosX(),herro->GetPosY());
                    }
                    break;
                case SDLK_KP9:
                    if(dungeon->Map()[herro->GetPosX()+1][herro->GetPosY()-1].can_move){
                        herro->SetPosition(herro->GetPosX()+1,herro->GetPosY()-1);
                        dungeon->SetViewportToTarget(herro->GetPosX(),herro->GetPosY());
                        dungeon->CalculateLOS(herro->GetPosX(),herro->GetPosY());
                    }
                    break;
                case SDLK_KP1:
                    if(dungeon->Map()[herro->GetPosX()-1][herro->GetPosY()+1].can_move){
                        herro->SetPosition(herro->GetPosX()-1,herro->GetPosY()+1);
                        dungeon->SetViewportToTarget(herro->GetPosX(),herro->GetPosY());
                        dungeon->CalculateLOS(herro->GetPosX(),herro->GetPosY());
                    }
                    break;
                case SDLK_KP3:
                    if(dungeon->Map()[herro->GetPosX()+1][herro->GetPosY()+1].can_move){
                        herro->SetPosition(herro->GetPosX()+1,herro->GetPosY()+1);
                        dungeon->SetViewportToTarget(herro->GetPosX(),herro->GetPosY());
                        dungeon->CalculateLOS(herro->GetPosX(),herro->GetPosY());
                    }
                    break;
                default:
                    printf("Pressed key: %d\n",Event->key.keysym.sym);
                    break;
           }
    }
}


#ifdef __WIN32__
int WINAPI WinMain(HINSTANCE hInstance,
                  HINSTANCE hPrevInstance,
                  LPSTR lpCmdLine,
                  int nCmdShow){
#else
int main(int argc, char **argv){

    printf("Аргументов: %d\n",argc);
    for(int i=0;i<argc;i++){
        printf("\t%d. %s\n",i,argv[i]);
    }
#endif
    // get user directory
#ifdef __LINUX__
//printf("User Directory: %s\n", getenv("HOME"));
    struct passwd *user = NULL;
    uid_t user_id = getuid();
    user = getpwuid(user_id);
    printf("home dir: %s\n", user->pw_dir);
#else
    // Win: %HOMEPATH%, %USERPROFILE%
    printf("User Directory: %s\nUser Profile Dir: %s\n",getenv("HOMEPATH"),getenv("USERPROFILE"));
#endif

    App = new MyOGL::CApplication();

    if(!App->Init(800,600,32,false,"Моя RLG")){
        delete App;
        return -1;
    };

    App->SetWinIcon("data/ico.bmp");

    // set callback functions
    App->OnRender=OnRender;
    App->OnLoop=OnLoop;
    App->OnEvent=OnEvent;

    // load font textures
    font_texture = new MyOGL::CTexture();
    font_texture ->LoadFromFile("data/myfont.bmp");

    font=new MyOGL::CFont();
    font->LoadGlyphData("data/myfont.fnt",font_texture);
    // display list text
    text=new MyOGL::CText(font);
    text->SetText("Мама мыла раму!\nРама блистела!");

    // CreateLightMaterails
    for(unsigned int i=0;i<10;i++){
        float l=i*0.1;
        LightMaterials[i].SetDiffuseColor(1-l,1-l,1-l,1.0);
        Log->puts("Created light material %d\n",i);
    }

    // CreateTileset
    tiles_texture=new MyOGL::CTexture();
    tiles_texture->LoadFromFile("data/tileset.bmp");
    Tileset=new CTileset(tiles_texture);

    // create dungeon
    dungeon=new CDungeonLevel(200,200,20,10,80);
    dungeon->SetTileset(Tileset);
    dungeon->NewLevel();

    // create herro
    sroom FirstRoom=dungeon->GetRoom(0);
    herro=new CCreature();
    herro->SetPosition(rand()%FirstRoom.width+FirstRoom.left,rand()%FirstRoom.height+FirstRoom.top);
    herro_sprite=new CHudSprite(tiles_texture);
    herro_sprite->SetUVPixelCoords(0,128,32,32);
    herro_sprite->SetSize(32,32);
    herro->SetSprite(herro_sprite);

    dungeon->SetViewportToTarget(herro->GetPosX(),herro->GetPosY());
    dungeon->CalculateLOS(herro->GetPosX(),herro->GetPosY());

    App->Run();

    delete dungeon;
    delete font;
    delete text;
    delete App; // required
    delete herro_sprite;
    delete herro;

    printf("Normal Exit\n");
    return 0;
}
