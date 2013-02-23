
#include "fov.h"
#include <math.h> // for sqrt

#include "level_map.h"

// claculate FOV for coords
void CFOV::Calculate(int x_pos, int y_pos, CLevelMap *m_Map, int set_distance){
// set FOV window coords
    m_left_top.x=x_pos-m_distance;
    m_left_top.y=y_pos-m_distance;

    int map_width=m_Map->GetWidth();
    int map_height=m_Map->GetHeight();
//
    if(m_left_top.x<0) m_left_top.x=0;
    if(m_left_top.y<0) m_left_top.y=0;
    m_right_bottom.x=m_left_top.x+m_fov_size-1;
    m_right_bottom.y=m_left_top.y+m_fov_size-1;
    if(m_right_bottom.x>=map_width) m_right_bottom.x=map_width-1;
    if(m_right_bottom.y>=map_height) m_right_bottom.y=map_height-1;

    Log->puts("start calculate FOV left_top: %d,%d ",m_left_top.x,m_left_top.y);
    Log->puts("right_bottom: %d,%d\n",m_right_bottom.x,m_right_bottom.y);
    Log->puts("see point: %d,%d\n",x_pos,y_pos);

    int vp_x, vp_y;
    int fov_index;
    ClearOldData();

//    int width=m_right_bottom.x-m_left_top.x;
//    int height=m_right_bottom.y-m_left_top.y;

    // check all tiles in viewport
    for(vp_x=m_left_top.x; vp_x <= m_right_bottom.x; vp_x++){    // x
        for(vp_y=m_left_top.y; vp_y <= m_right_bottom.y; vp_y++){   // y
            // skip visible
            fov_index=(vp_x-m_left_top.x)+(vp_y-m_left_top.y)*m_fov_size;
            if(m_fov_field[fov_index].is_visible) continue;
            // skip far tiles
            int distance_to_tile = sqrt(((vp_x - x_pos) * (vp_x - x_pos)) + ((vp_y - y_pos) * (vp_y - y_pos)));
            if(m_distance < distance_to_tile){ continue;}
            // if in LOS - set visible
            if(m_Map->LineOfSight(vp_x, vp_y, x_pos, y_pos)){
                if(set_distance){
                    if(set_distance-distance_to_tile>0){
                        m_fov_field[fov_index].distance=(set_distance-distance_to_tile)*3;
                    }
                }
                m_fov_field[fov_index].is_visible=true;
                m_Map->SetViewed(vp_x, vp_y, true);
            }
        }
    }

#if 0
    int x,y,map_x,map_y;
    // post processing

    for(vp_x=0; vp_x<m_fov_size; vp_x++){
        for(vp_y=0;vp_y<m_fov_size;vp_y++){

            map_x=vp_x+m_left_top.x;
            map_y=vp_y+m_left_top.y;
            if(map_x>=map_width) continue;
            if(map_y>=map_height) continue;

            if(!m_Map->IsSkipLight(map_x,map_y) && !m_fov_field[vp_x+vp_y*m_fov_size].is_visible){ // invisible block light tile
                // north - empty field
                x = map_x;
                y = map_y - 1;
                if(x>0 && x<map_width && vp_y>0){
                    if(y>0 && y<map_height){
                        if(m_Map->IsSkipLight(x,y) && m_fov_field[vp_x+(vp_y-1)*m_fov_size].is_visible){  // if near viewed floor
                            //m_Map->GetMap()[i][j].visible=true;
                            m_fov_field[vp_x+vp_y*m_fov_size].is_visible=true;
                            m_fov_field[vp_x+vp_y*m_fov_size].north=true;
                            //m_Map->GetMap()[map_x][map_y].viewed=true;
                            m_Map->SetViewed(map_x,map_y,true);
                        }
                    }
                }

                // south - empty looked field
                x = map_x;
                y = map_y + 1;
                if(x>0 && x<map_width && vp_y < (m_fov_size-1)){
                    if(y>0 && y<map_height){
                        if(m_Map->IsSkipLight(x,y) && m_fov_field[vp_x+(vp_y+1)*m_fov_size].is_visible){  // if near viewed floor
                            //m_Map->GetMap()[i][j].visible=true;
                            m_fov_field[vp_x+vp_y*m_fov_size].is_visible=true;
                            m_fov_field[vp_x+vp_y*m_fov_size].south=true;
                            //m_Map->GetMap()[map_x][map_y].viewed=true;
                            m_Map->SetViewed(map_x,map_y,true);
                        }
                    }
                }
                // west - empty looked field
                x = map_x - 1;
                y = map_y;
                if(x>0 && x<map_width && vp_x>0){
                    if(y>0 && y<map_height){
                        if(m_Map->IsSkipLight(x,y) && m_fov_field[vp_x-1+vp_y*m_fov_size].is_visible){  // if near viewed floor
                            //m_Map->GetMap()[i][j].visible=true;
                            m_fov_field[vp_x+vp_y*m_fov_size].is_visible=true;
                            m_fov_field[vp_x+vp_y*m_fov_size].west=true;
                            m_Map->SetViewed(map_x,map_y,true);
                        }
                    }
                }
                // east - empty looked field
                x = map_x + 1;
                y = map_y;
                if(x>0 && x<map_width && vp_x<(m_fov_size-1)){
                    if(y>0 && y<map_height){
                        if(m_Map->IsSkipLight(x,y) && m_fov_field[vp_x+1+vp_y*m_fov_size].is_visible){  // if near viewed floor
                            m_fov_field[vp_x+vp_y*m_fov_size].is_visible=true;
                            m_fov_field[vp_x+vp_y*m_fov_size].east=true;
                            m_Map->SetViewed(map_x,map_y,true);
                        }
                    }
                }

                // nort-west

                x = map_x - 1;
                y = map_y - 1;
                if(x>0 && x<map_width && vp_x>0 && vp_y>0){
                    if(y>0 && y<map_height){
                        if(m_Map->IsSkipLight(x,y) && m_fov_field[vp_x-1+(vp_y-1)*m_fov_size].is_visible &&         // if near viewed floor
                            !m_Map->IsSkipLight(x,map_y) && !m_Map->IsSkipLight(map_x,y)){    // and 2 block
                            m_fov_field[vp_x+vp_y*m_fov_size].is_visible=true;
                            //m_light_vp[vp_x+vp_y*m_ViewPort.width].north=true;
                            //m_light_vp[vp_x+vp_y*m_ViewPort.width].west=true;
                            m_Map->SetViewed(map_x,map_y,true);
                        }
                    }
                }

                // north-east
                x = map_x + 1;
                y = map_y - 1;
                if(x>0 && x<map_width && vp_x<(m_fov_size-1) && vp_y>0){
                    if(y>0 && y<map_height){
                        if(m_Map->IsSkipLight(x,y) && m_fov_field[vp_x+1+(vp_y-1)*m_fov_size].is_visible  &&         // if near viewed floor
                            !m_Map->IsSkipLight(x, map_y) && !m_Map->IsSkipLight(map_x,y)){    // and 2 block
                            //m_Map->GetMap()[i][j].visible=true;
                            m_fov_field[vp_x+vp_y*m_fov_size].is_visible=true;
                            //m_light_vp[vp_x+vp_y*m_ViewPort.width].north=true;
                            //m_light_vp[vp_x+vp_y*m_ViewPort.width].east=true;
                            m_Map->SetViewed(map_x,map_y,true);
                        }
                    }
                }
                // south-east
                x = map_x + 1;
                y = map_y + 1;
                if(x>0 && x<map_width && vp_x<(m_fov_size-1) && vp_y<(m_fov_size-1)){
                    if(y>0 && y<map_height){
                        if(m_Map->IsSkipLight(x,y) && m_fov_field[vp_x+1+(vp_y+1)*m_fov_size].is_visible &&         // if near viewed floor
                            !m_Map->IsSkipLight(x,map_y) && !m_Map->IsSkipLight(map_x,y)){    // and 2 block
                            m_fov_field[vp_x+vp_y*m_fov_size].is_visible=true;
                            //m_light_vp[vp_x+vp_y*m_ViewPort.width].south=true;
                            //m_light_vp[vp_x+vp_y*m_ViewPort.width].east=true;
                            m_Map->SetViewed(map_x,map_y,true);
                        }
                    }
                }
                //south-west
                x = map_x - 1;
                y = map_y + 1;
                if(x>0 && x<map_width && vp_x>0 && vp_y<(m_fov_size-1)){
                    if(y>0 && y<map_height){
                        if(m_Map->IsSkipLight(x,y) && m_fov_field[vp_x-1+(vp_y+1)*m_fov_size].is_visible &&         // if near viewed floor
                            !m_Map->IsSkipLight(x,map_y) && !m_Map->IsSkipLight(map_x,y)){    // and near 2 block
                                m_fov_field[vp_x+vp_y*m_fov_size].is_visible=true;
                            //m_light_vp[vp_x+vp_y*m_ViewPort.width].south=true;
                            //m_light_vp[vp_x+vp_y*m_ViewPort.width].west=true;
                                m_Map->SetViewed(map_x,map_y,true);
                        }
                    }
                }

            } // end checking invisible tile

        }
    }  // end post processing fixes

#else
    // new postprocessing
    int x,y;
    for(vp_y=m_left_top.y; vp_y <= m_right_bottom.y; vp_y++){   // y
        for(vp_x=m_left_top.x; vp_x <= m_right_bottom.x; vp_x++){    // x
            fov_index=(vp_x-m_left_top.x)+(vp_y-m_left_top.y)*m_fov_size;
            if(!m_Map->IsSkipLight(vp_x, vp_y) && !IsVisible(vp_x, vp_y)){ // invisible blocked tile
                // north - empty field
                y = vp_y - 1;
                if(y>=0){
                    if(m_Map->IsSkipLight(vp_x,y) && IsVisible(vp_x,y)){
                        m_Map->SetViewed(vp_x,vp_y,true);
                        m_fov_field[fov_index].is_visible=true;
                        m_fov_field[fov_index].north=true;
                        continue;
                    }
                }
                // south - empty visible field
                y = vp_y + 1;
                if(y<map_height){
                    if(m_Map->IsSkipLight(vp_x,y) && IsVisible(vp_x,y)){
                        m_Map->SetViewed(vp_x,vp_y,true);
                        m_fov_field[fov_index].is_visible=true;
                        m_fov_field[fov_index].south=true;
                        continue;
                    }
                }
                // west - empty visible field
                x = vp_x - 1;
                if(x>=0){
                    if(m_Map->IsSkipLight(x,vp_y) && IsVisible(x,vp_y)){
                        m_Map->SetViewed(vp_x,vp_y,true);
                        m_fov_field[fov_index].is_visible=true;
                        m_fov_field[fov_index].west=true;
                        continue;
                    }
                }
                // east - empty visible field
                x = vp_x + 1;
                if(x<map_width){
                    if(m_Map->IsSkipLight(x,vp_y) && IsVisible(x,vp_y)){
                        m_Map->SetViewed(vp_x,vp_y,true);
                        m_fov_field[fov_index].is_visible=true;
                        m_fov_field[fov_index].east=true;
                        continue;
                    }
                }
                // nort-west
                x = vp_x - 1;
                y = vp_y - 1;
                if( x >= 0 && y >= 0){
                    if(m_Map->IsSkipLight(x,y) && IsVisible(x,y)){
                        m_Map->SetViewed(vp_x,vp_y,true);
                        m_fov_field[fov_index].is_visible=true;
                        //m_fov_field[fov_index].west=true;
                        //m_fov_field[fov_index].north=true;
                        continue;
                    }
                }
                // nort-east
                x = vp_x + 1;
                y = vp_y - 1;
                if( x < map_width && y >= 0){
                    if(m_Map->IsSkipLight(x,y) && IsVisible(x,y)){
                        m_Map->SetViewed(vp_x,vp_y,true);
                        m_fov_field[fov_index].is_visible=true;
                        //m_fov_field[fov_index].east=true;
                        //m_fov_field[fov_index].north=true;
                        continue;
                    }
                }
                // south-west
                x = vp_x - 1;
                y = vp_y + 1;
                if( x >= 0 && y <= map_height ){
                    if(m_Map->IsSkipLight(x,y) && IsVisible(x,y)){
                        m_Map->SetViewed(vp_x,vp_y,true);
                        m_fov_field[fov_index].is_visible=true;
                        //m_fov_field[fov_index].west=true;
                        //m_fov_field[fov_index].south=true;
                        continue;
                    }
                }
                // south-east
                x = vp_x + 1;
                y = vp_y + 1;
                if( x < map_width && y <= map_height ){
                    if(m_Map->IsSkipLight(x,y) && IsVisible(x,y)){
                        m_Map->SetViewed(vp_x,vp_y,true);
                        m_fov_field[fov_index].is_visible=true;
                        //m_fov_field[fov_index].east=true;
                        //m_fov_field[fov_index].south=true;
                        continue;
                    }
                }
            }
        }
    }
#endif

    Log->puts("FOV calculated\n");
}
