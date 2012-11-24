#ifndef DUNGEON_MAP_H_INCLUDED
#define DUNGEON_MAP_H_INCLUDED

#include "tileset.h"
#include "myogl/vector_types.h"
#include "myogl/animation.h"

// tiles data
typedef struct{
    unsigned char strength;
    bool broken;
    bool opened;
    bool locked;
    bool hidden;
}sTileDataDoor;

typedef struct{
    eTileTypes tile_type;
    unsigned char layer[3];
    unsigned char light;
    bool viewed;
    bool visible;
    bool skip_light;
    bool can_move;  // TODO: move_speed, if 0 - cannot move
    void *p_tile_data;
    void *p_monsters;
    void *p_items;
}sMapField;

typedef struct{
    bool filled;
    unsigned int room_id;
} grid_field;

typedef struct{
    unsigned int left,top,width,height; // room position & size
    bool connected; // connected to first room
} sroom;

class CMapDynamicTile{
        CAnimation *m_animation;
    //CTexture  *m_texture;
    //        CMaterial *m_material;
        std::vector<unsigned char> TileNamesList;
 //       unsigned char layer; // graphics layer for change sprite
    public:
        CMapDynamicTile(){
            m_animation=new CAnimation;
        };
        ~CMapDynamicTile(){
            delete m_animation;
            TileNamesList.clear();
        };
        void AddTiles(unsigned char start_sprite_id, unsigned char sprites_count=1){
            for(unsigned char i=0; i < sprites_count; i++){
                TileNamesList.push_back(start_sprite_id+i);
            }
            m_animation->FramesCount=TileNamesList.size();
        }
        void Update(double DeltaTime){
            m_animation->OnAnimate(DeltaTime);
        };
        unsigned char GetCurrentTile(){ return TileNamesList[m_animation->GetCurrentFrame()];};
        void SetAnimationOscillate(bool Oscillate){ m_animation->Oscillate=Oscillate; }
        void SetAnimationRate(double Rate){ m_animation->SetFrameRate(Rate); };
};

typedef struct{
    Vector2ui position;
    unsigned char strength;
    CMapDynamicTile *dynamic_tile;
} sMapLightSource;

// Dungeon level class
class CDungeonLevel{
        //TODO add dyn tiles list for use in method "Update"
        //std::vector <CMapDynamicTile> DynamicTilesLIst
        unsigned int m_width, m_height;
        sMapField **m_Map;
        unsigned int m_grid_w, m_grid_h;
        grid_field **m_grid;
        unsigned int m_rooms_num;
        sroom *m_rooms;
        unsigned int m_room_min_w, m_room_min_h;
        CTileset *m_tileset;
        std::vector <sMapLightSource> LightSourcesList;
        unsigned int m_global_light;
        bool m_light_changed;
        Vector4ui m_ViewPort;
        //bool **LOS;
        bool LineOfSight(int x1, int y1, int x2, int y2);
        void CreateRoom(sroom *sroom, unsigned int grid_x, unsigned int grid_y);
        void RoomToMap(unsigned int room_id);
        void CreateCoridor(unsigned int start_room_id, unsigned int end_room_id);
        void AddDoors(unsigned int room_id);
        void AddMapTile(eTileTypes TileType, unsigned int x, unsigned int y);
    public:

        CDungeonLevel(unsigned int width, unsigned int height, unsigned int grid_width, unsigned int grid_height, unsigned int rooms_num){
            unsigned int i;
            m_height=height; m_width=width;
            m_Map = new sMapField *[m_width];
            for (i = 0; i < m_width; i++){
                m_Map[i] = new sMapField[m_height];
            }
            m_grid_w=grid_width; m_grid_h=grid_height;
            m_grid=new grid_field *[m_grid_w];
            for(i=0;i<m_grid_w;i++){
                m_grid[i] = new grid_field [m_grid_h];
            }
            m_rooms_num=rooms_num;
            m_rooms = new sroom [m_rooms_num];
            m_room_min_w=3;
            m_room_min_h=3;
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
            for (unsigned int i = 0; i < m_width; i++){
                for(unsigned int j=0; j < m_height; j++){
                    if(m_Map[i][j].p_tile_data!=NULL){
                        switch(m_Map[i][j].tile_type){
                            case ttDoor:
                                delete ((sTileDataDoor *)m_Map[i][j].p_tile_data);
                                break;
                            default:
                                printf("WARNING! not implementd delete *p_tile_data for tile_type %d!\n",m_Map[i][j].tile_type);
                                break;
                        }
                    }
                }
                delete m_Map[i];
            }
            delete m_Map;
            for(unsigned int i=0;i<m_grid_w;i++) delete m_grid[i];
            delete m_grid;
            delete m_rooms;
            for(unsigned int i=0;i<LightSourcesList.size();i++){
                delete LightSourcesList[i].dynamic_tile;
            }
            LightSourcesList.clear();
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
        void NewLevel(void);
        int GetRoomIdByCoords(unsigned int x, unsigned int y);
        void CalculateLight();
        void SetGlobalLight(unsigned int light_index){ m_global_light=light_index;m_light_changed=true;};
        unsigned int GetGlobalLight(void){ return m_global_light;};
        void Render(void);
        void AddLightSource(unsigned int x, unsigned int y, unsigned char strength);
        void CalculateLOS(int x, int y, int distance=0);
        unsigned int GetWidth(){ return m_width;};
        unsigned int GetHeight(){ return m_height;};
        sMapField **Map(){
            return m_Map;
        }

        sroom GetRoom(unsigned int room_id){ return m_rooms[room_id]; }
};

#endif // DUNGEON_MAP_H_INCLUDED
