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
#include "uiwin/cell_info.h"

//#include "myogl/frame_buffer.h"

CHudSprite *hero_sprite;
CTextBox *messages;
CUICellInfo *ui_tile_info_panel;

bool diffuse_light;

void OnRender(double dt){

    MyOGL::Vector4f font_color;
    static char tmp[400];

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
// set default viewport
    Render->Set2D();

    dungeon->Render();
    //
    // herro render
    glPushMatrix();
    glTranslatef((hero->GetPosX()-dungeon->GetViewportLeft())*32,(hero->GetPosY()-dungeon->GetViewportTop())*32,0);
    hero->Render();
    glPopMatrix();

    if(ActiveWindow==gwInventory){
        hero->inventory->Render();
    }else if(ActiveWindow==gwInventoryItemDescription){
        hero->inventory->RenderItemDetail();
    }

    glLoadIdentity();       // for hud - need set default matrix

    // show FPS
    font_color.r=0; font_color.g=1,font_color.b=0, font_color.a=1;
    sprintf(tmp,"FPS: ^ffff00%d",(int)App->GetFPS());
    font->PrintAt(1,1,tmp,font_color);

    // render messages
    font_color.r=1; font_color.g=1,font_color.b=1;
    messages->Render(font_color);

    // show selected tile
    if(mouse_on_tile && ActiveWindow==gwMain){
        int box_coord_x=mouse_on_tile_x*32, box_coord_y=mouse_on_tile_y*32;
        MyOGL::GL.Disable(GL_TEXTURE_2D);
        // display tile box
        MyOGL::Render->SetBlendMode(blNone);
        MyOGL::Render->SetColor(1,1,0);
        glBegin(GL_LINE_LOOP);
        glVertex2i(box_coord_x,box_coord_y);
        glVertex2i(box_coord_x+34,box_coord_y);
        glVertex2i(box_coord_x+34,box_coord_y+34);
        glVertex2i(box_coord_x,box_coord_y+34);
        glEnd();
        MyOGL::GL.Enable(GL_TEXTURE_2D);
        // display tile info
        // info mouse cursor coords
        Vector2i mouse_pos;
        mouse_pos=App->GetMousePos();
        // get tile coords for display tile info
        int map_tile_x=dungeon->GetViewportLeft()+mouse_on_tile_x;
        int map_tile_y=dungeon->GetViewportTop()+mouse_on_tile_y;
        // display tile information box
        ui_tile_info_panel->SetPosition(mouse_pos.x+10, mouse_pos.y+10);
        ui_tile_info_panel->SetMapTileCoords(map_tile_x, map_tile_y);
        ui_tile_info_panel->Render();

    }

}

// Update objects
void OnLoop(double DeltaTime){
//    float speed=100;
    if(ActiveWindow==gwMain){
        if(App->IsKeyPressed(SDLK_LEFT) || App->IsKeyPressed(SDLK_KP4)){
            hero->Move(-1,0, dungeon);
        }
        if(App->IsKeyPressed(SDLK_RIGHT)|| App->IsKeyPressed(SDLK_KP6)){
            hero->Move(1,0, dungeon);
        }
        if(App->IsKeyPressed(SDLK_UP)|| App->IsKeyPressed(SDLK_KP8)){
            hero->Move(0,-1, dungeon);
        }
        if(App->IsKeyPressed(SDLK_DOWN)|| App->IsKeyPressed(SDLK_KP2)){
            hero->Move(0,1, dungeon);
        }
        if(App->IsKeyPressed(SDLK_KP7)){
            hero->Move(-1,-1, dungeon);
        }
        if(App->IsKeyPressed(SDLK_KP9)){
            hero->Move(1,-1, dungeon);
        }
        if(App->IsKeyPressed(SDLK_KP1)){
            hero->Move(-1,1, dungeon);
        }
        if(App->IsKeyPressed(SDLK_KP3)){
            hero->Move(1,1, dungeon);
        }
        if(App->IsKeyPressed(SDLK_i)){  // goto inventory window
            ActiveWindow=gwInventory;
        }
    }
    // update map - only animations and lights
    dungeon->Update(DeltaTime);

}

