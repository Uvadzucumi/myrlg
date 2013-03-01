
#include "light_source.h"

#include "level_map.h"

void CMapLightSource::ApplyToFOV(CFOV *fov, CLevelMap *map){
    MyOGL::Vector2i left_top, right_bottom;   // map coords
    MyOGL::Vector2i fov_lt, fov_rb, light_lt, light_rb;

    sMapFovField *fov_fld, *light_fld;
    int new_light;
// get fov and light areas
    fov_lt=fov->GetLeftTop();
    fov_rb=fov->GetRightBottom();
    light_lt=m_light_fov->GetLeftTop();
    light_rb=m_light_fov->GetRightBottom();
// get intersection
        if(light_lt.x < fov_lt.x){ left_top.x=fov_lt.x;  }else{ left_top.x=light_lt.x; }
        if(light_lt.y < fov_lt.y){ left_top.y=fov_lt.y;  }else{ left_top.y=light_lt.y; }
        if(light_rb.x > fov_rb.x){ right_bottom.x=fov_rb.x;  }else{ right_bottom.x=light_rb.x; }
        if(light_rb.y > fov_rb.y){ right_bottom.y=fov_rb.y;  }else{ right_bottom.y=light_rb.y; }
// Apply light intensivity in intersect area
        for(int dx=left_top.x; dx<=right_bottom.x; dx++){
            for(int dy=left_top.y; dy<=right_bottom.y; dy++){
                // only visible fields & if have this light intensivity
                new_light=m_light_fov->GetDistance(dx,dy);
                if(fov->IsVisible(dx,dy) && m_light_fov->GetDistance(dx,dy)){
                    // compare light & view vectors

                    fov_fld=fov->GetFovField(dx,dy);
                    light_fld=m_light_fov->GetFovField(dx,dy);
                    if(  map->IsSkipLight(dx,dy) ||
                        (fov_fld->north && light_fld->north) ||
                        (fov_fld->south && light_fld->south) ||
                        (fov_fld->east && light_fld->east) ||
                        (fov_fld->west && light_fld->west) ||
                        (fov_fld->north_east && light_fld->north_east) ||
                        (fov_fld->north_west && light_fld->north_west) ||
                        (fov_fld->south_east && light_fld->south_east) ||
                        (fov_fld->south_west && light_fld->south_west)){
                       /*
                        (fov_fld->north && m_position.y<dy) ||
                        (fov_fld->south && m_position.y>dy) ||
                        (fov_fld->east && m_position.x>dx) ||
                        (fov_fld->west && m_position.x<dx) ||
                        (fov_fld->north_east && m_position.y<dy && m_position.x>dx) ||
                        (fov_fld->north_west && m_position.y<dy && m_position.x<dx) ||
                        (fov_fld->south_east && m_position.y>dy && m_position.x>dx) ||
                        (fov_fld->south_west && m_position.y>dy && m_position.x<dx)){
                        */
                        // compared, add light if needed
                            if(fov->GetDistance(dx,dy) < new_light){
                               fov->SetDistance(dx,dy,new_light);
                            }
                        }
                }
            } // dy
        } // dx
    };
