#include "cell_info.h"
#include "../game.h" // for dungeon global class

void CUICellInfo::Render(){
            char tmp[100];
            MyOGL::Vector4f font_color;
            // get map field
            m_field=dungeon->GetMapFiled(m_map_tile_x, m_map_tile_y);
            // set panel width and height
            if(!m_field.viewed){
                SetDimension(200, 35); // for 2 strings
            }else{
                SetDimension(280, 180); // for full information
            }

            // display base class (panel border and background)
            CUIPanel::Render();
            // display field information
            font_color.r=0; font_color.g=1,font_color.b=0, font_color.a=1;
            sprintf(tmp,"Координаты: ^ffffff%d, %d", m_map_tile_x, m_map_tile_y);
            m_font->PrintAt(m_left_top.x+2,m_left_top.y+2,tmp,font_color);
            if(!m_field.viewed){ // unknown field
                sprintf(tmp,"Местность: ^ffffffНеизвестная");
                m_font->PrintAt(m_left_top.x+2,m_left_top.y+14,tmp,font_color);
            }else{
                sprintf(tmp,"Местность: ^ffffff%s",GetTileNameByType(m_field.tile_type));
                m_font->PrintAt(m_left_top.x+2,m_left_top.y+14,tmp,font_color);
                CFOV *fov=dungeon->GetFOV();
                sMapFovField *fov_field=fov->GetFovField(m_map_tile_x, m_map_tile_y);
                if(fov_field){
                    sprintf(tmp,"Интенсивность света: ^ffffff%d",fov_field->distance);
                    m_font->PrintAt(m_left_top.x+2, m_left_top.y+14+14,tmp,font_color);
                    m_font->PrintAt(m_left_top.x+2, m_left_top.y+14+14*2,"Вектор взгляда:",font_color);
                    font_color.Set(0,1,1,1);
                    sprintf(tmp,"north: ^ffffff%d",fov_field->north);
                    m_font->PrintAt(m_left_top.x+22, m_left_top.y+14+14*3,tmp,font_color);
                    sprintf(tmp,"south: ^ffffff%d",fov_field->south);
                    m_font->PrintAt(m_left_top.x+22, m_left_top.y+14+14*4,tmp,font_color);
                    sprintf(tmp,"east: ^ffffff%d",fov_field->east);
                    m_font->PrintAt(m_left_top.x+22, m_left_top.y+14+14*5,tmp,font_color);
                    sprintf(tmp,"west: ^ffffff%d",fov_field->west);
                    m_font->PrintAt(m_left_top.x+22, m_left_top.y+14+14*6,tmp,font_color);
                    sprintf(tmp,"north-east: ^ffffff%d",fov_field->north_east);
                    m_font->PrintAt(m_left_top.x+22, m_left_top.y+14+14*7,tmp,font_color);
                    sprintf(tmp,"north-west: ^ffffff%d",fov_field->north_west);
                    m_font->PrintAt(m_left_top.x+22, m_left_top.y+14+14*8,tmp,font_color);
                    sprintf(tmp,"south-east: ^ffffff%d",fov_field->south_east);
                    m_font->PrintAt(m_left_top.x+22, m_left_top.y+14+14*9,tmp,font_color);
                    sprintf(tmp,"south-west: ^ffffff%d",fov_field->south_west);
                    m_font->PrintAt(m_left_top.x+22, m_left_top.y+14+14*10,tmp,font_color);
                }else{
                    m_font->PrintAt(m_left_top.x+2, m_left_top.y+14+14,"FOV field: ^ffffffNULL",font_color);
                }
            }
        };