void OnEvent(SDL_Event *Event, double DeltaTime){
    //char str[100];
    // change camera position if button pressed
    switch(Event->type) {
        case SDL_KEYDOWN:

        if(ActiveWindow==gwMain){

            switch(Event->key.keysym.sym){
                case SDLK_ESCAPE:
                    App->OnExit();
                    break;
                case SDLK_SPACE:
                /*
                    if(dungeon->GetGlobalLight()<9){
                        dungeon->SetGlobalLight(dungeon->GetGlobalLight()+1);
                    }else{
                        dungeon->SetGlobalLight(0);
                    }
                    //Log->puts("Current Global Light: %d\n",dungeon->GetGlobalLight());
                    sprintf(str,"Current Global Light: ^00ff00%d", dungeon->GetGlobalLight());
                    messages->AddString(str);
                    // show map light
                    dungeon->DebugMapLight();
                    */
                    diffuse_light=!diffuse_light;
                    dungeon->SetDiffuseLight(diffuse_light);
                    dungeon->CalculateFOV(hero->GetPosX(), hero->GetPosY());
                    if(diffuse_light){
                        messages->AddString("Diffuse light: ^00ff00Enabled");
                    }else{
                        messages->AddString("Diffuse light: ^ffff00Disabled");
                    }
                    break;
                case SDLK_o:
                    hero->OpenDoor(dungeon);
                    break;
                case SDLK_g:
                    hero->PikUp(dungeon);
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
                if(hero->inventory->SelectItemByKey(Event->key.keysym.sym)){
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
                    case 'w':
                            hero->inventory->Equip();
                        break;
                    case 'e':
                            MyOGL::Log->puts("Eat/Drink action!\n");
                        break;
                    case 't':
                        hero->inventory->Unequip(); // UnEquip current selected item
                        break;
                    case 'd':
                        MyOGL::Log->puts("Drop action!\n");
                        break;
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


void OnMouseMove(int x, int y, int xrel, int yrel, bool l_button, bool r_button, bool m_button){
    if(ActiveWindow==gwMain){
        int dungeon_area_w=dungeon->GetViewportWidth()*32; // tile seize - 32poxels
        int dungeon_area_h=dungeon->GetViewportHeight()*32;
        if(x<dungeon_area_w && y<dungeon_area_h){ // mouse in tiles area
            mouse_on_tile=true;
            mouse_on_tile_x=x/32;
            mouse_on_tile_y=y/32;
        }else{
            mouse_on_tile=false;
        }
    }else{
        mouse_on_tile=false;
    }

}

void OnWindowResize(unsigned int Width, unsigned int Height){
    // resize dungeon Viewport
//    printf("w: %d h: %d\n",Width, Height);
    int new_vp_h=(Height - Height % 32)/32;
    dungeon->SetViewportSize(new_vp_h, new_vp_h);
    dungeon->SetViewportToTarget(hero->GetPosX(), hero->GetPosY());
    dungeon->CalculateFOV(hero->GetPosX(), hero->GetPosY());
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

    //Log->printf("FOV size: %d Int Size: %d CharSize: %d\n",sizeof(sMapFovField),sizeof(int),sizeof(char));

    App->SetWinIcon("data/ico.bmp");
    // set callback functions
    App->OnRender=OnRender;
    App->OnLoop=OnLoop;
    App->OnEvent=OnEvent;
    App->OnWindowResize=OnWindowResize;
    App->OnMouseMove=OnMouseMove;

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
    dungeon->SetSeeOnlyWithLight(cfg_file->GetParamValue("see_only_with_light",1));
    dungeon->SetDiffuseLight(cfg_file->GetParamValue("enable_diffuse_light",1));
    diffuse_light=cfg_file->GetParamValue("enable_diffuse_light",1);
    // create herro

    MyOGL::Vector2i pos=dungeon->GetStartPosition();

    hero=new CHero();
    hero->SetMovementDelay(cfg_file->GetParamValue("movement_delay",50));
    hero->SetAutoopenDoors(cfg_file->GetParamValue("autoopen_doors",1));
    hero->SetPosition(pos.x, pos.y);
    hero_sprite=new CHudSprite(tiles_texture);
    hero_sprite->SetUVPixelCoords(0,128,32,32);
    hero_sprite->SetSize(32,32);
    hero->SetSprite(hero_sprite);

    hero->inventory->SetItemTileset(Tileset); // Items tileset
    hero->inventory->SetFont(font);
    hero->inventory->AddItem(100,1);
    hero->inventory->AddItem(101,1);
    hero->inventory->AddItem(102,1);
    hero->inventory->AddItem(103,1);
    hero->inventory->AddItem(104,1);

    Log->puts("Minimum map tile size: %d Bytes\n",sizeof(sMapField));

    ui_tile_info_panel=new CUICellInfo(font);

    messages=new MyOGL::CTextBox(font);
    // right from dungeon vievport
    messages->SetViewPort(dungeon->GetViewportWidth()*32, 50, Render->GetWidth()-dungeon->GetViewportWidth()*32, Render->GetHeight()-50);
    messages->AddString("^ffff00MyRLG");
    messages->AddString("Это тест окна текстовых строк со скролом.");
    messages->AddString("Можно прямо в тексте ^00ff00менять цвет^ffffff на лету.\nДля этого есть спец символы: ^^^ff0000rr^00ff00gg^0000ffbb^ffffff, где:\n^ff0000rr^ffffff - красная составляющая цвета,\n^00ff00gg^ffffff - зеленая, \n^0000ffbb^ffffff - синяя.");
    messages->AddString("Строка может быть ^ffff00ну очень^ffffff длинной, тогда будет работать ^00ffffавтоматический перенос^ffffff на новую строку!");
    messages->AddString(".");
    messages->AddString("..");
    messages->AddString("...");

    dungeon->SetViewportToTarget(hero->GetPosX(),hero->GetPosY());
    dungeon->CalculateFOV(hero->GetPosX(),hero->GetPosY());

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
    delete hero_sprite;
    delete hero;
    delete ui_tile_info_panel;

    printf("Normal Exit\n");
    return 0;
}
