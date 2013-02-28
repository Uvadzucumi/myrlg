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
#include "myogl/ui/panel.h"

//#include "myogl/frame_buffer.h"

CHudSprite *herro_sprite;
CTextBox *messages;
CUIPanel *ui_panel;

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
    // herro render
    glPushMatrix();
    glTranslatef((herro->GetPosX()-dungeon->GetViewportLeft())*32,(herro->GetPosY()-dungeon->GetViewportTop())*32,0);
    herro->Render();
    glPopMatrix();

    if(ActiveWindow==gwInventory){
        herro->inventory->Render();
    }else if(ActiveWindow==gwInventoryItemDescription){
        herro->inventory->RenderItemDetail();
    }

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
        // get tile info
        int map_tile_x=dungeon->GetViewportLeft()+mouse_on_tile_x;
        int map_tile_y=dungeon->GetViewportTop()+mouse_on_tile_y;
        sMapField field=dungeon->GetMapFiled(map_tile_x, map_tile_y);
        // display tile information box
        if(!field.viewed){
            ui_panel=new CUIPanel(mouse_pos.x+10, mouse_pos.y+10, 200, 35);
        }else{
            ui_panel=new CUIPanel(mouse_pos.x+10, mouse_pos.y+10, 280, 180);
        }
        ui_panel->Render();
        delete ui_panel;

        font_color.r=0; font_color.g=1,font_color.b=0, font_color.a=1;
        sprintf(tmp,"Координаты: ^ffffff%d, %d", map_tile_x, map_tile_y);
        font->PrintAt(mouse_pos.x+12,mouse_pos.y+10,tmp,font_color);
        if(!field.viewed){ // unknown field
            sprintf(tmp,"Местность: ^ffffffНеизвестная");
            font->PrintAt(mouse_pos.x+12,mouse_pos.y+24,tmp,font_color);
        }else{
            sprintf(tmp,"Местность: ^ffffff%s",GetTileNameByType(field.tile_type));
            font->PrintAt(mouse_pos.x+12,mouse_pos.y+24,tmp,font_color);
            CFOV *fov=dungeon->GetFOV();
            sMapFovField *fov_field=fov->GetFovField(map_tile_x, map_tile_y);
            if(fov_field){
                sprintf(tmp,"Интенсивность света: ^ffffff%d",fov_field->distance);
                font->PrintAt(mouse_pos.x+12,mouse_pos.y+24+14,tmp,font_color);
                font->PrintAt(mouse_pos.x+12,mouse_pos.y+24+14*2,"Вектор взгляда:",font_color);
                font_color.Set(0,1,1,1);
                sprintf(tmp,"north: ^ffffff%d",fov_field->north);
                font->PrintAt(mouse_pos.x+32,mouse_pos.y+24+14*3,tmp,font_color);
                sprintf(tmp,"south: ^ffffff%d",fov_field->south);
                font->PrintAt(mouse_pos.x+32,mouse_pos.y+24+14*4,tmp,font_color);
                sprintf(tmp,"east: ^ffffff%d",fov_field->east);
                font->PrintAt(mouse_pos.x+32,mouse_pos.y+24+14*5,tmp,font_color);
                sprintf(tmp,"west: ^ffffff%d",fov_field->west);
                font->PrintAt(mouse_pos.x+32,mouse_pos.y+24+14*6,tmp,font_color);
                sprintf(tmp,"north-east: ^ffffff%d",fov_field->north_east);
                font->PrintAt(mouse_pos.x+32,mouse_pos.y+24+14*7,tmp,font_color);
                sprintf(tmp,"north-west: ^ffffff%d",fov_field->north_west);
                font->PrintAt(mouse_pos.x+32,mouse_pos.y+24+14*8,tmp,font_color);
                sprintf(tmp,"south-east: ^ffffff%d",fov_field->south_east);
                font->PrintAt(mouse_pos.x+32,mouse_pos.y+24+14*9,tmp,font_color);
                sprintf(tmp,"south-west: ^ffffff%d",fov_field->south_west);
                font->PrintAt(mouse_pos.x+32,mouse_pos.y+24+14*10,tmp,font_color);

            }else{
                font->PrintAt(mouse_pos.x+12,mouse_pos.y+24+14,"FOV field: ^ffffffNULL",font_color);
            }
        }

    }// end show tile info window

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
        if(App->IsKeyPressed(SDLK_i)){  // goto inventory window
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
                    // show map light
                    dungeon->DebugMapLight();
                    break;
                case SDLK_o:
                    herro->OpenDoor(dungeon);
                    break;
                case SDLK_g:
                    herro->PikUp(dungeon);
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
                    case 'w':
                            herro->inventory->Equip();
                        break;
                    case 'e':
                            MyOGL::Log->puts("Eat/Drink action!\n");
                        break;
                    case 't':
                        herro->inventory->Unequip(); // UnEquip current selected item
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
    dungeon->SetViewportToTarget(herro->GetPosX(), herro->GetPosY());
    dungeon->CalculateFOV(herro->GetPosX(), herro->GetPosY());
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

    Log->printf("FOV size: %d Int Size: %d CharSize: %d\n",sizeof(sMapFovField),sizeof(int),sizeof(char));

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
    // create herro

    MyOGL::Vector2i pos=dungeon->GetStartPosition();

    herro=new CHerro();
    herro->SetMovementDelay(cfg_file->GetParamValue("movement_delay",50));
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
    messages->AddString("^ffff00MyRLG");
    messages->AddString("Это тест окна текстовых строк со скролом.");
    messages->AddString("Можно прямо в тексте ^00ff00менять цвет^ffffff на лету.\nДля этого есть пец символы: ^^^ff0000rr^00ff00gg^0000ffbb^ffffff, где:\n^ff0000rr^ffffff - красная составляющая цвета,\n^00ff00gg^ffffff - зеленая, \n^0000ffbb^ffffff - синяя.");
    messages->AddString("Строка может быть ^ffff00ну очень^ffffff длинной, тогда будет работать ^00ffffавтоматический перенос^ffffff на новую строку!");
    messages->AddString(".");
    messages->AddString("..");
    messages->AddString("...");

    dungeon->SetViewportToTarget(herro->GetPosX(),herro->GetPosY());
    dungeon->CalculateFOV(herro->GetPosX(),herro->GetPosY());

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
