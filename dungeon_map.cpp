#include "dungeon_map.h"

#include <math.h>
#include "game.h"

#include<SDL/SDL.h>

void CDungeonLevel::NewGridDungeon(int grid_w, int grid_h, int rooms){
    CDungeonGeneration dlg(grid_w, grid_h, rooms);   // grid_w, grid_h, rooms
    dlg.Generate(m_Map);
};


void CDungeonLevel::CalculateLOS(int x_pos, int y_pos, int distance){
// convert to local coordinates
    x_pos-=m_ViewPort.left;
    y_pos-=m_ViewPort.top;

    //int i,j;
    //unsigned int map_x, map_y;   // los array to map array coords
    // clear LOS data
    if(!distance) distance=m_ViewPort.width/2;
        // clear old LOS
/*        for(i=m_ViewPort.left;i<(m_ViewPort.left+m_ViewPort.width);i++){
            for(j=m_ViewPort.top;j<(m_ViewPort.top+m_ViewPort.height);j++){
                m_Map[i][j].visible=false;
            }
        }
*/
        // clear full map, TODO: remember and clean last filled LOS field
    /*
    for(i=0;i<m_map_width;i++){
        for(j=0;j<m_map_height;j++){
            m_Map->GetMap()[i][j].visible=false;
        }
    }
    */
    // clear VP data
    int vp_x, vp_y;
    for(vp_x=0;vp_x<m_ViewPort.width;vp_x++){
        for(vp_y=0;vp_y<m_ViewPort.height;vp_y++){
            m_light_intensivity[vp_x+vp_y*m_ViewPort.width].light=0;
            m_light_intensivity[vp_x+vp_y*m_ViewPort.width].is_visible=false;
            m_light_intensivity[vp_x+vp_y*m_ViewPort.width].north=false;
            m_light_intensivity[vp_x+vp_y*m_ViewPort.width].south=false;
            m_light_intensivity[vp_x+vp_y*m_ViewPort.width].east=false;
            m_light_intensivity[vp_x+vp_y*m_ViewPort.width].west=false;
        }
    }

    // check all tiles in viewport
        //distance*=distance; // distance^2 - for skip sqrt calculation
    for(vp_x=0; vp_x<m_ViewPort.width; vp_x++){    // x
        for(vp_y=0; vp_y<m_ViewPort.height; vp_y++){   // y
        // skip visible
            //if(m_Map->IsVisible(i,j)) continue;
            if(m_light_intensivity[vp_x+vp_y*m_ViewPort.width].is_visible) continue;
            // skip far tiles
            int distance_to_tile = sqrt(((vp_x - x_pos) * (vp_x - x_pos)) + ((vp_y - y_pos) * (vp_y - y_pos)));
                // skip far tiles
            if(distance < distance_to_tile){ continue;}
            if(m_Map->LineOfSight(vp_x+m_ViewPort.left,vp_y+m_ViewPort.top,x_pos+m_ViewPort.left,y_pos+m_ViewPort.top)){
                    //m_Map->GetMap()[i][j].visible=true;
                    m_light_intensivity[vp_x+vp_y*m_ViewPort.width].is_visible=true;
                    m_Map->SetViewed(vp_x+m_ViewPort.left,vp_y+m_ViewPort.top,true);
            }
        }
    }

    int x,y,map_x,map_y;
    // post processing

    for(vp_x=0; vp_x<m_ViewPort.width; vp_x++){
        for(vp_y=0;vp_y<m_ViewPort.height;vp_y++){

            map_x=vp_x+m_ViewPort.left;
            map_y=vp_y+m_ViewPort.top;

            if(!m_Map->IsSkipLight(map_x,map_y) && !m_light_intensivity[vp_x+vp_y*m_ViewPort.width].is_visible){ // invisible block light tile
                // north
                x = map_x;
                y = map_y - 1;
                if(x>0 && x<m_map_width && vp_y>0){
                    if(y>0 && y<m_map_height){
                        if(m_Map->IsSkipLight(x,y) && m_light_intensivity[vp_x+(vp_y-1)*m_ViewPort.width].is_visible){  // if near viewed floor
                            //m_Map->GetMap()[i][j].visible=true;
                            m_light_intensivity[vp_x+vp_y*m_ViewPort.width].is_visible=true;
                            //m_Map->GetMap()[map_x][map_y].viewed=true;
                            m_Map->SetViewed(map_x,map_y,true);
                        }
                    }
                }


                // south
                x = map_x;
                y = map_y + 1;
                if(x>0 && x<m_map_width && vp_y < (m_ViewPort.width-1)){
                    if(y>0 && y<m_map_height){
                        if(m_Map->IsSkipLight(x,y) && m_light_intensivity[vp_x+(vp_y+1)*m_ViewPort.width].is_visible){  // if near viewed floor
                            //m_Map->GetMap()[i][j].visible=true;
                            m_light_intensivity[vp_x+vp_y*m_ViewPort.width].is_visible=true;
                            //m_Map->GetMap()[map_x][map_y].viewed=true;
                            m_Map->SetViewed(map_x,map_y,true);
                        }
                    }
                }
                // west
                x = map_x - 1;
                y = map_y;
                if(x>0 && x<m_map_width && vp_x>0){
                    if(y>0 && y<m_map_height){
                        if(m_Map->IsSkipLight(x,y) && m_light_intensivity[vp_x-1+vp_y*m_ViewPort.width].is_visible){  // if near viewed floor
                            //m_Map->GetMap()[i][j].visible=true;
                            m_light_intensivity[vp_x+vp_y*m_ViewPort.width].is_visible=true;
                            //m_Map->GetMap()[map_x][map_y].viewed=true;
                            m_Map->SetViewed(map_x,map_y,true);
                        }
                    }
                }
                // east
                x = map_x + 1;
                y = map_y;
                if(x>0 && x<m_map_width && vp_x<(m_ViewPort.width-1)){
                    if(y>0 && y<m_map_height){
                        if(m_Map->IsSkipLight(x,y) && m_light_intensivity[vp_x+1+vp_y*m_ViewPort.width].is_visible){  // if near viewed floor
                            m_light_intensivity[vp_x+vp_y*m_ViewPort.width].is_visible=true;
                            //m_Map->GetMap()[map_x][map_y].viewed=true;
                            m_Map->SetViewed(map_x,map_y,true);
                        }
                    }
                }

                // nort-west

                x = map_x - 1;
                y = map_y - 1;
                if(x>0 && x<m_map_width && vp_x>0 && vp_y>0){
                    if(y>0 && y<m_map_height){
                        if(m_Map->IsSkipLight(x,y) && m_light_intensivity[vp_x-1+(vp_y-1)*m_ViewPort.width].is_visible &&         // if near viewed floor
                            !m_Map->IsSkipLight(x,map_y) && !m_Map->IsSkipLight(map_x,y)){    // and 2 block
                            //m_Map->GetMap()[i][j].visible=true;
                            m_light_intensivity[vp_x+vp_y*m_ViewPort.width].is_visible=true;
                            //m_Map->GetMap()[map_x][map_y].viewed=true;
                            m_Map->SetViewed(map_x,map_y,true);
                        }
                    }
                }

                // north-east
                x = map_x + 1;
                y = map_y - 1;
                if(x>0 && x<m_map_width && vp_x<(m_ViewPort.width-1) && vp_y>0){
                    if(y>0 && y<m_map_height){
                        if(m_Map->IsSkipLight(x,y) && m_light_intensivity[vp_x+1+(vp_y-1)*m_ViewPort.width].is_visible  &&         // if near viewed floor
                            !m_Map->IsSkipLight(x, map_y) && !m_Map->IsSkipLight(map_x,y)){    // and 2 block
                            //m_Map->GetMap()[i][j].visible=true;
                            m_light_intensivity[vp_x+vp_y*m_ViewPort.width].is_visible=true;
                            //m_Map->GetMap()[map_x][map_y].viewed=true;
                            m_Map->SetViewed(map_x,map_y,true);
                        }
                    }
                }
                // south-east
                x = map_x + 1;
                y = map_y + 1;
                if(x>0 && x<m_map_width && vp_x<(m_ViewPort.width-1) && vp_y<(m_ViewPort.height-1)){
                    if(y>0 && y<m_map_height){
                        if(m_Map->IsSkipLight(x,y) && m_light_intensivity[vp_x+1+(vp_y+1)*m_ViewPort.width].is_visible &&         // if near viewed floor
                            !m_Map->IsSkipLight(x,map_y) && !m_Map->IsSkipLight(map_x,y)){    // and 2 block
                            m_light_intensivity[vp_x+vp_y*m_ViewPort.width].is_visible=true;
                            //m_Map->GetMap()[map_x][map_y].viewed=true;
                            m_Map->SetViewed(map_x,map_y,true);
                        }
                    }
                }
                //south-west
                x = map_x - 1;
                y = map_y + 1;
                if(x>0 && x<m_map_width && vp_x>0 && vp_y<(m_ViewPort.height-1)){
                    if(y>0 && y<m_map_height){
                        if(m_Map->IsSkipLight(x,y) && m_light_intensivity[vp_x-1+(vp_y+1)*m_ViewPort.width].is_visible &&         // if near viewed floor
                            !m_Map->IsSkipLight(x,map_y) && !m_Map->IsSkipLight(map_x,y)){    // and near 2 block
                                m_light_intensivity[vp_x+vp_y*m_ViewPort.width].is_visible=true;
                                //m_Map->GetMap()[map_x][map_y].viewed=true;
                                m_Map->SetViewed(map_x,map_y,true);
                        }
                    }
                }

            } // end checking invisible tile

        }
    }  // end post processing fixes

    //Log->puts("%d ticks\n",SDL_GetTicks()-start);
}

