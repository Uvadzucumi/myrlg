
#include "fov.h"
#include <math.h> // for sqrt

#include "level_map.h"

void CFOV::SetDistance(int tile_x, int tile_y, int x_pos, int y_pos, int set_distance){
    if(set_distance){
        int distance_to_tile = sqrt(((tile_x - x_pos) * (tile_x - x_pos)) + ((tile_y - y_pos) * (tile_y - y_pos)));
        if(set_distance-distance_to_tile>0){
            m_fov_field[tile_x-m_left_top.x+(tile_y-m_left_top.y)*m_fov_size].distance=set_distance-distance_to_tile;
        }
    }
}

// claculate FOV for coords
void CFOV::Calculate(int x_pos, int y_pos, CLevelMap *m_Map, int set_distance){
// set FOV window coords
    m_left_top.x=x_pos-m_distance;
    m_left_top.y=y_pos-m_distance;

    int map_width=m_Map->GetWidth();
    int map_height=m_Map->GetHeight();

    if(m_left_top.x<0) m_left_top.x=0;
    if(m_left_top.y<0) m_left_top.y=0;
    m_right_bottom.x=m_left_top.x+m_fov_size-1;
    m_right_bottom.y=m_left_top.y+m_fov_size-1;
    if(m_right_bottom.x>=map_width) m_right_bottom.x=map_width-1;
    if(m_right_bottom.y>=map_height) m_right_bottom.y=map_height-1;

    int vp_x, vp_y;
    int fov_index;

    ClearOldData();

    // check all tiles in viewport
    for(vp_x=m_left_top.x; vp_x <= m_right_bottom.x; vp_x++){    // x
        for(vp_y=m_left_top.y; vp_y <= m_right_bottom.y; vp_y++){   // y
            // skip visible
            fov_index=(vp_x-m_left_top.x)+(vp_y-m_left_top.y)*m_fov_size;
            if(m_fov_field[fov_index].is_visible) continue;
            // if in LOS - set visible
            if(m_Map->LineOfSight(vp_x, vp_y, x_pos, y_pos)){
                SetDistance(vp_x, vp_y, x_pos, y_pos, set_distance);
                m_fov_field[fov_index].is_visible=true;
            }
        }
    }

    // postprocessing
    int x,y;
    for(vp_y=m_left_top.y; vp_y <= m_right_bottom.y; vp_y++){   // y
        for(vp_x=m_left_top.x; vp_x <= m_right_bottom.x; vp_x++){    // x
            fov_index=(vp_x-m_left_top.x)+(vp_y-m_left_top.y)*m_fov_size;
            if(!m_Map->IsSkipLight(vp_x, vp_y) && !IsVisible(vp_x, vp_y)){ // invisible blocked tile
                // north - empty field
                y = vp_y - 1;
                if(y >= m_left_top.y){
                    if(m_Map->IsSkipLight(vp_x,y) && IsVisible(vp_x,y)){
                        SetDistance(vp_x, vp_y, x_pos, y_pos, set_distance);
                        m_fov_field[fov_index].is_visible=true;
                        m_fov_field[fov_index].north=true;
                        //continue;
                    }
                }
                // south - empty visible field
                y = vp_y + 1;
                if(y <= m_right_bottom.y){
                    if(m_Map->IsSkipLight(vp_x,y) && IsVisible(vp_x,y)){
                        SetDistance(vp_x, vp_y, x_pos, y_pos, set_distance);
                        m_fov_field[fov_index].is_visible=true;
                        m_fov_field[fov_index].south=true;
                        //continue;
                    }
                }
                // west - empty visible field
                x = vp_x - 1;
                if(x >= m_left_top.x){
                    if(m_Map->IsSkipLight(x,vp_y) && IsVisible(x,vp_y)){
                        SetDistance(vp_x, vp_y, x_pos, y_pos, set_distance);
                        m_fov_field[fov_index].is_visible=true;
                        m_fov_field[fov_index].west=true;
                        //continue;
                    }
                }
                // east - empty visible field
                x = vp_x + 1;
                if(x <= m_right_bottom.x){
                    if(m_Map->IsSkipLight(x,vp_y) && IsVisible(x,vp_y)){
                        SetDistance(vp_x, vp_y, x_pos, y_pos, set_distance);
                        m_fov_field[fov_index].is_visible=true;
                        m_fov_field[fov_index].east=true;
                        //continue;
                    }
                }

                // nort-west
                if(x_pos < vp_x && y_pos < vp_y){
                    x = vp_x - 1;
                    y = vp_y - 1;
                    if( x >= m_left_top.x && y >= m_left_top.y){
                        if(m_Map->IsSkipLight(x,y) && IsVisible(x,y)){
                            SetDistance(vp_x, vp_y, x_pos, y_pos, set_distance);
                            m_fov_field[fov_index].is_visible=true;
                            m_fov_field[fov_index].north_west=true;
                        //continue;
                        }
                    }
                }
                // nort-east
                if(x_pos > vp_x && y_pos < vp_y){
                    x = vp_x + 1;
                    y = vp_y - 1;
                    if(x <= m_right_bottom.x && y >= m_left_top.y){
                        if(m_Map->IsSkipLight(x,y) && IsVisible(x,y)){
                            SetDistance(vp_x, vp_y, x_pos, y_pos, set_distance);
                            m_fov_field[fov_index].is_visible=true;
                            m_fov_field[fov_index].north_east=true;
                        //continue;
                        }
                    }
                }
                // south-west
                if(x_pos < vp_x && y_pos > vp_y){
                    x = vp_x - 1;
                    y = vp_y + 1;
                    if( x >= m_left_top.x && y <= m_right_bottom.y ){
                        if(m_Map->IsSkipLight(x,y) && IsVisible(x,y)){
                            SetDistance(vp_x, vp_y, x_pos, y_pos, set_distance);
                            m_fov_field[fov_index].is_visible=true;
                            m_fov_field[fov_index].south_west=true;
                        //continue;
                        }
                    }
                }
                // south-east
                if(x_pos > vp_x && y_pos > vp_y){
                    x = vp_x + 1;
                    y = vp_y + 1;
                    if( x <= m_right_bottom.x && y <= m_right_bottom.y ){
                        if(m_Map->IsSkipLight(x,y) && IsVisible(x,y)){
                            SetDistance(vp_x, vp_y, x_pos, y_pos, set_distance);
                            m_fov_field[fov_index].is_visible=true;
                            m_fov_field[fov_index].south_east=true;
                        }
                    }
                }

            }
        }
    }
}

