#ifndef LEVEL_MAP_H_INCLUDED
#define LEVEL_MAP_H_INCLUDED

#include "../myogl/vector_types.h"
#include "../myogl/log.h"
#include "../myogl/animation.h"

//#include <stdlib.h> // NULL, rand
#include "../tileset.h" // tile names - for addTile

#include "../items.h"

#include <math.h> // sqrt

// tile types
enum eTileTypes{
    ttWall=0,
    ttCoridor,
    ttRoom,
    ttDoor,/*
    ttDoorClosed, // closed door
    ttDoorOpened,
    ttDoorBroken,
    ttDoorHidden,
    ttDoorLocked,*/
    ttContainer,
    ttStairs,
    ttFire
};

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
    short layer[3];
    //short light;
    bool viewed;
    //bool visible;
    bool skip_light;
    bool can_move;  // TODO: move_speed, if 0 - cannot move
    void *p_tile_data;
    void *p_monsters;
    //void *p_items;
    CItemsContainer items;
}sMapField;

struct sMapFovField{
    bool is_visible: 1;
    bool north: 1;
    bool south: 1;
    bool east: 1;
    bool west: 1;
    unsigned char light;
};

class CMapDynamicTile{
        CAnimation *m_animation;
        std::vector<unsigned char> TileNamesList;
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
        short GetCurrentTile(){ return TileNamesList[m_animation->GetCurrentFrame()]; };
        int GetCurrentFrame(){ return m_animation->GetCurrentFrame(); };
        void SetAnimationOscillate(bool Oscillate){ m_animation->Oscillate=Oscillate; }
        void SetAnimationRate(double Rate){ m_animation->SetFrameRate(Rate); };
};

typedef struct{
    Vector2i position;
    //float diffuse;
    int strength;
    CMapDynamicTile *dynamic_tile;
    int GetIntnsivity(int x, int y){    // return light intensivity in coords x,y
        // calculate distance
        int distance = sqrt(((x - position.x) * (x - position.x)) + ((y - position.y) * (y - position.y)));
        //int ret=strength-distance-1+dynamic_tile->GetCurrentFrame()%2;
        //int ret=strength-distance-dynamic_tile->GetCurrentFrame();
        int ret=strength-distance;
        if(ret<=0){
            return 0;
        }else{
            return ret;
        }
    };
} sMapLightSource;

class CLevelMap{
        // map data
        int m_width, m_height;
        sMapField **m_Map;
        std::vector <sMapLightSource> LightSourcesList;
        // map player position (useing for )
        MyOGL::Vector2i m_UnitPosition;
    public:
        CLevelMap(int Width, int Height){

            m_width=Width; m_height=Height;

            m_Map = new sMapField *[m_width];
            for (int i = 0; i < m_width; i++){
                m_Map[i] = new sMapField[m_height];
                for(int j=0; j < m_height; j++){
                    m_Map[i][j].p_tile_data=NULL;
                    m_Map[i][j].p_monsters=NULL;
//                    m_Map[i][j].p_items=NULL;
                }
            }
        };

        ~CLevelMap(){
            // clear map data
            for (int i = 0; i < m_width; i++){
                for(int j=0; j < m_height; j++){
                    if(m_Map[i][j].p_tile_data!=NULL){
                        switch(m_Map[i][j].tile_type){
                            case ttDoor:
                                if(m_Map[i][j].p_tile_data){
                                    delete ((sTileDataDoor *)m_Map[i][j].p_tile_data);
                                }
                                break;
                            default:
                                MyOGL::Log->puts("WARNING! not implementd delete *p_tile_data for tile_type %d!\n",m_Map[i][j].tile_type);
                                break;
                        }
                    }
                }
                delete m_Map[i];
            }
            // clear lights sources
            for(unsigned int i=0;i<LightSourcesList.size();i++){
                delete LightSourcesList[i].dynamic_tile;
            }
            LightSourcesList.clear();

        };

        bool LineOfSight(int x1, int y1, int x2, int y2); // line on sight between x1,y1 and x2,y2
        void AddMapTile(eTileTypes TileType, int x, int y, void *tile_data=NULL);
        void AddLightSource(unsigned int x, unsigned int y, unsigned char strength);
        void Update(double DeltaTime);
        int GetWidth(){ return m_width; }
        int GetHeight() { return m_height; }

        //sMapField **GetMap(){ return m_Map; };
        //bool IsVisible(int x, int y){ return m_Map[x][y].visible; };
        bool IsSkipLight(int x, int y){ return m_Map[x][y].skip_light; };
        bool IsViewed(int x, int y){ return m_Map[x][y].viewed; };
        bool IsCanMove(int x, int y){ return m_Map[x][y].can_move; }; // TODO: add check big items, mobs - etc?
        bool IsDoor(int x, int y);
        bool IsDoorClosed(int x, int y);
        bool OpenDoor(int x, int y);
        bool CloseDoor(int x, int y);
        void SetViewed(int x, int y, bool flag){
            if(x<0 || y< 0 || x>=m_width || y>=m_height){
                MyOGL::Log->puts("RANGE ERROR (SetViewed): x: %d y: %d\n",x,y);
                return;
            }
             m_Map[x][y].viewed=flag;
        }
        CItemsContainer* GetItemsInField(int x, int y){ return &m_Map[x][y].items; };   // return pointer to items list in field x, y

        MyOGL::Vector2i GetStartPosition() { return m_UnitPosition; };
        void SetStartPosition(int x, int y){ m_UnitPosition.x=x; m_UnitPosition.y=y; };
        eTileTypes GetTileType(int x, int y){ return m_Map[x][y].tile_type;}
        int GetTileID(int x, int y, int layer){
            return m_Map[x][y].layer[layer];
        }
        // calculate light in map part, light_array - result array, left,top, width, height - map part coords
        void CalculateMapLight(sMapFovField *light_array, int left, int top, int width, int height);
};

#endif // LEVEL_MAP_H_INCLUDED
