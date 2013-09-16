#include "dungeon_map.h"

#include <math.h>
#include "game.h"


void CDungeonLevel::NewGridDungeon(int grid_w, int grid_h, int rooms){
    CDungeonGeneration dlg(grid_w, grid_h, rooms);   // grid_w, grid_h, rooms
    dlg.Generate(m_Map);
};

void CDungeonLevel::CalculateFOV(int x_pos, int y_pos, int distance){
    // hero FOV
    m_fov->Calculate(x_pos, y_pos, m_Map, 3); // 3 - herro light size
    // Apply lights to Hero FOV
    m_Map->CalculateMapLight(m_fov);
    // calculate diffuse lights (if need)
    if(m_diffuse_light){
        m_fov->CalculateDiffuse(1); // number of diffyse steps
    }
    // Apply Viewed fields from Hero FOV to Map Array
    m_fov->ApplyOnMap(m_Map, m_see_only_with_light, m_Minimap);
}

// Update map. change animated tiles and etc.
void CDungeonLevel::Update(double DeltaTime){
    m_Map->Update(m_fov, DeltaTime);
}

void CDungeonLevel::Render(){
    int dy,dx;
    int pos_x, pos_y;
    pos_y=0, pos_x=0;
    unsigned int tile_light;
    MyOGL::GL.Enable(GL_TEXTURE_2D);
    for(dy=m_ViewPort.top; dy<m_ViewPort.top+m_ViewPort.height; dy++, pos_y+=32){
        glTranslatef(-pos_x,0,0);
        for(dx=m_ViewPort.left, pos_x=0;dx<m_ViewPort.left+m_ViewPort.width;dx++,pos_x+=32){
            if(m_Map->IsViewed(dx,dy)){
                if(m_fov->IsInArea(dx, dy) && m_fov->IsVisible(dx,dy)){
                    // get light
                    tile_light=m_fov->GetDistance(dx,dy);
                    m_tileset->Tile(m_Map->GetTileID(dx,dy,0))->Render(&LightMaterials[9-tile_light]);
                    if(m_Map->GetTileID(dx,dy,1)!=255){
                        m_tileset->Tile(m_Map->GetTileID(dx,dy,1))->Render(&LightMaterials[9-tile_light]);
                    }
                    if(m_Map->GetTileID(dx,dy,2)!=255){
                        m_tileset->Tile(m_Map->GetTileID(dx,dy,2))->Render(&LightMaterials[9-tile_light]);
                    }
                    // TODO, show dropped items
                    // show monsters
                    //if(dx==Herro->GetPosX() && dy==Herro->GetPosY()){
                    //    m_tileset->RenderAt(0,0,tnHerro);
                    //}
                }else{  // viewed
                    m_tileset->Tile(m_Map->GetTileID(dx,dy,0))->Render(&LightMaterials[9]);
                    if(m_Map->GetTileID(dx,dy,1)!=255){
                        m_tileset->Tile(m_Map->GetTileID(dx,dy,1))->Render(&LightMaterials[9]);
                    }
                    if(m_Map->GetTileID(dx,dy,2)!=255){
                        m_tileset->Tile(m_Map->GetTileID(dx,dy,2))->Render(&LightMaterials[9]);
                    }
                }
            }
            glTranslatef(32, 0,0);
        }
        glTranslatef(0,32,0);
    }
    glTranslatef(-pos_x,-pos_y,0);
    // Render Hero
    glPushMatrix();
    glTranslatef((hero->GetPosX()-dungeon->GetViewportLeft())*32,(hero->GetPosY()-dungeon->GetViewportTop())*32,0);
    hero->Render();
    glPopMatrix();
    // Set MiniMap Matherial
    LightMaterials[0].Apply();
    m_Minimap->Render();
    // Render Hero Point on the map
    MyOGL::Render->SetColor(0,255,0);
    //MyOGL::Render->DrawQuad(600+hero->GetPosX(),hero->GetPosY(),3,3);
    MyOGL::Render->SetBlendMode(blNone);
    MyOGL::GL.Disable(GL_TEXTURE_2D);
    MyOGL::Render->DrawQuad(m_Minimap->GetX()+hero->GetPosX()-1,hero->GetPosY()-1,3,3);
    MyOGL::GL.Enable(GL_TEXTURE_2D);
};
