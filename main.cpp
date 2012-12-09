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
#include "myogl/ui/text_box.h"

CHudSprite *herro_sprite;
CTextBox *messages;

void OnRender(double dt){

//    static unsigned int savedTime=0;

    MyOGL::Vector4f font_color;
    static char tmp[400];

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
// set default viewport
    Render->Set2D();

    dungeon->Render();
    //
    if(ActiveWindow==gwInventory){
        herro->inventory->Render();
    }else if(ActiveWindow==gwInventoryItemDescription){
        herro->inventory->RenderItemDetail();
    }

    glTranslatef((herro->GetPosX()-dungeon->GetViewportLeft())*32,(herro->GetPosY()-dungeon->GetViewportTop())*32,0);
    herro->Render();


//    for(unsigned int i = 0;i < MyOGL::EntityList.size();i++) {
//        if(!MyOGL::EntityList[i]) continue;
//        MyOGL::EntityList[i]->OnRender();
//    }

    glLoadIdentity();       // for hud - need set default matrix

//    font_color.r=0;font_color.g=1;font_color.b=0;font_color.a=1;
//    text->PrintAt(300,10,font_color);

// set new viewport
/*
        glViewport(0,400,100,100);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        //  x left, y top
        // left, right, bottom, top
        glOrtho(0, 100, 100, 0, 1, -1);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
*/

    font_color.r=0; font_color.g=1,font_color.b=0, font_color.a=1;
    sprintf(tmp,"FPS: ^ffff00%d",(int)App->GetFPS());
    font->PrintAt(1,1,tmp,font_color);

    sprintf(tmp,"Координаты героя: [^ffffff%d^ff0000,^ffffff%d^ff0000]",herro->GetPosX(), herro->GetPosY());
    font_color.r=1; font_color.g=0,font_color.b=0;
    font->PrintAt(1,14,tmp,font_color);

    // set white color
    font_color.r=1; font_color.g=1,font_color.b=1;
    messages->Render(font_color);

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
    if(ActiveWindow==gwMain){
        if(App->IsKeyPressed(SDLK_LEFT) || App->IsKeyPressed(SDLK_KP4)){
            herro->Move(-1,0, dungeon);
        }
        if(App->IsKeyPressed(SDLK_RIGHT)|| App->IsKeyPressed(SDLK_KP6)){
            herro->Move(1,0, dungeon);
        }
        if(App->IsKeyPressed(SDLK_UP)|| App->IsKeyPressed(SDLK_KP8)){
            herro->Move(0,-1, dungeon);
        }
        if(App->IsKeyPressed(SDLK_DOWN)|| App->IsKeyPressed(SDLK_KP2)){
            herro->Move(0,1, dungeon);
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
        if(App->IsKeyPressed(SDLK_i)){  // goto imventory window
            ActiveWindow=gwInventory;
        }
    }
    // update map - only animations and lights
    dungeon->Update(DeltaTime);
}

void OnEvent(SDL_Event *Event, double DeltaTime){
    char str[100];
    // change camera position if button pressed
    switch(Event->type) {
        case SDL_KEYDOWN:

        if(ActiveWindow==gwMain){

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
                    //Log->puts("Current Global Light: %d\n",dungeon->GetGlobalLight());
                    sprintf(str,"Current Global Light: ^00ff00%d", dungeon->GetGlobalLight());
                    messages->AddString(str);
                    break;
                case SDLK_o:
                    herro->OpenDoor(dungeon);
                    break;
                default:
//                    printf("Pressed key: %d\n",Event->key.keysym.sym);
                    break;
           }
        }else if(ActiveWindow==gwInventory){
            if(Event->key.keysym.sym==SDLK_ESCAPE){ // exit to main window
                ActiveWindow=gwMain;
            }else{
                // get items from pressed button
                if(herro->inventory->SelectItemByKey(Event->key.keysym.sym)){
                    ActiveWindow=gwInventoryItemDescription;
                }else{
                    MyOGL::Log->puts("Pressed %d key in Inventory Window\n",Event->key.keysym.sym);
                }
            }
        }else if(ActiveWindow==gwInventoryItemDescription){
            if(Event->key.keysym.sym==SDLK_ESCAPE){ // exit to inventory window
                ActiveWindow=gwInventory;
            }else{
            // TODO - equip, drop item(s)
                switch(Event->key.keysym.sym){
                    case 'e':
                        if(App->IsKeyPressed(SDLK_RSHIFT) || App->IsKeyPressed(SDLK_LSHIFT)){
                            MyOGL::Log->puts("Eat/Drink action!\n");
                        }else{
                            //MyOGL::Log->puts("Equip action!\n");
                            herro->inventory->Equip(); // equip current selected item
                        }
                        break;
                    case 't':
                        //MyOGL::Log->puts("UnEquip action!\n");
                        herro->inventory->Unequip(); // UnEquip current selected item
                        break;
                    case 'd':
                        MyOGL::Log->puts("Drop action!\n");
                        break;
                    //case 'E':
                    //    MyOGL::Log->puts("Eat/Drink action!\n");
                    //    break;
                    case 'r':
                        MyOGL::Log->puts("Read action!\n");
                        break;
                    default:
                        //MyOGL::Log->puts("Unknown item action\n",Event->key.keysym.sym);
                        break;
                }

            }
        }
    }
}