// apply current visible field to map viewed
void CFOV::ApplyOnMap(CLevelMap *map, bool only_light_visible, CDungeonMinimap *minimap){
    int index;
    for(int y=m_left_top.y;  y<=m_right_bottom.y; y++ ){
        for(int x=m_left_top.x; x<=m_right_bottom.x; x++){
            index=(y-m_left_top.y)*m_fov_size+(x-m_left_top.x);
            if(m_fov_field[index].is_visible &&
              (!only_light_visible || (only_light_visible && m_fov_field[index].distance))){
                map->SetViewed(x,y,true);
                if(minimap){
                    minimap->SetMapPoint(x,y,map->GetTileType(x,y));
                }
            }
        }
    }
    for(int y=m_left_top.y;  y<=m_right_bottom.y; y++ ){
        for(int x=m_left_top.x; x<=m_right_bottom.x; x++){
            map->LandPostprocessing(this, x,y);
        }
    }
}

void CFOV::CalculateDiffuse(int depth){
  // check all fov fields and add lights in nearest field
    int dx, dy;
    depth=3;
    for(int n=0; n<depth; n++){
    for(int y=m_left_top.y;  y<=m_right_bottom.y; y++ ){
        for(int x=m_left_top.x; x<=m_right_bottom.x; x++){
            int light=GetDistance(x,y)/2;
            if(!light) continue;
            if(IsVisible(x,y)){

                if(x!=m_left_top.x){
                    // west
                    dx=x-1; dy=y;
                    if(IsVisible(dx,dy) && light>GetDistance(dx,dy)){
                    SetDistance(dx,dy,light);
                    }
                    if(y!=m_left_top.y){
                        // north-west
                        dx=x-1; dy=y-1;
                        if(IsVisible(dx,dy) && light>GetDistance(dx,dy)){
                            SetDistance(dx,dy,light);
                        }
                    }
                    if(y!=m_right_bottom.y){
                    // south-west
                        dx=x-1; dy=y+1;
                        if(IsVisible(dx,dy) && light>GetDistance(dx,dy)){
                            SetDistance(dx,dy,light);
                        }
                    }
                }
                if(x!=m_right_bottom.x){
                    // east
                    dx=x+1; dy=y;
                    if(IsVisible(dx,dy) && light>GetDistance(dx,dy)){
                        SetDistance(dx,dy,light);
                    }
                    if(y!=m_left_top.y){
                        // north-east
                        dx=x+1; dy=y-1;
                        if(IsVisible(dx,dy) && light>GetDistance(dx,dy)){
                            SetDistance(dx,dy,light);
                        }
                    }
                    if(y!=m_right_bottom.y){
                        // south-east
                        dx=x+1; dy=y+1;
                        if(IsVisible(dx,dy) && light>GetDistance(dx,dy)){
                            SetDistance(dx,dy,light);
                        }
                    }
                }
                // north
                if(y!=m_left_top.y){
                    dx=x; dy=y-1;
                    if(IsVisible(dx,dy) && light>GetDistance(dx,dy)){
                        SetDistance(dx,dy,light);
                    }
                }
                // south
                if(y!=m_right_bottom.y){
                    dx=x; dy=y+1;
                    if(IsVisible(dx,dy) && light>GetDistance(dx,dy)){
                        SetDistance(dx,dy,light);
                    }
                }
            }
        }
    }
    }
}
