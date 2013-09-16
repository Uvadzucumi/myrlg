#ifndef DUNGEON_MAP_H_INCLUDED
#define DUNGEON_MAP_H_INCLUDED

#include "tileset.h"
#include "myogl/vector_types.h"

#include "map/level_map.h"
#include "map/minimap.h"
#include "map/grid_dungeon.h"

// Dungeon level class
// mapmanager class
class CDungeonLevel{
        //TODO add dyn tiles list for use in method "Update"
        //std::vector <CMapDynamicTile> DynamicTilesLIst
        int m_map_width, m_map_height;


        CLevelMap *m_Map;
        CDungeonMinimap *m_Minimap;
        CTileset *m_tileset;
        CFOV *m_fov;

        unsigned int m_global_light;
        bool m_light_changed;
        bool m_diffuse_light;
        Vector4i m_ViewPort;
        bool m_see_only_with_light;
    public:

        CDungeonLevel(int width, int height){

            m_see_only_with_light=false;
            m_diffuse_light=false;
            m_map_height=height; m_map_width=width;

            m_Map=new CLevelMap(m_map_width, m_map_height);

            m_ViewPort.left=0;
            m_ViewPort.top=0;
            m_ViewPort.width=18;
            m_ViewPort.height=18;
            m_global_light=0;
            m_light_changed=true;
            m_fov=new CFOV(18/2);

            //m_Minimap=NULL;
            m_Minimap=new CDungeonMinimap(m_map_width, m_map_height);
            m_Minimap->SetPosition(32*m_ViewPort.width,0);
            // create LOS array
//            LOS=new bool *[m_ViewPort.width];
//            for(i=0;i<m_ViewPort.width;i++){
//                LOS[i]=new bool[m_ViewPort.height];
//            }
            //m_light_vp=new sMapFovField[m_ViewPort.width*m_ViewPort.height];
        };

        ~CDungeonLevel(){
            Log->puts("Delete dungeon obj\n");
//            delete m_light_vp; // old fov
            delete m_fov;
            delete m_Map;
            delete m_Minimap;
//            for(unsigned int i=0;i<m_ViewPort.width;i++) delete LOS[i];
//            delete LOS;
        };
        void SetSeeOnlyWithLight(bool light_see){ m_see_only_with_light=light_see; }
        void SetDiffuseLight(bool diffuse_light){ m_diffuse_light=diffuse_light; }

        void Update(double DeltaTime);  // update map animated tiles (TODO: lights and etc.)
        void SetViewportPosition(unsigned int x, unsigned int y){ m_ViewPort.left=x; m_ViewPort.top=y; m_light_changed=true;};
        void SetViewportToTarget(unsigned int x, unsigned int y){
            int new_x=(int)(x-m_ViewPort.width/2);
            int new_y=(int)(y-m_ViewPort.height/2);
            if(new_x<0){ new_x=0; }
            if(new_y<0){ new_y=0; }
            if(new_x>(int)(m_map_width-m_ViewPort.width)){ new_x=m_map_width-m_ViewPort.width;}
            if(new_y>(int)(m_map_height-m_ViewPort.height)){ new_y=m_map_height-m_ViewPort.height;}
            SetViewportPosition(new_x, new_y);
        }
        void SetViewportSize(unsigned int width, unsigned int height){
            m_ViewPort.width=width; m_ViewPort.height=height; m_light_changed=true;
            m_Minimap->SetPosition(32*m_ViewPort.width,0);
            // recreate light intensivity array
//            delete m_light_vp;
//            m_light_vp=new sMapFovField[m_ViewPort.width * m_ViewPort.height];
            if(width>height){
                m_fov->Resize(width/2);
            }else{
                m_fov->Resize(height/2);
            }
        };
        unsigned int GetViewportLeft(){ return m_ViewPort.left; };
        unsigned int GetViewportTop(){ return m_ViewPort.top; };
        unsigned int GetViewportWidth(){ return m_ViewPort.width; };
        unsigned int GetViewportHeight(){ return m_ViewPort.height; };
        void ChangeViewportPosition(int dx, int dy){
            if(((int)m_ViewPort.left+dx)<=0){
                m_ViewPort.left=0;
            }else if ((m_ViewPort.left+dx+m_ViewPort.width)>m_map_width){
                m_ViewPort.left=m_map_width-m_ViewPort.width;
            }else{
                m_ViewPort.left+=dx;
            }

            if(((int)m_ViewPort.top+dy)<0){
                m_ViewPort.top=0;
            }else if ((m_ViewPort.top+dy+m_ViewPort.height)>m_map_height){
                m_ViewPort.top=m_map_height-m_ViewPort.height;
            }else{
                m_ViewPort.top+=dy;
            }
            m_light_changed=true;
        }
        void SetTileset(CTileset *tileset){ m_tileset=tileset;};
        void NewGridDungeon(int grid_w, int grid_h, int rooms);
        //int GetRoomIdByCoords(unsigned int x, unsigned int y);
        void SetGlobalLight(unsigned int light_index){ m_global_light=light_index;m_light_changed=true;};
        unsigned int GetGlobalLight(void){ return m_global_light;};
        void Render();
        void CalculateFOV(int x, int y, int distance=0);
        unsigned int GetWidth(){ return m_map_width;};
        unsigned int GetHeight(){ return m_map_height;};

        bool IsCanMove(int x, int y){ return m_Map->IsCanMove(x, y);};
        bool IsDoor(int x, int y){ return m_Map->IsDoor(x, y); };
        bool IsDoorClosed(int x, int y){ return m_Map->IsDoorClosed(x, y); };
        bool OpenDoor(int x, int y){ return m_Map->OpenDoor(x,y); };
        bool CloseDoor(int x, int y){ return m_Map->CloseDoor(x,y); };
        void SearchItemsIn(int x, int y){
            printf("in field %d items\n",m_Map->GetItemsInField(x,y)->size());
        };

        MyOGL::Vector2i GetStartPosition(){ return m_Map->GetStartPosition(); };

        CFOV* GetFOV(){ return m_fov; } // return for object
        sMapField GetMapFiled(int x, int y){
            return m_Map->GetMapFiled(x,y);
        }

        void DebugMapLight(){
            // debug visible field
            for(int dy=m_ViewPort.top; dy<m_ViewPort.top+m_ViewPort.height; dy++){
                for(int dx=m_ViewPort.left;dx<m_ViewPort.left+m_ViewPort.width;dx++){
                    //printf("%d ",m_light_vp[dx+dy*m_ViewPort.width].is_visible);
                    printf("%d ",m_fov->IsVisible(dx,dy));
                }
                printf("\n");
            }
        }
};

#endif // DUNGEON_MAP_H_INCLUDED