void OnWindowResize(unsigned int Width, unsigned int Height){
    // resize dungeon Viewport
//    printf("w: %d h: %d\n",Width, Height);
    int new_vp_h=(Height - Height % 32)/32;
    dungeon->SetViewportSize(new_vp_h, new_vp_h);
    dungeon->SetViewportToTarget(herro->GetPosX(), herro->GetPosY());
    dungeon->CalculateLOS(herro->GetPosX(), herro->GetPosY());
    // change message viewport
    messages->SetViewPort(dungeon->GetViewportWidth()*32,50,MyOGL::Render->GetWidth()-dungeon->GetViewportWidth()*32,MyOGL::Render->GetHeight()-50);

//    printf("New Dungeon Viewport Size=%dx%d\n",new_vp_h,new_vp_h);
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
    App->OnWindowResize=OnWindowResize;

    // load font textures
    font_texture = new MyOGL::CTexture();
    font_texture ->LoadFromFile("data/myfont.bmp");

    font=new MyOGL::CFont();
    font->LoadGlyphData("data/myfont.fnt",font_texture);
    // display list text
    text=new MyOGL::CText(font);
    text->SetText("Мама мыла раму!\n^ffff00Рама блистела!",30);

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

    // Inicialize Items Database
    InitItemsDB();

    // create dungeon
    dungeon=new CDungeonLevel(200,200);
    dungeon->SetTileset(Tileset);
    dungeon->NewGridDungeon(20,20,150);

    // create herro

    MyOGL::Vector2i pos=dungeon->GetStartPosition();

    herro=new CHerro();
    herro->movement_delay=cfg_file->GetParamValue("movement_delay",50);
    herro->SetPosition(pos.x, pos.y);
    herro_sprite=new CHudSprite(tiles_texture);
    herro_sprite->SetUVPixelCoords(0,128,32,32);
    herro_sprite->SetSize(32,32);
    herro->SetSprite(herro_sprite);

    herro->inventory->SetItemTileset(Tileset); // Items tileset
    herro->inventory->SetFont(font);
    herro->inventory->AddItem(100,1);
    herro->inventory->AddItem(101,1);
    herro->inventory->AddItem(102,1);
    herro->inventory->AddItem(103,1);
    herro->inventory->AddItem(104,1);

    Log->puts("Minimum map tile size: %d Bytes\n",sizeof(sMapField));

    messages=new MyOGL::CTextBox(font);
    // right from dungeon vievport
    messages->SetViewPort(dungeon->GetViewportWidth()*32, 50, Render->GetWidth()-dungeon->GetViewportWidth()*32, Render->GetHeight()-50);
    messages->AddString("First Test string!!!");
    messages->AddString("Second string!!!");
    messages->AddString("Next string! Строка может быть очень длинной, тогда будет работать перенос на новую!!!");

    dungeon->SetViewportToTarget(herro->GetPosX(),herro->GetPosY());
    dungeon->CalculateLOS(herro->GetPosX(),herro->GetPosY());

    Log->puts("Dungeond Created\n");

    ActiveWindow=gwMain;

    App->Run();

    // Clear Items Database
    DeleteItemsDB();

    delete dungeon;
    delete text;
    delete messages;
    delete font;
    delete App; // required
    delete herro_sprite;
    delete herro;

    printf("Normal Exit\n");
    return 0;
}
