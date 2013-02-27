#ifndef FOV_H_INCLUDED
#define FOV_H_INCLUDED

#include "../myogl/vector_types.h"
#include "../myogl/log.h"

class CLevelMap;

struct sMapFovField{
    bool is_visible: 1;
    bool north: 1;
    bool south: 1;
    bool east: 1;
    bool west: 1;
    bool north_east: 1;
    bool north_west: 1;
    bool south_east: 1;
    bool south_west: 1;
    unsigned char distance;
};

class CFOV{

        sMapFovField *m_fov_field;
        int m_distance, m_fov_size;
        MyOGL::Vector2i m_left_top, m_right_bottom;    // last calculated FOV position in map coords

    public:
        CFOV(int distance){
            m_distance=distance;
            m_fov_size=m_distance*2+1;  // width & height
            MyOGL::Log->puts("Create FOV array for distance: %d.  array size: %d tiles\n", m_distance, m_fov_size*m_fov_size);
            m_fov_field=new sMapFovField[m_fov_size*m_fov_size];
            m_left_top.x=0;m_left_top.y=0;
            m_right_bottom.x=0;m_right_bottom.y=0;
        };

        ~CFOV(){
            MyOGL::Log->puts("Free FOV array\n");
            delete m_fov_field;
        };

        void Resize(int distance){
            m_distance=distance;
            delete m_fov_field;
            m_fov_size=m_distance*2+1;  // width & height
            MyOGL::Log->puts("Resize FOV array for %d tiles\n", m_fov_size*m_fov_size);
            m_fov_field=new sMapFovField[m_fov_size*m_fov_size];
        };

        // clean previous FOV data
        void ClearOldData(){
            int index;
            for(int y=0; y<m_fov_size; y++){
                for(int x=0; x<m_fov_size; x++){
                    index=x+y*m_fov_size;
                    m_fov_field[index].distance=0;
                    m_fov_field[index].is_visible=false;
                    m_fov_field[index].north=false;
                    m_fov_field[index].south=false;
                    m_fov_field[index].east=false;
                    m_fov_field[index].west=false;
                    m_fov_field[index].north_east=false;
                    m_fov_field[index].north_west=false;
                    m_fov_field[index].south_east=false;
                    m_fov_field[index].south_west=false;
                };
            };
        };
        // set tile (tile_x, tile_y) distance to x_pos, y_pos (if set_distance>0)
        void SetDistance(int tile_x, int tile_y, int x_pos, int y_pos, int set_distance);

        void Calculate(int x, int y, CLevelMap *map, int set_light_distane=0);
        void ApplyOnMap(CLevelMap *map);    // apply vieved field on map array

        int GetDistance(int x, int y){
            if(x < m_left_top.x || x > m_right_bottom.x || y < m_left_top.y || y > m_right_bottom.y){

                MyOGL::Log->puts("ERROR (CFOV.GetDistance): get wrong x,y (%d,%d)",x,y);
                MyOGL::Log->puts(" left-top: %d %d ",m_left_top.x, m_left_top.y);
                                MyOGL::Log->puts(" right-bottom: %d %d\n",m_right_bottom.x, m_right_bottom.y);

                return 0;
            }
            return m_fov_field[x-m_left_top.x+(y-m_left_top.y)*m_fov_size].distance;
        };
        void SetDistance(int x, int y, int distance){
            if(x < m_left_top.x || x > m_right_bottom.x || y < m_left_top.y || y > m_right_bottom.y){
                MyOGL::Log->puts("ERROR (CFOV.SetDistance): get wrong x,y (%d,%d)\n",x,y);
                return;
            }
            m_fov_field[x-m_left_top.x+(y-m_left_top.y)*m_fov_size].distance=distance;
        };
        MyOGL::Vector2i GetLeftTop(){ return m_left_top; }
        MyOGL::Vector2i GetRightBottom(){ return m_right_bottom; }
        bool IsInArea(int x, int y){
            return x >= m_left_top.x && x <=m_right_bottom.x && y >= m_left_top.y && y<= m_right_bottom.y;
        }
        bool IsVisible(int x, int y){
            if(IsInArea(x, y)){
                return m_fov_field[x-m_left_top.x+(y-m_left_top.y)*m_fov_size].is_visible;
            }
            MyOGL::Log->puts("ERROR (IsVisible): get wrong x,y (%d,%d)\n",x,y);
            return false;
        };
        // temporary full data array access
        sMapFovField *GetFovField(int x, int y){
            if(x < m_left_top.x || x > m_right_bottom.x || y < m_left_top.y || y > m_right_bottom.y){
                MyOGL::Log->puts("ERROR (CFOV.GetFovField): get wrong x,y for point (%d,%d)\n",x,y);
                return NULL;
            }
            return &m_fov_field[x-m_left_top.x+(y-m_left_top.y)*m_fov_size];
        }

        int get_size(){ return m_fov_size; }
};


#endif // FOV_H_INCLUDED
