#ifndef DUNGEON_MAP_H_INCLUDED
#define DUNGEON_MAP_H_INCLUDED

#include "tileset.h"
#include "myogl/vector_types.h"

#include "map/level_map.h"
#include "map/grid_dungeon.h"



// Dungeon level class
class CDungeonLevel{
        //TODO add dyn tiles list for use in method "Update"
        //std::vector <CMapDynamicTile> DynamicTilesLIst
        unsigned int m_width, m_height;

        CLevelMap *m_Map;
        CTileset *m_tileset;

        unsigned int m_global_light;
        bool m_light_changed;
        Vector4ui m_ViewPort;
        //bool **LOS;
        bool LineOfSight(int x1, int y1, int x2, int y2);

    public:

        CDungeonLevel(unsigned int width, unsigned int height){
        //, unsigned int grid_width, unsigned int grid_height, unsigned int rooms_num){

            m_height=height; m_width=width;

            m_Map=new CLevelMap(width, height);

            m_ViewPort.left=0;
            m_ViewPort.top=0;
            m_ViewPort.width=18;
            m_ViewPort.height=18;
            m_global_light=0;
            m_light_changed=true;
            // create LOS array
//            LOS=new bool *[m_ViewPort.width];
//            for(i=0;i<m_ViewPort.width;i++){
//                LOS[i]=new bool[m_ViewPort.height];
//            }
        };

        ~CDungeonLevel(){
            Log->puts("Delete dungeon obj\n");

            delete m_Map;
//            for(unsigned int i=0;i<m_ViewPort.width;i++) delete LOS[i];
//            delete LOS;
        };
        void Update(double DeltaTime);  // update map animated tiles (TODO: lights and etc.)
        void SetViewportPosition(unsigned int x, unsigned int y){ m_ViewPort.left=x; m_ViewPort.top=y; m_light_changed=true;};
        void SetViewportToTarget(unsigned int x, unsigned int y){
            int new_x=(int)(x-m_ViewPort.width/2);
            int new_y=(int)(y-m_ViewPort.height/2);
            if(new_x<0){ new_x=0; }
            if(new_y<0){ new_y=0; }
            if(new_x>(int)(m_width-m_ViewPort.width)){ new_x=m_width-m_ViewPort.width;}
            if(new_y>(int)(m_height-m_ViewPort.height)){ new_y=m_height-m_ViewPort.height;}
            SetViewportPosition(new_x, new_y);
        }
        void SetViewportSize(unsigned int width, unsigned int height){
            m_ViewPort.width=width; m_ViewPort.height=height; m_light_changed=true;
        };
        unsigned int GetViewportLeft(){ return m_ViewPort.left; };
        unsigned int GetViewportTop(){ return m_ViewPort.top; };
        unsigned int GetViewportWidth(){ return m_ViewPort.width; };
        unsigned int GetViewportHeight(){ return m_ViewPort.height; };
        void ChangeViewportPosition(int dx, int dy){
            if(((int)m_ViewPort.left+dx)<=0){
                m_ViewPort.left=0;
            }else if ((m_ViewPort.left+dx+m_ViewPort.width)>m_width){
                m_ViewPort.left=m_width-m_ViewPort.width;
            }else{
                m_ViewPort.left+=dx;
            }

            if(((int)m_ViewPort.top+dy)<0){
                m_ViewPort.top=0;
            }else if ((m_ViewPort.top+dy+m_ViewPort.height)>m_height){
                m_ViewPort.top=m_height-m_ViewPort.height;
            }else{
                m_ViewPort.top+=dy;
            }
            m_light_changed=true;
        }
        void SetTileset(CTileset *tileset){ m_tileset=tileset;};
        void NewGridDungeon(int grid_w, int grid_h, int rooms);
        int GetRoomIdByCoords(unsigned int x, unsigned int y);
        void CalculateLight();
        void SetGlobalLight(unsigned int light_index){ m_global_light=light_index;m_light_changed=true;};
        unsigned int GetGlobalLight(void){ return m_global_light;};
        void Render(void);
        void CalculateLOS(int x, int y, int distance=0);
        unsigned int GetWidth(){ return m_width;};
        unsigned int GetHeight(){ return m_height;};

        sMapField **Map(){
            return m_Map->GetMap();
        };

        MyOGL::Vector2i GetStartPosition(){ return m_Map->GetStartPosition(); };

};

#endif // DUNGEON_MAP_H_INCLUDED