// set global light to ViewPort,TODO: add normal light calculation
void CDungeonLevel::CalculateLight(){
    // check viewport and set global light
/*
    if(m_light_changed){
        m_Map->CalculateMapLight(m_ViewPort.left, m_ViewPort.top, m_ViewPort.width, m_ViewPort.height);
        int dx, dy;
        for(dy=m_ViewPort.top; dy<m_ViewPort.top+m_ViewPort.height; dy++){
            for(dx=m_ViewPort.left;dx<m_ViewPort.left+m_ViewPort.width;dx++){
                //m_Map->GetMap()[dx][dy].light=m_global_light;
                if(m_Map->GetMap()[dx][dy].light==0){
                    m_Map->GetMap()[dx][dy].light=1;
                }
            }
        }
        m_light_changed=false;
    }
*/
        m_Map->CalculateMapLight(m_light_intensivity,m_ViewPort.left, m_ViewPort.top, m_ViewPort.width, m_ViewPort.height);

}

// Update map. change animated tiles and etc.
void CDungeonLevel::Update(double DeltaTime){
    m_Map->Update(DeltaTime);
}

void CDungeonLevel::Render(){
    int dy,dx;
    int pos_x, pos_y;
    pos_y=0, pos_x=0;
    CalculateLight();
    int vp_y,vp_x;
    unsigned int tile_light;
    for(vp_y=0,dy=m_ViewPort.top; dy<m_ViewPort.top+m_ViewPort.height; dy++, pos_y+=32,vp_y++){
        glTranslatef(-pos_x,0,0);
        for(vp_x=0,dx=m_ViewPort.left, pos_x=0;dx<m_ViewPort.left+m_ViewPort.width;dx++,pos_x+=32,vp_x++){
            //if(m_Map[dx][dy].visible){
            if(m_Map->IsViewed(dx,dy)){
            //if(m_Map[dx][dy].layer[0]!=255){
                //m_tileset->RenderAt(pos_x,pos_y,m_Map[dx][dy].layer[0],&LightMaterials[m_Map[dx][dy].light]);
                if(m_light_intensivity[dx-m_ViewPort.left+(dy-m_ViewPort.top)*m_ViewPort.width].is_visible){
                    // get light
                    tile_light=m_light_intensivity[dx-m_ViewPort.left+(dy-m_ViewPort.top)*m_ViewPort.width].light;
                    //tile_light=m_Map->GetMap()[dx][dy].light;

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
