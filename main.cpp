#ifdef __WIN32__
    #include <windows.h>
#else
/* For detect user directory */
    #include <unistd.h> // getuid()
    #include <pwd.h>    // getpwuid()
#endif
#include <stdlib.h>
#include <time.h>   // for inicialize rand seed

//#include <wchar.h>
//#include <locale.h>

#include "game.h"
#include "myogl/config_file.h"

CHudSprite *herro_sprite;

void OnRender(double dt){

//    static unsigned int savedTime=0;

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
/*
    font_color.r=0;font_color.g=0;font_color.b=0;font_color.a=1;
    font->PrintAt(301,11,"Мама мыла раму!\nРама блистела!",font_color);
    font_color.g=1;
    font->PrintAt(300,10,"Мама мыла раму!\n^ffff00Рама блистела!",font_color);
*/
    font_color.r=0;font_color.g=1;font_color.b=0;font_color.a=1;
    text->PrintAt(300,10,font_color);


    //font_color.r=0;font_color.g=0;font_color.b=0;font_color.a=1;
    font_color.r=0; font_color.g=1,font_color.b=0;
    sprintf(tmp,"FPS: ^ffff00%d",(int)App->GetFPS());
    font->PrintAt(1,1,tmp,font_color);

    sprintf(tmp,"Координаты героя: [^ffffff%d,%d^ff0000]",herro->GetPosX(), herro->GetPosY());
    font_color.r=1; font_color.g=0,font_color.b=0;
    font->PrintAt(0,14,tmp,font_color);

/*
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
*/
}

// Update objects
void OnLoop(double DeltaTime){
//    float speed=100;
    if(App->IsKeyPressed(SDLK_LEFT) || App->IsKeyPressed(SDLK_KP4)){
        herro->Move(-1,0, dungeon);
        //dungeon->ChangeViewportPosition(-1,0);
    }
    if(App->IsKeyPressed(SDLK_RIGHT)|| App->IsKeyPressed(SDLK_KP6)){
        herro->Move(1,0, dungeon);
        //dungeon->ChangeViewportPosition(1,0);
    }
    if(App->IsKeyPressed(SDLK_UP)|| App->IsKeyPressed(SDLK_KP8)){
        herro->Move(0,-1, dungeon);
        //dungeon->ChangeViewportPosition(0,-1);
    }
    if(App->IsKeyPressed(SDLK_DOWN)|| App->IsKeyPressed(SDLK_KP2)){
        herro->Move(0,1, dungeon);
        //dungeon->ChangeViewportPosition(0,1);;
    }
    if(App->IsKeyPressed(SDLK_KP7)){
        herro->Move(-1,-1, dungeon);
    }
    if(App->IsKeyPressed(SDLK_KP9)){
        herro->Move(1,-1, dungeon);
    }
    if(App->IsKeyPressed(SDLK_KP1)){
        herro->Move(-1,1, dungeon);
    }
    if(App->IsKeyPressed(SDLK_KP3)){
        herro->Move(1,1, dungeon);
    }
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
                case SDLK_o:
                    herro->OpenDoor(dungeon);
                    break;
/*
                case SDLK_UP:
                case SDLK_KP8:
                    herro->Move(0,-1,dungeon);
                    break;
                case SDLK_DOWN:
                case SDLK_KP2:
                    herro->Move(0,1,dungeon);
                    break;
                case SDLK_RIGHT:
                case SDLK_KP6:
                    herro->Move(1,0,dungeon);
                    break;
                case SDLK_LEFT:
                case SDLK_KP4:
                    herro->Move(-1,0,dungeon);
                    break;
                case SDLK_KP7:
                    herro->Move(-1,-1,dungeon);
                    break;
                case SDLK_KP9:
                    herro->Move(1,-1,dungeon);
                    break;
                case SDLK_KP1:
                    herro->Move(-1,1,dungeon);
                    break;
                case SDLK_KP3:
                    herro->Move(1,1,dungeon);
                    break;
*/
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

    srand(time(NULL));

    // get user directory
    cfg_file = new CConfigFile("myrlg.conf");
    App = new MyOGL::CApplication();

    if(!App->Init(
                    cfg_file->GetParamValue("screen_width",800),
                    cfg_file->GetParamValue("screen_height",600),
                    cfg_file->GetParamValue("screen_bpp",32),
                    (bool)cfg_file->GetParamValue("full_screen",0),
                    "Моя RLG"
      //  800,600,32,false,"Моя RLG"
    )){
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
    text->SetText("Мама мыла раму!\n^ffff00Рама блистела!");

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
    herro=new CHerro();
    herro->movement_delay=cfg_file->GetParamValue("movement_delay",50);
    herro->SetPosition(rand()%FirstRoom.width+FirstRoom.left,rand()%FirstRoom.height+FirstRoom.top);
    herro_sprite=new CHudSprite(tiles_texture);
    herro_sprite->SetUVPixelCoords(0,128,32,32);
    herro_sprite->SetSize(32,32);
    herro->SetSprite(herro_sprite);

    Log->puts("Minimum map tile size: %d Bytes\n",sizeof(sMapField));

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
