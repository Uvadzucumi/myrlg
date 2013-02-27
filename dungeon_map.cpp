#include "dungeon_map.h"

#include <math.h>
#include "game.h"

#include<SDL/SDL.h>

void CDungeonLevel::NewGridDungeon(int grid_w, int grid_h, int rooms){
    CDungeonGeneration dlg(grid_w, grid_h, rooms);   // grid_w, grid_h, rooms
    dlg.Generate(m_Map);
};

void CDungeonLevel::CalculateFOV(int x_pos, int y_pos, int distance){
    m_fov->Calculate(x_pos, y_pos, m_Map, 3); // 3 - herro light size
    m_Map->CalculateMapLight(m_fov,m_ViewPort.left, m_ViewPort.top, m_ViewPort.width, m_ViewPort.height);
    // apply viewed fields (vieved+light)
    m_fov->ApplyOnMap(m_Map);
}

// Update map. change animated tiles and etc.
void CDungeonLevel::Update(double DeltaTime){
    m_Map->Update(m_fov, DeltaTime);
}

void CDungeonLevel::Render(){
    int dy,dx;
    int pos_x, pos_y;
    pos_y=0, pos_x=0;
    //CalculateLight();
    int vp_y,vp_x;
    unsigned int tile_light;
    for(vp_y=0,dy=m_ViewPort.top; dy<m_ViewPort.top+m_ViewPort.height; dy++, pos_y+=32,vp_y++){
        glTranslatef(-pos_x,0,0);
        for(vp_x=0,dx=m_ViewPort.left, pos_x=0;dx<m_ViewPort.left+m_ViewPort.width;dx++,pos_x+=32,vp_x++){
            if(m_Map->IsViewed(dx,dy)){
                if(m_fov->IsVisible(dx,dy)){
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
};
