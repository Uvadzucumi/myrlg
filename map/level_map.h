#ifndef LEVEL_MAP_H_INCLUDED
#define LEVEL_MAP_H_INCLUDED

#include "../myogl/vector_types.h"
#include "../myogl/log.h"
#include "../myogl/animation.h"

//#include <stdlib.h> // NULL, rand
#include "../tileset.h" // tile names - for addTile

#include "../items.h"

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
    unsigned char layer[3];
    unsigned char light;
    bool viewed;
    bool visible;
    bool skip_light;
    bool can_move;  // TODO: move_speed, if 0 - cannot move
    void *p_tile_data;
    void *p_monsters;
    //void *p_items;
    CItemsContainer items;
}sMapField;

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
        unsigned char GetCurrentTile(){ return TileNamesList[m_animation->GetCurrentFrame()];};
        void SetAnimationOscillate(bool Oscillate){ m_animation->Oscillate=Oscillate; }
        void SetAnimationRate(double Rate){ m_animation->SetFrameRate(Rate); };
};

typedef struct{
    Vector2i position;
    unsigned char strength;
    CMapDynamicTile *dynamic_tile;
} sMapLightSource;

class CLevelMap{
        // map data
        int m_width, m_height;
        sMapField **m_Map;
        std::vector <sMapLightSource> LightSourcesList;
        // map start position
        MyOGL::Vector2i m_StartPosition;
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

        void AddMapTile(eTileTypes TileType, int x, int y, void *tile_data=NULL);
        void AddLightSource(unsigned int x, unsigned int y, unsigned char strength);
        void Update(double DeltaTime);
        int GetWidth(){ return m_width; }
        int GetHeight() { return m_height; }

        sMapField **GetMap(){ return m_Map; };
        bool IsVisible(int x, int y){ return m_Map[x][y].visible; };
        bool IsSkipLight(int x, int y){ return m_Map[x][y].skip_light; };
        bool IsViewed(int x, int y){ return m_Map[x][y].viewed; };
        bool IsCanMove(int x, int y){ return m_Map[x][y].can_move; }; // TODO: add check big items, mobs - etc?
        bool IsDoor(int x, int y);
        bool IsDoorClosed(int x, int y);
        bool OpenDoor(int x, int y);
        bool CloseDoor(int x, int y);
        CItemsContainer* GetItemsInField(int x, int y){ return &m_Map[x][y].items; };   // return pointer to items list in field x, y

        MyOGL::Vector2i GetStartPosition() { return m_StartPosition; };
        void SetStartPosition(int x, int y){ m_StartPosition.x=x; m_StartPosition.y=y; };
        eTileTypes GetTileType(int x, int y){ return m_Map[x][y].tile_type;}


};

#endif // LEVEL_MAP_H_INCLUDED
